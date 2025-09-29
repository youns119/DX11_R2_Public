#include "CMesh.h"

CMesh::CMesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CVIBuffer{ _pDevice, _pContext }
{
}

CMesh::CMesh(const CMesh& _Prototype)
	: CVIBuffer(_Prototype)
	, m_iMaterialIndex{ _Prototype.m_iMaterialIndex }
{
	strcpy_s(m_szName, _Prototype.m_szName);
}

HRESULT CMesh::Init(HANDLE& _hFile, _ulong& _dwByte)
{
	if (!ReadFile(_hFile, &m_szName, sizeof(_char) * MAX_PATH, &_dwByte, nullptr)	||
		!ReadFile(_hFile, &m_iMaterialIndex, sizeof(_uint), &_dwByte, nullptr)		||
		!ReadFile(_hFile, &m_iNumVertice, sizeof(_uint), &_dwByte, nullptr)			||
		!ReadFile(_hFile, &m_iNumIndice, sizeof(_uint), &_dwByte, nullptr))
		return E_FAIL;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iNumVertexBuffer = 1;
	m_iNumIndice *= 3;
	m_iIndexStride = 4;

	ZeroMemory(&m_tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tBufferDesc.ByteWidth = m_iNumVertice * m_iVertexStride;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iVertexStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMesh::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMesh::Bind_Material(aiTextureType _eTextureType, const _char* _pConstantName, _uint _iTextureIndex)
{
	if (_eTextureType < 0 || _eTextureType >= AI_TEXTURE_TYPE_MAX)
		return E_FAIL;

	m_MeshMaterials[_eTextureType].bBinded = true;
	m_MeshMaterials[_eTextureType].pConstantName = _pConstantName;
	m_MeshMaterials[_eTextureType].iTextureIndex = _iTextureIndex;

	return S_OK;
}

HRESULT CMesh::Unbind_Material(aiTextureType _eTextureType)
{
	if (_eTextureType < 0 || _eTextureType >= AI_TEXTURE_TYPE_MAX)
		return E_FAIL;

	m_MeshMaterials[_eTextureType].bBinded = false;

	return S_OK;
}

HRESULT CMesh::Ready_IndexBuffer(HANDLE& _hFile, _ulong& _dwByte)
{
	ZeroMemory(&m_tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_tBufferDesc.ByteWidth = m_iNumIndice * m_iIndexStride;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.StructureByteStride = m_iIndexStride;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;

	_uint* pIndices = new _uint[m_iNumIndice];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndice);

	_uint iNumIndice{};

	for (size_t i = 0; i < m_iNumIndice / 3; i++)
		if (!ReadFile(_hFile, &pIndices[iNumIndice++], sizeof(_uint), &_dwByte, nullptr) ||
			!ReadFile(_hFile, &pIndices[iNumIndice++], sizeof(_uint), &_dwByte, nullptr) ||
			!ReadFile(_hFile, &pIndices[iNumIndice++], sizeof(_uint), &_dwByte, nullptr))
			return E_FAIL;

	ZeroMemory(&m_tInitialData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tInitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

void CMesh::Free()
{
	__super::Free();
}