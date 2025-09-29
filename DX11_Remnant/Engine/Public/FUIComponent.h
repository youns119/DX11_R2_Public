#pragma once

#include "FPrimitiveComponent.h"

BEGIN(Engine)

struct FUIComponent
	: public FPrimitiveComponent
{
	_float fNear{}, fFar{};
};

END