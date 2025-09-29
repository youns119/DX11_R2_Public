#include "CInputDevice.h"

CInputDevice::CInputDevice()
{
}

HRESULT CInputDevice::Init(HINSTANCE _hInst, HWND _hWnd)
{
	m_hWnd = _hWnd;

	if (FAILED(DirectInput8Create
	(
		_hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		nullptr
	)))
		return E_FAIL;

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoard->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyBoard->Acquire();

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->Acquire();

	return S_OK;
}

void CInputDevice::Update()
{
	m_bFocus = GetForegroundWindow() == m_hWnd;

	_byte byKeyState[VK_MAX]{};

	m_pKeyBoard->GetDeviceState(VK_MAX, byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

	// Keyboard
	for (int i = 0; i < VK_MAX; i++)
	{
		if (m_bFocus)
		{
			if (byKeyState[i] & 0x80)
			{
				if (m_tKeyInfo[i].bPrevPress)
					m_tKeyInfo[i].eKeyState = EKeyState::KEYSTATE_HOLD;
				else
					m_tKeyInfo[i].eKeyState = EKeyState::KEYSTATE_PRESS;

				m_tKeyInfo[i].bPrevPress = true;
			}
			else
			{
				if (m_tKeyInfo[i].bPrevPress)
					m_tKeyInfo[i].eKeyState = EKeyState::KEYSTATE_RELEASE;
				else
					m_tKeyInfo[i].eKeyState = EKeyState::KEYSTATE_END;

				m_tKeyInfo[i].bPrevPress = false;
			}
		}
		else
		{
			if (m_tKeyInfo[i].bPrevPress)
				m_tKeyInfo[i].eKeyState = EKeyState::KEYSTATE_RELEASE;
			else
				m_tKeyInfo[i].eKeyState = EKeyState::KEYSTATE_END;

			m_tKeyInfo[i].bPrevPress = false;
		}
	}

	// Mouse
	for (_int i = 0; i < MS_MAX; i++)
	{
		if (m_bFocus)
		{
			if (m_tMouseState.rgbButtons[i] & 0x80)
			{
				if (m_tMouseInfo[i].bPrevPress)
					m_tMouseInfo[i].eKeyState = EKeyState::KEYSTATE_HOLD;
				else
					m_tMouseInfo[i].eKeyState = EKeyState::KEYSTATE_PRESS;

				m_tMouseInfo[i].bPrevPress = true;
			}
			else
			{
				if (m_tMouseInfo[i].bPrevPress)
					m_tMouseInfo[i].eKeyState = EKeyState::KEYSTATE_RELEASE;
				else
					m_tMouseInfo[i].eKeyState = EKeyState::KEYSTATE_END;

				m_tMouseInfo[i].bPrevPress = false;
			}
		}
		else
		{
			if (m_tMouseInfo[i].bPrevPress)
				m_tMouseInfo[i].eKeyState = EKeyState::KEYSTATE_RELEASE;
			else
				m_tMouseInfo[i].eKeyState = EKeyState::KEYSTATE_END;

			m_tMouseInfo[i].bPrevPress = false;
		}
	}
}

CInputDevice* CInputDevice::Create(HINSTANCE _hInst, HWND _hWnd)
{
	CInputDevice* pInstance = new CInputDevice();

	if (FAILED(pInstance->Init(_hInst, _hWnd)))
	{
		MSG_BOX("Failed to Created : CInputDevice");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInputDevice::Free()
{
	__super::Free();

	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyBoard);

	Safe_Release(m_pInputSDK);
}