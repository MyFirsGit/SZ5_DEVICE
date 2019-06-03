#pragma once

#include "BOMForeService.h"

class CNoBusinessIncomeSvc : public CBOMForeService
{
public:
	CNoBusinessIncomeSvc();
	~CNoBusinessIncomeSvc();


	void InitBusinessInfo();											// ��ʼ������������Ϣ
	PLABEL_GROUP GetBusinessInfo();										// ȡ������������Ϣ
	void SetBusinessType(BUSINESS_TYPE businessType,CString businessTypeText);					// ������֧����
	void SetProcessType(AdministrationProcessType processType,CString processTypeText);			// ���ô������
	void SetAmount(long amount);										// ���ý��
	long GetFee(AdministrationProcessType processType);					// ��ȡ��������������
	void DoAdministrationProcess();										// ��������
	//bool IsPrinterException();                                          // ����ӡ���Ƿ�����쳣
	void PrinterPrint();                                                // ��ӡ
private:
	LABEL_GROUP m_BusinessInfo[LABEL_GROUP_EDIT_ROWS_4];				// ������ʾ�Ĺ�����Ϣ
	typedef enum _dialog_group{
		DIALOG_GROUP_BASE	= 1,
	} DIALOG_GROUP;

	BUSINESS_TYPE m_BusinessType;										// ��֧����
	CString m_BusinessType_CN;
	AdministrationProcessType m_ProcessType;							// �������
	CString m_ProcessType_CN;
	long m_Amount;														// ��֧���
	long m_Fee;															// ������
};
