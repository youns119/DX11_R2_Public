#pragma once

#include "Client_Define.h"

#include "CCharacter.h"

#include "EPlayerState.h"

BEGIN(Engine)

class CSpringArmComponent;
class CCameraComponent;
class CColliderComponent;
class CNavigationComponent;

END

BEGIN(Client)

class CAssaultRifle;
class CPlayerUI;

class CPlayer final
	: public CCharacter
{
private :
	CPlayer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CPlayer(const CPlayer& _Prototype);
	virtual ~CPlayer() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public :
	_float Get_MaxHP() const { return m_fMaxHP; }
	_float Get_CurrHP() const { return m_fCurrHP; }

	_float Get_BossCurrHP() const { return m_fBossCurrHP; }

public :
	void Damaged(_float _fDamage) { m_fCurrHP -= _fDamage; }
	void Damage_Boss(_float _fDamage) 
	{ 
		m_fBossCurrHP -= _fDamage;
		if (m_fBossCurrHP <= 0)
			m_fBossCurrHP = 0;
	}

	_bool Get_Shoot() { return m_bShoot; }
	void Set_Shoot(_bool _bShoot) { m_bShoot = _bShoot; }

private :
	CSpringArmComponent* m_pSpringArmComponent{};
	CCameraComponent* m_pCameraComponent{};
	CColliderComponent* m_pColliderComponent{};
	CNavigationComponent* m_pNavigationComponent{};

private :
	CAssaultRifle* m_pWeapon_Rifle{};
	CPlayerUI* m_pPlayerUI{};

private :
	_uint m_iPlayerState{};

private :
	_float m_fMaxHP{};
	_float m_fCurrHP{};

	_float m_fBossMaxHP{};
	_float m_fBossCurrHP{};

private :
	_float m_fShootTime{};
	_bool m_bShoot{};

private :
	virtual HRESULT Ready_Component() override;
	virtual HRESULT Ready_Resource() override;
	virtual HRESULT Ready_SubActor() override;
	virtual HRESULT Ready_AnimStateMachine() override;

public :
	static CPlayer* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END