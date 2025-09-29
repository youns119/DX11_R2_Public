#include "pch.h"
#include "CAnimStateMachine_Player.h"

#include "CCharacter.h"
#include "CSkeletalMeshComponent.h"

#include "CPlayerState_Locomotion.h"
#include "CPlayerState_Aim.h"
#include "CPlayerState_Roll.h"

#include "EPlayerState.h"

CAnimStateMachine_Player::CAnimStateMachine_Player()
{
}

HRESULT CAnimStateMachine_Player::Init
(
	CCharacter* _pCharacter, 
	_uint* _pState, 
	const _wstring& _strState
)
{
	if (FAILED(__super::Init(_pCharacter, _pState, _strState)))
		return E_FAIL;

	return S_OK;
}

void CAnimStateMachine_Player::Update(_float _fTimeDelta)
{
	m_fTime += _fTimeDelta;

	if(*m_pState & EPlayerState::PLAYERSTATE_ROLL && m_fTime >= 1.3f)
		ChangeState(TEXT("Locomotion"));

	__super::Update(_fTimeDelta);
}

HRESULT CAnimStateMachine_Player::ChangeState(const _wstring& _strState)
{
	if (_strState == L"Aim")
	{
		if (*m_pState & EPlayerState::PLAYERSTATE_ROLL)
			return S_OK;
	}

	if (FAILED(__super::ChangeState(_strState)))
		return E_FAIL;

	m_fTime = 0.f;

	return S_OK;
}

HRESULT CAnimStateMachine_Player::Ready_AnimState()
{
	if (FAILED(__super::Add_AnimState(TEXT("Locomotion"), CPlayerState_Locomotion::Create())))
		return E_FAIL;

	if (FAILED(__super::Add_AnimState(TEXT("Aim"), CPlayerState_Aim::Create())))
		return E_FAIL;

	if (FAILED(__super::Add_AnimState(TEXT("Roll"), CPlayerState_Roll::Create())))
		return E_FAIL;

	return S_OK;
}

CAnimStateMachine_Player* CAnimStateMachine_Player::Create
(
	CCharacter* _pCharacter,
	_uint* _pState,
	const _wstring& _strState
)
{
	CAnimStateMachine_Player* pInstance = new CAnimStateMachine_Player;

	if (FAILED(pInstance->Init(_pCharacter, _pState, _strState)))
	{
		MSG_BOX("Failed To Create : CAnimStateMachine_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimStateMachine_Player::Free()
{
	__super::Free();
}