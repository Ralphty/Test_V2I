#pragma once


// CTestDlg3 dialog

#define  WM_NET_MAPDATAREFRESH (WM_USER + 109)

class CTestDlg3 : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDlg3)

public:
	CTestDlg3(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTestDlg3();

	CString Map_Road_Longitude;
	CString Map_Road_latitude;
	int   Map_Region_ID;
	int   Map_Limit_Speed;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

	static UINT MapDataRefresh(LPVOID pParam);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	LRESULT CTestDlg3::Map_Refresh(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
