#pragma once

#include "IFontManager.h"

#include "CObject.h"

BEGIN(Engine)

class CFont;

class ENGINE_DLL CFontManager final
	: public IFontManager
	, public CObject
{
private :
	CFontManager(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CFontManager() = default;

public :
	HRESULT Init();
	virtual HRESULT Render_Font
	(
		const _wstring& _strFontTag, 
		const _tchar* _pText,
		const _float2& _vPosition, 
		_fvector _vColor = XMVectorSet(0.f, 1.f, 0.f, 1.f),
		_float _fRotation = 0.f, 
		const _float2& _vOrigin = _float2(0.f, 0.f), 
		const _float _fScale = 1.f
	) override;

public :
	virtual HRESULT Add_Font(const _wstring& _strFontTag, const _tchar* _pFontFilePath) override;

private :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

private :
	map<const _wstring, CFont*>	m_vecFont;

private :
	CFont* Find_Font(const _wstring& _strFontTag);

public :
	static CFontManager* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
};

END