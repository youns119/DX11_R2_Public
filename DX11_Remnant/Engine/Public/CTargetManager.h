#pragma once

#include "ITargetManager.h"

#include "CObject.h"

BEGIN(Engine)

class CVIBuffer_Rect;
class CShader;
class CRenderTarget;

class ENGINE_DLL CTargetManager final 
	: public ITargetManager
	, public CObject
{
private :
	CTargetManager(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CTargetManager() = default;

public:
	HRESULT Init();

public :
	HRESULT Add_RenderTarget
	(
		const _wstring& _strRenderTargetTag, 
		_uint _iWidth, _uint _iHeight, 
		DXGI_FORMAT _ePixelFormat, 
		const _float4& _vClearColor
	);
	HRESULT Add_MRT(const _wstring& _strMRTTag, const _wstring& _strRenderTargetTag);
	HRESULT Begin_MRT(const _wstring& _strMRTTag);
	HRESULT End_MRT();

public :
	HRESULT Bind_RT_ShaderResource
	(
		const _wstring& _strRenderTargetTag, 
		CShader* _pShader, 
		const _char* _pConstantName
	);
	HRESULT Copy_RT_Resource(const _wstring& _strRenderTargetTag, ID3D11Texture2D* _pTexture2D);

#ifdef _DEBUG

public :
	HRESULT Ready_RT_Debug
	(
		const _wstring& _strRenderTargetTag,
		_float _fX, _float _fY, 
		_float _fSizeX, _float _fSizeY
	);
	HRESULT Render_RT_Debug(const _wstring& _strMRTTag, CShader* _pShader, CVIBuffer_Rect* _pVIBuffer);

#endif

private :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

private :
	ID3D11RenderTargetView* m_pBackRTV{};
	ID3D11DepthStencilView* m_pOriginalDSV{};

private :
	map<const _wstring, CRenderTarget*> m_vecRenderTarget;
	map<const _wstring, vector<CRenderTarget*>>	m_vecMRT;

private :
	CRenderTarget* Find_RenderTarget(const _wstring& _strRenderTargetTag);
	vector<CRenderTarget*>* Find_MRT(const _wstring& _strMRTTag);

public :
	static CTargetManager* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
};

END