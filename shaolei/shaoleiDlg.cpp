// shaoleiDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "shaolei.h"
#include "shaoleiDlg.h"
#include "sl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CshaoleiDlg �Ի���




CshaoleiDlg::CshaoleiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CshaoleiDlg::IDD, pParent)
	, m_time(0)
	, m_mine(0)
	, m_rows(9)
	, m_columns(9)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CshaoleiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CshaoleiDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, &CshaoleiDlg::OnBnClickedButton1)
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_ROW, &CshaoleiDlg::OnEnKillfocusRow)
	ON_EN_KILLFOCUS(IDC_COLUMN, &CshaoleiDlg::OnEnKillfocusColumn)
END_MESSAGE_MAP()


// CshaoleiDlg ��Ϣ�������

BOOL CshaoleiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	ModifyStyle(0, WS_SYSMENU);

	m_mine = m_rows * m_columns / 6;
	m_sl.init_blocks(m_rows,m_columns, m_mine);

	GetDlgItem(IDC_STATIC_TM)->SetWindowText(_T("000"));

	CString text;
	text.Format(_T("%03d"), m_mine);
	GetDlgItem(IDC_STATIC_MINE)->SetWindowText(text);

	CFont font;
	font.CreatePointFont(110, _T("Arial"));
	GetDlgItem(IDC_STATIC_TM)->SetFont(&font);
	GetDlgItem(IDC_STATIC_MINE)->SetFont(&font);

	CString tx;
	tx.Format(_T("%d"), m_rows);
	GetDlgItem(IDC_ROW)->SetWindowText(tx);
	tx.Format(_T("%d"), m_columns);
	GetDlgItem(IDC_COLUMN)->SetWindowText(tx);
	
	replace();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CshaoleiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC sdc(this);
		CDC dc;
		dc.CreateCompatibleDC(&sdc);
		CRect cr;
		GetClientRect(&cr);
		CBitmap btm;
		btm.CreateCompatibleBitmap(&sdc, cr.Width(),cr.Height());
		dc.SelectObject(&btm);
		dc.FillRect(&cr, &CBrush(RGB(210,210,210)));
		vector<vector<block> >::iterator iv = m_sl.block_matrix.begin();
		vector<block>::iterator iiv;
		for(; iv != m_sl.block_matrix.end(); ++iv)
			for(iiv = iv->begin(); iiv != iv->end(); ++iiv)
			{
				if((iiv->get_block_state() & block::STATE_COVER) != 0)
				{
					rect r = iiv->get_block_location();
					CRect rt(r.top_left.x,r.top_left.y,r.bottom_right.x,r.bottom_right.y);
					rt.right += 1;
					rt.bottom += 1;
					dc.Draw3dRect(&rt, RGB(210,210,210,), RGB(150,150,150));
					rt.DeflateRect(1,1);
					dc.Draw3dRect(&rt, RGB(210,210,210,), RGB(150,150,150));

					if((iiv->get_block_state() & block::STATE_FLAG) != 0)
					{
						rect r = iiv->get_block_location();
						CRect rt(r.top_left.x,r.top_left.y,r.bottom_right.x,r.bottom_right.y);
						rt.right += 2;
						rt.bottom += 2;
						rt.DeflateRect(7,7);
						CBrush br(RGB(255,0,150));
						CBrush* poldbrush = dc.SelectObject(&br);
						CPen* poldpen = (CPen*)dc.SelectStockObject(NULL_PEN);
						CPen pen(PS_SOLID, 1, RGB(255,0,0));
						poldpen = (CPen*)dc.SelectObject(&pen);
						POINT pts[4] = {rt.left, rt.top, rt.right, rt.top + rt.Height()/2, rt.left, rt.bottom, rt.left, rt.top};
						dc.Polygon(pts, 4);
						dc.SelectObject(poldpen);
						dc.SelectObject(poldbrush);
					}
					continue;
				}

				if((iiv->get_block_state() & block::STATE_OPEN) != 0)
				{
					rect r = iiv->get_block_location();
					CRect rt(r.top_left.x,r.top_left.y,r.bottom_right.x,r.bottom_right.y);
					rt.right += 1;
					rt.bottom += 1;
					CPen pen(PS_SOLID, 1, RGB(150,150,150));
					CPen* poldpen = dc.SelectObject(&pen);
					CBrush* poldbrush = (CBrush*)dc.SelectStockObject(WHITE_BRUSH);
					dc.Rectangle(&rt);
					dc.SelectObject(poldbrush);
					dc.SelectObject(poldpen);
				}


				if((iiv->get_block_state() & block::STATE_YES) != 0)
				{
					rect r = iiv->get_block_location();
					CRect rt(r.top_left.x,r.top_left.y,r.bottom_right.x,r.bottom_right.y);
					rt.right += 2;
					rt.bottom += 2;
					rt.DeflateRect(4,4);
					CBrush br(RGB(70,70,70));
					CBrush* poldbrush = dc.SelectObject(&br);
					CPen* poldpen = (CPen*)dc.SelectStockObject(NULL_PEN);
					dc.Ellipse(&rt);
					dc.SelectObject(poldpen);
					dc.SelectObject(poldbrush);
				}

				if((iiv->get_block_state() & block::STATE_BOOM) != 0)
				{
					rect r = iiv->get_block_location();
					CRect rt(r.top_left.x,r.top_left.y,r.bottom_right.x,r.bottom_right.y);
					rt.right += 2;
					rt.bottom += 2;
					rt.DeflateRect(4,4);
					CBrush br(RGB(70,70,70));
					CBrush* poldbrush = dc.SelectObject(&br);
					CPen* poldpen = (CPen*)dc.SelectStockObject(NULL_PEN);
					dc.Ellipse(&rt);
					CPen pen(PS_SOLID, 1, RGB(255,0,0));
					poldpen = (CPen*)dc.SelectObject(&pen);
					dc.MoveTo(rt.TopLeft());
					dc.LineTo(rt.BottomRight());
					dc.MoveTo(rt.right, rt.top);
					dc.LineTo(rt.left, rt.bottom);
					dc.SelectObject(poldpen);
					dc.SelectObject(poldbrush);
				}

				if((iiv->get_block_state() & block::STATE_NUM) != 0)
				{
					rect r = iiv->get_block_location();
					CRect rt(r.top_left.x,r.top_left.y,r.bottom_right.x,r.bottom_right.y);
					rt.DeflateRect(3,3);
					CString text;
					text.Format(_T("%d"), iiv->get_number());
					dc.SetBkMode(TRANSPARENT);

					COLORREF cl;
					switch(iiv->get_number())
					{
					case 1:
						cl = RGB(0,0,255);
						break;
					case 2:
						cl = RGB(0,180,0);
						break;
					case 3:
						cl = RGB(255,100,0);
						break;
					case 4:
						cl = RGB(20,20,20);
						break;
					case 5:
						cl = RGB(255,0,0);
						break;
					case 6:
						cl = RGB(255,0,100);
						break;
					case 7: case 8:
						cl = RGB(255,0,255);
						break;
					}
					dc.SetTextColor(cl);

					dc.DrawText(text, &rt, DT_CENTER | DT_VCENTER| DT_SINGLELINE);
				}
			
			}

			
		sdc.BitBlt(0,0,cr.Width(),cr.Height(), &dc,0,0,SRCCOPY);

		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CshaoleiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CshaoleiDlg::Display(CDC& dc)
{
}

void CshaoleiDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::point pt(point.x, point.y);

	::point ij;
	if((MK_RBUTTON & nFlags) != 0 && m_sl.get_current_block(pt, ij))
	{
		m_sl.right_left_clicked(ij);
		rect rt = m_sl.get_rect();
		CRect rrt(rt.top_left.x,rt.top_left.y,rt.bottom_right.x,rt.bottom_right.y);
		InvalidateRect(&rrt);
		check();
		return;
	}

	::point nouse;
	if(m_sl.get_current_block(pt, nouse))
	{
		if(m_sl.get_state() == 1)
			SetTimer(1, 1000, NULL);
	}

	if(m_sl.open_block(pt))
	{
		rect rt = m_sl.get_rect();
		CRect rrt(rt.top_left.x,rt.top_left.y,rt.bottom_right.x,rt.bottom_right.y);
		InvalidateRect(&rrt);

		check();
	}


	//CDialog::OnLButtonDown(nFlags, point);
}


void CshaoleiDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::point pt(point.x, point.y);

	::point ij;
	if((MK_LBUTTON & nFlags) != 0 && m_sl.get_current_block(pt, ij))
	{
		m_sl.right_left_clicked(ij);
		rect rt = m_sl.get_rect();
		CRect rrt(rt.top_left.x,rt.top_left.y,rt.bottom_right.x,rt.bottom_right.y);
		InvalidateRect(&rrt);
		check();
		return;
	}

	int res = m_sl.set_flag(pt);
	if(res != 0)
	{
		rect rt = m_sl.get_rect();
		CRect rrt(rt.top_left.x,rt.top_left.y,rt.bottom_right.x,rt.bottom_right.y);
		InvalidateRect(&rrt);
		if(res == -1)
			++m_mine;
		else if(res == 1)
			--m_mine;
		CString text;
		text.Format(_T("%03d"), m_mine);
		GetDlgItem(IDC_STATIC_MINE)->SetWindowText(text);

		check();
	}

//	CDialog::OnRButtonDown(nFlags, point);
}

void CshaoleiDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_rows = GetDlgItemInt(IDC_ROW);
	m_columns = GetDlgItemInt(IDC_COLUMN);

	m_time = 0;
	GetDlgItem(IDC_STATIC_TM)->SetWindowText(_T("000"));
	m_mine = m_rows * m_columns / 6;
	CString text;
	text.Format(_T("%03d"), m_mine);
	GetDlgItem(IDC_STATIC_MINE)->SetWindowText(text);

	m_sl.freshen(m_rows, m_columns, m_mine);
	replace();
	Invalidate();
}

void CshaoleiDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	CWnd::DestroyWindow();
	//CDialog::OnCancel();
}

BOOL CshaoleiDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
//	cs.style = WS_OVERLAPPED;// | WS_CLIPCHILDREN;

	return CDialog::PreCreateWindow(cs);
}

BOOL CshaoleiDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return true;
//	return CDialog::OnEraseBkgnd(pDC);
}

void CshaoleiDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	++m_time;
	CString text;
	text.Format(_T("%03d"), m_time);
	GetDlgItem(IDC_STATIC_TM)->SetWindowText(text);
//	CDialog::OnTimer(nIDEvent);
}

HBRUSH CshaoleiDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(CTLCOLOR_STATIC == nCtlColor)
	{
		pDC->SetBkMode(TRANSPARENT);
		if(pWnd->GetDlgCtrlID() == IDC_STATIC_TM || pWnd->GetDlgCtrlID() == IDC_STATIC_MINE)
			pDC->SetTextColor(RGB(255,0,0));

		CBrush br(RGB(210,210,210));
		hbr = (HBRUSH)br.m_hObject;
		br.Detach();
	}

	return hbr;
}

void CshaoleiDlg::replace(void)
{
	rect rt = m_sl.get_rect();
	CRect ret(rt.top_left.x,rt.top_left.y, rt.bottom_right.x + 100,rt.bottom_right.y);
	CalcWindowRect(&ret);
	ClientToScreen(&ret);
	MoveWindow(&ret);
	GetClientRect(&ret);
	CRect rr;
	GetDlgItem(IDC_BUTTON1)->GetWindowRect(&rr);
	ScreenToClient(&rr);
	int w = rr.Width();
	int a = ret.right - w - 10;
	rr.left = a;
	rr.right = rr.left + w;
	GetDlgItem(IDC_BUTTON1)->MoveWindow(&rr);
//
	GetDlgItem(IDC_STATIC_ROW)->GetWindowRect(&rr);
	ScreenToClient(&rr);
	w = rr.Width();
	a = ret.right - w - 60;
	rr.left = a;
	rr.right = rr.left + w;
	GetDlgItem(IDC_STATIC_ROW)->MoveWindow(&rr);

	GetDlgItem(IDC_STATIC_COL)->GetWindowRect(&rr);
	ScreenToClient(&rr);
	w = rr.Width();
	a = ret.right - w - 60;
	rr.left = a;
	rr.right = rr.left + w;
	GetDlgItem(IDC_STATIC_COL)->MoveWindow(&rr);

	GetDlgItem(IDC_ROW)->GetWindowRect(&rr);
	ScreenToClient(&rr);
	w = rr.Width();
	a = ret.right - w - 20;
	rr.left = a;
	rr.right = rr.left + w;
	GetDlgItem(IDC_ROW)->MoveWindow(&rr);

	GetDlgItem(IDC_COLUMN)->GetWindowRect(&rr);
	ScreenToClient(&rr);
	w = rr.Width();
	a = ret.right - w - 20;
	rr.left = a;
	rr.right = rr.left + w;
	GetDlgItem(IDC_COLUMN)->MoveWindow(&rr);
//

	GetDlgItem(IDC_STATIC_TM)->GetWindowRect(&rr);
	ScreenToClient(&rr);
	w = rr.Width();
	a = ret.right - w - 30;
	rr.left = a;
	rr.right = rr.left + w;
	GetDlgItem(IDC_STATIC_TM)->MoveWindow(&rr);

	GetDlgItem(IDC_STATIC_MINE)->GetWindowRect(&rr);
	ScreenToClient(&rr);
	w = rr.Width();
	a = ret.right - w - 30;
	rr.left = a;
	rr.right = rr.left + w;
	GetDlgItem(IDC_STATIC_MINE)->MoveWindow(&rr);

	GetDlgItem(IDC_STATIC_QQ)->GetWindowRect(&rr);
	ScreenToClient(&rr);
	w = rr.Width();
	a = ret.right - w - 15;
	rr.left = a;
	rr.right = rr.left + w;
	rr.top = 570;
	rr.bottom = 600;
	GetDlgItem(IDC_STATIC_QQ)->MoveWindow(&rr);
}

void CshaoleiDlg::OnEnKillfocusRow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int row = GetDlgItemInt(IDC_ROW);
	if(row < 7 || row >30)
	{
		MessageBox(_T("������ֻ����7-30��"));
		SetDlgItemInt(IDC_ROW, 9);
	}

}

void CshaoleiDlg::OnEnKillfocusColumn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int col = GetDlgItemInt(IDC_COLUMN);
	if(col < 7 || col >30)
	{
		MessageBox(_T("����7-30�ỳ�еġ�"));
		SetDlgItemInt(IDC_COLUMN, 9);
	}

}

void CshaoleiDlg::check(void)
{
	int res = m_sl.check();
	if(res == 1)
	{
		KillTimer(1);
		m_mine = 0;
		CString text;
		text.Format(_T("%03d"), m_mine);
		GetDlgItem(IDC_STATIC_MINE)->SetWindowText(text);
	}
	else if( res == -1)
	{
		KillTimer(1);
	}
}
