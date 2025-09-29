#pragma once

#include "Client_Define.h"

#include "CAnimState.h"

BEGIN(Client)

class CPlayerState_Locomotion final
    : public CAnimState
{
private :
    CPlayerState_Locomotion();
    virtual ~CPlayerState_Locomotion() = default;

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
    _float m_fWalkSpeed{ 4.f };
    _float m_fRunSpeed{ 8.f };
    _float m_fSprintSpeed{ 16.f };

private :
    _float m_fSoundTimeAcc{};
    _float m_fWalkSoundDist{};

public :
    static CPlayerState_Locomotion* Create();
    virtual void Free() override;
};

END