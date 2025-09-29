#pragma once

#include "FPrimitiveComponent.h"

BEGIN(Engine)

struct FTerrainComponent
	: public FPrimitiveComponent
{
	_uint iNumTexture{};
};

END