#pragma once

#include "FSceneComponent.h"

BEGIN(Engine)

struct FCameraComponent
	: public FSceneComponent
{
	_float fFov{};
	_float fNear{}, fFar{};
};

END