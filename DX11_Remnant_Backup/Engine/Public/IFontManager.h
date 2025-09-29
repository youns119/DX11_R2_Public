#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class CFont;

class ENGINE_DLL IFontManager abstract
{
protected :
	virtual ~IFontManager() = default;

public:
	virtual HRESULT Render_Font
	(
		const _wstring& _strFontTag,
		const _tchar* _pText,
		const _float2& _vPosition,
		_fvector _vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f),
		_float _fRotation = 0.f,
		const _float2& _vOrigin = _float2(0.f, 0.f),
		const _float _fScale = 1.f
	) PURE;

public :
	virtual HRESULT Add_Font(const _wstring& _strFontTag, const _tchar* _pFontFilePath) PURE;
};

END