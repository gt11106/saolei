// shaolei.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CshaoleiApp:
// �йش����ʵ�֣������ shaolei.cpp
//

class CshaoleiApp : public CWinApp
{
public:
	CshaoleiApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CshaoleiApp theApp;