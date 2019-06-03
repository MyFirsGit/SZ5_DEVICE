#pragma once
#include "BOMForeService.h"

// 功能类型定义
typedef enum sr_svc_type{		
	SVC_TYPE_SUSPEND = 1,      // 挂失
	SVC_TYPE_RESUME = 2,		// 解挂
}SR_SVC_TYPE;

/**
@brief    其他业务 - 挂失/解挂业务
*/
class CCardSuspendAndResumeSvc : public CBOMForeService
{
public:
	CCardSuspendAndResumeSvc();
	~CCardSuspendAndResumeSvc();

	void SetSvcType(SR_SVC_TYPE svcType);	//设置功能类型
	SR_SVC_TYPE GetSvcType();				//返回功能类型
	void SetUserName(CString userName);					// 设置用户名
	void SetSexType(BYTE sexType,CString sexTypeText);									// 设置性别
	void SetCertificateType(CERTIFICATE_TYPE certificateType,CString certificateTypeText);	//设置证件类型
	CERTIFICATE_TYPE GetCertificateType();
	void SetCertificateNumber(CString certificateNumber);		// 设置证件编码

	void SuspendCard();				//挂失
	void ResumeCard();				//解挂
	//bool IsPrinterException();		// 检查打印机是否存在异常
	void PrinterPrint();			// 打印

	void InitLableUserInfo();								// 初始化用户信息数据构成
	PLABEL_GROUP GetLableUserInfo();						// 取得用户信息

private:
	LABEL_GROUP m_labelUserInfo[LABEL_GROUP_EDIT_ROWS_4];		// 画面显示的用户信息

	typedef enum _dialog_group
	{
		DIALOG_GROUP_CARD_SUSPEBD_AND_RESUME_BASE = 1,                       
	} DIALOG_GROUP;

	SR_SVC_TYPE m_SvcType;
	CString m_UserName;
	BYTE m_SexType;
	CString m_SexType_CN;
	CERTIFICATE_TYPE m_CertificateType;
	CString m_CertificateType_CN;
	CString m_CertificateNumber;
};