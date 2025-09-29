#pragma once

#include "Client_Define.h"

#include "CActor.h"

BEGIN(Engine)

class CUIComponent;

END

BEGIN(Client)

class CLoadingUI final
	: public CActor
{
private :
	CLoadingUI(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CLoadingUI(const CLoadingUI& _Prototype);
	virtual ~CLoadingUI() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private :
	CUIComponent* m_pLogo{};

private :
	virtual HRESULT Ready_Component() override;
	virtual HRESULT Ready_Resource() override;

public :
	static CLoadingUI* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END