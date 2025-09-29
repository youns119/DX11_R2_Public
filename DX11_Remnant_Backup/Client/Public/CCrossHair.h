#pragma once

#include "Client_Define.h"

#include "CActor.h"

#include "ECrossHair.h"

BEGIN(Engine)

class CUIComponent;

END

BEGIN(Client)

class CCrossHair final 
	: public CActor
{
private :
	CCrossHair(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CCrossHair(const CCrossHair& _Prototype);
	virtual ~CCrossHair() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public :
	void Set_CrossHair(ECrossHair _eCrossHair);

private :
	CUIComponent* m_UIs[static_cast<_uint>(ECrossHair::CROSSHAIR_END)]{};

private :
	virtual HRESULT Ready_Component() override;
	virtual HRESULT Ready_Resource() override;

public :
	static CCrossHair* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END