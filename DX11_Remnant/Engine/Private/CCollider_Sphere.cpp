#include "CCollider_Sphere.h"

#include "FColliderComponent.h"

CCollider_Sphere::CCollider_Sphere(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CCollider{ _pDevice, _pContext }
{
}

HRESULT CCollider_Sphere::Init(void* _pDesc)
{
	FColliderComponent* pColliderComponentDesc = static_cast<FColliderComponent*>(_pDesc);

	_float3 vPosition = pColliderComponentDesc->vPosition;
	_float fRadius = pColliderComponentDesc->vScale.x;

	m_pOriginalDesc = new BoundingSphere(vPosition, fRadius);
	m_pDesc = new BoundingSphere(*m_pOriginalDesc);

	return S_OK;
}

void CCollider_Sphere::Update(_fmatrix _matWorld)
{
	m_pOriginalDesc->Transform(*m_pDesc, _matWorld);
}

HRESULT CCollider_Sphere::Render(PrimitiveBatch<VertexPositionColor>* _pBatch, _fvector _vColor)
{
	DX::Draw(_pBatch, *m_pDesc, _vColor);

	return S_OK;
}

_bool CCollider_Sphere::Intersect(ECollider _eCollider, CCollider* _pTargetCollider)
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

CCollider_Sphere* CCollider_Sphere::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, void* _pDesc)
{
	CCollider_Sphere* pInstance = new CCollider_Sphere(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Created : CCollider_Sphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Sphere::Free()
{
	__super::Free();

	Safe_Delete(m_pDesc);
	Safe_Delete(m_pOriginalDesc);
}