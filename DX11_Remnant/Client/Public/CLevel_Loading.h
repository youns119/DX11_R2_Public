#pragma once

#include "Client_Define.h"

#include "CLevel.h"

#include "ELevel.h"

BEGIN(Client)

class CLoader;

class CLevel_Loading final 
	: public CLevel
{
private :
	CLevel_Loading(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CLevel_Loading() = default;

public :
	virtual HRESULT Init(ELevel _eNextLevel);
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private :
	CLoader* m_pLoader{};

	ELevel m_eNextLevel{ ELevel::LEVEL_END };

public :
	static CLevel_Loading* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext, 
		ELevel _eNextLevel
	);
	virtual void Free() override;
};

END