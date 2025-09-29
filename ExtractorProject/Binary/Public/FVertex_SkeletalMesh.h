#pragma once

struct VTXSKELETALMESH
{
	XMFLOAT3 vPosition{};
	XMFLOAT3 vNormal{};
	XMFLOAT2 vTexCoord{};
	XMFLOAT3 vTangent{};

	XMUINT4 vBlendIndex{};
	XMFLOAT4 vBlendWeight{};

	const static unsigned int iNumElement = 6;
	const static D3D11_INPUT_ELEMENT_DESC tElements[iNumElement];
};