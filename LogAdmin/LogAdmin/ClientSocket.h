#pragma once
#include <afxsock.h>

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	~CClientSocket();

	CAsyncSocket *m_pListenSocket;
	void SetListenSocket(CAsyncSocket* pSocket);
	void OnClose(int nErrorCode);
	void OnReceive(int nErrorCode);
};

