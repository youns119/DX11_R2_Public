#pragma once

#include "Client_Define.h"

#include "CActor.h"

BEGIN(Engine)

class CSkeletalMeshComponent;

END

BEGIN(Client)

class CAssaultRifle final
	: public CActor
{
private :
	CAssaultRifle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CAssaultRifle(const CAssaultRifle& _Prototype);
	virtual ~CAssaultRifle() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private :
	CSkeletalMeshComponent* m_pMeshComponent{};

private :
	virtual HRESULT Ready_Component() override;
	virtual HRESULT Ready_Resource() override;

public :
	static CAssaultRifle* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END