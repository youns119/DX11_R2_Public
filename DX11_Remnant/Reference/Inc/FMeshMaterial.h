#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

struct FMeshMaterial
{
	_bool bBinded{};
	const _char* pConstantName{};
	_uint iTextureIndex{};
};

END