#pragma once

#include "Client_Define.h"

#include "CAnimState.h"

BEGIN(Client)

class CPlayerState_Aim final
    : public CAnimState
{
private :
    CPlayerState_Aim();
    virtual ~CPlayerState_Aim() = default;

public :
    virtual HRESULT Init() override;

public :
    virtual void Enter(CCharacter* _pCharacter, _uint* _pState) override;
    virtual void Update
    (
        CCharacter* _pCharacter,
        _uint* _pState,
        _float _fTimeDelta
    ) override;
    virtual void Exit(CCharacter* _pCharacter, _uint* _pState) override;

private :
    _float m_fAttackAcc{};
    _float m_fAttackTime{1.f};

public :
    static CPlayerState_Aim* Create();
    virtual void Free() override;
};

END