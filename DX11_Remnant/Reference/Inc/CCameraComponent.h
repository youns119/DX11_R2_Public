#pragma once

#include "CSceneComponent.h"

#include "FCameraComponent.h"

BEGIN(Engine)

class ENGINE_DLL CCameraComponent final
	: public CSceneComponent
{
protected :
	CCameraComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CCameraComponent(const CCameraComponent& _Prototype);
	virtual ~CCameraComponent() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;

public :
	_bool Get_CameraFix() { return m_bCameraFix; }

public :
	void Camera_Fix(HWND _hWnd, _bool _bCameraFix);
	void Zoom(_float _fRatio);
	void Shake();

private :
	_float m_fBaseFov{};
	_float m_fFov{};
	_float m_fNear{}, m_fFar{};

private :
	_float m_fAspect{};

private :
	HWND m_hWnd{};

private :
	_bool m_bCameraFix{};
	_bool m_bShowCursor{ true };

private :
	_bool m_bZoomIn{};
	_bool m_bZoomOut{};
	_float m_fFovRatio{ 1.f };
	_float m_fDstRatio{};
	_float m_fZoomSpeed{ 0.03f };

private :
	_bool m_bShake{};
	_float m_fRecoil{ 1.f };
	_float m_fRecoilRatio{ 0.f };
	_float m_fRecoilSpeed{ 0.1f };


private :
	void Camear_Zoom();
	void Camera_Shake();

public :
	static CCameraComponent* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END