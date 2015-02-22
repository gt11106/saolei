// shaoleiDlg.h : 头文件
//

#pragma once
#include "sl.h"


// CshaoleiDlg 对话框
class CshaoleiDlg : public CDialog
{
// 构造
public:
	CshaoleiDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SHAOLEI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	sl m_sl;
public:
	void Display(CDC& dc);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnBnClickedButton1();
protected:
	virtual void OnCancel();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	int m_time;
public:
	int m_mine;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	int m_rows;
public:
	int m_columns;
public:
	void replace(void);
public:
	afx_msg void OnEnKillfocusRow();
public:
	afx_msg void OnEnKillfocusColumn();
public:
	void check(void);
};
