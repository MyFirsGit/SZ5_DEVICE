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
	CBOMForeService(SERVICE_ID serviceID, SERVICE_ID parentServiceID = MAIN_SVC);      // 构造函数
	~CBOMForeService();                                                                // 析构函数
	virtual void OnStart();                                                         // Service启动后处理
	virtual bool OnStop();                                                          // Service停止前处理
	void DoDialogFlow(const TCHAR* pButtonMsg);                                      // 处理画面跳转按钮消息
	UINT GetCurDialogGroupID();                                                     // 获取当前Dialog组编号
	COperationDlg* GetCurOperationDlg();												 // 获取当前Dialog;
	UINT GetPreDialogGroupID();                                                     // 获取前一组Dialog组编号
	void ShowCurDialogGroup();                                                      // 显示当前Dialog组
	void HideCurDialogGroup();                                                      // 隐藏当前Dialog组
	virtual void NotifyDataModelChanged();
	CBOMDialogFlow* GetDialogFlow();
	virtual void OnModeChange(UINT mode);                                           // 模式变化响应函数
	virtual void OnBusy(bool busy);
	//CString GetCompleteGuide(ResultCode_t resultCode,OPERATION_MSG operationMsg);   // 获取拼接后的文言
	
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