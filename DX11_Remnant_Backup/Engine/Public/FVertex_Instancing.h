#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

struct VTXINSTANCING
{
	XMFLOAT4 vRight;
	XMFLOAT4 vUp;
	XMFLOAT4 vLook;
	XMFLOAT4 vTranslation;
	XMFLOAT2 vLifeTime;
};

END