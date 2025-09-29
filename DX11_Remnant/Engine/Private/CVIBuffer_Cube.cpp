#include "CVIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(ID3D11Device* _pDevice, ID3D11DeviceContext * _pContext)
	: CVIBuffer{ _pDevice, _pContext }
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube& _Prototype)
	: CVIBuffer(_Prototype)
{
}

HRESULT CVIBuffer_Cube::Init()
{
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iNumVertexBuffer = 1;
	m_iNumVertice = 8;
	m_iVertexStride = sizeof(VTXCUBE);
	m_iNumIndice = 36;
	m_iIndexStride = 2;

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tBufferDesc.ByteWidth = m_iNumVertice * m_iVertexStride;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iVertexStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	VTXCUBE* pVertices = new VTXCUBE[m_iNumVertice];
	ZeroMemory(pVertices, sizeof(VTXCUBE) * m_iNumVertice);

	// Front
	pVertices[0].vPosition = _float3(-1.f, 1.f, -1.f);
	pVertices[0].vTexcoord = pVertices[0].vPosition;

	pVertices[1].vPosition = _float3(1.f, 1.f, -1.f);
	pVertices[1].vTexcoord = pVertices[1].vPosition;

	pVertices[2].vPosition = _float3(1.f, -1.f, -1.f);
	pVertices[2].vTexcoord = pVertices[2].vPosition;

	pVertices[3].vPosition = _float3(-1.f, -1.f, -1.f);
	pVertices[3].vTexcoord = pVertices[3].vPosition;

	// Back
	pVertices[4].vPosition = _float3(-1.f, 1.f, 1.f);
	pVertices[4].vTexcoord = pVertices[4].vPosition;

	pVertices[5].vPosition = _float3(1.f, 1.f, 1.f);
	pVertices[5].vTexcoord = pVertices[5].vPosition;

	pVertices[6].vPosition = _float3(1.f, -1.f, 1.f);
	pVertices[6].vTexcoord = pVertices[6].vPosition;

	pVertices[7].vPosition = _float3(-1.f, -1.f, 1.f);
	pVertices[7].vTexcoord = pVertices[7].vPosition;


	ZeroMemory(&m_tInitialData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tInitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion


#pragma region INDEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tBufferDesc.ByteWidth = m_iNumIndice * m_iIndexStride;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iIndexStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	_ushort* pIndices = new _ushort[m_iNumIndice];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndice);

	pIndices[0] = 1; pIndices[1] = 5; pIndices[2] = 6;
	pIndices[3] = 1; pIndices[4] = 6; pIndices[5] = 2;

	pIndices[6] = 4; pIndices[7] = 0; pIndices[8] = 3;
	pIndices[9] = 4; pIndices[10] = 3; pIndices[11] = 7;


	pIndices[12] = 4; pIndices[13] = 5; pIndices[14] = 1;
	pIndices[15] = 4; pIndices[16] = 1; pIndices[17] = 0;


	pIndices[18] = 3; pIndices[19] = 2; pIndices[20] = 6;
	pIndices[21] = 3; pIndices[22] = 6; pIndices[23] = 7;


	pIndices[24] = 7; pIndices[25] = 6; pIndices[26] = 5;
	pIndices[27] = 7; pIndices[28] = 5; pIndices[29] = 4;


	pIndices[30] = 0; pIndices[31] = 1; pIndices[32] = 2;
	pIndices[33] = 0; pIndices[34] = 2; pIndices[35] = 3;

	ZeroMemory(&m_tInitialData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tInitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Cube::Init(void* _pDesc)
{
	return S_OK;
}

CVIBuffer_Cube* CVIBuffer_Cube::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Created : CVIBuffer_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CVIBuffer_Cube::Clone(void* _pDesc)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Created : CVIBuffer_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();
}