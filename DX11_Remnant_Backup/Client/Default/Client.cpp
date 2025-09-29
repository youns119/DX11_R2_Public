#include "pch.h"
#include "framework.h"
#include "Client.h"

#include "CMainApp.h"

#include "CGameInstance.h"

#define MAX_LOADSTRING 100

HWND g_hWnd;
HINSTANCE g_hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifdef _DEBUG

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

    CMainApp* pMainApp{};

    pMainApp = CMainApp::Create();
    if (pMainApp == nullptr)
        return FALSE;

    if (FAILED(CGameInstance::Add_Timer(TEXT("Timer_Default"))))
        return FALSE;
    if (FAILED(CGameInstance::Add_Timer(TEXT("Timer_60"))))
        return FALSE;

    _float fTimerAcc{};

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        CGameInstance::Update_Timer(TEXT("Timer_Default"));
        fTimerAcc += CGameInstance::Get_TimeDelta(TEXT("Timer_Default"));

        if (fTimerAcc >= 1.f / 60.f)
        {
            CGameInstance::Update_Timer(TEXT("Timer_60"));

            _float fDeltaTime = CGameInstance::Get_TimeDelta(TEXT("Timer_60"));

            pMainApp->Pre_Update(fDeltaTime);
            pMainApp->Update(fDeltaTime);
            pMainApp->Post_Update(fDeltaTime);
            pMainApp->Render();

            fTimerAcc = 0.f;
        }
    }

    if (Safe_Release(pMainApp) != 0)
    {
        MSG_BOX("Failed to Release : CMainApp");
        return FALSE;
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance;

   RECT tRect = { 0, 0, g_iWinSizeX, g_iWinSizeY };

   AdjustWindowRect(&tRect, WS_OVERLAPPEDWINDOW, true);

   HWND hWnd = CreateWindowW
   (
       szWindowClass, 
       szTitle, 
       WS_OVERLAPPEDWINDOW,
       0, 0, 
       tRect.right - tRect.left,
       tRect.bottom - tRect.top,
       nullptr, 
       nullptr, 
       hInstance, 
       nullptr
   );

   if (!hWnd)
      return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   g_hWnd = hWnd;

   return TRUE;
}

#ifdef _DEBUG

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG

    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

#endif

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_KEYDOWN :
        switch (wParam)
        {
        case VK_ESCAPE :
            DestroyWindow(g_hWnd);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}