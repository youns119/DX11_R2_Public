#include "Model.h"

CModel::CModel()
{
}

HRESULT CModel::Init(const _char* _szSrcPath, const _char* _szDstPath)
{
	m_szSrcPath = _szSrcPath;

	_char szName[MAX_PATH] = "";
	_splitpath_s(m_szSrcPath, nullptr, 0, nullptr, 0, szName, MAX_PATH, nullptr, 0);

	cout << szName << "\n\n";

	if(FAILED(MeshSorting()))
		return E_FAIL;

	Save_Mesh(szName, _szDstPath);
	Save_Material(szName, _szDstPath);

	if(m_eMesh == EMesh::MESH_SKELETAL)
		Save_Animation(szName, _szDstPath);

	cout << "\n";

	return S_OK;
}

HRESULT CModel::MeshSorting()
{
	_uint iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	string strSrcPath = ConvertToUTF8(m_szSrcPath);

	m_pAIScene = m_Importer.ReadFile(strSrcPath, iFlag);
	if (m_pAIScene == nullptr)
	{
		std::cerr << "Assimp Error : " << m_Importer.GetErrorString() << std::endl;
		return E_FAIL;
	}

	_uint iNumMesh = m_pAIScene->mNumMeshes;
	_bool bHasBone{ false };

	for (_uint i = 0; i < iNumMesh; i++)
		if (m_pAIScene->mMeshes[i]->HasBones() == true)
			bHasBone = true;

	_bool bHasAnimation = m_pAIScene->HasAnimations();

	if (!bHasBone && !bHasAnimation)
	{
		m_Importer.FreeScene();
		m_eMesh = EMesh::MESH_STATIC;
	}
	else
	{
		m_eMesh = EMesh::MESH_SKELETAL;
		return S_OK;
	}

	iFlag |= aiProcess_PreTransformVertices;

	m_pAIScene = m_Importer.ReadFile(strSrcPath, iFlag);
	if (m_pAIScene == nullptr)
	{
		std::cerr << "Assimp Error : " << m_Importer.GetErrorString() << std::endl;
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Save_Mesh(const _char* _szName, const _char* _szDstPath)
{
	cout << "- " << _szName << " Mesh Extracting";

	_char szMeshPath[MAX_PATH]{};
	strcpy_s(szMeshPath, _szDstPath);
	strcat_s(szMeshPath, MESH);

	_char szMeshExt[MAX_PATH] = ".MESH";
	strcat_s(szMeshPath, _szName);

	string strMeshPath = string(szMeshPath);
	CreateDirectories(strMeshPath);

	strcat_s(szMeshPath, "/");
	strcat_s(szMeshPath, _szName);
	strcat_s(szMeshPath, szMeshExt);

	_tchar szFinalMeshPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, szMeshPath, static_cast<_int>(strlen(szMeshPath)), szFinalMeshPath, MAX_PATH);

	_ulong dwByte = 0;
	HANDLE hFile = CreateFile(szFinalMeshPath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	if(m_eMesh == EMesh::MESH_SKELETAL)
		Save_Bone(hFile, &dwByte, m_pAIScene->mRootNode);

	_uint m_iNumMeshes = m_pAIScene->mNumMeshes;
	WriteFile(hFile, &m_iNumMeshes, sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		aiMesh* pAIMesh = m_pAIScene->mMeshes[i];
		_uint iNumVertice = pAIMesh->mNumVertices;
		_uint iNumFace = pAIMesh->mNumFaces;

		WriteFile(hFile, &pAIMesh->mName.data, sizeof(_char) * MAX_PATH, &dwByte, nullptr);
		WriteFile(hFile, &pAIMesh->mMaterialIndex, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &iNumVertice, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &iNumFace, sizeof(_uint), &dwByte, nullptr);

		if (m_eMesh == EMesh::MESH_STATIC)
		{
			for (_uint j = 0; j < iNumVertice; ++j)
			{
				VTXSTATICMESH pVertices{};

				memcpy(&pVertices.vPosition, &pAIMesh->mVertices[j], sizeof(_float3));
				memcpy(&pVertices.vNormal, &pAIMesh->mNormals[j], sizeof(_float3));
				memcpy(&pVertices.vTexCoord, &pAIMesh->mTextureCoords[0][j], sizeof(_float2));
				memcpy(&pVertices.vTangent, &pAIMesh->mTangents[j], sizeof(_float3));

				WriteFile(hFile, &pVertices, sizeof(VTXSTATICMESH), &dwByte, nullptr);
			}
		}
		else if (m_eMesh == EMesh::MESH_SKELETAL)
		{
			VTXSKELETALMESH* pVertices = new VTXSKELETALMESH[iNumVertice];
			ZeroMemory(pVertices, sizeof(VTXSKELETALMESH) * iNumVertice);

			for (_uint j = 0; j < iNumVertice; ++j)
			{
				memcpy(&pVertices[j].vPosition, &pAIMesh->mVertices[j], sizeof(_float3));
				memcpy(&pVertices[j].vNormal, &pAIMesh->mNormals[j], sizeof(_float3));
				memcpy(&pVertices[j].vTexCoord, &pAIMesh->mTextureCoords[0][j], sizeof(_float2));
				memcpy(&pVertices[j].vTangent, &pAIMesh->mTangents[j], sizeof(_float3));
			}

			m_iNumBone = pAIMesh->mNumBones;
			_uint iNumBone{};

			vector<const _char*> vecName;
			vector<_float4x4> vecOffset;

			if (m_iNumBone == 0)
			{
				iNumBone++;
				_float4x4 matOffset{};

				XMStoreFloat4x4(&matOffset, XMMatrixIdentity());

				vecName.push_back(pAIMesh->mName.data);
				vecOffset.push_back(matOffset);
			}
			else
			{
				for (_uint j = 0; j < m_iNumBone; ++j)
				{
					aiBone* pAIBone = pAIMesh->mBones[j];
					_uint iNumWeight = pAIMesh->mBones[j]->mNumWeights;

					_float4x4 matOffset{};

					if (iNumWeight != 0)
					{
						memcpy(&matOffset, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
						XMStoreFloat4x4(&matOffset, XMMatrixTranspose(XMLoadFloat4x4(&matOffset)));

						vecName.push_back(pAIBone->mName.data);
						vecOffset.push_back(matOffset);
					}

					for (_uint k = 0; k < iNumWeight; k++)
					{
						aiVertexWeight tAIWeight = pAIBone->mWeights[k];
						_uint iBlendVertexID = tAIWeight.mVertexId;
						_float fBlendWeight = tAIWeight.mWeight;

						if (pVertices[iBlendVertexID].vBlendWeight.x == 0.f)
						{
							pVertices[iBlendVertexID].vBlendIndex.x = iNumBone;
							pVertices[iBlendVertexID].vBlendWeight.x = fBlendWeight;
						}
						else if (pVertices[iBlendVertexID].vBlendWeight.y == 0.f)
						{
							pVertices[iBlendVertexID].vBlendIndex.y = iNumBone;
							pVertices[iBlendVertexID].vBlendWeight.y = fBlendWeight;
						}
						else if (pVertices[iBlendVertexID].vBlendWeight.z == 0.f)
						{
							pVertices[iBlendVertexID].vBlendIndex.z = iNumBone;
							pVertices[iBlendVertexID].vBlendWeight.z = fBlendWeight;
						}
						else if (pVertices[iBlendVertexID].vBlendWeight.w == 0.f)
						{
							pVertices[iBlendVertexID].vBlendIndex.w = iNumBone;
							pVertices[iBlendVertexID].vBlendWeight.w = fBlendWeight;
						}
					}

					if (iNumWeight != 0)
						iNumBone++;
				}
			}

			m_iNumBone = iNumBone;

			WriteFile(hFile, &m_iNumBone, sizeof(_uint), &dwByte, nullptr);

			for (_uint i = 0; i < m_iNumBone; i++)
			{
				_char szName[MAX_PATH]{};
				strcpy_s(szName, vecName[i]);

				WriteFile(hFile, &szName, sizeof(_char)* MAX_PATH, &dwByte, nullptr);
				WriteFile(hFile, &vecOffset[i], sizeof(_float4x4), &dwByte, nullptr);
			}

			for (_uint j = 0; j < iNumVertice; ++j)
				WriteFile(hFile, &pVertices[j], sizeof(VTXSKELETALMESH), &dwByte, nullptr);

			Safe_Delete_Array(pVertices);
		}

		for (_uint j = 0; j < iNumFace; ++j)
		{
			WriteFile(hFile, &pAIMesh->mFaces[j].mIndices[0], sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &pAIMesh->mFaces[j].mIndices[1], sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &pAIMesh->mFaces[j].mIndices[2], sizeof(_uint), &dwByte, nullptr);
		}
	}

	CloseHandle(hFile);

	cout << " (Complete)\n";

	return S_OK;
}

void CModel::Save_Bone(HANDLE& _pFile, _ulong* _pByte, const aiNode* _pAINode)
{
	_float4x4 matTransform{};

	memcpy(&matTransform, &_pAINode->mTransformation, sizeof(_float4x4));
	XMStoreFloat4x4(&matTransform, XMMatrixTranspose(XMLoadFloat4x4(&matTransform)));

	WriteFile(_pFile, &_pAINode->mName.data, sizeof(_char) * MAX_PATH, _pByte, nullptr);
	WriteFile(_pFile, &matTransform, sizeof(_float4x4), _pByte, nullptr);

	WriteFile(_pFile, &_pAINode->mNumChildren, sizeof(int), _pByte, nullptr);

	for (size_t i = 0; i < _pAINode->mNumChildren; i++)
		Save_Bone(_pFile, _pByte, _pAINode->mChildren[i]);
}

HRESULT CModel::Save_Material(const _char* _szName, const _char* _szDstPath)
{
	cout << "- " << _szName << " Material Extracting";

	_char szMaterialPath[MAX_PATH]{};
	strcpy_s(szMaterialPath, _szDstPath);
	strcat_s(szMaterialPath, MATERIAL);

	_char szMaterialExt[MAX_PATH] = ".MATERIAL";
	strcat_s(szMaterialPath, _szName);

	string strMaterialPath = string(szMaterialPath);
	CreateDirectories(strMaterialPath);

	strcat_s(szMaterialPath, "/");
	strcat_s(szMaterialPath, _szName);
	strcat_s(szMaterialPath, szMaterialExt);

	_tchar szFinalMaterialPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, szMaterialPath, static_cast<_int>(strlen(szMaterialPath)), szFinalMaterialPath, MAX_PATH);

	_ulong dwByte = 0;
	HANDLE hFile = CreateFile(szFinalMaterialPath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	m_iNumMaterial = m_pAIScene->mNumMaterials;
	WriteFile(hFile, &m_iNumMaterial, sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < m_iNumMaterial; ++i)
	{
		aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			_uint iNumTexture = pAIMaterial->GetTextureCount(aiTextureType(j));
			WriteFile(hFile, &iNumTexture, sizeof(_uint), &dwByte, nullptr);

			_char szTextureFilePath[MAX_PATH]{};

			for (size_t k = 0; k < iNumTexture; k++)
			{
				aiString strTextureFilePath{};

				if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), static_cast<_uint>(k), &strTextureFilePath)))
				{
					strcpy_s(szTextureFilePath, "");
					continue;
				}

				_char szFileName[MAX_PATH]{};
				_char szEXT[MAX_PATH]{};
				_char szTextureFileName[MAX_PATH] = "";

				_splitpath_s(strTextureFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

				strcpy_s(szTextureFileName, szFileName);
				strcat_s(szTextureFileName, szEXT);

				WriteFile(hFile, szTextureFileName, sizeof(_char) * MAX_PATH, &dwByte, nullptr);
			}
		}
	}

	CloseHandle(hFile);

	cout << " (Complete)\n";

	return S_OK;
}

HRESULT CModel::Save_Animation(const _char* _szName, const _char* _szDstPath)
{
	cout << "- " << _szName << " Animation Extracting";

	_uint iNumAnimations = m_pAIScene->mNumAnimations;

	for (_uint i = 0; i < iNumAnimations; ++i)
	{
		aiAnimation* pAIAnimation = m_pAIScene->mAnimations[i];
		
		_char szAnimName[MAX_PATH]{};
		strcpy_s(szAnimName, pAIAnimation->mName.data);

		string strAnimName = string(szAnimName);
		ValidateFileName(strAnimName);
		strcpy_s(szAnimName, strAnimName.c_str());

		_char szAnimPath[MAX_PATH]{};
		strcpy_s(szAnimPath, _szDstPath);
		strcat_s(szAnimPath, ANIM);

		_char szAnimExt[MAX_PATH] = ".ANIM";
		strcat_s(szAnimPath, _szName);

		string strAnimPath = string(szAnimPath);
		CreateDirectories(strAnimPath);

		strcat_s(szAnimPath, "/");
		strcat_s(szAnimPath, _szName);
		strcat_s(szAnimPath, "_");
		strcat_s(szAnimPath, szAnimName);
		strcat_s(szAnimPath, szAnimExt);

		_tchar szFinalAnimPath[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, szAnimPath, static_cast<_int>(strlen(szAnimPath)), szFinalAnimPath, MAX_PATH);

		_ulong dwByte = 0;
		HANDLE hFile = CreateFile(szFinalAnimPath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == 0)
			return E_FAIL;

		_uint iNumChannel = pAIAnimation->mNumChannels;
		_float fDuration = static_cast<_float>(pAIAnimation->mDuration);
		_float fTickPerSecond = static_cast<_float>(pAIAnimation->mTicksPerSecond);

		WriteFile(hFile, &szAnimName, sizeof(_char) * MAX_PATH, &dwByte, nullptr);
		WriteFile(hFile, &iNumChannel, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &fDuration, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &fTickPerSecond, sizeof(_float), &dwByte, nullptr);

		for (_uint j = 0; j < iNumChannel; ++j)
		{
			aiNodeAnim* pAIChannel = pAIAnimation->mChannels[j];

			WriteFile(hFile, &pAIChannel->mNodeName.data, sizeof(_char) * MAX_PATH, &dwByte, nullptr);

			_uint iNumScalingKeys = pAIChannel->mNumScalingKeys;
			_uint iNumRotationKeys = pAIChannel->mNumRotationKeys;
			_uint iNumPositionKeys = pAIChannel->mNumPositionKeys;

			_uint iNumKeyFrames = max(iNumScalingKeys, iNumRotationKeys);
			iNumKeyFrames = max(iNumKeyFrames, iNumPositionKeys);

			WriteFile(hFile, &iNumKeyFrames, sizeof(_uint), &dwByte, nullptr);

			for (_uint k = 0; k < iNumKeyFrames; ++k)
			{
				FKeyFrame tKeyFrame{};
				ZeroMemory(&tKeyFrame, sizeof(FKeyFrame));

				if (iNumScalingKeys > k)
				{
					memcpy(&tKeyFrame.vScale, &pAIChannel->mScalingKeys[k].mValue, sizeof(_float3));

					tKeyFrame.fTrackPosition = static_cast<_float>(pAIChannel->mScalingKeys[k].mTime);
				}
				if (iNumRotationKeys > k)
				{
					memcpy(&tKeyFrame.vRotation.x, &pAIChannel->mRotationKeys[k].mValue.x, sizeof(_float));
					memcpy(&tKeyFrame.vRotation.y, &pAIChannel->mRotationKeys[k].mValue.y, sizeof(_float));
					memcpy(&tKeyFrame.vRotation.z, &pAIChannel->mRotationKeys[k].mValue.z, sizeof(_float));
					memcpy(&tKeyFrame.vRotation.w, &pAIChannel->mRotationKeys[k].mValue.w, sizeof(_float));

					tKeyFrame.fTrackPosition = static_cast<_float>(pAIChannel->mRotationKeys[k].mTime);
				}
				if (iNumPositionKeys > k)
				{
					memcpy(&tKeyFrame.vTranslation, &pAIChannel->mPositionKeys[k].mValue, sizeof(_float3));

					tKeyFrame.fTrackPosition = static_cast<_float>(pAIChannel->mPositionKeys[k].mTime);
				}

				WriteFile(hFile, &tKeyFrame, sizeof(FKeyFrame), &dwByte, nullptr);
			}
		}

		CloseHandle(hFile);
	}

	cout << " (Complete)\n";

	return S_OK;
}

CModel* CModel::Create(const _char* _szSrcPath, const _char* _szDstPath)
{
	CModel* pInstance = new CModel();

	if (FAILED(pInstance->Init( _szSrcPath, _szDstPath)))
	{
		MSG_BOX("Failed To Create : CModel");
		Safe_Delete(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	m_Importer.FreeScene();

	delete this;
}