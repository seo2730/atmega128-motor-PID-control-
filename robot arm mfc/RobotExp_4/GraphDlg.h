#pragma once
#include "NTGraph.h"

// CGraphDlg 대화 상자입니다.

class CGraphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphDlg)

public:
	CGraphDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGraphDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAPH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	CNTGraph m_ntgraphPos;
	CNTGraph m_ntgraphVel;
	CNTGraph m_ntgraphTorq;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void InitNTGraph();
	virtual BOOL OnInitDialog();
};
