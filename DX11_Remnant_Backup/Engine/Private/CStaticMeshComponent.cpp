#include "CStaticMeshComponent.h"

#include "CActor.h"
#include "CStaticMesh.h"
#include "CFileUtility.h"

CStaticMeshComponent::CStaticMeshComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CMeshComponent{ _pDevice, _pContext }
{
	m_strName = g_strStaticMeshComponent;
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& _Prototype)
	: CMeshComponent(_Prototype)
{
}

HRESULT CStaticMeshComponent::Init(const _tchar* _szModelName, _fmatrix _matPreTransform)
{
	XMStoreFloat4x4(&m_matPreTransform, _matPreTransform);

	_wstring strModelName = _wstring(_szModelName);

	vector<_wstring> vecPath;
	vector<_wstring> vecSrcPath;
	vecSrcPath.push_back(MESH + strModelName);
	vecSrcPath.push_back(MATERIAL + strModelName);
	vecSrcPath.push_back(ANIM + strModelName);

	// Mesh
	CFileUtility::TraverseDirectory(vecSrcPath[0], vecPath);

	_ulong dwByte{};
	HANDLE hFile = CreateFile(vecPath[0].c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	if (FAILED(Ready_Mesh(hFile, dwByte, _matPreTransform)))
		return E_FAIL;

	CloseHandle(hFile);
	vecPath.clear();

	// Material
	CFileUtility::TraverseDirectory(vecSrcPath[1], vecPath);

	dwByte = 0;
	hFile = CreateFile(vecPath[0].c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	if (FAILED(Ready_Material(hFile, dwByte, _szModelName)))
		return E_FAIL;

	CloseHandle(hFile);
	vecPath.clear();

	if (FAILED(__super::Init(_szModelName, _matPreTransform)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStaticMeshComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStaticMeshComponent::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	for (auto& Mesh : m_vecMesh)
	{
		if (FAILED(Bind_MeshMaterial(Mesh))		||
			FAILED(Begin_Shader())				||
			FAILED(Mesh->Render()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CStaticMeshComponent::Ready_Mesh(HANDLE& _hFile, _ulong& _dwByte, _fmatrix _matPreTransform)
{
	if (!ReadFile(_hFile, &m_iNumMesh, sizeof(_uint), &_dwByte, nullptr))
		return E_FAIL;

	for (size_t i = 0; i < m_iNumMesh; i++)
	{
		CStaticMesh* pMesh = CStaticMesh::Create
		(
			m_pDevice,
			m_pContext,
			_hFile,
			_dwByte,
			_matPreTransform
		);
		if (pMesh == nullptr)
			return E_FAIL;

		m_vecMesh.push_back(pMesh);
	}

	return S_OK;
}

CStaticMeshComponent* CStaticMeshComponent::Create
(
	ID3D11Device* _pDevice,
	ID3D11DeviceContext* _pContext,
	const _tchar* _szModelName,
	_fmatrix _matPreTransform
)
{
	CStaticMeshComponent* pInstance = new CStaticMeshComponent(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_szModelName, _matPreTransform)))
	{
		MSG_BOX("Failed To Create : CStaticMeshComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CStaticMeshComponent::Clone(void* _pDesc)
{
	CStaticMeshComponent* pInstance = new CStaticMeshComponent(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CStaticMeshComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaticMeshComponent::Free()
{
	__super::Free();
}