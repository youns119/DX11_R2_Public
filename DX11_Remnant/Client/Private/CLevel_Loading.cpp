#include "pch.h"
#include "CLevel_Loading.h"

#include "CGameInstance.h"

#include "CLoader.h"
#include "CLevel_Logo.h"
#include "CLevel_Stage1.h"
#include "CLevel_Terrain.h"

#include "ELevel.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CLevel{ _pDevice, _pContext }
{
}

HRESULT CLevel_Loading::Init(ELevel _eNextLevel)
{
	m_iLevelIndex = static_cast<_uint>(ELevel::LEVEL_LOADING);

	m_eNextLevel = _eNextLevel;

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, _eNextLevel);
	if (m_pLoader == nullptr)
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Pre_Update(_float _fTimeDelta)
{
	if (m_pLoader->IsFinished())
	{
		HRESULT hr{};

		switch (m_eNextLevel)
		{
		case ELevel::LEVEL_LOGO:
			hr = WORLD->Load_MainLevel(CLevel_Logo::Create(m_pDevice, m_pContext));
			break;
		case ELevel::LEVEL_STAGE1:
			hr = WORLD->Load_MainLevel(CLevel_Stage1::Create(m_pDevice, m_pContext));
			break;
		case ELevel::LEVEL_TERRAIN:
			hr = WORLD->Load_MainLevel(CLevel_Terrain::Create(m_pDevice, m_pContext));
			break;
		default:
			break;
		}

		if (FAILED(hr)) return;
		else WORLD->Pre_Update(_fTimeDelta);
	}
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->SetUp_WindowText();

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create
(
	ID3D11Device* _pDevice, 
	ID3D11DeviceContext* _pContext, 
	ELevel _eNextLevel
)
{
	CLevel_Loading* pInstance = new CLevel_Loading(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_eNextLevel)))
	{
		MSG_BOX("Failed To Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}