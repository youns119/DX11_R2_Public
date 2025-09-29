#ifndef InputDev_h__
#define InputDev_h__

#include "IInputDevice.h"

#include "CObject.h"

BEGIN(Engine)

class ENGINE_DLL CInputDevice final
	: public IInputDevice
	, public CObject
{
private :
	CInputDevice();
	virtual ~CInputDevice() = default;

public :
	HRESULT Init(HINSTANCE _hInst, HWND _hWnd);
	virtual void Update() override;

public :
	virtual _bool Key_Press(_ubyte _byKeyID) const override 
	{ return m_tKeyInfo[_byKeyID].eKeyState == EKeyState::KEYSTATE_PRESS; }
	virtual _bool Key_Hold(_ubyte _byKeyID) const override  
	{ return m_tKeyInfo[_byKeyID].eKeyState == EKeyState::KEYSTATE_HOLD; }
	virtual _bool Key_Release(_ubyte _byKeyID) const override  
	{ return m_tKeyInfo[_byKeyID].eKeyState == EKeyState::KEYSTATE_RELEASE; }

	virtual _bool Mouse_Press(EMouseKey _eMouseKey) const override  
	{ return m_tMouseInfo[(_uint)_eMouseKey].eKeyState == EKeyState::KEYSTATE_PRESS; }
	virtual _bool Mouse_Hold(EMouseKey _eMouseKey) const override  
	{ return m_tMouseInfo[(_uint)_eMouseKey].eKeyState == EKeyState::KEYSTATE_HOLD; }
	virtual _bool Mouse_Release(EMouseKey _eMouseKey) const override  
	{ return m_tMouseInfo[(_uint)_eMouseKey].eKeyState == EKeyState::KEYSTATE_RELEASE; }

	virtual _long Get_DIMouseMove(EMouseDir _eMouseDir) const override  
	{ 
		if (m_bFocus)
			return *(((_long*)&m_tMouseState) + (_uint)_eMouseDir);
		else return 0;
	}

private :
	LPDIRECTINPUT8 m_pInputSDK{};

private :
	LPDIRECTINPUTDEVICE8 m_pKeyBoard{};
	LPDIRECTINPUTDEVICE8 m_pMouse{};

private :
#define VK_MAX 256
#define MS_MAX 3

	_bool m_bFocus{};

	HWND m_hWnd{};

	struct KeyInfo
	{
		EKeyState eKeyState{ EKeyState::KEYSTATE_END };
		bool bPrevPress{ false };
	};

	KeyInfo m_tKeyInfo[VK_MAX]{};
	KeyInfo m_tMouseInfo[MS_MAX]{};

	DIMOUSESTATE m_tMouseState{};

public :
	static CInputDevice* Create(HINSTANCE _hInst, HWND _hWnd);
	virtual void Free() override;
};

END

#endif // InputDev_h__