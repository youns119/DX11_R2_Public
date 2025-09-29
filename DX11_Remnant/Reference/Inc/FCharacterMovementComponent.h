#pragma once

#include "FMovementComponent.h"

BEGIN(Engine)

struct FCharacterMovementComponent
	: public FMovementComponent
{
	_float fMoveAccel{};
	_float fMoveDecel{};
	_float fMaxVelocity{};
};

END