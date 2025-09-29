#include "CRenderTarget.h"

#include "CVIBuffer_Rect.h"
#include "CShader.h"

CRenderTarget::CRenderTarget(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderTarget::Init(_uint _iWidth, _uint _iHeight, DXGI_FORMAT _ePixelFormat, const _float4& _vClearColor)
{
	D3D11_TEXTURE2D_DESC tTextureDesc;
	ZeroMemory(&tTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	tTextureDesc.Width = _iWidth;
	tTextureDesc.Height = _iHeight;
	tTextureDesc.MipLevels = 1;
	tTextureDesc.ArraySize = 1;
	tTextureDesc.Format = _ePixelFormat;

	tTextureDesc.SampleDesc.Quality = 0;
	tTextureDesc.SampleDesc.Count = 1;

	tTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	tTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tTextureDesc.CPUAccessFlags = 0;
	tTextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&tTextureDesc, nullptr, &m_pTexture2D)) ||
		FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)) ||
		FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
		return E_FAIL;

	m_vClearColor = _vClearColor;

	return S_OK;
}

HRESULT CRenderTarget::Bind_ShaderResource(CShader* _pShader, const _char* _pConstantName)
{
	return _pShader->Bind_SRV(_pConstantName, m_pSRV);
}

HRESULT CRenderTarget::Copy_Resource(ID3D11Texture2D* _pTexture2D)
{
	m_pContext->CopyResource(_pTexture2D, m_pTexture2D);

	return S_OK;
}

void CRenderTarget::Clear()
{
	m_pContext->ClearRenderTargetView(m_pRTV, reinterpret_cast<_float*>(&m_vClearColor));
}

#ifdef _DEBUG

HRESULT CRenderTarget::Ready_Debug(_float _fX, _float _fY, _float _fSizeX, _float _fSizeY)
{
	_uint iNumViewport{ 1 };
	D3D11_VIEWPORT tViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewport, &tViewportDesc);

	XMStoreFloat4x4(&m_matWorld, XMMatrixScaling(_fSizeX, _fSizeY, 1.f));

	XMStoreFloat4
	(
		(_float4*)&m_matWorld.m[3],
		XMVectorSet(_fX - (tViewportDesc.Width * 0.5f), -_fY + (tViewportDesc.Height * 0.5f), 0.f, 1.f)
	);

	return S_OK;
}

HRESULT CRenderTarget::Render_Debug(CShader* _pShader, CVIBuffer_Rect* _pVIBuffer)
{
	_pShader->Bind_Matrix("g_matWorld", &m_matWorld);
	_pShader->Bind_SRV("g_Texture", m_pSRV);

	_pShader->Begin(0);

	_pVIBuffer->Render();

	return S_OK;
}

#endif

CRenderTarget* CRenderTarget::Create
(
	ID3D11Device* _pDevice,
	ID3D11DeviceContext* _pContext,
	_uint _iWidth, _uint _iHeight,
	DXGI_FORMAT _ePixelFormat,
	const _float4& _vClearColor
)
{
	CRenderTarget* pInstance = new CRenderTarget(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_iWidth, _iHeight, _ePixelFormat, _vClearColor)))
	{
		MSG_BOX("Failed to Created : CRenderTarget");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderTarget::Free()
{
	__super::Free();

	Safe_Release(m_pSRV);
	Safe_Release(m_pRTV);
	Safe_Release(m_pTexture2D);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}