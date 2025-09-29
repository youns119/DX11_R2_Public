#include "CMovementComponent.h"

#include "CActor.h"
#include "CSceneComponent.h"

CMovementComponent::CMovementComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CActorComponent{ _pDevice, _pContext }
{
	m_strName = g_strMovementComponent;
}

CMovementComponent::CMovementComponent(const CMovementComponent& _Prototype)
	: CActorComponent(_Prototype)
{
}

HRESULT CMovementComponent::Init()
{
	return S_OK;
}

HRESULT CMovementComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FMovementComponent* pDesc = static_cast<FMovementComponent*>(_pDesc);
	m_fMoveSpeed = pDesc->fMoveSpeed;
	m_fRotationSpeed = pDesc->fRotationSpeed;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

void CMovementComponent::Pre_Update(_float _fTimeDelta)
{
}

void CMovementComponent::Set_UpdateComponent(CSceneComponent* _pSceneComponent)
{
	m_pUpdateComponent = _pSceneComponent;
	Safe_AddRef(_pSceneComponent);

	_uint iCurrID = m_iComponentID;
	_uint iUpdateID = _pSceneComponent->Get_ComponentID();

	m_pOwner->Get_UpdateGraph()->Add_Edge(iCurrID, iUpdateID);
	m_pOwner->Update_Graph();
}

void CMovementComponent::Move(_fvector _vDir, _float _fTimeDelta)
{
	_vector vDeltaPos = _vDir * m_fMoveSpeed * _fTimeDelta;

	m_pUpdateComponent->Set_DeltaPosition(vDeltaPos);
}

void CMovementComponent::Turn(_fvector _vRotation, _float _fTimeDelta)
{
	_float fRadian = m_fRotationSpeed * _fTimeDelta;
	_vector vRotation = XMVectorSet
	(
		XMVectorGetX(_vRotation) * fRadian,
		XMVectorGetY(_vRotation) * fRadian,
		XMVectorGetZ(_vRotation) * fRadian,
		0.f
	);

	m_pUpdateComponent->Set_DeltaRotation(vRotation);
}

CMovementComponent* CMovementComponent::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CMovementComponent* pInstance = new CMovementComponent(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CMovementComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CMovementComponent::Clone(void* _pDesc)
{
	CMovementComponent* pInstance = new CMovementComponent(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CMovementComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMovementComponent::Free()
{
	__super::Free();

	Safe_Release(m_pUpdateComponent);
}