// TVMTokenCyclerRfid.h : TVMTokenCyclerRfid DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTVMTokenCyclerRfidApp
// �йش���ʵ�ֵ���Ϣ������� TVMTokenCyclerRfid.cpp
//

class CTVMTokenCyclerRfidApp : public CWinApp
{
public:
	CTVMTokenCyclerRfidApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
