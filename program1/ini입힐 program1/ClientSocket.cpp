#include "stdafx.h"
#include "ClientSocket.h"
#include "ListenSocket.h"
#include "ini입힐 program1Dlg.h"
#include "Server.h"
#include "Client.h"
#include "Log.h"

extern CServer* g_pServer;

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
		//CTCIServerDlg* pMain = (CTCIServerDlg*)AfxGetMainWnd();
		strTmp.Format(_T("[%s:%d]: %s"), strIPAddress, uPortNumber, strBuffer);

		//로그 생성처리
		//AfxMessageBox((LPCTSTR)strBuffer);
		if (g_pServer->bReadFile == FALSE)
			g_pServer->bReadFile = TRUE;
		CString log;
		log.Format(_T("%s"), strBuffer);
		WriteLog(log);
		//------------------------------------------------------------------
		CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
		//pServerSocket->SendAllMessage(strTempBuffer); // 다른 클라이언트들에게 메시지 전달
		pServerSocket->SendOneMessage(strBuffer, this); //같은 클라이언트들에게 메시지 전달
	}

	CSocket::OnReceive(nErrorCode);
}