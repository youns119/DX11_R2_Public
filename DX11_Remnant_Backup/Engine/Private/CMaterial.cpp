#include "CMaterial.h"

#include "CShader.h"

CMaterial::CMaterial(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CMaterial::Init(HANDLE& _hFile, _ulong& _dwByte, const _tchar* _szModelName)
{
	for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
	{
		_uint iNumTexture{};
		if (!ReadFile(_hFile, &iNumTexture, sizeof(_uint), &_dwByte, nullptr))
			return E_FAIL;

		m_vecMtrlTexture->reserve(iNumTexture);

		for (size_t j = 0; j < iNumTexture; j++)
		{
			ID3D11ShaderResourceView* pSRV{};

			_char szTextureFileName[MAX_PATH] = "";
			if (!ReadFile(_hFile, szTextureFileName, sizeof(_char) * MAX_PATH, &_dwByte, nullptr))
				return E_FAIL;

			_char szFileName[MAX_PATH]{};
			_char szEXT[MAX_PATH]{};

			_splitpath_s(szTextureFileName, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);
			
			if(lstrcmp(_szModelName, L"Pino") && lstrcmp(_szModelName, L"Kena"))
				strcpy_s(szEXT, ".dds");

			_char szFullPath[MAX_PATH]{};

			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szEXT);

			_tchar szFinalPath[MAX_PATH]{};

			MultiByteToWideChar(CP_ACP, 0, szFullPath, static_cast<_int>(strlen(szFullPath)), szFinalPath, MAX_PATH);

			_wstring strFinalPath = _wstring(_szModelName);
			strFinalPath = MESHTEXTURE + strFinalPath + L"/" + szFinalPath;
			const _tchar* szTexturePath = strFinalPath.c_str();

			HRESULT hr{};

			if (strcmp(szEXT, ".dds") == false)
				hr = CreateDDSTextureFromFile(m_pDevice, szTexturePath, nullptr, &pSRV);
			else if (strcmp(szEXT, ".tga") == false)
				return E_FAIL;
			else
				hr = CreateWICTextureFromFile(m_pDevice, szTexturePath, nullptr, &pSRV);

			if (FAILED(hr))
				return E_FAIL;

			m_vecMtrlTexture[i].push_back(pSRV);
		}
	}

	return S_OK;
}

HRESULT CMaterial::Bind_Material
(
	CShader* _pShader, 
	aiTextureType _eTextureType,
	const _char* _pConstantName, 
	_uint _iTextureIndex
)
{
	if (_iTextureIndex >= m_vecMtrlTexture[_eTextureType].size() ||
		m_vecMtrlTexture[_eTextureType][_iTextureIndex] == nullptr)
		return S_OK;

	return _pShader->Bind_SRV(_pConstantName, m_vecMtrlTexture[_eTextureType][_iTextureIndex]);
}

CMaterial* CMaterial::Create
(
	ID3D11Device* _pDevice,
	ID3D11DeviceContext* _pContext,
	HANDLE& _hFile,
	_ulong& _dwByte,
	const _tchar* _szModelName
)
{
	CMaterial* pInstance = new CMaterial(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_hFile, _dwByte, _szModelName)))
	{
		MSG_BOX("Failed To Create : CMaterial");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMaterial::Free()
{
	__super::Free();

	for (auto& SRVs : m_vecMtrlTexture)
	{
		for (auto& SRV : SRVs)
			Safe_Release(SRV);

		SRVs.clear();
	}

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}