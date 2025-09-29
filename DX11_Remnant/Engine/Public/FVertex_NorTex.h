#pragma once

BEGIN(Engine)

struct ENGINE_DLL VTXNORTEX
{
	XMFLOAT3 vPosition{};
	XMFLOAT3 vNormal{};
	XMFLOAT2 vTexCoord{};

	const static unsigned int iNumElement = 3;
	const static D3D11_INPUT_ELEMENT_DESC tElements[iNumElement];
};

END