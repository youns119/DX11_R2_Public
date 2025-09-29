#pragma once

#include "Extractor_Define.h"

#include "FVertex_StaticMesh.h"
#include "FVertex_SkeletalMesh.h"
#include "FKeyFrame.h"

#include "EMesh.h"

class CModel final
{
private :
	CModel();
	~CModel() = default;

public :
	HRESULT Init(const _char* _szSrcPath, const _char* _szDstPath);

private :
	const aiScene* m_pAIScene{};
	Importer m_Importer{};

private :
	const _char* m_szSrcPath{};

	_uint m_iNumBone{};
	_uint m_iNumMesh{};
	_uint m_iNumMaterial{};
	_uint m_iNumAnimation{};

	EMesh m_eMesh = EMesh::MESH_END;

private :
	HRESULT MeshSorting();

private :
	HRESULT Save_Mesh(const _char* _szName, const _char* _szDstPath);
	void Save_Bone(HANDLE& _pFile, _ulong* _pByte, const aiNode* _pAINode);
	HRESULT Save_Material(const _char* _szName, const _char* _szDstPath);
	HRESULT Save_Animation(const _char* _szName, const _char* _szDstPath);

public :
	static CModel* Create(const _char* _szSrcPath, const _char* _szDstPath);
	void Free();
};