#include "pch.h"
#include "CPlayerState_Aim.h"

#include "CGameInstance.h"
#include "CCharacter.h"
#include "CSkeletalMeshComponent.h"
#include "CSpringArmComponent.h"
#include "CCameraComponent.h"
#include "CCharacterMovementComponent.h"
#include "CAnimStateMachine.h"

#include "CPlayer.h"

#include "EPlayerState.h"

CPlayerState_Aim::CPlayerState_Aim()
{
}

HRESULT CPlayerState_Aim::Init()
{
	return S_OK;
}

void CPlayerState_Aim::Enter(CCharacter* _pCharacter, _uint* _pState)
{
	_uint iMaskBit = 0xFFFFFFF0;

	*_pState &= iMaskBit;
	*_pState |= EPlayerState::PLAYERSTATE_AIM;

	CCharacterMovementComponent* pMovementComponent = _pCharacter->Get_MovementComponent();

	pMovementComponent->Set_Accel(4.f);
	pMovementComponent->Set_Decel(3.f);
	pMovementComponent->Set_MaxSpeed(4.f);

	CCameraComponent* pCameraComponent = static_cast<CCameraComponent*>(_pCharacter->Find_ActorComponent(g_strCameraComponent));
	if (pCameraComponent == nullptr)
		return;

	pCameraComponent->Zoom(0.7f);
}

void CPlayerState_Aim::Update(CCharacter* _pCharacter, _uint* _pState, _float _fTimeDelta)
{
	CSkeletalMeshComponent* pMeshComponent = _pCharacter->Get_MeshComponent();

	if (CGameInstance::Mouse_Release(EMouseKey::MOUSEKEY_RB))
	{
		_pCharacter->Get_AnimStateMachine()->ChangeState(L"Locomotion");
		return;
	}

	if (CGameInstance::Mouse_Press(EMouseKey::MOUSEKEY_LB) && m_fAttackAcc >= m_fAttackTime)
	{
		SOUND->Play_Sound(L"AssaultRifle_Shoot.wav", EChannel::ECHANNEL_PLAYER_GUN, 0.4f);

		CCameraComponent* pCameraComponent = static_cast<CCameraComponent*>(_pCharacter->Find_ActorComponent(g_strCameraComponent));
		if (pCameraComponent == nullptr)
			return;

		pMeshComponent->Set_CurrAnim(pMeshComponent->Get_AnimIndex("2R_Aim_Shoot_AssaultRifle"), false);

		m_fAttackAcc = 0.f;
		pCameraComponent->Shake();

		CPlayer* pPlayer = static_cast<CPlayer*>(_pCharacter);
		pPlayer->Set_Shoot(true);
	}

	m_fAttackAcc += _fTimeDelta;

	CCharacterMovementComponent* pMovementComponent = _pCharacter->Get_MovementComponent();

	_float3 vVelocity = _pCharacter->Get_MovementComponent()->Get_Velocity();
	_vector vVel = XMLoadFloat3(&vVelocity);
	_float fSpeed = XMVectorGetX(XMVector3Length(vVel));
	_float fScaling = pMovementComponent->Get_Scaling();

	if(fSpeed == 0.f)
		pMeshComponent->Set_CurrAnim(pMeshComponent->Get_AnimIndex("2R_Aim_Idle"), true);
	else
		pMeshComponent->Set_CurrAnim(pMeshComponent->Get_AnimIndex("2R_Aim_Walk_F"), true);
}

void CPlayerState_Aim::Exit(CCharacter* _pCharacter, _uint* _pState)
{
	*_pState ^= EPlayerState::PLAYERSTATE_LOCOMOTION;

	CCharacterMovementComponent* pMovementComponent = _pCharacter->Get_MovementComponent();

	pMovementComponent->Set_Accel(8.f);
	pMovementComponent->Set_Decel(6.f);
	pMovementComponent->Set_MaxSpeed(8.f);

	CCameraComponent* pCameraComponent = static_cast<CCameraComponent*>(_pCharacter->Find_ActorComponent(g_strCameraComponent));
	if (pCameraComponent == nullptr)
		return;

	pCameraComponent->Zoom(1.f);
}

CPlayerState_Aim* CPlayerState_Aim::Create()
{
	CPlayerState_Aim* pInstance = new CPlayerState_Aim;

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CPlayerState_Aim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerState_Aim::Free()
{
	__super::Free();
}