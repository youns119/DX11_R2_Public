#include "pch.h"
#include "CPlayerHP.h"

#include "CGameInstance.h"
#include "CLevel.h"
#include "CUIComponent.h"

#include "ELevel.h"

CPlayerHP::CPlayerHP(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CActor{ _pDevice, _pContext }
{
}

CPlayerHP::CPlayerHP(const CPlayerHP& _Prototype)
	: CActor(_Prototype)
{
}

HRESULT CPlayerHP::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerHP::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return E_FAIL;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

void CPlayerHP::Pre_Update(_float _fTimeDelta)
{
	__super::Pre_Update(_fTimeDelta);
}

void CPlayerHP::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CPlayerHP::Post_Update(_float _fTimeDelta)
{
	_float3 vScale = m_UIs[static_cast<_uint>(EPlayerHP::PLAYERHP_HP)]->Get_Transform(ETransform::TRANSFORM_SCALE);

	_float fRatio = *m_pCurrHP / m_fMaxHP;
	_float fScale = 395.f * fRatio;
	_float fDiff = fScale - vScale.x;

	m_UIs[static_cast<_uint>(EPlayerHP::PLAYERHP_HP)]->Set_DeltaScale(XMVectorSet(fDiff, 0.f, 0.f, 0.f));
	m_UIs[static_cast<_uint>(EPlayerHP::PLAYERHP_HP)]->Set_DeltaPosition(XMVectorSet(fDiff * 0.5f, 0.f, 0.f, 1.f));



	vScale = m_UIs[static_cast<_uint>(EPlayerHP::BOSS_HP)]->Get_Transform(ETransform::TRANSFORM_SCALE);

	fRatio = *m_pBossCurrHP / m_fBossMaxHP;
	fScale = 495.f * fRatio;
	fDiff = fScale - vScale.x;

	m_UIs[static_cast<_uint>(EPlayerHP::BOSS_HP)]->Set_DeltaScale(XMVectorSet(fDiff, 0.f, 0.f, 0.f));
	m_UIs[static_cast<_uint>(EPlayerHP::BOSS_HP)]->Set_DeltaPosition(XMVectorSet(fDiff * 0.5f, 0.f, 0.f, 1.f));

	__super::Post_Update(_fTimeDelta);
}

HRESULT CPlayerHP::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CPlayerHP::Set_BossShow(_bool _bShow)
{
	static_cast<CUIComponent*>(m_UIs[static_cast<_uint>(EPlayerHP::BOSS_BACK)])->Set_Show(_bShow);
	static_cast<CUIComponent*>(m_UIs[static_cast<_uint>(EPlayerHP::BOSS_HP)])->Set_Show(_bShow);
}

HRESULT CPlayerHP::Ready_Component()
{
	// PlayerHP_Back
	FUIComponent tUIComponent{};
	tUIComponent.pOwner = this;
	tUIComponent.eRenderType = ERender::RENDER_UI;
	tUIComponent.vScale = _float3(400.f, 15.f, 0.f);
	tUIComponent.vPosition = _float3(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f + 300.f, 0.f);
	tUIComponent.fNear = 0.f;
	tUIComponent.fFar = 1.f;

	m_UIs[static_cast<_uint>(EPlayerHP::PLAYERHP_BACK)] = static_cast<CUIComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		g_strUIComponent,
		TEXT("Bar_Back"),
		&tUIComponent
	));
	if (m_UIs[static_cast<_uint>(EPlayerHP::PLAYERHP_BACK)] == nullptr)
		return E_FAIL;

	// PlayerHP_HP
	tUIComponent.pOwner = this;
	tUIComponent.eRenderType = ERender::RENDER_UI;
	tUIComponent.vScale = _float3(395.f, 10.f, 0.f);
	tUIComponent.vPosition = _float3(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f + 300.f, 0.f);
	//tUIComponent.vPosition = _float3(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f, 0.f);
	tUIComponent.fNear = 0.f;
	tUIComponent.fFar = 1.f;

	m_UIs[static_cast<_uint>(EPlayerHP::PLAYERHP_HP)] = static_cast<CUIComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		g_strUIComponent,
		TEXT("Bar_HP"),
		&tUIComponent
	));
	if (m_UIs[static_cast<_uint>(EPlayerHP::PLAYERHP_HP)] == nullptr)
		return E_FAIL;

	// BossHP_Back
	tUIComponent.pOwner = this;
	tUIComponent.eRenderType = ERender::RENDER_UI;
	tUIComponent.vScale = _float3(500.f, 20.f, 0.f);
	tUIComponent.vPosition = _float3(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 300.f, 0.f);
	tUIComponent.fNear = 0.f;
	tUIComponent.fFar = 1.f;

	m_UIs[static_cast<_uint>(EPlayerHP::BOSS_BACK)] = static_cast<CUIComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		g_strUIComponent,
		TEXT("Boss_Bar_Back"),
		&tUIComponent
	));
	if (m_UIs[static_cast<_uint>(EPlayerHP::BOSS_BACK)] == nullptr)
		return E_FAIL;

	m_UIs[static_cast<_uint>(EPlayerHP::BOSS_BACK)]->Set_Show(false);

	// BossHP_HP
	tUIComponent.pOwner = this;
	tUIComponent.eRenderType = ERender::RENDER_UI;
	tUIComponent.vScale = _float3(495.f, 15.f, 0.f);
	tUIComponent.vPosition = _float3(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f - 300.f, 0.f);
	tUIComponent.fNear = 0.f;
	tUIComponent.fFar = 1.f;

	m_UIs[static_cast<_uint>(EPlayerHP::BOSS_HP)] = static_cast<CUIComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		g_strUIComponent,
		TEXT("Boss_Bar_HP"),
		&tUIComponent
	));
	if (m_UIs[static_cast<_uint>(EPlayerHP::BOSS_HP)] == nullptr)
		return E_FAIL;

	m_UIs[static_cast<_uint>(EPlayerHP::BOSS_HP)]->Set_Show(false);

	return S_OK;
}

HRESULT CPlayerHP::Ready_Resource()
{
	// PlayerHP_Back
	if (FAILED(m_UIs[static_cast<_uint>(EPlayerHP::PLAYERHP_BACK)]->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		L"Shader_VtxPosTex"
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EPlayerHP::PLAYERHP_BACK)]->Add_Texture
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Texture_Bar_Back")
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EPlayerHP::PLAYERHP_BACK)]->Bind_Material("g_Texture", 0)))
		return E_FAIL;

	// PlayerHP_HP
	if (FAILED(m_UIs[static_cast<_uint>(EPlayerHP::PLAYERHP_HP)]->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		L"Shader_VtxPosTex"
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EPlayerHP::PLAYERHP_HP)]->Add_Texture
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Texture_Bar_HP")
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EPlayerHP::PLAYERHP_HP)]->Bind_Material("g_Texture", 0)))
		return E_FAIL;


	// BossHP_Back
	if (FAILED(m_UIs[static_cast<_uint>(EPlayerHP::BOSS_BACK)]->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		L"Shader_VtxPosTex"
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EPlayerHP::BOSS_BACK)]->Add_Texture
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Texture_Bar_Back")
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EPlayerHP::BOSS_BACK)]->Bind_Material("g_Texture", 0)))
		return E_FAIL;

	// BossHP_HP
	if (FAILED(m_UIs[static_cast<_uint>(EPlayerHP::BOSS_HP)]->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		L"Shader_VtxPosTex"
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EPlayerHP::BOSS_HP)]->Add_Texture
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Texture_Bar_HP")
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EPlayerHP::BOSS_HP)]->Bind_Material("g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CPlayerHP* CPlayerHP::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CPlayerHP* pInstance = new CPlayerHP(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed to Create : CPlayerHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CPlayerHP::Clone(void* _pDesc)
{
	CPlayerHP* pInstance = new CPlayerHP(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed to Create : CPlayerHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerHP::Free()
{
	__super::Free();

	for (_uint i = 0; i < static_cast<_uint>(EPlayerHP::PLAYERHP_END); i++)
		Safe_Release(m_UIs[i]);
}