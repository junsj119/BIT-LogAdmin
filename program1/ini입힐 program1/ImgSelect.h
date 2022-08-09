#pragma once
#include "stdafx.h"
#include "ini¿‘»˙ program1Dlg.h"

enum  ThreadWorkingType
{
	THREAD_STOP = 0,
	THREAD_RUNNING = 1,
	THREAD_PAUSE = 2
};

UINT ImgReading_1(LPVOID lpParam);
UINT ImgReading_2(LPVOID lpParam);