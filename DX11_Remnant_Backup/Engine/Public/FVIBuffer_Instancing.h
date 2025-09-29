#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

struct FVIBuffer_Instancing
{
	_bool bLoop{};
	_uint iNumInstance{};
	_float2 vSize{};
	_float2 vSpeed{};
	_float2	vLifeTime{};
	_float3 vRange{};
	_float3 vCenter{};
	_float3	vPivot{};
};

END