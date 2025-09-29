#include "CLayer.h"

#include "CActor.h"

CLayer::CLayer()
{
}

void CLayer::Pre_Update(_float _fTimeDelta)
{
	for (auto& Actor : m_listActor)
		Actor->Pre_Update(_fTimeDelta);
}

void CLayer::Update(_float _fTimeDelta)
{
	for (auto& Actor : m_listActor)
		Actor->Update(_fTimeDelta);
}

void CLayer::Post_Update(_float _fTimeDelta)
{
	for (auto& Actor : m_listActor)
		Actor->Post_Update(_fTimeDelta);
}

HRESULT CLayer::Render()
{
	for (auto& Actor : m_listActor)
		if(FAILED(Actor->Render()))
			return E_FAIL;

	return S_OK;
}

CActor* CLayer::Get_Actor(const _wstring& _strActorTag) const
{
	for (auto& Actor : m_listActor)
		if (Actor->Get_Name() == _strActorTag)
			return Actor;

	return nullptr;
}

HRESULT CLayer::Delete_Actor(CActor* _pActor)
{
	auto iter = find_if(m_listActor.begin(), m_listActor.end(), [&](CActor* _pSrcActor)->bool
		{
			if (_pSrcActor == _pActor)
				return true;

			return false;
		});

	if (iter == m_listActor.end())
		return E_FAIL;

	_pActor->Clear();
	Safe_Release(_pActor);

	m_listActor.erase(iter);

	return S_OK;
}

void CLayer::Clear()
{
	for (auto& Actor : m_listActor)
		Actor->Clear();
}

void CLayer::Free()
{
	__super::Free();

	for (auto& Actor : m_listActor)
		Safe_Release(Actor);

	m_listActor.clear();
}