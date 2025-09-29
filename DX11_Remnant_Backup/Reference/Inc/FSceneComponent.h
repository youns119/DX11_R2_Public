#pragma once

#include "FActorComponent.h"

#include "ETransform.h"

BEGIN(Engine)

struct FSceneComponent
	: public FActorComponent
{
	_float3 vScale = _float3(1.f, 1.f, 1.f);
	_float3 vRotation = _float3(0.f, 0.f, 0.f);
	_float3 vPosition = _float3(0.f, 0.f, 0.f);
};

END