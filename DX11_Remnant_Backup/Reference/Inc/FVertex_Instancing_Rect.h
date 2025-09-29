#pragma once

BEGIN(Engine)

struct ENGINE_DLL VTXINSTANCINGRECT
{
	const static unsigned int iNumElement = 7;
	const static D3D11_INPUT_ELEMENT_DESC tElements[iNumElement];
};

END