#include "ObjectDialog.h"
#include <commdlg.h>

#define IDB_CHOOSECOLOR 0xb000

BOOL CALLBACK ObjectDialog::objDialogCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//CChook

	switch (msg)
	{
	case WM_CREATE:
		CreateWindow(L"Static", L"Position", WS_CHILD | WS_VISIBLE, 10, 10, 80, 24, hwnd, NULL, NULL, NULL);
		CreateWindow(L"Static", L"X", WS_CHILD | WS_VISIBLE, 20, 41, 80, 24, hwnd, NULL, NULL, NULL);
		CreateWindow(L"Static", L"Y", WS_CHILD | WS_VISIBLE, 110, 41, 80, 24, hwnd, NULL, NULL, NULL);
		CreateWindow(L"Static", L"Z", WS_CHILD | WS_VISIBLE, 200, 41, 80, 24, hwnd, NULL, NULL, NULL);
		CreateWindow(L"Edit", L"0.0", WS_CHILD | WS_VISIBLE | WS_BORDER, 35, 40, 60, 20, hwnd, NULL, NULL, NULL);
		CreateWindow(L"Edit", L"0.0", WS_CHILD | WS_VISIBLE | WS_BORDER, 125, 40, 60, 20, hwnd, NULL, NULL, NULL);
		CreateWindow(L"Edit", L"0.0", WS_CHILD | WS_VISIBLE | WS_BORDER, 215, 40, 60, 20, hwnd, NULL, NULL, NULL);

		CreateWindow(L"Static", L"Color", WS_CHILD | WS_VISIBLE, 10, 70, 80, 24, hwnd, NULL, NULL, NULL);
		CreateWindow(L"Static", L"", WS_CHILD | WS_VISIBLE, 100, 70, 24, 24, hwnd, NULL, NULL, NULL);
		CreateWindow(L"Button", L"Choose", WS_CHILD | WS_VISIBLE, 130, 70, 80, 24, hwnd, (HMENU)IDB_CHOOSECOLOR, NULL, NULL);

		CreateWindow(L"Static", L"Refraction", WS_CHILD | WS_VISIBLE, 10, 100, 80, 24, hwnd, NULL, NULL, NULL);
		CreateWindow(L"Static", L"Radius", WS_CHILD | WS_VISIBLE, 10, 130, 80, 24, hwnd, NULL, NULL, NULL);
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_NOTIFY:
		break;

	case WM_PAINT:
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case IDB_CHOOSECOLOR:
			{
				static COLORREF customArray[16];
				static DWORD rgbCurrent = 0xff0000;
				CHOOSECOLOR color = {0};
				color.lStructSize = sizeof(CHOOSECOLOR);
				color.Flags = CC_FULLOPEN | CC_RGBINIT;
				color.lpCustColors = customArray;
				color.rgbResult = rgbCurrent;

				if (ChooseColor(&color))
				{
					MessageBoxA(NULL, "GREAT", "", MB_OK);
				} 
				else 
				{
					int error = GetLastError();
					MessageBoxA(NULL, "FAIL", "", MB_OK);
				}
			}
			
			break;
		}
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);;
}