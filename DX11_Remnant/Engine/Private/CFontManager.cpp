#include "CFontManager.h"

#include "CFont.h"

CFontManager::CFontManager(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CFontManager::Init()
{
	return S_OK;
}

HRESULT CFontManager::Render_Font
(
	const _wstring& _strFontTag, 
	const _tchar* _pText, 
	const _float2& _vPosition, 
	_fvector _vColor, 
	_float _fRotation, 
	const _float2& _vOrigin,
	const _float _fScale
)
{
	CFont* pFont = Find_Font(_strFontTag);
	if (pFont == nullptr)
		return E_FAIL;

	return pFont->Render(_pText, _vPosition, _vColor, _fRotation, _vOrigin, _fScale);
}

HRESULT CFontManager::Add_Font(const _wstring& _strFontTag, const _tchar* _pFontFilePath)
{
	if (Find_Font(_strFontTag) != nullptr)
		return E_FAIL;

	CFont* pFont = CFont::Create(m_pDevice, m_pContext, _pFontFilePath);
	if (pFont == nullptr)
		return E_FAIL;

	m_vecFont.emplace(_strFontTag, pFont);

	return S_OK;
}

CFont* CFontManager::Find_Font(const _wstring& _strFontTag)
{
	auto iter = m_vecFont.find(_strFontTag);
	if (iter == m_vecFont.end())
		return nullptr;

	return iter->second;
}

CFontManager* CFontManager::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CFontManager* pInstance = new CFontManager(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed to Created : CFont_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFontManager::Free()
{
	__super::Free();

	for (auto& Font : m_vecFont)
		Safe_Release(Font.second);

	m_vecFont.clear();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}