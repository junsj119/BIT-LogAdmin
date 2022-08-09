#include "Log.h"
#include "stdafx.h"
#include "ClientSocket.h"
#include "LogAdmin.h"
#include "LogAdminDlg.h"
#include "stdafx.h"

int ListNum = 0;

void SetZero()
{
	ListNum = 0;
}

void AddLogList(CString msg)
{
	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;

	//if (m_MainDlg->b_LiveLog == TRUE)
	//{
	int idx = m_MainDlg->n_LogViewSet;
	CString error = _T("[Error]");
	CString warning = _T("[Warning]");
	BOOL setLog = FALSE;
	BOOL setLog1 = FALSE; // ����
	BOOL setProc = FALSE;
	BOOL setLog2 = FALSE; // ����

	CString strDate, strRealName, strPname, strMsg;
	AfxExtractSubString(strDate, msg, 0, '_');
	AfxExtractSubString(strRealName, msg, 1, '_');	//Receive a���� .exe �̸�
	AfxExtractSubString(strPname, msg, 2, '_');
	AfxExtractSubString(strMsg, msg, 3, '_');

	int nSetProc = m_MainDlg->comBoIdx; // ���⼭ �޺� �ڽ� ���� ������ ���� ��������

	CString strComboName, strTemp;	//	 �޺��ڽ� �ȿ��ִ� .exe �̸�
	int num = -1;
	if (nSetProc != 0)
	{
		//���⼭ �޺� �ڽ� ������ �̸� ��������
		strTemp = m_MainDlg->selectprocName;
		AfxExtractSubString(strComboName, strTemp, 0, '.');
		num = strRealName.Find(strComboName);

		if (num != -1)
		{
			setProc = TRUE;
		}
		else
		{
			setProc = FALSE;
		}
	}

	/////////////////////////////////////////
	if (msg.Find(error) != -1)
	{
		setLog1 = TRUE;
	}
	else
	{
		setLog1 = FALSE;
	}
	if (msg.Find(warning) != -1)
	{
		setLog2 = TRUE;
	}
	else
	{
		setLog2 = FALSE;
	}

	if (setLog1 == FALSE && setLog2 == FALSE)
		setLog = TRUE;
	else
		setLog = FALSE;


	//selectprocName
	//////////////////////////////////////
	if (nSetProc == 0 || setProc == TRUE)
	{
		if (idx == 0)
		{
			m_MainDlg->m_logList.InsertItem(ListNum, strDate);
			m_MainDlg->m_logList.SetItemText(ListNum, 1, strRealName);
			m_MainDlg->m_logList.SetItemText(ListNum, 2, strPname);
			m_MainDlg->m_logList.SetItemText(ListNum, 3, strMsg);
			ListNum++;
		}
		else if (idx == 1)
		{
			if (setLog == TRUE && setLog1 == FALSE && setLog2 == FALSE)
			{
				m_MainDlg->m_logList.InsertItem(ListNum, strDate);
				m_MainDlg->m_logList.SetItemText(ListNum, 1, strRealName);
				m_MainDlg->m_logList.SetItemText(ListNum, 2, strPname);
				m_MainDlg->m_logList.SetItemText(ListNum, 3, strMsg);
				ListNum++;
			}
		}
		else if (idx == 2)
		{
			if (setLog == FALSE && setLog1 == TRUE && setLog2 == FALSE)
			{
				m_MainDlg->m_logList.InsertItem(ListNum, strDate);
				m_MainDlg->m_logList.SetItemText(ListNum, 1, strRealName);
				m_MainDlg->m_logList.SetItemText(ListNum, 2, strPname);
				m_MainDlg->m_logList.SetItemText(ListNum, 3, strMsg);
				ListNum++;
			}
		}
		else if (idx == 3)
		{
			if (setLog == FALSE && setLog1 == FALSE && setLog2 == TRUE)
			{
				m_MainDlg->m_logList.InsertItem(ListNum, strDate);
				m_MainDlg->m_logList.SetItemText(ListNum, 1, strRealName);
				m_MainDlg->m_logList.SetItemText(ListNum, 2, strPname);
				m_MainDlg->m_logList.SetItemText(ListNum, 3, strMsg);
				ListNum++;
			}
		}
		else if (idx == 4)
		{
			if (setLog == TRUE || setLog1 == TRUE && setLog2 == FALSE)
			{
				m_MainDlg->m_logList.InsertItem(ListNum, strDate);
				m_MainDlg->m_logList.SetItemText(ListNum, 1, strRealName);
				m_MainDlg->m_logList.SetItemText(ListNum, 2, strPname);
				m_MainDlg->m_logList.SetItemText(ListNum, 3, strMsg);
				ListNum++;
			}
		}
		else if (idx == 5)
		{
			if (setLog == TRUE || setLog2 == TRUE && setLog1 == FALSE)
			{
				m_MainDlg->m_logList.InsertItem(ListNum, strDate);
				m_MainDlg->m_logList.SetItemText(ListNum, 1, strRealName);
				m_MainDlg->m_logList.SetItemText(ListNum, 2, strPname);
				m_MainDlg->m_logList.SetItemText(ListNum, 3, strMsg);
				ListNum++;
			}
		}
		else if (idx == 6)
		{
			if (setLog == FALSE && setLog1 == TRUE || setLog2 == TRUE)
			{
				m_MainDlg->m_logList.InsertItem(ListNum, strDate);
				m_MainDlg->m_logList.SetItemText(ListNum, 1, strRealName);
				m_MainDlg->m_logList.SetItemText(ListNum, 2, strPname);
				m_MainDlg->m_logList.SetItemText(ListNum, 3, strMsg);
				ListNum++;
			}
		}
		else if (idx == 7)
		{
			AfxMessageBox(_T("������ üũ ���ּ���"));
		}
	}
	//}

}

void TodayMakeLog(CString msg)
{
	CTime date = CTime::GetCurrentTime();
	CString strFileName;
	int nFileIndex = 0;
	CFile FileName;
	CString log;

	strFileName.Format(_T("c://temp//LogFolder//today.txt"), date.GetYear(), date.GetMonth(), date.GetDay(), ++nFileIndex);
	if (FileName.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		FileName.SeekToEnd();
		log.Format(_T("%s"), msg);
		FileName.Write(log, lstrlen(log) * sizeof(TCHAR));
	}
	FileName.Close();
}

void TodayRead()
{
	CStdioFile tip_file;
	CString str;
	if (tip_file.Open(_T("c://temp//LogFolder//today.txt"), CFile::modeRead | CFile::typeText))
	{
		while (tip_file.ReadString(str))
		{
			AddLogList(str);
		}
		tip_file.Close();
	}
}

void MakeTXTFileDay(CString msg, int nFileIndex)
{
	CTime date = CTime::GetCurrentTime();
	CString log;
	CString strFileName;
	CFile FileName;

	strFileName.Format(_T("c://temp//LogFolder//day//%04d-%02d-%02d.txt"), date.GetYear(), date.GetMonth(), date.GetDay(), ++nFileIndex);
	if (FileName.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		FileName.SeekToEnd();
		log.Format(_T("%s"), msg);
		FileName.Write(log, lstrlen(log) * sizeof(TCHAR));
	}
	FileName.Close();
}

void MakeTXTFileHour(CString msg, int nFileIndex)
{
	CTime date = CTime::GetCurrentTime();
	CString log;
	CString strFileName;
	CFile FileName;

	strFileName.Format(_T("c://temp//LogFolder//hour//%04d-%02d-%02d-%d.txt"), date.GetYear(), date.GetMonth(), date.GetDay(), date.GetHour(), ++nFileIndex);
	if (FileName.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		FileName.SeekToEnd();
		log.Format(_T("%s"), msg);
		FileName.Write(log, lstrlen(log) * sizeof(TCHAR));
	}
	FileName.Close();
}

void MakeEXLSFileDay(CString msg, int nFileIndex)
{
	CTime date = CTime::GetCurrentTime();
	CString log;
	CString strFileName;
	CFile FileName;

	strFileName.Format(_T("c://temp//LogFolder//day//%04d-%02d-%02d.xls"), date.GetYear(), date.GetMonth(), date.GetDay(), ++nFileIndex);
	if (FileName.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		FileName.SeekToEnd();
		log.Format(_T("%s"), msg);
		FileName.Write(log, lstrlen(log) * sizeof(TCHAR));
	}
	FileName.Close();
}

void MakeEXLSFileHour(CString msg, int nFileIndex)
{
	CTime date = CTime::GetCurrentTime();
	CString log;
	CString strFileName;
	CFile FileName;

	strFileName.Format(_T("c://temp//LogFolder//hour//%04d-%02d-%02d-%d.xls"), date.GetYear(), date.GetMonth(), date.GetDay(), date.GetHour(), ++nFileIndex);
	if (FileName.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		FileName.SeekToEnd();
		log.Format(_T("%s"), msg);
		FileName.Write(log, lstrlen(log) * sizeof(TCHAR));
	}
	FileName.Close();
}

//void InsertCSV(CString name, int x, int y, int z)
//{
//	CString strFileName = _T("");
//	CString Temp = _T("");
//
//	Temp.Format(_T("%s, %d, %d, %d\n"), name, x, y, z);
//	strFileName.Format(_T("C:\\Temp\\LogFolder\\Result.csv"));
//
//	FILE* fp;
//	fp = fopen(strFileName, "a+");
//	//fp = fopen(strFileName, CFile::modeReadWrite);
//	fprintf(fp, Temp);
//	fclose(fp);
//	//8fflush(fp);
//	fp = NULL;
//}

void MakeHourLog(CString msg)
{
	//////////////////////////////////////////////////////////////////
	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;
	int idx = m_MainDlg->n_LogSaveSet;
	CString error = _T("[Error]");
	CString warning = _T("[Warning]");
	CString success = _T("[Success]");

	BOOL setLog = FALSE;
	BOOL setLog1 = FALSE; // ����
	BOOL setLog2 = FALSE; // ����
	BOOL setProc = FALSE;

	CString strRealName, strProcName, strTemp, strDate, strCSVname;
	AfxExtractSubString(strDate, msg, 0, '_');
	AfxExtractSubString(strRealName, msg, 1, '_');	//Receive a���� .exe �̸�

	AfxExtractSubString(strCSVname, strRealName, 0, '.'); // .exe ���� �̸�

	int nSaveSet = m_MainDlg->nSaveProg;
	int num = -1;
	if (nSaveSet != 0)
	{
		strProcName = m_MainDlg->strSaveProg;
		AfxExtractSubString(strTemp, strProcName, 0, '.');
		num = strRealName.Find(strTemp);
		if (num != -1)
		{
			setProc = TRUE;
		}
		else
		{
			setProc = FALSE;
		}
	}
	/*
	msg.Find(warning);
	msg.Find(error);
	*/
	BOOL suc = FALSE, war = FALSE, err = FALSE; // csv���


	//////////////////////
	if (msg.Find(success) != -1)
	{
		suc = TRUE;
	}
	//////////////////////
	if (msg.Find(error) != -1)
	{
		setLog1 = TRUE;
		err = TRUE;
	}
	else
	{
		setLog1 = FALSE;
	}
	if (msg.Find(warning) != -1)
	{
		setLog2 = TRUE;
		war = TRUE;
	}
	else
	{
		setLog2 = FALSE;
	}


	if (setLog1 == FALSE && setLog2 == FALSE)
		setLog = TRUE;
	else
		setLog = FALSE;
	//////////////////////////////////////////////////////////////////

	int nFileIndex = 0;

	if (nSaveSet == 0 || setProc == TRUE)
	{
		if (idx == 0)
		{
			MakeTXTFileHour(msg, nFileIndex);
			//InsertCSV(strCSVname, suc, war, err);
			++nFileIndex;
		}
		else if (idx == 1)
		{
			if (setLog == TRUE && setLog1 == FALSE && setLog2 == FALSE)
			{
				MakeTXTFileHour(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 2)
		{
			if (setLog == FALSE && setLog1 == TRUE && setLog2 == FALSE)
			{
				MakeTXTFileHour(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 3)
		{
			if (setLog == FALSE && setLog1 == FALSE && setLog2 == TRUE)
			{
				MakeTXTFileHour(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 4)
		{
			if (setLog == TRUE || setLog1 == TRUE && setLog2 == FALSE)
			{
				MakeTXTFileHour(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 5)
		{
			if (setLog == TRUE || setLog2 == TRUE && setLog1 == FALSE)
			{
				MakeTXTFileHour(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 6)
		{
			if (setLog == FALSE && setLog1 == TRUE || setLog2 == TRUE)
			{
				MakeTXTFileHour(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}

		m_MainDlg->SimpleListSet(strCSVname,suc, war, err);
	}
}

void MakeDayLog(CString msg)
{
	//////////////////////////////////////////////////////////////////
	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;
	int idx = m_MainDlg->n_LogSaveSet;
	CString error = _T("[Error]");
	CString warning = _T("[Warning]");
	CString success = _T("[Success]");

	BOOL setLog = FALSE;
	BOOL setLog1 = FALSE; // ����
	BOOL setLog2 = FALSE; // ����
	BOOL setProc = FALSE;

	CString strRealName, strProcName, strTemp, strDate, strCSVname;
	AfxExtractSubString(strDate, msg, 0, '_');
	AfxExtractSubString(strRealName, msg, 1, '_');	//Receive a���� .exe �̸�

	AfxExtractSubString(strCSVname, strRealName, 0, '.'); // .exe ���� �̸�

	int nSaveSet = m_MainDlg->nSaveProg;
	int num = -1;
	if (nSaveSet != 0)
	{
		strProcName = m_MainDlg->strSaveProg;
		AfxExtractSubString(strTemp, strProcName, 0, '.');
		num = strRealName.Find(strTemp);
		if (num != -1)
		{
			setProc = TRUE;
		}
		else
		{
			setProc = FALSE;
		}
	}

	BOOL suc = FALSE, war = FALSE, err = FALSE; // csv���


												//////////////////////
	if (msg.Find(success) != -1)
	{
		suc = TRUE;
	}
	//////////////////////
	if (msg.Find(error) != -1)
	{
		setLog1 = TRUE;
		err = TRUE;
	}
	else
	{
		setLog1 = FALSE;
	}
	if (msg.Find(warning) != -1)
	{
		setLog2 = TRUE;
		war = TRUE;
	}
	else
	{
		setLog2 = FALSE;
	}


	if (setLog1 == FALSE && setLog2 == FALSE)
		setLog = TRUE;
	else
		setLog = FALSE;
	//////////////////////////////////////////////////////////////////

	int nFileIndex = 0;

	if (nSaveSet == 0 || setProc == TRUE)
	{
		if (idx == 0)
		{
			MakeTXTFileDay(msg, nFileIndex);
			//InsertCSV(strCSVname, suc, war, err);
			++nFileIndex;
		}
		else if (idx == 1)
		{
			if (setLog == TRUE && setLog1 == FALSE && setLog2 == FALSE)
			{
				MakeTXTFileDay(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 2)
		{
			if (setLog == FALSE && setLog1 == TRUE && setLog2 == FALSE)
			{
				MakeTXTFileDay(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 3)
		{
			if (setLog == FALSE && setLog1 == FALSE && setLog2 == TRUE)
			{
				MakeTXTFileDay(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 4)
		{
			if (setLog == TRUE || setLog1 == TRUE && setLog2 == FALSE)
			{
				MakeTXTFileDay(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 5)
		{
			if (setLog == TRUE || setLog2 == TRUE && setLog1 == FALSE)
			{
				MakeTXTFileDay(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 6)
		{
			if (setLog == FALSE && setLog1 == TRUE || setLog2 == TRUE)
			{
				MakeTXTFileDay(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}

		m_MainDlg->SimpleListSet(strCSVname, suc, war, err);
	}
}

void MakeHourExcel(CString msg)
{
	//////////////////////////////////////////////////////////////////
	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;
	int idx = m_MainDlg->n_LogSaveSet;
	CString error = _T("[Error]");
	CString warning = _T("[Warning]");
	CString success = _T("[Success]");

	BOOL setLog = FALSE;
	BOOL setLog1 = FALSE; // ����
	BOOL setLog2 = FALSE; // ����
	BOOL setProc = FALSE;

	CString strRealName, strProcName, strTemp, strDate, strCSVname;
	AfxExtractSubString(strDate, msg, 0, '_');
	AfxExtractSubString(strRealName, msg, 1, '_');	//Receive a���� .exe �̸�

	AfxExtractSubString(strCSVname, strRealName, 0, '.'); // .exe ���� �̸�

	int nSaveSet = m_MainDlg->nSaveProg;
	int num = -1;
	if (nSaveSet != 0)
	{
		strProcName = m_MainDlg->strSaveProg;
		AfxExtractSubString(strTemp, strProcName, 0, '.');
		num = strRealName.Find(strTemp);
		if (num != -1)
		{
			setProc = TRUE;
		}
		else
		{
			setProc = FALSE;
		}
	}
	/*
	msg.Find(warning);
	msg.Find(error);
	*/
	BOOL suc = FALSE, war = FALSE, err = FALSE; // csv���


												//////////////////////
	if (msg.Find(success) != -1)
	{
		suc = TRUE;
	}
	//////////////////////
	if (msg.Find(error) != -1)
	{
		setLog1 = TRUE;
		err = TRUE;
	}
	else
	{
		setLog1 = FALSE;
	}
	if (msg.Find(warning) != -1)
	{
		setLog2 = TRUE;
		war = TRUE;
	}
	else
	{
		setLog2 = FALSE;
	}


	if (setLog1 == FALSE && setLog2 == FALSE)
		setLog = TRUE;
	else
		setLog = FALSE;
	//////////////////////////////////////////////////////////////////

	int nFileIndex = 0;

	if (nSaveSet == 0 || setProc == TRUE)
	{
		if (idx == 0)
		{
			MakeEXLSFileHour(msg, nFileIndex);
			//InsertCSV(strCSVname, suc, war, err);
			++nFileIndex;
		}

		else if (idx == 1)
		{
			if (setLog == TRUE && setLog1 == FALSE && setLog2 == FALSE)
			{
				MakeEXLSFileHour(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 2)
		{
			if (setLog == FALSE && setLog1 == TRUE && setLog2 == FALSE)
			{
				MakeEXLSFileHour(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 3)
		{
			if (setLog == FALSE && setLog1 == FALSE && setLog2 == TRUE)
			{
				MakeEXLSFileHour(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 4)
		{
			if (setLog == TRUE || setLog1 == TRUE && setLog2 == FALSE)
			{
				MakeEXLSFileHour(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 5)
		{
			if (setLog == TRUE || setLog2 == TRUE && setLog1 == FALSE)
			{
				MakeEXLSFileHour(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 6)
		{
			if (setLog == FALSE && setLog1 == TRUE || setLog2 == TRUE)
			{
				MakeEXLSFileHour(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}

		m_MainDlg->SimpleListSet(strCSVname, suc, war, err);
	}

}

void MakeDayExcel(CString msg)
{
	//////////////////////////////////////////////////////////////////
	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;
	int idx = m_MainDlg->n_LogSaveSet;
	CString error = _T("[Error]");
	CString warning = _T("[Warning]");
	CString success = _T("[Success]");

	BOOL setLog = FALSE;
	BOOL setLog1 = FALSE; // ����
	BOOL setLog2 = FALSE; // ����
	BOOL setProc = FALSE;

	CString strRealName, strProcName, strTemp, strDate, strCSVname;
	AfxExtractSubString(strDate, msg, 0, '_');
	AfxExtractSubString(strRealName, msg, 1, '_');	//Receive a���� .exe �̸�

	AfxExtractSubString(strCSVname, strRealName, 0, '.'); // .exe ���� �̸�


	int nSaveSet = m_MainDlg->nSaveProg;
	int num = -1;
	if (nSaveSet != 0)
	{
		strProcName = m_MainDlg->strSaveProg;
		AfxExtractSubString(strTemp, strProcName, 0, '.');
		num = strRealName.Find(strTemp);
		if (num != -1)
		{
			setProc = TRUE;
		}
		else
		{
			setProc = FALSE;
		}
	}
	/*
	msg.Find(warning);
	msg.Find(error);
	*/
	BOOL suc = FALSE, war = FALSE, err = FALSE; // csv���


												//////////////////////
	if (msg.Find(success) != -1)
	{
		suc = TRUE;
	}
	//////////////////////
	if (msg.Find(error) != -1)
	{
		setLog1 = TRUE;
		err = TRUE;
	}
	else
	{
		setLog1 = FALSE;
	}
	if (msg.Find(warning) != -1)
	{
		setLog2 = TRUE;
		war = TRUE;
	}
	else
	{
		setLog2 = FALSE;
	}


	if (setLog1 == FALSE && setLog2 == FALSE)
		setLog = TRUE;
	else
		setLog = FALSE;
	//////////////////////////////////////////////////////////////////

	int nFileIndex = 0;

	if (nSaveSet == 0 || setProc == TRUE)
	{
		if (idx == 0)
		{
			MakeEXLSFileDay(msg, nFileIndex);
			//InsertCSV(strCSVname, suc, war, err);
			++nFileIndex;
		}
		else if (idx == 1)
		{
			if (setLog == TRUE && setLog1 == FALSE && setLog2 == FALSE)
			{
				MakeEXLSFileDay(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 2)
		{
			if (setLog == FALSE && setLog1 == TRUE && setLog2 == FALSE)
			{
				MakeEXLSFileDay(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 3)
		{
			if (setLog == FALSE && setLog1 == FALSE && setLog2 == TRUE)
			{
				MakeEXLSFileDay(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 4)
		{
			if (setLog == TRUE || setLog1 == TRUE && setLog2 == FALSE)
			{
				MakeEXLSFileDay(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 5)
		{
			if (setLog == TRUE || setLog2 == TRUE && setLog1 == FALSE)
			{
				MakeEXLSFileDay(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		else if (idx == 6)
		{
			if (setLog == FALSE && setLog1 == TRUE || setLog2 == TRUE)
			{
				MakeEXLSFileDay(msg, nFileIndex);
				//InsertCSV(strCSVname, suc, war, err);
				++nFileIndex;
			}
		}
		m_MainDlg->SimpleListSet(strCSVname, suc, war, err);
	}
}
