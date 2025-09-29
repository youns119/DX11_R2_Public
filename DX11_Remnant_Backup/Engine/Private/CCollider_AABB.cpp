#include "CCollider_AABB.h"

#include "FColliderComponent.h"

CCollider_AABB::CCollider_AABB(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CCollider{ _pDevice, _pContext }
{
}

HRESULT CCollider_AABB::Init(void* _pDesc)
{
	FColliderComponent* pColliderComponentDesc = static_cast<FColliderComponent*>(_pDesc);
	_float3 vPosition = pColliderComponentDesc->vPosition;
	_float3 vExtent = pColliderComponentDesc->vScale;

	m_pOriginalDesc = new BoundingBox(vPosition, vExtent);
	m_pDesc = new BoundingBox(*m_pOriginalDesc);

	return S_OK;
}

void CCollider_AABB::Update(_fmatrix _matWorld)
{
	_matrix matTransform = _matWorld;

	matTransform.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(_matWorld.r[0]);
	matTransform.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(_matWorld.r[1]);
	matTransform.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(_matWorld.r[2]);

	m_pOriginalDesc->Transform(*m_pDesc, matTransform);
}

HRESULT CCollider_AABB::Render(PrimitiveBatch<VertexPositionColor>* _pBatch, _fvector _vColor)
{
	DX::Draw(_pBatch, *m_pDesc, _vColor);

	return S_OK;
}

_bool CCollider_AABB::Intersect(ECollider _eCollider, CCollider* _pTargetCollider)
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

CCollider_AABB* CCollider_AABB::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, void* _pDesc)
{
	CCollider_AABB* pInstance = new CCollider_AABB(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Created : CCollider_AABB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_AABB::Free()
{
	__super::Free();

	Safe_Delete(m_pDesc);
	Safe_Delete(m_pOriginalDesc);
}