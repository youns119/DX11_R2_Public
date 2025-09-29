#include "CCharacter.h"

#include "CSkeletalMeshComponent.h"
#include "CCharacterMovementComponent.h"
#include "CAnimStateMachine.h"

CCharacter::CCharacter(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CActor{ _pDevice, _pContext }
{
}

CCharacter::CCharacter(const CCharacter& _Prototype)
	: CActor(_Prototype)
{
}

HRESULT CCharacter::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharacter::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FCharacter* pCharacterDesc = static_cast<FCharacter*>(_pDesc);

	FCharacterMovementComponent tMovementComponent{};
	tMovementComponent.pOwner = this;
	tMovementComponent.fMoveSpeed = pCharacterDesc->fMoveSpeed;
	tMovementComponent.fRotationSpeed = pCharacterDesc->fRotationSpeed;
	tMovementComponent.fMoveAccel = pCharacterDesc->fMoveAccel;
	tMovementComponent.fMoveDecel = pCharacterDesc->fMoveDecel;
	tMovementComponent.fMaxVelocity = pCharacterDesc->fMaxVelocity;

	m_pMovementComponent = CCharacterMovementComponent::Create(m_pDevice, m_pContext);
	if (m_pMovementComponent == nullptr ||
		FAILED(m_pMovementComponent->Init(&tMovementComponent)))
		return E_FAIL;

	Add_ActorComponent(g_strCharacterMovementComponent, m_pMovementComponent);

	FActor tActor{};
	tActor.pOuter = pCharacterDesc->pOuter;

	if (FAILED(__super::Init(&tActor)) ||
		FAILED(Ready_AnimStateMachine()))
		return E_FAIL;

	return S_OK;
}

void CCharacter::Pre_Update(_float _fTimeDelta)
{
	if (m_pAnimStateMachine)
		m_pAnimStateMachine->Update(_fTimeDelta);

	__super::Pre_Update(_fTimeDelta);
}

void CCharacter::Clear()
{
	if(m_pAnimStateMachine)
		m_pAnimStateMachine->Clear();

	__super::Clear();
}

HRESULT CCharacter::Bind_AnimStateMachine(CAnimStateMachine* _pAnimStateMachine)
{
	if (_pAnimStateMachine == nullptr)
		return E_FAIL;

	m_pAnimStateMachine = _pAnimStateMachine;

	return S_OK;
}

void CCharacter::Free()
{
	__super::Free();

	Safe_Release(m_pAnimStateMachine);

	Safe_Release(m_pMovementComponent);
	Safe_Release(m_pMeshComponent);
}