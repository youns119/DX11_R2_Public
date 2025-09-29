#pragma once

#include "Client_Define.h"

#include "CCharacter.h"

BEGIN(Engine)

class CSpringArmComponent;
class CCameraComponent;
class CColliderComponent;

END

BEGIN(Client)

class CPino final
	: public CCharacter
{
private:
	CPino(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CPino(const CPino& _Prototype);
	virtual ~CPino() = default;

public:
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CSpringArmComponent* m_pSpringArmComponent{};
	CCameraComponent* m_pCameraComponent{};
	CColliderComponent* m_pColliderComponent{};

private:
	virtual HRESULT Ready_Component() override;
	virtual HRESULT Ready_Resource() override;
	virtual HRESULT Ready_SubActor() override;
	virtual HRESULT Ready_AnimStateMachine() override;

public:
	static CPino* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END