
// ini���� program1Dlg.h : ��� ����
//

#pragma once
#define LOGADMIN_IP	_T("127.0.0.1")
#define SERVER_IP 	_T("127.0.0.1")

// Cini����program1Dlg ��ȭ ����
class Cini����program1Dlg : public CDialogEx
{
// �����Դϴ�.
public:
	Cini����program1Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INIPROGRAM1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
