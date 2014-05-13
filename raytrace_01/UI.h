#ifndef UI_H
#define UI_H

#include <Windows.h>
#include <CommCtrl.h>
#include "RayTrace.h"

class UI
{
public:
	static UI&	getInstance();
	int			RenderUI(HWND hwnd);
	void		HideTab();
	void		ShowTab();

	static LRESULT CALLBACK		WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK		ButtonProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	UI();

	void	AddListItem(wchar_t* type, Vector position, Color color, float radius);

	HWND	hTab;
	HWND	hTab0;
	HWND	hTab1;
	HWND	hList;
	int		listItemCount;
};

#endif
