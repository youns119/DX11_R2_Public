#pragma once

#include "FPrimitiveComponent.h"

BEGIN(Engine)

struct FSkeletalMeshComponent
	: public FPrimitiveComponent
{
	const _char* szAnimName{ "" };
	const _char* szRootName{ "" };
};

END