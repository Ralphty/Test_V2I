// CTestDlg4.cpp : implementation file
//

#include "pch.h"
#include "test.h"
#include "CTestDlg4.h"
#include "afxdialogex.h"


// CTestDlg4 dialog

extern float Signage_Longitude;
extern float Signage_Latitude;
extern int    Warn_Type;

IMPLEMENT_DYNAMIC(CTestDlg4, CDialogEx)

CTestDlg4::CTestDlg4(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

CTestDlg4::~CTestDlg4()
{
}

void CTestDlg4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Signage_Longit, Rsi_Signage_Longitude);
	DDX_Text(pDX, IDC_EDIT_Signage_Latitu, Rsi_Signage_latitude);
	//DDX_Text(pDX, IDC_EDIT_Region_ID, Map_Region_ID);
}


BEGIN_MESSAGE_MAP(CTestDlg4, CDialogEx)
	ON_MESSAGE(WM_NET_RSIDATAREFRESH, &CTestDlg4::Rsi_Refresh)
END_MESSAGE_MAP()


// CTestDlg4 message handlers


BOOL CTestDlg4::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	SetDlgItemText(IDC_STATIC_Warn_Type, _T("NO WARNING"));

	UpdateData(TRUE);
	Rsi_Signage_Longitude = "0";
	Rsi_Signage_latitude = "0";
	UpdateData(FALSE);

	CWinThread *my_pthread = AfxBeginThread(RsiDataRefresh, this);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

UINT CTestDlg4::RsiDataRefresh(LPVOID pParam)
{
	CTestDlg4 *pDlg = (CTestDlg4 *)pParam;
	while (1)
	{
		pDlg->PostMessage(WM_NET_RSIDATAREFRESH, 0);
		Sleep(1000);
	}

	return 0;
}

LRESULT CTestDlg4::Rsi_Refresh(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	Rsi_Signage_Longitude.Format("%10.7f", Signage_Longitude);
	Rsi_Signage_latitude.Format("%10.7f", Signage_Latitude);
	UpdateData(FALSE);
	Warning_Type = Warn_Type;
	switch (Warning_Type)
	{
		case 1:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("交叉路口"));
			break;
		case 2:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("急转弯"));
			break;
		case 7:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("窄路"));
			break;
		case 8:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("窄桥"));
			break;
		case 10:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("注意行人"));
			break;
		case 14:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("注意信号灯"));
			break;
		case 15:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("注意落石"));
			break;
		case 17:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("易滑"));
			break;
		case 28:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("有人看守铁路道口"));
			break;
		case 37:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("注意危险"));
			break;
		case 38:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("施工"));
			break;
		case 47:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("注意前方车辆排队"));
			break;
		case 48:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("停车让行"));
			break;
		case 81:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("限制宽度"));
			break;
		case 82:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("限制高度"));
			break;
		case 83:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("限制质量"));
			break;
		case 85:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("限制速度"));
			break;
		case 242:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("前方学校"));
			break;
		default :
			break;
	}
	return 0;
}
