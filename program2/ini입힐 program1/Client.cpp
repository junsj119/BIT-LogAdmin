// Client.cpp : implementation file
//

#include "stdafx.h"
#include "ini입힐 program1.h"
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


extern CServer* g_pServer;
// CClient dialog
///////////////////////////////////////////////////
CWinThread *m_pImgRead_2;
ThreadWorkingType m_eThreadWork_2;

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
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Control(pDX, IDC_EDIT3, m_Edit3);
	DDX_Control(pDX, IDC_EDIT4, m_Edit4);
	DDX_Control(pDX, IDC_EDIT5, m_Edit5);
	DDX_Control(pDX, IDC_STATIC3, m_afterPicture);
	DDX_Control(pDX, IDC_BUTTON2, m_threadBtn);
	DDX_Control(pDX, IDC_BUTTON3, m_btnRestart);
}


BEGIN_MESSAGE_MAP(CClient, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CClient::OnBnClickedButton2)
	ON_WM_CLOSE()
	ON_WM_PAINT()

	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON3, &CClient::OnBnClickedButton3)
END_MESSAGE_MAP()

BOOL CClient::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_btnRestart.ShowWindow(SW_HIDE);

	m_threadBtn.LoadBitmaps(IDB_BITMAP4, IDB_BITMAP3);
	m_threadBtn.SizeToContent();

	m_btnRestart.LoadBitmaps(IDB_BITMAP2, IDB_BITMAP1);
	m_btnRestart.SizeToContent();

	g_pServer->LoadVerTxt();
	// TODO:  Add extra initialization here
	m_Socket.Create();
	m_Socket_Program.Create();

	if (m_Socket_Program.Connect(_T(SERVER_IP), 26000) == TRUE)
	{
			
	}
	else
	{
		AfxMessageBox(_T("ERROR : Failed to connect Server"));
	}
	Sleep(2000);
	//logadmin
	if (m_Socket.Connect(LOGADMIN_IP, 21000) == TRUE)
	{
		CString start = Program2Start(_T("_[Program2]_[Centering]"), _T(" "));
		m_Socket.Send((LPVOID)(LPCTSTR)start, start.GetLength() * 2);
		Sleep(2000);

		CString connect = Server2Connect(_T("_[Program2]_[Centering]"), _T(" "));
		m_Socket.Send((LPVOID)(LPCTSTR)connect, connect.GetLength() * 2);
		Sleep(2000);
		//서버연결 로그
		CString temp = ConnectServer(_T("_[Program2]_[Centering]"), _T(" "));
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
	//-- 이미지 로드 로그
	CString temp = ImageLoad(_T("_[Program2]_[Centering]"), _T(" "));
	m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);

#pragma region 스레드 시작
	if (m_pImgRead_2 == NULL)
	{
		m_pImgRead_2 = AfxBeginThread(ImgReading_2, this);
		CString temp1 = CenteringStart(_T("_[Program2]_[Centering]"), _T(" "));
		m_Socket.Send((LPVOID)(LPCTSTR)temp1, temp1.GetLength() * 2);
		m_Socket_Program.Send((LPVOID)(LPCTSTR)temp1, temp1.GetLength() * 2);
		if (m_pImgRead_2 == NULL)
		{
			AfxMessageBox(_T("스레드 생성 에러"));
		}
		m_pImgRead_2->m_bAutoDelete = FALSE;
		m_eThreadWork_2 = THREAD_RUNNING;
	}
#pragma endregion
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
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
	image.Load(_T("C:\\Temp1\\sample1.bmp"));//이미지 로드

	image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
	ReleaseDC(dc);//DC 해제
}

//다 가져오기
void CClient::LoadBmpList()
{
	CString tPath = _T("C:\\Temp1/*.bmp");

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


//ResultPicture 띄우는 함수
void CClient::ResultFile(CString resultName)
{
	CRect rect;
	m_afterPicture.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.

	CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_afterPicture.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.

	CImage image;//불러오고 싶은 이미지를 로드할 CImage 
	image.Load(resultName);//이미지 로드

	image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
	ReleaseDC(dc);//DC 해제
}

//검출코드
void CClient::detection(CString fullname, CString picName)
{
#pragma region 0. 선언
	// 0.1. 이미지 변수 선언 및 저장
	// matSample matching
	Mat matOriginal, matSample;

	CString str(fullname);
	CString str1(picName);
	CT2CA pszConvertedAnsiString(str);
	std::string s(pszConvertedAnsiString);

	matSample = imread("C:\\Temp1\\sample.bmp", 0);
	matOriginal = imread(s, 0);

	Mat matSrc;									// 이미지 선언
	matSrc = matOriginal.clone();				// Original이미지를 작업 이미지(matSrc)로 복사

	Mat matBin(matSrc.size(), CV_8U);			// grayscale 이미지 저장
	Mat matBinT(matSample.size(), CV_8U);		// grayscale 이미지 저장

	Mat matRgb(matOriginal.size(), CV_8UC3);	// 여러색(BGR순) 이미지 저장
	Mat matRgbT(matSample.size(), CV_8UC3);		// 여러색(BGR순) 이미지 저장
	vector<vector<Point>> vecStorage;			// Original에서 특정 contours(vecStorage)정보 저장
	vector<vector<Point>> vecSeqT, vecStorageT;	// Sample에서 특정 contour(vecSeqT), contours(vecStorageT)정보 저장
	vector<vector<Point>> vecSeqM;
	// 0.2. contours 변수 선언
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
	findContours(matBin, vecStorage, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < vecStorage.size(); ++i) {
		rectangle(matRgb, boundingRect(vecStorage[i]), Scalar(0, 255, 0), 1);
		drawContours(matRgb, vecStorage, i, Scalar(0, 0, 255), 1, 8, vector<Vec4i>(), 0, Point());
	}


#pragma endregion vecStorage : Original의 contour들의 정보

#pragma region 3. Sample이미지에서 Contour그리기 작업
	//	- Sample이미지에서 contours 찾기
	findContours(matBinT, vecStorageT, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

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
			if (dMatchScore < dMinMatchScore) {
				dMinMatchScore = dMatchScore;
				vecSeqM.push_back(c);
			}
		}
	}
	// 가장 유사한 도형은 [초록색]으로 표시된다.
	int idx = vecSeqM.size() - 1;
	RotatedRect rectO = minAreaRect(vecSeqM[idx]);

#pragma endregion

#pragma region 5. 중앙값과 회전각 구하기
	// 중앙 포인트 구하기
	Moments m = moments(vecSeqM[idx]);
	pntCenterO.x = int(m.m10 / m.m00);
	pntCenterO.y = int(m.m01 / m.m00);
	dTargetAngle = rectT.angle;
	dDstAngle = rectO.angle;

	//printf("최초각도 : %.2f\n", dDstAngle);
	CString firstAngle;
	firstAngle.Format(_T("%.2f"), dDstAngle);
	m_Edit1.SetWindowText(firstAngle);

	double dDstAngle1 = atof(firstAngle);

	if (dDstAngle1 > 10 && dDstAngle < 80)
		dDstAngle1 = rectO.angle - 45;
	else {
		dDstAngle1 -= dTargetAngle;
	}


#pragma endregion

#pragma region 6. geometric 알고리즘 적용(회전 및 중앙으로 이미지 이동)
	// 6.1. 이미지 회전
	/// Mat getRotationMatrix2D(Point2f center, double angle, double scale)
	/// center – 원본 이미지 회전의 중앙
	/// angle – 회전의 각도(Angle of rotation in degrees) (양수값은 반시계방향, 음수값은 시계 방향)
	/// scale – 이미지의 배율계수(배율계수 1은 원본크기를 의미)
	Mat rot = getRotationMatrix2D(pntCenterO, dDstAngle1, 1.0);
	Mat dst;								// 최종 이미지 선언
	dst = matOriginal.clone();				// Original이미지를 결과 이미지(dst)로 복사
	warpAffine(matOriginal, dst, rot, matOriginal.size());

	// 6.2. 이미지 이동
	pntCenterT = Point(dst.cols / 2, dst.rows / 2);

	Point pRange = pntCenterT - pntCenterO;


	Mat M(2, 3, CV_64F, Scalar(0.0));

	M.at<double>(0, 0) = 1;
	M.at<double>(1, 1) = 1;
	M.at<double>(0, 2) = pRange.x;
	M.at<double>(1, 2) = pRange.y;

	//이동 행렬을 이미지에 적용함.
	warpAffine(dst, dst, M, Size(dst.cols, dst.rows));

	//저장하는 코드
	//
	//rPicture.Format(_T("c://Temp2//result%d.bmp"), nCnt++);
	//CT2CA pszConvertedAnsiString1(rPicture);
	//std::string s1(pszConvertedAnsiString1);
	//imwrite(s1, dst);

	rPicture.Format(_T("c://Temp2//%s_result.bmp"), picName);

	CT2CA pszConvertedAnsiString1(rPicture);
	std::string cvpicName(pszConvertedAnsiString1);

	imwrite(cvpicName, dst);
	ResultFile(rPicture);


#pragma endregion

	//printf("틀어진 각도 : %.2f\n", dDstAngle);
	CString TwistedAngle;
	TwistedAngle.Format(_T("%.2f"), dDstAngle1);
	m_Edit2.SetWindowText(TwistedAngle);

	//printf("<중앙좌표>\nX좌표 : %d\nY좌표 : %d \n", pntCenterO.x, pntCenterO.y);
	CString strx, stry;
	CPoint m_pt(pntCenterO.x, pntCenterO.y);
	strx.Format(_T("%d"), m_pt.x);
	stry.Format(_T("%d"), m_pt.y);
	m_Edit3.SetWindowText(strx);
	m_Edit4.SetWindowText(stry);


	//printf("유사도 점수 : %.2f\n", (1 - dMinMatchScore) * 100);
	CString Similarity;
	Similarity.Format(_T("%.2f"), (1 - dMinMatchScore) * 100);
	double Similarity1 = atof(Similarity);
	CString sumLog;
	m_Edit5.SetWindowText(Similarity);

	if (Similarity1 <= 99.20)
	{
		CString temp = CenteringWarning(_T("_[Program2]_[Centering]"), _T(" "));
		sumLog = _T("[Warning] DON'T FIND SIMILAR A PATTERN.");
		m_cLogList.AddString(sumLog);
		m_cLogList.SetCurSel(m_cLogList.GetCount() - 1);
		m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
		m_Socket_Program.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
	}
	else
	 {
	 sumLog = str1 + _T("=>") + _T("  FirstAngle : ") + firstAngle + _T("   TwistedAngle : ") + TwistedAngle + _T("   X : ") + strx + _T("   Y : ") + stry + _T("   Similarity : ") + Similarity;
	 m_cLogList.AddString(sumLog);
	 m_cLogList.SetCurSel(m_cLogList.GetCount() - 1);
	 CString temp = Coordinates(_T("_[Program2]_[Centering]"), _T("_[Success]") + sumLog);
	 m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
	 m_Socket_Program.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
	 }
}


// CClient message handlers

//Stop
void CClient::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
#pragma region 스레드 일시정지 및 재시작
	if (m_pImgRead_2 == NULL)
	{
		AfxMessageBox(_T("생성된 스레드가 없습니다."));
	}
	else if (m_eThreadWork_2 == THREAD_PAUSE)
	{
		CString temp1 = CenteringReStart(_T("_[Program2]_[Centering]"), _T(" "));
		m_Socket.Send((LPVOID)(LPCTSTR)temp1, temp1.GetLength() * 2);
		m_Socket_Program.Send((LPVOID)(LPCTSTR)temp1, temp1.GetLength() * 2);
		m_pImgRead_2->ResumeThread();
		m_eThreadWork_2 = THREAD_RUNNING;
		m_threadBtn.SetWindowTextA("Stop");
	}
	else
	{
		m_pImgRead_2->SuspendThread();
		m_eThreadWork_2 = THREAD_PAUSE;
		CString temp = CenteringStop(_T("_[Program2]_[Centering]"), _T(""));
		m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
		m_Socket_Program.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
		m_threadBtn.SetWindowTextA("Restart");
	}
	m_threadBtn.ShowWindow(SW_HIDE);
	m_btnRestart.ShowWindow(SW_SHOWNORMAL);
#pragma endregion
}

//Restart
void CClient::OnBnClickedButton3()
{
	if (m_pImgRead_2 == NULL)
	{
		AfxMessageBox(_T("생성된 스레드가 없습니다."));
	}
	else if (m_eThreadWork_2 == THREAD_PAUSE)
	{
		CString temp1 = CenteringReStart(_T("_[Program2]_[Centering]"), _T(" "));
		m_Socket.Send((LPVOID)(LPCTSTR)temp1, temp1.GetLength() * 2);
		m_Socket_Program.Send((LPVOID)(LPCTSTR)temp1, temp1.GetLength() * 2);
		m_pImgRead_2->ResumeThread();
		m_eThreadWork_2 = THREAD_RUNNING;
		m_threadBtn.SetWindowTextA("Stop");
	}
	else
	{
		m_pImgRead_2->SuspendThread();
		m_eThreadWork_2 = THREAD_PAUSE;
		CString temp = CenteringStop(_T("_[Program2]_[Centering]"), _T(""));
		m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
		m_Socket_Program.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
		m_threadBtn.SetWindowTextA("Restart");
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
	//프로그램2 서버 끊길시
	if (bServerCloseCheck == TRUE)
	{

		CString disconcet = DisconnectServer2(_T("_[Program2]_[Centering]"), _T(""));
		m_Socket.Send((LPVOID)(LPCTSTR)disconcet, disconcet.GetLength() * 2);
	}

	CString temp = Exit(_T("_[Program2]_[Centering]"), _T(""));
	m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
	m_Socket_Program.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);

	WritePrivateProfileString(_T("Program"), _T("Select"), _T("2"), _T("C:\\Temp\\ProjectSimul2.ini"));
	::SendMessage(((Cini입힐program1Dlg*)GetParent())->GetSafeHwnd(), WM_CLOSE, NULL, NULL);
	DeleteLog();
#pragma endregion
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



