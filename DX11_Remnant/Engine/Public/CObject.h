#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class CWorld;

class ENGINE_DLL CObject abstract
{
protected :
	CObject();
	virtual ~CObject() = default;

public :
	_uint AddRef();
	_uint Release();

private :
	_uint m_iRefCnt{};

public :
	virtual void Free();
};

END