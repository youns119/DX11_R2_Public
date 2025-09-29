#include "CPrototypeManager.h"

#include "CClonable.h"

CPrototypeManager::CPrototypeManager()
{
}

HRESULT CPrototypeManager::Init(_uint _iNumLevel)
{
	m_iNumLevel = _iNumLevel;

	m_mapPrototype = new map<const _wstring, CClonable*>[m_iNumLevel];

	return S_OK;
}

HRESULT CPrototypeManager::Add_Prototype
(
	_uint _iPrototypeLevelIndex, 
	const _wstring& _strPrototypeTag, 
	CClonable* _pPrototype
)
{
	if (Find_Prototype(_iPrototypeLevelIndex, _strPrototypeTag) != nullptr)
		return E_FAIL;

	m_mapPrototype[_iPrototypeLevelIndex].emplace(_strPrototypeTag, _pPrototype);

	return S_OK;
}

CClonable* CPrototypeManager::Clone_Prototype
(
	_uint _iPrototypeLevelIndex, 
	const _wstring& _strPrototypeTag, 
	void* _pDesc
)
{
	CClonable* pPrototype = Find_Prototype(_iPrototypeLevelIndex, _strPrototypeTag);
	if (pPrototype == nullptr)
		return nullptr;

	pPrototype = pPrototype->Clone(_pDesc);

	return pPrototype;
}

void CPrototypeManager::Clear(_uint _iLevelIndex)
{
	for (auto& Pair : m_mapPrototype[_iLevelIndex])
		Safe_Release(Pair.second);

	m_mapPrototype[_iLevelIndex].clear();
}

CClonable* CPrototypeManager::Find_Prototype(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag)
{
	auto iter = m_mapPrototype[_iPrototypeLevelIndex].find(_strPrototypeTag);
	if (iter == m_mapPrototype[_iPrototypeLevelIndex].end())
		return nullptr;

	return iter->second;
}

CPrototypeManager* CPrototypeManager::Create(_uint _iNumLevel)
{
	CPrototypeManager* pInstance = new CPrototypeManager();

	if (FAILED(pInstance->Init(_iNumLevel)))
	{
		MSG_BOX("Failed To Create : CPrototypeManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPrototypeManager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumLevel; i++)
	{
		for (auto& Prototype : m_mapPrototype[i])
			Safe_Release(Prototype.second);

		m_mapPrototype[i].clear();
	}

	Safe_Delete_Array(m_mapPrototype);
}