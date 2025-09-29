#include "CVIBuffer_Cell.h"

CVIBuffer_Cell::CVIBuffer_Cell(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CVIBuffer{ _pDevice, _pContext }
{
}

CVIBuffer_Cell::CVIBuffer_Cell(const CVIBuffer_Cell& _Prototype)
	: CVIBuffer(_Prototype)
{
}

HRESULT CVIBuffer_Cell::Init(const _float3* _pPoints)
{
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
	m_iNumVertexBuffer = 1;
	m_iNumVertice = 3;
	m_iVertexStride = sizeof(VTXPOS);
	m_iNumIndice = 4;
	m_iIndexStride = 2;


#pragma region VERTEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tBufferDesc.ByteWidth = m_iNumVertice * m_iVertexStride;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iVertexStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	VTXPOS* pVertices = new VTXPOS[m_iNumVertice];
	ZeroMemory(pVertices, sizeof(VTXPOS) * m_iNumVertice);

	memcpy(pVertices, _pPoints, sizeof(_float3) * 3);

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

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	pIndices[3] = 0;

	ZeroMemory(&m_tInitialData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tInitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Cell::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Cell* CVIBuffer_Cell::Create
(
	ID3D11Device* _pDevice, 
	ID3D11DeviceContext* _pContext, 
	const _float3* _pPoints
)
{
	CVIBuffer_Cell* pInstance = new CVIBuffer_Cell(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_pPoints)))
	{
		MSG_BOX("Failed To Create : CVIBuffer_Cell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CVIBuffer_Cell::Clone(void* _pDesc)
{
	CVIBuffer_Cell* pInstance = new CVIBuffer_Cell(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CVIBuffer_Cell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cell::Free()
{
	__super::Free();
}