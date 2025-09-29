#include "pch.h"
#include "CWeaponBox.h"

#include "CGameInstance.h"
#include "CLevel.h"
#include "CUIComponent.h"

#include "ELevel.h"

CWeaponBox::CWeaponBox(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CActor{ _pDevice, _pContext }
{
}

CWeaponBox::CWeaponBox(const CWeaponBox& _Prototype)
	: CActor(_Prototype)
{
}

HRESULT CWeaponBox::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeaponBox::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return E_FAIL;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

void CWeaponBox::Pre_Update(_float _fTimeDelta)
{
	__super::Pre_Update(_fTimeDelta);
}

void CWeaponBox::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CWeaponBox::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CWeaponBox::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeaponBox::Ready_Component()
{
	// WeaponBox_Box
	FUIComponent tUIComponent{};
	tUIComponent.pOwner = this;
	tUIComponent.eRenderType = ERender::RENDER_UI;
	tUIComponent.vScale = _float3(250.f, 100.f, 0.f);
	tUIComponent.vPosition = _float3(g_iWinSizeX * 0.5f + 450.f, g_iWinSizeY * 0.5f + 300.f, 0.f);
	tUIComponent.fNear = 0.f;
	tUIComponent.fFar = 1.f;

	m_UIs[static_cast<_uint>(EWeaponBox::WEAPONBOX_BOX)] = static_cast<CUIComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		g_strUIComponent,
		TEXT("Box_Box"),
		&tUIComponent
	));
	if (m_UIs[static_cast<_uint>(EWeaponBox::WEAPONBOX_BOX)] == nullptr)
		return E_FAIL;

	// WeaponBox_Icon
	tUIComponent.pOwner = this;
	tUIComponent.eRenderType = ERender::RENDER_UI;
	tUIComponent.vScale = _float3(200.f, 50.f, 0.f);
	tUIComponent.vPosition = _float3(g_iWinSizeX * 0.5f + 440.f, g_iWinSizeY * 0.5f + 300.f, 0.f);
	tUIComponent.fNear = 0.f;
	tUIComponent.fFar = 1.f;

	m_UIs[static_cast<_uint>(EWeaponBox::WEAPONBOX_ICON)] = static_cast<CUIComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		g_strUIComponent,
		TEXT("Box_Icon"),
		&tUIComponent
	));
	if (m_UIs[static_cast<_uint>(EWeaponBox::WEAPONBOX_ICON)] == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CWeaponBox::Ready_Resource()
{
	// Weapon_Box
	if (FAILED(m_UIs[static_cast<_uint>(EWeaponBox::WEAPONBOX_BOX)]->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		L"Shader_VtxPosTex"
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EWeaponBox::WEAPONBOX_BOX)]->Add_Texture
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Texture_Box")
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EWeaponBox::WEAPONBOX_BOX)]->Bind_Material("g_Texture", 0)))
		return E_FAIL;

	// Weapon_Icon
	if (FAILED(m_UIs[static_cast<_uint>(EWeaponBox::WEAPONBOX_ICON)]->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		L"Shader_VtxPosTex"
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EWeaponBox::WEAPONBOX_ICON)]->Add_Texture
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Texture_Icon_AssaultRifle")
	)))
		return E_FAIL;

	if (FAILED(m_UIs[static_cast<_uint>(EWeaponBox::WEAPONBOX_ICON)]->Bind_Material("g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CWeaponBox* CWeaponBox::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CWeaponBox* pInstance = new CWeaponBox(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed to Create : CWeaponBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CWeaponBox::Clone(void* _pDesc)
{
	CWeaponBox* pInstance = new CWeaponBox(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed to Create : CWeaponBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeaponBox::Free()
{
	__super::Free();

	for (_uint i = 0; i < static_cast<_uint>(EWeaponBox::WEAPONBOX_END); i++)
		Safe_Release(m_UIs[i]);
}