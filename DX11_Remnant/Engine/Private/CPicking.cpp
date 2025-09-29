#include "CPicking.h"

#include "CGameInstance.h"

CPicking::CPicking(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CPicking::Init(HWND _hWnd)
{
	m_hWnd = _hWnd;

	D3D11_TEXTURE2D_DESC tTextureDesc{};
	D3D11_VIEWPORT tViewportDesc{};

	_uint iNumViewport{ 1 };

	m_pContext->RSGetViewports(&iNumViewport, &tViewportDesc);

	m_fViewportWidth = tViewportDesc.Width;
	m_fViewportHeight = tViewportDesc.Height;

	tTextureDesc.Width = static_cast<_uint>(tViewportDesc.Width);
	tTextureDesc.Height = static_cast<_uint>(tViewportDesc.Height);
	tTextureDesc.MipLevels = 1;
	tTextureDesc.ArraySize = 1;
	tTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	tTextureDesc.SampleDesc.Quality = 0;
	tTextureDesc.SampleDesc.Count = 1;
	tTextureDesc.Usage = D3D11_USAGE_STAGING;
	tTextureDesc.BindFlags = 0;
	tTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	tTextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&tTextureDesc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	return S_OK;
}

void CPicking::Update()
{
	CGameInstance::Copy_RT_Resource(TEXT("Target_PickDepth"), m_pTexture2D);
}

_bool CPicking::Compute_PickPos(_float3* _pOut)
{
	POINT ptMouse;

	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	D3D11_MAPPED_SUBRESOURCE tSubResource{};

	m_pContext->Map(m_pTexture2D, 0, D3D11_MAP_READ_WRITE, 0, &tSubResource);

	_uint iPixelIndex = ptMouse.y * static_cast<_uint>(m_fViewportWidth) + ptMouse.x;
	_float4 vDepthDesc = static_cast<_float4*>(tSubResource.pData)[iPixelIndex];

	m_pContext->Unmap(m_pTexture2D, 0);

	if (vDepthDesc.z == 0.f)
		return false;

	_vector	vWorldPos = XMVectorSet(static_cast<_float>(ptMouse.x), static_cast<_float>(ptMouse.y), 0.f, 0.f);

	vWorldPos = XMVectorSetX(vWorldPos, ptMouse.x / (m_fViewportWidth * 0.5f) - 1.f);
	vWorldPos = XMVectorSetY(vWorldPos, ptMouse.y / (m_fViewportHeight * -0.5f) + 1.f);
	vWorldPos = XMVectorSetZ(vWorldPos, vDepthDesc.x);
	vWorldPos = XMVectorSetW(vWorldPos, 1.f);

	vWorldPos = XMVector3TransformCoord(vWorldPos, CGameInstance::Get_TransformMatrix_Inverse(ED3DTransform::D3DTS_PROJ));
	vWorldPos = XMVector3TransformCoord(vWorldPos, CGameInstance::Get_TransformMatrix_Inverse(ED3DTransform::D3DTS_VIEW));

	XMStoreFloat3(_pOut, vWorldPos);

	return true;
}

CPicking* CPicking::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, HWND _hWnd)
{
	CPicking* pInstance = new CPicking(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_hWnd)))
	{
		MSG_BOX("Failed To Created : CPicking");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPicking::Free()
{
	__super::Free();

	Safe_Release(m_pTexture2D);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}