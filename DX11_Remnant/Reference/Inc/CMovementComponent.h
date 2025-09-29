#pragma once

#include "CActorComponent.h"

#include "FMovementComponent.h"

BEGIN(Engine)

class CSceneComponent;

class ENGINE_DLL CMovementComponent
	: public CActorComponent
{
protected :
	CMovementComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CMovementComponent(const CMovementComponent& _Prototype);
	virtual ~CMovementComponent() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;

public :
	virtual void Set_UpdateComponent(CSceneComponent* _pSceneComponent);

public :
	virtual void Move(_fvector _vDir, _float _fTimeDelta);
	virtual void Turn(_fvector _vAxis, _float _fTimeDelta);

protected :
	CSceneComponent* m_pUpdateComponent{};

protected :
	_float m_fMoveSpeed{};
	_float m_fRotationSpeed{};

public :
	static CMovementComponent* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc);
	virtual void Free() override;
};

END