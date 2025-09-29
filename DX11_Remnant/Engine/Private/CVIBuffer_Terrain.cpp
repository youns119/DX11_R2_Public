#include "CVIBuffer_Terrain.h"

#include "CSceneComponent.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CVIBuffer{ _pDevice, _pContext }
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& _Prototype)
	: CVIBuffer(_Prototype)
	, m_iNumVerticeX{ _Prototype.m_iNumVerticeX }
	, m_iNumVerticeZ{ _Prototype.m_iNumVerticeZ }
{
	m_pVerticePos = new _float3[_Prototype.m_iNumVertice];
	memcpy(m_pVerticePos, _Prototype.m_pVerticePos, sizeof(_float3) * m_iNumVertice);
}

HRESULT CVIBuffer_Terrain::Init(const _tchar* _pHeightMapFile)
{
	_ulong dwByte{};
	HANDLE hFile = CreateFile(_pHeightMapFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	BITMAPFILEHEADER tFH{};
	BITMAPINFOHEADER tIH{};

	if (!ReadFile(hFile, &tFH, sizeof(BITMAPFILEHEADER), &dwByte, nullptr))
		return E_FAIL;
	if (!ReadFile(hFile, &tIH, sizeof(BITMAPINFOHEADER), &dwByte, nullptr))
		return E_FAIL;

	m_iNumVerticeX = tIH.biWidth;
	m_iNumVerticeZ = tIH.biHeight;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iNumVertexBuffer = 1;
	m_iNumVertice = m_iNumVerticeX * m_iNumVerticeZ;
	m_iVertexStride = sizeof(VTXNORTEX);
	m_iNumIndice = (m_iNumVerticeX - 1) * (m_iNumVerticeZ - 1) * 2 * 3;
	m_iIndexStride = 4;

	_uint* pPixels = new _uint[m_iNumVertice];
	if(!ReadFile(hFile, pPixels, sizeof(_uint) * m_iNumVertice, &dwByte, nullptr))
		return E_FAIL;

#pragma region VERTEX_BUFFER

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertice];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertice);

	m_pVerticePos = new _float3[m_iNumVertice];

	for (size_t i = 0; i < static_cast<size_t>(m_iNumVerticeZ); i++)
	{
		for (size_t j = 0; j < static_cast<size_t>(m_iNumVerticeX); j++)
		{
			_uint iIndex = static_cast<_uint>(i) * m_iNumVerticeX + static_cast<_uint>(j);

			pVertices[iIndex].vPosition = m_pVerticePos[iIndex] = 
				_float3(static_cast<_float>(j), (pPixels[iIndex] & 0x000000ff) / 30.f, static_cast<_float>(i));
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexCoord = _float2(j / (m_iNumVerticeX - 1.f), i / (m_iNumVerticeZ - 1.f));
		}
	}

#pragma endregion

#pragma region INDEX_BUFFER

	_uint* pIndices = new _uint[m_iNumIndice];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndice);

	_uint iNumIndice{};

	for (size_t i = 0; i < static_cast<size_t>(m_iNumVerticeZ - 1); i++)
	{
		for(size_t j = 0; j < static_cast<size_t>(m_iNumVerticeX - 1); j++)
		{
			_uint iIndex = static_cast<_uint>(i) * m_iNumVerticeX + static_cast<_uint>(j);

			_uint iIndices[4] =
			{
				iIndex + m_iNumVerticeX,
				iIndex + m_iNumVerticeX + 1,
				iIndex + 1,
				iIndex
			};

			_vector vSrc{}, vDst{}, vNorm{};

			pIndices[iNumIndice++] = iIndices[0];
			pIndices[iNumIndice++] = iIndices[1];
			pIndices[iNumIndice++] = iIndices[2];

			vSrc = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDst = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
			vNorm = XMVector3Normalize(XMVector3Cross(vSrc, vDst));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNorm);
			XMStoreFloat3(&pVertices[iIndices[1]].vNormal, XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNorm);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNorm);

			pIndices[iNumIndice++] = iIndices[0];
			pIndices[iNumIndice++] = iIndices[2];
			pIndices[iNumIndice++] = iIndices[3];

			vSrc = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDst = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
			vNorm = XMVector3Normalize(XMVector3Cross(vSrc, vDst));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNorm);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNorm);
			XMStoreFloat3(&pVertices[iIndices[3]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNorm);
		}
	}

	for (size_t i = 0; i < m_iNumVertice; i++)
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));

#pragma endregion
	
	ZeroMemory(&m_tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tBufferDesc.ByteWidth = m_iNumVertice * m_iVertexStride;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iVertexStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	ZeroMemory(&m_tInitialData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tInitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	ZeroMemory(&m_tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tBufferDesc.ByteWidth = m_iNumIndice * m_iIndexStride;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iIndexStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	ZeroMemory(&m_tInitialData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tInitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);
	Safe_Delete_Array(pPixels);

	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

_bool CVIBuffer_Terrain::Picking_Terrain
(
	CSceneComponent* _pSceneComponent, 
	_float3 _vRayDir, 
	_float3 _vRayPos, 
	_float3& _pOut
)
{
	_matrix	matWorldInverse = _pSceneComponent->Get_WorldMatrix_Inverse();

	_float3 vTempRayDir, vTempRayPos;
	XMVECTOR vRayDir, vRayPos;

	XMStoreFloat3(&vTempRayDir, XMVector3TransformNormal(XMLoadFloat3(&_vRayDir), matWorldInverse));
	XMStoreFloat3(&vTempRayPos, XMVector3TransformCoord(XMLoadFloat3(&_vRayPos), matWorldInverse));

	vRayPos = XMLoadFloat3(&vTempRayPos);
	vRayPos = XMVectorSetW(vRayPos, 1.f);
	vRayDir = XMLoadFloat3(&vTempRayDir);
	vRayDir = XMVector3Normalize(vRayDir);

	for (_uint i = 0; i < m_iNumVerticeZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticeX - 1; ++j)
		{
			_uint iIndex = i * m_iNumVerticeX + j;

			_uint iIndices[] = {
				iIndex + m_iNumVerticeX,
				iIndex + m_iNumVerticeX + 1,
				iIndex + 1,
				iIndex
			};

			//iIndices[0]이랑 iIndices[2]사이에 직선이 있거나
			//iIndices[1]이랑 iIndices[3]사이에 직선이 있으면 픽킹 체크 
			_float fTemp1 = (vTempRayDir.z * (m_pVerticePos[iIndices[0]].x - vTempRayPos.x) + vTempRayPos.z) / vTempRayDir.x - m_pVerticePos[iIndices[0]].z;
			_float fTemp2 = (vTempRayDir.z * (m_pVerticePos[iIndices[1]].x - vTempRayPos.x) + vTempRayPos.z) / vTempRayDir.x - m_pVerticePos[iIndices[1]].z;
			_float fTemp3 = (vTempRayDir.z * (m_pVerticePos[iIndices[2]].x - vTempRayPos.x) + vTempRayPos.z) / vTempRayDir.x - m_pVerticePos[iIndices[2]].z;
			_float fTemp4 = (vTempRayDir.z * (m_pVerticePos[iIndices[3]].x - vTempRayPos.x) + vTempRayPos.z) / vTempRayDir.x - m_pVerticePos[iIndices[3]].z;

			if (fTemp1 * fTemp3 < 0 && fTemp2 * fTemp4 < 0)
				continue;

			_float fDist;
			_matrix	matWorld = _pSceneComponent->Get_WorldMatrix();

			_vector vTemp_1 = XMLoadFloat3(&m_pVerticePos[iIndices[0]]);
			vTemp_1 = XMVectorSetW(vTemp_1, 1.f);
			_vector vTemp_2 = XMLoadFloat3(&m_pVerticePos[iIndices[1]]);
			vTemp_2 = XMVectorSetW(vTemp_2, 1.f);
			_vector vTemp_3 = XMLoadFloat3(&m_pVerticePos[iIndices[2]]);
			vTemp_3 = XMVectorSetW(vTemp_3, 1.f);

			if (true == TriangleTests::Intersects((FXMVECTOR)vRayPos, (FXMVECTOR)vRayDir, (FXMVECTOR)vTemp_1, (GXMVECTOR)vTemp_2, (HXMVECTOR)vTemp_3, fDist))
			{
				_vector	vPickPos = vRayPos + vRayDir * fDist;
				_vector vResult = XMVector3TransformCoord(vPickPos, matWorld);

				_float3 vOut{};
				XMStoreFloat3(&vOut, vResult);

				_pOut = vOut;
				//XMStoreFloat3(_pOut, vResult);

				return true;
			}

			vTemp_1 = XMLoadFloat3(&m_pVerticePos[iIndices[0]]);
			vTemp_1 = XMVectorSetW(vTemp_1, 1.f);
			vTemp_2 = XMLoadFloat3(&m_pVerticePos[iIndices[2]]);
			vTemp_2 = XMVectorSetW(vTemp_2, 1.f);
			vTemp_3 = XMLoadFloat3(&m_pVerticePos[iIndices[3]]);
			vTemp_3 = XMVectorSetW(vTemp_3, 1.f);
			if (true == TriangleTests::Intersects((FXMVECTOR)vRayPos, (FXMVECTOR)vRayDir, (FXMVECTOR)vTemp_1, (GXMVECTOR)vTemp_2, (HXMVECTOR)vTemp_3, fDist))
			{
				_vector	vPickPos = vRayPos + vRayDir * fDist;
				_vector vResult = XMVector3TransformCoord(vPickPos, matWorld);

				//XMStoreFloat3(_pOut, XMVector3TransformCoord(vPickPos, matWorld));

				_float3 vOut{};
				XMStoreFloat3(&vOut, vResult);

				_pOut = vOut;
				//XMStoreFloat3(_pOut, vResult);

				return true;
			}
		}
	}

	return false;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create
(
	ID3D11Device* _pDevice, 
	ID3D11DeviceContext* _pContext, 
	const _tchar* _pHeightMapFile
)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_pHeightMapFile)))
	{
		MSG_BOX("Faild To Create : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CVIBuffer_Terrain::Clone(void* _pDesc)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Faild To Create : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pVerticePos);
	m_pVerticePos = nullptr;
}