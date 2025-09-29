#pragma once

#include "CActor.h"

#include "FLight.h"

BEGIN(Engine)

class CLightComponent;

class ENGINE_DLL CLight abstract
	: public CActor
{
protected :
	CLight(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CLight(const CLight& _Prototype);
	virtual ~CLight() = default;

public :
	virtual HRESULT Init();
	virtual HRESULT Init(void* _pDesc) override;

public :
	void Set_LightOnOff(_bool _bOnOff);

protected :
	CLightComponent* m_pLightComponent{};

public :
	virtual void Free() override;
};

END