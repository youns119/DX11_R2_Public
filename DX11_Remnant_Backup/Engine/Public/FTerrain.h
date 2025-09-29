#pragma once

#include "FActor.h"
#include "FTerrainComponent.h"

BEGIN(Engine)

struct FTerrain
	: public FActor
	, public FTerrainComponent
{
};

END