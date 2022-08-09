
// ini입힐 program1Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ini입힐 program1.h"
#include "ini입힐 program1Dlg.h"
#include "afxdialogex.h"
#include "Server.h"
#include "Client.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CServer serverDlg;
CClient clientDlg;
CServer* g_pServer = NULL;
CClient* g_pClient = NULL;

int num;

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


// Cini입힐program1Dlg 대화 상자



Cini입힐program1Dlg::Cini입힐program1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INIPROGRAM1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cini입힐program1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cini입힐program1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_BUTTON1, &Cini입힐program1Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// Cini입힐program1Dlg 메시지 처리기

BOOL Cini입힐program1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

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
	num = 0;

	char *cBuf = NULL;
	cBuf = (char *)malloc(sizeof(char) * 256);
	memset(cBuf, 0x00, sizeof(cBuf));
	GetPrivateProfileString(_T("Program"), _T("Select"), _T("-"), (LPTSTR)cBuf, 256, _T("C:\\Temp\\ProjectSimul2.ini"));
	num = atoi(cBuf);

	if (num == 1)
	{
		serverDlg.DoModal();
		ShowWindow(SW_SHOWMINIMIZED);//! 최소화후 숨겨야 화면에 나타나지 않음
		PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
	}
	else if (num == 2)
	{
		clientDlg.DoModal();
		ShowWindow(SW_SHOWMINIMIZED);//! 최소화후 숨겨야 화면에 나타나지 않음
		PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void Cini입힐program1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void Cini입힐program1Dlg::OnPaint()
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
HCURSOR Cini입힐program1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cini입힐program1Dlg::OnBnClickedButton1()
{
	if (IsDlgButtonChecked(IDC_RADIO1) == FALSE && IsDlgButtonChecked(IDC_RADIO2) == FALSE)
	{
		AfxMessageBox(_T("체크하세요."));
		WritePrivateProfileString(_T("Program"), _T("Select"), _T("0"), _T("C:\\Temp\\ProjectSimul2.ini"));
	}
	else if (IsDlgButtonChecked(IDC_RADIO1) == TRUE)
	{
		WritePrivateProfileString(_T("Program"), _T("Select"), _T("1"), _T("C:\\Temp\\ProjectSimul2.ini"));
	}
	else if (IsDlgButtonChecked(IDC_RADIO2) == TRUE)
	{
		WritePrivateProfileString(_T("Program"), _T("Select"), _T("2"), _T("C:\\Temp\\ProjectSimul2.ini"));
	}

	char *cBuf = NULL;
	cBuf = (char *)malloc(sizeof(char) * 256);
	memset(cBuf, 0x00, sizeof(cBuf));
	GetPrivateProfileString(_T("Program"), _T("Select"), _T("-"), (LPTSTR)cBuf, 256, _T("C:\\Temp\\ProjectSimul2.ini"));
	num = atoi(cBuf);

	if (num == 1)
	{
		serverDlg.DoModal();
		ShowWindow(SW_SHOWMINIMIZED);//! 최소화후 숨겨야 화면에 나타나지 않음
		PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
	}
	else if (num == 2)
	{
		clientDlg.DoModal();
		ShowWindow(SW_SHOWMINIMIZED);//! 최소화후 숨겨야 화면에 나타나지 않음
		PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
	}
}
// ** 사용이 완료되면 프로그래머가 해제를 해줘야한다.
// m_pTestDlg->DestroyWindow();
// m_pTestDlg = NULL;