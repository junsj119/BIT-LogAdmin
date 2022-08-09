#pragma once
#include "ChartViewer.h"
#include "afxwin.h"
#include "ListenSocket.h"
#include "ConnectSocket.h"
#include<vector>
#include "afxext.h"
using namespace std;

// CServer dialog

class CServer : public CDialogEx
{
	DECLARE_DYNAMIC(CServer)

public:
	CServer(CWnd* pParent = NULL);   // standard constructor
	~CServer();
	ConnectSocket m_Socket;
	CListenSocket m_ListenSocket;
	BOOL bServerCloseCheck;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	struct Log
	{
		CString Time;
		CString ProcName;
		CString Message;
	};

	vector<Log> vec;

	BOOL bReadFile;
	
	Log log;

protected:
	HICON m_hIcon;

public:
	CStatic m_ValueA1;
	CStatic m_ValueB1;
	CStatic m_ValueC1;
	CComboBox m_UpdatePeriod1;
	CBitmapButton m_PointerPB;
	CBitmapButton m_btnSave;
	CChartViewer m_ChartViewer;
	CScrollBar m_HscrollBar1;

	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeUpdateperiod();
	afx_msg void OnBnClickedPointerpb();
	afx_msg void OnBnClickedPointerpb3();
	afx_msg void OnViewPortChanged();
	afx_msg void OnMouseMovePlotArea();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void OnDataRateTimer();
	void OnChartUpdateTimer();
	void CServer::Readtxt();
	// The number of samples per data series used in this demo
	static const int sampleSize = 10000;

	// The initial full range is set to 60 seconds of data.
	static const int initialFullRange = 60;

	// The maximum zoom in is 10 seconds.
	static const int zoomInLimit = 10;

	double m_timeStamps[sampleSize];	// The timestamps for the data series
	double m_dataSeriesA[sampleSize];	// The values for the data series A
	double m_dataSeriesB[sampleSize];	// The values for the data series B
	double m_dataSeriesC[sampleSize];	// The values for the data series C

										// The index of the array position to which new data values are added.
	int m_currentIndex;

	// Used by the random number generator to generate real time data.
	double m_nextDataTime;

	// Draw chart
	void drawChart(CChartViewer *viewer);
	void trackLineLabel(XYChart *c, int mouseX);

	// Update controls when the view port changed
	void updateControls(CChartViewer *viewer);
	// Moves the scroll bar when the user clicks on it
	double moveScrollBar(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// utility to load icon resource to a button
	void loadButtonIcon(int buttonId, int iconId, int width, int height);



	afx_msg void OnClose();

	void CServer::LoadVerTxt();
	vector<int> ClientList;

	afx_msg void OnDestroy();
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
