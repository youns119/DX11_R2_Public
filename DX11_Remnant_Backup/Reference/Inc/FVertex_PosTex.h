#pragma once

BEGIN(Engine)

struct ENGINE_DLL VTXPOSTEX
{
	XMFLOAT3 vPosition{};
	XMFLOAT2 vTexCoord{};

	const static unsigned int iNumElement = 2;
	const static D3D11_INPUT_ELEMENT_DESC tElements[iNumElement];
};

END