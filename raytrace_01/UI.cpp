#include "UI.h"
#include "ObjectDialog.h"
#include <process.h>

#define	OBJECTDIALOG		0x8000
#define	IDB_RENDER			0x8001
#define	IDB_ADDOBJECT		0x8002
#define	IDB_DELETEOBJECT	0x8003
#define	IDB_ADDLIGHT		0x8004
#define	IDB_DELETELIGHT		0x8005
#define	IDB_CREATEBMP		0x8006

static HWND	hDisplay;
WNDPROC OriginalButtonProc;

UI& UI::getInstance()
{
	static UI instance;
	return instance;
}

UI::UI()
{
	listItemCount = 0;
}

void UI::HideTab()
{
	switch (TabCtrl_GetCurFocus(hTab))
	{
	case 0:
		ShowWindow(hTab0, SW_HIDE);
		break;
	case 1:
		ShowWindow(hTab1, SW_HIDE);
		break;
	}
}

void UI::ShowTab()
{
	switch (TabCtrl_GetCurFocus(hTab))
	{
	case 0:
		ShowWindow(hTab0, SW_SHOW);
		break;
	case 1:
		ShowWindow(hTab1, SW_SHOW);
		break;
	}
}

LRESULT CALLBACK UI::ButtonProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
		{
			HWND dialog = CreateWindowEx(WS_EX_CONTROLPARENT, L"static", L"Add Object", WS_SYSMENU | WS_BORDER | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, NULL, NULL);
			SetWindowLong(dialog, GWL_WNDPROC, (LONG)ObjectDialog::objDialogCallback);
			int error = GetLastError();
			SendMessage(dialog, WM_CREATE, 0, 0);
		}
		break;

	default:
		OriginalButtonProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int UI::RenderUI(HWND hwnd)
{
	HINSTANCE hInst = GetModuleHandle(NULL);
	hDisplay = CreateWindow(L"Static", L"", WS_CHILD | WS_BORDER | WS_VISIBLE, 5, 5, 640, 480, hwnd, (HMENU)NULL, hInst, NULL);
	CreateWindowW(L"Button", L"Render", WS_CHILD | WS_VISIBLE, 1100, 460, 80, 24, hwnd, (HMENU)IDB_RENDER, hInst, NULL);
	CreateWindowW(L"Button", L"Export to BMP", WS_CHILD | WS_VISIBLE, 650, 460, 120, 24, hwnd, (HMENU)IDB_CREATEBMP, hInst, NULL);
	EnableWindow(GetDlgItem(hwnd, IDB_CREATEBMP), false);

	//create tab handles
	hTab	= CreateWindowExW(WS_EX_CONTROLPARENT, L"SysTabControl32", L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 650, 5, 530, 450, hwnd, NULL, hInst, NULL);
	hTab0	= CreateWindowExW(WS_EX_CONTROLPARENT, L"SysTabControl32", L"", WS_CHILD | WS_CLIPSIBLINGS, 0, 22, 530, 428, hTab, NULL, hInst, NULL);
	hTab1	= CreateWindowExW(WS_EX_CONTROLPARENT, L"SysTabControl32", L"", WS_CHILD | WS_CLIPSIBLINGS, 0, 22, 530, 428, hTab, NULL, hInst, NULL);
	ShowWindow(hTab0, SW_SHOW);
	ShowWindow(hTab1, SW_HIDE);

	//create tab buttons
	TC_ITEM tie;
	tie.mask = TCIF_TEXT;
	tie.pszText = L"Objects";
	TabCtrl_InsertItem(hTab, 0, &tie);
	tie.pszText = L"Lights";
	TabCtrl_InsertItem(hTab, 1, &tie);

	//create tab0 content
	HWND btn01 = CreateWindowEx(WS_EX_CONTROLPARENT, L"Button", L"Add", WS_CHILD | WS_VISIBLE, 5, 396, 80, 24, hTab0, (HMENU)IDB_ADDOBJECT, hInst, NULL);
	HWND btn02 = CreateWindowEx(WS_EX_CONTROLPARENT, L"Button", L"Remove", WS_CHILD | WS_VISIBLE, 90, 396, 80, 24, hTab0, (HMENU)IDB_DELETEOBJECT, hInst, NULL);
	OriginalButtonProc = (WNDPROC)GetWindowLong(hTab0, GWL_WNDPROC);
	SetWindowLong(hTab0, GWL_WNDPROC, (LONG)ButtonProc);

	hList = CreateWindowExW(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES, WC_LISTVIEWW, L"", WS_CHILD | WS_BORDER | WS_VISIBLE | LVS_REPORT, 5, 10, 515, 380, hTab0, NULL, hInst, NULL);
	DWORD style = LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES;
	ListView_SetExtendedListViewStyle(hList, style);

	LV_COLUMN column = {0};
	column.mask = LVCF_TEXT | LVCF_WIDTH;
	column.cx = 20;
	ListView_InsertColumn(hList, 0, &column);
	column.cx = 60;
	column.pszText = L"Type";
	ListView_InsertColumn(hList, 1, &column);
	column.cx = 180;
	column.pszText = L"Position";
	ListView_InsertColumn(hList, 2, &column);
	column.cx = 180;
	column.pszText = L"Color";
	ListView_InsertColumn(hList, 3, &column);
	column.cx = 50;
	column.pszText = L"Refraction";
	ListView_InsertColumn(hList, 4, &column);
	column.cx = 50;
	column.pszText = L"Radius";
	ListView_InsertColumn(hList, 5, &column);

	AddListItem(L"Sphere", Vector(), Color(), 1.0);
	AddListItem(L"Sphere", Vector(), Color(), 0.5);
	return 0;
}

void UI::AddListItem(wchar_t* type, Vector position, Color color, float radius)
{
	LVITEM item = {0};
	item.mask = LVIF_TEXT;
	item.iItem = this->listItemCount++;
	item.iSubItem = 0;
	ListView_InsertItem(hList, &item);

	item.iSubItem = 1;
	item.pszText = type;
	ListView_SetItem(hList, &item);

	//convert vector to wchar_t*
	wchar_t str[80];
	swprintf(str, 80, L"X: %.4lf, Y: %.4lf, Z: %.4lf", position.x(), position.y(), position.z());	

	//add item
	item.iSubItem = 2;
	item.pszText = str;
	ListView_SetItem(hList, &item);

	//convert color to wchar_t*
	swprintf(str, 80, L"red: %d, green: %d, blue: %d", (int)(color.red()*255), (int)(color.green()*255), (int)(color.blue()*255));
	item.iSubItem = 3;
	item.pszText = str;
	ListView_SetItem(hList, &item);

	swprintf(str, 80, L"%.4lf", color.special());
	item.iSubItem = 4;
	item.pszText = str;
	ListView_SetItem(hList, &item);

	swprintf(str, 80, L"%.4lf", radius);
	item.iSubItem = 5;
	item.pszText = str;
	ListView_SetItem(hList, &item);

	ListView_SetCheckState(hList, this->listItemCount-1, true);
}

DWORD WINAPI RenderThreadFunc(void* hwnd)
{
	SetWindowTextA((HWND)hwnd, "Rendering...");
	RayTrace::getInstance().Render();
	SetWindowTextA((HWND)hwnd, "Rendering done");

	RedrawWindow((HWND)hwnd, NULL, NULL, RDW_INTERNALPAINT | RDW_ALLCHILDREN);
	return GetLastError();
}

//winapi main message loop processing method
LRESULT CALLBACK UI::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static Timer timer;
	int error = 0;

	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		UI::getInstance().RenderUI(hwnd);
		break;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
		case TCN_SELCHANGING: 
			UI::getInstance().HideTab();
			break;
		case TCN_SELCHANGE:
			UI::getInstance().ShowTab();
			break;			
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam)) 
		{
		case IDB_RENDER:
			//PTHREAD_START_ROUTINE
			DWORD threadID;
			HANDLE thread = CreateThread(0, 0, RenderThreadFunc, hwnd, 0, &threadID);
			int error = GetLastError();
			
			break;
		}
		break;

	case WM_PAINT:
		if (RayTrace::getInstance().isReady()) 
		{
			RayTrace::getInstance().Draw(hDisplay);
		}

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

