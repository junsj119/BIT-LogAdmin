#pragma once
#include "afxwin.h"
#include "ConnectSocket.h"
#include "afxext.h"


// CClient dialog

class CClient : public CDialogEx
{
	DECLARE_DYNAMIC(CClient)

public:
	CClient(CWnd* pParent = NULL);   // standard constructor
	virtual ~CClient();
	ConnectSocket m_Socket;
	ConnectSocket m_Socket_Program;
	BOOL bServerCloseCheck;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_cPicture;
	CStatic m_cTestPicture;
	CListBox m_cLogList;
	CListBox m_cPictureList;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnClose();
	void CClient::LoadBmpList();
	void CClient::FirstArt();
	void CClient::SetDirFile(CString fulllName);
	void CClient::detection(CString fullname, CString picName);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();


	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBitmapButton m_threadBtn;
	CBitmapButton m_btnRestart;
	afx_msg void OnBnClickedButton5();
};
