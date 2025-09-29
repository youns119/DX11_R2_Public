#pragma once

#include "Client_Define.h"

#include "CCamera.h"

BEGIN(Engine)

class CMovementComponent;

END

BEGIN(Client)

class CCamera_Free final
	: public CCamera
{
private :
	CCamera_Free(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CCamera_Free(const CCamera_Free& _Prototype);
	virtual ~CCamera_Free() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;

private :
	CMovementComponent* m_pMovementComponent{};

private :
	_float m_fCamSpeed{};

private :
	virtual HRESULT Ready_Component() override;
	virtual HRESULT Ready_Resource() override { return S_OK; }

public :
	static CCamera_Free* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END