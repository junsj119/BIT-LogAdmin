#pragma once
#include "ChartViewer.h"
#include "afxwin.h"
#include "resource.h"
#include<vector>
using namespace std;

// CAddGraph dialog

class CAddGraph : public CDialogEx
{
	DECLARE_DYNAMIC(CAddGraph)

public:
	CAddGraph(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddGraph();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
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

	Log log;

protected:
	HICON m_hIcon;


public:
	void CAddGraph::LoadPath(CString path);

	CButton m_PointerPB;
	CComboBox m_UpdatePeriod1;
	CStatic m_ValueA1;
	CStatic m_ValueB1;
	CStatic m_ValueC1;
	CChartViewer m_ChartViewer;
	CScrollBar m_HScrollBar1;
	afx_msg void OnCbnSelchangeUpdateperiod();

	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();

	afx_msg void OnViewPortChanged();
	afx_msg void OnMouseMovePlotArea();

	afx_msg void OnBnClickedPointerpb();
	afx_msg void OnBnClickedPointerpb2();

	void OnDataRateTimer();
	void OnChartUpdateTimer();

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

	void CAddGraph::Readtxt();
};
