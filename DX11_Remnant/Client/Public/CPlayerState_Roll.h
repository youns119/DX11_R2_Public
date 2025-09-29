#pragma once

#include "Client_Define.h"

#include "CAnimState.h"

BEGIN(Client)

class CPlayerState_Roll final
    : public CAnimState
{
private :
    CPlayerState_Roll();
    virtual ~CPlayerState_Roll() = default;

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
    _float m_fRollTime{ 1.5f };

public :
    static CPlayerState_Roll* Create();
    virtual void Free() override;
};

END