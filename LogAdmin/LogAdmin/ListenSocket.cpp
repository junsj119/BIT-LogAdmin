#include "stdafx.h"
#include "ListenSocket.h"
#include "ClientSocket.h"
#include "LogAdmin.h"
#include "LogAdminDlg.h"


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

		CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;
		//////////////CTCIServerDlg* pMain = (CTCIServerDlg*)AfxGetMainWnd(); // CSocketServerDlg�� �ڵ��� ������
		str.Format(_T("Client (%d)"), (int)m_ptrClientSocketList.Find(pClient)); // Ŭ���̾�Ʈ ��ȣ(POSITION(�ּ�) ��)
																				 //////////////pMain->clientList->AddString(str); // Ŭ���̾�Ʈ�� �����Ҷ����� ����Ʈ�� Ŭ���̾�Ʈ �̸� �߰�
		m_MainDlg->ClientList.push_back((int)m_ptrClientSocketList.Find(pClient));

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

		CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;
		UINT Client = 0, posNum;

		for (int i = 0; i < m_MainDlg->ClientList.size(); i++)
		{
			posNum = (int)m_ptrClientSocketList.Find(pClient);
			Client = m_MainDlg->ClientList[i];
			if (posNum == Client)
			{
				m_MainDlg->ClientList.erase(m_MainDlg->ClientList.begin() + i);
				break;
			}
		}

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

	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;
	////////////CTCIServerDlg* pMain = (CTCIServerDlg*)AfxGetMainWnd();
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
}