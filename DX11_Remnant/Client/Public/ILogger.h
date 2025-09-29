#pragma once

#include "Engine_Define.h"

BEGIN(Client)

class CPlayer;

class ILogger abstract
{
protected :
	virtual ~ILogger() = default;

public :
	virtual HRESULT Init() PURE;
	virtual void Pre_Update(_float _fTimeDelta) PURE;
	virtual void Update(_float _fTimeDelta) PURE;
	virtual void Post_Update(_float _fTimeDelta) PURE;
	virtual HRESULT Render() PURE;

public :
	virtual void Register_Player(CPlayer* _pPlayer) PURE;
};

END