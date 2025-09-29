#pragma once

#include "Client_Define.h"

#include "CTerrain.h"

#include "ETerrainTexture.h"

BEGIN(Engine)

class CNavigationComponent;

END

BEGIN(Client)

class CTerrain_Test final
	: public CTerrain
{
private :
	CTerrain_Test(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CTerrain_Test(const CTerrain_Test& _Prototype);
	virtual ~CTerrain_Test() = default;

public :
	virtual HRESULT Init() override;
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private :
	CNavigationComponent* m_pNavigationComponent{};

private :
	virtual HRESULT Ready_Component() override;
	virtual HRESULT Ready_Resource() override;

public :
	static CTerrain_Test* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CClonable* Clone(void* _pDesc) override;
	virtual void Free() override;
};

END