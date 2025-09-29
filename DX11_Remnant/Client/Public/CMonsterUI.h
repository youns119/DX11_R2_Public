#pragma once

#include "Client_Define.h"

#include "CActor.h"

#include "EMonsterUI.h"

BEGIN(Engine)

class CUIComponent;

END

BEGIN(Client)

class CMonsterUI final
	: public CActor
{
private :
	CMonsterUI(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CMonsterUI(const CMonsterUI& _Prototype);
	virtual ~CMonsterUI() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public :
	void Set_Pos(_float3 _vPos);

public :
	void Ready_HPUI(const _float* _pHP);

private :
	CActor* m_UIs[static_cast<_uint>(EMonsterUI::MONSTERUI_END)]{};

private :
	virtual HRESULT Ready_Component() override { return S_OK; }
	virtual HRESULT Ready_Resource() override { return S_OK; }
	virtual HRESULT Ready_SubActor() override;

public :
	static CMonsterUI* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END