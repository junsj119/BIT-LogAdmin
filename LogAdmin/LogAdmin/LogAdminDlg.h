
// LogAdminDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "ClientSocket.h"
#include "ListenSocket.h"
#include "Process.h"
#include "TotalResult.h"
#include<vector>
#include "afxwin.h"
#include "afxext.h"
using namespace std;


// CLogAdminDlg 대화 상자
class CLogAdminDlg : public CDialogEx
{
// 생성입니다.
public:
	CLogAdminDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	CListenSocket m_ListenSocket;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGADMIN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
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

	CListCtrl m_logList;
	CListCtrl m_watchProc;
	CButton m_cInfo;
	CButton m_cError;
	CButton m_cWarning;
	afx_msg void OnBnClickedButton1();/*
	afx_msg LRESULT ListCtrlView(WPARAM wp, LPARAM lp);*/
	BOOL CopyListToClipboard(CListCtrl* pListCtrl, LPCTSTR lpszSeparator = _T("\t"));
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ListCtrlView(ProcMember p);
	void CLogAdminDlg::ReadTxt();
	void SetLogFile(CString Dir, BOOL bSelect);
	BOOL b_LogFile;
	BOOL b_LiveLog;
	BOOL ProcSetCheck;
	BOOL DelFolderCheck;
	BOOL VersionCheck;
	BOOL LogSaveSet;
	BOOL Alwaysontop;
	BOOL b_LogGraph;
	BOOL b_LoadGraph;

	BOOL b_LogSave;
	BOOL b_LogSaveDay;
	BOOL b_DayLogSave;
	BOOL b_HourLogSave;
	int n_LogSaveSet;
	CString selectprocName;
	int comBoIdx;
	int n_LogViewSet;

	int n_DelDay;
	bool b_Popup;

	//////////////////////
	int nSaveProg;
	CString strSaveProg;
	/////////////////////

	vector<ProcMember> ProcList;
	vector<int> ClientList;

	afx_msg void OnToolAutodel();
	afx_msg void OnViewAlwaysontop();
	afx_msg void OnViewAutoscroll();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	//afx_msg void OnSaveText();
	//afx_msg void OnSaveExcel();
	afx_msg void OnHelpVersion();
	afx_msg void OnCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton2();

	afx_msg void OnOptionSaveset();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnToolLivelog();

	afx_msg void OnToolLoadlog();

	void OnThread();
	afx_msg void OnClose();
	BOOL DeleteDirectoryFile1(CString RootDir);
	CComboBox m_procListComboBox;

	protected:
	afx_msg void OnLvnInsertItenList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnViewGraph();
	afx_msg LRESULT OnTaryNotifyAction(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTraymenuExit();
	afx_msg void OnTraymenuOpendialog();
	afx_msg void OnViewClearlogging();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBitmapButton m_btnDel;
	CBitmapButton m_btnSetting;
	CBitmapButton m_btnApply;
	CBitmapButton m_btnSearch;
	afx_msg void OnViewLoadgraph();
	afx_msg void OnBnClickedButton6();
	CEdit m_editCtrl;
	int splitString(CString str, CString var, CStringArray &strs);
	CBitmapButton m_btnSend;
	CListCtrl m_List_SimepleView;

	void SimpleListNameSet(CString ProcName);
	void SimpleListSet(CString ProcName, BOOL bSuc, BOOL bWar, BOOL bErr);
	void TotalSetItem(BOOL bSuc, BOOL bWar, BOOL bErr);
	void TotalPercent(int num);
	CStatic m_static_Suc;
	CStatic m_static_War;
	CStatic m_static_Err;
};
//void Search();

