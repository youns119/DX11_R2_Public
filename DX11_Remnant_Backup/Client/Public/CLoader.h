#pragma once

#include "Client_Define.h"

#include "CObject.h"

#include "ELevel.h"

BEGIN(Client)

enum class ELevel;

class CLoader final 
	: public CObject
{
private :
	CLoader(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CLoader() = default;

public :
	HRESULT Init(ELevel _eNextLevel);

public :
	HRESULT Start_Loading();
	_bool IsFinished() const { return m_bFinished; }

#ifdef _DEBUG

public :
	void SetUp_WindowText() const;

#endif

private :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

private :
	HANDLE m_hThread{};
	CRITICAL_SECTION m_tCriticalSection{};

private :
	ELevel	m_eNextLevel{ ELevel::LEVEL_END };

	_bool m_bFinished{ false };
	static _bool m_bLoaded[static_cast<_uint>(ELevel::LEVEL_END)];
	_tchar m_szLoadingText[MAX_PATH]{};

private :
	HRESULT Loading_For_Level_Logo();
	HRESULT Loading_For_Level_Stage1();
	HRESULT Loading_For_Level_Terrain();

public :
	static CLoader* Create
	(
		ID3D11Device* _pDevice, 
		ID3D11DeviceContext* _pContext, 
		ELevel _eNextLevel
	);
	virtual void Free() override;
};

END