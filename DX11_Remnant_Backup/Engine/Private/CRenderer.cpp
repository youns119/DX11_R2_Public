#include "CRenderer.h"

#include "CGameInstance.h"
#include "CTargetManager.h"
#include "CPrimitiveComponent.h"
#include "CLightComponent.h"
#include "CShader.h"
#include "CVIBuffer_Rect.h"

CRenderer::CRenderer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderer::Init(CTargetManager* _pTargetManager)
{
	_uint iNumViewport{ 1 };
	D3D11_VIEWPORT tViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewport, &tViewportDesc);

	_uint iViewWidth = static_cast<_uint>(tViewportDesc.Width);
	_uint iViewHeight = static_cast<_uint>(tViewportDesc.Height);

	// Target_Diffuse
	if (FAILED(_pTargetManager->Add_RenderTarget
	(
		TEXT("Target_Diffuse"), 
		iViewWidth, iViewHeight,
		DXGI_FORMAT_R8G8B8A8_UNORM, 
		_float4(0.f, 0.f, 0.f, 0.f)
	)))
		return E_FAIL;

	// Target_Normal
	if (FAILED(_pTargetManager->Add_RenderTarget
	(
		TEXT("Target_Normal"), 
		iViewWidth, iViewHeight,
		DXGI_FORMAT_R16G16B16A16_UNORM, 
		_float4(0.f, 0.f, 0.f, 0.f)
	)))
		return E_FAIL;

	// Target_Depth
	if (FAILED(_pTargetManager->Add_RenderTarget
	(
		TEXT("Target_Depth"), 
		iViewWidth, iViewHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 
		_float4(0.f, 1000.f, 0.f, 0.f)
	)))
		return E_FAIL;

	// Target_PickDepth
	if (FAILED(_pTargetManager->Add_RenderTarget
	(
		TEXT("Target_PickDepth"), 
		iViewWidth, iViewHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 
		_float4(0.f, 0.f, 0.f, 0.f)
	)))
		return E_FAIL;

	// Target_Shade
	if (FAILED(_pTargetManager->Add_RenderTarget
	(
		TEXT("Target_Shade"), 
		iViewWidth, iViewHeight,
		DXGI_FORMAT_R16G16B16A16_UNORM, 
		_float4(0.f, 0.f, 0.f, 0.f)
	)))
		return E_FAIL;

	// Target_Specular
	if (FAILED(_pTargetManager->Add_RenderTarget
	(
		TEXT("Target_Specular"), 
		iViewWidth, iViewHeight,
		DXGI_FORMAT_R16G16B16A16_UNORM, 
		_float4(0.f, 0.f, 0.f, 0.f)
	)))
		return E_FAIL;

	// MRT_Component
	if (FAILED(_pTargetManager->Add_MRT(TEXT("MRT_Component"), TEXT("Target_Diffuse")))		||
		FAILED(_pTargetManager->Add_MRT(TEXT("MRT_Component"), TEXT("Target_Normal")))		||
		FAILED(_pTargetManager->Add_MRT(TEXT("MRT_Component"), TEXT("Target_Depth")))		||
		FAILED(_pTargetManager->Add_MRT(TEXT("MRT_Component"), TEXT("Target_PickDepth"))))
		return E_FAIL;

	// MRT_LightAcc
	if (FAILED(_pTargetManager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))) ||
		FAILED(_pTargetManager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;

	m_pShader = CShader::Create
	(
		m_pDevice, 
		m_pContext, 
		SHADER("Shader_Deferred.hlsl"), 
		VTXPOSTEX::tElements,
		VTXPOSTEX::iNumElement
	);
	if (m_pShader == nullptr)
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (m_pVIBuffer == nullptr)
		return E_FAIL;

	XMStoreFloat4x4(&m_matWorld, XMMatrixScaling(tViewportDesc.Width, tViewportDesc.Height, 1.f));
	XMStoreFloat4x4(&m_matView, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH(tViewportDesc.Width, tViewportDesc.Height, 0.f, 1.f));

#ifdef _DEBUG

	//if (FAILED(_pTargetManager->Ready_RT_Debug(TEXT("Target_Diffuse"), 100.f, 100.f, 200.f, 200.f))		||
	//	FAILED(_pTargetManager->Ready_RT_Debug(TEXT("Target_Normal"), 100.f, 300.f, 200.f, 200.f))		||
	//	FAILED(_pTargetManager->Ready_RT_Debug(TEXT("Target_Depth"), 100.f, 500.f, 200.f, 200.f))		||
	//	FAILED(_pTargetManager->Ready_RT_Debug(TEXT("Target_Shade"), 350.f, 150.f, 300.f, 300.f))		||
	//	FAILED(_pTargetManager->Ready_RT_Debug(TEXT("Target_Specular"), 350.f, 450.f, 300.f, 300.f)))
	//	return E_FAIL;

#endif

	return S_OK;
}

HRESULT CRenderer::Render()
{
	if (FAILED(Render_Priority()) ||
		FAILED(Render_NonBlend()) ||
		FAILED(Render_LightAcc()) ||
		FAILED(Render_Deferred()) ||
		FAILED(Render_NonLight()) ||
		FAILED(Render_Blend())	  ||
		FAILED(Render_UI()))
		return E_FAIL;

#ifdef _DEBUG

	if (FAILED(Render_Debug()))
		return E_FAIL;

#endif

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(ERender _eRender, CPrimitiveComponent* _pComponent)
{
	if (static_cast<_uint>(_eRender) >= static_cast<_uint>(ERender::RENDER_END) ||
		_pComponent == nullptr)
		return E_FAIL;

	m_listRender[static_cast<_uint>(_eRender)].push_back(_pComponent);
	Safe_AddRef(_pComponent);

	return S_OK;
}

HRESULT CRenderer::Add_Light(CLightComponent* _pComponent)
{
	if (_pComponent == nullptr)
		return E_FAIL;

	m_listLight.push_back(_pComponent);
	Safe_AddRef(_pComponent);

	return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderer::Add_DebugComponent(CPrimitiveComponent* _pComponent)
{
	m_listDebugComponent.push_back(_pComponent);
	Safe_AddRef(_pComponent);

	return S_OK;
}

#endif

HRESULT CRenderer::Render_Priority()
{
	for (auto& Component : m_listRender[static_cast<_uint>(ERender::RENDER_PRIORITY)])
	{
		if (FAILED(Component->Render()))
			return E_FAIL;

		Safe_Release(Component);
	}

	m_listRender[static_cast<_uint>(ERender::RENDER_PRIORITY)].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	// Diffuse + Normal
	if (FAILED(CGameInstance::Begin_MRT(TEXT("MRT_Component"))))
		return E_FAIL;

	for (auto& Component : m_listRender[static_cast<_uint>(ERender::RENDER_NONBLEND)])
	{
		if (FAILED(Component->Render()))
			return E_FAIL;

		Safe_Release(Component);
	}

	m_listRender[static_cast<_uint>(ERender::RENDER_NONBLEND)].clear();

	if (FAILED(CGameInstance::End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	// Shade
	if (FAILED(CGameInstance::Begin_MRT(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	if (FAILED(CGameInstance::Bind_RT_ShaderResource(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")) ||
		FAILED(CGameInstance::Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_matWorld", &m_matWorld))	||
		FAILED(m_pShader->Bind_Matrix("g_matView", &m_matView))		||
		FAILED(m_pShader->Bind_Matrix("g_matProj", &m_matProj)))
		return E_FAIL;

	_float4x4 matViewInv = CGameInstance::Get_TransformFloat4x4_Inverse(ED3DTransform::D3DTS_VIEW);
	_float4x4 matProjInv = CGameInstance::Get_TransformFloat4x4_Inverse(ED3DTransform::D3DTS_PROJ);
	_float4 vCamPos = CGameInstance::Get_CamPos();

	if (FAILED(m_pShader->Bind_Matrix("g_matViewInv", &matViewInv)) ||
		FAILED(m_pShader->Bind_Matrix("g_matProjInv", &matProjInv)) ||
		FAILED(m_pShader->Bind_RawValue("g_vCamPos", &vCamPos, sizeof(_float4))))
		return E_FAIL;

	for (auto& Component : m_listLight)
	{
		if(FAILED(Component->Render(m_pShader, m_pVIBuffer)))
			return E_FAIL;

		Safe_Release(Component);
	}

	m_listLight.clear();

	if (FAILED(CGameInstance::End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Deferred()
{
	if (FAILED(CGameInstance::Bind_RT_ShaderResource(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")) ||
		FAILED(CGameInstance::Bind_RT_ShaderResource(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture"))	 ||
		FAILED(CGameInstance::Bind_RT_ShaderResource(TEXT("Target_Specular"), m_pShader, "g_SpecularTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_matWorld", &m_matWorld)) ||
		FAILED(m_pShader->Bind_Matrix("g_matView", &m_matView)) ||
		FAILED(m_pShader->Bind_Matrix("g_matProj", &m_matProj)))
		return E_FAIL;

	m_pShader->Begin(3);

	m_pVIBuffer->Bind_InputAssembler();
	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	for (auto& Component : m_listRender[static_cast<_uint>(ERender::RENDER_NONLIGHT)])
	{
		if (FAILED(Component->Render()))
			return E_FAIL;

		Safe_Release(Component);
	}

	m_listRender[static_cast<_uint>(ERender::RENDER_NONLIGHT)].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	//m_listRender[static_cast<_uint>(ERender::RENDER_BLEND)].sort([](CActor* pSour, CActor* pDest)->_bool
	//	{
	//		return static_cast<CBlendObject*>(pSour)->Get_CamDistance() > static_cast<CBlendObject*>(pDest)->Get_CamDistance();
	//	});

	//for (auto& Actor : m_listRender[static_cast<_uint>(ERender::RENDER_BLEND)])
	//{
	//	if (FAILED(Actor->Render()))
	//		return E_FAIL;

	//	Safe_Release(Actor);
	//}

	//m_listRender[static_cast<_uint>(ERender::RENDER_BLEND)].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& Component : m_listRender[static_cast<_uint>(ERender::RENDER_UI)])
	{
		if (FAILED(Component->Render()))
			return E_FAIL;

		Safe_Release(Component);
	}

	m_listRender[static_cast<_uint>(ERender::RENDER_UI)].clear();

	return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderer::Render_Debug()
{
	if (FAILED(m_pShader->Bind_Matrix("g_matView", &m_matView)) ||
		FAILED(m_pShader->Bind_Matrix("g_matProj", &m_matProj)))
		return E_FAIL;

	if (FAILED(CGameInstance::Render_RT_Debug(TEXT("MRT_Component"), m_pShader, m_pVIBuffer)) ||
		FAILED(CGameInstance::Render_RT_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	for (auto& Component : m_listDebugComponent)
	{
		//if (Component != nullptr)
		//	Component->Render();

		Safe_Release(Component);
	}

	m_listDebugComponent.clear();

	return S_OK;
}

#endif

CRenderer* CRenderer::Create
(
	ID3D11Device* _pDevice,
	ID3D11DeviceContext* _pContext,
	CTargetManager* _pTargetManager
)
{
	CRenderer* pInstance = new CRenderer(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_pTargetManager)))
	{
		MSG_BOX("Failed To Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderer::Free()
{
	__super::Free();

#ifdef _DEBUG

	for (auto& Component : m_listDebugComponent)
	{
		if (Component != nullptr)
			Safe_Release(Component);
	}

	m_listDebugComponent.clear();

#endif


	for (auto& Components: m_listRender)
	{
		for (auto& Component : Components)
			Safe_Release(Component);

		Components.clear();
	}

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}