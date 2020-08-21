#pragma once


// CTestDlg4 dialog

#define  WM_NET_RSIDATAREFRESH (WM_USER + 110)

class CTestDlg4 : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDlg4)

public:
	CTestDlg4(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTestDlg4();

	CString Rsi_Signage_Longitude;
	CString Rsi_Signage_latitude;
	int   Warning_Type;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

	static UINT RsiDataRefresh(LPVOID pParam);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	LRESULT CTestDlg4::Rsi_Refresh(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
