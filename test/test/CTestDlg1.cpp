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


using namespace std;

int g_nIndex = 0;

// CTestDlg1 dialog

UINT Thread_Show(LPVOID pParam);
UINT Thread_Checking(LPVOID pParam);
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

	ON_MESSAGE(WM_NET_CHECK, RSUChecking)

	ON_BN_CLICKED(IDOK, &CTestDlg1::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTestDlg1::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTestDlg1::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CTestDlg1 message handlers


BOOL CTestDlg1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	SetDlgItemText(IDC_ConnectInfo, _T("�źŻ�"));

	//���������׽���
	AfxSocketInit();
	BOOL b = m_socket.Create(0, SOCK_DGRAM);
	if (!b)
	{
		cout << GetLastError() << endl;
	}

	pthread_Senddata1 = AfxBeginThread(Thread_Linstening, this);

	UpdateData(TRUE);
	m_GreenTime = 30;
	m_YellowTime = 3;
	m_RedTime = 28;
	UpdateData(FALSE);
	m_CmbDir.ResetContent();
	m_CmbDir.AddString(_T("��ת"));
	m_CmbDir.AddString(_T("ֱ��"));
	m_CmbDir.AddString(_T("��ת"));
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
	strTmp.Format("��ǰʱ��                           ��ǰ��ɫ         ��ƽ���ʱ��          �̵ƽ���ʱ��          �Ƶƽ���ʱ��\r\n");
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

	// �����߳�
	CWinThread *pthread_Senddata1 = AfxBeginThread(Thread_Show, pDlg, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pthread_Senddata1->ResumeThread();

	return 0;
}

uint8_t RSURegister = 0;
uint8_t RSUServerCode = 0;
uint8_t RSUDeviceRegister = 0;

UINT Thread_Checking(LPVOID pParam)
{
	CTestDlg1 * pDlg = (CTestDlg1 *)pParam;
	int RevFlag = 0;
	while (RevFlag == 0)
	{
		pDlg->PostMessage(WM_NET_CHECK, 0);
		Sleep(500);
		if (RSURegister == 1 && RSUServerCode == 1 && RSUDeviceRegister == 1)
		{
			RevFlag = 1;
		}

	}

	return 0;
}

LRESULT CTestDlg1::RSUChecking(WPARAM wParam, LPARAM lParam)
{
	char RecBuffer[60];
	char OK[] = "ok";
	m_socket.Receive(RecBuffer, 60);

	memcpy(&Register.Head, &RecBuffer, 2);
	memcpy(&Register.Version, &RecBuffer[2], 1);
	memcpy(&Register.FrameType, &RecBuffer[3], 1);

	if (Register.Head == 0x595A)
	{
		if (Register.FrameType == 0x41)
		{
			SetDlgItemText(IDC_ConnectInfo, _T("RSUע��ɹ���"));
			RSURegister = 1;
			m_socket.Send((void *)OK, sizeof(OK));
		}
		else if (Register.FrameType == 0x42)
		{
			SetDlgItemText(IDC_ConnectInfo, _T("RSU��Ŀ������ע��ɹ���"));
			RSUServerCode = 1;
			m_socket.Send((void *)OK, sizeof(OK));
		}
		else
		{
			SetDlgItemText(IDC_ConnectInfo, _T("RSU·���豸ע��ɹ���"));
			RSUDeviceRegister = 1;
			m_socket.Send((void *)OK, sizeof(OK));
		}
	}
	if (RSURegister == 1 && RSUServerCode == 1 && RSUDeviceRegister == 1)
	{
		CWinThread *pthread_Senddata1 = AfxBeginThread(Thread_Show, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		pthread_Senddata1->ResumeThread();
		RSURegister = 0;
		RSUServerCode = 0;
		RSUDeviceRegister = 0;
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
	m_socket.SendTo((void *)StatusBuf, 24, 6000, _T("192.168.20.199"));

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