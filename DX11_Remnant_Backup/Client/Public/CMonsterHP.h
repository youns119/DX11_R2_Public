#pragma once

#include "Client_Define.h"

#include "CActor.h"

#include "EMonsterHP.h"

BEGIN(Engine)

class CUIComponent;

END

BEGIN(Client)

class CMonsterHP final
	: public CActor
{
private :
	CMonsterHP(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CMonsterHP(const CMonsterHP& _Prototype);
	virtual ~CMonsterHP() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public :
	void Set_Pos(_float3 _vPos) { m_vCurrPos = _vPos; }

public :
	void Ready_HPUI(const _float* _pHP)
	{
		m_fMaxHP = *_pHP;
		m_pCurrHP = _pHP;
	}

private :
	CUIComponent* m_UIs[static_cast<_uint>(EMonsterHP::MONSTER_END)]{};

private :
	_float m_fMaxHP{};
	const _float* m_pCurrHP{};

	_float3 m_vCurrPos{};

private :
	virtual HRESULT Ready_Component() override;
	virtual HRESULT Ready_Resource() override;

public :
	static CMonsterHP* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END