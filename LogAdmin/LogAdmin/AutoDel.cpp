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
// 폴더 경로 얻음
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
// 일정 날짜 기준 로그파일 제거
//
BOOL bDelType = FALSE;

void DeleteLogfiles(int nDay)
{
	BOOL bDelType = FALSE;
	// 로그파일 형태 20111116.log
	// 30일 기준 로그파일 삭제
	CTime CurTime = CTime::GetCurrentTime(); // 현재 날짜 시간 알아오기
	CTime DayTime;
	DayTime = CurTime - CTimeSpan(nDay, 0, 0, 0); // 일, 시, 분, 초 삭제할 프로그램 날짜
	CString path, file_path, file_name; 
	if (bDelType == FALSE)
	{
		path.Format(_T("%s*.*"), _T("C:\\Temp\\LogFolder\\DAY\\")); // 찾을 파일
	}
	else
	{
		path.Format(_T("%s*.*"), _T("C:\\Temp\\LogFolder\\HOUR\\")); // 찾을 파일
	}
	CFileFind finder;
	BOOL bRes;
	
	bRes = finder.FindFile(path); // 주어진 경로의 파일을 찾는다. true, false 반환
	while (bRes)
	{
		bRes = finder.FindNextFile(); // 파일이 존재할 경우 다음 파일을 찾을 수있도록 해준다.
		if (!finder.IsDirectory()) // 폴더가 아니고 파일일 경우
		{
			// 삭제 상태 변수 초기화
			bool bDelete = false;
			// 현재 정보가 파일인 경우, file_data.cFileName에 파일이름이 들어있다.
			file_name = finder.GetFileName(); 
			file_path = GetFolderPath(path) + file_name;
			CString strLogDate;
			//strLogDate = file_name.Left(10);
			strLogDate = finder.GetFileTitle();


			// 문자 길이가 맞고, 숫자로만 구성되었는지 확인
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
				// 30일이 지난 로그파일은 삭제
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