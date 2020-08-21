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
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("����·��"));
			break;
		case 2:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("��ת��"));
			break;
		case 7:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("խ·"));
			break;
		case 8:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("խ��"));
			break;
		case 10:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("ע������"));
			break;
		case 14:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("ע���źŵ�"));
			break;
		case 15:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("ע����ʯ"));
			break;
		case 17:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("�׻�"));
			break;
		case 28:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("���˿�����·����"));
			break;
		case 37:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("ע��Σ��"));
			break;
		case 38:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("ʩ��"));
			break;
		case 47:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("ע��ǰ�������Ŷ�"));
			break;
		case 48:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("ͣ������"));
			break;
		case 81:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("���ƿ��"));
			break;
		case 82:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("���Ƹ߶�"));
			break;
		case 83:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("��������"));
			break;
		case 85:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("�����ٶ�"));
			break;
		case 242:
			SetDlgItemText(IDC_STATIC_Warn_Type, _T("ǰ��ѧУ"));
			break;
		default :
			break;
	}
	return 0;
}
