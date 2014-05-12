#include <Windows.h>
#include "UI.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//winapi init, window creation, message loop
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS	wc = {0};
	HWND		hwnd;
	MSG			msg;

	wc.hInstance		= hInst;
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName	= L"myClass";
	wc.lpfnWndProc		= (WNDPROC)(UI::WndProc);

	if (!RegisterClass(&wc)) 
	{
		MessageBox(NULL, L"Window registration failed", L"Error", MB_OK);
		return -1;
	}

	hwnd = CreateWindow(wc.lpszClassName, L"Main", WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 530, NULL, NULL, NULL, NULL);
	if (!hwnd) 
	{
		MessageBox(NULL, L"Window creation failed", L"Error", MB_OK);
		return -2;
	}

	InitCommonControls();

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}