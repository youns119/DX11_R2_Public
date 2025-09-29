#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class CTimer;

class ENGINE_DLL ITimerManager abstract
{
protected :
	virtual ~ITimerManager() = default;

public :
	virtual HRESULT	Add_Timer(const _wstring& _strTimerTag) PURE;
	virtual void Update_Timer(const _wstring& _strTimerTag) PURE;

public :
	virtual _float Get_TimeDelta(const _wstring& _strTimerTag) PURE;
};

END