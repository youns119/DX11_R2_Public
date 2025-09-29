#include "CTargetManager.h"

#include "CRenderTarget.h"

CTargetManager::CTargetManager(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CTargetManager::Init()
{
	return S_OK;
}

HRESULT CTargetManager::Add_RenderTarget
(
	const _wstring& _strRenderTargetTag, 
	_uint _iWidth, _uint _iHeight, 
	DXGI_FORMAT _ePixelFormat, 
	const _float4& _vClearColor
)
{
	if (Find_RenderTarget(_strRenderTargetTag) != nullptr)
		return E_FAIL;

	CRenderTarget* pRenderTarget = CRenderTarget::Create
	(
		m_pDevice, 
		m_pContext,
		_iWidth, _iHeight,
		_ePixelFormat, 
		_vClearColor
	);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	m_vecRenderTarget.emplace(_strRenderTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CTargetManager::Add_MRT(const _wstring& _strMRTTag, const _wstring& _strRenderTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(_strRenderTargetTag);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	vector<CRenderTarget*>* pMRT = Find_MRT(_strMRTTag);
	if (pMRT == nullptr)
	{
		vector<CRenderTarget*> vecMRT;

		vecMRT.reserve(8);
		vecMRT.push_back(pRenderTarget);

		m_vecMRT.emplace(_strMRTTag, vecMRT);
	}
	else
		pMRT->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

HRESULT CTargetManager::Begin_MRT(const _wstring& _strMRTTag)
{
	m_pContext->OMGetRenderTargets(1, &m_pBackRTV, &m_pOriginalDSV);

	ID3D11ShaderResourceView* pSRV[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]{};

	m_pContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pSRV);

	vector<CRenderTarget*>* pMRT = Find_MRT(_strMRTTag);
	if (pMRT == nullptr)
		return E_FAIL;

	ID3D11RenderTargetView* pRTVs[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

	_uint iNumRenderTargets{};

	for (auto& RenderTarget : *pMRT)
	{
		RenderTarget->Clear();
		pRTVs[iNumRenderTargets++] = RenderTarget->Get_RTV();
	}

	m_pContext->OMSetRenderTargets(iNumRenderTargets, pRTVs, m_pOriginalDSV);

	return S_OK;
}

HRESULT CTargetManager::End_MRT()
{
	m_pContext->OMSetRenderTargets(1, &m_pBackRTV, m_pOriginalDSV);

	Safe_Release(m_pOriginalDSV);
	Safe_Release(m_pBackRTV);

	return S_OK;
}

HRESULT CTargetManager::Bind_RT_ShaderResource
(
	const _wstring& _strRenderTargetTag, 
	CShader* _pShader, 
	const _char* _pConstantName
)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(_strRenderTargetTag);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	return pRenderTarget->Bind_ShaderResource(_pShader, _pConstantName);
}

HRESULT CTargetManager::Copy_RT_Resource(const _wstring& _strRenderTargetTag, ID3D11Texture2D* _pTexture2D)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(_strRenderTargetTag);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	return pRenderTarget->Copy_Resource(_pTexture2D);
}

#ifdef _DEBUG

HRESULT CTargetManager::Ready_RT_Debug
(
	const _wstring& _strRenderTargetTag, 
	_float _fX, _float _fY, 
	_float _fSizeX, _float _fSizeY
)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(_strRenderTargetTag);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	return pRenderTarget->Ready_Debug(_fX, _fY, _fSizeX, _fSizeY);
}

HRESULT CTargetManager::Render_RT_Debug(const _wstring& _strMRTTag, CShader* _pShader, CVIBuffer_Rect* _pVIBuffer)
{
	vector<CRenderTarget*>* pMRT = Find_MRT(_strMRTTag);
	if (pMRT == nullptr)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRT)
	{
		if (pRenderTarget != nullptr)
			pRenderTarget->Render_Debug(_pShader, _pVIBuffer);
	}

	return S_OK;
}

#endif

CRenderTarget* CTargetManager::Find_RenderTarget(const _wstring& _strRenderTargetTag)
{
	auto iter = m_vecRenderTarget.find(_strRenderTargetTag);
	if (iter == m_vecRenderTarget.end())
		return nullptr;

	return iter->second;
}

vector<CRenderTarget*>* CTargetManager::Find_MRT(const _wstring& _strMRTTag)
{
	auto iter = m_vecMRT.find(_strMRTTag);
	if (iter == m_vecMRT.end())
		return nullptr;

	return &iter->second;
}

CTargetManager* CTargetManager::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CTargetManager* pInstance = new CTargetManager(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed to Created : CTargetManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTargetManager::Free()
{
	__super::Free();

	for (auto& MRT : m_vecMRT)
	{
		for (auto& RenderTarget : MRT.second)
			Safe_Release(RenderTarget);

		MRT.second.clear();
	}
	m_vecMRT.clear();

	for (auto& RenderTarget : m_vecRenderTarget)
		Safe_Release(RenderTarget.second);

	m_vecRenderTarget.clear();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}