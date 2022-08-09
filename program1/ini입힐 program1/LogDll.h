#pragma once
#include "stdafx.h"
extern "C" __declspec (dllimport) CString ConnectServer(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString ProgramServer1Start(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString ProgramServer2Start(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString Server1Connect(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString Server2Connect(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString Program1Start(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString Program2Start(CString ProgramName, CString msg);
extern "C" __declspec (dllimport)CString ImageLoad(CString ProgramName, CString msg);
extern "C" __declspec (dllimport)CString Exit(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString Exit1(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString Warning(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString Coordinates(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString CenteringWarning(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString Detection(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString ImageWarning(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString Coordinates1(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString OutScratchStop(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString CenteringStop(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString Success1(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString Warning1(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString OutScratchStart(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString OutScratchReStart(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString CenteringReStart(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString CenteringStart(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString DisconnectServer1(CString ProgramName, CString msg);
extern "C" __declspec (dllimport) CString DisconnectServer2(CString ProgramName, CString msg);
