#pragma once

#include "Client_Define.h"

#include "ILogger.h"

#include "CObject.h"

BEGIN(Client)

class CLogger final
	: public ILogger
	, public CObject
{
private :
	CLogger(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CLogger() = default;

public :
	virtual HRESULT Init() override;
	virtual void Pre_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Post_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public :
	virtual void Register_Player(CPlayer* _pPlayer) override;

private :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

private :
	_tchar m_szFPS[MAX_PATH]{};

	_uint m_iDrawCount{};
	_uint m_iFPS{};
	_float m_fTimerAcc{};

private :
	CPlayer* m_pPlayer{};

private :
	HRESULT Log_Player();

public :
	static CLogger* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
};

END