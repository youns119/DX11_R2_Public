#pragma once

#include "CObject.h"

BEGIN(Engine)

class CCharacter;
class CAnimState;

class ENGINE_DLL CAnimStateMachine abstract
	: public CObject
{
protected :
	CAnimStateMachine();
	virtual ~CAnimStateMachine() = default;

public :
	virtual HRESULT Init
	(
		CCharacter* _pCharacter, 
		_uint* _pState, 
		const _wstring& _strState
	);
	virtual void Update(_float _fTimeDelta);

public :
	virtual HRESULT ChangeState(const _wstring& _strState);

public :
	void Clear();

protected :
	CCharacter* m_pCharacter{};

protected :
	_uint* m_pState{};

protected :
	map<const _wstring, CAnimState*> m_mapAnimState;

	CAnimState* m_pCurrAnimState{};

protected :
	virtual HRESULT Add_AnimState(const _wstring& _strAnimState, CAnimState* _pAnimState);

protected :
	virtual HRESULT Ready_AnimState() PURE;

protected :
	CAnimState* Find_AnimState(const _wstring& _strAnimState);

public :
	virtual void Free() override;
};

END