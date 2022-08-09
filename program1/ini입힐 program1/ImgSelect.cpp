#include "stdafx.h"
#include "ini입힐 program1.h"
#include "ini입힐 program1Dlg.h"
#include"Server.h"
#include"Client.h"
#include <Windows.h>
#include<time.h>

extern CServer* g_pServer;
extern CClient* g_pClient;

UINT ImgReading_2(LPVOID lpParam)
{
	int nCount = g_pClient->m_cPictureList.GetCount();

	CString picName;
	CString basicPath = _T("C:\\Temp\\");
	CString extension = _T(".bmp");
	CString fullName, fileName;
	int ran = 0;

	while (1)
	{
		srand((unsigned)time(NULL));
		ran = rand() % nCount; // 14개의 사진이 있으면 0~13
		g_pClient->m_cPictureList.GetText(ran, picName);

		fullName = basicPath + picName + extension;
		//fileName = picName + extension;

		g_pClient->SetDirFile(fullName);
		g_pClient->detection(fullName, picName);

		Sleep(500);
	}
	return 0;
}