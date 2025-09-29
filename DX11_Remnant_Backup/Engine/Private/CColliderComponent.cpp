#include "CColliderComponent.h"

#include "CGameInstance.h"
#include "CActor.h"
#include "CCollider_AABB.h"
#include "CCollider_OBB.h"
#include "CCollider_Sphere.h"

CColliderComponent::CColliderComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPrimitiveComponent{ _pDevice, _pContext }
{
}

CColliderComponent::CColliderComponent(const CColliderComponent& _Prototype)
	: CPrimitiveComponent(_Prototype)
	, m_eCollider{ _Prototype.m_eCollider }
#ifdef _DEBUG

	, m_pBatch{ _Prototype.m_pBatch }
	, m_pEffect{ _Prototype.m_pEffect }
	, m_pInputLayout{ _Prototype.m_pInputLayout }

#endif
{
#ifdef _DEBUG

	Safe_AddRef(m_pInputLayout);

#endif
}

HRESULT CColliderComponent::Init(ECollider _eCollider)
{
	m_eCollider = _eCollider;

#ifdef _DEBUG

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode{};
	size_t iShaderByteCodeLength{};

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout
	(
		VertexPositionColor::InputElements, 
		VertexPositionColor::InputElementCount, 
		pShaderByteCode, 
		iShaderByteCodeLength, 
		&m_pInputLayout
	)))
		return E_FAIL;

#endif

	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CColliderComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FColliderComponent* pColliderComponentDesc = static_cast<FColliderComponent*>(_pDesc);

	switch (m_eCollider)
	{
	case ECollider::COLLIDER_AABB:
		m_pCollider = CCollider_AABB::Create(m_pDevice, m_pContext, pColliderComponentDesc);
		m_strName = g_strColliderComponent_AABB;
		break;
	case ECollider::COLLIDER_OBB:
		m_pCollider = CCollider_OBB::Create(m_pDevice, m_pContext, pColliderComponentDesc);
		m_strName = g_strColliderComponent_OBB;
		break;
	case ECollider::COLLIDER_SPHERE:
		m_pCollider = CCollider_Sphere::Create(m_pDevice, m_pContext, pColliderComponentDesc);
		m_strName = g_strColliderComponent_Sphere;
		break;
	}

	if (m_pCollider == nullptr)
		return E_FAIL;

	pColliderComponentDesc->vScale = _float3(1.f, 1.f, 1.f);
	pColliderComponentDesc->vRotation = _float3(0.f, 0.f, 0.f);
	pColliderComponentDesc->vPosition = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Init(pColliderComponentDesc)))
		return E_FAIL;

	return S_OK;
}

void CColliderComponent::Update(_float _fTimeDelta)
{
	m_pCollider->Update(Get_WorldMatrix());

	__super::Update(_fTimeDelta);
}

void CColliderComponent::Post_Update(_float _fTimeDelta)
{
	if (m_pOwner->Get_Show())
	{
		if (m_bCulling)
		{
			_vector vPosition = Get_TransformState(ETransformState::TRANSFORMSTATE_POS);

			if (CGameInstance::IsIn_WorldSpace(vPosition, m_fCullingRange))
				CGameInstance::Add_DebugComponent(this);
		}
		else
			CGameInstance::Add_DebugComponent(this);
	}

	CSceneComponent::Post_Update(_fTimeDelta);
}

#ifdef _DEBUG

HRESULT CColliderComponent::Render()
{
	if (m_pCollider == nullptr)
		return E_FAIL;

	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(CGameInstance::Get_TransformMatrix(ED3DTransform::D3DTS_VIEW));
	m_pEffect->SetProjection(CGameInstance::Get_TransformMatrix(ED3DTransform::D3DTS_PROJ));
	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();

	m_pCollider->Render
	(
		m_pBatch, 
		m_bCollided == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f)
	);

	m_pBatch->End();

	return S_OK;
}

#endif

_bool CColliderComponent::Intersect(CColliderComponent* _pTargetCollider)
{
	m_bCollided = m_pCollider->Intersect(_pTargetCollider->m_eCollider, _pTargetCollider->m_pCollider);

	return m_bCollided;
}

CColliderComponent* CColliderComponent::Create
(
	ID3D11Device* _pDevice,
	ID3D11DeviceContext* _pContext, 
	ECollider _eCollider
)
{
	CColliderComponent* pInstance = new CColliderComponent(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_eCollider)))
	{
		MSG_BOX("Failed To Created : CColliderComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CColliderComponent::Clone(void* _pDesc)
{
	CColliderComponent* pInstance = new CColliderComponent(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Created : CColliderComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CColliderComponent::Free()
{
	__super::Free();

#ifdef _DEBUG

	Safe_Release(m_pInputLayout);

	if (!m_bCloned)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}

#endif

	Safe_Release(m_pCollider);
}