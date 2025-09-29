#pragma once

BEGIN(Engine)

struct ENGINE_DLL VTXSTATICMESH
{
	XMFLOAT3 vPosition{};
	XMFLOAT3 vNormal{};
	XMFLOAT2 vTexCoord{};
	XMFLOAT3 vTangent{};

	XMUINT4 vBlendIndex{};
	XMFLOAT4 vBlendWeight{};

	const static unsigned int iNumElement = 4;
	const static D3D11_INPUT_ELEMENT_DESC tElements[iNumElement];
};

END