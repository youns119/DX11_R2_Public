#include "CTexture.h"

#include "CShader.h"

CTexture::CTexture(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CTexture::CTexture(const CTexture& _Prototype)
	: m_pDevice{ _Prototype.m_pDevice }
	, m_pContext{ _Prototype.m_pContext }
	, m_vecSRV{ _Prototype.m_vecSRV }
	, m_iNumSRV{ _Prototype.m_iNumSRV }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	for (auto& SRV : m_vecSRV)
		Safe_AddRef(SRV);
}

HRESULT CTexture::Init(const _tchar* _pTextureFilePath, _uint _iNumTexture)
{
	m_iNumSRV = _iNumTexture;

	_tchar szEXT[MAX_PATH]{};

	_wsplitpath_s(_pTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);

	for (size_t i = 0; i < m_iNumSRV; i++)
	{
		ID3D11ShaderResourceView* pSRV{};

		_tchar szTextureFilePath[MAX_PATH] = TEXT("");

		wsprintf(szTextureFilePath, _pTextureFilePath, i);

		HRESULT hr{};

		if (lstrcmp(szEXT, TEXT(".dds")) == false)
			hr = CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		else if (lstrcmp(szEXT, TEXT(".tga")) == false)
			hr = E_FAIL;
		else
			hr = CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);

		if (FAILED(hr))
			return E_FAIL;

		m_vecSRV.push_back(pSRV);
	}

	return S_OK;
}

HRESULT CTexture::Init(void* _pDesc)
{
	return S_OK;
}

HRESULT CTexture::Bind_ShaderResource(CShader* _pShader, const _char* _pConstantName, _uint _iIndex)
{
	if (_iIndex >= m_iNumSRV)
		return E_FAIL;

	return _pShader->Bind_SRV(_pConstantName, m_vecSRV[_iIndex]);
}

HRESULT CTexture::Bind_ShaderResources(CShader* _pShader, const _char* _pConstantName)
{
	return _pShader->Bind_SRVs(_pConstantName, &m_vecSRV.front(), m_iNumSRV);
}

CTexture* CTexture::Create
(
	ID3D11Device* _pDevice, 
	ID3D11DeviceContext* _pContext, 
	const _tchar* _pTextureFilePath,
	_uint _iNumTexture
)
{
	CTexture* pInstance = new CTexture(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_pTextureFilePath, _iNumTexture)))
	{
		MSG_BOX("Failed To Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CTexture::Clone(void* _pDesc)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& SRV : m_vecSRV)
		Safe_Release(SRV);

	m_vecSRV.clear();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}