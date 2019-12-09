
// RobotExp_4Dlg.cpp : 구현 파일
//
#include <complex>

#include "stdafx.h"
#include "RobotExp_4.h"
#include "RobotExp_4Dlg.h"
#include "afxdialogex.h"
#include "DataType.h"
#include <math.h>
#include "SystemMemory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRobotExp_4Dlg 대화 상자



CRobotExp_4Dlg::CRobotExp_4Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ROBOTEXP_4_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRobotExp_4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_comboPort);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_comboBaud);
	DDX_Control(pDX, IDC_CHECK_OPEN, m_checkOpen);
	DDX_Control(pDX, IDC_EDIT_SEND, m_editSend);
	DDX_Control(pDX, IDC_EDIT_RECV, m_editRecv);
	DDX_Control(pDX, IDC_EDIT_TAR_X, m_editTarX);
	DDX_Control(pDX, IDC_EDIT_CUR_X, m_editCurX);
	DDX_Control(pDX, IDC_EDIT_TAR_Y, m_editTarY);
	DDX_Control(pDX, IDC_EDIT_CUR_Y, m_editCurY);
	DDX_Control(pDX, IDC_EDIT_TAR_Z, m_editTarZ);
	DDX_Control(pDX, IDC_EDIT_CUR_Z, m_editCurZ);
	DDX_Control(pDX, IDC_EDIT_TAR_POS_1, m_editTarPos1);
	DDX_Control(pDX, IDC_EDIT_CUR_POS_1, m_editCurPos1);
	DDX_Control(pDX, IDC_EDIT_TAR_POS_2, m_editTarPos2);
	DDX_Control(pDX, IDC_EDIT_CUR_POS_2, m_editCurPos2);
	DDX_Control(pDX, IDC_EDIT_TAR_VEL, m_editTarVel);
	DDX_Control(pDX, IDC_EDIT_CUR_VEL, m_editCurVel);
	DDX_Control(pDX, IDC_EDIT_TAR_TORQ, m_editTarTorq);
	DDX_Control(pDX, IDC_EDIT_CUR_TORQ, m_editCurTorq);
}

BEGIN_MESSAGE_MAP(CRobotExp_4Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_DROPDOWN(IDC_COMBO_PORT, &CRobotExp_4Dlg::OnCbnDropdownComboPort)
	ON_BN_CLICKED(IDC_CHECK_OPEN, &CRobotExp_4Dlg::OnBnClickedCheckOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CRobotExp_4Dlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CRobotExp_4Dlg::OnBnClickedButtonSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CRobotExp_4Dlg::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_FORWARD, &CRobotExp_4Dlg::OnBnClickedButtonForward)
	ON_BN_CLICKED(IDC_BUTTON_INVERSE, &CRobotExp_4Dlg::OnBnClickedButtonInverse)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_GRAPH, &CRobotExp_4Dlg::OnBnClickedButtonGraph)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CRobotExp_4Dlg::OnBnClickedButtonSet)
END_MESSAGE_MAP()


// CRobotExp_4Dlg 메시지 처리기

BOOL CRobotExp_4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetTimer(1001, 33, NULL);

	_commWorker.SetPeriod(0.01);
	_commWorker.SetWork(CreateWork <CCommWork>("Comm1Work"));

	m_editTarPos1.SetWindowText("0");
	m_editTarPos2.SetWindowText("0");
	m_editTarVel.SetWindowText("0");
	m_editTarTorq.SetWindowText("0");
	m_editTarX.SetWindowText("0");
	m_editTarY.SetWindowText("0");
	m_editTarZ.SetWindowText("0");

	m_pGraphDlg = new CGraphDlg;
	m_pGraphDlg->Create(IDD_GRAPH_DIALOG);




	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CRobotExp_4Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CRobotExp_4Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CRobotExp_4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRobotExp_4Dlg::SolveForwardKinematics(double dAngle, double dAngle2, double* pdPos)
{
	double L1 = 1.0;
	double L2 = 0.5; 
	


	pdPos[0] = L1 * sin(dAngle * DEG2RAD) + L2 * sin(dAngle * DEG2RAD + dAngle2 * DEG2RAD);
	pdPos[1] = -L1 * cos(dAngle * DEG2RAD) - L2 * cos(dAngle * DEG2RAD + dAngle2 * DEG2RAD);
	pdPos[2] = 0.0;
}






void CRobotExp_4Dlg::SolveInverseKinematics(double dX, double dY, double dZ, double* pdAngle)
{
	double L1 = 1.0;
	double L2 = 0.5;

	double cos2 = (dX * dX + dY * dY - (L1 * L1 + L2 * L2)) / (2.0 * L1 * L2);
	double sin2 = sqrt(1 - cos2 * cos2);
	double cos1 = (L2 * sin2 * dX - (L1 + L2 * cos2) * dY) / ((L1 + L2 * cos2) * (L1 + L2 * cos2) + (L2 * sin2) * (L2 * sin2));
	double sin1 = ((L1 + L2 * cos2) * dX + L2 * sin2 * dY) / ((L1 + L2 * cos2) * (L1 + L2 * cos2) + (L2 * sin2) * (L2 * sin2));

	pdAngle[0] = atan2(sin1, cos1);
	pdAngle[1] = atan2(sin2, cos2);

}


void CRobotExp_4Dlg::OnCbnDropdownComboPort()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDeviceListReader reader;
	std::vector<std::string> list;

	//Combo Box 초기화
	m_comboPort.ResetContent();

	//컴퓨터에 연결된 시리얼 장비 리스트를 얻어옴
	reader.UpdateDeviceList("SERIALCOMM");
	reader.GetDeviceList(list);

	//Combo Box에 list 추가
	for (int i = 0; i < list.size(); i++) {
		m_comboPort.AddString(list[i].c_str());
	}
}


void CRobotExp_4Dlg::OnBnClickedCheckOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//버튼의 체크 여부 확인
	if (m_checkOpen.GetCheck()) {
		//체크 된 경우
		//선택된 포트이름과 통신속도를 가져옴
		CString port, baud;
		m_comboPort.GetLBText(m_comboPort.GetCurSel(), port);
		m_comboBaud.GetLBText(m_comboBaud.GetCurSel(), baud);
		int nTmp = atoi(baud.GetBuffer());

		//포트 열기 시도
		if (((CCommWork*)_commWorker.GetWork())->OpenPort(port.GetBuffer(), nTmp)) {


			//성공하면 버튼의 텍스트를 Close로 변경
			_commWorker.StartWork();
			m_checkOpen.SetWindowText("Close");
		}
		else {
			//실패 시 에러 메세지를 띄우고 초기화
			AfxMessageBox("Can`t open port");
			m_checkOpen.SetCheck(false);
		}
	}
	else {
		//체크 해제된 경우
		//포트를 닫고 버튼 텍스트를 Open으로 변경
		_commWorker.StopWork();
		((CCommWork*)_commWorker.GetWork())->ClosePort();
		m_checkOpen.SetWindowText("Open");
	}
}

void CRobotExp_4Dlg::OnBnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	//포트가 열린지 확인
	if (m_comm.isOpen()) {

		//문자열을 가져옴
		CString str;
		m_editSend.GetWindowText(str);

		//문자열 전송
		int size = m_comm.Write(str.GetBuffer(), str.GetLength());

		//Edit Control 문자열 제거
		m_editSend.SetWindowText("");
	}
	
}


void CRobotExp_4Dlg::OnBnClickedButtonClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_editRecv.SetWindowText("");
}


void CRobotExp_4Dlg::OnBnClickedButtonInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//ODE 및 실제 Motor의 Setting을 초기화
	m_editTarX.SetWindowText("0");
	m_editTarY.SetWindowText("0");
	m_editTarZ.SetWindowText("0");

	m_editTarPos1.SetWindowText("0");
	m_editTarPos2.SetWindowText("0");

	m_editTarVel.SetWindowText("10");
	m_editTarTorq.SetWindowText("0.1");

	ControlData_t motor_data;
	DataType_t ode_data;

	GET_SYSTEM_MEMORY("JointData", ode_data);

	ode_data.Q_tar[0] = ode_data.Q_tar[1] = 0.;
	SET_SYSTEM_MEMORY("JointData", ode_data);

	motor_data.position = 0.;
	motor_data.velocity = 0. * DEG2RAD;
	motor_data.current = 0. / 0.0683;




	GET_SYSTEM_MEMORY("JointData", ode_data);

	CString str;
	m_editTarPos1.GetWindowText(str);
	ode_data.Q_tar[0] = atof(str.GetBuffer()) * DEG2RAD;

	m_editTarPos2.GetWindowText(str);
	ode_data.Q_tar[1] = atof(str.GetBuffer()) * DEG2RAD;


	m_editTarPos1.GetWindowText(str);
	motor_data.position = atof(str.GetBuffer()) * DEG2RAD;

	m_editTarVel.GetWindowText(str);
	motor_data.velocity = atof(str.GetBuffer()) * DEG2RAD;

	m_editTarTorq.GetWindowText(str);
	motor_data.current = atof(str.GetBuffer());// / 0.0683; //Torque constant

	SET_SYSTEM_MEMORY("JointData", ode_data);
	SET_SYSTEM_MEMORY("Comm1Work_Controller_Target", motor_data);

}


void CRobotExp_4Dlg::OnBnClickedButtonForward()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//Target Pos 값을 가져와 Forward Kinematics 푼 후 Current에 출력 및 해당 값으로 Motor 제어
	char cTmp[10];
	double dTmp[2];
	m_editTarPos1.GetWindowText(cTmp, 10);
	dTmp[0] = atof(cTmp);
	m_editTarPos2.GetWindowText(cTmp, 10);
	dTmp[1] = atof(cTmp);

	DataType_t jointData;
	GET_SYSTEM_MEMORY("JointData", jointData);

	//jointData.Q_tar[0] = dTmp[0] * DEG2RAD;
	jointData.Q_tar[1] = dTmp[1] * DEG2RAD;

	SET_SYSTEM_MEMORY("JointData", jointData);

	double dPos[3] = { 0, 0, 0 };

	SolveForwardKinematics(dTmp[0], dTmp[1], dPos);

	char pszTmp[512];
	sprintf_s(pszTmp, "%.2lf", dPos[0]);
	m_editTarX.SetWindowText(pszTmp);
	sprintf_s(pszTmp, "%.2lf", dPos[1]);
	m_editTarY.SetWindowText(pszTmp);
	sprintf_s(pszTmp, "%.2lf", dPos[2]);
	m_editTarZ.SetWindowText(pszTmp);

	ControlData_t motor_data;
	DataType_t ode_data;

	GET_SYSTEM_MEMORY("JointData", ode_data);

	CString str;
	m_editTarPos1.GetWindowText(str);
	ode_data.Q_tar[0] = atof(str.GetBuffer()) * DEG2RAD;

	m_editTarPos2.GetWindowText(str);
	ode_data.Q_tar[1] = atof(str.GetBuffer()) * DEG2RAD;


	m_editTarPos1.GetWindowText(str);
	motor_data.position = atof(str.GetBuffer()) * DEG2RAD;

	m_editTarVel.GetWindowText(str);
	motor_data.velocity = atof(str.GetBuffer()) * DEG2RAD;

	m_editTarTorq.GetWindowText(str);
	motor_data.current = atof(str.GetBuffer());// / 0.0683; //Torque constant

	SET_SYSTEM_MEMORY("JointData", ode_data);
	SET_SYSTEM_MEMORY("Comm1Work_Controller_Target", motor_data);
}


void CRobotExp_4Dlg::OnBnClickedButtonInverse()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//End Effector Position의 Target값을 가져와 Inverse Kinematics 푼 후 Current에 출력 및 해당 값으로 Motor 제어
	char cTmp[10];
	double dTmp[3];
	m_editTarX.GetWindowText(cTmp, 10);
	dTmp[0] = atof(cTmp);
	m_editTarY.GetWindowText(cTmp, 10);
	dTmp[1] = atof(cTmp);
	m_editTarZ.GetWindowText(cTmp, 10);
	dTmp[2] = atof(cTmp);

	double dAngle[2] = { 0, 0 };
	SolveInverseKinematics(dTmp[0], dTmp[1], dTmp[2], dAngle);

	char pszTmp[512];
	sprintf_s(pszTmp, "%.2lf", dAngle[0] * RAD2DEG);
	m_editTarPos1.SetWindowText(pszTmp);
	sprintf_s(pszTmp, "%.2lf", dAngle[1] * RAD2DEG);
	m_editTarPos2.SetWindowText(pszTmp);

	DataType_t jointData;
	GET_SYSTEM_MEMORY("JointData", jointData);

	//jointData.Q_tar[0] = dAngle[0];
	jointData.Q_tar[1] = dAngle[1];

	SET_SYSTEM_MEMORY("JointData", jointData);

	ControlData_t motor_data;
	DataType_t ode_data;

	GET_SYSTEM_MEMORY("JointData", ode_data);

	CString str;
	m_editTarPos1.GetWindowText(str);
	ode_data.Q_tar[0] = atof(str.GetBuffer()) * DEG2RAD;

	m_editTarPos2.GetWindowText(str);
	ode_data.Q_tar[1] = atof(str.GetBuffer()) * DEG2RAD;


	m_editTarPos1.GetWindowText(str);
	motor_data.position = atof(str.GetBuffer()) * DEG2RAD;

	m_editTarVel.GetWindowText(str);
	motor_data.velocity = atof(str.GetBuffer()) * DEG2RAD;

	m_editTarTorq.GetWindowText(str);
	motor_data.current = atof(str.GetBuffer());// / 0.0683; //Torque constant

	SET_SYSTEM_MEMORY("JointData", ode_data);
	SET_SYSTEM_MEMORY("Comm1Work_Controller_Target", motor_data);

}

void CRobotExp_4Dlg::OnBnClickedButtonSet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ControlData_t motor_data;
	DataType_t ode_data;

	GET_SYSTEM_MEMORY("JointData", ode_data);

	CString str;
	//m_editTarPos1.GetWindowText(str);
	//ode_data.Q_tar[0] = atof(str.GetBuffer()) * DEG2RAD;

	m_editTarPos2.GetWindowText(str);
	ode_data.Q_tar[1] = atof(str.GetBuffer()) * DEG2RAD;

	
	m_editTarPos1.GetWindowText(str);
	motor_data.position = atof(str.GetBuffer()) * DEG2RAD;

	m_editTarVel.GetWindowText(str);
	motor_data.velocity = atof(str.GetBuffer()) * DEG2RAD;

	m_editTarTorq.GetWindowText(str);
	motor_data.current = atof(str.GetBuffer());// / 0.0683; //Torque constant

	SET_SYSTEM_MEMORY("JointData", ode_data);
	SET_SYSTEM_MEMORY("Comm1Work_Controller_Target", motor_data);
}

void CRobotExp_4Dlg::OnBnClickedButtonGraph()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bCheck = m_pGraphDlg->IsWindowVisible();
	if (bCheck) m_pGraphDlg->ShowWindow(SW_HIDE);
	else m_pGraphDlg->ShowWindow(SW_SHOW);
}





void CRobotExp_4Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	//포트가 열렸는지 확인
	if (m_comm.isOpen()) {
		CString str;
		char buf[4096] = { 0, };

		//현재 Edit_Recv 값을 가져옴
		m_editRecv.GetWindowText(str);

		//포트로부터 받은 데이터를 가져옴
		int size = m_comm.Read(buf, 4096);

		//Edit_Recv문자열에 데이터를 추가함
		str += buf;

		//Edit_Recv 텍스트 업데이트
		m_editRecv.SetWindowText(str);
	}
	


	//ODE연동
	DataType_t jointData;
	GET_SYSTEM_MEMORY("JointData", jointData);

	char pszTmp[512];
	sprintf_s(pszTmp, "%.2lf", jointData.Q_cur[0] * RAD2DEG);
	m_editCurPos1.SetWindowText(pszTmp);
	sprintf_s(pszTmp, "%.2lf", jointData.Q_cur[1] * RAD2DEG);
	m_editCurPos2.SetWindowText(pszTmp);

	double dTmp[3] = { 0, 0, 0 };
	SolveForwardKinematics(jointData.Q_cur[0] * RAD2DEG, jointData.Q_cur[1] * RAD2DEG, dTmp);
	sprintf_s(pszTmp, "%.2lf", dTmp[0]);
	m_editCurX.SetWindowText(pszTmp);
	sprintf_s(pszTmp, "%.2lf", dTmp[1]);
	m_editCurY.SetWindowText(pszTmp);
	sprintf_s(pszTmp, "%.2lf", dTmp[2]);
	m_editCurZ.SetWindowText(pszTmp);




	ControlData_t motor_data;
	DataType_t ode_data;
	//forward누르면 통신으로 보내는건 포지션0번, 벨로시티, 토크값을 전송
	//inverse누르면 통신으로 보내는건 포지션0번, 벨로시티, 토크값을 전송
	GET_SYSTEM_MEMORY("JointData", ode_data);
	GET_SYSTEM_MEMORY("Comm1Work_Controller_Current", motor_data); //통신을통해받아온 모터데이터를 ODE데이터에 타겟으로 넣어줘야한다.
																		//통신으로 읽어온값을 ODE로 연동해줘야한다.
	CString str;

	//ODE
	str.Format("%.4f", ode_data.Q_cur[1] * RAD2DEG); 
	m_editCurPos2.SetWindowText(str);

	//Motor
	str.Format("%.4f", motor_data.position * RAD2DEG);
	m_editCurPos1.SetWindowText(str);
	str.Format("%.4f", motor_data.velocity * RAD2DEG);
	m_editCurVel.SetWindowText(str);
	str.Format("%.4f", motor_data.current); //torque constant
	m_editCurTorq.SetWindowText(str);

	//Forward Kinematics //이부분 없애면 잘되긴함 SET버튼추가해보면 될듯? 히읗
	double Pcur[3] = { 0, };
	SolveForwardKinematics(ode_data.Q_cur[0], ode_data.Q_cur[1], Pcur);

	str.Format("%.4f", Pcur[0]);
	m_editCurX.SetWindowText(str);
	str.Format("%.4f", Pcur[1]);
	m_editCurY.SetWindowText(str);
	str.Format("%.4f", Pcur[2]);
	m_editCurZ.SetWindowText(str);

	m_editCurPos1.GetWindowText(str);
	ode_data.Q_tar[0] = atof(str.GetBuffer()) * DEG2RAD;
	SET_SYSTEM_MEMORY("JointData", ode_data);

	CDialogEx::OnTimer(nIDEvent);
	 
}
void CRobotExp_4Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	delete m_pGraphDlg;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}



