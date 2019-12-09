#pragma once
#include "NTGraph.h"

// CGraphDlg ��ȭ �����Դϴ�.

class CGraphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphDlg)

public:
	CGraphDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGraphDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAPH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

	CNTGraph m_ntgraphPos;
	CNTGraph m_ntgraphVel;
	CNTGraph m_ntgraphTorq;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void InitNTGraph();
	virtual BOOL OnInitDialog();
};
