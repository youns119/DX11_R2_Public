#include "pch.h"
#include "CTerrain_Test.h"

#include "CGameInstance.h"
#include "CLevel.h"
#include "CTerrainComponent.h"
#include "CNavigationComponent.h"
#include "CShader.h"
#include "CTexture.h"

#include "ELevel.h"

CTerrain_Test::CTerrain_Test(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CTerrain{ _pDevice, _pContext }
{
}

CTerrain_Test::CTerrain_Test(const CTerrain_Test& _Prototype)
	:  CTerrain(_Prototype)
{
}

HRESULT CTerrain_Test::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_Test::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FTerrain* pTerrainDesc = static_cast<FTerrain*>(_pDesc);
	pTerrainDesc->eRenderType = ERender::RENDER_NONBLEND;
	pTerrainDesc->iNumTexture = static_cast<_uint>(ETerrainTexture::TERRAINTEXTURE_END);
	pTerrainDesc->vScale = _float3(1.f, 1.f, 1.f);
	pTerrainDesc->vRotation = _float3(0.f, 0.f, 0.f);
	pTerrainDesc->vPosition = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Init(pTerrainDesc)))
		return E_FAIL;

	return S_OK;
}

void CTerrain_Test::Pre_Update(_float _fTimeDelta)
{
	//m_pNavigationComponent->SetUp_WorldMatrix(XMLoadFloat4x4(m_pTerrainComponent->Get_WorldMatrix_Ptr()));

	__super::Pre_Update(_fTimeDelta);
}

void CTerrain_Test::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CTerrain_Test::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CTerrain_Test::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_Test::Ready_Component()
{
	//// NavigationComponent
	//FNavigationComponent tNavigationComponent{};
	//tNavigationComponent.pOwner = this;

	//m_pNavigationComponent = static_cast<CNavigationComponent*>(__super::Add_ActorComponent
	//(
	//	static_cast<_uint>(ELevel::LEVEL_STATIC),
	//	g_strNavigationComponent,
	//	g_strNavigationComponent,
	//	&tNavigationComponent
	//));
	//if (m_pNavigationComponent == nullptr)
	//	return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_Test::Ready_Resource()
{
	// Shader
	if (FAILED(m_pTerrainComponent->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		L"Shader_VtxNorTex"
	)))
		return E_FAIL;

	// VIBuffer
	if(FAILED(m_pTerrainComponent->Add_VIBuffer
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("VIBuffer_Terrain_Test")
	)))
		return E_FAIL;

	// Texture_Diffuse
	if (FAILED(m_pTerrainComponent->Add_Texture
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		static_cast<_uint>(ETerrainTexture::TERRAINTEXTURE_DIFFUSE),
		TEXT("Texture_Terrain_Diffuse")
	)))
		return E_FAIL;

	//// Texture_Normal
	//if (FAILED(m_pTerrainComponent->Add_Texture
	//(
	//	static_cast<_uint>(ELevel::LEVEL_STAGE1),
	//	static_cast<_uint>(ETerrainTexture::TERRAINTEXTURE_NORMAL),
	//	TEXT("Texture_Terrain_Normal")
	//)))
	//	return E_FAIL;

	//// Texture_Mask
	//if (FAILED(m_pTerrainComponent->Add_Texture
	//(
	//	static_cast<_uint>(ELevel::LEVEL_STAGE1),
	//	static_cast<_uint>(ETerrainTexture::TERRAINTEXTURE_MASK),
	//	TEXT("Texture_Terrain_Mask")
	//)))
	//	return E_FAIL;

	//// Texture_Brush
	//if (FAILED(m_pTerrainComponent->Add_Texture
	//(
	//	static_cast<_uint>(ELevel::LEVEL_STAGE1),
	//	static_cast<_uint>(ETerrainTexture::TERRAINTEXTURE_BRUSH),
	//	TEXT("Texture_Terrain_Brush")
	//)))
	//	return E_FAIL;

	// Texture
	if (FAILED(m_pTerrainComponent->Bind_Textures
	(static_cast<_uint>(ETerrainTexture::TERRAINTEXTURE_DIFFUSE), "g_DiffuseTexture"))	/*||
	FAILED(m_pTerrainComponent->Bind_Texture
	(static_cast<_uint>(ETerrainTexture::TERRAINTEXTURE_NORMAL), "g_NormalTexture", 0))*/)
		return E_FAIL;

	return S_OK;
}

CTerrain_Test* CTerrain_Test::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CTerrain_Test* pInstance = new CTerrain_Test(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CTerrain_Test");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CTerrain_Test::Clone(void* _pDesc)
{
	CTerrain_Test* pInstance = new CTerrain_Test(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CTerrain_Test");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain_Test::Free()
{
	__super::Free();

	//Safe_Release(m_pNavigationComponent);
}