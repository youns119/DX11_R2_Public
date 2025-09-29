#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL IFrustum abstract
{
protected :
	virtual ~IFrustum() = default;

public :
	virtual void Update() PURE;

public :
	virtual _bool IsIn_WorldSpace(_fvector _vPosition, _float _fRange = 0.f) PURE;
};

END