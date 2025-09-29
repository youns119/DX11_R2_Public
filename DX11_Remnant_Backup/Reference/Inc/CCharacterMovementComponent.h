#pragma once

#include "CMovementComponent.h"

#include "FCharacterMovementComponent.h"

BEGIN(Engine)

class ENGINE_DLL CCharacterMovementComponent final
	: public CMovementComponent
{
private :
	CCharacterMovementComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CCharacterMovementComponent(const CCharacterMovementComponent& _Prototype);
	virtual ~CCharacterMovementComponent() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;

public :
	_float3 Get_Velocity() { return m_vVelocity; }

	_float Get_Speed() const
	{ return XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vVelocity))); }
	_float Get_Accel() const { return m_fMoveAccel; }
	_float Get_Decel() const { return m_fMoveDecel; }
	_float Get_MaxSpeed() const { return m_fMaxSpeed; }

	_float Get_Scaling() const { return m_fScaling; }

	void Set_Accel(_float _fAccel) { m_fMoveAccel = _fAccel * m_fScaling; }
	void Set_Decel(_float _fDecel) { m_fMoveDecel = _fDecel * m_fScaling; }
	void Set_MaxSpeed(_float _fMaxSpeed) { m_fMaxSpeed = _fMaxSpeed * m_fScaling; }

public :
	virtual void Add_Velocity(_fvector _vDir, _float _fTimeDelta);

private :
	_float3 m_vVelocity{};

	_float m_fMoveAccel{};
	_float m_fMoveDecel{};
	_float m_fMaxSpeed{};

private :
	_float m_fScaling{ 0.01f };

private :
	virtual void Decrease_Velocity(_float _fTimeDelta);

public :
	static CCharacterMovementComponent* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext
	);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END