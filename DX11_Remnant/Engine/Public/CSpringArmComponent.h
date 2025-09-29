#pragma once

#include "CSceneComponent.h"

#include "FSpringArmComponent.h"

BEGIN(Engine)

class CSceneComponent;

class ENGINE_DLL CSpringArmComponent final
	: public CSceneComponent
{
private :
	CSpringArmComponent(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSpringArmComponent(const CSpringArmComponent& _Prototype);
	virtual ~CSpringArmComponent() = default;

public :
	virtual HRESULT Init();
	virtual HRESULT Init(void* _pDesc);
	virtual void Pre_Update(_float _fTimeDelta);
	virtual void Update(_float _fTimeDelta);
	virtual void Post_Update(_float _fTimeDelta);

private :
	_matrix m_matOffset{};

private :
	_float m_fArmLength{};

public :
	static CSpringArmComponent* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END