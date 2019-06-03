#pragma once

#include "BOMForeService.h"

class CNoBusinessIncomeSvc : public CBOMForeService
{
public:
	CNoBusinessIncomeSvc();
	~CNoBusinessIncomeSvc();


	void InitBusinessInfo();											// 初始化行政处理信息
	PLABEL_GROUP GetBusinessInfo();										// 取得行政处理信息
	void SetBusinessType(BUSINESS_TYPE businessType,CString businessTypeText);					// 设置收支类型
	void SetProcessType(AdministrationProcessType processType,CString processTypeText);			// 设置处理类别
	void SetAmount(long amount);										// 设置金额
	long GetFee(AdministrationProcessType processType);					// 获取行政处理手续费
	void DoAdministrationProcess();										// 行政处理
	//bool IsPrinterException();                                          // 检查打印机是否存在异常
	void PrinterPrint();                                                // 打印
private:
	LABEL_GROUP m_BusinessInfo[LABEL_GROUP_EDIT_ROWS_4];				// 画面显示的故障信息
	typedef enum _dialog_group{
		DIALOG_GROUP_BASE	= 1,
	} DIALOG_GROUP;

	BUSINESS_TYPE m_BusinessType;										// 收支类型
	CString m_BusinessType_CN;
	AdministrationProcessType m_ProcessType;							// 处理类别
	CString m_ProcessType_CN;
	long m_Amount;														// 收支金额
	long m_Fee;															// 手续费
};
