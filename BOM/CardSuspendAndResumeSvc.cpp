operator=
#include "StdAfx.h"
#include "CardSuspendAndResumeBaseOfficeDlg.h"
#include "CardSuspendAndResumeSvc.h"
#include "GuideInfo.h"
#include "WelcomeReceptionDlg.h"
#include "TemplateHelper.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CCardSuspendAndResumeSvc::CCardSuspendAndResumeSvc()
	:CBOMForeService(CARD_SUSPEND_AND_RESUME_SVC)
{
	//设置默认值
	m_SvcType = SVC_TYPE_SUSPEND;
	m_SexType = 1;
	m_CertificateType = CERTIFICATE_ID_CARD_MAINLAND;

	InitLableUserInfo();

	// 设置操作员画面
	m_pDialogFlow->AddDialog(CARD_SUSPEND_AND_RESUME_BASE_DLG, new CCardSuspendAndResumeBaseOfficeDlg(this));

	//设置乘客画面
	m_pDialogFlow->AddDialog(IDD_10002_WELCOME_DLG, new CWelcomeReceptionDlg(NULL));

	//设置挂失解挂画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_CARD_SUSPEBD_AND_RESUME_BASE, theGUIDE_INFO.GetOperationMessage(GUIDE_SUSPEND_AND_RESUME_SELECT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CARD_SUSPEBD_AND_RESUME_BASE, CARD_SUSPEND_AND_RESUME_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CARD_SUSPEBD_AND_RESUME_BASE, IDD_10002_WELCOME_DLG);

	//设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_CARD_SUSPEBD_AND_RESUME_BASE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CCardSuspendAndResumeSvc::~CCardSuspendAndResumeSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置功能类型

@param      (i)S_R_SVC_TYPE    功能类型

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardSuspendAndResumeSvc::SetSvcType(SR_SVC_TYPE svcType)
{
	m_SvcType = svcType;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      返回功能类型

@param      none

@retval     S_R_SVC_TYPE	功能类型

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
SR_SVC_TYPE CCardSuspendAndResumeSvc::GetSvcType()
{
	return m_SvcType;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置姓名

@param      (i)CString    输入姓名

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardSuspendAndResumeSvc::SetUserName(CString userName)
{
	m_UserName = userName;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置性别类型

@param      (i)S_R_SEX_TYPE    性别类型

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardSuspendAndResumeSvc::SetSexType(BYTE sexType,CString sexTypeText)
{
	m_SexType = sexType;
	m_SexType_CN = sexTypeText;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置证件类型

@param      (i)S_R_CERTIFICATE_TYPE    证件类型

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardSuspendAndResumeSvc::SetCertificateType(CERTIFICATE_TYPE certificateType,CString certificateTypeText)
{
	m_CertificateType = certificateType;
	m_CertificateType_CN = certificateTypeText;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      返回证件类型

@param      none

@retval     CERTIFICATE_TYPE	证件类型

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CERTIFICATE_TYPE CCardSuspendAndResumeSvc::GetCertificateType()
{
	return m_CertificateType;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置证件编码

@param      (i)CString 证件编码

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardSuspendAndResumeSvc::SetCertificateNumber(CString certificateNumber)
{
	m_CertificateNumber = certificateNumber;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     挂失处理

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardSuspendAndResumeSvc::SuspendCard()
{
	ReportLoss suspendInfo;
	memcpy(suspendInfo.name,m_UserName.GetBuffer(),m_UserName.GetLength());
	suspendInfo.gender = m_SexType;
	suspendInfo.certificateType = m_CertificateType;
	memcpy(suspendInfo.certificateID,m_CertificateNumber.GetBuffer(),m_CertificateNumber.GetLength());
	theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertSignCardReportLoss(suspendInfo);
	//if (!IsPrinterException()){
		PrinterPrint();
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      解挂处理

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardSuspendAndResumeSvc::ResumeCard()
{
	CancelReportLoss resumeInfo;
	memcpy(resumeInfo.name,m_UserName.GetBuffer(),m_UserName.GetLength());
	resumeInfo.gender = m_SexType;
	resumeInfo.certificateType = m_CertificateType;
	memcpy(resumeInfo.certificateID,m_CertificateNumber.GetBuffer(),m_CertificateNumber.GetLength());
	theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertSignCardCancelReportLoss(resumeInfo);
	//if (!IsPrinterException()){
		PrinterPrint();
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化用户信息数据构成

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCardSuspendAndResumeSvc::InitLableUserInfo()
{
	int i=0;

	// 姓名
	m_labelUserInfo[i].nameLeft = add_wsp(CString(TXT_USER_NAME) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 性别
	m_labelUserInfo[i].nameLeft = add_wsp(CString(TXT_USER_SEX) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 证件类型
	m_labelUserInfo[i].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_TYPE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// 证件编号
	m_labelUserInfo[i].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_NUM) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得用户信息

@param      none

@retval     PLABEL_GROUP 用户信息LABEL_GROUP

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
PLABEL_GROUP CCardSuspendAndResumeSvc::GetLableUserInfo()
{
	return m_labelUserInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断打印机是否正常

@param      none

@retval     bool true:  false:异常正常

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//bool CCardSuspendAndResumeSvc::IsPrinterException() 
//{
//	return m_pPRTHelper->IsPrinterHasException();
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打印凭证

@param      none

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCardSuspendAndResumeSvc::PrinterPrint()
{
	try{
		CString templateName= _T("SUSPEND_RESUME.template");
		CStringArray printArr;
		for(int printIndex=0;printIndex<theBOM_INFO.GetReceiptPrintTimes(_T("SUSPEND_RESUME"));printIndex++)
		{
			CStringArray printTexts;
			theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);
			const int hspCount = 20;
			int i = 0;
			while(i < printTexts.GetCount())
			{
				CString& line = printTexts.ElementAt(i);
				if(m_SvcType == SVC_TYPE_SUSPEND){
					line.Replace(_T("{SUSPEND_TITLE}"),_T(""));
					if(line.Find(_T("{RESUME_TITLE}")) >=0)
					{
						printTexts.RemoveAt(i);
						continue;
					}
				}
				else if(m_SvcType == SVC_TYPE_RESUME){
					line.Replace(_T("{RESUME_TITLE}"),_T(""));
					if(line.Find(_T("{SUSPEND_TITLE}")) >=0)
					{
						printTexts.RemoveAt(i);
						continue;
					}
				}
				line.Replace(_T("{USER_NAME}"),m_UserName);
				line.Replace(_T("{USER_SEX}"),m_SexType_CN);
				line.Replace(_T("{CERTIFICATE_TYPE}"),m_CertificateType_CN);
				line.Replace(_T("{CERTIFICATE_NUM}"),m_CertificateNumber);
				CString billNum;
				billNum.Format(_T("%d"),theCOUNT_INFO.GetUDSNForDisplay());
				line.Replace(_T("{BILL_NUM}"),billNum);
				line.Replace(_T("{TRANS_TIME}"),ComGetCurTime().ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d")));
				line.Replace(_T("{STATION_NAME}"),theAPP_SESSION.GetStationNameCN());
				line.Replace(_T("{DEVICE_CODE}"),theMAINTENANCE_INFO.GetCurrentACCDevice().ToString());
				line.Replace(_T("{USER_ID}"),theAPP_SESSION.GetUserInfo().sUserID);
				CStringArray printLine;
				ComPrintLineFormatForTemplate(line,printLine);
				printArr.Append(printLine);
				i++;
			}
		}
		m_pPRTHelper->PrinterPrint(&printArr,false);

	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
}