#include "stdafx.h"
#include "LogAdmin.h"
#include "LogAdminDlg.h"
#include "ProcSetting.h"

BOOL IsStringDigit(CString str)
{
	int len = str.GetLength();
	char *data = new char[len];
	data = LPSTR(LPCTSTR(str.GetBuffer(len)));
	if (!len) return FALSE;
	for (int x = 0; x < len; ++x)
	{
		if (!isdigit(data[x])) return FALSE;
	}
	return TRUE;
}

//
// ���� ��� ����
//
CString GetFolderPath(CString path)
{
	if (path.Right(1) == _T('\\'))
	{
		return path;
	}
	else
	{
		CString strFolderPath;
		strFolderPath = path.Left(path.ReverseFind(_T('\\')) + 1);
		return strFolderPath;
	}
}

//
// ���� ��¥ ���� �α����� ����
//
BOOL bDelType = FALSE;

void DeleteLogfiles(int nDay)
{
	BOOL bDelType = FALSE;
	// �α����� ���� 20111116.log
	// 30�� ���� �α����� ����
	CTime CurTime = CTime::GetCurrentTime(); // ���� ��¥ �ð� �˾ƿ���
	CTime DayTime;
	DayTime = CurTime - CTimeSpan(nDay, 0, 0, 0); // ��, ��, ��, �� ������ ���α׷� ��¥
	CString path, file_path, file_name; 
	if (bDelType == FALSE)
	{
		path.Format(_T("%s*.*"), _T("C:\\Temp\\LogFolder\\DAY\\")); // ã�� ����
	}
	else
	{
		path.Format(_T("%s*.*"), _T("C:\\Temp\\LogFolder\\HOUR\\")); // ã�� ����
	}
	CFileFind finder;
	BOOL bRes;
	
	bRes = finder.FindFile(path); // �־��� ����� ������ ã�´�. true, false ��ȯ
	while (bRes)
	{
		bRes = finder.FindNextFile(); // ������ ������ ��� ���� ������ ã�� ���ֵ��� ���ش�.
		if (!finder.IsDirectory()) // ������ �ƴϰ� ������ ���
		{
			// ���� ���� ���� �ʱ�ȭ
			bool bDelete = false;
			// ���� ������ ������ ���, file_data.cFileName�� �����̸��� ����ִ�.
			file_name = finder.GetFileName(); 
			file_path = GetFolderPath(path) + file_name;
			CString strLogDate;
			//strLogDate = file_name.Left(10);
			strLogDate = finder.GetFileTitle();


			// ���� ���̰� �°�, ���ڷθ� �����Ǿ����� Ȯ��
			//////////////////////////////////////////////////////////

			CString strYear, strMonth, strDay, strTemp;
			AfxExtractSubString(strYear, strLogDate, 0, '-');
			AfxExtractSubString(strMonth, strLogDate, 1, '-');
			AfxExtractSubString(strDay, strLogDate, 2, '-');

			int nLogYear = _ttoi(strYear);
			int nLogMonth = _ttoi(strMonth);
			int nLogDay = _ttoi(strDay);

			strTemp = strYear + strMonth + strDay;
			//////////////////////////////////////////////////////////

			if (strLogDate.GetLength() == 10 && IsStringDigit(strTemp))
			{
				CTime LogTime(nLogYear, nLogMonth, nLogDay, 0, 0, 0, 0);
				if (LogTime < DayTime)
					bDelete = true;
			}
			if (bDelete)
			{
				// 30���� ���� �α������� ����
				DeleteFile(file_path);
			}
		}
	}

	if (bDelType == FALSE)
	{
		bDelType = TRUE;
	}
	else
	{
		bDelType = FALSE;
	}
}

UINT LogFileDel(LPVOID lpParam)
{
	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;
	int num = 0;
	while (1)
	{
		num = m_MainDlg->n_DelDay;

		DeleteLogfiles(num);

		//Sleep(3000);
		Sleep(3600000);
	}
	return 0;
}

void DeleteTodayTXT()
{
	CString path;
	path.Format(_T("C:\\Temp\\LogFolder\\today.txt"));

	CFileFind finder;
	BOOL bRes;			
																																																																																																													
	bRes = finder.FindFile(path);
	if (bRes)
	{
		DeleteFile(path);
	}
}