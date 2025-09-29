#include "CSpringArmComponent.h"

CSpringArmComponent::CSpringArmComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSceneComponent{ _pDevice, _pContext }
{
	m_strName = g_strSpringArmComponent;
}

CSpringArmComponent::CSpringArmComponent(const CSpringArmComponent& _Prototype)
	: CSceneComponent(_Prototype)
	//, m_matOffset{ _Prototype.m_matOffset }
	, m_fArmLength{ _Prototype.m_fArmLength }
{
}

HRESULT CSpringArmComponent::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	//m_matOffset = XMMatrixIdentity();

	return S_OK;
}

HRESULT CSpringArmComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FSpringArmComponent* pSpringArmDesc = static_cast<FSpringArmComponent*>(_pDesc);
	m_fArmLength = pSpringArmDesc->fArmLength;

	_vector vDir = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	vDir = XMVector3Normalize(vDir) * -1.f;
	
	_vector vPosition{};
	vPosition = vDir * m_fArmLength;
	vPosition = vPosition + 
		XMVector3Normalize(Get_TransformState(ETransformState::TRANSFORMSTATE_RIGHT)) * pSpringArmDesc->vOffset.x;
	vPosition = vPosition + 
		XMVector3Normalize(Get_TransformState(ETransformState::TRANSFORMSTATE_LOOK)) * pSpringArmDesc->vOffset.z;
	vPosition = vPosition + 
		XMVector3Normalize(Get_TransformState(ETransformState::TRANSFORMSTATE_UP)) * pSpringArmDesc->vOffset.y;

	_float3 vPos{};
	XMStoreFloat3(&vPos, vPosition);

	_matrix matTranslation = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	m_matOffset = matTranslation;

	if (FAILED(__super::Init(pSpringArmDesc)))
		return E_FAIL;

	return S_OK;
}

void CSpringArmComponent::Pre_Update(_float _fTimeDelta)
{
	if (m_pAttachComponent == nullptr)
		return;

	__super::Pre_Update(_fTimeDelta);

	XMStoreFloat4x4
	(
		&m_matWorld,
		m_matOffset *
		XMLoadFloat4x4(&m_matWorld)
	);
}

void CSpringArmComponent::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CSpringArmComponent::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

CSpringArmComponent* CSpringArmComponent::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CSpringArmComponent* pInstance = new CSpringArmComponent(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CSpringArmComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CSpringArmComponent::Clone(void* _pDesc)
{
	CSpringArmComponent* pInstance = new CSpringArmComponent(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CSpringArmComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpringArmComponent::Free()
{
	__super::Free();
}