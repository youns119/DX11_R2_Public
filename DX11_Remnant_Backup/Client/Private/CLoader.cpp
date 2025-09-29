#include "pch.h"
#include "CLoader.h"

#include "CGameInstance.h"

#include "CStaticMeshComponent.h"
#include "CSkeletalMeshComponent.h"
#include "CTerrainComponent.h"
#include "CColliderComponent.h"
#include "CParticleComponent.h"
#include "CCubeComponent.h"
#include "CUIComponent.h"
#include "CCameraComponent.h"
#include "CSpringArmComponent.h"
#include "CLightComponent.h"
#include "CCharacterMovementComponent.h"
#include "CNavigationComponent.h"

#include "CVIBuffer_Rect.h"
#include "CVIBuffer_Terrain.h"
#include "CVIBuffer_Cube.h"
#include "CVIBuffer_Instancing_Rect.h"
#include "CVIBuffer_Instancing_Point.h"
#include "CStaticMesh.h"
#include "CSkeletalMesh.h"
#include "CShader.h"
#include "CTexture.h"

#include "CPlayer.h"
#include "CMonster_Brute.h"
#include "CMonster_ManyFaces.h"
#include "CPino.h"
#include "CAssaultRifle.h"
#include "CBuilding_A.h"
#include "CCamera_Free.h"
#include "CTerrain_Test.h"
#include "CSky_Default.h"
#include "CSky_Stage1.h"
#include "CCrossHair.h"
#include "CPlayerHP.h"
#include "CWeaponBox.h"
#include "CPlayerUI.h"
#include "CMonsterHP.h"
#include "CMonsterUI.h"
#include "CLoadingUI.h"
#include "CLight_Directional.h"

#include "ELevel.h"

_bool CLoader::m_bLoaded[static_cast<_uint>(ELevel::LEVEL_END)]{};

CLoader::CLoader(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

static _uint APIENTRY Thread_Main(void* _pArg)
{
	CLoader* pLoader = static_cast<CLoader*>(_pArg);

	pLoader->Start_Loading();

	return 0;
}

HRESULT CLoader::Init(ELevel _eNextLevel)
{
	m_eNextLevel = _eNextLevel;

	InitializeCriticalSection(&m_tCriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (m_hThread == 0)
	{
		MSG_BOX("Failed To Initialize Thread");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLoader::Start_Loading()
{
	EnterCriticalSection(&m_tCriticalSection);

	HRESULT hr = CoInitializeEx(nullptr, 0);
	if (FAILED(hr))
	{
		LeaveCriticalSection(&m_tCriticalSection);
		return E_FAIL;
	}

	switch (m_eNextLevel)
	{
	case ELevel::LEVEL_LOGO :
		hr = Loading_For_Level_Logo();
		hr = Loading_For_Level_Stage1();
		break;
	case ELevel::LEVEL_STAGE1 :
		hr = Loading_For_Level_Stage1();
		break;
	default :
		hr = Loading_For_Level_Logo();
		hr = Loading_For_Level_Stage1();
		hr = Loading_For_Level_Terrain();
		break;
	}

	LeaveCriticalSection(&m_tCriticalSection);

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

void CLoader::SetUp_WindowText() const
{
	SetWindowText(g_hWnd, m_szLoadingText);
}

HRESULT CLoader::Loading_For_Level_Logo()
{
	if (m_bLoaded[static_cast<_uint>(ELevel::LEVEL_LOGO)])
	{
		lstrcpyW(m_szLoadingText, TEXT("Stage1 Resource Loaded"));

		m_bFinished = true;
		return S_OK;
	}

	//////// Shader /////////
	{
		// Shader_VtxPosTex
		lstrcpyW(m_szLoadingText, TEXT("Shader_VtxPosTex Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			TEXT("Shader_VtxPosTex"),
			CShader::Create
			(
				m_pDevice,
				m_pContext,
				SHADER("Shader_VtxPosTex.hlsl"),
				VTXPOSTEX::tElements,
				VTXPOSTEX::iNumElement
			)
		)))
			return E_FAIL;

		// Shader_VtxNorTex
		lstrcpyW(m_szLoadingText, TEXT("Shader_VtxNorTex Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			TEXT("Shader_VtxNorTex"),
			CShader::Create
			(
				m_pDevice,
				m_pContext,
				SHADER("Shader_VtxNorTex.hlsl"),
				VTXNORTEX::tElements,
				VTXNORTEX::iNumElement
			)
		)))
			return E_FAIL;

		// Shader_VtxCube
		lstrcpyW(m_szLoadingText, TEXT("Shader_VtxCube Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			TEXT("Shader_VtxCube"),
			CShader::Create
			(
				m_pDevice,
				m_pContext,
				SHADER("Shader_VtxCube.hlsl"),
				VTXCUBE::tElements,
				VTXCUBE::iNumElement
			)
		)))
			return E_FAIL;

		// Shader_VtxStaticMesh
		lstrcpyW(m_szLoadingText, TEXT("Shader_VtxStaticMesh Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			TEXT("Shader_VtxStaticMesh"),
			CShader::Create
			(
				m_pDevice,
				m_pContext,
				SHADER("Shader_VtxStaticMesh.hlsl"),
				VTXSTATICMESH::tElements,
				VTXSTATICMESH::iNumElement
			)
		)))
			return E_FAIL;

		// Shader_VtxSkeletalMesh
		lstrcpyW(m_szLoadingText, TEXT("Shader_VtxSkeletalMesh Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			TEXT("Shader_VtxSkeletalMesh"),
			CShader::Create
			(
				m_pDevice,
				m_pContext,
				SHADER("Shader_VtxSkeletalMesh.hlsl"),
				VTXSKELETALMESH::tElements,
				VTXSKELETALMESH::iNumElement
			)
		)))
			return E_FAIL;

		// Shader_VtxPosTexInstancing
		lstrcpyW(m_szLoadingText, TEXT("Shader_VtxPosTexInstancing Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			TEXT("Shader_VtxPosTexInstancing"),
			CShader::Create
			(
				m_pDevice,
				m_pContext,
				SHADER("Shader_VtxPosTexInstancing.hlsl"),
				VTXINSTANCINGRECT::tElements,
				VTXINSTANCINGRECT::iNumElement
			)
		)))
			return E_FAIL;

		// Shader_VtxPointInstancing
		lstrcpyW(m_szLoadingText, TEXT("Shader_VtxPointInstancing Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			TEXT("Shader_VtxPointInstancing"),
			CShader::Create
			(
				m_pDevice,
				m_pContext,
				SHADER("Shader_VtxPointInstancing.hlsl"),
				VTXINSTANCINGPOINT::tElements,
				VTXINSTANCINGPOINT::iNumElement
			)
		)))
			return E_FAIL;
	}

	//////// VIBuffer /////////
	{
		//// VIBuffer_Rect
		//lstrcpyW(m_szLoadingText, TEXT("VIBuffer_Rect Loading"));
		//if (FAILED(CGameInstance::Add_Prototype
		//(
		//	static_cast<_uint>(ELevel::LEVEL_STATIC),
		//	TEXT("VIBuffer_Rect"),
		//	CVIBuffer_Rect::Create(m_pDevice, m_pContext)
		//)))
		//	return E_FAIL;

		//// VIBuffer_Cube
		//lstrcpyW(m_szLoadingText, TEXT("VIBuffer_Cube Loading"));
		//if (FAILED(CGameInstance::Add_Prototype
		//(
		//	static_cast<_uint>(ELevel::LEVEL_STATIC),
		//	TEXT("VIBuffer_Cube"),
		//	CVIBuffer_Cube::Create(m_pDevice, m_pContext)
		//)))
		//	return E_FAIL;
	}

	//////// Texture /////////
	{
		// Texture_Logo
		lstrcpyW(m_szLoadingText, TEXT("Texture_Logo Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_LOGO),
			TEXT("Texture_Logo"),
			CTexture::Create(m_pDevice, m_pContext, TEXTURE("UI/Logo.png"), 1)
		)))
			return E_FAIL;
	}

	//////// Component /////////
	{
		// Component_Scene
		lstrcpyW(m_szLoadingText, TEXT("Component_Scene Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			g_strSceneComponent,
			CSceneComponent::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Component_Terrain
		lstrcpyW(m_szLoadingText, TEXT("Component_Terrain Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			g_strTerrainComponent,
			CTerrainComponent::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Component_Collider_AABB
		lstrcpyW(m_szLoadingText, TEXT("Component_Collider_AABB Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			g_strColliderComponent_AABB,
			CColliderComponent::Create(m_pDevice, m_pContext, ECollider::COLLIDER_AABB)
		)))
			return E_FAIL;

		// Component_Collider_OBB
		lstrcpyW(m_szLoadingText, TEXT("Component_Collider_OBB Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			g_strColliderComponent_OBB,
			CColliderComponent::Create(m_pDevice, m_pContext, ECollider::COLLIDER_OBB)
		)))
			return E_FAIL;

		// Component_Collider_Sphere
		lstrcpyW(m_szLoadingText, TEXT("Component_Collider_Sphere Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			g_strColliderComponent_Sphere,
			CColliderComponent::Create(m_pDevice, m_pContext, ECollider::COLLIDER_SPHERE)
		)))
			return E_FAIL;

		// Component_Particle
		lstrcpyW(m_szLoadingText, TEXT("Component_Particle Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			g_strParticleComponent,
			CParticleComponent::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Component_Cube
		lstrcpyW(m_szLoadingText, TEXT("Component_Cube Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			g_strCubeComponent,
			CCubeComponent::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Component_UI
		lstrcpyW(m_szLoadingText, TEXT("Component_UI Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			g_strUIComponent,
			CUIComponent::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Component_Camera
		lstrcpyW(m_szLoadingText, TEXT("Component_Camera Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			g_strCameraComponent,
			CCameraComponent::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Component_SpringArm
		lstrcpyW(m_szLoadingText, TEXT("Component_SpringArm Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			g_strSpringArmComponent,
			CSpringArmComponent::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Component_Light
		lstrcpyW(m_szLoadingText, TEXT("Component_Light Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			g_strLightComponent,
			CLightComponent::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Component_Movement
		lstrcpyW(m_szLoadingText, TEXT("Component_Movement Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			g_strMovementComponent,
			CMovementComponent::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Component_CharacterMovement
		lstrcpyW(m_szLoadingText, TEXT("Component_CharacterMovement Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STATIC),
			g_strCharacterMovementComponent,
			CCharacterMovementComponent::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		//// Component_Navigation
		//lstrcpyW(m_szLoadingText, TEXT("Component_Navigation Loading"));
		//if (FAILED(CGameInstance::Add_Prototype
		//(
		//	static_cast<_uint>(ELevel::LEVEL_STATIC),
		//	g_strNavigationComponent,
		//	CNavigationComponent::Create(m_pDevice, m_pContext, RESOURCE("DataFile/Navigation.dat"))
		//)))
		//	return E_FAIL;
	}

	// Actor
	{
		// Actor_LoadingUI
		lstrcpyW(m_szLoadingText, TEXT("Actor_LoadingUI Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_LOGO),
			TEXT("Actor_LoadingUI"),
			CLoadingUI::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;
	}

	lstrcpyW(m_szLoadingText, TEXT("Logo Resource Loaded"));

	m_bFinished = true;
	m_bLoaded[static_cast<_uint>(ELevel::LEVEL_LOGO)] = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Stage1()
{
	if (m_bLoaded[static_cast<_uint>(ELevel::LEVEL_STAGE1)])
	{
		lstrcpyW(m_szLoadingText, TEXT("Stage1 Resource Loaded"));

		m_bFinished = true;
		return S_OK;
	}

	//////// VIBuffer /////////
	{
		// VIBuffer_Terrain_Test
		lstrcpyW(m_szLoadingText, TEXT("VIBuffer_Terrain_Test Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("VIBuffer_Terrain_Test"),
			CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXTURE("Terrain/Height1.bmp"))
		)))
			return E_FAIL;
	}

	//////// Texture /////////
	{
		// Texture_Terrain_Diffuse
		lstrcpyW(m_szLoadingText, TEXT("Texture_Terrain_Diffuse Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Texture_Terrain_Diffuse"),
			CTexture::Create(m_pDevice, m_pContext, TEXTURE("Terrain/Tile/Tile02_Diffuse.dds"), 1)
		)))
			return E_FAIL;

		//// Texture_Terrain_Normal
		//lstrcpyW(m_szLoadingText, TEXT("Texture_Terrain_Normal Loading"));
		//if (FAILED(CGameInstance::Add_Prototype
		//(
		//	static_cast<_uint>(ELevel::LEVEL_STAGE1),
		//	TEXT("Texture_Terrain_Normal"),
		//	CTexture::Create(m_pDevice, m_pContext, TEXTURE("Terrain/Tile/Tile02_Normal.dds.dds"), 1)
		//)))
		//	return E_FAIL;

		//// Texture_Terrain_Mask
		//lstrcpyW(m_szLoadingText, TEXT("Texture_Terrain_Mask Loading"));
		//if (FAILED(CGameInstance::Add_Prototype
		//(
		//	static_cast<_uint>(ELevel::LEVEL_STAGE1),
		//	TEXT("Texture_Terrain_Mask"),
		//	CTexture::Create(m_pDevice, m_pContext, TEXTURE("Terrain/Mask.dds"), 1)
		//)))
		//	return E_FAIL;

		//// Texture_Terrain_Brush
		//lstrcpyW(m_szLoadingText, TEXT("Texture_Terrain_Brush Loading"));
		//if (FAILED(CGameInstance::Add_Prototype
		//(
		//	static_cast<_uint>(ELevel::LEVEL_STAGE1),
		//	TEXT("Texture_Terrain_Brush"),
		//	CTexture::Create(m_pDevice, m_pContext, TEXTURE("Terrain/Brush.png"), 1)
		//)))
		//	return E_FAIL;

		// Texture_Sky
		lstrcpyW(m_szLoadingText, TEXT("Texture_Sky Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Texture_Sky"),
			CTexture::Create(m_pDevice, m_pContext, TEXTURE("SkyBox/Sky_%d.dds"), 4)
		)))
			return E_FAIL;

		// Texture_CrossHair_Dot
		lstrcpyW(m_szLoadingText, TEXT("Texture_CrossHair_Dot Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Texture_CrossHair_Dot"),
			CTexture::Create(m_pDevice, m_pContext, TEXTURE("UI/Player/CrossHair_Dot.png"), 1)
		)))
			return E_FAIL;

		// Texture_CrossHair_Rifle
		lstrcpyW(m_szLoadingText, TEXT("Texture_CrossHair_Rifle Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Texture_CrossHair_Rifle"),
			CTexture::Create(m_pDevice, m_pContext, TEXTURE("UI/Player/CrossHair_Rifle.png"), 1)
		)))
			return E_FAIL;

		// Texture_Bar_Back
		lstrcpyW(m_szLoadingText, TEXT("Texture_Bar_Back Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Texture_Bar_Back"),
			CTexture::Create(m_pDevice, m_pContext, TEXTURE("UI/Player/Bar_Back.png"), 1)
		)))
			return E_FAIL;

		// Texture_Bar_HP
		lstrcpyW(m_szLoadingText, TEXT("Texture_Bar_HP Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Texture_Bar_HP"),
			CTexture::Create(m_pDevice, m_pContext, TEXTURE("UI/Player/Bar_HP.png"), 1)
		)))
			return E_FAIL;

		// Texture_Box
		lstrcpyW(m_szLoadingText, TEXT("Texture_Box Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Texture_Box"),
			CTexture::Create(m_pDevice, m_pContext, TEXTURE("UI/Player/WeaponBox.png"), 1)
		)))
			return E_FAIL;

		// Texture_Icon_AssaultRifle
		lstrcpyW(m_szLoadingText, TEXT("Texture_Icon_AssaultRifle Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Texture_Icon_AssaultRifle"),
			CTexture::Create(m_pDevice, m_pContext, TEXTURE("UI/Player/Icon_AssaultRifle.png"), 1)
		)))
			return E_FAIL;
	}

	//////// Component /////////
	{
		//////// Mesh /////////
		_matrix matPreTransform = XMMatrixIdentity();

		// SkeletalMeshComponent_Bandit
		matPreTransform = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		lstrcpyW(m_szLoadingText, TEXT("SkeletalMeshComponent_Bandit Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("SkeletalMeshComponent_Bandit"),
			CSkeletalMeshComponent::Create
			(
				m_pDevice,
				m_pContext,
				TEXT("Bandit"),
				matPreTransform
			)
		)))
			return E_FAIL;

		// SkeletalMeshComponent_Pan_Brute
		matPreTransform = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		lstrcpyW(m_szLoadingText, TEXT("SkeletalMeshComponent_Pan_Brute Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("SkeletalMeshComponent_Pan_Brute"),
			CSkeletalMeshComponent::Create
			(
				m_pDevice,
				m_pContext,
				TEXT("Pan_Brute"),
				matPreTransform
			)
		)))
			return E_FAIL;

		// SkeletalMeshComponent_ManyFaces
		matPreTransform = XMMatrixScaling(0.001f, 0.001f, 0.001f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		lstrcpyW(m_szLoadingText, TEXT("SkeletalMeshComponent_ManyFaces Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("SkeletalMeshComponent_ManyFaces"),
			CSkeletalMeshComponent::Create
			(
				m_pDevice,
				m_pContext,
				TEXT("ManyFaces"),
				matPreTransform
			)
		)))
			return E_FAIL;

		// SkeletalMeshComponent_AssaultRifle_Earth
		matPreTransform = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		lstrcpyW(m_szLoadingText, TEXT("SkeletalMeshComponent_AssaultRifle_Earth Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("SkeletalMeshComponent_AssaultRifle_Earth"),
			CSkeletalMeshComponent::Create
			(
				m_pDevice,
				m_pContext,
				TEXT("AssaultRifle_Earth"),
				matPreTransform
			)
		)))
			return E_FAIL;

		// SkeletalMeshComponent_Pino
		matPreTransform = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		lstrcpyW(m_szLoadingText, TEXT("SkeletalMeshComponent_Pino Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("SkeletalMeshComponent_Pino"),
			CSkeletalMeshComponent::Create
			(
				m_pDevice,
				m_pContext,
				TEXT("Pino"),
				matPreTransform
			)
		)))
			return E_FAIL;

		// SkeletalMeshComponent_Kena
		matPreTransform = XMMatrixScaling(0.001f, 0.001f, 0.001f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		lstrcpyW(m_szLoadingText, TEXT("SkeletalMeshComponent_Kena Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("SkeletalMeshComponent_Kena"),
			CSkeletalMeshComponent::Create
			(
				m_pDevice,
				m_pContext,
				TEXT("Kena"),
				matPreTransform
			)
		)))
			return E_FAIL;

		// StaticMeshComponent_Building_A
		matPreTransform = XMMatrixScaling(0.0003f, 0.0003f, 0.0003f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		lstrcpyW(m_szLoadingText, TEXT("StaticMeshComponent_Building_A Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("StaticMeshComponent_Building_A"),
			CStaticMeshComponent::Create
			(
				m_pDevice,
				m_pContext,
				TEXT("Building_A"),
				matPreTransform
			)
		)))
			return E_FAIL;
	}

	//////// Actor /////////
	{
		// Actor_Player
		lstrcpyW(m_szLoadingText, TEXT("Actor_Player Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_Player"),
			CPlayer::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Actor_Pino
		lstrcpyW(m_szLoadingText, TEXT("Actor_Pino Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_Pino"),
			CPino::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Actor_Monster_Brute
		lstrcpyW(m_szLoadingText, TEXT("Actor_Monster_Brute Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_Monster_Brute"),
			CMonster_Brute::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Actor_Monster_ManyFaces
		lstrcpyW(m_szLoadingText, TEXT("Actor_Monster_ManyFaces Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_Monster_ManyFaces"),
			CMonster_ManyFaces::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Actor_DynamicActor_AssaultRifle
		lstrcpyW(m_szLoadingText, TEXT("Actor_DynamicActor_AssaultRifle Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_DynamicActor_AssaultRifle"),
			CAssaultRifle::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Actor_StaticActor_Building_A
		lstrcpyW(m_szLoadingText, TEXT("Actor_StaticActor_Building_A Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_StaticActor_Building_A"),
			CBuilding_A::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Actor_Terrain_Test
		lstrcpyW(m_szLoadingText, TEXT("Actor_Terrain_Test Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_Terrain_Test"),
			CTerrain_Test::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Actor_Camera_Free
		lstrcpyW(m_szLoadingText, TEXT("Actor_Camera_Free Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_Camera_Free"),
			CCamera_Free::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Actor_Sky_Stage1
		lstrcpyW(m_szLoadingText, TEXT("Actor_Sky_Stage1 Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_Sky_Stage1"),
			CSky_Stage1::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Actor_UI_CrossHair
		lstrcpyW(m_szLoadingText, TEXT("Actor_UI_CrossHair Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_UI_CrossHair"),
			CCrossHair::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Actor_UI_PlayerHP
		lstrcpyW(m_szLoadingText, TEXT("Actor_UI_PlayerHP Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_UI_PlayerHP"),
			CPlayerHP::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Actor_UI_WeaponBox
		lstrcpyW(m_szLoadingText, TEXT("Actor_UI_WeaponBox Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_UI_WeaponBox"),
			CWeaponBox::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Actor_UI_PlayerUI
		lstrcpyW(m_szLoadingText, TEXT("Actor_UI_PlayerUI Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_UI_PlayerUI"),
			CPlayerUI ::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Actor_UI_MonsterHP
		lstrcpyW(m_szLoadingText, TEXT("Actor_UI_MonsterHP Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_UI_MonsterHP"),
			CMonsterHP::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Actor_UI_MonsterUI
		lstrcpyW(m_szLoadingText, TEXT("Actor_UI_MonsterUI Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_UI_MonsterUI"),
			CMonsterUI::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;

		// Actor_Light_Directional
		lstrcpyW(m_szLoadingText, TEXT("Actor_Light_Directional Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_Light_Directional"),
			CLight_Directional::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;
	}

	lstrcpyW(m_szLoadingText, TEXT("Stage1 Resource Loaded"));

	m_bFinished = true;
	m_bLoaded[static_cast<_uint>(ELevel::LEVEL_STAGE1)] = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Terrain()
{
	if (m_bLoaded[static_cast<_uint>(ELevel::LEVEL_TERRAIN)])
	{
		lstrcpyW(m_szLoadingText, TEXT("Terrain Resource Loaded"));

		m_bFinished = true;
		return S_OK;
	}

	//////// Actor /////////
	{
		// Actor_Sky_Default
		lstrcpyW(m_szLoadingText, TEXT("Actor_Sky_Default Loading"));
		if (FAILED(CGameInstance::Add_Prototype
		(
			static_cast<_uint>(ELevel::LEVEL_TERRAIN),
			TEXT("Actor_Sky_Default"),
			CSky_Default::Create(m_pDevice, m_pContext)
		)))
			return E_FAIL;
	}

	lstrcpyW(m_szLoadingText, TEXT("ActorEditor Resource Loaded"));

	m_bFinished = true;
	m_bLoaded[static_cast<_uint>(ELevel::LEVEL_TERRAIN)] = true;

	return S_OK;
}

CLoader* CLoader::Create
(
	ID3D11Device* _pDevice,
	ID3D11DeviceContext* _pContext, 
	ELevel _eNextLevelID)
{
	CLoader* pInstance = new CLoader(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_eNextLevelID)))
	{
		MSG_BOX("Failed To Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_tCriticalSection);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}