// Server.cpp : implementation file
//

#include<iostream>
#include "stdafx.h"
#include "ini???? program1.h"
#include "ini???? program1Dlg.h"
#include "afxdialogex.h"
#include "Server.h"
#include "ImgSelect.h"
#include"Client.h"
#include "ConnectSocket.h"
#include "ClientSocket.h"
#include "ListenSocket.h"
#include "Log.h"
#include "LogDll.h"
#include <math.h>
#include <sstream>
#include<vector>
#include <algorithm>
#pragma comment (lib, "version.lib")

// CServer dialog

#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;

IMPLEMENT_DYNAMIC(CServer, CDialogEx)

static const int DataRateTimer = 1;
static const int ChartUpdateTimer = 2;
static const int DataInterval = 250;	//?ε????ִ? ?ð?

///////////////////////////////////////////////////////////
CWinThread *m_pImgRead;
ThreadWorkingType m_eThreadWork;


///////////////////////////////////////////////////////////

extern CServer* g_pServer;
CClient* a;
CServer::CServer(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVER, pParent)
{
	g_pServer = this;

	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for (int i = 0; i < sampleSize; ++i)
		m_timeStamps[i] = m_dataSeriesA[i] = m_dataSeriesB[i] = m_dataSeriesC[i] = Chart::NoValue;
	m_nextDataTime = m_currentIndex = 0;
}
CServer::~CServer()
{
	delete m_ChartViewer.getChart();
}

void CServer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_ValueA1);
	DDX_Control(pDX, IDC_STATIC2, m_ValueB1);
	DDX_Control(pDX, IDC_STATIC3, m_ValueC1);
	DDX_Control(pDX, IDC_UpdatePeriod, m_UpdatePeriod1);
	DDX_Control(pDX, IDC_PointerPB, m_PointerPB);
	DDX_Control(pDX, IDC_STATIC5, m_ChartViewer);
	DDX_Control(pDX, IDC_HScrollBar, m_HscrollBar1);
	DDX_Control(pDX, IDC_PointerPB3, m_btnSave);
}


BEGIN_MESSAGE_MAP(CServer, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()

	ON_CONTROL(CVN_ViewPortChanged, IDC_STATIC5, OnViewPortChanged)
	ON_CONTROL(CVN_MouseMovePlotArea, IDC_STATIC5, OnMouseMovePlotArea)
	ON_CBN_SELCHANGE(IDC_UpdatePeriod, &CServer::OnCbnSelchangeUpdateperiod)
	ON_BN_CLICKED(IDC_PointerPB, &CServer::OnBnClickedPointerpb)
	ON_BN_CLICKED(IDC_PointerPB3, &CServer::OnBnClickedPointerpb3)
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CServer message handlers

BOOL CServer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_PointerPB.LoadBitmaps(IDB_BITMAP6, IDB_BITMAP5);
	m_PointerPB.SizeToContent();

	m_btnSave.LoadBitmaps(IDB_BITMAP8, IDB_BITMAP7);
	m_btnSave.SizeToContent();
	// TODO:  Add extra initialization here
	LoadVerTxt();

	// TODO: Add your control notification handler code here
	if (m_ListenSocket.Create(15000, SOCK_STREAM)) { // ???ϻ???
		if (!m_ListenSocket.Listen()) {
			AfxMessageBox(_T("ERROR:Listen() return False"));
		}
	}
	else {
		AfxMessageBox(_T("ERROR:Failed to create server socket!"));
	}

	//?L
	m_Socket.Create();
	if (m_Socket.Connect(LOGADMIN_IP, 21000) == TRUE)
	{
		//???α׷? ??ŸƮ
		CString start = ProgramServer1Start(_T("_[Program1]_[Server]"), _T(" "));
		m_Socket.Send((LPVOID)(LPCTSTR)start, start.GetLength() * 2);
		Sleep(3000);
		//???????? ?α?
		CString temp = ConnectServer(_T("_[Program1]_[Server]"), _T(" "));
		m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);
	}
	else
	{
		AfxMessageBox(_T("ERROR : Failed to connect Server"));
		::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
		::PostQuitMessage(0);
	}
	Sleep(1000);

	//???? ?ð?
	SYSTEMTIME st;
	GetLocalTime(&st);
	m_nextDataTime = Chart::chartTime(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute,
		st.wSecond);

	// ?ý??? ?޴??? "????..." ?޴? ?׸??? ?߰??մϴ?.

	// IDM_ABOUTBOX?? ?ý??? ???? ?????? ?־??? ?մϴ?.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	bReadFile = FALSE;
	// ?? ??ȭ ?????? ???????? ?????մϴ?.  ???? ???α׷??? ?? â?? ??ȭ ???ڰ? ?ƴ? ???쿡??
	//  ?????ӿ?ũ?? ?? ?۾??? ?ڵ????? ?????մϴ?.
	SetIcon(m_hIcon, TRUE);			// ū ???????? ?????մϴ?.
	SetIcon(m_hIcon, FALSE);		// ???? ???????? ?????մϴ?.

	m_PointerPB.SetCheck(1);
	m_ChartViewer.setMouseUsage(Chart::MouseUsageScroll);

	// Enable mouse wheel zooming by setting the zoom ratio to 1.1 per wheel event
	m_ChartViewer.setMouseWheelZoomRatio(1.1);

	// Set up the data acquisition mechanism. In this demo, we just use a timer to get a 
	// sample every 250ms.
	SetTimer(DataRateTimer, DataInterval, 0);

	// The chart update rate initially set to 1000ms
	m_UpdatePeriod1.SelectString(-1, _T("250"));
	OnCbnSelchangeUpdateperiod();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CServer::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ?׸??⸦ ???? ?????̽? ???ؽ?Ʈ?Դϴ?.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ???̾?Ʈ ?簢?????? ???????? ????? ?????ϴ?.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ???????? ?׸??ϴ?.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


void CServer::OnClose()
{
#pragma region ???? ????
	POSITION pos;

	pos = m_ListenSocket.m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;


	while (pos != NULL) {
		pClient = (CClientSocket*)m_ListenSocket.m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL) {
			pClient->ShutDown();
			pClient->Close();

			delete pClient;
		}
	}
	m_ListenSocket.ShutDown();
	m_ListenSocket.Close();
#pragma endregion
	WritePrivateProfileString(_T("Program"), _T("Select"), _T("1"), _T("C:\\Temp\\ProjectSimul.ini"));
	::SendMessage(((Cini????program1Dlg*)GetParent())->GetSafeHwnd(), WM_CLOSE, NULL, NULL);

	CString temp = Exit(_T("_[Program1]_[Server]"), _T(""));
	m_Socket.Send((LPVOID)(LPCTSTR)temp, temp.GetLength() * 2);

	DeleteLog();
	CDialogEx::OnClose();
}

void CServer::LoadVerTxt()
{
	// ???? ?????? ???α׷??? ???θ? ?????? ?????̴?.
	char temp_path[MAX_PATH];

	// ???? ?????? ???α׷??? ???θ? ???´?.
	GetModuleFileName(AfxGetInstanceHandle(), temp_path, sizeof(temp_path));

	// ???? ?????? ???? ???? ?????? ?ڵ鰪?? ?????ϴ? ?????̴?.
	DWORD h_version_handle;
	// ?????????? ?׸??? ?????ڰ? ?߰?/???? ?Ҽ? ?ֱ? ?????? ?????? ũ?Ⱑ ?ƴϴ?.
	// ?????? ???? ???α׷??? ?????????? ???? ũ?⸦ ??? ?? ũ?⿡ ?´? ?޸𸮸? ?Ҵ??ϰ? ?۾??ؾ??Ѵ?.
	DWORD version_info_size = GetFileVersionInfoSize(temp_path, &h_version_handle);

	// ?????????? ?????ϱ? ???? ?ý??? ?޸𸮸? ?????Ѵ?. ( ?ڵ? ???????? ???? )
	HANDLE h_memory = GlobalAlloc(GMEM_MOVEABLE, version_info_size);
	// ?ڵ? ?????? ?޸𸮸? ?????ϱ? ???ؼ? ?ش? ?ڵ鿡 ?????Ҽ? ?ִ? ?ּҸ? ???´?.
	LPVOID p_info_memory = GlobalLock(h_memory);

	// ???? ???α׷??? ???? ?????? ?????´?.
	GetFileVersionInfo(temp_path, h_version_handle, version_info_size, p_info_memory);

	// ???? ?????? ???Ե? ?? ?׸??? ???? ??ġ?? ?????? ?????̴?. ?? ?????Ϳ? ???޵? ?ּҴ?
	// p_info_memory ?? ???? ??ġ?̱? ?????? ?????ϸ? ?ȵ˴ϴ?.
	// ( p_info_memory ?? ?????ϴ? ?????? ?????? ?Դϴ?. )
	char *p_data = NULL;
	// ?????? ???? ?????? ũ?⸦ ?????? ?????̴?.
	UINT data_size = 0;

	CString a, b, c, d, str;
	// ?????׸? ???ÿ? ?????? 041204b0 ?? ?????ڵ??̰? "Korean"?? ?ǹ??մϴ?.
	// ?????????? ???Ե? Comments ?????? ??? ?????մϴ?.
	if (VerQueryValue(p_info_memory, _T("\\StringFileInfo\\041204b0\\FileVersion1-1"), (void **)&p_data, &data_size))
	{
		a = p_data;
	}

	// ?????????? ???Ե? CompanyName ?????? ??? ?????Ѵ?.
	if (VerQueryValue(p_info_memory, _T("\\StringFileInfo\\041204b0\\Comment1-1"), (void **)&p_data, &data_size))
	{
		b = p_data;
	}
	if (VerQueryValue(p_info_memory, _T("\\StringFileInfo\\041204b0\\FileVersion1-2"), (void **)&p_data, &data_size))
	{
		c = p_data;
	}

	// ?????????? ???Ե? CompanyName ?????? ??? ?????Ѵ?.
	if (VerQueryValue(p_info_memory, _T("\\StringFileInfo\\041204b0\\Comment1-2"), (void **)&p_data, &data_size))
	{
		d = p_data;
	}

	str = a + ("\n") + b + ("\n") + c + ("\n") + d;

	CStdioFile m_file;
	// TODO: ???⿡ ??Ʈ?? ?˸? ó???? ?ڵ带 ?߰??մϴ?. 
	BOOL bOpen = m_file.Open(_T("C:\\Temp\\Program1_Version.txt"), CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeReadWrite);
	if (!bOpen)
	{
		AfxMessageBox(_T("Open Error"));
	}

	m_file.WriteString(str);

	m_file.Close();
}

void CServer::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


HCURSOR CServer::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServer::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case DataRateTimer:
		if (bReadFile == TRUE)
		{
			// Is data acquisition timer.
			Readtxt();
			OnDataRateTimer();
			break;
		}
	case ChartUpdateTimer:
		// Is chart update timer
		OnChartUpdateTimer();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CServer::OnCbnSelchangeUpdateperiod()
{
	CString s;
	m_UpdatePeriod1.GetLBText(m_UpdatePeriod1.GetCurSel(), s);
	SetTimer(ChartUpdateTimer, _tcstol(s, 0, 0), 0);
}

//pointer
void CServer::OnBnClickedPointerpb()
{
	m_ChartViewer.setMouseUsage(Chart::MouseUsageScroll);
}

//save as file
void CServer::OnBnClickedPointerpb3()
{
	// Supported formats = PNG, JPG, GIF, BMP, SVG and PDF
	TCHAR szFilters[] = _T("PNG (*.png)|*.png|JPG (*.jpg)|*.jpg|GIF (*.gif)|*.gif|")
		_T("BMP (*.bmp)|*.bmp|SVG (*.svg)|*.svg|PDF (*.pdf)|*.pdf||");

	// The standard CFileDialog
	CFileDialog fileDlg(FALSE, _T("png"), _T("chartdirector_demo"), OFN_HIDEREADONLY |
		OFN_OVERWRITEPROMPT, szFilters);
	if (fileDlg.DoModal() != IDOK)
		return;

	// Save the chart
	CString path = fileDlg.GetPathName();
	BaseChart *c = m_ChartViewer.getChart();
	if (0 != c)
		c->makeChart(TCHARtoUTF8(path));
}



void CServer::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// Update the view port if the scroll bar has moved

	double newViewPortLeft = moveScrollBar(nSBCode, nPos, pScrollBar);
	if (newViewPortLeft != m_ChartViewer.getViewPortLeft())
	{
		m_ChartViewer.setViewPortLeft(newViewPortLeft);
		m_ChartViewer.updateViewPort(true, false);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CServer::OnDestroy()
{
	CDialogEx::OnDestroy();


	KillTimer(1);
}


//
// View port changed event
//
void CServer::OnViewPortChanged()
{
	// In addition to updating the chart, we may also need to update other controls that
	// changes based on the view port.
	updateControls(&m_ChartViewer);

	// Update the chart if necessary
	if (m_ChartViewer.needUpdateChart())
		drawChart(&m_ChartViewer);
}

//
// Draw track cursor when mouse is moving over plotarea
//
void CServer::OnMouseMovePlotArea()
{
	trackLineLabel((XYChart *)m_ChartViewer.getChart(), m_ChartViewer.getPlotAreaMouseX());
	m_ChartViewer.updateDisplay();
}
//parsing.txt?? ?ҷ????? ?Լ?
void CServer::Readtxt()
{
	CStdioFile file;
	CString  str;

	if (file.Open(_T("c://temp//LogFolder//Program1log.txt"), CFile::modeRead | CFile::typeText))
		{
			//vec.clear();
			while (file.ReadString(str))
			{
				CString Date, ProcName, FuncName, strMsg;

				BOOL Same = FALSE;

				AfxExtractSubString(Date, str, 0, '_');
				AfxExtractSubString(ProcName, str, 1, '_');
				AfxExtractSubString(FuncName, str, 2, '_');
				AfxExtractSubString(strMsg, str, 3, '_');

				log.Time = Date;
				log.ProcName = ProcName;
				log.Message = strMsg;

				//???װ? ?߰????? ???????? ?޶??? ???
				for (int i = 0; i < vec.size();i++)
				{
					if ((vec[i].Message == strMsg) && (vec[i].Time == Date))
					{
						Same = TRUE;
						break;
					}
				}
				if (Same == FALSE)
				{
					vec.push_back(log);
				}
			}
		}

	int a = vec.size() - 1;
	vec.erase(vec.begin() + a);

	file.Close();
}


//
// The data acquisition routine. In this demo, this is invoked every 250ms.
//
void CServer::OnDataRateTimer()
{
	// The current time in millisecond resolution
	SYSTEMTIME st;
	GetLocalTime(&st);
	double now = Chart::chartTime(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond)
		//?׷??? ????????????? 10?̸? ???? ?????????? ????
		+ st.wMilliseconds / 1000.0;

	//
	// Use a loop to generate random numbers since the last time this method is called.
	//
	do
	{
		double a1 = 0, b1 = 0, c1 = 0;

		for (int i = 0; i < vec.size(); i++)
		{
			
				CString a, b, c;

				CString message = vec[i].Message;

				CString error1 = _T("[Warning]");
				CString error2 = _T("[Error]");

				int index1 = message.Find(error1);
				int index2 = message.Find(error2);

				if (index1 == 0)
				{
					b1++;
				}
				else if (index2 == 0)
				{
					c1++;
				}
				else
				{
					a1++;
				}
			
		}
		// In this example, we use some formulas to generate new values.
		double p = m_nextDataTime * 4;
		double dataA = a1;//20 + cos(p * 2.2) * 10 + 1 / (cos(p) * cos(p) + 0.01);
		double dataB = b1;//150 + 100 * sin(p / 27.7) * sin(p / 10.1);
		double dataC = c1;//150 + 100 * cos(p / 6.7) * cos(p / 11.9);

						  // If the data arrays are full, we remove the oldest 5% of data.
		if (m_currentIndex >= sampleSize)
		{
			m_currentIndex = sampleSize * 95 / 100 - 1;

			for (int i = 0; i < m_currentIndex; ++i)
			{
				int srcIndex = i + sampleSize - m_currentIndex;
				m_timeStamps[i] = m_timeStamps[srcIndex];
				m_dataSeriesA[i] = m_dataSeriesA[srcIndex];
				m_dataSeriesB[i] = m_dataSeriesB[srcIndex];
				m_dataSeriesC[i] = m_dataSeriesC[srcIndex];
			}
		}

		// Store the new values in the current index position, and increment the index.
		m_timeStamps[m_currentIndex] = m_nextDataTime;
		m_dataSeriesA[m_currentIndex] = dataA;
		m_dataSeriesB[m_currentIndex] = dataB;
		m_dataSeriesC[m_currentIndex] = dataC;
		++m_currentIndex;

		m_nextDataTime += DataInterval / 1000.0;

	} while (m_nextDataTime < now);

	//
	// We provide some visual feedback to the latest numbers generated, so you can see the
	// data being generated.
	//

	//???? ?ϴܿ? ???? ??Ÿ ???? ??
	char buffer[1024];

	sprintf(buffer, " %.2f", m_dataSeriesA[m_currentIndex - 1]);
	m_ValueA1.SetWindowText(CString(buffer));

	sprintf(buffer, " %.2f", m_dataSeriesB[m_currentIndex - 1]);
	m_ValueB1.SetWindowText(CString(buffer));

	sprintf(buffer, " %.2f", m_dataSeriesC[m_currentIndex - 1]);
	m_ValueC1.SetWindowText(CString(buffer));
}

//
// Update the chart and the viewport periodically
//
void CServer::OnChartUpdateTimer()
{
	if (m_currentIndex > 0)
	{
		//
		// As we added more data, we may need to update the full range of the viewport. 
		//

		double startDate = m_timeStamps[0];
		double endDate = m_timeStamps[m_currentIndex - 1];

		// Use the initialFullRange (which is 60 seconds in this demo) if this is sufficient.
		double duration = endDate - startDate;
		if (duration < initialFullRange)
			endDate = startDate + initialFullRange;

		// Update the full range to reflect the actual duration of the data. In this case, 
		// if the view port is viewing the latest data, we will scroll the view port as new
		// data are added. If the view port is viewing historical data, we would keep the 
		// axis scale unchanged to keep the chart stable.
		int updateType = Chart::ScrollWithMax;
		if (m_ChartViewer.getViewPortLeft() + m_ChartViewer.getViewPortWidth() < 0.999)
			updateType = Chart::KeepVisibleRange;
		bool scaleHasChanged = m_ChartViewer.updateFullRangeH("x", startDate, endDate, updateType);

		// Set the zoom in limit as a ratio to the full range
		m_ChartViewer.setZoomInWidthLimit(zoomInLimit / (m_ChartViewer.getValueAtViewPort("x", 1) -
			m_ChartViewer.getValueAtViewPort("x", 0)));

		// Trigger the viewPortChanged event to update the display if the axis scale has changed 
		// or if new data are added to the existing axis scale.
		if (scaleHasChanged || (duration < initialFullRange))
			m_ChartViewer.updateViewPort(true, false);
	}
}

//
// Handle scroll bar events
//
double CServer::moveScrollBar(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//
	// Get current scroll bar position
	//
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;
	pScrollBar->GetScrollInfo(&info);

	//
	// Compute new position based on the type of scroll bar events
	//
	int newPos = info.nPos;
	switch (nSBCode)
	{
	case SB_LEFT:
		newPos = info.nMin;
		break;
	case SB_RIGHT:
		newPos = info.nMax;
		break;
	case SB_LINELEFT:
		newPos -= (info.nPage > 10) ? info.nPage / 10 : 1;
		break;
	case SB_LINERIGHT:
		newPos += (info.nPage > 10) ? info.nPage / 10 : 1;
		break;
	case SB_PAGELEFT:
		newPos -= info.nPage;
		break;
	case SB_PAGERIGHT:
		newPos += info.nPage;
		break;
	case SB_THUMBTRACK:
		newPos = info.nTrackPos;
		break;
	}
	if (newPos < info.nMin) newPos = info.nMin;
	if (newPos > info.nMax) newPos = info.nMax;

	// Update the scroll bar with the new position
	pScrollBar->SetScrollPos(newPos);

	// Returns the position of the scroll bar as a ratio of its total length
	return ((double)(newPos - info.nMin)) / (info.nMax - info.nMin);
}

//
// Update controls when the view port changed
//
void CServer::updateControls(CChartViewer *viewer)
{
	// Update the scroll bar to reflect the view port position and width of the view port.
	m_HscrollBar1.EnableWindow(viewer->getViewPortWidth() < 1);
	if (viewer->getViewPortWidth() < 1)
	{
		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_ALL;
		info.nMin = 0;
		info.nMax = 0x1fffffff;
		info.nPage = (int)ceil(viewer->getViewPortWidth() * (info.nMax - info.nMin));
		info.nPos = (int)(0.5 + viewer->getViewPortLeft() * (info.nMax - info.nMin)) + info.nMin;
		m_HscrollBar1.SetScrollInfo(&info);
	}
}

//
// Draw the chart and display it in the given viewer
//
void CServer::drawChart(CChartViewer *viewer)
{
	// Get the start date and end date that are visible on the chart.
	double viewPortStartDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft());
	double viewPortEndDate = viewer->getValueAtViewPort("x", viewer->getViewPortRight());

	// Extract the part of the data arrays that are visible.
	DoubleArray viewPortTimeStamps;
	DoubleArray viewPortDataSeriesA;
	DoubleArray viewPortDataSeriesB;
	DoubleArray viewPortDataSeriesC;

	if (m_currentIndex > 0)
	{
		// Get the array indexes that corresponds to the visible start and end dates
		int startIndex = (int)floor(Chart::bSearch(DoubleArray(m_timeStamps, m_currentIndex), viewPortStartDate));
		int endIndex = (int)ceil(Chart::bSearch(DoubleArray(m_timeStamps, m_currentIndex), viewPortEndDate));
		int noOfPoints = endIndex - startIndex + 1;

		// Extract the visible data
		viewPortTimeStamps = DoubleArray(m_timeStamps + startIndex, noOfPoints);
		viewPortDataSeriesA = DoubleArray(m_dataSeriesA + startIndex, noOfPoints);
		viewPortDataSeriesB = DoubleArray(m_dataSeriesB + startIndex, noOfPoints);
		viewPortDataSeriesC = DoubleArray(m_dataSeriesC + startIndex, noOfPoints);
	}

	//
	// At this stage, we have extracted the visible data. We can use those data to plot the chart.
	//

	//================================================================================
	// Configure overall chart appearance.
	//================================================================================

	// Create an XYChart object of size 640 x 350 pixels
	XYChart *c = new XYChart(640, 350);

	// Set the plotarea at (55, 50) with width 80 pixels less than chart width, and height 80 pixels
	// less than chart height. Use a vertical gradient from light blue (f0f6ff) to sky blue (a0c0ff)
	// as background. Set border to transparent and grid lines to white (ffffff).
	c->setPlotArea(55, 50, c->getWidth() - 85, c->getHeight() - 80, c->linearGradientColor(0, 50, 0,
		c->getHeight() - 35, 0xf0f6ff, 0xa0c0ff), -1, Chart::Transparent, 0xffffff, 0xffffff);

	// As the data can lie outside the plotarea in a zoomed chart, we need enable clipping.
	c->setClipping();

	// Add a title to the chart using 18pt Arial font
	c->addTitle("Real-time Log graph", "Arial", 18);

	// Add a legend box at (55, 25) using horizontal layout. Use 10pt Arial Bold as font. Set the
	// background and border color to transparent and use line style legend key.
	LegendBox *b = c->addLegend(55, 25, false, "Arial Bold", 10);
	b->setBackground(Chart::Transparent);
	b->setLineStyleKey();

	// Set the x and y axis stems to transparent and the label font to 10pt Arial
	c->xAxis()->setColors(Chart::Transparent);
	c->yAxis()->setColors(Chart::Transparent);
	c->xAxis()->setLabelStyle("Arial", 10);
	c->yAxis()->setLabelStyle("Arial", 10);

	// Set the y-axis tick length to 0 to disable the tick and put the labels closer to the axis.
	c->yAxis()->setTickLength(0);

	// Add axis title using 12pt Arial Bold Italic font
	//c->yAxis()->setTitle("hongju hihihihihihihihi", "Arial Bold", 12);

	//================================================================================
	// Add data to chart
	//================================================================================

	//
	// In this example, we represent the data by lines. You may modify the code below to use other
	// representations (areas, scatter plot, etc).
	//

	// Add a line layer for the lines, using a line width of 2 pixels
	LineLayer *layer = c->addLineLayer();
	layer->setLineWidth(2);
	layer->setFastLineMode();

	// Now we add the 3 data series to a line layer, using the color red (ff0000), green (00cc00)
	// and blue (0000ff)
	layer->setXData(viewPortTimeStamps);
	layer->addDataSet(viewPortDataSeriesA, 0x0000ff, "Info");
	layer->addDataSet(viewPortDataSeriesB, 0xfff000, "Warnning");
	layer->addDataSet(viewPortDataSeriesC, 0xff0000, "Error");

	//================================================================================
	// Configure axis scale and labelling
	//================================================================================

	// Set the x-axis as a date/time axis with the scale according to the view port x range.
	if (m_currentIndex > 0)
		c->xAxis()->setDateScale(viewPortStartDate, viewPortEndDate);

	// For the automatic axis labels, set the minimum spacing to 75/30 pixels for the x/y axis.
	c->xAxis()->setTickDensity(75);
	c->yAxis()->setTickDensity(30);

	//
	// In this example, the axis range can change from a few seconds to thousands of seconds. 
	// We can need to define the axis label format for the various cases. 
	//

	// If all ticks are minute algined, then we use "hh:nn" as the label format.
	c->xAxis()->setFormatCondition("align", 60);
	c->xAxis()->setLabelFormat("{value|hh:nn}");

	// If all other cases, we use "hh:nn:ss" as the label format.
	c->xAxis()->setFormatCondition("else");
	c->xAxis()->setLabelFormat("{value|hh:nn:ss}");

	// We make sure the tick increment must be at least 1 second.
	c->xAxis()->setMinTickInc(1);

	//================================================================================
	// Output the chart
	//================================================================================

	// We need to update the track line too. If the mouse is moving on the chart, the track line
	// will be updated in MouseMovePlotArea. Otherwise, we need to update the track line here.
	if (!viewer->isInMouseMoveEvent())
		trackLineLabel(c, (0 == viewer->getChart()) ? c->getWidth() : viewer->getPlotAreaMouseX());

	// Set the chart image to the WinChartViewer
	delete viewer->getChart();
	viewer->setChart(c);
}

//
// Draw track line with data labels
//
void CServer::trackLineLabel(XYChart *c, int mouseX)
{
	// Obtain the dynamic layer of the chart
	DrawArea *d = c->initDynamicLayer();

	// The plot area object
	PlotArea *plotArea = c->getPlotArea();

	// Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
	double xValue = c->getNearestXValue(mouseX);
	int xCoor = c->getXCoor(xValue);
	if (xCoor < plotArea->getLeftX())
		return;

	// Draw a vertical track line at the x-position
	d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, 0x888888);

	// Draw a label on the x-axis to show the track line position.
	std::ostringstream xlabel;
	xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "hh:nn:ss.ff")
		<< " <*/font*>";
	TTFText *t = d->text(xlabel.str().c_str(), "Arial Bold", 10);

	// Restrict the x-pixel position of the label to make sure it stays inside the chart image.
	int xLabelPos = (std::max)(0, (std::min)(xCoor - t->getWidth() / 2, c->getWidth() - t->getWidth()));
	t->draw(xLabelPos, plotArea->getBottomY() + 6, 0xffffff);
	t->destroy();

	// Iterate through all layers to draw the data labels
	for (int i = 0; i < c->getLayerCount(); ++i) {
		Layer *layer = c->getLayerByZ(i);

		// The data array index of the x-value
		int xIndex = layer->getXIndexOf(xValue);

		// Iterate through all the data sets in the layer
		for (int j = 0; j < layer->getDataSetCount(); ++j)
		{
			DataSet *dataSet = layer->getDataSetByZ(j);
			const char *dataSetName = dataSet->getDataName();

			// Get the color, name and position of the data label
			int color = dataSet->getDataColor();
			int yCoor = c->getYCoor(dataSet->getPosition(xIndex), dataSet->getUseYAxis());

			// Draw a track dot with a label next to it for visible data points in the plot area
			if ((yCoor >= plotArea->getTopY()) && (yCoor <= plotArea->getBottomY()) && (color !=
				Chart::Transparent) && dataSetName && *dataSetName)
			{
				d->circle(xCoor, yCoor, 4, 4, color, color);

				std::ostringstream label;
				label << "<*font,bgColor=" << std::hex << color << "*> "
					<< c->formatValue(dataSet->getValue(xIndex), "{value|P4}") << " <*font*>";
				t = d->text(label.str().c_str(), "Arial Bold", 10);

				// Draw the label on the right side of the dot if the mouse is on the left side the
				// chart, and vice versa. This ensures the label will not go outside the chart image.
				if (xCoor <= (plotArea->getLeftX() + plotArea->getRightX()) / 2)
					t->draw(xCoor + 6, yCoor, 0xffffff, Chart::Left);
				else
					t->draw(xCoor - 6, yCoor, 0xffffff, Chart::Right);

				t->destroy();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// General utilities

//
// Load an icon resource into a button
//
void CServer::loadButtonIcon(int buttonId, int iconId, int width, int height)
{
	// Resize the icon to match the screen DPI for high DPI support
	HDC screen = ::GetDC(0);
	double scaleFactor = GetDeviceCaps(screen, LOGPIXELSX) / 96.0;
	::ReleaseDC(0, screen);
	width = (int)(width * scaleFactor + 0.5);
	height = (int)(height * scaleFactor + 0.5);

	GetDlgItem(buttonId)->SendMessage(BM_SETIMAGE, IMAGE_ICON, (LPARAM)::LoadImage(
		AfxGetResourceHandle(), MAKEINTRESOURCE(iconId), IMAGE_ICON, width, height, LR_DEFAULTCOLOR));
}


HBRUSH CServer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CServer::OnEraseBkgnd(CDC* pDC)
{
	/// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(50, 50, 50));
	return TRUE;
}
