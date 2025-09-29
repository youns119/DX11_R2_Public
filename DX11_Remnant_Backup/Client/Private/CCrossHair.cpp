#include "pch.h"
#include "CCrossHair.h"

#include "CUIComponent.h"

#include "ELevel.h"

CCrossHair::CCrossHair(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CActor{ _pDevice, _pContext }
{
}

CCrossHair::CCrossHair(const CCrossHair& _Prototype)
	: CActor(_Prototype)
{
}

HRESULT CCrossHair::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCrossHair::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return E_FAIL;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

void CCrossHair::Pre_Update(_float _fTimeDelta)
{
	__super::Pre_Update(_fTimeDelta);
}

void CCrossHair::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CCrossHair::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CCrossHair::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CCrossHair::Set_CrossHair(ECrossHair _eCrossHair)
{
	for (_uint i = 0; i < static_cast<_uint>(ECrossHair::CROSSHAIR_END); i++)
		m_UIs[i]->Set_Show(false);

	m_UIs[static_cast<_uint>(_eCrossHair)]->Set_Show(true);
}

HRESULT CCrossHair::Ready_Component()
{
	// CrossHair_Dot
	FUIComponent tUIComponent{};
	tUIComponent.pOwner = this;
	tUIComponent.eRenderType = ERender::RENDER_UI;
	tUIComponent.vScale = _float3(100.f, 100.f, 0.f);
	tUIComponent.vPosition = _float3(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f, 0.f);
	tUIComponent.fNear = 0.f;
	tUIComponent.fFar = 1.f;

	m_UIs[static_cast<_uint>(ECrossHair::CROSSHAIR_DOT)] = static_cast<CUIComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		g_strUIComponent,
		TEXT("CrossHair_Dot"),
		&tUIComponent
	));
	if (m_UIs[static_cast<_uint>(ECrossHair::CROSSHAIR_DOT)] == nullptr)
		return E_FAIL;

	// CrossHair_Rifle
	tUIComponent.pOwner = this;
	tUIComponent.eRenderType = ERender::RENDER_UI;
	tUIComponent.vScale = _float3(100.f, 100.f, 0.f);
	tUIComponent.vPosition = _float3(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f, 0.f);
	tUIComponent.fNear = 0.f;
	tUIComponent.fFar = 1.f;

	m_UIs[static_cast<_uint>(ECrossHair::CROSSHAIR_RIFLE)] = static_cast<CUIComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		g_strUIComponent,
		TEXT("CrossHair_Rifle"),
		&tUIComponent
	));
	if (m_UIs[static_cast<_uint>(ECrossHair::CROSSHAIR_RIFLE)] == nullptr)
		return E_FAIL;

	m_UIs[static_cast<_uint>(ECrossHair::CROSSHAIR_RIFLE)]->Set_Show(false);

	return S_OK;
}

HRESULT CCrossHair::Ready_Resource()
{
	// CrossHair_Dot
	if (FAILED(m_UIs[static_cast<_uint>(ECrossHair::CROSSHAIR_DOT)]->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		L"Shader_VtxPosTex"
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(ECrossHair::CROSSHAIR_DOT)]->Add_Texture
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Texture_CrossHair_Dot")
	)))
		return E_FAIL;

	if(FAILED(m_UIs[static_cast<_uint>(ECrossHair::CROSSHAIR_DOT)]->Bind_Material("g_Texture", 0)))
		return E_FAIL;

	// CrossHair_Rifle
	if (FAILED(m_UIs[static_cast<_uint>(ECrossHair::CROSSHAIR_RIFLE)]->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		L"Shader_VtxPosTex"
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(ECrossHair::CROSSHAIR_RIFLE)]->Add_Texture
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Texture_CrossHair_Rifle")
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(ECrossHair::CROSSHAIR_RIFLE)]->Bind_Material("g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CCrossHair* CCrossHair::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CCrossHair* pInstance = new CCrossHair(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed to Create : CCrossHair");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CCrossHair::Clone(void* _pDesc)
{
	CCrossHair* pInstance = new CCrossHair(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed to Create : CCrossHair");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCrossHair::Free()
{
	__super::Free();

	for (_uint i = 0; i < static_cast<_uint>(ECrossHair::CROSSHAIR_END); i++)
		Safe_Release(m_UIs[i]);
}