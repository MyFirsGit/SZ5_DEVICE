// TVMBRHLib.h : TVMBRHLib DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTVMBRHLibApp
// �йش���ʵ�ֵ���Ϣ������� TVMBRHLib.cpp
//

class CTVMBRHLibApp : public CWinApp
{
public:
	CTVMBRHLibApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
