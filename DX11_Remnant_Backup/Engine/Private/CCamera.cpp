#include "CCamera.h"

#include "CGameInstance.h"
#include "CCameraComponent.h"
#include "CMovementComponent.h"

CCamera::CCamera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CActor{ _pDevice, _pContext }
{
}

CCamera::CCamera(const CCamera& _Prototype)
	: CActor(_Prototype)
{
}

HRESULT CCamera::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FCamera* pCameraDesc = static_cast<FCamera*>(_pDesc);
	m_fMouseSensor = pCameraDesc->fMouseSensor;

	FCameraComponent tCameraComponent{};
	tCameraComponent.pOwner = this;
	tCameraComponent.vRotation = pCameraDesc->vRotation;
	tCameraComponent.vPosition = pCameraDesc->vPosition;
	tCameraComponent.fFov = pCameraDesc->fFov;
	tCameraComponent.fNear = pCameraDesc->fNear;
	tCameraComponent.fFar = pCameraDesc->fFar;

	m_pCameraComponent = CCameraComponent::Create(m_pDevice, m_pContext);
	if (m_pCameraComponent == nullptr ||
		FAILED(m_pCameraComponent->Init(&tCameraComponent)))
		return E_FAIL;

	Add_ActorComponent(g_strCameraComponent, m_pCameraComponent);
	Set_RootComponent(m_pCameraComponent);

	FActor tActor{};
	tActor.pOuter = pCameraDesc->pOuter;

	if (FAILED(__super::Init(&tActor)))
		return E_FAIL;

	return S_OK;
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pCameraComponent);
}