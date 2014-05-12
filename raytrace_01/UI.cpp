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

int UI::RenderUI(HWND hwnd)
{
	hDisplay = CreateWindow(L"Static", L"", WS_CHILD | WS_BORDER | WS_VISIBLE, 5, 5, 640, 480, hwnd, NULL, NULL, NULL);
	CreateWindowW(L"Button", L"Render", WS_CHILD | WS_VISIBLE, 1090, 450, 80, 24, hwnd, (HMENU)IDB_RENDER, NULL, NULL);
	CreateWindowW(L"Button", L"Export to BMP", WS_CHILD | WS_VISIBLE, 650, 450, 120, 24, hwnd, (HMENU)IDB_CREATEBMP, NULL, NULL);
	EnableWindow(GetDlgItem(hwnd, IDB_CREATEBMP), false);

	//create tab handles
	hTab	= CreateWindowW(L"SysTabControl32", L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 650, 5, 523, 440, hwnd, NULL, NULL, NULL);
	hTab0	= CreateWindowW(L"SysTabControl32", L"", WS_CHILD | WS_CLIPSIBLINGS, 0, 22, 523, 418, hTab, NULL, NULL, NULL);
	hTab1	= CreateWindowW(L"SysTabControl32", L"", WS_CHILD | WS_CLIPSIBLINGS, 0, 22, 523, 418, hTab, NULL, NULL, NULL);
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
	CreateWindow(L"Button", L"Add", WS_CHILD | WS_VISIBLE, 5, 385, 80, 24, hTab0, (HMENU)IDB_ADDOBJECT, NULL, NULL);
	CreateWindow(L"Button", L"Remove", WS_CHILD | WS_VISIBLE, 90, 385, 80, 24, hTab0, (HMENU)IDB_DELETEOBJECT, NULL, NULL);
	hList = CreateWindowExW(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES, WC_LISTVIEWW, L"", WS_CHILD | WS_BORDER | WS_VISIBLE | LVS_REPORT, 5, 10, 510, 370, hTab0, NULL, NULL, NULL);
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
	swprintf(str, L"X: %.4lf, Y: %.4lf, Z: %.4lf", position.x(), position.y(), position.z());	
	
	//add item
	item.iSubItem = 2;
	item.pszText = str;
	ListView_SetItem(hList, &item);

	//convert color to wchar_t*
	swprintf(str, L"red: %d, green: %d, blue: %d", (int)(color.red()*255), (int)(color.green()*255), (int)(color.blue()*255));
	item.iSubItem = 3;
	item.pszText = str;
	ListView_SetItem(hList, &item);

	swprintf(str, L"%.4lf", color.special());
	item.iSubItem = 4;
	item.pszText = str;
	ListView_SetItem(hList, &item);

	swprintf(str, L"%.4lf", radius);
	item.iSubItem = 5;
	item.pszText = str;
	ListView_SetItem(hList, &item);

	ListView_SetCheckState(hList, this->listItemCount-1, true);
}

void RenderThreadFunc(void* hwnd)
{
	SetWindowTextA((HWND)hwnd, "Rendering...");
	RayTrace::getInstance().Render();
	SetWindowTextA((HWND)hwnd, "Rendering done");

	SendMessage((HWND)hwnd, WM_PAINT, NULL, NULL);
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
			//HANDLE thread = (HANDLE)_beginthreadex(0, 0, RenderThreadFunc, hwnd, 0, 0);
			//WaitForSingleObject(thread, INFINITE);
			RenderThreadFunc(hwnd);			
			break;
		}
		break;

	case WM_PAINT:
		if (RayTrace::getInstance().isReady()) 
		{
			RayTrace::getInstance().Draw(hDisplay);
			//break;
		}

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

