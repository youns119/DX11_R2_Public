#pragma once

#include "FActor.h"
#include "FLightComponent.h"

BEGIN(Engine)

struct FLight
	: public FActor
	, public FLightComponent
{
};

END