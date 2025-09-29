#include "CWorld.h"

#include "CLevel.h"

CWorld::CWorld(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CWorld::Init(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	return S_OK;
}

void CWorld::Pre_Update(_float _fTimeDelta)
{
	if (m_pMainLevel != nullptr)
		m_pMainLevel->Pre_Update(_fTimeDelta);

	for (auto& SubLevel : m_mapSubLevel)
		SubLevel.second->Pre_Update(_fTimeDelta);
}

void CWorld::Update(_float _fTimeDelta)
{
	if (m_pMainLevel != nullptr)
		m_pMainLevel->Update(_fTimeDelta);

	for (auto& SubLevel : m_mapSubLevel)
		SubLevel.second->Update(_fTimeDelta);
}

void CWorld::Post_Update(_float _fTimeDelta)
{
	if (m_pMainLevel != nullptr)
		m_pMainLevel->Post_Update(_fTimeDelta);

	for (auto& SubLevel : m_mapSubLevel)
		SubLevel.second->Post_Update(_fTimeDelta);
}

HRESULT CWorld::Render()
{
	if (m_pMainLevel != nullptr)
		m_pMainLevel->Render();

	for (auto& SubLevel : m_mapSubLevel)
		SubLevel.second->Render();

	return S_OK;
}

HRESULT CWorld::Load_MainLevel(CLevel* _pMainLevel)
{
	UnLoad_MainLevel();

	m_pMainLevel = _pMainLevel;

	return S_OK;
}

HRESULT CWorld::Load_SubLevel(const _wstring& _strSubLevelTag, CLevel* _pLevel)
{
	if (Find_SubLevel(_strSubLevelTag) != nullptr)
		return E_FAIL;

	m_mapSubLevel.emplace(_strSubLevelTag, _pLevel);

	return S_OK;
}

HRESULT CWorld::UnLoad_MainLevel()
{
	if (m_pMainLevel != nullptr)
	{
		m_pMainLevel->Clear();
		Safe_Release(m_pMainLevel);

		for (auto& SubLevel : m_mapSubLevel)
		{
			SubLevel.second->Clear();
			Safe_Release(SubLevel.second);
		}
	}

	return S_OK;
}

HRESULT CWorld::UnLoad_SubLevel(const _wstring& _strSubLevelTag)
{
	CLevel* pSubLevel = Find_SubLevel(_strSubLevelTag);

	if (pSubLevel == nullptr)
		return E_FAIL;

	pSubLevel->Clear();
	Safe_Release(pSubLevel);

	return S_OK;
}

CLevel* CWorld::Find_SubLevel(const _wstring& _strSubLevelTag)
{
	auto iter = m_mapSubLevel.find(_strSubLevelTag);
	if (iter == m_mapSubLevel.end())
		return nullptr;

	return iter->second;
}

CWorld* CWorld::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CWorld* pInstance = new CWorld(_pDevice, _pContext);

	if (FAILED(pInstance->Init(_pDevice, _pContext)))
	{
		MSG_BOX("Failed To Create : CWorld");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWorld::Free()
{
	__super::Free();

	m_pMainLevel->Clear();
	Safe_Release(m_pMainLevel);

	for (auto& iter : m_mapSubLevel)
	{
		iter.second->Clear();
		Safe_Release(iter.second);
	}

	m_mapSubLevel.clear();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}