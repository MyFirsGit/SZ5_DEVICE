#pragma once
#include "stdafx.h"	
#include "resource.h"
#include "IOException.h"
#include "IOExp.h"
#include "DIOMessages.h"
/**
@brief  TVM IO�ӿ�Ӧ��
*/

class CIOApp : public CWinApp
{
public:
	CIOApp();

public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};