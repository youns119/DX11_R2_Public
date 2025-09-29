#include "pch.h"
#include "CMainApp.h"

#include "CGameInstance.h"
#include "CGraphicDevice.h"
#include "CInputDevice.h"
#include "CTimerManager.h"
#include "CSoundManager.h"
#include "CPrototypeManager.h"
#include "CTargetManager.h"
#include "CRenderer.h"
#include "CPipeLine.h"
#include "CFrustum.h"
#include "CWorld.h"
#include "CFontManager.h"

#include "CUIComponent.h"
#include "CVIBuffer_Rect.h"
#include "CShader.h"

#include "CPicking.h"

#ifdef _DEBUG

#include "CTool.h"
#include "CImGuiManager.h"
#include "CLogger.h"

#endif

#include "CLevel_Loading.h"

#include "ELevel.h"

CMainApp::CMainApp()
{
}

HRESULT CMainApp::Init()
{
	if (FAILED(Ready_GameInstance())	||
		FAILED(Ready_Font()))
		return E_FAIL;

#ifdef _DEBUG

	if (FAILED(Ready_Tool()))
		return E_FAIL;

#endif

	if (FAILED(Ready_Prototype_For_Static()))
		return E_FAIL;

	if (FAILED(Start_Level(ELevel::LEVEL_LOGO)))
		return E_FAIL;

	srand(static_cast<_uint>(time(NULL)));

	return S_OK;
}

void CMainApp::Pre_Update(_float _fTimeDelta)
{
	CGameInstance::Pre_Update(_fTimeDelta);
	CTool::Pre_Update(_fTimeDelta);
}

void CMainApp::Update(_float _fTimeDelta)
{
	CGameInstance::Update(_fTimeDelta);

#ifdef _DEBUG

	CTool::Update(_fTimeDelta);

#endif
}

void CMainApp::Post_Update(_float _fTimeDelta)
{
	CGameInstance::Post_Update(_fTimeDelta);
	CTool::Post_Update(_fTimeDelta);
}

HRESULT CMainApp::Render()
{
	if (FAILED(CGameInstance::Render_Begin(_float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	CGameInstance::Draw();

#ifdef _DEBUG

	CTool::Render();

#endif

	CGameInstance::Render_End();

	return S_OK;
}

HRESULT CMainApp::Ready_GameInstance()
{
	FEngine tEngineDesc{};

	tEngineDesc.iViewportWidth = g_iWinSizeX;
	tEngineDesc.iViewportHeight = g_iWinSizeY;

	CGraphicDevice* pGraphicDevice = CGraphicDevice::Create(g_hWnd, true, g_iWinSizeX, g_iWinSizeY, &m_pDevice, &m_pContext);
	CInputDevice* pInputDevice = CInputDevice::Create(g_hInst, g_hWnd);
	CTimerManager* pTimerManager = CTimerManager::Create();
	CSoundManager* pSoundManager = CSoundManager::Create();

	CPrototypeManager* pPrototypeManager = CPrototypeManager::Create(static_cast<_uint>(ELevel::LEVEL_END));
	CTargetManager* pTargetManager = CTargetManager::Create(m_pDevice, m_pContext);
	CRenderer* pRenderer = CRenderer::Create(m_pDevice, m_pContext, pTargetManager);
	CPipeLine* pPipeLine = CPipeLine::Create();
	CFrustum* pFrustum = CFrustum::Create();
	CWorld* pWorld = CWorld::Create(m_pDevice, m_pContext);
	CFontManager* pFontManager = CFontManager::Create(m_pDevice, m_pContext);

	CPicking* pPicking = CPicking::Create(m_pDevice, m_pContext, g_hWnd);

	if (pGraphicDevice == nullptr		||
		pInputDevice == nullptr			||
		pTimerManager == nullptr		||
		pSoundManager == nullptr		||
		pPrototypeManager == nullptr	||
		pTargetManager == nullptr		||
		pRenderer == nullptr			||
		pPipeLine == nullptr			||
		pFrustum == nullptr				||
		pWorld == nullptr				||
		pFontManager == nullptr			||
		pPicking == nullptr)
		return E_FAIL;

	CGameInstance::Init
	(
		pGraphicDevice, 
		pInputDevice, 
		pTimerManager, 
		pSoundManager,
		pPrototypeManager, 
		pTargetManager,
		pRenderer,
		pPipeLine,
		pFrustum,
		pWorld,
		pFontManager,
		pPicking,
		tEngineDesc
	);

	return S_OK;
}

HRESULT CMainApp::Ready_Tool()
{
	CLogger* pLogger = CLogger::Create(m_pDevice, m_pContext);
	CImGuiManager* pImGuiManager = CImGuiManager::Create(m_pDevice, m_pContext);

	if (pLogger == nullptr ||
		pImGuiManager == nullptr)
		return E_FAIL;

	CTool::Init
	(
		pLogger,
		pImGuiManager
	);

	return S_OK;
}

HRESULT CMainApp::Ready_Font()
{
	if (FAILED(CGameInstance::Add_Font
	(
		TEXT("Font_Gulim_Default"), 
		RESOURCE("Font/147ex.spritefont")
	)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_For_Static()
{
	return S_OK;
}

HRESULT CMainApp::Start_Level(ELevel _eStartLevel)
{
	if (_eStartLevel == ELevel::LEVEL_LOADING)
		return E_FAIL;

	if (FAILED(WORLD->Load_MainLevel(CLevel_Loading::Create
	(
		m_pDevice,
		m_pContext,
		_eStartLevel
	))))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Release_GameIntance()
{
	CGraphicDevice* pGraphicDevice = static_cast<CGraphicDevice*>(CGameInstance::Get_GraphicDevice());
	CInputDevice* pInputDevice = static_cast<CInputDevice*>(CGameInstance::Get_InputDevice());
	CTimerManager* pTimerManager = static_cast<CTimerManager*>(CGameInstance::Get_TimerManager());
	CSoundManager* pSoundManager = static_cast<CSoundManager*>(SOUND);

	CPrototypeManager* pPrototypeManager = static_cast<CPrototypeManager*>(CGameInstance::Get_PrototypeManager());
	CTargetManager* pTargetManager = static_cast<CTargetManager*>(CGameInstance::Get_TargetManager());
	CRenderer* pRenderer = static_cast<CRenderer*>(CGameInstance::Get_Renderer());
	CPipeLine* pPipeLine = static_cast<CPipeLine*>(CGameInstance::Get_PipeLine());
	CFrustum* pFrustum = static_cast<CFrustum*>(CGameInstance::Get_Frustum());
	CWorld* pWorld = static_cast<CWorld*>(WORLD);
	CFontManager* pFontManager = static_cast<CFontManager*>(CGameInstance::Get_FontManager());

	CPicking* pPicking = static_cast<CPicking*>(CGameInstance::Get_Picking());

	Safe_Release(pPicking);

	Safe_Release(pFontManager);
	Safe_Release(pWorld);
	Safe_Release(pFrustum);
	Safe_Release(pPipeLine);
	Safe_Release(pRenderer);
	Safe_Release(pTargetManager);
	Safe_Release(pPrototypeManager);

	Safe_Release(pSoundManager);
	Safe_Release(pTimerManager);
	Safe_Release(pInputDevice);
	Safe_Release(pGraphicDevice);
}

void CMainApp::Release_Tool()
{
	CLogger* pLogger = static_cast<CLogger*>(CTool::Get_Logger());
	CImGuiManager* pImGuiManager = static_cast<CImGuiManager*>(CTool::Get_ImGuiManager());

	Safe_Release(pLogger);
	Safe_Release(pImGuiManager);
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

#ifdef _DEBUG

	Release_Tool();

#endif

	Release_GameIntance();
}