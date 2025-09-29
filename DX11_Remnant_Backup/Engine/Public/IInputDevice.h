#pragma once

#include "Engine_Define.h"

#include "EKeyState.h"
#include "EMouseDir.h"
#include "EMouseKey.h"

BEGIN(Engine)

class ENGINE_DLL IInputDevice abstract
{
protected :
	virtual ~IInputDevice() = default;

public :
	virtual void Update() PURE;

public :
	virtual _bool Key_Press(_ubyte _byKeyID) const PURE;
	virtual _bool Key_Hold(_ubyte _byKeyID) const PURE;
	virtual _bool Key_Release(_ubyte _byKeyID) const PURE;

	virtual _bool Mouse_Press(EMouseKey _eMouseKey) const PURE;
	virtual _bool Mouse_Hold(EMouseKey _eMouseKey) const PURE;
	virtual _bool Mouse_Release(EMouseKey _eMouseKey) const PURE;

	virtual _long Get_DIMouseMove(EMouseDir _eMouseDir) const PURE;
};

END