#pragma once
#include "stdafx.h"
#include "ServiceHelper.h"
#include "RWMessage.h"
#include "VersionInfo.h"
#include "prthelper.h"
#include "SCAuditData.h"

/**
@brief  ��ӡ�����Ϣ��
*/

class CAuditHelper : public CServiceHelper
{
public:

	CAuditHelper(CService& service);                                                 // ���캯��
	~CAuditHelper();                                                                 // ��������

	void PrintAuditReport(SALESTAT_KIND kind, PurchaseData& purchaseData);               //��ӡ�����Ϣ

private:
    PRTHelper* m_pPRTHelper;	
	CString GetPrintLineInfo(CString strTitle,int number,int amount);
	CString GetPrintLineInfo(unsigned short ticketID,int number,int amount,bool isTotal,bool isYKT,bool isAdjust = false);


};
