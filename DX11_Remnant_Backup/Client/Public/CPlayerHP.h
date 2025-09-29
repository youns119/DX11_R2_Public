#pragma once

#include "Client_Define.h"

#include "CActor.h"

#include "EPlayerHP.h"

BEGIN(Engine)

class CUIComponent;

END

BEGIN(Client)

class CPlayerHP final
	: public CActor
{
private :
	CPlayerHP(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CPlayerHP(const CPlayerHP& _Prototype);
	virtual ~CPlayerHP() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public :
	void Set_HP(const _float* _pHP, const _float* _pBossHP)
	{ 
		m_fMaxHP = *_pHP;
		m_pCurrHP = _pHP;

		m_fBossMaxHP = *_pBossHP;
		m_pBossCurrHP = _pBossHP;
	}

	void Set_BossShow(_bool _bShow);

private :
	CUIComponent* m_UIs[static_cast<_uint>(EPlayerHP::PLAYERHP_END)]{};

private :
	_float m_fMaxHP{};
	const _float* m_pCurrHP{};

	_float m_fBossMaxHP{};
	const _float* m_pBossCurrHP{};

private :
	virtual HRESULT Ready_Component() override;
	virtual HRESULT Ready_Resource() override;

public :
	static CPlayerHP* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END