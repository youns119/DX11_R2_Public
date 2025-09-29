#include "pch.h"
#include "CPlayerState_Locomotion.h"

#include "CGameInstance.h"
#include "CCharacter.h"
#include "CSkeletalMeshComponent.h"
#include "CSpringArmComponent.h"
#include "CCameraComponent.h"
#include "CCharacterMovementComponent.h"

#include "EPlayerState.h"

CPlayerState_Locomotion::CPlayerState_Locomotion()
{
}

HRESULT CPlayerState_Locomotion::Init()
{
	return S_OK;
}

void CPlayerState_Locomotion::Enter(CCharacter* _pCharacter, _uint* _pState)
{
	_uint iMaskBit = 0xFFFFFFF0;

	*_pState &= iMaskBit;
	*_pState |= EPlayerState::PLAYERSTATE_LOCOMOTION;

	m_fSoundTimeAcc = 1.f;
}

void CPlayerState_Locomotion::Update
(
	CCharacter* _pCharacter, 
	_uint* _pState, 
	_float _fTimeDelta
)
{
	CCharacterMovementComponent* pMovementComponent = _pCharacter->Get_MovementComponent();

	if (CGameInstance::Key_Hold(DIK_LSHIFT))
	{
		pMovementComponent->Set_Accel(16.f);
		pMovementComponent->Set_Decel(12.f);
		pMovementComponent->Set_MaxSpeed(16.f);

		CCameraComponent* pCameraComponent = static_cast<CCameraComponent*>(_pCharacter->Find_ActorComponent(g_strCameraComponent));
		if (pCameraComponent == nullptr)
			return;

		pCameraComponent->Zoom(1.3f);
	}
	else if (CGameInstance::Key_Release(DIK_LSHIFT))
	{
		pMovementComponent->Set_Accel(8.f);
		pMovementComponent->Set_Decel(6.f);
		pMovementComponent->Set_MaxSpeed(8.f);

		CCameraComponent* pCameraComponent = static_cast<CCameraComponent*>(_pCharacter->Find_ActorComponent(g_strCameraComponent));
		if (pCameraComponent == nullptr)
			return;

		pCameraComponent->Zoom(1.f);
	}

	_float3 vVelocity = _pCharacter->Get_MovementComponent()->Get_Velocity();
	_vector vVel = XMLoadFloat3(&vVelocity);
	_float fSpeed = XMVectorGetX(XMVector3Length(vVel));
	_float fScaling = pMovementComponent->Get_Scaling();

	CSkeletalMeshComponent* pMeshComponent = _pCharacter->Get_MeshComponent();

	if (fSpeed == 0.f)
		pMeshComponent->Set_CurrAnim(pMeshComponent->Get_AnimIndex("2R_Relax_Idle"), true);
	else if (fSpeed <= m_fWalkSpeed * fScaling)
	{
		pMeshComponent->Set_CurrAnim(pMeshComponent->Get_AnimIndex("2R_Relax_Walk_F"), true);
		m_fWalkSoundDist = 0.5f;
	}
	else if (fSpeed <= m_fRunSpeed * fScaling)
	{
		pMeshComponent->Set_CurrAnim(pMeshComponent->Get_AnimIndex("2R_Relax_Jog_F"), true);
		m_fWalkSoundDist = 0.5f;
	}
	else if (fSpeed <= m_fSprintSpeed * fScaling)
	{
		pMeshComponent->Set_CurrAnim(pMeshComponent->Get_AnimIndex("2R_Relax_Sprint_F"), true);
		m_fWalkSoundDist = 0.35f;
	}

	if (pMovementComponent->Get_Speed() != 0.f)
	{
		m_fSoundTimeAcc += _fTimeDelta;

		if (m_fSoundTimeAcc >= m_fWalkSoundDist)
		{
			SOUND->Play_Sound(L"Player_Walk.wav", EChannel::ECHANNEL_PLAYER, 0.05f);
			m_fSoundTimeAcc = 0.f;
		}
	}
}

void CPlayerState_Locomotion::Exit(CCharacter* _pCharacter, _uint* _pState)
{
	*_pState ^= EPlayerState::PLAYERSTATE_LOCOMOTION;
}

CPlayerState_Locomotion* CPlayerState_Locomotion::Create()
{
	CPlayerState_Locomotion* pInstance = new CPlayerState_Locomotion;

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CPlayerState_Locomotion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerState_Locomotion::Free()
{
	__super::Free();
}