#include "pch.h"
#include "CPlayer.h"

#include "CGameInstance.h"
#include "CLevel.h"
#include "CTerrain.h"
#include "CSkeletalMeshComponent.h"
#include "CSpringArmComponent.h"
#include "CCameraComponent.h"
#include "CColliderComponent.h"
#include "CCharacterMovementComponent.h"
#include "CNavigationComponent.h"
#include "CShader.h"
#include "CMathUtility.h"

#include "CAnimStateMachine_Player.h"
#include "CAssaultRifle.h"
#include "CPlayerUI.h"

#include "ELevel.h"
#include "ECrossHair.h"

CPlayer::CPlayer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CCharacter{ _pDevice, _pContext }
{
}

CPlayer::CPlayer(const CPlayer& _Prototype)
	: CCharacter(_Prototype)
	, m_fMaxHP{ _Prototype.m_fMaxHP }
	, m_fCurrHP{ _Prototype.m_fCurrHP }
	, m_fBossMaxHP{ _Prototype.m_fBossMaxHP }
	, m_fBossCurrHP{ _Prototype.m_fBossCurrHP }
{
}

HRESULT CPlayer::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	m_fMaxHP = 100.f;
	m_fCurrHP = m_fMaxHP;

	m_fBossMaxHP = 100.f;
	m_fBossCurrHP = m_fBossMaxHP;

	return S_OK;
}

HRESULT CPlayer::Init(void* _pDesc)
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

void CPlayer::Pre_Update(_float _fTimeDelta)
{
	if (CGameInstance::Get_Controller() == this)
	{
		// Keyboard
		if (m_iPlayerState & EPlayerState::PLAYERSTATE_LOCOMOTION ||
			m_iPlayerState & EPlayerState::PLAYERSTATE_AIM)
		{
			_vector vCameraLook = m_pSpringArmComponent->Get_TransformState(ETransformState::TRANSFORMSTATE_LOOK);
			_vector vCameraRight = m_pSpringArmComponent->Get_TransformState(ETransformState::TRANSFORMSTATE_RIGHT);

			if (m_iPlayerState & EPlayerState::PLAYERSTATE_LOCOMOTION)
			{
				if (CGameInstance::Key_Hold(DIK_W))
					m_pMovementComponent->Add_Velocity(XMVectorSetY(vCameraLook, 0.f), _fTimeDelta);
				if (CGameInstance::Key_Hold(DIK_S))
					m_pMovementComponent->Add_Velocity(XMVectorSetY(vCameraLook, 0.f), -_fTimeDelta);
				if (CGameInstance::Key_Hold(DIK_D))
					m_pMovementComponent->Add_Velocity(XMVectorSetY(vCameraRight, 0.f), _fTimeDelta);
				if (CGameInstance::Key_Hold(DIK_A))
					m_pMovementComponent->Add_Velocity(XMVectorSetY(vCameraRight, 0.f), -_fTimeDelta);

				_float3 vVel = m_pMovementComponent->Get_Velocity();
				_vector vLook = m_pMeshComponent->Get_TransformState(ETransformState::TRANSFORMSTATE_LOOK);

				if (XMVectorGetX(XMVector3Length(XMLoadFloat3(&vVel))) != 0.f)
				{
					vVel.y = 0.f;
					vLook = XMVectorSetY(vLook, 0.f);

					_float fDegree = CMathUtility::GetBetweenAngle(vLook, XMLoadFloat3(&vVel));
					m_pMovementComponent->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDegree * _fTimeDelta);
				}
			}
			else
			{
				if (CGameInstance::Key_Hold(DIK_W))
					m_pMovementComponent->Add_Velocity(XMVectorSetY(vCameraLook, 0.f), _fTimeDelta);
				if (CGameInstance::Key_Hold(DIK_S))
					m_pMovementComponent->Add_Velocity(XMVectorSetY(vCameraLook, 0.f), -_fTimeDelta);
				if (CGameInstance::Key_Hold(DIK_D))
					m_pMovementComponent->Add_Velocity(XMVectorSetY(vCameraRight, 0.f), _fTimeDelta);
				if (CGameInstance::Key_Hold(DIK_A))
					m_pMovementComponent->Add_Velocity(XMVectorSetY(vCameraRight, 0.f), -_fTimeDelta);

				_vector vLook = m_pMeshComponent->Get_TransformState(ETransformState::TRANSFORMSTATE_LOOK);

				vCameraLook = XMVectorSetY(vCameraLook, 0.f);
				vLook = XMVectorSetY(vLook, 0.f);

				if (XMVectorGetX(XMVector3Length(vCameraLook - vLook)) != 0.f)
				{
					_float fDegree = CMathUtility::GetBetweenAngle(vLook, vCameraLook);
					m_pMovementComponent->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDegree * _fTimeDelta);
				}
			}
		}

		if (CGameInstance::Key_Press(DIK_SPACE))
			m_pAnimStateMachine->ChangeState(TEXT("Roll"));

		//if (CGameInstance::Key_Press(DIK_TAB))
		//	m_pCameraComponent->Camera_Fix(g_hWnd, !m_pCameraComponent->Get_CameraFix());

		// Mouse
		_long lMouseMove{};

		if (lMouseMove = CGameInstance::Get_DIMouseMove(EMouseDir::MOUSEDIR_X))
			m_pSpringArmComponent->Set_DeltaRotation(XMVectorSet(0.f, 1.f * lMouseMove * _fTimeDelta * 5.f, 0.f, 0.f));
		if (lMouseMove = CGameInstance::Get_DIMouseMove(EMouseDir::MOUSEDIR_Y))
			m_pSpringArmComponent->Set_DeltaRotation(XMVectorSet(1.f * lMouseMove * _fTimeDelta * 5.f, 0.f, 0.f, 0.f));

		if (CGameInstance::Mouse_Hold(EMouseKey::MOUSEKEY_RB))
			m_pAnimStateMachine->ChangeState(TEXT("Aim"));

		if(m_iPlayerState & EPlayerState::PLAYERSTATE_AIM)
			m_pPlayerUI->Set_CrossHair(ECrossHair::CROSSHAIR_RIFLE);
		else
			m_pPlayerUI->Set_CrossHair(ECrossHair::CROSSHAIR_DOT);

		if (CGameInstance::Key_Press(DIK_F1))
		{
			SOUND->Stop_Sound(EChannel::ECHANNEL_BGM);
			SOUND->Play_BGM(L"Boss_Bgm_Loop.wav", 0.8f);

			m_pPlayerUI->Set_BossShow(true);
		}


		if (CGameInstance::Key_Press(DIK_F2))
		{
			SOUND->Stop_Sound(EChannel::ECHANNEL_BGM);
			SOUND->Play_BGM(L"Boss_Success.wav", 0.8f);
		}
	}

	__super::Pre_Update(_fTimeDelta);
}

void CPlayer::Update(_float _fTimeDelta)
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

void CPlayer::Post_Update(_float _fTimeDelta)
{
	const _float4x4* pSocket = m_pMeshComponent->Get_BoneCombined_MatrixPtr("Bone_M_Weapon_Main");

	__super::Post_Update(_fTimeDelta);
}

HRESULT CPlayer::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_Component()
{
	// SkeletalMeshComponent
	FSkeletalMeshComponent tSkeletalMeshComponent{};
	tSkeletalMeshComponent.pOwner = this;
	tSkeletalMeshComponent.eRenderType = ERender::RENDER_NONBLEND;
	tSkeletalMeshComponent.bCulling = true;
	tSkeletalMeshComponent.fCullingRange = 10.f;
	tSkeletalMeshComponent.szAnimName = "2R_Relax_Idle";
	tSkeletalMeshComponent.szRootName = "Bone_M_Root";

	m_pMeshComponent = static_cast<CSkeletalMeshComponent*>(__super::Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("SkeletalMeshComponent_Bandit"),
		TEXT("SkeletalMeshComponent_Bandit"),
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
	//m_pCameraComponent->Camera_Fix(g_hWnd, true);

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

	//// NavigationComponent
	//FNavigationComponent tNavigationComponent{};
	//tNavigationComponent.pOwner = this;
	//tNavigationComponent.iCurrCellIndex = 0;

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

HRESULT CPlayer::Ready_Resource()
{
	// MeshComponent
	if(FAILED(m_pMeshComponent->Add_Shader
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC), 
		L"Shader_VtxSkeletalMesh"
	)))
		return E_FAIL;

	for (_uint i = 0; i < m_pMeshComponent->Get_NumMesh(); i++)
		if (FAILED(m_pMeshComponent->Bind_Material(i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)) ||
			FAILED(m_pMeshComponent->Bind_Material(i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
			return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_SubActor()
{
	// Actor_DynamicActor_AssaultRifle
	FActor tActor{};
	tActor.pOuter = m_pOuter;

	const _float4x4* pSocketMatrix = m_pMeshComponent->Get_BoneCombined_MatrixPtr("Bone_M_Weapon_Main");

	m_pWeapon_Rifle = static_cast<CAssaultRifle*>(__super::Add_SubActor
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Actor_DynamicActor_AssaultRifle"),
		TEXT("Weapon"),
		&tActor
	));
	if (m_pWeapon_Rifle == nullptr)
		return E_FAIL;

	m_pWeapon_Rifle->AttachToComponent(m_pRootComponent, pSocketMatrix);

	_vector vScale{}, vRotation{}, vPosition{};
	vScale = XMVectorSet(1.f, 1.f, 1.f, 0.f);
	vRotation = XMVectorSet(90.f, 0.f, 180.f, 0.f);
	vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	m_pWeapon_Rifle->Get_RootComponent()->Update_Transform(vScale, vRotation, vPosition);

	// Actor_UI_PlayerUI
	m_pPlayerUI = static_cast<CPlayerUI*>(__super::Add_SubActor
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Actor_UI_PlayerUI"),
		TEXT("PlayerUI"),
		&tActor
	));
	if (m_pPlayerUI == nullptr)
		return E_FAIL;

	m_pPlayerUI->Set_HP(&m_fCurrHP, &m_fBossCurrHP);

	return S_OK;
}

HRESULT CPlayer::Ready_AnimStateMachine()
{
	if (FAILED(__super::Bind_AnimStateMachine(CAnimStateMachine_Player::Create
	(
		this, 
		&m_iPlayerState, 
		TEXT("Locomotion")
	))))
		return E_FAIL;

	return S_OK;
}

CPlayer* CPlayer::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CPlayer* pInstance = new CPlayer(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CPlayer::Clone(void* _pDesc)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pPlayerUI);
	Safe_Release(m_pWeapon_Rifle);

	//Safe_Release(m_pNavigationComponent);
	Safe_Release(m_pColliderComponent);
	Safe_Release(m_pCameraComponent);
	Safe_Release(m_pSpringArmComponent);
	Safe_Release(m_pMeshComponent);
}