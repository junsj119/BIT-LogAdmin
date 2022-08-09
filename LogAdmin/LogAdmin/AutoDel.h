#pragma once
#include "LogAdminDlg.h"

enum  ThreadWorkingType
{
	THREAD_STOP = 0,
	THREAD_RUNNING = 1,
	THREAD_PAUSE = 2
};

BOOL IsStringDigit(CString str);
CString GetFolderPath(CString path);
void DeleteLogfiles(int nDay);
UINT LogFileDel(LPVOID lpParam);
void DeleteTodayTXT();