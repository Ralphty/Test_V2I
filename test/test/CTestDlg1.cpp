// CTestDlg1.cpp : implementation file
//

#include "pch.h"
#include "test.h"
#include "CTestDlg1.h"
#include "afxdialogex.h"
#include "afx.h"
#include "locale.h"
#include "afxwin.h"
#include <iostream>
#include "testDlg.h"
#include "define.h"


using namespace std;

int g_nIndex = 0;

// CTestDlg1 dialog

UINT Thread_Receive(LPVOID pParam);
UINT Thread_Show(LPVOID pParam);
uint8_t SendFlag = 0;

IMPLEMENT_DYNAMIC(CTestDlg1, CDialogEx)

CTestDlg1::CTestDlg1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_GreenTime(0)
	, m_YellowTime(0)
	, m_RedTime(0)
	, m_TimeLeft(_T(""))
{
	m_ServerIP = _T("127.0.0.1");
	m_ServerPort = 6000;
}

CTestDlg1::~CTestDlg1()
{
}

void CTestDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GreenTime, m_GreenTime);
	DDX_Text(pDX, IDC_EDIT_YellowTime, m_YellowTime);
	DDX_Text(pDX, IDC_EDIT_RedTime, m_RedTime);
	DDX_Control(pDX, IDC_Light_STATIC, m_StatusLight);
	DDX_Text(pDX, IDC_STATIC_TimeLeft, m_TimeLeft);

	DDX_Control(pDX, IDC_COMBO1, m_CmbDir);
}


BEGIN_MESSAGE_MAP(CTestDlg1, CDialogEx)

	ON_MESSAGE(WM_NET_SENDSignalINFO, OnSendData)

	ON_MESSAGE(WM_NET_RECEIVE, Receive_Handle)

	ON_BN_CLICKED(IDOK, &CTestDlg1::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTestDlg1::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTestDlg1::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CTestDlg1 message handlers


BOOL CTestDlg1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	SetDlgItemText(IDC_ConnectInfo, _T("信号机"));

	//创建网络套接字
	AfxSocketInit();
	BOOL b = send_socket.Create(0, SOCK_DGRAM);
	if (!b)
	{
		cout << GetLastError() << endl;
	}


	BOOL c = receive_socket.Create(5555, SOCK_DGRAM);
	if (!c)
	{
		cout << GetLastError() << endl;
	}

	pthread_Senddata1 = AfxBeginThread(Thread_Linstening, this);
	//pthread_Senddata2 = AfxBeginThread(Thread_Receive, this);
	//pthread_Senddata1->ResumeThread();
	//pthread_Senddata2->ResumeThread();


	UpdateData(TRUE);
	m_GreenTime = 30;
	m_YellowTime = 3;
	m_RedTime = 28;
	UpdateData(FALSE);
	m_CmbDir.ResetContent();
	m_CmbDir.AddString(_T("左转"));
	m_CmbDir.AddString(_T("直行"));
	m_CmbDir.AddString(_T("右转"));
	m_CmbDir.SetCurSel(0);
	g_nIndex = 0;

	CString timeStr;
	CTime tm;
	tm = CTime::GetCurrentTime();
	timeStr = tm.Format("%Y%m%d_%H%M%S");


	CString strTmp;
	CString str;
	CString fileStr;
	CString Dirstr = "C:\\file";
	if (!PathIsDirectory(Dirstr))
	{
		::CreateDirectory(Dirstr, NULL);
	}
	fileStr.Format(_T("C:\\file\\Save%s.txt"), timeStr);
	file.Open(fileStr, CFile::modeCreate | CFile::modeWrite);
	strTmp.Format("当前时间                           当前灯色         红灯结束时间          绿灯结束时间          黄灯结束时间\r\n");
	str += strTmp;
	file.Write((LPCTSTR)str, str.GetLength());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CTestDlg1::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	SendFlag = 1;
	//pthread_Senddata1->ResumeThread();
	//CDialogEx::OnOK();
}


void CTestDlg1::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//pthread_Senddata1->SuspendThread();
	//m_socketSend.Close();
	//file.Close();
	SendFlag = 0;
	//CDialogEx::OnCancel();
}


void CTestDlg1::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	g_nIndex = m_CmbDir.GetCurSel();
}

UINT CTestDlg1::Thread_Linstening(LPVOID pParam)
{
	CTestDlg1 * pDlg = (CTestDlg1 *)pParam;

	// 创建线程
	CWinThread *pthread_Senddata1 = AfxBeginThread(Thread_Show, pDlg, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	CWinThread *pthread_Senddata2 = AfxBeginThread(Thread_Receive, pDlg, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pthread_Senddata1->ResumeThread();
	pthread_Senddata2->ResumeThread();

	return 0;
}

UINT Thread_Receive(LPVOID pParam)
{
	CTestDlg1 * pDlg = (CTestDlg1 *)pParam;
	while (1)
	{
		pDlg->PostMessage(WM_NET_RECEIVE, 0);
		//pDlg->Receive_Handle();
		Sleep(1000);
	}

	return 0;
}

LRESULT CTestDlg1::Receive_Handle(WPARAM wParam, LPARAM lParam)
{
	char RecBuffer[60];
	//CString serverIP;
	//UINT serverPort;
	uint16_t Head;

	receive_socket.Receive(RecBuffer, 20);
	//receive_socket.ReceiveFrom(RecBuffer,20,serverIP,serverPort);
	//OutputDebugString(RecBuffer);

	memcpy(&Head, &RecBuffer, 2);

	if (Head == 0xAA55)
	{
		memcpy(&g_MapData.Node_Longitude, &RecBuffer[2], 4);
		memcpy(&g_MapData.Node_Latitude, &RecBuffer[6], 4);
		memcpy(&g_MapData.RegionID, &RecBuffer[10], 4);
		memcpy(&g_MapData.LimitSpeed, &RecBuffer[14], 4);
		Road_Longitude = (float)g_MapData.Node_Longitude / 10000000;
		Road_latitude = (float)g_MapData.Node_Latitude / 10000000;
		Region_ID = g_MapData.RegionID;
		Limit_Speed = g_MapData.LimitSpeed;
	}
	else if (Head == 0x55AA)
	{
		memcpy(&g_RsiData.Signage_Longitude, &RecBuffer[2], 4);
		memcpy(&g_RsiData.Signage_Latitude, &RecBuffer[6], 4);
		memcpy(&g_RsiData.Warning_Type, &RecBuffer[10], 4);

		Signage_Longitude = (float)g_RsiData.Signage_Longitude / 10000000;
		Signage_Latitude = (float)g_RsiData.Signage_Latitude / 10000000;
		Warn_Type = g_RsiData.Warning_Type;
	}
	else 
	{
	}

	return 0;
}

uint16_t Period = 0;
int Time = 0;
UINT Thread_Show(LPVOID pParam)
{
	CTestDlg1 * pDlg = (CTestDlg1 *)pParam;

	int p_nTimeInterval = 1000;

	while (1)
	{
		if (SendFlag == 1) {
			pDlg->PostMessage(WM_NET_SENDSignalINFO, 0);
			Sleep(p_nTimeInterval);
			Time++;

			if (Time == Period)
			{
				Time = 0;
			}
		}

	}

	return 0;
}

LRESULT CTestDlg1::OnSendData(WPARAM wParam, LPARAM lParam)
{
	uint8_t Status;
	uint16_t TimeLeft;
	UpdateData(TRUE);
	int length = sizeof(g_SPaTMsg);

	uint16_t RedTimeLeft;
	uint16_t YellowTimeLeft;
	uint16_t GreenTimeLeft;

	CString Lightstr;

	char StatusBuf[60];
	CString str;
	CString timeStr;
	CTime tm;
	tm = CTime::GetCurrentTime();
	timeStr = tm.Format("%Y%m%d_%H%M%S");


	g_SPaTMsg.Head = 0xFEFE;
	g_SPaTMsg.FunCode = 0x10;
	g_SPaTMsg.MsgLength = SW_16(0x000E);

	g_SPaTMsg.PhaseNum = 0x01;
	g_SPaTMsg.Mehtod = g_nIndex;

	g_SPaTMsg.m_Phase.PhaseId = 0x01;
	g_SPaTMsg.m_Phase.Green = SW_16(m_GreenTime);
	g_SPaTMsg.m_Phase.Yellow = SW_16(m_YellowTime);
	g_SPaTMsg.m_Phase.Red = SW_16(m_RedTime);
	Period = m_GreenTime + m_YellowTime + m_RedTime;
	g_SPaTMsg.Cycle = SW_16(Period);

	g_SPaTMsg.Reserved = 0;
	g_SPaTMsg.End = 0x55;
	UpdateData(false);


	StatusChange(m_GreenTime, m_YellowTime, m_RedTime, Time, &Status, &TimeLeft);
	g_SPaTMsg.m_Phase.Status = Status;
	g_SPaTMsg.m_Phase.TimeLeft = TimeLeft;
	uint16_t timeBig = SW_16(g_SPaTMsg.m_Phase.TimeLeft);

	if (Status == 3)
	{
		Lightstr = "Red";
		RedTimeLeft = TimeLeft;
		GreenTimeLeft = TimeLeft + m_GreenTime;
		YellowTimeLeft = TimeLeft + m_GreenTime + m_YellowTime;
	}
	else if (Status == 1)
	{
		Lightstr = "Green";
		GreenTimeLeft = TimeLeft;
		YellowTimeLeft = TimeLeft + m_YellowTime;
		RedTimeLeft = TimeLeft + m_YellowTime + m_RedTime;
	}
	else
	{
		Lightstr = "Yellow";
		YellowTimeLeft = TimeLeft;
		RedTimeLeft = TimeLeft + m_RedTime;
		GreenTimeLeft = TimeLeft + m_RedTime + m_GreenTime;
	}

	str.Format("%s                  %s                      %d                           %d                          %d\r\n", timeStr, Lightstr, RedTimeLeft, GreenTimeLeft, YellowTimeLeft);
	file.Write((LPCTSTR)str, str.GetLength());

	memcpy(&StatusBuf, &g_SPaTMsg.Head, 2);
	memcpy(&StatusBuf[2], &g_SPaTMsg.FunCode, 1);
	memcpy(&StatusBuf[3], &g_SPaTMsg.MsgLength, 2);

	memcpy(&StatusBuf[5], &g_SPaTMsg.Cycle, 2);
	memcpy(&StatusBuf[7], &g_SPaTMsg.PhaseNum, 1);
	memcpy(&StatusBuf[8], &g_SPaTMsg.Mehtod, 1);

	memcpy(&StatusBuf[9], &g_SPaTMsg.m_Phase.PhaseId, 1);
	memcpy(&StatusBuf[10], &g_SPaTMsg.m_Phase.Green, 2);
	memcpy(&StatusBuf[12], &g_SPaTMsg.m_Phase.Yellow, 2);
	memcpy(&StatusBuf[14], &g_SPaTMsg.m_Phase.Red, 2);
	memcpy(&StatusBuf[16], &g_SPaTMsg.m_Phase.Status, 1);
	memcpy(&StatusBuf[17], &timeBig, 2);

	memcpy(&StatusBuf[19], &g_SPaTMsg.Reserved, 4);
	memcpy(&StatusBuf[23], &g_SPaTMsg.End, 1);

	//m_socketSend.Send((void *)StatusBuf, 24, 0);
	send_socket.SendTo((void *)StatusBuf, 24, 6000, _T("192.168.20.199"));

	PictureLoad(Status);

	SetDlgItemInt(IDC_STATIC_TimeLeft, TimeLeft, 1);
	return 0;
}

void CTestDlg1::StatusChange(uint16_t Green, uint16_t Yellow, uint16_t Red, uint8_t Time, uint8_t * Status, uint16_t *TimeLeft)
{
	uint8_t p_Status;
	uint16_t p_TimeLeft;
	uint16_t Period = Green + Yellow + Red;

	if (Time < Red)
	{
		p_Status = 3;
		p_TimeLeft = Red - Time;
	}
	else if (Time < (Red + Green) && Time >= Red)
	{
		p_Status = 1;
		p_TimeLeft = Red + Green - Time;
	}
	else
	{
		p_Status = 2;
		p_TimeLeft = Period - Time;
	}
	*Status = p_Status;
	*TimeLeft = p_TimeLeft;

}

void CTestDlg1::PictureLoad(uint8_t status)
{
	CBitmap bitmap;
	HBITMAP bBmp;

	if (status == 1)
	{
		bitmap.LoadBitmap(IDB_Green);
		bBmp = (HBITMAP)bitmap.GetSafeHandle(); 

		m_StatusLight.SetBitmap(bitmap);
	}
	else if (status == 2)
	{
		bitmap.LoadBitmap(IDB_Yellow);
		bBmp = (HBITMAP)bitmap.GetSafeHandle();
		m_StatusLight.SetBitmap(bBmp);
	}
	else
	{
		bitmap.LoadBitmap(IDB_Red);
		bBmp = (HBITMAP)bitmap.GetSafeHandle();
		m_StatusLight.SetBitmap(bBmp);
	}
	UpdateData(FALSE);
}