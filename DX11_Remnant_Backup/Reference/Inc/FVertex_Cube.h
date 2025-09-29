#pragma once

BEGIN(Engine)

struct ENGINE_DLL VTXCUBE
{
	XMFLOAT3 vPosition{};
	XMFLOAT3 vTexcoord{};

	const static unsigned int iNumElement = 2;
	const static D3D11_INPUT_ELEMENT_DESC tElements[iNumElement];
};

END