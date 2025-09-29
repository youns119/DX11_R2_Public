#include "pch.h"
#include "CPino.h"

#include "CGameInstance.h"
#include "CLevel.h"
#include "CTerrain.h"
#include "CSkeletalMeshComponent.h"
#include "CSpringArmComponent.h"
#include "CCameraComponent.h"
#include "CColliderComponent.h"
#include "CCharacterMovementComponent.h"
#include "CShader.h"
#include "CMathUtility.h"

#include "ELevel.h"

CPino::CPino(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CCharacter{ _pDevice, _pContext }
{
}

CPino::CPino(const CPino& _Prototype)
	: CCharacter(_Prototype)
{
}

HRESULT CPino::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPino::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FCharacter* pCharacterDesc = static_cast<FCharacter*>(_pDesc);
	pCharacterDesc->fMoveSpeed = 8.f;
	pCharacterDesc->fRotationSpeed = 20.f;
	pCharacterDesc->fMoveAccel = 8.f;
	pCharacterDesc->fMoveDecel = 6.f;
	pCharacterDesc->fMaxVelocity = 8.f;

	if (FAILED(__super::Init(pCharacterDesc)))
		return E_FAIL;

	return S_OK;
}

void CPino::Pre_Update(_float _fTimeDelta)
{
	if (CGameInstance::Key_Hold(DIK_UP))
		m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Idle"), true);
	if (CGameInstance::Key_Hold(DIK_DOWN))
		m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Aim_Loop"), true);
	if (CGameInstance::Key_Hold(DIK_RIGHT))
		m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Bow_Aim_RefPose"), true);
	if (CGameInstance::Key_Hold(DIK_LEFT))
		m_pMeshComponent->Set_CurrAnim(m_pMeshComponent->Get_AnimIndex("Bomb_Loop"), true);


	// Mouse
	_long lMouseMove{};

	if (lMouseMove = CGameInstance::Get_DIMouseMove(EMouseDir::MOUSEDIR_X))
		m_pSpringArmComponent->Set_DeltaRotation(XMVectorSet(0.f, 1.f * lMouseMove * _fTimeDelta * 5.f, 0.f, 0.f));
	if (lMouseMove = CGameInstance::Get_DIMouseMove(EMouseDir::MOUSEDIR_Y))
		m_pSpringArmComponent->Set_DeltaRotation(XMVectorSet(1.f * lMouseMove * _fTimeDelta * 5.f, 0.f, 0.f, 0.f));

	__super::Pre_Update(_fTimeDelta);
}

void CPino::Update(_float _fTimeDelta)
{
	// Terrain Tracking
	{
		CActor* pActor = m_pOuter->Get_Actor(L"Layer_BackGround", L"Actor_Terrain_Test");
		CTerrain* pTerrain = static_cast<CTerrain*>(pActor);

		_vector vSrcPos = m_pRootComponent->Get_TransformState(ETransformState::TRANSFORMSTATE_POS);
		_float3 vPos{};
		XMStoreFloat3(&vPos, vSrcPos);

		_float fHeight = pTerrain->Get_Height(&vPos);
		m_pRootComponent->Update_Position(XMVectorSetY(vSrcPos, fHeight));
	}

	__super::Update(_fTimeDelta);
}

void CPino::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CPino::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPino::Ready_Component()
{
	// SkeletalMeshComponent
	FSkeletalMeshComponent tSkeletalMeshComponent{};
	tSkeletalMeshComponent.pOwner = this;
	tSkeletalMeshComponent.eRenderType = ERender::RENDER_NONBLEND;
	tSkeletalMeshComponent.bCulling = true;
	tSkeletalMeshComponent.fCullingRange = 10.f;
	tSkeletalMeshComponent.szAnimName = "Idle";
	tSkeletalMeshComponent.szRootName = "kena_RIG";

	m_pMeshComponent = static_cast<CSkeletalMeshComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("SkeletalMeshComponent_Kena"),
		TEXT("SkeletalMeshComponent_Kena"),
		&tSkeletalMeshComponent
	));
	if (m_pMeshComponent == nullptr)
		return E_FAIL;

	Set_RootComponent(m_pMeshComponent);
	m_pMovementComponent->Set_UpdateComponent(m_pRootComponent);

	// SpringArmComponent
	_float3 vRotation = m_pRootComponent->Get_Transform(ETransform::TRANSFORM_ROTATION);

	FSpringArmComponent tSpringArmComponent{};
	tSpringArmComponent.pOwner = this;
	tSpringArmComponent.vRotation = _float3(0.f, 90.f, 0.f);
	tSpringArmComponent.vPosition = _float3(0.f, 1.3f, 0.f);
	tSpringArmComponent.vOffset = _float3(0.6f, 0.4f, 0.f);
	tSpringArmComponent.fArmLength = 1.7f;

	m_pSpringArmComponent = static_cast<CSpringArmComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		g_strSpringArmComponent,
		g_strSpringArmComponent,
		&tSpringArmComponent
	));
	if (m_pSpringArmComponent == nullptr)
		return E_FAIL;

	m_pSpringArmComponent->AttachToComponent(m_pRootComponent);
	m_pSpringArmComponent->Set_InheritRotation(false);

	// CameraComponent
	FCameraComponent tCameraComponent{};
	tCameraComponent.pOwner = this;
	tCameraComponent.fFov = XMConvertToRadians(60.f);
	tCameraComponent.fNear = 0.1f;
	tCameraComponent.fFar = 1000.f;

	m_pCameraComponent = static_cast<CCameraComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		g_strCameraComponent,
		g_strCameraComponent,
		&tCameraComponent
	));
	if (m_pCameraComponent == nullptr)
		return E_FAIL;

	m_pCameraComponent->AttachToComponent(m_pSpringArmComponent);
	m_pCameraComponent->Camera_Fix(g_hWnd, false);

	// ColliderComponent
	FColliderComponent tColliderComponent{};
	tColliderComponent.pOwner = this;
	tColliderComponent.vScale = _float3(0.4f, 0.9f, 0.4f);
	tColliderComponent.vRotation = _float3(0.f, 0.f, 0.f);
	tColliderComponent.vPosition = _float3(0.f, 0.9f, 0.f);

	m_pColliderComponent = static_cast<CColliderComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		g_strColliderComponent_OBB,
		g_strColliderComponent_OBB,
		&tColliderComponent
	));
	if (m_pColliderComponent == nullptr)
		return E_FAIL;

	m_pColliderComponent->AttachToComponent(m_pRootComponent);
}

HRESULT CPino::Ready_Resource()
{
	// MeshComponent
	if (FAILED(m_pMeshComponent->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		L"Shader_VtxSkeletalMesh"
	)))
		return E_FAIL;

	for (_uint i = 0; i < m_pMeshComponent->Get_NumMesh(); i++)
		if (FAILED(m_pMeshComponent->Bind_Material(i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
			return E_FAIL;

	return S_OK;
}

HRESULT CPino::Ready_SubActor()
{
	return S_OK;
}

HRESULT CPino::Ready_AnimStateMachine()
{
	return S_OK;
}

CPino* CPino::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CPino* pInstance = new CPino(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CPino");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CPino::Clone(void* _pDesc)
{
	CPino* pInstance = new CPino(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CPino");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPino::Free()
{
	__super::Free();

	Safe_Release(m_pColliderComponent);
	Safe_Release(m_pCameraComponent);
	Safe_Release(m_pSpringArmComponent);
	Safe_Release(m_pMeshComponent);
}