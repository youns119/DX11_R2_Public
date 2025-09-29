#pragma once

#include "CObject.h"

BEGIN(Engine)

class CVIBuffer_Rect;
class CShader;

class CRenderTarget final 
	: public CObject
{
private :
	CRenderTarget(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CRenderTarget() = default;

public:
	HRESULT Init(_uint _iWidth, _uint _iHeight, DXGI_FORMAT _ePixelFormat, const _float4& _vClearColor);
	HRESULT Bind_ShaderResource(CShader* _pShader, const _char* _pConstantName);

public :
	ID3D11RenderTargetView* Get_RTV() const { return m_pRTV; }

public :
	HRESULT Copy_Resource(ID3D11Texture2D* _pTexture2D);
	void Clear();

#ifdef _DEBUG

public :
	HRESULT Ready_Debug(_float _fX, _float _fY, _float _fSizeX, _float _fSizeY);
	HRESULT Render_Debug(CShader* _pShader, CVIBuffer_Rect* _pVIBuffer);

#endif

private :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

private :
	ID3D11Texture2D* m_pTexture2D{};
	ID3D11RenderTargetView* m_pRTV{};
	ID3D11ShaderResourceView* m_pSRV{};

private :
	_float4	m_vClearColor{};

#ifdef _DEBUG

private :
	_float4x4 m_matWorld{};

#endif

public :
	static CRenderTarget* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext, 
		_uint _iWidth, _uint _iHeight, 
		DXGI_FORMAT _ePixelFormat, 
		const _float4& _vClearColor
	);
	virtual void Free() override;
};

END