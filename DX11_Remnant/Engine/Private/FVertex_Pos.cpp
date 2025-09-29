#include "Engine_Define.h"

#include "FVertex_Pos.h"

const D3D11_INPUT_ELEMENT_DESC VTXPOS::tElements[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};