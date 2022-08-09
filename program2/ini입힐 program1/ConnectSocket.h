#pragma once
class ConnectSocket : public CSocket
{
public:
	ConnectSocket();
	virtual ~ConnectSocket();

public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};

