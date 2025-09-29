#pragma once

BEGIN(Engine)

struct ENGINE_DLL VTXPOS
{
	XMFLOAT3 vPosition{};

	const static unsigned int iNumElement = 1;
	const static D3D11_INPUT_ELEMENT_DESC tElements[iNumElement];
};

END