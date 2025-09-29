#include "CCollider_OBB.h"

#include "FColliderComponent.h"

CCollider_OBB::CCollider_OBB(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CCollider{ _pDevice, _pContext }
{
}

HRESULT CCollider_OBB::Init(void* _pDesc)
{
	FColliderComponent* pColliderComponentDesc = static_cast<FColliderComponent*>(_pDesc);
	_float3 vPosition = pColliderComponentDesc->vPosition;
	_float3 vExtent = pColliderComponentDesc->vScale;
	_float4 vOrientation{};

	XMStoreFloat4
	(
		&vOrientation, 
		XMQuaternionRotationRollPitchYaw
		(
			pColliderComponentDesc->vRotation.x, 
			pColliderComponentDesc->vRotation.y,
			pColliderComponentDesc->vRotation.z
		)
	);

	m_pOriginalDesc = new BoundingOrientedBox(vPosition, vExtent, vOrientation);
	m_pDesc = new BoundingOrientedBox(*m_pOriginalDesc);

	return S_OK;
}

void CCollider_OBB::Update(_fmatrix _matWorld)
{
	m_pOriginalDesc->Transform(*m_pDesc, _matWorld);
}

HRESULT CCollider_OBB::Render(PrimitiveBatch<VertexPositionColor>* _pBatch, _fvector _vColor)
{
	DX::Draw(_pBatch, *m_pDesc, _vColor);

	return S_OK;
}

_bool CCollider_OBB::Intersect(ECollider _eCollider, CCollider* _pTargetCollider)
{
	void* pTargetDesc = _pTargetCollider->Get_Desc();

	_bool bCollided{ false };

	switch (_eCollider)
	{
	case ECollider::COLLIDER_AABB:
		bCollided = m_pDesc->Intersects(*static_cast<BoundingBox*>(pTargetDesc));
		break;
	case ECollider::COLLIDER_OBB:
		bCollided = m_pDesc->Intersects(*static_cast<BoundingOrientedBox*>(pTargetDesc));
		break;
	case ECollider::COLLIDER_SPHERE:
		bCollided = m_pDesc->Intersects(*static_cast<BoundingSphere*>(pTargetDesc));
		break;
	}

	return bCollided;
}

CCollider_OBB* CCollider_OBB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* _pDesc)
{
	CCollider_OBB* pInstance = new CCollider_OBB(pDevice, pContext);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Created : CCollider_OBB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_OBB::Free()
{
	__super::Free();

	Safe_Delete(m_pDesc);
	Safe_Delete(m_pOriginalDesc);
}