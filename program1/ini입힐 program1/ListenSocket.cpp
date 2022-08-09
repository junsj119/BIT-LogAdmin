#include "stdafx.h"
#include "ListenSocket.h"
#include "ClientSocket.h"
#include "ini���� program1Dlg.h"
#include "Server.h"
#include "Client.h"

extern CServer* g_pServer;

CListenSocket::CListenSocket()
{
}


CListenSocket::~CListenSocket()
{
}

void CListenSocket::OnAccept(int nErrorCode)
{
	CClientSocket* pClient = new CClientSocket;
	CString str;

	if (Accept(*pClient)) { // Ŭ���̾�Ʈ ���� ��û�� ���� ����-Ŭ���̾�Ʈ�� ��������ش�
		pClient->SetListenSocket(this);
		m_ptrClientSocketList.AddTail(pClient); // ����Ʈ�� Ŭ���̾�Ʈ ���� ����

		//CTCIServerDlg* pMain = (CTCIServerDlg*)AfxGetMainWnd(); // CSocketServerDlg�� �ڵ��� ������
		str.Format(_T("Client (%d)"), (int)m_ptrClientSocketList.Find(pClient)); // Ŭ���̾�Ʈ ��ȣ(POSITION(�ּ�) ��)
		//pMain->clientList->AddString(str); // Ŭ���̾�Ʈ�� �����Ҷ����� ����Ʈ�� Ŭ���̾�Ʈ �̸� �߰�
		g_pServer->ClientList.push_back((int)m_ptrClientSocketList.Find(pClient));
	}
	else {
		delete pClient;
		AfxMessageBox(_T("ERROR : Failed can't accept new Client!"));
	}

	CAsyncSocket::OnAccept(nErrorCode);
}
void CListenSocket::CloseClientSocket(CSocket* pClient)
{
	POSITION pos;
	pos = m_ptrClientSocketList.Find(pClient);

	if (pos != NULL) {
		if (pClient != NULL) {
			// Ŭ���̾�Ʈ ���������� ����
			pClient->ShutDown();
			pClient->Close();
		}

		//CTCIServerDlg* pMain = (CTCIServerDlg*)AfxGetMainWnd(); //
		CString str1, str2;
		UINT indx = 0, posNum;
		//pMain->clientList->SetCurSel(0); //
		// ���� ����Ǵ� Ŭ���̾�Ʈ ã��

		for (; indx < g_pServer->ClientList.size(); indx++)
		{
			posNum = (int)m_ptrClientSocketList.Find(pClient);

			if (g_pServer->ClientList[indx] == posNum)
			{
				g_pServer->ClientList.erase(g_pServer->ClientList.begin() + indx);
			}
		}
		////while (indx < pMain->clientList->GetCount()) {
		////	posNum = (int)m_ptrClientSocketList.Find(pClient);
		////	pMain->clientList->GetText(indx, str1);
		////	str2.Format(_T("%d"), posNum);
		////	// ���ϸ���Ʈ, Ŭ���̾�Ʈ����Ʈ�� ���ؼ� ���� Ŭ���̾�Ʈ ��ȣ(POSITION ��) ������ ����Ʈ���� ����
		////	if (str1.Find(str2) != -1) {
		////		AfxMessageBox(str1 + str2);
		////		pMain->clientList->DeleteString(indx);
		////		break;
		////	}
		////	indx++;
		////}

		m_ptrClientSocketList.RemoveAt(pos);
		delete pClient;
	}
}
void CListenSocket::SendAllMessage(TCHAR* pszMessage)
{
	POSITION pos;
	pos = m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	while (pos != NULL) {
		pClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL) {
			// Send�Լ��� �ι�° ���ڴ� �޸��� ũ���ε� �����ڵ带 ����ϰ� �����Ƿ� *2�� �� ũ�Ⱑ �ȴ�.
			// �� �Լ��� ������ �������� ���̸� ��ȯ�Ѵ�.
			int checkLenOfData = pClient->Send(pszMessage, lstrlen(pszMessage) * 2);
			if (checkLenOfData != lstrlen(pszMessage) * 2) {
				AfxMessageBox(_T("�Ϻ� �����Ͱ� �������� ���۵��� ���߽��ϴ�!"));
			}
		}
	}

}

void CListenSocket::SendOneMessage(TCHAR* pszMessage, CSocket* pClient)
{
	POSITION pos;
	pos = m_ptrClientSocketList.Find(pClient);

	//CTCIServerDlg* pMain = (CTCIServerDlg*)AfxGetMainWnd();
	CString str1, str2;
	UINT indx = 0, posNum;
	//pMain->clientList->SetCurSel(0);
	// ��� Ŭ���̾�Ʈ ã��
	pClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);
	if (pClient != NULL) {
		// Send�Լ��� �ι�° ���ڴ� �޸��� ũ���ε� �����ڵ带 ����ϰ� �����Ƿ� *2�� �� ũ�Ⱑ �ȴ�.
		// �� �Լ��� ������ �������� ���̸� ��ȯ�Ѵ�.
		int checkLenOfData = pClient->Send(pszMessage, lstrlen(pszMessage) * 2);
		if (checkLenOfData != lstrlen(pszMessage) * 2) {
			AfxMessageBox(_T("�Ϻ� �����Ͱ� �������� ���۵��� ���߽��ϴ�!"));
		}
	}

	//while (indx < pMain->clientList->GetCount()) {
	//	posNum = (int)m_ptrClientSocketList.Find(pClient);
	//	pMain->clientList->GetText(indx, str1);
	//	str2.Format(_T("%d"), posNum);
	//	// �޼��� �����
	//	
	//	}
	//	indx++;
}