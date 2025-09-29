#include "CFont.h"

CFont::CFont(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CFont::Init(const _tchar* _pFontFilePath)
{
	m_pBatch = new SpriteBatch(m_pContext);
	m_pFont = new SpriteFont(m_pDevice, _pFontFilePath);

	return S_OK;
}

HRESULT CFont::Render
(
	const _tchar* _pText, 
	const _float2& _vPosition, 
	_fvector _vColor, 
	_float _fRotation, 
	const _float2& _vOrigin, 
	const _float _fScale
)
{
	if (m_pFont == nullptr || m_pBatch == nullptr)
		return E_FAIL;

	m_pBatch->Begin();

	m_pFont->DrawString(m_pBatch, _pText, _vPosition, _vColor, _fRotation, _vOrigin, _fScale);

	m_pBatch->End();

	return S_OK;
}

CFont* CFont::Create
(
	ID3D11Device* _pDevice, 
	ID3D11DeviceContext* _pContext, 
	const _tchar* _pFontFilePath
)
{
	CFont* pInstance = new CFont(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_pFontFilePath)))
	{
		MSG_BOX("Failed To Create : CFont");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFont::Free()
{
	__super::Free();

	Safe_Delete(m_pFont);
	Safe_Delete(m_pBatch);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}