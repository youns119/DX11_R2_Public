#include "pch.h"
#include "CLevel_Stage1.h"

#include "CGameInstance.h"
#include "CActor.h"
#include "CLight.h"
#include "CSceneComponent.h"

#include "FLight.h"

#include "CLevel_Loading.h"
#include "CTool.h"
#include "CPlayer.h"

#include "ELevel.h"
#include "FCharacter.h"
#include "FCamera.h"
#include "FTerrain.h"
#include "FSkyBox.h"

CLevel_Stage1::CLevel_Stage1(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CLevel{ _pDevice, _pContext }
{
}

HRESULT CLevel_Stage1::Init()
{
	m_iLevelIndex = static_cast<_uint>(ELevel::LEVEL_STAGE1);

	m_bLoadFile = true;

	if (m_bLoadFile)
	{
		IMGUI->Set_ImGuiMode(EImGuiMode::IMGUI_LEVELMODE, this);

		if (FAILED(IMGUI->ImGui_Load("../../../Resource/Binary/Level/Final.data")) ||
			FAILED(Ready_Layer_Picking(TEXT("Layer_Picking"))))
			return E_FAIL;

		IMGUI->ImGui_PlayerMode();
	}
	else
	{
		if (FAILED(Ready_Layer_Light(TEXT("Layer_Light")))				||
			FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround")))	||
			FAILED(Ready_Layer_Camera(TEXT("Layer_Camera")))			||
			FAILED(Ready_Layer_Player(TEXT("Layer_Player")))			||
			FAILED(Ready_Layer_Monster(TEXT("Layer_Monster")))			||
			FAILED(Ready_Layer_Static(TEXT("Layer_Static")))			||
			FAILED(Ready_Layer_Picking(TEXT("Layer_Picking"))))
			return E_FAIL;

		IMGUI->Set_ImGuiMode(EImGuiMode::IMGUI_LEVELMODE, this);
	}

	// Logger
	{
		CPlayer* pPlayer{};

		pPlayer = static_cast<CPlayer*>(__super::Get_Actor(L"Layer_Player", L"Actor_Player"));
		if (pPlayer == nullptr)
			return E_FAIL;

		LOGGER->Register_Player(pPlayer);
	}

	SOUND->Stop_Sound(EChannel::ECHANNEL_BGM);
	SOUND->Play_BGM(L"Stage_01.wav", 0.8f);
	SOUND->Play_BGM(L"Stage_01_BGM.wav", 0.5f);

	return S_OK;
}

void CLevel_Stage1::Pre_Update(_float _fTimeDelta)
{
	__super::Pre_Update(_fTimeDelta);
}

void CLevel_Stage1::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CLevel_Stage1::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CLevel_Stage1::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Light(const _tchar* _pLayerTag)
{
	m_vecLayerTag.push_back(_pLayerTag);

	FLight tLightDesc{};
	tLightDesc.pOuter = this;

	if (__super::Spawn_Actor
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Actor_Light_Directional"),
		_pLayerTag,
		&tLightDesc
	) == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_BackGround(const _tchar* _pLayerTag)
{
	m_vecLayerTag.push_back(_pLayerTag);

	FTerrain tTerrain{};
	tTerrain.pOuter = this;

	if(__super::Spawn_Actor
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1), 
		TEXT("Actor_Terrain_Test"), 
		_pLayerTag,
		&tTerrain
	) == nullptr)
		return E_FAIL;

	FSkyBox tSkyBox{};
	tSkyBox.pOuter = this;

	if (__super::Spawn_Actor
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Actor_Sky_Stage1"),
		_pLayerTag,
		&tSkyBox
	) == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Camera(const _tchar* _pLayerTag)
{
	m_vecLayerTag.push_back(_pLayerTag);

	FCamera tCamera{};
	tCamera.pOuter = this;

	CActor* pActor{};

	pActor = __super::Spawn_Actor
	(
		static_cast<_uint>(ELevel::LEVEL_STAGE1),
		TEXT("Actor_Camera_Free"),
		_pLayerTag,
		&tCamera
	);
	if(pActor == nullptr)
		return E_FAIL;

	CGameInstance::Set_Controller(pActor);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Player(const _tchar* _pLayerTag)
{
	m_vecLayerTag.push_back(_pLayerTag);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Monster(const _tchar* _pLayerTag)
{
	m_vecLayerTag.push_back(_pLayerTag);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Static(const _tchar* _pLayerTag)
{
	m_vecLayerTag.push_back(_pLayerTag);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Picking(const _tchar* _pLayerTag)
{
	m_vecLayerTag.push_back(_pLayerTag);

	CActor* pActor{};

	// Light
	{
		FLight tLightDesc{};
		tLightDesc.pOuter = this;

		CLight* pLight = static_cast<CLight*>(__super::Spawn_Actor
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_Light_Directional"),
			_pLayerTag,
			&tLightDesc
		));
		if(pLight == nullptr)
			return E_FAIL;

		pLight->Set_LightOnOff(false);
	}

	// Player
	{
		FCharacter tCharacter{};
		tCharacter.pOuter = this;

		pActor = __super::Spawn_Actor
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_Player"),
			_pLayerTag,
			&tCharacter
		);
		if (pActor == nullptr)
			return E_FAIL;

		pActor->Set_Show(false);
	}

	// Pino
	{
		FCharacter tCharacter{};
		tCharacter.pOuter = this;

		pActor = __super::Spawn_Actor
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_Pino"),
			_pLayerTag,
			&tCharacter
		);
		if (pActor == nullptr)
			return E_FAIL;

		pActor->Set_Show(false);
	}

	// Monster_Brute
	{
		FCharacter tCharacter{};
		tCharacter.pOuter = this;

		pActor = __super::Spawn_Actor
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_Monster_Brute"),
			_pLayerTag,
			&tCharacter
		);
		if(pActor == nullptr)
			return E_FAIL;

		pActor->Set_Show(false);
	}

	// Monster_ManyFaces
	{
		FCharacter tCharacter{};
		tCharacter.pOuter = this;

		pActor = __super::Spawn_Actor
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_Monster_ManyFaces"),
			_pLayerTag,
			&tCharacter
		);
		if (pActor == nullptr)
			return E_FAIL;

		pActor->Set_Show(false);
	}

	// DynamicActor_AssaultRifle
	{
		FActor tActor{};
		tActor.pOuter = this;

		pActor = __super::Spawn_Actor
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_DynamicActor_AssaultRifle"),
			_pLayerTag,
			&tActor
		);
		if (pActor == nullptr)
			return E_FAIL;

		pActor->Set_Show(false);
	}

	// StaticActor_Building_A
	{
		FActor tActor{};
		tActor.pOuter = this;

		pActor = __super::Spawn_Actor
		(
			static_cast<_uint>(ELevel::LEVEL_STAGE1),
			TEXT("Actor_StaticActor_Building_A"),
			_pLayerTag,
			&tActor
		);
		if (pActor == nullptr)
			return E_FAIL;

		pActor->Set_Show(false);
	}

	return S_OK;
}

CLevel_Stage1* CLevel_Stage1::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLevel_Stage1* pInstance = new CLevel_Stage1(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Created : CLevel_Stage1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage1::Free()
{
	__super::Free();
}