#pragma once
#include "stdafx.h"
#include "ServiceHelper.h"
#include "LPT.h"


/**
@brief  LPT������
*/

class CLPTHelper : public CServiceHelper
{
public:

	CLPTHelper(CService& service);                                                    // ���캯��
	~CLPTHelper();                                                                    // ��������
	
	// LPT���нӿ� 
	long  LPTOpenLPT();								                                   // �򿪲���
	long  LPTReadLPT(BYTE&);											               // ������
	long  LPTCloseLPT();															   // �رղ���
	DOORSTATUS GetTicketboxDoorStatus();
};
