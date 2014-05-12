#ifndef OBJECTDIALOG_H
#define OBJECTDIALOG_H

#include <Windows.h>

class ObjectDialog
{
public:
	ObjectDialog();

	static BOOL CALLBACK objDialogCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:

};

#endif