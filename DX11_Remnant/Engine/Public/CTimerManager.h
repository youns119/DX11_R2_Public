#pragma once

#include "ITimerManager.h"

#include "CObject.h"

BEGIN(Engine)

class CTimer;

class ENGINE_DLL CTimerManager final
	: public ITimerManager
	, public CObject
{
private :
	CTimerManager();
	virtual ~CTimerManager() = default;

public :
	virtual HRESULT	Add_Timer(const _wstring& _strTimerTag) override;
	virtual void Update_Timer(const _wstring& _strTimerTag) override;

public :
	virtual _float Get_TimeDelta(const _wstring& _strTimerTag) override;

private :
	map<const _wstring, CTimer*> m_mapTimer;

private :
	CTimer* Find_Timer(const _wstring& _strTimerTag);

public :
	static CTimerManager* Create();
	virtual void Free() override;
};

END