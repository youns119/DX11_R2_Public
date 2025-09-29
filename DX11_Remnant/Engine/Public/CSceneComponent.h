#pragma once

#include "CActorComponent.h"

#include "FSceneComponent.h"
#include "ETransformState.h"

BEGIN(Engine)

class ENGINE_DLL CSceneComponent
	: public CActorComponent
{
protected :
	CSceneComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSceneComponent(const CSceneComponent& _Prototype);
	virtual ~CSceneComponent() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;

public :
	_matrix Get_WorldMatrix() const { return XMLoadFloat4x4(&m_matWorld); }
	_matrix Get_WorldMatrix_Inverse() const { return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_matWorld)); }
	const _float4x4* Get_WorldMatrix_Ptr() const { return &m_matWorld; }

	_float3 Get_Transform(ETransform _eTransform) const
	{ return m_vRelativeTransform[static_cast<_uint>(_eTransform)]; }
	_vector Get_TransformState(ETransformState _eTransformState) const
	{ return XMLoadFloat4x4(&m_matWorld).r[static_cast<_uint>(_eTransformState)]; }

	_bool Get_DirtyFlag() const { return m_bDirtyFlag; }

public :
	void Set_DeltaScale(_fvector _vScale)
	{
		if (XMVectorGetX(XMVector3Length(_vScale)) != 0.f) m_bDirtyFlag = true;
		XMStoreFloat3(&m_vDeltaTransform[0], XMLoadFloat3(&m_vDeltaTransform[0]) + _vScale);
	}
	void Set_DeltaRotation(_fvector _vRotation)
	{
		if (XMVectorGetX(XMVector3Length(_vRotation)) != 0.f) m_bDirtyFlag = true;
		XMStoreFloat3(&m_vDeltaTransform[1], XMLoadFloat3(&m_vDeltaTransform[1]) + _vRotation);
	}
	void Set_DeltaPosition(_fvector _vPosition)
	{
		if (XMVectorGetX(XMVector3Length(_vPosition)) != 0.f) m_bDirtyFlag = true;
		XMStoreFloat3(&m_vDeltaTransform[2], XMLoadFloat3(&m_vDeltaTransform[2]) + _vPosition);
	}

	void Set_RelativeScale(_fvector _vScale)
	{ 
		m_bDirtyFlag = true;
		XMStoreFloat3(&m_vRelativeTransform[0], _vScale); 
	}
	void Set_RelativeRotation(_fvector _vRotation)
	{
		m_bDirtyFlag = true;
		XMStoreFloat3(&m_vRelativeTransform[1], _vRotation); 
	}
	void Set_RelativePosition(_fvector _vPosition)
	{ 
		m_bDirtyFlag = true;
		XMStoreFloat3(&m_vRelativeTransform[2], _vPosition); 
	}

public :
	void Update_FinalTransform();
	void Update_Transform(_fvector _vScale, _gvector _vRotation, _hvector _vPosition);

	void Update_Scale(_fvector _vScale);
	void Update_Rotation(_fvector _vRotation);
	void Update_Position(_fvector _vPosition);

public :
	void LookAt(_fvector _vAt);

public :
	void AttachToComponent(CSceneComponent* _pAttachComponent);
	void AttachToComponent(CSceneComponent* _pAttachComponent, const _float4x4* _pSocketMatrix);

	void Set_InheritRotation(_bool _bInheritRotation) { m_bInheritRotation = _bInheritRotation; }

public :
	virtual void Clear() override;

protected :
	CSceneComponent* m_pAttachComponent{};

	list<CSceneComponent*> m_listChildComponent{};

protected :
	_float4x4 m_matWorld{};
	_float4x4 m_matRelative{};

	_float3 m_vDeltaTransform[static_cast<_uint>(ETransform::TRANSFORM_END)]{};
	_float3 m_vRelativeTransform[static_cast<_uint>(ETransform::TRANSFORM_END)]{};

protected :
	const _float4x4* m_pSocketMatrix{};

protected :
	_bool m_bInheritRotation{ true };

protected :
	_bool m_bDirtyFlag{};

private :
	_float3 QuaternionToEuler(_fvector _vQuaternion);
	_float3 AdjustRotation(_fvector _vRotation);
	void Reset_DeltaTransform();

public :
	static CSceneComponent* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END