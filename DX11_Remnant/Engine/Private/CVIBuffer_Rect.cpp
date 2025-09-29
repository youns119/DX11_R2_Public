#include "CVIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CVIBuffer{ _pDevice, _pContext }
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect& _Prototype)
	: CVIBuffer(_Prototype)
{
}

HRESULT CVIBuffer_Rect::Init()
{
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iNumVertexBuffer = 1;
	m_iNumVertice = 4;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iNumIndice = 6;
	m_iIndexStride = 2;

#pragma region VERTEX_BUFFER

	ZeroMemory(&m_tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tBufferDesc.ByteWidth = m_iNumVertice * m_iVertexStride;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iVertexStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertice];
	ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertice);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexCoord = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexCoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexCoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexCoord = _float2(0.f, 1.f);

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
	pIndices[4] = 2;
	pIndices[5] = 3;

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

HRESULT CVIBuffer_Rect::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Rect* CVIBuffer_Rect::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Created : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CVIBuffer_Rect::Clone(void* _pDesc)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Created : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();
}