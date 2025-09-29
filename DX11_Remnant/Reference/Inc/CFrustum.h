#pragma once

#include "IFrustum.h"
#include "CObject.h"

BEGIN(Engine)

class ENGINE_DLL CFrustum final
	: public IFrustum
	, public CObject
{
private :
	CFrustum();
	virtual ~CFrustum() = default;

public :
	HRESULT Init();
	virtual void Update() override;

public :
	virtual _bool IsIn_WorldSpace(_fvector _vPosition, _float _fRange = 0.f) override;

private :
	_float4 m_vOriginalPoints[8]{};
	_float4 m_vWorldPoints[8]{};
	_float4 m_vWorldPlanes[6]{};

private :
	void Make_Planes(const _float4* _pPoints, _float4* _pPlanes);

public :
	static CFrustum* Create();
	virtual void Free() override;
};

END