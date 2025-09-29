#pragma once

BEGIN(Engine)

struct ENGINE_DLL VTXPOINT
{
	XMFLOAT3 vPosition;
	float fPSize;

	const static unsigned int iNumElement = 2;
	const static D3D11_INPUT_ELEMENT_DESC tElements[iNumElement];
};

struct ENGINE_DLL VTXINSTANCINGPOINT
{
	const static unsigned int iNumElement = 7;
	const static D3D11_INPUT_ELEMENT_DESC tElements[iNumElement];
};

END