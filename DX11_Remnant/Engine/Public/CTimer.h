#pragma once

#include "CObject.h"

BEGIN(Engine)

class CTimer final
	: public CObject
{
private :
	CTimer();
	virtual ~CTimer() = default;

public :
	HRESULT	Init();
	void Update();

public :
	_float Get_TimeDelta() const { return m_fTimeDelta; }

private :
	LARGE_INTEGER m_FrameTime{};
	LARGE_INTEGER m_FixTime{};
	LARGE_INTEGER m_LastTime{};
	LARGE_INTEGER m_CpuTick{};

	_float m_fTimeDelta{};

public :
	static CTimer* Create();
	virtual void Free() override;
};

END