#include "stdafx.h"
#include "ListenSocket.h"
#include "ClientSocket.h"
#include "ini입힐 program1Dlg.h"
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

	if (Accept(*pClient)) { // 클라이언트 접속 요청이 오면 서버-클라이언트를 연결시켜준다
		pClient->SetListenSocket(this);
		m_ptrClientSocketList.AddTail(pClient); // 리스트에 클라이언트 소켓 저장

		//CTCIServerDlg* pMain = (CTCIServerDlg*)AfxGetMainWnd(); // CSocketServerDlg의 핸들을 가져옴
		str.Format(_T("Client (%d)"), (int)m_ptrClientSocketList.Find(pClient)); // 클라이언트 번호(POSITION(주소) 값)
		//pMain->clientList->AddString(str); // 클라이언트가 접속할때마다 리스트에 클라이언트 이름 추가
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
			// 클라이언트 연결중지후 종료
			pClient->ShutDown();
			pClient->Close();
		}

		//CTCIServerDlg* pMain = (CTCIServerDlg*)AfxGetMainWnd(); //
		CString str1, str2;
		UINT indx = 0, posNum;
		//pMain->clientList->SetCurSel(0); //
		// 접속 종료되는 클라이언트 찾기

		for (; indx < g_pServer->ClientList.size(); indx++)
		{
			posNum = (int)m_ptrClientSocketList.Find(pClient);

			if (g_pServer->ClientList[indx] == posNum)
			{
				g_pServer->ClientList.erase(g_pServer->ClientList.begin() + indx);
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
			// Send함수의 두번째 인자는 메모리의 크기인데 유니코드를 사용하고 있으므로 *2를 한 크기가 된다.
			// 이 함수는 전송한 데이터의 길이를 반환한다.
			int checkLenOfData = pClient->Send(pszMessage, lstrlen(pszMessage) * 2);
			if (checkLenOfData != lstrlen(pszMessage) * 2) {
				AfxMessageBox(_T("일부 데이터가 정상적을 전송되지 못했습니다!"));
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
	// 대상 클라이언트 찾기
	pClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);
	if (pClient != NULL) {
		// Send함수의 두번째 인자는 메모리의 크기인데 유니코드를 사용하고 있으므로 *2를 한 크기가 된다.
		// 이 함수는 전송한 데이터의 길이를 반환한다.
		int checkLenOfData = pClient->Send(pszMessage, lstrlen(pszMessage) * 2);
		if (checkLenOfData != lstrlen(pszMessage) * 2) {
			AfxMessageBox(_T("일부 데이터가 정상적을 전송되지 못했습니다!"));
		}
	}


}