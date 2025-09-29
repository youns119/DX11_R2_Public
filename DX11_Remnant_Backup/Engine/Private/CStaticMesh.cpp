#include "CStaticMesh.h"

CStaticMesh::CStaticMesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CMesh{ _pDevice, _pContext }
{
}

CStaticMesh::CStaticMesh(const CStaticMesh& _Prototype)
	: CMesh(_Prototype)
{
}

HRESULT CStaticMesh::Init(HANDLE& _hFile, _ulong& _dwByte, _fmatrix _matPreTransform)
{
	m_iVertexStride = sizeof(VTXSTATICMESH);

	if (FAILED(__super::Init(_hFile, _dwByte)))
		return E_FAIL;

#pragma region VERTEX_BUFFER

	VTXSTATICMESH* pVertices = new VTXSTATICMESH[m_iNumVertice];
	ZeroMemory(pVertices, sizeof(VTXSTATICMESH) * m_iNumVertice);

	for (size_t i = 0; i < m_iNumVertice; i++)
	{
		if (!ReadFile(_hFile, &pVertices[i], sizeof(VTXSTATICMESH), &_dwByte, nullptr))
			return E_FAIL;

		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), _matPreTransform));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), _matPreTransform));
	}

	ZeroMemory(&m_tInitialData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tInitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER

	if (FAILED(__super::Ready_IndexBuffer(_hFile, _dwByte)))
		return E_FAIL;

#pragma endregion

	return S_OK;
}

HRESULT CStaticMesh::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

CStaticMesh* CStaticMesh::Create
(
	ID3D11Device* _pDevice,
	ID3D11DeviceContext* _pContext,
	HANDLE& _hFile,
	_ulong& _dwByte,
	_fmatrix _matPreTransform
)
{
	CStaticMesh* pInstance = new CStaticMesh(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_hFile, _dwByte, _matPreTransform)))
	{
		MSG_BOX("Failed To Create : CStaticMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CStaticMesh::Clone(void* _pDesc)
{
	CStaticMesh* pInstance = new CStaticMesh(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CStaticMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaticMesh::Free()
{
	__super::Free();
}