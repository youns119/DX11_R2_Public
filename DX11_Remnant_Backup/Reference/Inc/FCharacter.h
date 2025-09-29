#pragma once

#include "FActor.h"
#include "FCharacterMovementComponent.h"

BEGIN(Engine)

struct FCharacter
	: public FActor
	, public FCharacterMovementComponent
{
};

END