#pragma once

#include "FActorComponent.h"

BEGIN(Engine)

struct FMovementComponent
	: public FActorComponent
{
	_float fMoveSpeed{};
	_float fRotationSpeed{};
};

END