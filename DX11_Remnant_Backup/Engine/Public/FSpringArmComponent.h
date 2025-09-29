#pragma once

#include "FSceneComponent.h"

BEGIN(Engine)

struct FSpringArmComponent
	: public FSceneComponent
{
	_float3 vOffset{};

	_float fArmLength{};
};

END