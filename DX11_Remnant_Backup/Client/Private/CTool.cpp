#include "pch.h"
#include "CTool.h"

ILogger* CTool::m_pLogger= nullptr;
IImGuiManager* CTool::m_pImGuiManager = nullptr;

CTool::CTool()
{
}

HRESULT CTool::Init
(
	ILogger* _pLogger,
	IImGuiManager* _pImGuiManager
)
{
	m_pLogger = _pLogger;
	m_pImGuiManager = _pImGuiManager;

	return S_OK;
}

void CTool::Pre_Update(_float _fTimeDelta)
{
	m_pImGuiManager->Pre_Update(_fTimeDelta);
	m_pLogger->Pre_Update(_fTimeDelta);
}

void CTool::Update(_float _fTimeDelta)
{
	m_pImGuiManager->Update(_fTimeDelta);
	m_pLogger->Update(_fTimeDelta);
}

void CTool::Post_Update(_float _fTimeDelta)
{
	m_pImGuiManager->Post_Update(_fTimeDelta);
	m_pLogger->Post_Update(_fTimeDelta);
}

HRESULT CTool::Render()
{
	if (FAILED(m_pLogger->Render()) ||
		FAILED(m_pImGuiManager->Render()))
		return E_FAIL;

	return S_OK;
}