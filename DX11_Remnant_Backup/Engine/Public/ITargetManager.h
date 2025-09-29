#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class CVIBuffer_Rect;
class CShader;
class CRenderTarget;

class ENGINE_DLL ITargetManager abstract
{
protected :
	virtual ~ITargetManager() = default;

public :
	virtual HRESULT Add_RenderTarget
	(
		const _wstring& _strRenderTargetTag,
		_uint _iWidth, _uint _iHeight,
		DXGI_FORMAT _ePixelFormat,
		const _float4& _vClearColor
	) PURE;
	virtual HRESULT Add_MRT(const _wstring& _strMRTTag, const _wstring& _strRenderTargetTag) PURE;
	virtual HRESULT Begin_MRT(const _wstring& _strMRTTag) PURE;
	virtual HRESULT End_MRT() PURE;

public :
	virtual HRESULT Bind_RT_ShaderResource
	(
		const _wstring& _strRenderTargetTag,
		CShader* _pShader,
		const _char* _pConstantName
	) PURE;
	virtual HRESULT Copy_RT_Resource(const _wstring& _strRenderTargetTag, ID3D11Texture2D* _pTexture2D) PURE;

#ifdef _DEBUG

public:
	virtual HRESULT Ready_RT_Debug
	(
		const _wstring& _strRenderTargetTag,
		_float _fX, _float _fY,
		_float _fSizeX, _float _fSizeY
	) PURE;
	virtual HRESULT Render_RT_Debug(const _wstring& _strMRTTag, CShader* _pShader, CVIBuffer_Rect* _pVIBuffer) PURE;

#endif
};

END