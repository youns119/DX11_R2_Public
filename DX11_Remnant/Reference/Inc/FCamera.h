#pragma once

#include "FActor.h"
#include "FCameraComponent.h"

BEGIN(Engine)

struct FCamera
	: public FActor
	, public FCameraComponent
{
	_float fMouseSensor{};
};

END