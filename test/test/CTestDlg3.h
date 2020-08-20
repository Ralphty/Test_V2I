#pragma once


// CTestDlg3 dialog

class CTestDlg3 : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDlg3)

public:
	CTestDlg3(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTestDlg3();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
