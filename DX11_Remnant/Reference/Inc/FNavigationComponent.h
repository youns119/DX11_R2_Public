#pragma once

#include "FActorComponent.h"

BEGIN(Engine)

struct FNavigationComponent
	: public FActorComponent
{
	_int iCurrCellIndex{ -1 };
};

END