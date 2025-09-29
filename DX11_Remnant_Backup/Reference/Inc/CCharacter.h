#pragma once

#include "CActor.h"

#include "FCharacter.h"

BEGIN(Engine)

class CSkeletalMeshComponent;
class CCharacterMovementComponent;
class CAnimStateMachine;

class ENGINE_DLL CCharacter abstract
	: public CActor
{
protected :
	CCharacter(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CCharacter(const CCharacter& _Prototype);
	virtual ~CCharacter() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;

public :
	CSkeletalMeshComponent* Get_MeshComponent() { return m_pMeshComponent; }
	CCharacterMovementComponent* Get_MovementComponent() { return m_pMovementComponent; }

	CAnimStateMachine* Get_AnimStateMachine() { return m_pAnimStateMachine; }

public :
	virtual void Clear() override;

protected :
	CSkeletalMeshComponent* m_pMeshComponent{};
	CCharacterMovementComponent* m_pMovementComponent{};

protected :
	CAnimStateMachine* m_pAnimStateMachine{};

protected :
	HRESULT Bind_AnimStateMachine(CAnimStateMachine* _pAnimStateMachine);

protected :
	virtual HRESULT Ready_AnimStateMachine() { return S_OK; }

public :
	virtual CClonable* Clone(void* _pDesc) PURE;
	virtual void Free() override;
};

END