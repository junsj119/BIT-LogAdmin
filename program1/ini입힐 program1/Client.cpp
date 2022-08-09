// Client.cpp : implementation file
//

#include "stdafx.h"
#include "ini입힐 program1.h"
#include "ini입힐 program1Dlg.h"
#include "Client.h"
#include "afxdialogex.h"
#include "ImgSelect.h"
#include"Server.h"
#include "LogDll.h"
#include "Log.h"
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;


// CClient dialog
///////////////////////////////////////////////////
CWinThread *m_pImgRead_2;
ThreadWorkingType m_eThreadWork_2;
extern CServer* g_pServer;
extern CClient* g_pClient;
CServer* b;

int nCnt = 0;
CString rPicture;
////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(CClient, CDialogEx)



CClient::CClient(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT, pParent)
{
	g_pClient = this;
}

CClient::~CClient()
{
}

void CClient::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_cPicture);
	DDX_Control(pDX, IDC_STATIC2, m_cTestPicture);
	DDX_Control(pDX, IDC_LIST2, m_cLogList);
	DDX_Control(pDX, IDC_LIST1, m_cPictureList);

	DDX_Control(pDX, IDC_BUTTON2, m_threadBtn);
	DDX_Control(pDX, IDC_BUTTON5, m_btnRestart);
}


BEGIN_MESSAGE_MAP(CClient, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CClient::OnBnClickedButton2)
	ON_WM_CLOSE()
	ON_WM_PAINT()


	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON5, &CClient::OnBnClickedButton5)
END_MESSAGE_MAP()

BOOL CClient::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_btnRestart.ShowWindow(SW_HIDE);

	m_threadBtn.LoadBitmaps(IDB_BITMAP2, IDB_BITMAP1);
	m_threadBtn.SizeToContent();

	m_btnRestart.LoadBitmaps(IDB_BITMAP4, IDB_BITMAP3);
	m_btnRestart.SizeToContent();

	g_pServer->LoadVerTxt();
	// TODO:  Add extra initialization here
	m_Socket_Program.Create();
	m_Socket.Create();

	//server.cpp
	if (m_Socket_Program.Connect(_T(SERVER_IP), 15000) == TRUE)
	{
		CString start = Program1Start(_T("_[Program1]_[OutScratch]"), _T(" "));
		m_Socket_Program.Send((LPVOID)(LPCTSTR)start, start.GetLength() * 2);
		Sleep(2000);

		CString connect = Server1Connect(_T("_[Program1]_[OutScratch]"), _T(" "));
		m_Socket_Program.Send((LPVOID)(LPCTSTR)connect, connect.GetLength() * 2);
	}
	else
	{
		AfxMessageBox(_T("ERROR : Failed to connect Server"));
		//PostQuitMessage(0);
	}
	//logadmin
	if (m_Socket.Connect(_T(LOGADMIN_IP), 21000) == TRUE)
	{
		//프로그램 스타트
		CString start = Program1Start(_T("_[Program1]_[OutScratch]"), _T(" "));
		m_Socket.Send((LPVOID)(LPCTSTR)start, start.GetLength() * 2);
		Sleep(2000);

		CString connect = Server1Connect(_T("_[Program1]_[OutScratch]"), _T(" "));
		m_Socket.Send((LPVOID)(LPCTSTR)connect, connect.GetLength() * 2);
		Sleep(2000);
		//서버연결 로그
		CString temp = ConnectServer(_T("_[Program1]_[OutScratch]"), _T(" "));
		m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
	}
	else
	{
		AfxMessageBox(_T("ERROR : Failed to connect Server"));
		//PostQuitMessage(0);
	}
	bServerCloseCheck = FALSE;
	Sleep(2000);
	LoadBmpList();
	CString temp = ImageLoad(_T("_[Program1]_[OutScratch]"), _T(" "));
	m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);

	if (m_pImgRead_2 == NULL)
	{
		m_pImgRead_2 = AfxBeginThread(ImgReading_2, this);
		CString temp1 = OutScratchStart(_T("_[Program1]_[OutScratch]"), _T(" "));

		m_Socket.Send((LPVOID)(LPCTSTR)temp1, temp1.GetLength() * 2);
		m_Socket_Program.Send((LPVOID)(LPCTSTR)temp1, temp1.GetLength() * 2);

		if (m_pImgRead_2 == NULL)
		{
			CString temp = Warning(_T("_[Program1]_[OutScratch]"), _T(" "));
			m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
			m_Socket_Program.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
		}
		m_pImgRead_2->m_bAutoDelete = FALSE;
		m_eThreadWork_2 = THREAD_RUNNING;
	}

	return TRUE;
}

void CClient::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	FirstArt();
}


//첫 사진 띄우기
void CClient::FirstArt()
{
	CRect rect;
	m_cPicture.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.

	CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_cPicture.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.

	CImage image;//불러오고 싶은 이미지를 로드할 CImage 
	image.Load(_T("C:\\Temp\\Main.bmp"));//이미지 로드

	image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
	ReleaseDC(dc);//DC 해제
}

//다 가져오기
void CClient::LoadBmpList()
{
	CString tPath = _T("C:\\Temp/*.bmp");

	CFileFind finder;

	BOOL bWorking = finder.FindFile(tPath);

	CString fileName;
	CString DirName;

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDirectory() || finder.IsDots())
			continue;

		CString _fileName = finder.GetFileName();

		if (_fileName == _T("Thumbs.db"))	continue;

		fileName = finder.GetFileTitle();
		m_cPictureList.AddString(fileName);
	}
}


//fullNmae이 담겨있는 곳, testPicture 출력
void CClient::SetDirFile(CString fulllName)
{
	CRect rect;
	m_cTestPicture.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.

	CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_cTestPicture.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.

	CImage image;//불러오고 싶은 이미지를 로드할 CImage 
	image.Load(fulllName);//이미지 로드

	image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
	ReleaseDC(dc);//DC 해제
}


//검출
void CClient::detection(CString fulllName, CString picName)
{

	Mat matOriginal, matSample;

	vector<CPoint> pMypoint;
	CString str(fulllName);
	CString str1(picName);
	CT2CA pszConvertedAnsiString(str);
	std::string s(pszConvertedAnsiString);

	matSample = imread("C:\\Temp\\target.bmp", 0);
	matOriginal = imread(s, 0);

	Mat matSrc;									// 이미지 선언
	matSrc = matOriginal.clone();				// Original이미지를 작업 이미지(matSrc)로 복사

	Mat matBin(matSrc.size(), CV_8U);			// grayscale 이미지 저장
	Mat matBinT(matSample.size(), CV_8U);		// grayscale 이미지 저장

	Mat matRgb(matOriginal.size(), CV_8UC3);	// 여러색(BGR순) 이미지 저장
	Mat matRgbT(matSample.size(), CV_8UC3);		// 여러색(BGR순) 이미지 저장
												// 0.2. contours 변수 선언
	vector<vector<Point>> vecStorage;			// Original에서 특정 contours(vecStorage)정보 저장
	vector<vector<Point>> vecSeqT, vecStorageT;	// Sample에서 특정 contour(vecSeqT), contours(vecStorageT)정보 저장
	vector<vector<Point>> vecSeqM;

	double dMatchScore, dMinMatchScore = 1.0;	// matchShape한 점수값 저장
	double matchM = 1000;
	double dPerimT = 0;
	double dTargetAngle = 0;
	double dDstAngle = 0;
	Point pntCenterO, pntCenterT;				// pntCenterO : original에서 찾은 타겟 도형의 중앙 포인트 변수
												// pntCenterT : 전체이미지의 중앙 포인트를 구하기 위한 변수
#pragma endregion

#pragma region 1. 이미지 전처리
												// 1.1. Original, Sample이미지들(matSrc, Sample) matRgb와 matRgbT에 저장
	cvtColor(matSrc, matRgb, CV_GRAY2BGR);
	cvtColor(matSample, matRgbT, CV_GRAY2BGR);

	// 1.2. Edge Detected한 이미지들(matSrc, Sample)을 matBin와 matBinT에 저장
	Canny(matSrc, matBin, 50, 200);
	Canny(matSample, matBinT, 50, 200);
#pragma endregion

#pragma region 2. Original이미지에서 Contours그리기 작업(동일 강도를 가지고 있는 경계선)
	//	- Original 이미지에서 contours 찾기
	findContours(matBin, vecStorage, RETR_LIST, CHAIN_APPROX_TC89_L1); //CHAIN_APPROX_SIMPLE
	for (int i = 0; i < vecStorage.size(); ++i) {
		rectangle(matRgb, boundingRect(vecStorage[i]), Scalar(0, 255, 0), 1);
		drawContours(matRgb, vecStorage, i, Scalar(0, 0, 255), 1, 8, vector<Vec4i>(), 0, Point());

	}

#pragma endregion vecStorage : Original의 contour들의 정보

#pragma region 3. Sample이미지에서 Contour그리기 작업
	//	- Sample이미지에서 contours 찾기
	findContours(matBinT, vecStorageT, RETR_LIST, CHAIN_APPROX_TC89_L1);

	if (!vecStorageT.empty()) {
		// 가장 긴 contours 찾기(= 타겟 도형이 여러개가 되면 안되기 때문이다.)
		for (auto c : vecStorageT) {
			double perim = arcLength(c, true);	// 호의 길이 측정
			if (perim > dPerimT) {				// 가장 긴 contour를 찾는다.
				dPerimT = perim;
				vecSeqT.push_back(c);
			}
		}
	}
	RotatedRect rectT = minAreaRect(vecSeqT[0]);
#pragma endregion vecSeqT : Sample의 contour정보

#pragma region 4. Original도형들의 점수 측정
	if (!vecStorage.empty()) {
		// vecStorage : Original의 도형 contours정보
		// target과 가장 유사한 도형 찾기
		Moments m;
		int cX, cY;
		int i = 0;
		for (auto c : vecStorage) {
			m = moments(c);

			//중심점 좌표 
			cX = int(m.m10 / m.m00);
			cY = int(m.m01 / m.m00);

			// matchShapes : 내부 휴의 7개 불변 모먼트를 계산하여 두 모형 비교. 회전, 크기 변환, 대칭, 이동에 강함
			// 점수가 0에 가까울 수록 같은 도형일 확률이 매우 높다.(즉, 유사 도형이다.)
			dMatchScore = matchShapes(c, vecSeqT[0], CV_CONTOURS_MATCH_I3, 0);
			// vecSeqM의 가장 뒤쪽에 점수가 낮은 도형이 들어간다. 즉, Original에서 Sample과 가장 유사한 도형의 contour정보를 마지막에 저장한다.
			dMatchScore = matchShapes(c, vecSeqT[0], CV_CONTOURS_MATCH_I3, 0);

			if (1 - dMatchScore >= 0.75)
			{

			}
			else
			{
				vecSeqM.push_back(c);
			}
		}
		if (vecSeqM.size() == 0)
		{
			//성공
			CString Success = _T("[Success] Detection Complete");
			m_cLogList.AddString(Success);
			m_cLogList.SetCurSel(m_cLogList.GetCount() - 1);

			CString temp = Success1(_T("_[Program1]_[OutScratch]"), _T(" "));
			m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);

			m_Socket_Program.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
		}
	}
	
	// 가장 유사한 도형은 [초록색]으로 표시된다.
	//int idx = vecSeqM.size() - 1;
	//RotatedRect rectO = minAreaRect(vecSeqM[idx]);
#pragma endregion

#pragma region 5. 중앙값과 회전각 구하기
	// 중앙 포인트 구하기
	
	for (int i = 0; i < vecSeqM.size(); i++)
	{
		Moments m = moments(vecSeqM[i]);
		pntCenterO.x = int(m.m10 / m.m00);
		pntCenterO.y = int(m.m01 / m.m00);

		CString point1, sumLog;
		CString strx, stry;
		CPoint m_pt(pntCenterO.x, pntCenterO.y);
		strx.Format(_T("%d"), m_pt.x);
		stry.Format(_T("%d"), m_pt.y);
		pMypoint.push_back(m_pt);

		// 같은 포인터 출력 방지(같은 좌표를 2번출력하는 것을 방지)
		BOOL bVisit = FALSE;
		for (int j = 0; j < pMypoint.size()-1; j++) {
			if ((m_pt.x == pMypoint[j].x) && (m_pt.y == pMypoint[j].y)) {
				bVisit = TRUE;
				break;
			}
		}
		if(!bVisit){
			//point1.Format(_T(" => X : %d, Y : %d"), m_pt.x, m_pt.y);
			//sumLog = str1 + point1;
			sumLog = _T("[Warning] ") + str1 + _T("=>") + _T("  X : ") + strx + _T("   Y : ") + stry;
			m_cLogList.AddString(sumLog);
			m_cLogList.SetCurSel(m_cLogList.GetCount() - 1);
			CString temp = Coordinates(_T("_[Program1]_[OutScratch]"), _T("_") + sumLog);
			m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);

			m_Socket_Program.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
		}
	}
	vecSeqM.clear();
#pragma endregion

	waitKey();
}
// CClient message handlers



//Stop
void CClient::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
#pragma region 스레드 일시정지
	if (m_pImgRead_2 == NULL)
	{
		AfxMessageBox(_T("생성된 스레드가 없습니다."));
	}
	else if (m_eThreadWork_2 == THREAD_PAUSE)
	{
		m_pImgRead_2->ResumeThread();
		CString temp1 = OutScratchReStart(_T("_[Program1]_[OutScratch]"), _T(" "));
		m_Socket.Send((LPVOID)(LPCTSTR)temp1, temp1.GetLength() * 2);
		m_Socket_Program.Send((LPVOID)(LPCTSTR)temp1, temp1.GetLength() * 2);
		
		m_eThreadWork_2 = THREAD_RUNNING;

		
		
	}
	else
	{
		m_pImgRead_2->SuspendThread();
		m_eThreadWork_2 = THREAD_PAUSE;
		CString temp = OutScratchStop(_T("_[Program1]_[OutScratch]"), _T(""));

		m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
		m_Socket_Program.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
	}
	m_threadBtn.ShowWindow(SW_HIDE);
	m_btnRestart.ShowWindow(SW_SHOWNORMAL);
#pragma endregion
}

void CClient::OnBnClickedButton5()
{
	if (m_pImgRead_2 == NULL)
	{
		AfxMessageBox(_T("생성된 스레드가 없습니다."));
	}
	else if (m_eThreadWork_2 == THREAD_PAUSE)
	{
		m_pImgRead_2->ResumeThread();
		CString temp1 = OutScratchReStart(_T("_[Program1]_[OutScratch]"), _T(" "));
		m_Socket.Send((LPVOID)(LPCTSTR)temp1, temp1.GetLength() * 2);
		m_Socket_Program.Send((LPVOID)(LPCTSTR)temp1, temp1.GetLength() * 2);

		m_eThreadWork_2 = THREAD_RUNNING;
	}
	else
	{
		m_pImgRead_2->SuspendThread();
		m_eThreadWork_2 = THREAD_PAUSE;
		CString temp = OutScratchStop(_T("_[Program1]_[OutScratch]"), _T(""));

		m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
		m_Socket_Program.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
	}

	m_btnRestart.ShowWindow(SW_HIDE);
	m_threadBtn.ShowWindow(SW_SHOWNORMAL);
	
}

//Close
void CClient::OnClose()
{
	// TODO: Add your message handler code here and/or call default
#pragma region 스레드 종료
	if (m_pImgRead_2 == NULL)
	{
		AfxMessageBox(_T("생성된 스레드가 없습니다."));
	}
	else
	{
		m_pImgRead_2->SuspendThread();

		DWORD dwResult;
		::GetExitCodeThread(m_pImgRead_2->m_hThread, &dwResult);

		delete m_pImgRead_2;
		m_pImgRead_2 = NULL;

		m_eThreadWork_2 = THREAD_STOP;

	}
	//프로그램1 서버 끊길시
	if (bServerCloseCheck == TRUE)
	{
		CString disconcet = DisconnectServer1(_T("_[Program1]_[OutScratch]"), _T(""));
		m_Socket.Send((LPVOID)(LPCTSTR)disconcet, disconcet.GetLength() * 2);
	}

	CString temp = Exit(_T("_[Program1]_[OutScratch]"), _T(""));
	m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
	m_Socket_Program.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
#pragma endregion
	WritePrivateProfileString(_T("Program"), _T("Select"), _T("2"), _T("C:\\Temp\\ProjectSimul.ini"));
	::SendMessage(((Cini입힐program1Dlg*)GetParent())->GetSafeHwnd(), WM_CLOSE, NULL, NULL);
	DeleteLog();
	CDialogEx::OnClose();
}


HBRUSH CClient::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(RGB(50, 50, 50));
		return (HBRUSH)GetStockObject(NULL_BRUSH);
		break;

	}

	return hbr;
}


BOOL CClient::OnEraseBkgnd(CDC* pDC)
{
	/// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(50, 50, 50));
	return TRUE;
}



