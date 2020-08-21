#pragma once

#include "afxwin.h"
#include "stdint.h"
#include "afxsock.h"
#include <Winsock2.h>

// CTestDlg1 dialog

typedef long            nint64_t;
typedef int             nint32_t;
typedef short           nint16_t;
typedef char            nint8_t;

#define  WM_NET_SENDSignalINFO (WM_USER + 101)
#define	 WM_NET_Linsten (WM_USER + 102)
#define  WM_NET_RECEIVE (WM_USER + 103)

#define SW_16(x) ((short)((((short)(x)&(short)0x00ffU)<<8)|(((short)(x)&(short)0xff00U)>>8)))

class CTestDlg1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDlg1)

public:
	CTestDlg1(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTestDlg1();

	CSocket send_socket;
	CSocket receive_socket;

	UINT m_GreenTime;
	UINT m_YellowTime;
	UINT m_RedTime;

	typedef struct _PHASESTATUS
	{
		uint16_t Head;
		uint8_t Version;
		uint8_t FrameType;
		uint16_t Length;

		char SeqNo[16];
		uint8_t PhaseLength;
		uint8_t PhaseId;
		uint8_t MovementsLength;
		char Movements[19];
		uint16_t Green;
		uint16_t Yellow;
		uint16_t Red;
		uint8_t Status;
		uint16_t TimeLeft;
	}PHASESTATUS;

	PHASESTATUS PhaseStatus, Register;

	typedef struct _PHASESTATE
	{
		uint8_t PhaseId;
		uint16_t Green;
		uint16_t Yellow;
		uint16_t Red;
		uint8_t Status;
		uint16_t TimeLeft;

	} PhaseState;

	typedef struct _SPATMSG
	{
		uint16_t Head;
		uint8_t FunCode;
		uint16_t MsgLength;
		uint16_t Cycle;
		uint8_t PhaseNum;
		uint8_t Mehtod;
		PhaseState m_Phase;
		uint64_t Reserved;
		uint8_t End;
	}SPaTMSG;

	SPaTMSG g_SPaTMsg;

	typedef struct _MAPDATA
	{
		uint16_t Head;
		uint32_t Node_Longitude;
		uint32_t Node_Latitude;
		uint32_t  RegionID;
		uint32_t LimitSpeed;
	}MapData;

	MapData g_MapData;

	typedef struct _RSIDATA
	{
		uint16_t Head;
		uint32_t Signage_Longitude;
		uint32_t Signage_Latitude;
		uint32_t  Warning_Type;
	}RsiData;

	RsiData g_RsiData;

	void CTestDlg1::StatusChange(uint16_t Green, uint16_t Yellow, uint16_t Red, uint8_t Time, uint8_t * Status, uint16_t *TimeLeft);
	void CTestDlg1::PictureLoad(uint8_t status);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

	CString	m_ServerIP;
	int		m_ServerPort;
	static UINT Thread_Linstening(LPVOID pParam);
	//static UINT Thread_Receive(LPVOID pParam);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg LRESULT OnSendData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Receive_Handle(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CStatic m_StatusLight;
	CString m_TimeLeft;
	CFile file;
	CComboBox m_CmbDir;
	CWinThread *pthread_Senddata1;
	CWinThread *pthread_Senddata2;
	//	afx_msg void OnCbnSetfocusCombo1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeCombo1();
};
