#pragma once

#include "Client_Define.h"

#include "ILogger.h"
#include "IImGuiManager.h"

BEGIN(Client)

class CTool final
{
private :
	CTool();
	virtual ~CTool() = default;

public :
	static HRESULT Init
	(
		ILogger* _pLogger,
		IImGuiManager* _pImGuiManager
	);
	static void Pre_Update(_float _fTimeDelta);
	static void Update(_float _fTimeDelta);
	static void Post_Update(_float _fTimeDelta);
	static HRESULT Render();

public :
	static ILogger* Get_Logger() { return m_pLogger; }
	static IImGuiManager* Get_ImGuiManager() { return m_pImGuiManager; }

private :
	static ILogger* m_pLogger;
	static IImGuiManager* m_pImGuiManager;
};

#define LOGGER CTool::Get_Logger()
#define IMGUI CTool::Get_ImGuiManager()

END