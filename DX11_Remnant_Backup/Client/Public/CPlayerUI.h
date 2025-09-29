#pragma once

#include "Client_Define.h"

#include "CActor.h"

#include "EPlayerUI.h"
#include "ECrossHair.h"

BEGIN(Engine)

class CUIComponent;

END

BEGIN(Client)

class CPlayerUI final 
	: public CActor
{
private :
	CPlayerUI(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CPlayerUI(const CPlayerUI& _Prototype);
	virtual ~CPlayerUI() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public :
	void Set_CrossHair(ECrossHair _eCrossHair);

	void Set_HP(const _float* _pHP, const _float* _pBossHP);
	void Set_BossShow(_bool _bShow);

private :
	CActor* m_UIs[static_cast<_uint>(EPlayerUI::PLAYERUI_END)]{};

private :
	virtual HRESULT Ready_Component() override { return S_OK; }
	virtual HRESULT Ready_Resource() override { return S_OK; }
	virtual HRESULT Ready_SubActor() override;

public :
	static CPlayerUI* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END