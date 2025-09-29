#pragma once

#include "CPrimitiveComponent.h"

#include "FColliderComponent.h"
#include "ECollider.h"

BEGIN(Engine)

class CCollider;

class ENGINE_DLL CColliderComponent final 
	: public CPrimitiveComponent
{
private :
	CColliderComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CColliderComponent(const CColliderComponent& _Prototype);
	virtual ~CColliderComponent() = default;

public :
	virtual HRESULT Init(ECollider _eCollider);
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;

#ifdef _DEBUG

public :
	virtual HRESULT Render();

#endif

public :
	_bool Intersect(CColliderComponent* _pTargetCollider);

private :
	CCollider* m_pCollider{};

private :
	ECollider m_eCollider{ ECollider::COLLIDER_END };

	_bool m_bCollided{};

#ifdef _DEBUG

private :
	PrimitiveBatch<DirectX::VertexPositionColor>* m_pBatch{};
	BasicEffect* m_pEffect{};
	ID3D11InputLayout* m_pInputLayout{};

#endif

public :
	static CColliderComponent* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext, 
		ECollider _eCollider
	);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END