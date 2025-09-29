#pragma once

#include "Client_Define.h"

#include "CLight.h"

BEGIN(Client)

class CLight_Directional final
	: public CLight
{
private :
	CLight_Directional(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CLight_Directional(const CLight_Directional& _Prototype);
	virtual ~CLight_Directional() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;

private :
	virtual HRESULT Ready_Component() override;
	virtual HRESULT Ready_Resource() override { return S_OK; }

public :
	static CLight_Directional* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END