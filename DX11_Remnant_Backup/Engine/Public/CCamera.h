#pragma once

#include "CActor.h"

#include "FCamera.h"

BEGIN(Engine)

class CCameraComponent;

class ENGINE_DLL CCamera abstract
	: public CActor
{
protected :
	CCamera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CCamera(const CCamera& _Prototype);
	virtual ~CCamera() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;

protected :
	CCameraComponent* m_pCameraComponent{};

protected :
	_float m_fMouseSensor{};

public :
	virtual CClonable* Clone(void* _pDesc) PURE;
	virtual void Free() override;
};

END