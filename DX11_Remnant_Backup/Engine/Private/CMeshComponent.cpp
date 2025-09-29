#include "CMeshComponent.h"

#include "CActor.h"
#include "CMesh.h"
#include "CMaterial.h"

CMeshComponent::CMeshComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPrimitiveComponent{ _pDevice, _pContext }
{
}

CMeshComponent::CMeshComponent(const CMeshComponent& _Prototype)
	: CPrimitiveComponent(_Prototype)
	, m_vecMesh{ _Prototype.m_vecMesh }
	, m_vecMaterial{ _Prototype.m_vecMaterial }
	, m_matPreTransform{ _Prototype.m_matPreTransform }
	, m_iNumMesh{ _Prototype.m_iNumMesh }
	, m_iNumMaterial{ _Prototype.m_iNumMaterial }
{
	for (auto& Mesh : m_vecMesh)
		Safe_AddRef(Mesh);

	for (auto& Material : m_vecMaterial)
		Safe_AddRef(Material);
}

HRESULT CMeshComponent::Init(const _tchar* _szModelName, _fmatrix _matPreTransform)
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshComponent::Bind_Material
(
	_uint _iMeshIndex, 
	aiTextureType _eMaterialType,
	const _char* _pConstantName,
	_uint _iTextureIndex
)
{
	if (_iMeshIndex >= m_iNumMesh ||
		FAILED(m_vecMesh[_iMeshIndex]->Bind_Material(_eMaterialType, _pConstantName, _iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshComponent::Unbind_Material(_uint _iMeshIndex, aiTextureType _eMaterialType)
{
	if (_iMeshIndex >= m_iNumMesh ||
		FAILED(m_vecMesh[_iMeshIndex]->Unbind_Material(_eMaterialType)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshComponent::Bind_MeshMaterial(const CMesh* _Mesh)
{
	for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
	{
		FMeshMaterial tMeshMaterial = _Mesh->Get_MaterialDesc(i);

		if (tMeshMaterial.bBinded)
		{
			_uint iMaterialIndex = _Mesh->Get_MaterialIndex();

			const _char* pConstantName = tMeshMaterial.pConstantName;
			_uint iTextureIndex = tMeshMaterial.iTextureIndex;

			if (FAILED(m_vecMaterial[iMaterialIndex]->Bind_Material
			(
				m_pShader,
				aiTextureType(i),
				pConstantName,
				iTextureIndex
			)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CMeshComponent::Ready_Material(HANDLE& _hFile, _ulong& _dwByte, const _tchar* _szModelName)
{
	if (!ReadFile(_hFile, &m_iNumMaterial, sizeof(_uint), &_dwByte, nullptr))
		return E_FAIL;

	for (size_t i = 0; i < m_iNumMaterial; i++)
	{
		CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, _hFile, _dwByte, _szModelName);
		if (pMaterial == nullptr)
			return E_FAIL;

		m_vecMaterial.push_back(pMaterial);
	}

	return S_OK;
}

void CMeshComponent::Free()
{
	__super::Free();

	for (auto& Mesh : m_vecMesh)
		Safe_Release(Mesh);

	for (auto& Material : m_vecMaterial)
		Safe_Release(Material);

	m_vecMesh.clear();
	m_vecMaterial.clear();
}