#pragma once

#include "Client_Define.h"

#include "CLevel.h"

BEGIN(Client)

class CLevel_Stage1 final
	: public CLevel
{
private:
	CLevel_Stage1(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CLevel_Stage1() = default;

public :
	virtual HRESULT Init() override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private :
	HRESULT Ready_Layer_Light(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_Static(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_Picking(const _tchar* _pLayerTag);

private :
	_bool m_bLoadFile{};

public :
	static CLevel_Stage1* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
};

END