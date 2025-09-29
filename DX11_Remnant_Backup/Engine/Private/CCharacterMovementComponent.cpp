#include "CCharacterMovementComponent.h"

#include "CSceneComponent.h"

CCharacterMovementComponent::CCharacterMovementComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CMovementComponent{ _pDevice, _pContext }
{
	m_strName = g_strCharacterMovementComponent;
}

CCharacterMovementComponent::CCharacterMovementComponent(const CCharacterMovementComponent& _Prototype)
	: CMovementComponent(_Prototype)
{
}

HRESULT CCharacterMovementComponent::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharacterMovementComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FCharacterMovementComponent* pDesc = static_cast<FCharacterMovementComponent*>(_pDesc);
	m_fMoveAccel = pDesc->fMoveAccel * m_fScaling;
	m_fMoveDecel = pDesc->fMoveDecel * m_fScaling;
	m_fMaxSpeed = pDesc->fMaxVelocity * m_fScaling;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	return S_OK;
}

void CCharacterMovementComponent::Pre_Update(_float _fTimeDelta)
{
	if (m_pUpdateComponent == nullptr)
		return;

	m_pUpdateComponent->Set_DeltaPosition(XMLoadFloat3(&m_vVelocity));

	__super::Pre_Update(_fTimeDelta);
}

void CCharacterMovementComponent::Post_Update(_float _fTimeDelta)
{
	Decrease_Velocity(_fTimeDelta);

	__super::Post_Update(_fTimeDelta);
}

void CCharacterMovementComponent::Add_Velocity(_fvector _vDir, _float _fTimeDelta)
{
	_vector vVelocity = XMVector3Normalize(_vDir) * m_fMoveAccel * _fTimeDelta * 10.f;
	vVelocity = XMLoadFloat3(&m_vVelocity) + vVelocity;

	if (XMVectorGetX(XMVector3Length(vVelocity)) >= m_fMaxSpeed)
		vVelocity = XMVector3Normalize(vVelocity) * m_fMaxSpeed * 0.9f;

	XMStoreFloat3(&m_vVelocity, vVelocity);
}

void CCharacterMovementComponent::Decrease_Velocity(_float _fTimeDelta)
{
	_vector vVelocity = XMVector3Normalize(XMLoadFloat3(&m_vVelocity)) * -m_fMoveDecel * _fTimeDelta * 10.f;
	_vector vPreVelocity = XMLoadFloat3(&m_vVelocity);

	vVelocity = XMLoadFloat3(&m_vVelocity) + vVelocity;

	_float fVelX = XMVectorGetX(vVelocity);
	_float fVelY = XMVectorGetY(vVelocity);
	_float fVelZ = XMVectorGetZ(vVelocity);

	if (XMVectorGetX(vPreVelocity) * fVelX <= 0) fVelX = 0.f;
	if (XMVectorGetY(vPreVelocity) * fVelY <= 0) fVelY = 0.f;
	if (XMVectorGetZ(vPreVelocity) * fVelZ <= 0) fVelZ = 0.f;

	vVelocity = XMVectorSet(fVelX, fVelY, fVelZ, 1.f);
	XMStoreFloat3(&m_vVelocity, vVelocity);
}

CCharacterMovementComponent* CCharacterMovementComponent::Create
(
	ID3D11Device* _pDevice, 
	ID3D11DeviceContext* _pContext
)
{
	CCharacterMovementComponent* pInstance = new CCharacterMovementComponent(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CCharacterMovementComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CCharacterMovementComponent::Clone(void* _pDesc)
{
	CCharacterMovementComponent* pInstance = new CCharacterMovementComponent(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CCharacterMovementComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCharacterMovementComponent::Free()
{
	__super::Free();
}