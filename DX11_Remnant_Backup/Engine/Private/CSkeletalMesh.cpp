#include "CSkeletalMesh.h"

#include "CSkeleton.h"

CSkeletalMesh::CSkeletalMesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CMesh{ _pDevice, _pContext }
{
}

CSkeletalMesh::CSkeletalMesh(const CSkeletalMesh& _Prototype)
	: CMesh(_Prototype)
	, m_vecBoneIndice{ _Prototype.m_vecBoneIndice }
	, m_vecOffset{ _Prototype.m_vecOffset }
{
}

HRESULT CSkeletalMesh::Init(const CSkeleton* _pSkeleton, HANDLE& _hFile, _ulong& _dwByte)
{
	m_iVertexStride = sizeof(VTXSKELETALMESH);

	if (FAILED(__super::Init(_hFile, _dwByte)))
		return E_FAIL;

#pragma region VERTEX_BUFFER

	VTXSKELETALMESH* pVertices = new VTXSKELETALMESH[m_iNumVertice];
	ZeroMemory(pVertices, sizeof(VTXSKELETALMESH) * m_iNumVertice);

	if (!ReadFile(_hFile, &m_iNumBone, sizeof(_uint), &_dwByte, nullptr))
		return E_FAIL;

	for (_uint i = 0; i < m_iNumBone; i++)
	{
		_char szName[MAX_PATH]{};
		if (!ReadFile(_hFile, &szName, sizeof(_char) * MAX_PATH, &_dwByte, nullptr))
			return E_FAIL;

		m_vecBoneIndice.push_back(_pSkeleton->Get_BoneIndex(szName));

		_float4x4 vecOffset{};
		if (!ReadFile(_hFile, &vecOffset, sizeof(_float4x4), &_dwByte, nullptr))
			return E_FAIL;

		m_vecOffset.push_back(vecOffset);
	}

	for (_uint j = 0; j < m_iNumVertice; ++j)
		if (!ReadFile(_hFile, &pVertices[j], sizeof(VTXSKELETALMESH), &_dwByte, nullptr))
			return E_FAIL;

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

HRESULT CSkeletalMesh::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

CSkeletalMesh* CSkeletalMesh::Create
(
	ID3D11Device* _pDevice,
	ID3D11DeviceContext* _pContext,
	const CSkeleton* _pSkeleton,
	HANDLE& _hFile,
	_ulong& _dwByte
)
{
	CSkeletalMesh* pInstance = new CSkeletalMesh(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_pSkeleton, _hFile, _dwByte)))
	{
		MSG_BOX("Failed To Create : CSkeletalMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CSkeletalMesh::Clone(void* _pDesc)
{
	CSkeletalMesh* pInstance = new CSkeletalMesh(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CSkeletalMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkeletalMesh::Free()
{
	__super::Free();
}