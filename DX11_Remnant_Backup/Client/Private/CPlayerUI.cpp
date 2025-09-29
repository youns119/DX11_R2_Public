#include "pch.h"
#include "CPlayerUI.h"

#include "CCrossHair.h"
#include "CPlayerHP.h"
#include "CWeaponBox.h"

#include "ELevel.h"

CPlayerUI::CPlayerUI(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : CActor{ _pDevice, _pContext }
{
}

CPlayerUI::CPlayerUI(const CPlayerUI& _Prototype)
    : CActor(_Prototype)
{
}

HRESULT CPlayerUI::Init()
{
    if (FAILED(__super::Init()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerUI::Init(void* _pDesc)
{
    if (_pDesc == nullptr)
        return E_FAIL;

    if (FAILED(__super::Init(_pDesc)))
        return E_FAIL;

    return S_OK;
}

void CPlayerUI::Pre_Update(_float _fTimeDelta)
{
    __super::Pre_Update(_fTimeDelta);
}

void CPlayerUI::Update(_float _fTimeDelta)
{
    __super::Update(_fTimeDelta);
}

void CPlayerUI::Post_Update(_float _fTimeDelta)
{
    __super::Post_Update(_fTimeDelta);
}

HRESULT CPlayerUI::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

void CPlayerUI::Set_CrossHair(ECrossHair _eCrossHair)
{
    CCrossHair* pCrossHair = static_cast<CCrossHair*>(m_UIs[static_cast<_uint>(EPlayerUI::PLAYERUI_CROSSHAIR)]);

    pCrossHair->Set_CrossHair(_eCrossHair);
}

void CPlayerUI::Set_HP(const _float* _pHP, const _float* _pBossHP)
{
    CPlayerHP* pPlayerHP = static_cast<CPlayerHP*>(m_UIs[static_cast<_uint>(EPlayerUI::PLAYERUI_HP)]);

    pPlayerHP->Set_HP(_pHP, _pBossHP);
}

void CPlayerUI::Set_BossShow(_bool _bShow)
{
    CPlayerHP* pPlayerHP = static_cast<CPlayerHP*>(m_UIs[static_cast<_uint>(EPlayerUI::PLAYERUI_HP)]);

    pPlayerHP->Set_BossShow(_bShow);
}

HRESULT CPlayerUI::Ready_SubActor()
{
    FActor tActor{};
    tActor.pOuter = m_pOuter;

    m_UIs[static_cast<_uint>(EPlayerUI::PLAYERUI_CROSSHAIR)] = static_cast<CActor*>(__super::Add_SubActor
    (
        static_cast<_uint>(ELevel::LEVEL_STAGE1),
        TEXT("Actor_UI_CrossHair"),
        TEXT("CrossHair"),
        &tActor
    ));
    if (m_UIs[static_cast<_uint>(EPlayerUI::PLAYERUI_CROSSHAIR)] == nullptr)
        return E_FAIL;

    m_UIs[static_cast<_uint>(EPlayerUI::PLAYERUI_HP)] = static_cast<CActor*>(__super::Add_SubActor
    (
        static_cast<_uint>(ELevel::LEVEL_STAGE1),
        TEXT("Actor_UI_PlayerHP"),
        TEXT("PlayerHP"),
        &tActor
    ));
    if (m_UIs[static_cast<_uint>(EPlayerUI::PLAYERUI_HP)] == nullptr)
        return E_FAIL;

    m_UIs[static_cast<_uint>(EPlayerUI::PLAYERUI_WEAPON)] = static_cast<CActor*>(__super::Add_SubActor
    (
        static_cast<_uint>(ELevel::LEVEL_STAGE1),
        TEXT("Actor_UI_WeaponBox"),
        TEXT("WeaponBox"),
        &tActor
    ));
    if (m_UIs[static_cast<_uint>(EPlayerUI::PLAYERUI_HP)] == nullptr)
        return E_FAIL;

    return S_OK;
}

CPlayerUI* CPlayerUI::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CPlayerUI* pInstance = new CPlayerUI(_pDevice, _pContext);

    if (FAILED(pInstance->Init()))
    {
        MSG_BOX("Failed To Create : CPlayerUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CClonable* CPlayerUI::Clone(void* _pDesc)
{
    CPlayerUI* pInstance = new CPlayerUI(*this);

    if (FAILED(pInstance->Init(_pDesc)))
    {
        MSG_BOX("Failed To Create : CPlayerUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayerUI::Free()
{
    __super::Free();

    for (_uint i = 0; i < static_cast<_uint>(EPlayerUI::PLAYERUI_END); i++)
        Safe_Release(m_UIs[i]);
}