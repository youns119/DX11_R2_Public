#pragma once

#include "FActor.h"
#include "FCubeComponent.h"

BEGIN(Engine)

struct FSkyBox
	: public FActor
	, public FCubeComponent
{
};

END