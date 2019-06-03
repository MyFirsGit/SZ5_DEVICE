#pragma once
#include "ForeService.h"
#include "bomdialogflow.h"
#include "rwhelper.h"
#include "prthelper.h"
#include "THHelper.h"
#include "ECTCardInfo.h"
#include "ULCardInfo.h"
#include "CPUCardInfo.h"
#include "lpthelper.h"
#include "UPSHelper.h"
#include "twhelper.h"


class CBOMForeService:public CForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CBOMForeService(SERVICE_ID serviceID, SERVICE_ID parentServiceID = MAIN_SVC);      // ���캯��
	~CBOMForeService();                                                                // ��������
	virtual void OnStart();                                                         // Service��������
	virtual bool OnStop();                                                          // Serviceֹͣǰ����
	void DoDialogFlow(const TCHAR* pButtonMsg);                                      // ��������ת��ť��Ϣ
	UINT GetCurDialogGroupID();                                                     // ��ȡ��ǰDialog����
	COperationDlg* GetCurOperationDlg();												 // ��ȡ��ǰDialog;
	UINT GetPreDialogGroupID();                                                     // ��ȡǰһ��Dialog����
	void ShowCurDialogGroup();                                                      // ��ʾ��ǰDialog��
	void HideCurDialogGroup();                                                      // ���ص�ǰDialog��
	virtual void NotifyDataModelChanged();
	CBOMDialogFlow* GetDialogFlow();
	virtual void OnModeChange(UINT mode);                                           // ģʽ�仯��Ӧ����
	virtual void OnBusy(bool busy);
	//CString GetCompleteGuide(ResultCode_t resultCode,OPERATION_MSG operationMsg);   // ��ȡƴ�Ӻ������
	
	CRWHelper* GetRWHelper();
	CTWHelper* GetTWHelper();
	PRTHelper*	GetPRTHelper();
	CUPSHelper* GetUPSHelper();
	CTHHelper* GetTHHelper();
protected:
	CRWHelper* m_pRWHelper;
	CTWHelper* m_pTWHelper;
	PRTHelper* m_pPRTHelper;
	CTHHelper* m_pTHHelper;
	CLPTHelper* m_LPTHelper;
	CUPSHelper* m_UPSHelper;
	virtual void OnReadCard(UINT,CRWReadCard*);
private:
	void OnReadCardEx(UINT,CCommand*);
};