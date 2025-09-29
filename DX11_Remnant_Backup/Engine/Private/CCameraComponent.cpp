#include "CCameraComponent.h"

#include "CGameInstance.h"

CCameraComponent::CCameraComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSceneComponent{ _pDevice, _pContext }
{
	m_strName = g_strCameraComponent;
}

CCameraComponent::CCameraComponent(const CCameraComponent& _Prototype)
	: CSceneComponent(_Prototype)
{
}

HRESULT CCameraComponent::Init()
{
	if (FAILED(__super::Init()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCameraComponent::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FCameraComponent* pDesc = static_cast<FCameraComponent*>(_pDesc);
	m_fBaseFov = pDesc->fFov;
	m_fFov = pDesc->fFov;
	m_fNear = pDesc->fNear;
	m_fFar = pDesc->fFar;

	if (FAILED(__super::Init(_pDesc)))
		return E_FAIL;

	_uint2 vViewportSize = CGameInstance::Get_ViewportSize();
	m_fAspect = vViewportSize.x / static_cast<_float>(vViewportSize.y);

	return S_OK;
}

void CCameraComponent::Pre_Update(_float _fTimeDelta)
{
	__super::Pre_Update(_fTimeDelta);

	if (CGameInstance::Get_Controller() == m_pOwner)
	{
		CGameInstance::Set_Transform
		(ED3DTransform::D3DTS_VIEW, Get_WorldMatrix_Inverse());
		CGameInstance::Set_Transform
		(ED3DTransform::D3DTS_PROJ, XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fNear, m_fFar));
	}
}

void CCameraComponent::Update(_float _fTimeDelta)
{
	Camear_Zoom();
	Camera_Shake();

	if (m_bCameraFix)
	{
		_uint2 iViewportSize = CGameInstance::Get_ViewportSize();

		POINT tMousePos{ iViewportSize.x >> 1, iViewportSize.y >> 1 };

		ClientToScreen(m_hWnd, &tMousePos);
		SetCursorPos(tMousePos.x, tMousePos.y);
	}

	__super::Update(_fTimeDelta);
}

void CCameraComponent::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

void CCameraComponent::Camera_Fix(HWND _hWnd, _bool _bCameraFix)
{
	m_hWnd = _hWnd;
	m_bCameraFix = _bCameraFix;

	if (m_bCameraFix) m_bShowCursor = false;
	else m_bShowCursor = true;

	ShowCursor(m_bShowCursor);
}

void CCameraComponent::Zoom(_float _fRatio)
{
	if (m_fFovRatio > _fRatio)
	{
		m_bZoomIn = true;
		m_bZoomOut = false;
		m_fDstRatio = _fRatio;
	}
	else
		m_bZoomIn = false;

	if (m_fFovRatio < _fRatio)
	{
		m_bZoomIn = false;
		m_bZoomOut = true;
		m_fDstRatio = _fRatio;
	}
	else
		m_bZoomOut = false;
}

void CCameraComponent::Shake()
{
	m_bShake = true;

	m_fRecoilRatio = 1.f;
}

void CCameraComponent::Camear_Zoom()
{
	if (m_bZoomIn)
	{
		m_fFovRatio -= m_fZoomSpeed;

		if (m_fFovRatio <= m_fDstRatio)
		{
			m_fFovRatio = m_fDstRatio;
			m_bZoomIn = false;
		}
	}

	if (m_bZoomOut)
	{
		m_fFovRatio += m_fZoomSpeed;

		if (m_fFovRatio >= m_fDstRatio)
		{
			m_fFovRatio = m_fDstRatio;
			m_bZoomOut = false;
		}
	}

	m_fFov = m_fBaseFov * m_fFovRatio;
}

void CCameraComponent::Camera_Shake()
{
	if (m_bShake)
	{
		_float fRecoil = m_fRecoil * m_fRecoilRatio;
		
		m_fRecoilRatio -= m_fRecoilSpeed;
		if (m_fRecoilRatio < 0.f)
		{
			m_fRecoilRatio = 0.f;
			m_bShake = false;
		}

		m_pAttachComponent->Set_DeltaRotation(XMVectorSet(-fRecoil, 0.f, 0.f, 1.f));
	}
}

CCameraComponent* CCameraComponent::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CCameraComponent* pInstance = new CCameraComponent(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Create : CCameraComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CClonable* CCameraComponent::Clone(void* _pDesc)
{
	CCameraComponent* pInstance = new CCameraComponent(*this);

	if (FAILED(pInstance->Init(_pDesc)))
	{
		MSG_BOX("Failed To Create : CCameraComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCameraComponent::Free()
{
	__super::Free();
}