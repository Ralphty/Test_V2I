// CTestDlg3.cpp : implementation file
//

#include "pch.h"
#include "test.h"
#include "CTestDlg3.h"
#include "afxdialogex.h"


// CTestDlg3 dialog

extern float Road_Longitude;
extern float Road_latitude;
extern int   Region_ID;
extern int   Limit_Speed;

IMPLEMENT_DYNAMIC(CTestDlg3, CDialogEx)

CTestDlg3::CTestDlg3(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CTestDlg3::~CTestDlg3()
{
}

void CTestDlg3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Road_Long, Map_Road_Longitude);
	DDX_Text(pDX, IDC_EDIT_Road_Latit, Map_Road_latitude);
	DDX_Text(pDX, IDC_EDIT_Region_ID, Map_Region_ID);
	DDX_Text(pDX, IDC_EDIT_Limit_Speed, Map_Limit_Speed);
}


BEGIN_MESSAGE_MAP(CTestDlg3, CDialogEx)
	ON_MESSAGE(WM_NET_MAPDATAREFRESH, &CTestDlg3::Map_Refresh)
END_MESSAGE_MAP()


// CTestDlg3 message handlers


BOOL CTestDlg3::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	UpdateData(TRUE);
	Map_Road_Longitude = "0";
	Map_Road_latitude = "0";
	Map_Region_ID = 0;
	Map_Limit_Speed = 0;
	UpdateData(FALSE);

	CWinThread *my_pthread = AfxBeginThread(MapDataRefresh, this);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

UINT CTestDlg3::MapDataRefresh(LPVOID pParam)
{
	CTestDlg3 *pDlg = (CTestDlg3 *)pParam;
	while (1)
	{
		pDlg->PostMessage(WM_NET_MAPDATAREFRESH, 0);
		Sleep(1000);
	}

	return 0;
}

LRESULT CTestDlg3::Map_Refresh(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	Map_Road_Longitude.Format("%10.7f", Road_Longitude);
	Map_Road_latitude.Format("%10.7f", Road_latitude);
	//Map_Road_Longitude = Road_Longitude;
	//Map_Road_latitude = Road_latitude;
	Map_Region_ID = Region_ID;
	Map_Limit_Speed = Limit_Speed;
	UpdateData(FALSE);
	return 0;
}