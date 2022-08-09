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
	::ZeroMemory(strBuffer, sizeof(strBuffer)); // :: 붙이고 안붙이고 차이 알아보기

	GetPeerName(strIPAddress, uPortNumber);
	if (Receive(strBuffer, sizeof(strBuffer)) > 0) { // 전달된 데이터(문자열)가 있을 경우
		CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;
		strTmp.Format(_T("[%s:%d]: %s"), strIPAddress, uPortNumber, strBuffer);

		//------------------------------------------------------------------
		// 메시지 처리
		//LogDll Start함수 recv
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
			AddLogList(strBuffer); // 원래 log 였음
		}

		
		//------------------------------------------------------------------
		CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
		//pServerSocket->SendAllMessage(strTempBuffer); // 다른 클라이언트들에게 메시지 전달
		pServerSocket->SendOneMessage(strBuffer, this); //같은 클라이언트들에게 메시지 전달
	}
	CSocket::OnReceive(nErrorCode);
}