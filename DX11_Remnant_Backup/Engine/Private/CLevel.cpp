#include "CLevel.h"

#include "CGameInstance.h"
#include "CLayer.h"
#include "CActor.h"

CLevel::CLevel(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{_pDevice}
	, m_pContext{_pContext}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CLevel::Init()
{
	return S_OK;
}

void CLevel::Pre_Update(_float _fTimeDelta)
{
	for (auto& Layer : m_mapLayer)
		Layer.second->Pre_Update(_fTimeDelta);
}

void CLevel::Update(_float _fTimeDelta)
{
	for (auto& Layer : m_mapLayer)
		Layer.second->Update(_fTimeDelta);
}

void CLevel::Post_Update(_float _fTimeDelta)
{
	for (auto& Layer : m_mapLayer)
		Layer.second->Post_Update(_fTimeDelta);
}

HRESULT CLevel::Render()
{
	for (auto& Layer : m_mapLayer)
		if (FAILED(Layer.second->Render()))
			return E_FAIL;

	return S_OK;
}

CActor* CLevel::Get_Actor(const _wstring& _strLayerTag, const _wstring& _strActorTag)
{
	auto iter = m_mapLayer.find(_strLayerTag);
	if (iter == m_mapLayer.end())
		return nullptr;

	CLayer* pLayer = iter->second;
	CActor* pActor = pLayer->Get_Actor(_strActorTag);

	return pActor;
}

const list<CActor*>* CLevel::Get_ActorList(const _wstring& _strLayerTag) const
{
	auto iter = m_mapLayer.find(_strLayerTag);
	if (iter == m_mapLayer.end())
		return nullptr;

	CLayer* pLayer = iter->second;

	const list<CActor*>* pListActor = pLayer->Get_ActorList();

	return pListActor;
}

void CLevel::Clear()
{
	for (auto& Layer : m_mapLayer)
	{
		Layer.second->Clear();
		Safe_Release(Layer.second);
	}

	m_mapLayer.clear();
	m_vecLayerTag.clear();
}

CActor* CLevel::Spawn_Actor
(
	_uint _iPrototypeLevelIndex,
	const _wstring& _strPrototypeTag, 
	const _wstring& _strLayerTag,
	void* _pDesc
)
{
	CActor* pActor = dynamic_cast<CActor*>(CGameInstance::Clone_Prototype
	(
		_iPrototypeLevelIndex, 
		_strPrototypeTag, 
		_pDesc
	));
	if (pActor == nullptr)
		return nullptr;

	pActor->Set_Name(_strPrototypeTag);

	CLayer* pLayer = Find_Layer(_strLayerTag);
	if (pLayer == nullptr)
	{
		pLayer = CLayer::Create();
		pLayer->Add_Actor(pActor);

		m_mapLayer.emplace(_strLayerTag, pLayer);
	}
	else
		pLayer->Add_Actor(pActor);

	return pActor;
}

HRESULT CLevel::DeSpawn_Actor(const _wstring& _strLayerTag, CActor* _pActor)
{
	CLayer* pLayer = Find_Layer(_strLayerTag);
	if (pLayer == nullptr)
		return E_FAIL;

	if (FAILED(pLayer->Delete_Actor(_pActor)))
		return E_FAIL;

	return S_OK;
}

CLayer* CLevel::Find_Layer(const _wstring& _strLayerTag)
{
	auto iter = m_mapLayer.find(_strLayerTag);
	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second;
}

void CLevel::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}