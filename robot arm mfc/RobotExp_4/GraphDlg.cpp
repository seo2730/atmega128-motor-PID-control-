// GraphDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "RobotExp_4.h"
#include "GraphDlg.h"
#include "afxdialogex.h"
#include "SystemMemory.h"
#include "DataType.h"
#include "stdafx.h"

double m_dCnt = 0;
#define WHITE RGB(255, 255, 255)
#define RED RGB(255, 0, 0)
#define BLUE RGB(0, 0, 255)

// CGraphDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGraphDlg, CDialogEx)

CGraphDlg::CGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GRAPH_DIALOG, pParent)
{

}

CGraphDlg::~CGraphDlg()
{
}


void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NTGRAPH_POS, m_ntgraphPos);
	DDX_Control(pDX, IDC_NTGRAPH_VEL, m_ntgraphVel);
	DDX_Control(pDX, IDC_NTGRAPH_TORQ, m_ntgraphTorq);
}

BEGIN_MESSAGE_MAP(CGraphDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGraphDlg 메시지 처리기입니다.

void CGraphDlg::InitNTGraph()
{
	m_ntgraphPos.ClearGraph();
	m_ntgraphVel.ClearGraph();
	m_ntgraphTorq.ClearGraph();

	m_ntgraphPos.SetFrameStyle(0);
	m_ntgraphVel.SetFrameStyle(0);
	m_ntgraphTorq.SetFrameStyle(0);

	m_ntgraphPos.SetPlotAreaColor(WHITE);
	m_ntgraphVel.SetPlotAreaColor(WHITE);
	m_ntgraphTorq.SetPlotAreaColor(WHITE);

	m_ntgraphPos.SetShowGrid(TRUE);
	m_ntgraphVel.SetShowGrid(TRUE);
	m_ntgraphTorq.SetShowGrid(TRUE);

	m_ntgraphPos.SetFormatAxisBottom(_T("%.2f"));
	m_ntgraphVel.SetFormatAxisBottom(_T("%.2f"));
	m_ntgraphTorq.SetFormatAxisBottom(_T("%.2f"));

	m_ntgraphPos.SetCaption(_T("위치"));
	m_ntgraphVel.SetCaption(_T("속도"));
	m_ntgraphTorq.SetCaption(_T("전류"));

	m_ntgraphPos.SetXLabel(_T("Time[s]"));
	m_ntgraphVel.SetXLabel(_T("Time[s]"));
	m_ntgraphTorq.SetXLabel(_T("Time[s]"));

	m_ntgraphPos.SetYLabel(_T("Degree[deg]"));
	m_ntgraphVel.SetYLabel(_T("Velocity[deg/s]"));
	m_ntgraphTorq.SetYLabel(_T("Current[A]"));

	m_ntgraphPos.AddElement();
	m_ntgraphPos.SetElementWidth(3);
	m_ntgraphPos.SetElementLineColor(RED);// Target
	m_ntgraphPos.AddElement();
	m_ntgraphPos.SetElementWidth(3);
	m_ntgraphPos.SetElementLineColor(BLUE); // Current

	m_ntgraphPos.SetRange(0.0, 10.0, -360.0, 360.0);
	m_ntgraphPos.SetYGridNumber(4);

	m_ntgraphVel.AddElement();
	m_ntgraphVel.SetElementWidth(4);
	m_ntgraphVel.SetElementLineColor(RED);// Target
	m_ntgraphVel.AddElement();
	m_ntgraphVel.SetElementWidth(3);
	m_ntgraphVel.SetElementLineColor(BLUE); // Current

	m_ntgraphTorq.AddElement();
	m_ntgraphTorq.SetElementWidth(4);
	m_ntgraphTorq.SetElementLineColor(RED);// Target
	m_ntgraphTorq.AddElement();
	m_ntgraphTorq.SetElementWidth(3);
	m_ntgraphTorq.SetElementLineColor(BLUE); // Current

	SetTimer(1, 100, NULL);

}





void CGraphDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_dCnt += 0.1;
	ControlData_t motor_data_target;
	ControlData_t motor_data_current;
	GET_SYSTEM_MEMORY("Comm1Work_Controller_Target", motor_data_target);
	GET_SYSTEM_MEMORY("Comm1Work_Controller_Current", motor_data_current);
	
	if (m_dCnt >= 10.0)
	{
		m_ntgraphPos.SetRange(m_dCnt - 10.0, m_dCnt, -90.0, 360.0);
		m_ntgraphVel.SetRange(m_dCnt - 10.0, m_dCnt, -100.0, 100.0); //범위는 알아서
		m_ntgraphTorq.SetRange(m_dCnt - 10.0, m_dCnt, -2.0, 2.0);
	}
	m_ntgraphPos.PlotXY(m_dCnt, motor_data_target.position * RAD2DEG, 1);
	m_ntgraphPos.PlotXY(m_dCnt, motor_data_current.position * RAD2DEG, 2);

	m_ntgraphVel.PlotXY(m_dCnt, motor_data_target.velocity * RAD2DEG, 1);
	m_ntgraphVel.PlotXY(m_dCnt, motor_data_current.velocity * RAD2DEG, 2);

	m_ntgraphTorq .PlotXY(m_dCnt, motor_data_target.current, 1); //*0.0683
	m_ntgraphTorq.PlotXY(m_dCnt, motor_data_current.current, 2);


	CDialogEx::OnTimer(nIDEvent);
}


BOOL CGraphDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitNTGraph();

	return TRUE;  // return TRUE unless you set the focus to a control
			   // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}