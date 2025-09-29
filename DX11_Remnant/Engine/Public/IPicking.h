#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class CActor;

class ENGINE_DLL IPicking abstract
{
protected :
	virtual ~IPicking() = default;

public :
	virtual void Update() PURE;

public :
	virtual _bool Compute_PickPos(_float3* _pOut) PURE;
};

END