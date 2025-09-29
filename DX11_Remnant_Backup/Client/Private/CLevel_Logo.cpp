#include "pch.h"
#include "CLevel_Logo.h"

#include "CGameInstance.h"
#include "FActor.h"

#include "CLevel_Loading.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CLevel{ _pDevice, _pContext }
{
}

HRESULT CLevel_Logo::Init()
{
	m_iLevelIndex = static_cast<_uint>(ELevel::LEVEL_LOGO);

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	SOUND->Play_BGM(L"Main_Menu.wav", 0.8f);

	return S_OK;
}

void CLevel_Logo::Pre_Update(_float _fTimeDelta)
{
	__super::Pre_Update(_fTimeDelta);
}

void CLevel_Logo::Update(_float _fTimeDelta)
{
	if (CGameInstance::Key_Press(DIK_RETURN))
	{
		WORLD->Load_MainLevel(CLevel_Loading::Create(m_pDevice, m_pContext, ELevel::LEVEL_STAGE1));
		return;
	}

	__super::Pre_Update(_fTimeDelta);
}

void CLevel_Logo::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CLevel_Logo::Render()
{

#ifdef _DEBUG

	SetWindowText(g_hWnd, TEXT("Logo Level Loaded"));

#endif

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar* _pLayerTag)
{
	FActor tActor{};
	tActor.pOuter = this;

	if (__super::Spawn_Actor
	(
		static_cast<_uint>(ELevel::LEVEL_LOGO),
		TEXT("Actor_LoadingUI"),
		_pLayerTag,
		&tActor
	) == nullptr)
		return E_FAIL;

	return S_OK;
}

CLevel_Logo* CLevel_Logo::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLevel_Logo* pInstance = new CLevel_Logo(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}