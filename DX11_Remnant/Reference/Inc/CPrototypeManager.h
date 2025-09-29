#pragma once

#include "IPrototypeManager.h"

#include "CObject.h"

BEGIN(Engine)

class CClonable;

class ENGINE_DLL CPrototypeManager final
	: public IPrototypeManager
	, public CObject
{
private :
	CPrototypeManager();
	virtual ~CPrototypeManager() = default;

public :
	HRESULT Init(_uint _iNumLevel);

public :
	virtual HRESULT Add_Prototype
	(
		_uint _iPrototypeLevelIndex, 
		const _wstring& _strPrototypeTag, 
		CClonable* _pPrototype
	) override;
	virtual CClonable* Clone_Prototype
	(
		_uint _iPrototypeLevelIndex, 
		const _wstring& _strPrototypeTag, 
		void* _pDesc = nullptr
	) override;
	virtual void Clear(_uint _iLevelIndex) override;

private :
	map<const _wstring, CClonable*>* m_mapPrototype{};

private :
	_uint m_iNumLevel{};

private :
	CClonable* Find_Prototype(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag);

public :
	static CPrototypeManager* Create(_uint _iNumLevel);
	virtual void Free() override;
};

END