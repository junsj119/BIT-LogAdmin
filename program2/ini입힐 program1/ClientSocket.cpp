#include "stdafx.h"
#include "ClientSocket.h"
#include "ListenSocket.h"
#include "ini���� program1Dlg.h"
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
	::ZeroMemory(strBuffer, sizeof(strBuffer)); // :: ���̰� �Ⱥ��̰� ���� �˾ƺ���

	GetPeerName(strIPAddress, uPortNumber);
	if (Receive(strBuffer, sizeof(strBuffer)) > 0) { // ���޵� ������(���ڿ�)�� ���� ���
		//CTCIServerDlg* pMain = (CTCIServerDlg*)AfxGetMainWnd();
		strTmp.Format(_T("[%s:%d]: %s"), strIPAddress, uPortNumber, strBuffer);
		//pMain->m_msgList.AddString(strTmp);  // �޽��� ����Ʈ(�޽���â?)�� �Է¹��� �޽��� ����
		//pMain->m_msgList.SetCurSel(pMain->m_msgList.GetCount() - 1);

		//------------------------------------------------------------------
		if (g_pServer->bReadFile == FALSE)
			g_pServer->bReadFile = TRUE;
		CString program2;
		program2.Format(_T("%s"), strBuffer);
		WriteLog(program2);	

		CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
		//pServerSocket->SendAllMessage(strTempBuffer); // �ٸ� Ŭ���̾�Ʈ�鿡�� �޽��� ����
		pServerSocket->SendOneMessage(strBuffer, this); //���� Ŭ���̾�Ʈ�鿡�� �޽��� ����
	}

	CSocket::OnReceive(nErrorCode);
}