// CTestDlg2.cpp : implementation file
//

#include "pch.h"
#include "test.h"
#include "CTestDlg2.h"
#include "afxdialogex.h"
#include "afxdialogex.h"
#include "stdio.h"
#include <iostream>
#include "afxwin.h"

using namespace std;

int g_IndexNumber = 0;
uint8_t SendRsmFlag = 0;


// CTestDlg2 dialog

IMPLEMENT_DYNAMIC(CTestDlg2, CDialogEx)

CTestDlg2::CTestDlg2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CTestDlg2::~CTestDlg2()
{
}

void CTestDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NUMBER, m_CmbNumber);
	DDX_Control(pDX, IDC_COMBO_INFORMATION_SOURCE, m_CmbInfoSource);
	DDX_Control(pDX, IDC_COMBO_TYPE_1, m_CmbType1);
	DDX_Control(pDX, IDC_COMBO_COLOR1, m_CmbColor1);
	DDX_Text(pDX, IDC_EDIT_CONFIDENCE1, m_TextConfidence1);
	DDX_Text(pDX, IDC_EDIT_LONGITUDE_1, m_TextLongitude1);
	DDX_Text(pDX, IDC_EDIT_LATITUDE_1, m_TextLatitude1);
	DDX_Text(pDX, IDC_EDIT_SPEED_1, m_TextSpeed1);
	DDX_Text(pDX, IDC_EDIT_COURSE_ANGLE_1, m_TextCourseAngle1);
	DDX_Text(pDX, IDC_EDIT_X_1, m_TextPositionX1);
	DDX_Text(pDX, IDC_EDIT_Y_1, m_TextPositionY1);
	DDX_Text(pDX, IDC_EDIT_Z_1, m_TextPositionZ1);
	DDX_Text(pDX, IDC_EDIT_LIDAR_LATITUDE, m_TextLidarLongitude);
	DDX_Text(pDX, IDC_EDIT_LIDAR_LONGITUDE, m_TextLidarLatitude);
}


BEGIN_MESSAGE_MAP(CTestDlg2, CDialogEx)

	ON_MESSAGE(WM_NET_SENDRSMSignalINFO, &CTestDlg2::OnSendData)
	ON_BN_CLICKED(IDOK, &CTestDlg2::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTestDlg2::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTestDlg2 message handlers


BOOL CTestDlg2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	m_CmbNumber.ResetContent();
	m_CmbNumber.AddString(_T("1"));
	m_CmbNumber.AddString(_T("2"));
	m_CmbNumber.AddString(_T("3"));
	m_CmbNumber.SetCurSel(0);
	m_CmbInfoSource.ResetContent();
	m_CmbInfoSource.AddString(_T("0"));
	m_CmbInfoSource.AddString(_T("1"));
	m_CmbInfoSource.AddString(_T("2"));
	m_CmbInfoSource.SetCurSel(0);
	m_CmbType1.ResetContent();
	m_CmbType1.AddString(_T("0"));
	m_CmbType1.AddString(_T("1"));
	m_CmbType1.AddString(_T("2"));
	m_CmbType1.AddString(_T("3"));
	m_CmbType1.AddString(_T("4"));
	m_CmbType1.AddString(_T("5"));
	m_CmbType1.AddString(_T("6"));
	m_CmbType1.SetCurSel(4);
	m_CmbColor1.ResetContent();
	m_CmbColor1.AddString(_T("0"));
	m_CmbColor1.AddString(_T("1"));
	m_CmbColor1.AddString(_T("2"));
	m_CmbColor1.AddString(_T("3"));
	m_CmbColor1.AddString(_T("4"));
	m_CmbColor1.AddString(_T("5"));
	m_CmbColor1.AddString(_T("6"));
	m_CmbColor1.AddString(_T("7"));
	m_CmbColor1.SetCurSel(1);

	//网络初始化配置
	AfxSocketInit();
	BOOL b = rsm_socket.Create(0, SOCK_DGRAM);//创建SOCKET
	if (!b)
	{
		cout << GetLastError() << endl;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CTestDlg2::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CWinThread *pthread_Senddata1 = AfxBeginThread(SendDataThread, this);
	SendRsmFlag = 1;
	//CDialogEx::OnOK();
}


void CTestDlg2::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	SendRsmFlag = 0;
	//CDialogEx::OnCancel();
}

UINT CTestDlg2::SendDataThread(LPVOID pParam)
{
	CTestDlg2 *pDlg = (CTestDlg2 *)pParam;
	while (1)
	{
		if (SendRsmFlag == 1)
		{
			pDlg->PostMessage(WM_NET_SENDRSMSignalINFO, 0);
			//Sleep(1000);
		}
		Sleep(1000);
	}

	return 0;
}

LRESULT CTestDlg2::OnSendData(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	g_UdpSendRSM.m_MessageContent.m_ParticipantInformation = &g_ParticipantInformation;
	char StatusBuf[100];

	g_IndexNumber = m_CmbNumber.GetCurSel();

#if 1
	if (g_IndexNumber == 0)
	{
		g_UdpSendRSM.StartBit = 0xFFFF;
		g_UdpSendRSM.SerialNumber = 0;
		g_UdpSendRSM.MasterNumber = 0xE1;
		g_UdpSendRSM.SlaveNumber = 0x02;
		g_UdpSendRSM.StatusBit = 0;
		g_UdpSendRSM.MessageLength = SW_EX_16(0x0040);
		g_UdpSendRSM.m_MessageContent.FacilityID = SW_EX_16(0x0001);
		g_UdpSendRSM.m_MessageContent.Reserved1 = 0;
		g_UdpSendRSM.m_MessageContent.DataFrameNumber = 0;
		g_UdpSendRSM.m_MessageContent.TimeStamp = 0;
		g_UdpSendRSM.m_MessageContent.LidarLongitude = SW_EX_32(m_TextLidarLongitude);
		g_UdpSendRSM.m_MessageContent.LidarLatitude = SW_EX_32(m_TextLidarLatitude);
		g_UdpSendRSM.m_MessageContent.LidarAngle = SW_EX_16(0x0048);
		g_UdpSendRSM.m_MessageContent.TrafficParticipantNumber = 0x01;
		g_UdpSendRSM.m_MessageContent.Reserved2[0] = 0;
		g_UdpSendRSM.m_MessageContent.Reserved2[1] = 0;
		g_UdpSendRSM.m_MessageContent.Reserved2[2] = 0;
		g_UdpSendRSM.m_MessageContent.Reserved2[3] = 0;
		g_UdpSendRSM.m_MessageContent.Reserved2[4] = 0;
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantID = SW_EX_16(0x0001);
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantType = m_CmbType1.GetCurSel();
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantConfidence = m_TextConfidence1;
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantColor = m_CmbColor1.GetCurSel();
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantInformationSource = m_CmbInfoSource.GetCurSel();
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantReserved1[0] = 0;
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantReserved1[1] = 0;
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantLongitude = SW_EX_32(m_TextLongitude1);
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantLatitude = SW_EX_32(m_TextLatitude1);
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantAltitude = SW_EX_16(0x10F4);
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantSpeed = SW_EX_16(m_TextSpeed1);
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantCourseAngle = SW_EX_16(m_TextCourseAngle1);
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantLength = SW_EX_16(0x001E);
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantWidth = SW_EX_16(0x001E);
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantHeight = SW_EX_16(0x0084);
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantPositionX = SW_EX_16(m_TextPositionX1);
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantPosotionY = SW_EX_16(m_TextPositionY1);
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantPosotionZ = SW_EX_16(m_TextPositionZ1);
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantReserved2[0] = 0;
		g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantReserved2[1] = 0;
		g_UdpSendRSM.m_MessageContent.Reserved3[0] = 0;
		g_UdpSendRSM.m_MessageContent.Reserved3[1] = 0;
		g_UdpSendRSM.m_MessageContent.Reserved3[2] = 0;
		g_UdpSendRSM.m_MessageContent.Reserved3[3] = 0;
		g_UdpSendRSM.ParityBit = 0x01;
		g_UdpSendRSM.EndBit = 0xFF;
		UpdateData(false);



		memcpy(&StatusBuf[0], &g_UdpSendRSM.StartBit, 2);
		memcpy(&StatusBuf[2], &g_UdpSendRSM.SerialNumber, 1);
		memcpy(&StatusBuf[3], &g_UdpSendRSM.MasterNumber, 1);
		memcpy(&StatusBuf[4], &g_UdpSendRSM.SlaveNumber, 1);
		memcpy(&StatusBuf[5], &g_UdpSendRSM.StatusBit, 1);
		memcpy(&StatusBuf[6], &g_UdpSendRSM.MessageLength, 2);
		memcpy(&StatusBuf[8], &g_UdpSendRSM.m_MessageContent.FacilityID, 2);
		memcpy(&StatusBuf[10], &g_UdpSendRSM.m_MessageContent.Reserved1, 2);
		memcpy(&StatusBuf[12], &g_UdpSendRSM.m_MessageContent.DataFrameNumber, 4);
		memcpy(&StatusBuf[16], &g_UdpSendRSM.m_MessageContent.TimeStamp, 8);
		memcpy(&StatusBuf[24], &g_UdpSendRSM.m_MessageContent.LidarLongitude, 4);
		memcpy(&StatusBuf[28], &g_UdpSendRSM.m_MessageContent.LidarLatitude, 4);
		memcpy(&StatusBuf[32], &g_UdpSendRSM.m_MessageContent.LidarAngle, 2);
		memcpy(&StatusBuf[34], &g_UdpSendRSM.m_MessageContent.TrafficParticipantNumber, 1);
		memcpy(&StatusBuf[35], &g_UdpSendRSM.m_MessageContent.Reserved2, 5);
		memcpy(&StatusBuf[40], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantID, 2);
		memcpy(&StatusBuf[42], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantType, 1);
		memcpy(&StatusBuf[43], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantConfidence, 1);
		memcpy(&StatusBuf[44], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantColor, 1);
		memcpy(&StatusBuf[45], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantInformationSource, 1);
		memcpy(&StatusBuf[46], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantReserved1, 2);
		memcpy(&StatusBuf[48], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantLongitude, 4);
		memcpy(&StatusBuf[52], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantLatitude, 4);
		memcpy(&StatusBuf[56], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantAltitude, 2);
		memcpy(&StatusBuf[58], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantSpeed, 2);
		memcpy(&StatusBuf[60], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantCourseAngle, 2);
		memcpy(&StatusBuf[62], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantLength, 2);
		memcpy(&StatusBuf[64], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantWidth, 2);
		memcpy(&StatusBuf[66], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantHeight, 2);
		memcpy(&StatusBuf[68], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantPositionX, 2);
		memcpy(&StatusBuf[70], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantPosotionY, 2);
		memcpy(&StatusBuf[72], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantPosotionZ, 2);
		memcpy(&StatusBuf[74], &g_UdpSendRSM.m_MessageContent.m_ParticipantInformation->ParticipantReserved2, 2);
		memcpy(&StatusBuf[76], &g_UdpSendRSM.m_MessageContent.Reserved3, 4);
		memcpy(&StatusBuf[80], &g_UdpSendRSM.ParityBit, 1);
		memcpy(&StatusBuf[81], &g_UdpSendRSM.EndBit, 1);

	}
	else if (g_IndexNumber == 1)
	{
		g_UdpSendRSM.StartBit = 0xFFFF;
		g_UdpSendRSM.SerialNumber = 0;
		g_UdpSendRSM.MasterNumber = 0xE1;
		g_UdpSendRSM.SlaveNumber = 0x02;
		g_UdpSendRSM.StatusBit = 0;
		//g_UdpSendRSM.MessageLength =
	}
	else
	{
		g_UdpSendRSM.StartBit = 0xFFFF;
		g_UdpSendRSM.SerialNumber = 0;
		g_UdpSendRSM.MasterNumber = 0xE1;
		g_UdpSendRSM.SlaveNumber = 0x02;
		g_UdpSendRSM.StatusBit = 0;
		//g_UdpSendRSM.MessageLength =
	}
#endif
	rsm_socket.SendTo((void *)StatusBuf, 82, 5001, _T("192.168.20.199"));
	return 0;
}
