#include "pch.h"
#include "CPlayerState_Roll.h"

#include "CGameInstance.h"
#include "CCharacter.h"
#include "CSkeletalMeshComponent.h"
#include "CCameraComponent.h"
#include "CCharacterMovementComponent.h"

#include "EPlayerState.h"

CPlayerState_Roll::CPlayerState_Roll()
{
}

HRESULT CPlayerState_Roll::Init()
{
	return S_OK;
}

void CPlayerState_Roll::Enter(CCharacter* _pCharacter, _uint* _pState)
{
	_uint iMaskBit = 0x00000000;

	*_pState &= iMaskBit;
	*_pState |= EPlayerState::PLAYERSTATE_ROLL;

	CCharacterMovementComponent* pMovementComponent = _pCharacter->Get_MovementComponent();

	pMovementComponent->Set_Accel(16.f);
	pMovementComponent->Set_Decel(12.f);
	pMovementComponent->Set_MaxSpeed(16.f);

	m_fRollTime = 0.f;

	SOUND->Play_Sound(L"Player_Roll.wav", EChannel::ECHANNEL_PLAYER, 0.15f);

	CCameraComponent* pCameraComponent = static_cast<CCameraComponent*>(_pCharacter->Find_ActorComponent(g_strCameraComponent));
	if (pCameraComponent == nullptr)
		return;

	pCameraComponent->Zoom(1.5f);
}

void CPlayerState_Roll::Update(CCharacter* _pCharacter, _uint* _pState, _float _fTimeDelta)
{
	m_fRollTime += _fTimeDelta;

	CSkeletalMeshComponent* pMeshComponent = _pCharacter->Get_MeshComponent();
	CCharacterMovementComponent* pMovementComponent = _pCharacter->Get_MovementComponent();

	_vector vLook = pMeshComponent->Get_TransformState(ETransformState::TRANSFORMSTATE_LOOK);
	_float fVel = _fTimeDelta - _fTimeDelta * (m_fRollTime / 2.f);

	pMovementComponent->Add_Velocity(vLook, fVel);

	pMeshComponent->Set_CurrAnim(pMeshComponent->Get_AnimIndex("2R_EvadeRoll_Light_F"), false);
}

void CPlayerState_Roll::Exit(CCharacter* _pCharacter, _uint* _pState)
{
	*_pState ^= EPlayerState::PLAYERSTATE_ROLL;

	CCharacterMovementComponent* pMovementComponent = _pCharacter->Get_MovementComponent();

	if (!CGameInstance::Key_Hold(DIK_LSHIFT))
	{
		pMovementComponent->Set_Accel(8.f);
		pMovementComponent->Set_Decel(6.f);
		pMovementComponent->Set_MaxSpeed(8.f);
	}

	CCameraComponent* pCameraComponent = static_cast<CCameraComponent*>(_pCharacter->Find_ActorComponent(g_strCameraComponent));
	if (pCameraComponent == nullptr)
		return;

	pCameraComponent->Zoom(1.f);
}

CPlayerState_Roll* CPlayerState_Roll::Create()
{
	CPlayerState_Roll* pInstance = new CPlayerState_Roll;

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CPlayerState_Roll");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerState_Roll::Free()
{
	__super::Free();
}