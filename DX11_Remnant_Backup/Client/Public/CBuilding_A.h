#pragma once

#include "Client_Define.h"

#include "CActor.h"

BEGIN(Engine)

class CStaticMeshComponent;

END

BEGIN(Client)

class CBuilding_A final
	: public CActor
{
private :
	CBuilding_A(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CBuilding_A(const CBuilding_A& _Prototype);
	virtual ~CBuilding_A() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private :
	CStaticMeshComponent* m_pMeshComponent{};

private :
	virtual HRESULT Ready_Component() override;
	virtual HRESULT Ready_Resource() override;

public :
	static CBuilding_A* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END