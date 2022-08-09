#include "stdafx.h"
#include "LogAdmin.h"
#include "LogAdminDlg.h"
#include "ProcSetting.h"
#include "Tool.h"
#include<tlhelp32.h>

// .exe 검색 TRUE or FALSE 반환
bool IsExistProcess(CString ProcessName)
{
	bool ret = false;
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcess != FALSE) {
		PROCESSENTRY32 pe32 = { 0 };
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(hProcess, &pe32)) {
			do {
				if (CString(pe32.szExeFile) == ProcessName) {
					ret = true;
					break;
				}
			} while (Process32Next(hProcess, &pe32));
		}
		CloseHandle(hProcess);
	}
	return ret;
}

int NumExistProcess(CString ProcessName)
{
	int ProcNum = 0;
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcess != FALSE) {
		PROCESSENTRY32 pe32 = { 0 };
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(hProcess, &pe32)) {
			do {
				if (CString(pe32.szExeFile) == ProcessName) {
					ProcNum += 1;
				}
			} while (Process32Next(hProcess, &pe32));
		}
		CloseHandle(hProcess);
	}
	return ProcNum;
}

void LogFileFind(HWND hwnd, CListCtrl* ListView, CString Dir)
{
	ListView->DeleteAllItems();
	int i = 0;
	CString strPathName;
	//CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);
	CString fileName;
	CString DirName;
	CFileFind finder;

	for (int j = 0; j < 2; j++)
	{
		CString DirTemp;
		if (j == 0)
			DirTemp = Dir + "/*.txt";
		else
			DirTemp = Dir + "/*.xls";

		BOOL bWorking = finder.FindFile(DirTemp);

		while (bWorking)
		{

			bWorking = finder.FindNextFile();

			//폴더일때
			if (finder.IsDirectory() || finder.IsDots())
			{
				DirName = finder.GetFileName();
				strPathName = finder.GetFilePath();
				if (DirName == "." || DirName == "..")
					continue;
				ListView->InsertItem(i, DirName);
				ListView->SetItemText(i, 1, _T("Folder"));
				continue;
			}
			CString _fileName = finder.GetFileName();
			strPathName = PathFindExtension(finder.GetFilePath());

			if (_fileName == _T("Thumbs.db")) continue;

			fileName = finder.GetFileTitle();
			ListView->InsertItem(i, fileName);
			ListView->SetItemText(i, 1, strPathName);
			i++;
		}
	}
}

void TrayIconMake()
{
	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;

	NOTIFYICONDATA nid;

	ZeroMemory(&nid, sizeof(nid));

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uID = 0;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.hWnd = m_MainDlg->m_hWnd;
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	nid.uCallbackMessage = WM_TRAY_NOTIFYICACTOIN;
	lstrcpy(nid.szTip, _T("Log Admin 입니다."));

	::Shell_NotifyIcon(NIM_ADD, &nid);
	m_MainDlg->ShowWindow(SW_HIDE);
}
HANDLE hEventCalc;

//ProcSetting ps;
int intRbtCnt;
//vector<ProcMember> m_proSet;

DWORD WINAPI ThreadProc(LPVOID p)
{
	WaitForSingleObject(hEventCalc, INFINITE);

	CString procName;

	//CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)AfxGetApp()->GetMainWnd();
	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;
	HWND hWnd = FindWindow(NULL, _T("LogAdmin"));

	while (1)
	{
		for (int index = 0; index < m_MainDlg->ProcList.size(); index++)
		{

			procName = m_MainDlg->ProcList[index].procName;
			CString path = m_MainDlg->ProcList[index].procPath;

			if (IsExistProcess(procName) == false) // IsExistProcess 프로세스 검색 flase 실행중이 아님
			{
				if (m_MainDlg->ProcList[index].procClose == true)
				{
					m_MainDlg->b_Popup = TRUE;
					m_MainDlg->SetTimer(101, 1000, NULL);
					if (m_MainDlg->MessageBox(_T("종료되었습니다.")) == IDOK)
					{
						//Sleep(2000);
					}
					m_MainDlg->b_Popup = FALSE;
				}
				//PostMessage(hWnd, WM_CLOSE, 0, 0);

				ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWNORMAL);

				intRbtCnt = _tstoi(m_MainDlg->ProcList[index].rbtCnt);
				intRbtCnt = intRbtCnt + 1;
				m_MainDlg->ProcList[index].rbtCnt.Format(_T("%d"), intRbtCnt);

				m_MainDlg->ProcList[index].procClose = true;
				m_MainDlg->m_watchProc.SetItemText(index, 1, m_MainDlg->ProcList[index].rbtCnt);
			}

			if (NumExistProcess(procName) > 1 && m_MainDlg->ProcList[index].procOverlap == false) // 프로세스 갯수가 1 초과이며 중복이 이미 뜬게 아니면 들어간다.
			{
				AfxMessageBox(_T("프로그램이 이미 실행 중입니다."));
				m_MainDlg->ProcList[index].procOverlap = true;
			}
			else if (NumExistProcess(procName) <= 1 && m_MainDlg->ProcList[index].procOverlap == true) // 프로세스 갯수가 1 이하이며 중복이 이미 팝업이 떴으면 돌아간다.
			{
				m_MainDlg->ProcList[index].procOverlap = false;
			}

			Sleep(3000);
		}
	}
	return 0;
}
//////////////////////////////////////////////////