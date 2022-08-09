
// LogAdminDlg.cpp : 구현 파일
//
#include "stdafx.h"
#include "LogAdmin.h"
#include "LogAdminDlg.h"
#include "afxdialogex.h"
#include "Log.h"

#include "ClientSocket.h"
#include "ListenSocket.h"

#include "ProcSetting.h"
#include "DeleteFile.h"
#include "LogFileChoose.h"
#include "LogGraph.h"
#include "AddGraph.h"
#include "Tool.h"
#include"XLEzAutomation.h"

#include "Version.h"
#include "LogSaveSet.h"

#include "AutoDel.h"
#include "TotalResult.h"

#include <vector>
#include <algorithm>

#include "CSmtp.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//ProcSetting* m_proSet = (ProcSetting*)AfxGetApp();
CLogAdminDlg * wd;
ProcSetting *ab;
BOOL BthrCheck = false;
//////////////// 로그 파일
BOOL bFileSele = FALSE;
CString strDir = _T("");
BOOL bCheck = FALSE;
////////////////////////////

CMenu *hMenu;

CWinThread *m_pLogDel;
ThreadWorkingType m_eThreadWork;

static const int DataRateTimer = 1;
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();



	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };

#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLogAdminDlg 대화 상자



CLogAdminDlg::CLogAdminDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGADMIN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	wd = this;
}

void CLogAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_logList);
	DDX_Control(pDX, IDC_LIST2, m_watchProc);
	DDX_Control(pDX, IDC_CHECK1, m_cInfo);
	DDX_Control(pDX, IDC_CHECK2, m_cError);
	DDX_Control(pDX, IDC_CHECK3, m_cWarning);

	DDX_Control(pDX, IDC_COMBO1, m_procListComboBox);
	DDX_Control(pDX, IDC_BUTTON2, m_btnDel);
	DDX_Control(pDX, IDC_BUTTON1, m_btnSetting);
	DDX_Control(pDX, IDC_BUTTON4, m_btnApply);
	DDX_Control(pDX, IDC_BUTTON3, m_btnSearch);
	DDX_Control(pDX, IDC_EDIT3, m_editCtrl);
	DDX_Control(pDX, IDC_BUTTON6, m_btnSend);
	DDX_Control(pDX, IDC_LIST3, m_List_SimepleView);
	DDX_Control(pDX, IDC_STATIC_SUC, m_static_Suc);
	DDX_Control(pDX, IDC_STATIC_WAR, m_static_War);
	DDX_Control(pDX, IDC_STATIC_ERR, m_static_Err);
}

BEGIN_MESSAGE_MAP(CLogAdminDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CLogAdminDlg::OnBnClickedButton1)

	//ON_MESSAGE(WM_USER_MSG, &CLogAdminDlg::ListCtrlView)

	ON_COMMAND(ID_TOOL_AUTODEL, &CLogAdminDlg::OnToolAutodel)
	ON_COMMAND(ID_VIEW_ALWAYSONTOP, &CLogAdminDlg::OnViewAlwaysontop)
	ON_COMMAND(ID_VIEW_AUTOSCROLL, &CLogAdminDlg::OnViewAutoscroll)

	ON_BN_CLICKED(IDC_BUTTON3, &CLogAdminDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CLogAdminDlg::OnBnClickedButton4)
	//ON_COMMAND(ID_SAVE_TEXT, &CLogAdminDlg::OnSaveText)
	//ON_COMMAND(ID_SAVE_EXCEL, &CLogAdminDlg::OnSaveExcel)
	ON_COMMAND(ID_HELP_VERSION, &CLogAdminDlg::OnHelpVersion)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CLogAdminDlg::OnCustomdrawList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLogAdminDlg::OnBnClickedButton2)
	ON_COMMAND(ID_OPTION_SaveSet, &CLogAdminDlg::OnOptionSaveset)
	ON_WM_TIMER()
	ON_COMMAND(ID_TOOL_LIVELOG, &CLogAdminDlg::OnToolLivelog)
	ON_COMMAND(ID_TOOL_LOADLOG, &CLogAdminDlg::OnToolLoadlog)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_INSERTITEM, IDC_LIST1, &CLogAdminDlg::OnLvnInsertItenList1)
	ON_COMMAND(ID_VIEW_GRAPH, &CLogAdminDlg::OnViewGraph)
	ON_MESSAGE(WM_TRAY_NOTIFYICACTOIN, OnTaryNotifyAction)
	ON_COMMAND(ID_TRAYMENU_EXIT, &CLogAdminDlg::OnTraymenuExit)
	ON_COMMAND(ID_TRAYMENU_OPENDIALOG, &CLogAdminDlg::OnTraymenuOpendialog)
	ON_COMMAND(ID_VIEW_CLEARLOGGING, &CLogAdminDlg::OnViewClearlogging)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_VIEW_LOADGRAPH, &CLogAdminDlg::OnViewLoadgraph)
	ON_BN_CLICKED(IDC_BUTTON6, &CLogAdminDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CLogAdminDlg 메시지 처리기

BOOL CLogAdminDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.
	m_btnDel.LoadBitmaps(IDB_BITMAP4, IDB_BITMAP3);
	m_btnDel.SizeToContent();

	m_btnSetting.LoadBitmaps(IDB_BITMAP10, IDB_BITMAP9);
	m_btnSetting.SizeToContent();

	m_btnApply.LoadBitmaps(IDB_BITMAP2, IDB_BITMAP1);
	m_btnApply.SizeToContent();

	m_btnSearch.LoadBitmaps(IDB_BITMAP7, IDB_BITMAP8);
	m_btnSearch.SizeToContent();

	m_btnSend.LoadBitmaps(IDB_BITMAP25, IDB_BITMAP26);
	m_btnSend.SizeToContent();

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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
	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.


	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//ComboBox 초기화 작업
	m_procListComboBox.AddString(_T("All Application"));
	m_procListComboBox.SetCurSel(0);


	//SetTimer(DataRateTimer, 2000, 0);

	CRect rect;
	m_logList.GetClientRect(&rect);
	m_cInfo.SetCheck(1);
	m_cError.SetCheck(1);
	m_cWarning.SetCheck(1);

	//리스트 컨트롤 컬럼 추가
	m_logList.InsertColumn(0, _T("Time"), LVCFMT_LEFT, 150);
	m_logList.InsertColumn(1, _T("ProcName"), LVCFMT_LEFT, 100);
	m_logList.InsertColumn(2, _T("FuncName"), LVCFMT_LEFT, 100);
	m_logList.InsertColumn(3, _T("Message"), LVCFMT_LEFT, 650);

	int nCount = m_logList.GetItemCount();

	///////////////////////////////Txt 파일파싱 후 listcontrol에 저장 ///////////////////////////////

	DWORD dwStyle = m_logList.GetExtendedStyle();

	m_logList.SetExtendedStyle(dwStyle
		| LVS_EX_FULLROWSELECT	//뭘 눌러도 선택이 되게
		| LVS_EX_GRIDLINES)		//그리드 라인
		| LVS_EX_CHECKBOXES;	//| LVS_EX_CHECKBOXES);   //체크박스	


	CRect rect2;

	m_watchProc.GetClientRect(&rect2);
	//리스트 컨트롤 컬럼 추가
	m_watchProc.InsertColumn(0, _T("Program"), LVCFMT_LEFT, 140);
	m_watchProc.InsertColumn(1, _T("RbtCnt"), LVCFMT_LEFT, 50);
	m_watchProc.InsertColumn(2, _T("Check Mdoe"), LVCFMT_LEFT, 135);

	DWORD dwStyle2 = m_watchProc.GetExtendedStyle();

	m_watchProc.SetExtendedStyle(dwStyle2
		| LVS_EX_FULLROWSELECT	//뭘 눌러도 선택이 되게
		| LVS_EX_GRIDLINES 		//그리드 라인
		| LVS_EX_CHECKBOXES);   //체크박스

	//////////////////////---Simple View---/////////////////////////////
	CRect rect3;

	m_List_SimepleView.GetClientRect(&rect3);
	//리스트 컨트롤 컬럼 추가
	m_List_SimepleView.InsertColumn(0, _T("Program"), LVCFMT_LEFT, 90);
	m_List_SimepleView.InsertColumn(1, _T("Success"), LVCFMT_LEFT, 60);
	m_List_SimepleView.InsertColumn(2, _T("Warning"), LVCFMT_LEFT, 60);
	m_List_SimepleView.InsertColumn(3, _T("Error"), LVCFMT_LEFT, 60);

	DWORD dwStyle3 = m_List_SimepleView.GetExtendedStyle();

	m_List_SimepleView.SetExtendedStyle(dwStyle3
		| LVS_EX_FULLROWSELECT	//뭘 눌러도 선택이 되게
		| LVS_EX_GRIDLINES); 		//그리드 라인
	///////////////////////////////////////////////////

	b_LiveLog = TRUE;
	b_LogFile = FALSE;
	ProcSetCheck = FALSE;
	DelFolderCheck = FALSE;
	VersionCheck = FALSE;
	LogSaveSet = FALSE;
	b_LogSave = TRUE;
	b_LogSaveDay = TRUE;
	b_LogGraph = FALSE;
	b_LoadGraph = FALSE;

	n_LogViewSet = 0;
	n_LogSaveSet = 0;
	hMenu = GetMenu();
	n_DelDay = 30;

	nSaveProg = 0;


	OnThread();
	// TODO: Add your control notification handler code here

	if (m_ListenSocket.Create(21000, SOCK_STREAM)) { // 소켓생성
		if (!m_ListenSocket.Listen()) {
			AfxMessageBox(_T("ERROR:Listen() return False"));
		}
	}
	else
	{
		AfxMessageBox(_T("ERROR:Failed to create server socket!"));
	}


	return TRUE;
}



void CLogAdminDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == SC_MINIMIZE)
	{
		TrayIconMake();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CLogAdminDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CLogAdminDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

///////////////////////////////////////////////////////


ProcSetting *m_ProcSet;

//자동실행세팅 m_wathchProc
void CLogAdminDlg::OnBnClickedButton1()
{
	if (ProcSetCheck == FALSE)
	{
		m_ProcSet = new ProcSetting(this);

		m_ProcSet->Create(IDD_DIALOG1, this);
		m_ProcSet->ShowWindow(SW_SHOW);
		ProcSetCheck = TRUE;
	}
}

void CLogAdminDlg::ListCtrlView(ProcMember p)
{
	BOOL SameItemCheck = FALSE;

	for (int i = 0; i < ProcList.size(); i++)	// 이름 중복 검사
	{
		if (ProcList[i].procName == p.procName)
		{
			SameItemCheck = TRUE;
			break;
		}
	}

	if (SameItemCheck == FALSE)	// 중복이 없으면 vector에 저장
		ProcList.push_back(p);

	m_watchProc.DeleteAllItems();		// 리스트 컨트롤 출력

	for (int i = 0; i < ProcList.size(); i++)
	{
		m_watchProc.InsertItem(i, ProcList[i].procName);
		m_watchProc.SetItemText(i, 1, ProcList[i].rbtCnt);
		m_watchProc.SetItemText(i, 2, ProcList[i].procMode);
	}

	DWORD ThreadID;
	if (BthrCheck == false)		// 스레드 생성
	{
		CloseHandle(CreateThread(0, 0, ThreadProc, this, 0, &ThreadID));
		BthrCheck = true;
	}
}

CVersion *m_Version;
//version 확인 다이얼로그
void CLogAdminDlg::OnHelpVersion()
{
	if (VersionCheck == FALSE)
	{
		m_Version = new CVersion(this);

		m_Version->Create(IDD_DIALOG4, this);
		m_Version->ShowWindow(SW_SHOW);
		VersionCheck = TRUE;
	}
}

CLogSaveSet * m_SaveSet;

//SaveSet 다이얼로그
void CLogAdminDlg::OnOptionSaveset()
{
	if (LogSaveSet == FALSE)
	{
		m_SaveSet = new CLogSaveSet(this);

		m_SaveSet->Create(IDD_DIALOG3, this);
		m_SaveSet->ShowWindow(SW_SHOW);
		LogSaveSet = TRUE;
	}
}

CDeleteFile * m_ProDel;
void CLogAdminDlg::OnToolAutodel()
{
	if (DelFolderCheck == FALSE)
	{
		m_ProDel = new CDeleteFile(this);

		m_ProDel->Create(IDD_DIALOG2, this);
		m_ProDel->ShowWindow(SW_SHOW);
		DelFolderCheck = TRUE;
	}
}

//Delete
void CLogAdminDlg::OnBnClickedButton2()
{
	int fre = m_watchProc.GetItemCount();

	//for (int i = 0; i < fre; i++)
	for (int i = fre - 1; i >= 0; i--)
	{
		if (m_watchProc.GetCheck(i) == TRUE)
		{
			CString str = ProcList[i].procName;

			m_watchProc.DeleteItem(i);
			ProcList.erase(ProcList.begin() + i);
			
			//콤보박스 삭제
			if (m_procListComboBox.FindString(-1, str))
			{
				int idx = m_procListComboBox.SelectString(-1, str);
				m_procListComboBox.DeleteString(idx);
			}

			m_procListComboBox.SetCurSel(0);


			AfxMessageBox(_T("Delete."));
		}
	}
}

//Always On Top
void CLogAdminDlg::OnViewAlwaysontop()
{
	//Always On Top@@@@@@@@@@@@@
	if (hMenu->GetMenuState(ID_VIEW_ALWAYSONTOP, MF_BYCOMMAND) != MF_CHECKED)
	{
		hMenu->CheckMenuItem(ID_VIEW_ALWAYSONTOP, MF_CHECKED);
		SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}
	else
	{
		hMenu->CheckMenuItem(ID_VIEW_ALWAYSONTOP, MF_UNCHECKED);
		SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}
}

BOOL a = FALSE;
//Autoscroll
void CLogAdminDlg::OnViewAutoscroll()
{
	if (hMenu->GetMenuState(ID_VIEW_AUTOSCROLL, MF_BYCOMMAND) != MF_CHECKED)
	{
		a = TRUE;
		hMenu->CheckMenuItem(ID_VIEW_AUTOSCROLL, MF_CHECKED);
	}
	else
	{
		a = FALSE;
		hMenu->CheckMenuItem(ID_VIEW_AUTOSCROLL, MF_UNCHECKED);
	}
}

void CLogAdminDlg::OnLvnInsertItenList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (a == TRUE)
	{
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		m_logList.EnsureVisible(pNMLV->iItem, FALSE);
		*pResult = 0;
	}
}
//검색기능
void CLogAdminDlg::OnBnClickedButton3()
{
	CString str, strTemp;

	GetDlgItemText(IDC_EDIT2, str);
	for (int i = 0; i < m_logList.GetItemCount(); i++)
	{
		BOOL b_strCheck = FALSE;
		strTemp = m_logList.GetItemText(i, 3);
		int n_idx = strTemp.Find(str);

		if (n_idx != -1)
		{
			b_strCheck = TRUE;
		}

		if (b_strCheck == FALSE)
		{
			m_logList.DeleteItem(i);
			i -= 1;
		}
	}
}

//Apply
void CLogAdminDlg::OnBnClickedButton4()
{
	comBoIdx = m_procListComboBox.GetCurSel();

	//선택된 프로세스이름 가져오기
	selectprocName;
	m_procListComboBox.GetLBText(m_procListComboBox.GetCurSel(), selectprocName);

	m_logList.DeleteAllItems();
	SetZero();
	CString allstr, str;

	////////////////////////////check box 조건값////////////////////////
	if (m_cInfo.GetCheck() == true && m_cError.GetCheck() == false && m_cWarning.GetCheck() == false)
	{
		n_LogViewSet = 1;
	}
	else if (m_cInfo.GetCheck() == false && m_cError.GetCheck() == true && m_cWarning.GetCheck() == false)
	{
		n_LogViewSet = 2;
	}
	else if (m_cInfo.GetCheck() == false && m_cError.GetCheck() == false && m_cWarning.GetCheck() == true)
	{
		n_LogViewSet = 3;
	}
	else if (m_cInfo.GetCheck() == true && m_cError.GetCheck() == true && m_cWarning.GetCheck() == false)
	{
		n_LogViewSet = 4;
	}
	else if (m_cInfo.GetCheck() == true && m_cError.GetCheck() == false && m_cWarning.GetCheck() == true)
	{
		n_LogViewSet = 5;
	}
	else if (m_cInfo.GetCheck() == false && m_cError.GetCheck() == true && m_cWarning.GetCheck() == true)
	{
		n_LogViewSet = 6;
	}
	else if (m_cInfo.GetCheck() == true && m_cError.GetCheck() == true && m_cWarning.GetCheck() == true)
	{
		n_LogViewSet = 0;
	}
	else if (m_cInfo.GetCheck() == false && m_cError.GetCheck() == false && m_cWarning.GetCheck() == false)
	{
		n_LogViewSet = 7;
	}

	if (bFileSele == FALSE)
		TodayRead();
	else if (bFileSele == TRUE)
		SetLogFile(strDir, bCheck);
}

//클립보드 복사함수
BOOL CLogAdminDlg::CopyListToClipboard(CListCtrl* pListCtrl, LPCTSTR lpszSeparator /*= _T("\t")*/)
{
	ASSERT(pListCtrl && ::IsWindow(pListCtrl->GetSafeHwnd()));

	CString sResult;
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition();
	if (!pos)
		return TRUE;

	CWaitCursor wait;
	int nItem, nCount = 0;
	int nColumn = 1;

	//컬럼 이름 관련 구조체
	TCHAR szName[128]; // This is the buffer where the name will be stored
	LVCOLUMN lvColInfo;
	lvColInfo.mask = LVCF_TEXT;
	lvColInfo.pszText = szName;
	lvColInfo.cchTextMax = _countof(szName);

	//헤더와 컬럼 갯수 구하기
	if ((pListCtrl->GetStyle() & LVS_TYPEMASK) == LVS_REPORT &&
		pListCtrl->GetExtendedStyle() & LVS_EX_FULLROWSELECT)
	{
		CHeaderCtrl* pHeader = pListCtrl->GetHeaderCtrl();
		nColumn = pHeader ? pHeader->GetItemCount() : 1;
	}

	//컬럼 이름 삽입
	for (int i = 0; i < nColumn; ++i)
	{
		pListCtrl->GetColumn(i, &lvColInfo);

		sResult += lvColInfo.pszText;
		if (i != nColumn - 1)
			sResult += lpszSeparator;
	}
	sResult += _T("\r\n");

	//데이터 내용 삽입
	while (pos)
	{
		nItem = pListCtrl->GetNextSelectedItem(pos);
		if (0 != nCount)
			sResult += _T("\r\n");

		for (int i = 0; i < nColumn; ++i)
		{
			sResult += pListCtrl->GetItemText(nItem, i);
			if (i != nColumn - 1)
				sResult += lpszSeparator;
		}
		++nCount;
	}
	sResult += _T("\r\n");

	//클립보드 저장
	if (pListCtrl->OpenClipboard())
	{
		EmptyClipboard();

		int nLen = sResult.GetLength() * sizeof(WCHAR) + 1;
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nLen);
		LPBYTE pGlobalData = (LPBYTE)GlobalLock(hGlobal);

		USES_CONVERSION_EX;
		CopyMemory(pGlobalData, T2CW_EX(sResult, _ATL_SAFE_ALLOCA_DEF_THRESHOLD), nLen);
		SetClipboardData(CF_UNICODETEXT, hGlobal);

		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);

		CloseClipboard();
		return TRUE;
	}
	return FALSE;
}

//복사기능 추가
BOOL CLogAdminDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == 'c' || pMsg->wParam == 'C')
		{
			if (GetKeyState(VK_CONTROL) < 0)
			{
				// 처리
				CopyListToClipboard(&m_logList);
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//컬럼 색 변경
void CLogAdminDlg::OnCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString strType;
	BOOL bErrorFlag = FALSE;
	BOOL bWarnningFlag = FALSE;

	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	strType = m_logList.GetItemText(pLVCD->nmcd.dwItemSpec, 3);

	//CString message = vec[i].Message;
	//CString error = _T("[WARNING]");
	//int index = message.Find(error);

	if ((strType.Find(_T("[Error]")) != -1))
	{
		bErrorFlag = TRUE;
	}

	if ((strType.Find(_T("[Warning]")) != -1))
	{
		bWarnningFlag = TRUE;
	}

	*pResult = 0;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYITEMDRAW;

	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		if (bErrorFlag)
		{
			pLVCD->clrText = RGB(255, 0, 0);  // 글자 색 변경 
			pLVCD->clrTextBk = RGB(0, 0, 0);  // 배경 색 변경 
		}
		else if (bWarnningFlag)
		{
			pLVCD->clrText = RGB(0, 0, 255);
			pLVCD->clrTextBk = RGB(237, 255, 255);
		}
		else
		{
			pLVCD->clrText = RGB(0, 0, 0);
		}

		*pResult = CDRF_DODEFAULT;
	}
}

// 필요없는 OnTimer
void CLogAdminDlg::OnTimer(UINT_PTR nIDEvent)
{
	HWND wndDlg = ::GetLastActivePopup(m_hWnd);
	if (wndDlg && wndDlg != m_hWnd)
	{
		char buffer[256] = { 0 };
		::GetClassName(wndDlg, (LPSTR)buffer, 256);
		if (CString("#32770") == buffer)
		{
			Sleep(2000);
			::EndDialog(wndDlg, IDOK);
		}
	}
}


void CLogAdminDlg::OnToolLivelog()
{
	//b_LiveLog 가 false 면 초기화, true면 실시간 로그 쌓기
	// TODO: Add your command handler code here
	if (b_LogFile == TRUE)
	{
		AfxMessageBox(_T("현재 로그파일 선택창이 열려있습니다."));
	}
	else
	{
		if (hMenu->GetMenuState(ID_TOOL_LIVELOG, MF_BYCOMMAND) != MF_CHECKED) // 체크가 안되어있으면 체크
		{
			m_logList.DeleteAllItems();
			SetZero();
			TodayRead();
			b_LiveLog = TRUE;
			hMenu->CheckMenuItem(ID_TOOL_LIVELOG, MF_CHECKED);
		}
	}
}

CLogFileChoose *m_LogFileChos;

void CLogAdminDlg::OnToolLoadlog()
{
	if (b_LogFile == FALSE)
	{
		//if (hMenu->GetMenuState(ID_TOOL_LIVELOG, MF_BYCOMMAND) == MF_CHECKED)  // 체크가 되어있으면 해제
		//{
		b_LiveLog = FALSE;

		hMenu->CheckMenuItem(ID_TOOL_LIVELOG, MF_UNCHECKED);

		m_LogFileChos = new CLogFileChoose(this);

		m_LogFileChos->Create(IDD_DIALOG_LOGFILE, this);
		m_LogFileChos->ShowWindow(SW_SHOW);
		//}
		b_LogFile = TRUE;
	}
}


CLogGraph *m_LogGraph;
//Graph
void CLogAdminDlg::OnViewGraph()
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile("C:\\Temp\\LogFolder\\today.txt");


	if (b_LogGraph == FALSE && bWorking == TRUE)
	{
		m_LogGraph = new CLogGraph(this);
		m_LogGraph->Create(IDD_DIALOG6, this);
		m_LogGraph->ShowWindow(SW_SHOW);
		
		b_LogGraph = TRUE;
	}
	else
	{
		AfxMessageBox("프로그램을 등록해주세요...!!!");
	}

}

void CLogAdminDlg::SetLogFile(CString Dir, BOOL bSelect)
{
	//AfxMessageBox(Dir);

	m_logList.DeleteAllItems();
	SetZero();
	//////////// 로그 파일 내용 저장
	bFileSele = TRUE;
	strDir = Dir;
	bCheck = bSelect;
	//////////////////////////////////
	CStdioFile file;
	CString  str;
	if (bSelect == TRUE)
	{
		if (file.Open(Dir, CFile::modeRead | CFile::typeText))
		{
			while (file.ReadString(str))
			{
				AddLogList(str);
			}
		}
		file.Close();
	}
	else
	{
		CXLEzAutomation dataexcel(FALSE);
		dataexcel.OpenExcelFile(Dir);
		//BOOL bExcel = TRUE;
		int i = 1;
		while (1)
		{
			CString strTemp = dataexcel.GetCellValue(1, i);
			if (strTemp == _T(""))
				break;
			else
			{
				AddLogList(strTemp);
				i++;
			}
		}
		dataexcel.ReleaseExcel();
	}
}

void CLogAdminDlg::OnThread()
{//LogDelID;
	m_pLogDel = AfxBeginThread(LogFileDel, this);

	if (m_pLogDel == NULL)
	{
		AfxMessageBox(_T("스레드 생성 에러"));
	}
	m_pLogDel->m_bAutoDelete = FALSE;
	m_eThreadWork = THREAD_RUNNING;
}

void CLogAdminDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	//DeleteDirectoryFile1(log);
	DeleteTodayTXT();
	CDialogEx::OnClose();
}


// 트레이 아이콘
LRESULT CLogAdminDlg::OnTaryNotifyAction(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case WM_RBUTTONDOWN:
	{
		CPoint ptMouse;
		::GetCursorPos(&ptMouse);
		CMenu menu;
		menu.LoadMenu(IDR_MENU_TRAY);//IDR_TRAYMENU
		CMenu * pMenu = menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptMouse.x, ptMouse.y, AfxGetMainWnd());
	}
	break;
	case WM_LBUTTONDOWN:
	{
		NOTIFYICONDATA nid;
		ZeroMemory(&nid, sizeof(nid));

		nid.cbSize = sizeof(nid);
		nid.uID = 0;
		nid.hWnd = GetSafeHwnd();

		BOOL bRet = ::Shell_NotifyIcon(NIM_DELETE, &nid);
		if (!bRet)
		{
			AfxMessageBox(_T("트레이아이콘 제거 실패"));
			return -1;
		}
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
	}
	break;
	}
	return 1;
}

void CLogAdminDlg::OnTraymenuExit()
{
	// TODO: Add your command handler code here
	::SendMessage(GetSafeHwnd(), WM_CLOSE, NULL, NULL);
}


void CLogAdminDlg::OnTraymenuOpendialog()
{
	// TODO: Add your command handler code here
	NOTIFYICONDATA nid;
	ZeroMemory(&nid, sizeof(nid));

	nid.cbSize = sizeof(nid);
	nid.uID = 0;
	nid.hWnd = GetSafeHwnd();

	BOOL bRet = ::Shell_NotifyIcon(NIM_DELETE, &nid);
	if (!bRet)
	{
		AfxMessageBox(_T("트레이아이콘 제거 실패"));
	}
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
}

//Clear Logging
void CLogAdminDlg::OnViewClearlogging()
{
	m_logList.DeleteAllItems();
	SetZero();
}



HBRUSH CLogAdminDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CLogAdminDlg::OnEraseBkgnd(CDC* pDC)
{
	/// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(50, 50, 50));
	return TRUE;
}

CAddGraph *m_LoadGraph;
void CLogAdminDlg::OnViewLoadgraph()
{
	if (b_LoadGraph == FALSE)
	{
		m_LoadGraph = new CAddGraph(this);
		m_LoadGraph->Create(IDD_DIALOG7, this);
		m_LoadGraph->ShowWindow(SW_SHOW);

		b_LoadGraph = TRUE;
	}
}



//send mail
void CLogAdminDlg::OnBnClickedButton6()
{
	bool bError = false;
	BOOL SendMail = FALSE;
	
		CSmtp mail;
		CString mailad;
		m_editCtrl.GetWindowTextA(mailad);

		CStringArray parsAfter;

		int count = splitString(mailad, _T("@"), parsAfter);

		if (count)
		{
			for (int i = 0; i < count; i++)
			{
				if (parsAfter.GetAt(i) != "gmail.com")
				{
					SendMail = TRUE;
					bError = true;
					AfxMessageBox("등록되지 않은 이메일 입니다.");
				}
				
			}
		}

		if (SendMail == FALSE)
		{
			mail.SetSMTPServer("smtp.gmail.com", 465); // smtp 서버
			mail.SetSecurityType(USE_SSL);
			mail.SetLogin("aquamz23@gmail.com"); // 보내는 사람의 login 할 아이디 
			mail.SetPassword("juju9697"); // 보내는 사람의 login password
			mail.SetSenderName("전성영"); // 보내는 사람 이름
			mail.SetSenderMail("aquamz23@gmail.com"); // 보내는 사람 이메일에 표시할 이메일
			mail.SetReplyTo("aquamz23@gmail.com"); // 답장 받을 이메일

			mail.SetSubject("전성영"); // 메일의 제목
			mail.AddRecipient(mailad); // 받는 사람 이메일 (여러명 추가가능)
			mail.SetXPriority(XPRIORITY_NORMAL);
			mail.SetXMailer("The Bat! (v3.02) Professional");
			mail.AddMsgLine("예린,");
			mail.AddMsgLine("");
			mail.AddMsgLine("...");
			mail.AddMsgLine("How are you today?");
			mail.AddMsgLine("");
			mail.AddMsgLine("Regards");
			mail.ModMsgLine(5, "regards");
			mail.DelMsgLine(2);
			mail.AddMsgLine("User");

			mail.AddAttachment("C:\\temp\\LogFolder\\today.txt");
			mail.Send();
		}

	if (!bError)
	{
		AfxMessageBox(_T("Send To Mail"));
	}
}

int CLogAdminDlg::splitString(CString str, CString var, CStringArray &strs)
{
	int count = 0;

	CString tempStr = str;

	int length = str.GetLength();

	while (length)
	{
		int find = tempStr.Find(var);
		if (find != -1)
		{
			CString temp = tempStr.Left(find);
			int varLen = _tcslen(var);
			tempStr = tempStr.Mid(find + varLen);
			count++;
		}
		else
		{
			strs.Add(tempStr);
			length = 0;
		}
	}
	return count;
}

// 리스트컨트롤에 정보 추가
BOOL bSlcheck = FALSE;
void CLogAdminDlg::SimpleListNameSet(CString ProcName)
{
	BOOL check = FALSE;
	TotalRe tr;
	int nfinal = 0;
	int size = m_List_SimepleView.GetItemCount();
	for (int i = 0; i < size; i++)
	{
		CString strName = _T("");
		strName = m_List_SimepleView.GetItemText(i, 0);

		if (ProcName == strName)
		{
			check = TRUE;
			break;
		}
		nfinal = i;
	}
	if (check == FALSE)
	{
		CString ToSuc, ToWar, ToErr;

		tr.procName = ProcName;
		tr.suc = 0;
		tr.war = 0;
		tr.err = 0;

		m_List_SimepleView.InsertItem(nfinal, ProcName);
		m_List_SimepleView.SetItemText(nfinal, 1, _T("0"));
		m_List_SimepleView.SetItemText(nfinal, 2, _T("0"));
		m_List_SimepleView.SetItemText(nfinal, 3, _T("0"));

		size++;
		BOOL bc = FALSE;
		for (int i = 0; i < m_List_SimepleView.GetItemCount(); i++)
		{
			CString name = _T("Total");
			CString listName = m_List_SimepleView.GetItemText(i, 0);

			if (name == listName)
			{
				bc = TRUE;
				break;
			}
		}
		if (bc == FALSE)
		{
			m_List_SimepleView.InsertItem(size, _T("Total"));
			m_List_SimepleView.SetItemText(size, 1, _T("0"));
			m_List_SimepleView.SetItemText(size, 2, _T("0"));
			m_List_SimepleView.SetItemText(size, 3, _T("0"));
		}
	}
} 

// total % 출력
void CLogAdminDlg::TotalPercent(int num)
{
	float nsuc, nwar, nerr, total, percent;

	CString strSuc, strWar, strErr, Temp;
	strSuc = m_List_SimepleView.GetItemText(num, 1);
	strWar = m_List_SimepleView.GetItemText(num, 2);
	strErr = m_List_SimepleView.GetItemText(num, 3);

	nsuc = _ttoi(strSuc);
	nwar = _ttoi(strWar);
	nerr = _ttoi(strErr);

	total = nsuc + nwar + nerr;
	//percent = total / 100;

	nsuc = (nsuc / total) * 100;
	nwar = (nwar / total) * 100;
	nerr = (nerr / total) * 100;

	Temp.Format(_T("Suc(%%) : %.1f %%"), nsuc);
	m_static_Suc.SetWindowTextA(Temp);

	Temp.Format(_T("War(%%) : %.1f %%"), nwar);
	m_static_War.SetWindowTextA(Temp);

	Temp.Format(_T("Err(%%) : %.1f %%"), nerr);
	m_static_Err.SetWindowTextA(Temp);
}

// Total 정보 수정
void CLogAdminDlg::TotalSetItem(BOOL bSuc, BOOL bWar, BOOL bErr)
{
	int num = 0, nsuc, nwar, nerr;

	for (int i = 0; i < m_List_SimepleView.GetItemCount(); i++)
	{
		CString name = _T("Total");
		CString listName = m_List_SimepleView.GetItemText(i, 0);

		if (name == listName)
		{
			num = i;
			break;
		}
	}

	if (num != 0)
	{
		CString strSuc, strWar, strErr;
		strSuc = m_List_SimepleView.GetItemText(num, 1);
		strWar = m_List_SimepleView.GetItemText(num, 2);
		strErr = m_List_SimepleView.GetItemText(num, 3);

		nsuc = _ttoi(strSuc);
		nwar = _ttoi(strWar);
		nerr = _ttoi(strErr);

		if (bSuc == TRUE)
			nsuc++;
		if (bWar == TRUE)
			nwar++;
		if (bErr == TRUE)
			nerr++;

		strSuc.Format(_T("%d"), nsuc);
		strWar.Format(_T("%d"), nwar);
		strErr.Format(_T("%d"), nerr);

		m_List_SimepleView.SetItemText(num, 1, strSuc);
		m_List_SimepleView.SetItemText(num, 2, strWar);
		m_List_SimepleView.SetItemText(num, 3, strErr);

		TotalPercent(num);
	}
}

// 리스트 컨트롤 해당되는 program에 누적
void CLogAdminDlg::SimpleListSet(CString ProcName, BOOL bSuc, BOOL bWar, BOOL bErr)
{
	int nsuc, nwar, nerr;
	BOOL check = FALSE;
	TotalRe tr;
	int idx = 0;

	int size = m_List_SimepleView.GetItemCount();
	for (int i = 0; i < size; i++)
	{
		CString strName = _T("");
		strName = "[" + m_List_SimepleView.GetItemText(i, 0) + "]";

		if (ProcName == strName)
		{
			check = TRUE;
			idx = i;
			break;
		}
	}

	if (check == TRUE)
	{
		CString strSuc, strWar, strErr;
		strSuc = m_List_SimepleView.GetItemText(idx, 1);
		strWar = m_List_SimepleView.GetItemText(idx, 2);
		strErr = m_List_SimepleView.GetItemText(idx, 3);

		nsuc = _ttoi(strSuc);
		nwar = _ttoi(strWar);
		nerr = _ttoi(strErr);

		if (bSuc == TRUE)
			nsuc++;
		if (bWar == TRUE)
			nwar++;
		if (bErr == TRUE)
			nerr++;

		strSuc.Format(_T("%d"), nsuc);
		strWar.Format(_T("%d"), nwar);
		strErr.Format(_T("%d"), nerr);

		m_List_SimepleView.SetItemText(idx, 1, strSuc);
		m_List_SimepleView.SetItemText(idx, 2, strWar);
		m_List_SimepleView.SetItemText(idx, 3, strErr);

		TotalSetItem(bSuc, bWar, bErr);
	}
}
