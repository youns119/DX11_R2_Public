#pragma once

#include "FActorComponent.h"

#include "ELight.h"

BEGIN(Engine)

struct FLightComponent
	: public FActorComponent
{
	ELight eLight{ ELight::LIGHT_END };

	_float3 vPosition{};
	_float3 vDirection{};

	_float4 vDiffuse{};
	_float4 vAmbient{};
	_float4 vSpecular{};

	_float fRange{};
};

END