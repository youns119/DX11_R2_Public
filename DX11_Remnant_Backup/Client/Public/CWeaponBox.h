#pragma once

#include "Client_Define.h"

#include "CActor.h"

#include "EWeaponBox.h"

BEGIN(Engine)

class CUIComponent;

END

BEGIN(Client)

class CWeaponBox final
	: public CActor
{
private :
	CWeaponBox(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CWeaponBox(const CWeaponBox& _Prototype);
	virtual ~CWeaponBox() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private :
	CUIComponent* m_UIs[static_cast<_uint>(EWeaponBox::WEAPONBOX_END)]{};

private :
	virtual HRESULT Ready_Component() override;
	virtual HRESULT Ready_Resource() override;

public :
	static CWeaponBox* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END