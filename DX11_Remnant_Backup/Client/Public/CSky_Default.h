#pragma once

#include "Client_Define.h"

#include "CSkyBox.h"

BEGIN(Client)

class CSky_Default final 
	: public CSkyBox
{
private :
	CSky_Default(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSky_Default(const CSky_Default& _Prototype);
	virtual ~CSky_Default() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private :
	virtual HRESULT Ready_Component() override;
	virtual HRESULT Ready_Resource() override;

public :
	static CSky_Default* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END