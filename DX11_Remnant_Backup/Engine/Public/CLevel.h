#pragma once

#include "CObject.h"

BEGIN(Engine)

class CLayer;
class CActor;

class ENGINE_DLL CLevel abstract
	: public CObject
{
protected :
	CLevel(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CLevel() = default;

public :
	virtual HRESULT Init();
	virtual void Pre_Update(_float _fTimeDelta);
	virtual void Update(_float _fTimeDelta);
	virtual void Post_Update(_float _fTimeDelta);
	virtual HRESULT Render();

public :
	ID3D11Device* Get_Device() { return m_pDevice; }
	ID3D11DeviceContext* Get_Context() { return m_pContext; }

	_uint Get_LevelIndex() const { return m_iLevelIndex; }

	CActor* Get_Actor(const _wstring& _strLayerTag, const _wstring& _strActorTag);
	const list<CActor*>* Get_ActorList(const _wstring& _strLayerTag) const;

	vector<_wstring>* Get_LayerTags() { return &m_vecLayerTag; }

public:
	void Clear();

public :
	CActor* Spawn_Actor
	(
		_uint _iPrototypeLevelIndex,
		const _wstring& _strPrototypeTag, 
		const _wstring& _strLayerTag,
		void* _pDesc = nullptr
	);

	HRESULT DeSpawn_Actor(const _wstring& _strLayerTag, CActor* _pActor);

public :
	CLayer* Find_Layer(const _wstring& _strLayerTag);

protected :
	ID3D11Device* m_pDevice{};
	ID3D11DeviceContext* m_pContext{};

protected :
	_bool m_bLoadFile{};
	_uint m_iLevelIndex{};

protected :
	map<const _wstring, CLayer*> m_mapLayer;

	vector<_wstring> m_vecLayerTag;

protected :
	HRESULT Load_Level_FromFile(string _strFile) { return S_OK; }

public :
	virtual void Free() override;
};

END