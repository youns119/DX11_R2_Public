#include "CTimerManager.h"

#include "CTimer.h"

CTimerManager::CTimerManager()
{
}

HRESULT CTimerManager::Add_Timer(const _wstring& _strTimerTag)
{
	CTimer* pTimer = Find_Timer(_strTimerTag);
	if (pTimer != nullptr)
		return E_FAIL;

	pTimer = CTimer::Create();
	if (pTimer == nullptr)
		return E_FAIL;

	m_mapTimer.insert({ _strTimerTag, pTimer });

	return S_OK;
}

void CTimerManager::Update_Timer(const _wstring& _strTimerTag)
{
	CTimer* pTimer = Find_Timer(_strTimerTag);
	if (pTimer == nullptr)
		return;

	pTimer->Update();
}

_float CTimerManager::Get_TimeDelta(const _wstring& _strTimerTag)
{
	CTimer* pTimer = Find_Timer(_strTimerTag);
	if (pTimer == nullptr)
		return 0.f;

	return pTimer->Get_TimeDelta();
}

CTimer* CTimerManager::Find_Timer(const _wstring& _strTimerTag)
{
	auto iter = m_mapTimer.find(_strTimerTag);
	if (iter == m_mapTimer.end())
		return nullptr;

	return iter->second;
}

CTimerManager* CTimerManager::Create()
{
	return new CTimerManager();
}

void CTimerManager::Free()
{
	__super::Free();

	for (auto& Pair : m_mapTimer)
		Safe_Release(Pair.second);

	m_mapTimer.clear();
}