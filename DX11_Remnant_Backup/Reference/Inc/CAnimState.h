#pragma once

#include "CObject.h"

BEGIN(Engine)

class CCharacter;

class ENGINE_DLL CAnimState abstract
	: public CObject
{
protected :
    CAnimState();
    virtual ~CAnimState() = default;

public :
    virtual HRESULT Init() { return S_OK; }

public :
    virtual void Enter(CCharacter* _pCharacter, _uint* _pState) PURE;
    virtual void Update
    (
        CCharacter* _pCharacter, 
        _uint* _pState, 
        _float _fTimeDelta
    ) PURE;
    virtual void Exit(CCharacter* _pCharacter, _uint* _pState) PURE;

public :
    virtual void Free() override;
};

END