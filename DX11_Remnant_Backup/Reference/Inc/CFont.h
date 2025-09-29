#pragma once

#include "CObject.h"

BEGIN(Engine)

class CFont final 
	: public CObject
{
private :
	CFont(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CFont() = default;

public :
	HRESULT Init(const _tchar* _pFontFilePath);
	HRESULT Render
	(
		const _tchar* _pText,
		const _float2& _vPosition, 
		_fvector _vColor = XMVectorSet(0.f, 1.f, 0.f, 1.f), 
		_float _fRotation = 0.f, 
		const _float2& _vOrigin = _float2(0.f, 0.f), 
		const _float _fScale = 1.f
	);


private :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

private :
	SpriteBatch* m_pBatch{};
	SpriteFont* m_pFont{};

public :
	static CFont* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext, 
		const _tchar* _pFontFilePath
	);
	virtual void Free() override;
};

END