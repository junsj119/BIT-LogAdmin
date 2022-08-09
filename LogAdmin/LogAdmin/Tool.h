#pragma once
#include "LogAdminDlg.h"
#define WM_TRAY_NOTIFYICACTOIN WM_APP+10
enum {
	TRAY_BALLOON = 0
};

bool IsExistProcess(CString ProcessName);
int NumExistProcess(CString ProcessName);
void LogFileFind(HWND hwnd, CListCtrl* ListView, CString Dir);
void TrayIconMake();
DWORD WINAPI ThreadProc(LPVOID p);