#pragma once

#include "CObject.h"

BEGIN(Engine)

class CActor;

class CLayer final 
	: public CObject
{
private :
	CLayer();
	virtual ~CLayer() = default;

public :
	void Pre_Update(_float _fTimeDelta);
	void Update(_float _fTimeDelta);
	void Post_Update(_float _fTimeDelta);
	HRESULT Render();

public :
	CActor* Get_Actor(const _wstring& _strActorTag) const;
	const list<CActor*>* Get_ActorList() const { return &m_listActor; }

public :
	HRESULT Add_Actor(CActor* _pActor)
	{
		m_listActor.push_back(_pActor);
		return S_OK;
	}

	HRESULT Delete_Actor(CActor* _pActor);

public :
	void Clear();

private :
	list<CActor*> m_listActor;

public :
	static CLayer* Create() { return new CLayer(); };
	virtual void Free() override;
};

END