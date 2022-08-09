#pragma once

#include "ChartViewer.h"
#include "afxwin.h"
#include "resource.h"
#include<vector>
using namespace std;

// CLogGraph dialog

class CLogGraph : public CDialogEx
{
	DECLARE_DYNAMIC(CLogGraph)

public:
	CLogGraph(CWnd* pParent = NULL);   // standard constructor
	~CLogGraph();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	struct GLog
	{
		CString GTime;
		CString GProcName;
		CString GMessage;
	};

	vector<GLog> Gvec;

	GLog Glog;

	// 구현입니다.
protected:
	HICON m_hIcon;

public:

	CChartViewer m_ChartViewer;
	CStatic m_ValueA1;
	CStatic m_ValueB1;
	CStatic m_ValueC1;
	CComboBox m_UpdatePeriod1;
	CScrollBar m_HScrollBar1;
	CButton m_PointerPB;

	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSelchangeUpdatePeriod();
	afx_msg void OnViewPortChanged();
	afx_msg void OnMouseMovePlotArea();
	afx_msg void OnBnClickedButton1();//pointerPb
	afx_msg void OnBnClickedButton2();//PointerPB2

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClose();
	afx_msg void OnDestroy();



	void OnDataRateTimer();
	void OnChartUpdateTimer();
	void CLogGraph::Readtxt();
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



};
