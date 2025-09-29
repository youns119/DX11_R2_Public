#include "pch.h"
#include "CLevel_Terrain.h"

#include "CGameInstance.h"
#include "CActor.h"
#include "CSceneComponent.h"

#include "FLight.h"

#include "CTool.h"

#include "ELevel.h"
#include "FTerrain.h"
#include "FCamera.h"
#include "FCharacter.h"
#include "FSkyBox.h"

CLevel_Terrain::CLevel_Terrain(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CLevel{ _pDevice, _pContext }
{
}

HRESULT CLevel_Terrain::Init()
{
	m_iLevelIndex = static_cast<_uint>(ELevel::LEVEL_TERRAIN);

	if (FAILED(Ready_Light(TEXT("Layer_Light")))					||
		FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround")))	||
		FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	IMGUI->Set_ImGuiMode(EImGuiMode::IMGUI_TERRAIN, this);

	return S_OK;
}

void CLevel_Terrain::Pre_Update(_float _fTimeDelta)
{
	__super::Pre_Update(_fTimeDelta);
}

void CLevel_Terrain::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CLevel_Terrain::Post_Update(_float _fTimeDelta)
{
	__super::Post_Update(_fTimeDelta);
}

HRESULT CLevel_Terrain::Render()
{
	return S_OK;
}

HRESULT CLevel_Terrain::Ready_Light(const _tchar* _pLayerTag)
{
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

HRESULT CLevel_Terrain::Ready_Layer_BackGround(const _tchar* _pLayerTag)
{
	FSkyBox tSkyBox{};
	tSkyBox.pOuter = this;

	if (__super::Spawn_Actor
	(
		static_cast<_uint>(ELevel::LEVEL_TERRAIN),
		TEXT("Actor_Sky_Default"),
		_pLayerTag,
		&tSkyBox
	) == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Terrain::Ready_Layer_Camera(const _tchar* _pLayerTag)
{
	FCamera tCamera{};
	tCamera.pOuter = this;

	if (__super::Spawn_Actor
	(
		static_cast<_uint>(ELevel::LEVEL_STATIC),
		TEXT("Actor_Camera_Free"),
		_pLayerTag,
		&tCamera
	) == nullptr)
		return E_FAIL;

	return S_OK;
}

CLevel_Terrain* CLevel_Terrain::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLevel_Terrain* pInstance = new CLevel_Terrain(_pDevice, _pContext);

	if (FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed To Created : CLevel_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Terrain::Free()
{
	__super::Free();
}