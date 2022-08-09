#include "DeleteTool.h"

#include "stdafx.h"
#include "LogAdmin.h"
#include "LogAdminDlg.h"
#include "afxdialogex.h"

void SetListView(CListCtrl* ListView)
{
	CRect rect;

	ListView->GetClientRect(&rect);
	//리스트 컨트롤 컬럼 추가
	ListView->InsertColumn(0, _T("Program"), LVCFMT_LEFT, 100);
	ListView->InsertColumn(1, _T("확장자"), LVCFMT_LEFT, 100);
	ListView->InsertColumn(2, _T("위치"), LVCFMT_LEFT, 297);

	DWORD dwStyle = ListView->GetExtendedStyle();

	ListView->SetExtendedStyle(dwStyle
		| LVS_EX_FULLROWSELECT   //뭘 눌러도 선택이 되게
		| LVS_EX_GRIDLINES      //그리드 라인)
	);
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg) {
		// 폴더선택 다이얼로그의 초기화가 끝난 경우
	case BFFM_INITIALIZED:
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		break;
	}
	return 0;
}

// FolderChoice(g_pMainDlg->GetSafeHwnd())
CString FolderChoice(HWND hWnd, CString strInitPath)
{
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                      // 경로저장 버퍼 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = hWnd;//g_pMainDlg->GetSafeHwnd(); //GetSafeHwnd
	BrInfo.lpszTitle = _T("폴더를 선택하세요");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	BrInfo.lpfn = BrowseCallbackProc;

	//CString strInitPath = _T("C:\\");
	BrInfo.lParam = (LPARAM)strInitPath.GetBuffer();

	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);               // 파일경로 읽어오기

																// 경로를 가져와 사용할 경우, Edit Control 에 값 저장
	CString temp;
	temp.Format(_T("%s"), szBuffer);


	return temp;
}


// DeleteDirectoryFile(삭제할 폴더 주소)
BOOL DeleteDirectoryFile(CString RootDir)
{
	if (RootDir == _T(""))
	{
		return FALSE;
	}

	BOOL bRval = FALSE;

	CString szNextDirPath = _T("");
	CString szRoot = _T("");


	// 해당 디렉토리의 모든 파일을 검사한다.
	szRoot.Format(_T("%s\\*.*"), RootDir);

	CFileFind find;

	bRval = find.FindFile(szRoot);

	if (bRval == FALSE)
	{
		return bRval;
	}

	while (bRval)
	{
		bRval = find.FindNextFile();

		// . or .. 인 경우 무시 한다. 
		if (find.IsDots() == FALSE)
		{
			// Directory 일 경우 재귀호출 한다.
			if (find.IsDirectory())
			{
				DeleteDirectoryFile(find.GetFilePath());
			}
			// file일 경우 삭제
			else
			{
				bRval = DeleteFile(find.GetFilePath());
			}
		}
	}
	find.Close();
	bRval = RemoveDirectory(RootDir);
	return bRval;
}

//CheckDisk(&C드라이브 사용가능 출력 EditContrl, &c드라이브 최대용량 출력 EditContrl, & C드라이브사용량 출력 CStatic,
//	&D드라이브 사용가능 출력 EditContrl, &D드라이브 최대용량 출력 EditContrl, & D드라이브사용량 출력 CStatic)
void CheckDisk(CEdit* CFree, CEdit* CPull, CStatic* CDrive, CEdit* DFree, CEdit* DPull, CStatic* DDrive)
{
	ULARGE_INTEGER avail, total, free;
	avail.QuadPart = 0L;
	total.QuadPart = 0L;
	free.QuadPart = 0L;

	int m_avail;
	float m_total, m_free;

	CString strMsg;

	////////// Drive C
	// C:\의 하드디스크 용량 정보를 받아 옴
	GetDiskFreeSpaceEx(TEXT("c:\\"), &avail, &total, &free);


	// GByte 로 표현을 하기 위한 부분
	m_total = (float)(total.QuadPart >> 30);
	m_free = (float)(free.QuadPart >> 30);

	// Print out
	//strMsg.Format(_T("C: Total Size: %d GB , Free Size : %d GB\n"), m_total, m_free);
	strMsg.Format(_T("%.0f"), m_free);
	CFree->SetWindowText(strMsg);
	strMsg.Format(_T("%.0f"), m_total);
	CPull->SetWindowText(strMsg);

	float PosData = 100 - ((m_free / m_total) * 100);
	strMsg.Format(_T("%.0f%% 사용중"), PosData);
	CDrive->SetWindowText(strMsg);

	////////// Drive D
	// D:\의 하드디스크 용량 정보를 받아 옴
	GetDiskFreeSpaceEx(TEXT("e:\\"), &avail, &total, &free);

	// GByte 로 표현을 하기 위한 부분
	m_total = (float)(total.QuadPart >> 30);
	m_free = (float)(free.QuadPart >> 30);

	// Print out information of disks
	//strMsg.Format(strMsg + _T("D: Total Size: %d GB , Free Size : %d GB\n"), m_total, m_free);
	strMsg.Format(_T("%.0f"), m_free);
	DFree->SetWindowText(strMsg);
	strMsg.Format(_T("%.0f"), m_total);
	DPull->SetWindowText(strMsg);

	PosData = 100 - ((m_free / m_total) * 100);
	strMsg.Format(_T("%.0f%% 사용중"), PosData);
	DDrive->SetWindowText(strMsg);
}

//CheckFolder(g_pMainDlg->GetSafeHwnd(), 폴더 주소, &출력 ListContrl, &출력 EditContrl)
void CheckFolder(HWND hwnd, CString Dir, CListCtrl* ListView, CEdit* DirEdit)
{
	ListView->DeleteAllItems();
	if (Dir == _T(""))
		Dir = FolderChoice(hwnd, _T("C:\\"));
	CString DirTemp = Dir + "/*.*";
	DirEdit->SetWindowText(DirTemp);

	CFileFind finder;
	BOOL bWorking = finder.FindFile(DirTemp);
	CString strPathName;
	//CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	CString fileName;
	CString DirName;
	int i = 0;

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
			ListView->SetItemText(i, 2, Dir);
			continue;
		}
		CString _fileName = finder.GetFileName();
		strPathName = PathFindExtension(finder.GetFilePath());

		if (_fileName == _T("Thumbs.db")) continue;

		fileName = finder.GetFileTitle();
		ListView->InsertItem(i, fileName);
		ListView->SetItemText(i, 1, strPathName);
		ListView->SetItemText(i, 2, Dir);
		i++;
	}
}
