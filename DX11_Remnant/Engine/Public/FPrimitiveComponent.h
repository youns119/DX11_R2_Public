#pragma once

#include "FSceneComponent.h"

#include "ERender.h"

BEGIN(Engine)

struct FPrimitiveComponent
	: public FSceneComponent
{
	ERender eRenderType{ ERender::RENDER_END };

	_bool bShow{ true };
	_bool bCulling{ false };
	_uint iPassIndex{};
	_float fCullingRange{};
};

END