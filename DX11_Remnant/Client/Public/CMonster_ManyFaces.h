#pragma once

#include "Client_Define.h"

#include "CCharacter.h"

BEGIN(Engine)

class CSkeletalMeshComponent;
class CColliderComponent;

END

BEGIN(Client)

class CPlayer;

class CMonsterUI;

class CMonster_ManyFaces final
	: public CCharacter
{
private:
	CMonster_ManyFaces(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CMonster_ManyFaces(const CMonster_ManyFaces& _Prototype);
	virtual ~CMonster_ManyFaces() = default;

public:
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CSkeletalMeshComponent* m_pMeshComponent{};
	CColliderComponent* m_pColliderComponent{};

private:
	CMonsterUI* m_pMonsterUI{};

private:
	_bool m_bDeath{};
	_bool m_bAttack{};
	_bool m_bAttackSound{};
	_uint m_iAttackIndex{};
	_float m_fAttack{};
	_float m_fAttackAcc{};
	_float m_fAttackTime{};
	_float m_fDeath{};
	_uint m_iHit{};

private :
	_float m_fSoundTimeAcc{};
	_float m_fWalkSoundDist{ 0.5f };

private :
	CPlayer* m_pTarget{};

private :
	virtual HRESULT Ready_Component() override;
	virtual HRESULT Ready_Resource() override;
	virtual HRESULT Ready_SubActor() override;
	virtual HRESULT Ready_AnimStateMachine() override;

public :
	static CMonster_ManyFaces* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END