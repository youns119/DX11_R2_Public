#pragma once

#include "CActor.h"

#include "FSkyBox.h"

BEGIN(Engine)

class CCubeComponent;

class ENGINE_DLL CSkyBox abstract
	: public CActor
{
protected :
	CSkyBox(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext);
	CSkyBox(const CSkyBox& _Prototype);
	virtual ~CSkyBox() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Update(_float _fTimeDelta) override;

protected :
	CCubeComponent* m_pCubeComponent{};

public :
	virtual CClonable* Clone(void* _pDesc) PURE;
	virtual void Free() override;
};

END