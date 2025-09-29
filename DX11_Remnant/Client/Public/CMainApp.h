#pragma once

#include "Client_Define.h"

#include "CObject.h"

#ifdef _DEBUG

#include "CImGuiManager.h"

#endif

BEGIN(Client)

enum class ELevel;

class CMainApp final 
	: public CObject
{
private :
	CMainApp();
	virtual ~CMainApp() = default;

public :
	HRESULT Init();
	void Pre_Update(_float _fTimeDelta);
	void Update(_float _fTimeDelta);
	void Post_Update(_float _fTimeDelta);
	HRESULT Render();

private :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

private :
	HRESULT Ready_GameInstance();
	HRESULT Ready_Tool();
	HRESULT Ready_Font();

private :
	HRESULT Ready_Prototype_For_Static();

private :
	HRESULT Start_Level(ELevel _eStartLevel);

private :
	void Release_GameIntance();
	void Release_Tool();

public :
	static CMainApp* Create();
	virtual void Free() override;
};

END