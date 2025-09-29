#include "CGraphicDevice.h"

CGraphicDevice::CGraphicDevice()
{
}

HRESULT CGraphicDevice::Init
(
	HWND _hWnd, 
	_bool _bWindowed,
	_uint _iWinSizeX, _uint _iWinSizeY,
	_Inout_ ID3D11Device** _ppDevice,
	_Inout_ ID3D11DeviceContext** _ppDeviceContext
)
{
	_uint iFlag{};

#ifdef _DEBUG

	iFlag = D3D11_CREATE_DEVICE_DEBUG;

#endif

	D3D_FEATURE_LEVEL tFeatureLV{};

	if (FAILED(D3D11CreateDevice
	(
		nullptr, 
		D3D_DRIVER_TYPE_HARDWARE, 
		0, iFlag, nullptr, 0,
		D3D11_SDK_VERSION, 
		&m_pDevice, 
		&tFeatureLV, 
		&m_pContext
	)
	))
		return E_FAIL;

	if (FAILED(Ready_SwapChain(_hWnd, _bWindowed, _iWinSizeX, _iWinSizeY))	||
		FAILED(Ready_BackBufferRenderTargetView())							||
		FAILED(Ready_DepthStencilView(_iWinSizeX, _iWinSizeY)))
		return E_FAIL;

	ID3D11RenderTargetView* pRTVs[1] = { m_pBackBufferRTV, };
	m_pContext->OMSetRenderTargets(1, pRTVs, m_pDepthStencilView);

	D3D11_VIEWPORT tViewPortDesc{};

	tViewPortDesc.TopLeftX = 0;
	tViewPortDesc.TopLeftY = 0;
	tViewPortDesc.Width = (_float)_iWinSizeX;
	tViewPortDesc.Height = (_float)_iWinSizeY;
	tViewPortDesc.MinDepth = 0.f;
	tViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &tViewPortDesc);

	*_ppDevice = m_pDevice;
	*_ppDeviceContext = m_pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	return S_OK;
}

HRESULT CGraphicDevice::Clear_BackBuffer_View(_float4 _vClearColor)
{
	if (m_pContext == nullptr)
		return E_FAIL;

	m_pContext->ClearRenderTargetView(m_pBackBufferRTV, (_float*)&_vClearColor);

	return S_OK;
}

HRESULT CGraphicDevice::Clear_DepthStencil_View()
{
	if (m_pContext == nullptr)
		return E_FAIL;

	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	return S_OK;
}

HRESULT CGraphicDevice::Present()
{
	if (m_pSwapChain == nullptr)
		return E_FAIL;

	return m_pSwapChain->Present(0, 0);
}


HRESULT CGraphicDevice::Ready_SwapChain(HWND _hWnd, _bool _bWindowed, _uint _iWinCX, _uint _iWinCY)
{
	IDXGIDevice* pDevice{};
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice);

	IDXGIAdapter* pAdapter{};
	pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);

	IDXGIFactory* pFactory{};
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	DXGI_SWAP_CHAIN_DESC tSwapChain{};
	ZeroMemory(&tSwapChain, sizeof(DXGI_SWAP_CHAIN_DESC));

	tSwapChain.BufferDesc.Width = _iWinCX;
	tSwapChain.BufferDesc.Height = _iWinCY;

	tSwapChain.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	tSwapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tSwapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	tSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tSwapChain.BufferCount = 1;

	tSwapChain.BufferDesc.RefreshRate.Numerator = 60;
	tSwapChain.BufferDesc.RefreshRate.Denominator = 1;

	tSwapChain.SampleDesc.Quality = 0;
	tSwapChain.SampleDesc.Count = 1;

	tSwapChain.OutputWindow = _hWnd;
	tSwapChain.Windowed = _bWindowed;
	tSwapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(pFactory->CreateSwapChain(m_pDevice, &tSwapChain, &m_pSwapChain)))
		return E_FAIL;

	Safe_Release(pFactory);
	Safe_Release(pAdapter);
	Safe_Release(pDevice);

	return S_OK;
}


HRESULT CGraphicDevice::Ready_BackBufferRenderTargetView()
{
	if (m_pDevice == nullptr)
		return E_FAIL;

	ID3D11Texture2D* pBackBufferTexture{};

	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture)) ||
		FAILED(m_pDevice->CreateRenderTargetView(pBackBufferTexture, nullptr, &m_pBackBufferRTV)))
		return E_FAIL;

	Safe_Release(pBackBufferTexture);

	return S_OK;
}

HRESULT CGraphicDevice::Ready_DepthStencilView(_uint _iWinCX, _uint _iWinCY)
{
	if (m_pDevice == nullptr)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture{};
	D3D11_TEXTURE2D_DESC tTextureDesc{};

	tTextureDesc.Width = _iWinCX;
	tTextureDesc.Height = _iWinCY;
	tTextureDesc.MipLevels = 1;
	tTextureDesc.ArraySize = 1;
	tTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	tTextureDesc.SampleDesc.Quality = 0;
	tTextureDesc.SampleDesc.Count = 1;

	tTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	tTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tTextureDesc.CPUAccessFlags = 0;
	tTextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&tTextureDesc, nullptr, &pDepthStencilTexture)) ||
		FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDepthStencilView)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}

CGraphicDevice* CGraphicDevice::Create
(
	HWND _hWnd, 
	_bool _bWindowed,
	_uint _iViewportWidht, 
	_uint _iViewportHeight, 
	_Inout_ ID3D11Device** _ppDevice,
	_Inout_ ID3D11DeviceContext** _ppDeviceContextOut)
{
	CGraphicDevice* pInstance = new CGraphicDevice();

	if (FAILED(pInstance->Init
	(
		_hWnd, 
		_bWindowed, 
		_iViewportWidht, 
		_iViewportHeight, 
		_ppDevice, 
		_ppDeviceContextOut
	)
	))
	{
		MSG_BOX("Failed to Created : CGraphicDevice");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGraphicDevice::Free()
{
	Safe_Release(m_pDepthStencilView);
	Safe_Release(m_pBackBufferRTV);

	Safe_Release(m_pSwapChain);

//#if defined(DEBUG) || defined(_DEBUG)
//
//	ID3D11Debug* pD3DDebug;
//	HRESULT hr = m_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&pD3DDebug));
//
//	if (SUCCEEDED(hr))
//	{
//		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
//		OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker \r ");
//		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
//
//		hr = pD3DDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
//
//		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
//		OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker END \r ");
//		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
//	}
//
//	if (pD3DDebug != nullptr) pD3DDebug->Release();
//
//#endif

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}