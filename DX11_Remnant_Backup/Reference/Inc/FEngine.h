#pragma once

BEGIN(Engine)

struct FEngine
{
	HWND hWnd{};
	HINSTANCE hInstance{};

	unsigned int iViewportWidth{};
	unsigned int iViewportHeight{};
};

END