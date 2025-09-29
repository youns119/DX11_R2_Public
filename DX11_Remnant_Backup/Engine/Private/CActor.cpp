#include "CActor.h"

#include "CGameInstance.h"
#include "CLevel.h"
#include "CSceneComponent.h"

CActor::CActor(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CActor::CActor(const CActor& _Prototype)
	: m_pDevice{ _Prototype.m_pDevice }
	, m_pContext{ _Prototype.m_pContext }
	, m_strName{ _Prototype.m_strName }
	, m_pUpdateGraph{ _Prototype.m_pUpdateGraph->Clone() }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CActor::Init()
{
	m_pUpdateGraph = CGraph_Component::Create();

	return S_OK;
}

HRESULT CActor::Init(void* _pDesc)
{
	if (_pDesc == nullptr)
		return S_OK;

	FActor* pDesc = static_cast<FActor*>(_pDesc);
	m_pOuter = pDesc->pOuter;

	Safe_AddRef(m_pOuter);

	if (FAILED(Ready_Component())	||
		FAILED(Ready_SubActor())	||
		FAILED(Ready_Resource()))
		return E_FAIL;

	return S_OK;
}

void CActor::Pre_Update(_float _fTimeDelta)
{
	for (auto& ActorComponent : *m_pUpdateGraph->Get_ComponentList())
		ActorComponent->Pre_Update(_fTimeDelta);

	for (auto& Actor : m_mapSubActor)
		Actor.second->Pre_Update(_fTimeDelta);
}

void CActor::Update(_float _fTimeDelta)
{
	for (auto& ActorComponent : *m_pUpdateGraph->Get_ComponentList())
		ActorComponent->Update(_fTimeDelta);

	for (auto& Actor : m_mapSubActor)
		Actor.second->Update(_fTimeDelta);
}

void CActor::Post_Update(_float _fTimeDelta)
{
	for (auto& ActorComponent : *m_pUpdateGraph->Get_ComponentList())
		ActorComponent->Post_Update(_fTimeDelta);

	for (auto& Actor : m_mapSubActor)
		Actor.second->Post_Update(_fTimeDelta);
}

HRESULT CActor::Render()
{
	for (auto& Actor : m_mapSubActor)
		if (FAILED(Actor.second->Render()))
			return E_FAIL;

	return S_OK;
}

void CActor::Set_Show(_bool _bShow)
{
	m_bShow = _bShow;

	for (auto& Actor : m_mapSubActor)
		Actor.second->Set_Show(m_bShow);
}

void CActor::AttachToComponent(CSceneComponent* _pSceneComponent, const _float4x4* _pSocketMatrix)
{
	m_pRootComponent->AttachToComponent(_pSceneComponent, _pSocketMatrix);
}

void CActor::Update_Graph()
{
	m_pUpdateGraph->DFS(m_mapActorComponent);
}

void CActor::Clear()
{
	Safe_Release(m_pOuter);

	for (auto& ActorComponent : m_mapActorComponent)
		ActorComponent.second->Clear();

	for (auto& Actor : m_mapSubActor)
		Actor.second->Clear();
}

void CActor::Set_RootComponent(CSceneComponent* _pSceneComponent)
{
	m_pRootComponent = _pSceneComponent;
	Safe_AddRef(_pSceneComponent);
}

CActorComponent* CActor::Add_ActorComponent
(
	_uint _iPrototypeLevelIndex,
	const _wstring& _strPrototypeTag,
	const _wstring& _strComponentTag,
	void* _pDesc
)
{
	CActorComponent* pActorComponent = static_cast<CActorComponent*>(CGameInstance::Clone_Prototype
	(
		_iPrototypeLevelIndex,
		_strPrototypeTag,
		_pDesc
	));
	if (pActorComponent == nullptr)
		return nullptr;

	if (Find_ActorComponent(_strComponentTag) != nullptr)
		return nullptr;

	m_mapActorComponent.emplace(_strComponentTag, pActorComponent);
	Safe_AddRef(pActorComponent);

	Update_Graph();

	return pActorComponent;
}

void CActor::Add_ActorComponent
(
	const _wstring& _strComponentTag, 
	CActorComponent* _pActorComponent
)
{
	m_mapActorComponent.emplace(_strComponentTag, _pActorComponent);
	Safe_AddRef(_pActorComponent);

	Update_Graph();
}

CActor* CActor::Add_SubActor
(
	_uint _iPrototypeLevelIndex, 
	const _wstring& _strPrototypeTag, 
	const _wstring& _strActorTag, 
	void* _pDesc
)
{
	CActor* pActor = static_cast<CActor*>(CGameInstance::Clone_Prototype
	(
		_iPrototypeLevelIndex,
		_strPrototypeTag,
		_pDesc
	));
	if (pActor == nullptr)
		return nullptr;

	if (Find_ActorComponent(_strActorTag) != nullptr)
		return nullptr;

	m_mapSubActor.emplace(_strActorTag, pActor);
	Safe_AddRef(pActor);

	return pActor;
}

void CActor::Delete_ActorComponent(const _wstring& _strComponentTag)
{
	auto iter = m_mapActorComponent.find(_strComponentTag);
	if (iter == m_mapActorComponent.end())
		return;

	CActorComponent* pActorComponent = iter->second;

	while (pActorComponent)
		Safe_Release(pActorComponent);

	m_mapActorComponent.erase(_strComponentTag);
}

CActorComponent* CActor::Find_ActorComponent(const _wstring& _strComponentTag)
{
	auto iter = m_mapActorComponent.find(_strComponentTag);

	if (iter == m_mapActorComponent.end())
		return nullptr;

	return iter->second;
}

CActor* CActor::Find_SubActor(const _wstring& _strSubActorTag)
{
	auto iter = m_mapSubActor.find(_strSubActorTag);

	if (iter == m_mapSubActor.end())
		return nullptr;

	return iter->second;
}

void CActor::Free()
{
	__super::Free();

	Safe_Release(m_pUpdateGraph);

	for (auto& SubActor : m_mapSubActor)
		Safe_Release(SubActor.second);

	for (auto& Pair : m_mapActorComponent)
		Safe_Release(Pair.second);

	m_mapSubActor.clear();
	m_mapActorComponent.clear();

	Safe_Release(m_pRootComponent);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}

// CGraph_Component
void CGraph_Component::DFS(const map<const _wstring, CActorComponent*>& _mapActorComponent)
{
	queue<_uint> queZero;
	map<_uint, CActorComponent*> mapActorComponent;

	m_listActorComponent.clear();

	for (auto& Component : _mapActorComponent)
	{
		_uint iCurrID = Component.second->Get_ComponentID();
		mapActorComponent.emplace(iCurrID, Component.second);

		if (m_mapDegree.find(iCurrID) == m_mapDegree.end() ||
			m_mapDegree[iCurrID] == 0)
			queZero.push(iCurrID);
	}

	SubDFS(queZero, mapActorComponent);
}

void CGraph_Component::SubDFS(queue<_uint>& _queZero, map<_uint, CActorComponent*>& _mapActorComponent)
{
	unordered_map<_uint, _uint> mapDegree;

	for (auto& Degree : m_mapDegree)
		mapDegree[Degree.first] = Degree.second;

	while (!_queZero.empty())
	{
		_uint iNode = _queZero.front();
		m_listActorComponent.push_back(_mapActorComponent[iNode]);
		_queZero.pop();

		for (auto Adjacent : m_mapAdjacent[iNode])
		{
			mapDegree[Adjacent]--;

			if (mapDegree[Adjacent] == 0)
				_queZero.push(Adjacent);
		}
	}
}

void CGraph_Component::Free()
{
	__super::Free();

	m_listActorComponent.clear();

	m_mapAdjacent.clear();
	m_mapDegree.clear();
}