#pragma once

#include "CObject.h"

BEGIN(Engine)

class ENGINE_DLL CClonable abstract
	: public CObject
{
protected :
	virtual ~CClonable() = default;
	
public :
	virtual HRESULT Init(void* _pDesc) PURE;

	virtual CClonable* Clone(void* _pDesc) PURE;
};

END