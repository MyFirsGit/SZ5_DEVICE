// TVMCoinShutter.h : TVMCoinShutter DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTVMCoinShutterApp
// �йش���ʵ�ֵ���Ϣ������� TVMCoinShutter.cpp
//

class CTVMCoinShutterApp : public CWinApp
{
public:
	CTVMCoinShutterApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
