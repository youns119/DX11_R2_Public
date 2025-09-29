#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class CActor;

struct FActorComponent
{
	CActor* pOwner{};
};

END