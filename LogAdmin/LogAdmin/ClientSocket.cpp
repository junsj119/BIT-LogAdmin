#include "stdafx.h"
#include "ClientSocket.h"
#include "ListenSocket.h"
#include "LogAdmin.h"
#include "LogAdminDlg.h"
#include "Log.h"


CClientSocket::CClientSocket()
{
}


CClientSocket::~CClientSocket()
{
}

void CClientSocket::SetListenSocket(CAsyncSocket* pSocket)
{
	m_pListenSocket = pSocket;
}
void CClientSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);
}
void CClientSocket::OnReceive(int nErrorCode)
{
	CString strTmp = _T(""), strIPAddress = _T("");
	UINT uPortNumber = 0;
	TCHAR strBuffer[1024];
	::ZeroMemory(strBuffer, sizeof(strBuffer)); // :: ���̰� �Ⱥ��̰� ���� �˾ƺ���

	GetPeerName(strIPAddress, uPortNumber);
	if (Receive(strBuffer, sizeof(strBuffer)) > 0) { // ���޵� ������(���ڿ�)�� ���� ���
		CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;
		strTmp.Format(_T("[%s:%d]: %s"), strIPAddress, uPortNumber, strBuffer);

		//------------------------------------------------------------------
		// �޽��� ó��
		//LogDll Start�Լ� recv
		CString TodayLog;
		TodayLog.Format(_T("%s"), strBuffer);
		TodayMakeLog(TodayLog);

		if (m_MainDlg->b_LogSave == TRUE)
		{
			if (m_MainDlg->b_LogSaveDay == TRUE)
			{
				CString Daylog;
				Daylog.Format(_T("%s"), strBuffer);
				MakeDayLog(Daylog);
			}
			else
			{
				CString Hourlog;
				Hourlog.Format(_T("%s"), strBuffer);
				MakeHourLog(Hourlog);
			}
			
		}
		else
		{

			if (m_MainDlg->b_LogSaveDay == TRUE)
			{
				CString Daylog;
				Daylog.Format(_T("%s"), strBuffer);
				MakeDayExcel(Daylog);
			}
			else
			{
				CString Hourlog;
				Hourlog.Format(_T("%s"), strBuffer);
				MakeHourExcel(Hourlog);
			}
		}

		if (m_MainDlg->b_LiveLog == TRUE)
		{
			AddLogList(strBuffer); // ���� log ����
		}

		
		//------------------------------------------------------------------
		CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
		//pServerSocket->SendAllMessage(strTempBuffer); // �ٸ� Ŭ���̾�Ʈ�鿡�� �޽��� ����
		pServerSocket->SendOneMessage(strBuffer, this); //���� Ŭ���̾�Ʈ�鿡�� �޽��� ����
	}
	CSocket::OnReceive(nErrorCode);
}