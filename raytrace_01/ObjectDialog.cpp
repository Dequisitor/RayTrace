#include "ObjectDialog.h"

BOOL CALLBACK ObjectDialog::objDialogCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		CreateWindow(L"Button", L"csöcs", WS_CHILD | WS_VISIBLE, 10, 10, 80, 24, hwnd, NULL, NULL, NULL);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
	}

	return false;
}