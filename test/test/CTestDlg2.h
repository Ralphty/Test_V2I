#pragma once

#include "afxwin.h"
#include "afxsock.h"
#include <Winsock2.h>
#include "stdint.h"


// CTestDlg2 dialog

typedef long 				nint64_t;
typedef int 				nint32_t;
typedef short 				nint16_t;
typedef char 				nint8_t;

#define  WM_NET_SENDRSMSignalINFO (WM_USER + 108)

#define SW_EX_16(x) ((short)((((short)(x)&(short)0x00ffU)<<8)|(((short)(x)&(short)0xff00U)>>8)))
#define SW_EX_32(x)  ((((x)&(0x00ffU))<<24)|(((x)&(0xff00U))<<8)|(((x>>16)&(0x00ffU))<<8)|(((x>>16)&(0xff00U))>>8))

class CTestDlg2 : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDlg2)

public:
	CTestDlg2(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTestDlg2();

	CSocket rsm_socket;

	typedef struct _PARTICIPANTINFORMATION
	{
		uint16_t  ParticipantID;
		uint8_t   ParticipantType;
		uint8_t   ParticipantConfidence;
		uint8_t   ParticipantColor;
		uint8_t   ParticipantInformationSource;
		uint8_t   ParticipantReserved1[2];
		nint32_t  ParticipantLongitude;
		nint32_t  ParticipantLatitude;
		nint16_t  ParticipantAltitude;
		uint16_t  ParticipantSpeed;
		uint16_t  ParticipantCourseAngle;
		uint16_t  ParticipantLength;
		uint16_t  ParticipantWidth;
		uint16_t  ParticipantHeight;
		nint16_t  ParticipantPositionX;
		nint16_t  ParticipantPosotionY;
		nint16_t  ParticipantPosotionZ;
		uint8_t   ParticipantReserved2[2];

	}PARTICIPANTINFORMATION;

	typedef struct _MESSAGECONTNET
	{
		uint16_t  FacilityID;
		uint16_t  Reserved1;
		uint32_t  DataFrameNumber;
		uint64_t  TimeStamp;
		uint32_t  LidarLongitude;
		uint32_t  LidarLatitude;
		uint16_t  LidarAngle;
		uint8_t   TrafficParticipantNumber;
		uint8_t   Reserved2[5];
		PARTICIPANTINFORMATION *m_ParticipantInformation;
		uint8_t   Reserved3[4];

	}MESSAGECONTNET;

	typedef struct _UDPSENDRSM
	{
		uint16_t StartBit;
		uint8_t  SerialNumber;
		uint8_t  MasterNumber;
		uint8_t  SlaveNumber;
		uint8_t  StatusBit;
		uint16_t MessageLength;
		MESSAGECONTNET m_MessageContent;
		uint8_t  ParityBit;
		uint8_t  EndBit;
	}UdpSendRSM;

	UdpSendRSM g_UdpSendRSM;
	PARTICIPANTINFORMATION g_ParticipantInformation;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

	static UINT SendDataThread(LPVOID pParam);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	LRESULT CTestDlg2::OnSendData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:

	CComboBox m_CmbNumber;
	CComboBox m_CmbInfoSource;
	CComboBox m_CmbType1;
	CComboBox m_CmbColor1;
	uint8_t   m_TextConfidence1;
	nint32_t  m_TextLongitude1;
	nint32_t  m_TextLatitude1;
	uint32_t  m_TextSpeed1;
	uint32_t  m_TextCourseAngle1;
	nint32_t  m_TextPositionX1;
	nint32_t  m_TextPositionY1;
	nint32_t  m_TextPositionZ1;
	uint32_t  m_TextLidarLongitude;
	uint32_t  m_TextLidarLatitude;

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
