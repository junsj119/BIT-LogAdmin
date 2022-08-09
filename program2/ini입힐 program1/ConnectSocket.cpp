#include "stdafx.h"
#include "ConnectSocket.h"
#include "iniÀÔÈú program1.h"
//#include "LogDll.h"
#include "iniÀÔÈú program1Dlg.h"

extern CServer* g_pServer;
extern CClient* g_pClient;


ConnectSocket::ConnectSocket()
{
		
}


ConnectSocket::~ConnectSocket()
{
}

void ConnectSocket::OnClose(int nErrorCode)
{
	if (g_pServer != NULL)
	{
		g_pServer->m_ListenSocket.Close();
		g_pServer->m_ListenSocket.ShutDown();
		::SendMessage(g_pServer->m_hWnd, WM_CLOSE, NULL, NULL);
		::PostQuitMessage(0);
	}
	if (g_pClient != NULL)
	{
		g_pClient->bServerCloseCheck = TRUE;
		::SendMessage(g_pClient->m_hWnd, WM_CLOSE, NULL, NULL);
		::PostQuitMessage(0);
	}
	if (g_pServer == NULL && g_pClient == NULL)
	{
		::PostQuitMessage(0);
	}
	CSocket::OnClose(nErrorCode);

	AfxMessageBox(_T("ERROR:Disconnected from server!"));
}

void ConnectSocket::OnReceive(int nErrorCode)
{
	TCHAR szBuffer[1024];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	if (Receive(szBuffer, sizeof(szBuffer)) > 0) {
		CiniÀÔÈúprogram1Dlg* pMain = (CiniÀÔÈúprogram1Dlg*)AfxGetMainWnd();

		CString start;
		start.Format(_T("%s"), szBuffer);
	}

	CSocket::OnReceive(nErrorCode);
}