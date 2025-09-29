#pragma once

#include "Client_Define.h"

#include "CAnimStateMachine.h"

BEGIN(Client)

class CAnimStateMachine_Player final
	: public CAnimStateMachine
{
private :
	CAnimStateMachine_Player();
	virtual ~CAnimStateMachine_Player() = default;

public :
	virtual HRESULT Init
	(
		CCharacter* _pCharacter, 
		_uint* _pState, 
		const _wstring& _strState
	) override;
	virtual void Update(_float _fTimeDelta) override;

public :
	virtual HRESULT ChangeState(const _wstring& _strState) override;

protected :
	virtual HRESULT Ready_AnimState() override;

private :
	_float m_fTime{};

public :
	static CAnimStateMachine_Player* Create
	(
		CCharacter* _pCharacter,
		_uint* _pState,
		const _wstring& _strState
	);
	virtual void Free() override;
};

END