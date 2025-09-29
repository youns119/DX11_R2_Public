#include "pch.h"
#include "CCamera_Free.h"

#include "CGameInstance.h"
#include "CCameraComponent.h"
#include "CMovementComponent.h"

#include "ELevel.h"

CCamera_Free::CCamera_Free(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CCamera{ _pDevice, _pContext }
{
}

CCamera_Free::CCamera_Free(const CCamera_Free& _Prototype)
	: CCamera(_Prototype)
{
}

HRESULT CCamera_Free::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Free::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FCamera* pCameraDesc = static_cast<FCamera*>(_pDesc);
	pCameraDesc->fMouseSensor = 1.f;
	pCameraDesc->fFov = XMConvertToRadians(60.f);
	pCameraDesc->fNear = 0.1f;
	pCameraDesc->fFar = 1000.f;

	if (FAILED(__super::Init(pCameraDesc)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Free::Pre_Update(_float _fTimeDelta)
{
	if (CGameInstance::Get_Controller() == this)
	{
		if (CGameInstance::Key_Press(DIK_TAB))
			m_pCameraComponent->Camera_Fix(g_hWnd, !m_pCameraComponent->Get_CameraFix());

		if (CGameInstance::Key_Hold(DIK_LSHIFT))
			m_fCamSpeed = 5.f;
		else
			m_fCamSpeed = 1.f;

		if (CGameInstance::Key_Hold(DIK_W))
			m_pMovementComponent->Move
			(m_pRootComponent->Get_TransformState(ETransformState::TRANSFORMSTATE_LOOK), _fTimeDelta * m_fCamSpeed);
		if (CGameInstance::Key_Hold(DIK_S))
			m_pMovementComponent->Move
			(m_pRootComponent->Get_TransformState(ETransformState::TRANSFORMSTATE_LOOK), -_fTimeDelta * m_fCamSpeed);
		if (CGameInstance::Key_Hold(DIK_D))
			m_pMovementComponent->Move
			(m_pRootComponent->Get_TransformState(ETransformState::TRANSFORMSTATE_RIGHT), _fTimeDelta * m_fCamSpeed);
		if (CGameInstance::Key_Hold(DIK_A))
			m_pMovementComponent->Move
			(m_pRootComponent->Get_TransformState(ETransformState::TRANSFORMSTATE_RIGHT), -_fTimeDelta * m_fCamSpeed);

		_long lMouseMove{};

		if (CGameInstance::Mouse_Hold(EMouseKey::MOUSEKEY_MB) || m_pCameraComponent->Get_CameraFix())
		{
			if (lMouseMove = CGameInstance::Get_DIMouseMove(EMouseDir::MOUSEDIR_X))
				m_pMovementComponent->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), _fTimeDelta * lMouseMove * m_fMouseSensor);
			if (lMouseMove = CGameInstance::Get_DIMouseMove(EMouseDir::MOUSEDIR_Y))
				m_pMovementComponent->Turn(XMVectorSet(1.f, 0.f, 0.f, 0.f), _fTimeDelta * lMouseMove * m_fMouseSensor);
		}
	}

	__super::Pre_Update(_fTimeDelta);
}

void CCamera_Free::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CCamera_Free::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CCamera_Free::Ready_Component()
{
	// Component_Movement
	FMovementComponent tMovementDesc{};
	tMovementDesc.pOwner = this;
	tMovementDesc.fMoveSpeed = 10.f;
	tMovementDesc.fRotationSpeed = XMConvertToRadians(90.f);

	m_pMovementComponent = static_cast<CMovementComponent*>(Add_ActorComponent
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		g_strMovementComponent,
		g_strMovementComponent,
		&tMovementDesc
	));
	if (m_pMovementComponent == nullptr)
		return E_FAIL;

	m_pMovementComponent->Set_UpdateComponent(m_pRootComponent);

	return S_OK;
}

CCamera_Free* CCamera_Free::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CCamera_Free* pInstance = new CCamera_Free(_pDevice, _pContext);
	
	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CCamera_Free::Clone(void* _pDesc)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();

	Safe_Release(m_pMovementComponent);
}