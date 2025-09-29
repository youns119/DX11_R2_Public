#pragma once

#include "CClonable.h"

#include "FActor.h"

BEGIN(Engine)

class CLevel;
class CActorComponent;
class CSceneComponent;
class CGraph_Component;

class ENGINE_DLL CActor abstract
	: public CClonable
{
protected :
	CActor(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext);
	CActor(const CActor& _Prototype);
	virtual ~CActor() = default;

public :
	virtual HRESULT Init();
	virtual HRESULT Init(void* _pDesc) override;
	virtual void Pre_Update(_float _fTimeDelta);
	virtual void Update(_float _fTimeDelta);
	virtual void Post_Update(_float _fTimeDelta);
	virtual HRESULT Render();

public :
	CSceneComponent* Get_RootComponent() const { return m_pRootComponent; }

	_wstring Get_Name() const { return m_strName; }
	_bool Get_Show() const { return m_bShow; }
	
	CGraph_Component* Get_UpdateGraph() { return m_pUpdateGraph; }

	// ImGui Setter
	map<const _wstring, CActorComponent*>* Get_ActorComponents() { return &m_mapActorComponent; }

public :
	void Set_Name(_wstring _strName) { m_strName = _strName; }
	void Set_Show(_bool _bShow);

public :
	void AttachToComponent(CSceneComponent* _pSceneComponent, const _float4x4* _pSocketMatrix);

public :
	CActorComponent* Find_ActorComponent(const _wstring& _strComponentTag);
	CActor* Find_SubActor(const _wstring& _strSubActorTag);

public :
	void Update_Graph();
	virtual void Clear();

protected :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

protected :
	CLevel* m_pOuter{};

protected :
	CSceneComponent* m_pRootComponent{};

protected :
	map<const _wstring, CActorComponent*> m_mapActorComponent;
	map<const _wstring, CActor*> m_mapSubActor;

protected :
	CGraph_Component* m_pUpdateGraph{};

protected :
	_wstring m_strName{};

	_bool m_bShow{ true };

protected :
	void Set_RootComponent(CSceneComponent* _pSceneComponent);

protected :
	CActorComponent* Add_ActorComponent
	(
		_uint _iPrototypeLevelIndex,
		const _wstring& _strPrototypeTag,
		const _wstring& _strComponentTag,
		void* _pDesc = nullptr
	);
	void Add_ActorComponent
	(
		const _wstring& _strComponentTag,
		CActorComponent* _pActorComponent
	);

	CActor* Add_SubActor
	(
		_uint _iPrototypeLevelIndex,
		const _wstring& _strPrototypeTag,
		const _wstring& _strSubActorTag,
		void* _pDesc
	);


protected :
	void Delete_ActorComponent(const _wstring& _strComponentTag);

protected :
	virtual HRESULT Ready_Component() PURE;
	virtual HRESULT Ready_Resource() PURE;
	virtual HRESULT Ready_SubActor() { return S_OK; }

public :
	virtual CClonable* Clone(void* _pDesc) PURE;
	virtual void Free() override;
};

// CGraph_Component
class CGraph_Component final
	: public CObject
{
private :
	CGraph_Component() {};
	virtual ~CGraph_Component() = default;

public :
	virtual HRESULT Init()
	{ return S_OK; }

public :
	list<CActorComponent*>* Get_ComponentList()
	{ return &m_listActorComponent; }

public :
	void Add_Edge(_uint _iPrev, _uint _iNext)
	{
		m_mapAdjacent[_iPrev].push_back(_iNext);
		m_mapDegree[_iNext]++;
	}

public :
	void DFS(const map<const _wstring, CActorComponent*>& _mapActorComponent);
	void SubDFS(queue<_uint>& _queZero, map<_uint, CActorComponent*>& _mapActorComponent);

private :
	unordered_map<_uint, vector<_uint>> m_mapAdjacent;
	unordered_map<_uint, _uint> m_mapDegree;

private :
	list<CActorComponent*> m_listActorComponent;

public :
	static CGraph_Component* Create()
	{
		CGraph_Component* pInstance = new CGraph_Component;

		if (FAILED(pInstance->Init()))
		{
			MSG_BOX("Failed To Create : CGraph_Component");
			Safe_Release(pInstance);
		}

		return pInstance;
	}
	virtual void Free() override;
	CGraph_Component* Clone()
	{
		return new CGraph_Component;
	}
};

END