#pragma once

#include "Client_Define.h"

#include "CLevel.h"

BEGIN(Client)

class CLevel_Terrain final
	: public CLevel
{
private:
	CLevel_Terrain(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CLevel_Terrain() = default;

public:
	virtual HRESULT Init() override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Light(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* _pLayerTag);

public:
	static CLevel_Terrain* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
};

END