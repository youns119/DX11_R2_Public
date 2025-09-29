#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class CClonable;

class ENGINE_DLL IPrototypeManager abstract
{
protected :
	virtual ~IPrototypeManager() = default;

public :
	virtual HRESULT Add_Prototype
	(
		_uint _iPrototypeLevelIndex,
		const _wstring& _strPrototypeTag,
		CClonable* _pPrototype
	) PURE;
	virtual CClonable* Clone_Prototype
	(
		_uint _iPrototypeLevelIndex,
		const _wstring& _strPrototypeTag,
		void* _pDesc
	) PURE;
	virtual void Clear(_uint _iLevelIndex) PURE;
};

END