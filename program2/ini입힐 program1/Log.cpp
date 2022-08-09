#include "Log.h"
#include "stdafx.h"

void WriteLog(CString msg)
{
	CString strFileName;
	CFile FileName;
	CString log;

	strFileName.Format(_T("c://temp//LogFolder//Program2log.txt"));
	if (FileName.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		FileName.SeekToEnd();
		log.Format(_T("%s"), msg);
		FileName.Write(log, lstrlen(log) * sizeof(TCHAR));
	}
	FileName.Close();
}

void DeleteLog()
{
	CString program2;
	program2.Format(_T("C:\\Temp\\LogFolder\\Program2log.txt"));
	CFileFind finder;

	BOOL Program2;
	Program2 = finder.FindFile(program2);

	if (Program2)
	{
		DeleteFile(program2);
	}
}