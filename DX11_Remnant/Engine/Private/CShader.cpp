#include "CShader.h"

#include "CSceneComponent.h"
#include "CTexture.h"

CShader::CShader(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CShader::CShader(const CShader& _Prototype)
	: m_pDevice{ _Prototype.m_pDevice }
	, m_pContext{ _Prototype.m_pContext }
	, m_pEffect{ _Prototype.m_pEffect }
	, m_vecInputLayout{ _Prototype.m_vecInputLayout }
	, m_iNumPass{ _Prototype.m_iNumPass }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	Safe_AddRef(m_pEffect);

	for (auto& InputLayout : m_vecInputLayout)
		Safe_AddRef(InputLayout);
}

HRESULT CShader::Init
(
	const _tchar* _pShaderFilePath, 
	const D3D11_INPUT_ELEMENT_DESC* _pVertexElements, 
	_uint _iNumElement
)
{
	_uint iHlslFlag = {};

#ifdef _DEBUG

	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#else

	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;

#endif

	ID3DBlob* pErrMsg{};

	if (FAILED(D3DX11CompileEffectFromFile
	(
		_pShaderFilePath, 
		nullptr, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		iHlslFlag, 0, 
		m_pDevice, 
		&m_pEffect,
		&pErrMsg
	)))
		return E_FAIL;

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (pTechnique == nullptr)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC tTechniqueDesc{};
	pTechnique->GetDesc(&tTechniqueDesc);

	m_iNumPass = tTechniqueDesc.Passes;

	for (size_t i = 0; i < m_iNumPass; i++)
	{
		ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(static_cast<_uint>(i));
		ID3D11InputLayout* pInputLayout{};
		D3DX11_PASS_DESC tPassDesc{};

		pPass->GetDesc(&tPassDesc);

		if (FAILED(m_pDevice->CreateInputLayout
		(
			_pVertexElements,
			_iNumElement, 
			tPassDesc.pIAInputSignature,
			tPassDesc.IAInputSignatureSize,
			&pInputLayout
		)))
			return E_FAIL;

		m_vecInputLayout.push_back(pInputLayout);
	}

	return S_OK;
}

HRESULT CShader::Init(void* _pDesc)
{
	return S_OK;
}

HRESULT CShader::Begin(_uint _iPassIndex)
{
	if (m_iNumPass <= _iPassIndex)
		return E_FAIL;

	m_pContext->IASetInputLayout(m_vecInputLayout[_iPassIndex]);

	m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(_iPassIndex)->Apply(0, m_pContext);

	return S_OK;
}

HRESULT CShader::Bind_RawValue(const _char* _pConstantName, const void* _pData, _uint _iSize)
{
	if (m_pEffect == nullptr)
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (pVariable == nullptr)
		return E_FAIL;

	return pVariable->SetRawValue(_pData, 0, _iSize);
}

HRESULT CShader::Bind_Matrix(const _char* _pConstantName, const _float4x4* _pMatrix)
{
	if (m_pEffect == nullptr)
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (pVariable == nullptr)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (pMatrixVariable == nullptr)
		return E_FAIL;

	return pMatrixVariable->SetMatrix(reinterpret_cast<const _float*>(_pMatrix));
}

HRESULT CShader::Bind_Matrix(const _char* _pConstantName, CSceneComponent* _pSceneComponent)
{
	if (m_pEffect == nullptr)
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (pVariable == nullptr)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (pMatrixVariable == nullptr)
		return E_FAIL;

	return pMatrixVariable->SetMatrix(reinterpret_cast<const _float*>(_pSceneComponent->Get_WorldMatrix_Ptr()));
}

HRESULT CShader::Bind_Matrices(const _char* _pConstantName, const _float4x4* _pMatrix, _uint _iNumMatrix)
{
	if (m_pEffect == nullptr)
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (pVariable == nullptr)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (pMatrixVariable == nullptr)
		return E_FAIL;

	return pMatrixVariable->SetMatrixArray(reinterpret_cast<const _float*>(_pMatrix), 0, _iNumMatrix);
}

HRESULT CShader::Bind_SRV(const _char* _pConstantName, ID3D11ShaderResourceView* _pSRV)
{
	if (m_pEffect == nullptr)
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (pVariable == nullptr)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (pSRVariable == nullptr)
		return E_FAIL;

	return pSRVariable->SetResource(_pSRV);
}

HRESULT CShader::Bind_SRV(const _char* _pConstantName, CTexture* _pTexture, _uint _iIndex)
{
	if (m_pEffect == nullptr)
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (pVariable == nullptr)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (pSRVariable == nullptr)
		return E_FAIL;

	return pSRVariable->SetResource(_pTexture->Get_SRV(_iIndex));
}

HRESULT CShader::Bind_SRVs(const _char* _pConstantName, ID3D11ShaderResourceView** _ppSRV, _uint _iNumSRV)
{
	if (m_pEffect == nullptr)
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (pVariable == nullptr)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (pSRVariable == nullptr)
		return E_FAIL;

	return pSRVariable->SetResourceArray(_ppSRV, 0, _iNumSRV);
}

HRESULT CShader::Bind_SRVs(const _char* _pConstantName, CTexture* _pTexture)
{
	if (m_pEffect == nullptr)
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (pVariable == nullptr)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (pSRVariable == nullptr)
		return E_FAIL;

	vector<ID3D11ShaderResourceView*>* vecSRV = _pTexture->Get_SRVs();

	return pSRVariable->SetResourceArray(&vecSRV->front(), 0, static_cast<uint32_t>(vecSRV->size()));
}

CShader* CShader::Create
(
	ID3D11Device* _pDevice, 
	ID3D11DeviceContext* _pContext, 
	const _tchar* _pShaderFilePath, 
	const D3D11_INPUT_ELEMENT_DESC* _pVertexElements, 
	_uint _iNumElement
)
{
	CShader* pInstance = new CShader(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_pShaderFilePath, _pVertexElements, _iNumElement)))
	{
		MSG_BOX("Failed To Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CShader::Clone(void* _pDesc)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	for (auto& InputLayout : m_vecInputLayout)
		Safe_Release(InputLayout);

	m_vecInputLayout.clear();

	Safe_Release(m_pEffect);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}