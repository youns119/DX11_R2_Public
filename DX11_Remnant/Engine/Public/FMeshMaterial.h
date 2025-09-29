#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

struct FMeshMaterial
{
	const _char* pConstantName{};

	_bool bBinded{};
	_uint iTextureIndex{};
};

END