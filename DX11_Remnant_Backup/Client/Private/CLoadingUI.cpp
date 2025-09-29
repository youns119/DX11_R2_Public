#include "pch.h"
#include "CLoadingUI.h"

#include "CUIComponent.h"

#include "ELevel.h"

CLoadingUI::CLoadingUI(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CActor{ _pDevice, _pContext }
{
}

CLoadingUI::CLoadingUI(const CLoadingUI& _Prototype)
	: CActor(_Prototype)
{
}

HRESULT CLoadingUI::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingUI::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return E_FAIL;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

void CLoadingUI::Pre_Update(_float _fTimeDelta)
{
	__super::Pre_Update(_fTimeDelta);
}

void CLoadingUI::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CLoadingUI::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CLoadingUI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingUI::Ready_Component()
{
	// CrossHair_Dot
	FUIComponent tUIComponent{};
	tUIComponent.pOwner = this;
	tUIComponent.eRenderType = ERender::RENDER_UI;
	tUIComponent.vScale = _float3(g_iWinSizeX * 0.7f, g_iWinSizeY * 0.8f, 0.f);
	tUIComponent.vPosition = _float3(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f, 0.f);
	tUIComponent.fNear = 0.f;
	tUIComponent.fFar = 1.f;

	m_pLogo = static_cast<CUIComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		g_strUIComponent,
		TEXT("Logo"),
		&tUIComponent
	));
	if (m_pLogo == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingUI::Ready_Resource()
{
	// Logo
	if (FAILED(m_pLogo->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		L"Shader_VtxPosTex"
	)))
		return E_FAIL;

	if (FAILED(m_pLogo->Add_Texture
	(
		static_cast<_uint>(ELevel::LEVEL_LOGO),
		TEXT("Texture_Logo")
	)))
		return E_FAIL;

	if (FAILED(m_pLogo->Bind_Material("g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CLoadingUI* CLoadingUI::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLoadingUI* pInstance = new CLoadingUI(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CLoadingUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CLoadingUI::Clone(void* _pDesc)
{
	CLoadingUI* pInstance = new CLoadingUI(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CLoadingUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoadingUI::Free()
{
	__super::Free();

	Safe_Release(m_pLogo);
}