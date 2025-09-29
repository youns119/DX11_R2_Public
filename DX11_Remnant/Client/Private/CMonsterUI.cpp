#include "pch.h"
#include "CMonsterUI.h"

#include "CMonsterHP.h"

#include "ELevel.h"

CMonsterUI::CMonsterUI(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CActor{ _pDevice, _pContext }
{
}

CMonsterUI::CMonsterUI(const CMonsterUI& _Prototype)
	: CActor(_Prototype)
{
}

HRESULT CMonsterUI::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterUI::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return E_FAIL;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

void CMonsterUI::Pre_Update(_float _fTimeDelta)
{
	__super::Pre_Update(_fTimeDelta);
}

void CMonsterUI::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CMonsterUI::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CMonsterUI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CMonsterUI::Set_Pos(_float3 _vPos)
{
    CMonsterHP* pMonsterHP = static_cast<CMonsterHP*>(m_UIs[static_cast<_uint>(EMonsterUI::MONSTERUI_HP)]);

    pMonsterHP->Set_Pos(_vPos);
}

void CMonsterUI::Ready_HPUI(const _float* _pHP)
{
    CMonsterHP* pMonsterHP = static_cast<CMonsterHP*>(m_UIs[static_cast<_uint>(EMonsterUI::MONSTERUI_HP)]);

    pMonsterHP->Ready_HPUI(_pHP);
}

HRESULT CMonsterUI::Ready_SubActor()
{
    FActor tActor{};
    tActor.pOuter = m_pOuter;

    m_UIs[static_cast<_uint>(EMonsterUI::MONSTERUI_HP)] = static_cast<CActor*>(__super::Add_SubActor
    (
        static_cast<_uint>(ELevel::LEVEL_STAGE1),
        TEXT("Actor_UI_MonsterHP"),
        TEXT("MonsterHP"),
        &tActor
    ));
    if (m_UIs[static_cast<_uint>(EMonsterUI::MONSTERUI_HP)] == nullptr)
        return E_FAIL;

    return S_OK;
}

CMonsterUI* CMonsterUI::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CMonsterUI* pInstance = new CMonsterUI(_pDevice, _pContext);

    if (FAILED(pInstance->Init()))
    {
        MSG_BOX("Failed To Create : CMonsterUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CClonable* CMonsterUI::Clone(void* _pDesc)
{
    CMonsterUI* pInstance = new CMonsterUI(*this);

    if (FAILED(pInstance->Init(_pDesc)))
    {
        MSG_BOX("Failed To Create : CMonsterUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonsterUI::Free()
{
    __super::Free();

    for (_uint i = 0; i < static_cast<_uint>(EMonsterUI::MONSTERUI_END); i++)
        Safe_Release(m_UIs[i]);
}