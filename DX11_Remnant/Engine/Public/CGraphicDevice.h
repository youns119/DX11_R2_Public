#pragma once

#include "IGraphicDevice.h"

#include "CObject.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDevice final
	: public IGraphicDevice
	, public CObject
{
private :
	CGraphicDevice();
	virtual ~CGraphicDevice() = default;

public :
	HRESULT Init
	(
		HWND _hWnd, 
		_bool _bWindowed, 
		_uint _iWinSizeX, _uint _iWinSizeY,
		_Inout_ ID3D11Device** _ppDevice, 
		_Inout_ ID3D11DeviceContext** _ppDeviceContext
	);

public :
	virtual HRESULT Clear_BackBuffer_View(_float4 _vClearColor) override;
	virtual HRESULT Clear_DepthStencil_View() override;

	virtual HRESULT Present() override;

private :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

	IDXGISwapChain* m_pSwapChain{};

	ID3D11RenderTargetView* m_pBackBufferRTV{};
	ID3D11DepthStencilView* m_pDepthStencilView{};

private :
	HRESULT Ready_SwapChain(HWND _hWnd, _bool _bWindowed, _uint _iWinCX, _uint _iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilView(_uint _iWinCX, _uint _iWinCY);

public :
	static CGraphicDevice* Create
	(
		HWND _hWnd,
		_bool _bWindowed,
		_uint _iViewportWidth,
		_uint _iViewportHeight,
		_Inout_ ID3D11Device** _ppDevice,
		_Inout_ ID3D11DeviceContext** _ppDeviceContextOut
	);
	virtual void Free() override;
};

END