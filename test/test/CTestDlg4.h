#pragma once


// CTestDlg4 dialog

class CTestDlg4 : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDlg4)

public:
	CTestDlg4(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTestDlg4();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
