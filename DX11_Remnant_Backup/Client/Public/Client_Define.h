#pragma once

#ifdef _DEBUG

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "ImGuiFileDialog.h"
#include "ImGuiFileDialogConfig.h"

#endif

#include <process.h>

#include "Client_Macro.h"

namespace Client 
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;
}

using namespace Client;

extern HINSTANCE g_hInst;
extern HWND g_hWnd;