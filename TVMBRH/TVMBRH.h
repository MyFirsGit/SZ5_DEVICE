// TVMBRH.h : TVMBRH DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTVMBRHApp
// �йش���ʵ�ֵ���Ϣ������� TVMBRH.cpp
//

class CTVMBRHApp : public CWinApp
{
public:
	CTVMBRHApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
