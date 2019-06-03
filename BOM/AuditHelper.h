#pragma once
#include "stdafx.h"
#include "ServiceHelper.h"
#include "RWMessage.h"
#include "VersionInfo.h"
#include "prthelper.h"
#include "SCAuditData.h"

/**
@brief  打印审计信息类
*/

class CAuditHelper : public CServiceHelper
{
public:

	CAuditHelper(CService& service);                                                 // 构造函数
	~CAuditHelper();                                                                 // 析构函数

	void PrintAuditReport(SALESTAT_KIND kind, PurchaseData& purchaseData);               //打印审计信息

private:
    PRTHelper* m_pPRTHelper;	
	CString GetPrintLineInfo(CString strTitle,int number,int amount);
	CString GetPrintLineInfo(unsigned short ticketID,int number,int amount,bool isTotal,bool isYKT,bool isAdjust = false);


};
