// shaoleiDlg.h : ͷ�ļ�
//

#pragma once
#include "sl.h"


// CshaoleiDlg �Ի���
class CshaoleiDlg : public CDialog
{
// ����
public:
	CshaoleiDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SHAOLEI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
