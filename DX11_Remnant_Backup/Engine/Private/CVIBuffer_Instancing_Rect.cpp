#include "CVIBuffer_Instancing_Rect.h"

#include "CMathUtility.h"

CVIBuffer_Instancing_Rect::CVIBuffer_Instancing_Rect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CVIBuffer_Instancing{ _pDevice, _pContext }
{
}

CVIBuffer_Instancing_Rect::CVIBuffer_Instancing_Rect(const CVIBuffer_Instancing_Rect& _Prototype)
	: CVIBuffer_Instancing(_Prototype)
{
}

HRESULT CVIBuffer_Instancing_Rect::Init(const FVIBuffer_Instancing* _pDesc)
{
	const FVIBuffer_Instancing_Rect* pDesc = static_cast<const FVIBuffer_Instancing_Rect*>(_pDesc);

	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iNumVertexBuffer = 2;
	m_iNumVertice = 4;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iIndexStride = 2;
	m_iInstanceVertexStride = sizeof(VTXINSTANCING);
	m_iNumIndexPerInstance = 6;
	m_iNumInstance = pDesc->iNumInstance;
	m_iNumIndice = m_iNumIndexPerInstance * m_iNumInstance;

#pragma region INSTANCE_BUFFER

	ZeroMemory(&m_tInstanceBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tInstanceBufferDesc.ByteWidth = m_iNumInstance * m_iInstanceVertexStride;
	m_tInstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_tInstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tInstanceBufferDesc.StructureByteStride = m_iInstanceVertexStride;
	m_tInstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_tInstanceBufferDesc.MiscFlags = 0;

	m_pInstanceVertices = new VTXINSTANCING[m_iNumInstance];
	ZeroMemory(m_pInstanceVertices, sizeof(VTXINSTANCING) * m_iNumInstance);

	m_pSpeeds = new _float[m_iNumInstance];
	ZeroMemory(m_pSpeeds, sizeof(_float) * m_iNumInstance);

	m_bLoop = pDesc->bLoop;

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_float4 vTranslation{};

		vTranslation.x = CMathUtility::Random_Float
		(pDesc->vCenter.x - pDesc->vRange.x * 0.5f, pDesc->vCenter.x + pDesc->vRange.x * 0.5f);
		vTranslation.y = CMathUtility::Random_Float
		(pDesc->vCenter.y - pDesc->vRange.y * 0.5f, pDesc->vCenter.y + pDesc->vRange.y * 0.5f);
		vTranslation.z = CMathUtility::Random_Float
		(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, pDesc->vCenter.z + pDesc->vRange.z * 0.5f);
		vTranslation.w = 1;

		m_pSpeeds[i] = CMathUtility::Random_Float(pDesc->vSpeed.x, pDesc->vSpeed.y);

		m_pInstanceVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		m_pInstanceVertices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		m_pInstanceVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		m_pInstanceVertices[i].vTranslation = vTranslation;
		m_pInstanceVertices[i].vLifeTime.x = CMathUtility::Random_Float(pDesc->vLifeTime.x, pDesc->vLifeTime.y);

		_float fScale = CMathUtility::Random_Float(pDesc->vSize.x, pDesc->vSize.y);

		XMStoreFloat4(&m_pInstanceVertices[i].vRight, XMLoadFloat4(&m_pInstanceVertices[i].vRight) * fScale);
		XMStoreFloat4(&m_pInstanceVertices[i].vUp, XMLoadFloat4(&m_pInstanceVertices[i].vUp) * fScale);
		XMStoreFloat4(&m_pInstanceVertices[i].vLook, XMLoadFloat4(&m_pInstanceVertices[i].vLook) * fScale);
	}

	m_tInstanceInitialData.pSysMem = m_pInstanceVertices;

#pragma endregion

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

	_uint iNumInstance{};

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pIndices[iNumInstance++] = 0;
		pIndices[iNumInstance++] = 1;
		pIndices[iNumInstance++] = 2;

		pIndices[iNumInstance++] = 0;
		pIndices[iNumInstance++] = 2;
		pIndices[iNumInstance++] = 3;
	}

	ZeroMemory(&m_tInitialData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tInitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Instancing_Rect::Init(void* _pDesc)
{
	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Instancing_Rect* CVIBuffer_Instancing_Rect::Create
(
	ID3D11Device* _pDevice, 
	ID3D11DeviceContext* _pContext, 
	const FVIBuffer_Instancing* _pDesc
)
{
	CVIBuffer_Instancing_Rect* pInstance = new CVIBuffer_Instancing_Rect(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CVIBuffer_Instancing_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CVIBuffer_Instancing_Rect::Clone(void* _pDesc)
{
	CVIBuffer_Instancing_Rect* pInstance = new CVIBuffer_Instancing_Rect(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CVIBuffer_Instancing_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Instancing_Rect::Free()
{
	__super::Free();
}