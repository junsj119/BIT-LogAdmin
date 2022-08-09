#include "DeleteTool.h"

#include "stdafx.h"
#include "LogAdmin.h"
#include "LogAdminDlg.h"
#include "afxdialogex.h"

void SetListView(CListCtrl* ListView)
{
	CRect rect;

	ListView->GetClientRect(&rect);
	//����Ʈ ��Ʈ�� �÷� �߰�
	ListView->InsertColumn(0, _T("Program"), LVCFMT_LEFT, 100);
	ListView->InsertColumn(1, _T("Ȯ����"), LVCFMT_LEFT, 100);
	ListView->InsertColumn(2, _T("��ġ"), LVCFMT_LEFT, 297);

	DWORD dwStyle = ListView->GetExtendedStyle();

	ListView->SetExtendedStyle(dwStyle
		| LVS_EX_FULLROWSELECT   //�� ������ ������ �ǰ�
		| LVS_EX_GRIDLINES      //�׸��� ����)
	);
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg) {
		// �������� ���̾�α��� �ʱ�ȭ�� ���� ���
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
	TCHAR szBuffer[512];                                      // ������� ���� 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = hWnd;//g_pMainDlg->GetSafeHwnd(); //GetSafeHwnd
	BrInfo.lpszTitle = _T("������ �����ϼ���");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	BrInfo.lpfn = BrowseCallbackProc;

	//CString strInitPath = _T("C:\\");
	BrInfo.lParam = (LPARAM)strInitPath.GetBuffer();

	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);               // ���ϰ�� �о����

																// ��θ� ������ ����� ���, Edit Control �� �� ����
	CString temp;
	temp.Format(_T("%s"), szBuffer);


	return temp;
}


// DeleteDirectoryFile(������ ���� �ּ�)
BOOL DeleteDirectoryFile(CString RootDir)
{
	if (RootDir == _T(""))
	{
		return FALSE;
	}

	BOOL bRval = FALSE;

	CString szNextDirPath = _T("");
	CString szRoot = _T("");


	// �ش� ���丮�� ��� ������ �˻��Ѵ�.
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

		// . or .. �� ��� ���� �Ѵ�. 
		if (find.IsDots() == FALSE)
		{
			// Directory �� ��� ���ȣ�� �Ѵ�.
			if (find.IsDirectory())
			{
				DeleteDirectoryFile(find.GetFilePath());
			}
			// file�� ��� ����
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

//CheckDisk(&C����̺� ��밡�� ��� EditContrl, &c����̺� �ִ�뷮 ��� EditContrl, & C����̺��뷮 ��� CStatic,
//	&D����̺� ��밡�� ��� EditContrl, &D����̺� �ִ�뷮 ��� EditContrl, & D����̺��뷮 ��� CStatic)
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
	// C:\�� �ϵ��ũ �뷮 ������ �޾� ��
	GetDiskFreeSpaceEx(TEXT("c:\\"), &avail, &total, &free);


	// GByte �� ǥ���� �ϱ� ���� �κ�
	m_total = (float)(total.QuadPart >> 30);
	m_free = (float)(free.QuadPart >> 30);

	// Print out
	//strMsg.Format(_T("C: Total Size: %d GB , Free Size : %d GB\n"), m_total, m_free);
	strMsg.Format(_T("%.0f"), m_free);
	CFree->SetWindowText(strMsg);
	strMsg.Format(_T("%.0f"), m_total);
	CPull->SetWindowText(strMsg);

	float PosData = 100 - ((m_free / m_total) * 100);
	strMsg.Format(_T("%.0f%% �����"), PosData);
	CDrive->SetWindowText(strMsg);

	////////// Drive D
	// D:\�� �ϵ��ũ �뷮 ������ �޾� ��
	GetDiskFreeSpaceEx(TEXT("e:\\"), &avail, &total, &free);

	// GByte �� ǥ���� �ϱ� ���� �κ�
	m_total = (float)(total.QuadPart >> 30);
	m_free = (float)(free.QuadPart >> 30);

	// Print out information of disks
	//strMsg.Format(strMsg + _T("D: Total Size: %d GB , Free Size : %d GB\n"), m_total, m_free);
	strMsg.Format(_T("%.0f"), m_free);
	DFree->SetWindowText(strMsg);
	strMsg.Format(_T("%.0f"), m_total);
	DPull->SetWindowText(strMsg);

	PosData = 100 - ((m_free / m_total) * 100);
	strMsg.Format(_T("%.0f%% �����"), PosData);
	DDrive->SetWindowText(strMsg);
}

//CheckFolder(g_pMainDlg->GetSafeHwnd(), ���� �ּ�, &��� ListContrl, &��� EditContrl)
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

		//�����϶�
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
