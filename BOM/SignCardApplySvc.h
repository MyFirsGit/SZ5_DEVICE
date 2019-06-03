#pragma once
#include "BOMForeService.h"
#include "GStatic.h"
#include "SignCardApplyBaseOfficeDlg.h"
#include "signcardapplycheckofficedlg.h"
#include "CommonDefine.h"
/**
@brief 记名卡申请
*/


//记名卡申请记录数据结构体
typedef struct _SignCardApply_Info 
{   
	CString user_name;                      
	Sex_t sex;
	CERTIFICATE_TYPE certificate_type;
	CString str_certificate_type;
	CString certificate_number;
	_DATE_TIME validate_date;
	CString Phone;
	CString Email;
	CString first_addr;
	CString second_addr;

	_SignCardApply_Info(){
		user_name = "--";
		sex = SEX_UNSPECIFIED;
		certificate_type = CERTIFICATE_UNSPECIFIED;
		certificate_number = "--";
		Phone = "--";
		Email = "--";
		first_addr = "--";
		second_addr = "--";
	}
}SIGNCARDAPPLY_INFO,*LPSIGNCARDAPPLY_INFO;

typedef enum  {
	FIRST_APPLY =1,
	UPDATA_APPLY =2,
	CHECK_APPLY =3

} SERVE_TYPE;

static const TCHAR* const WM_BASE_TO_OVER = _T("WM_BASE_TO_OVER");
static const TCHAR* const WM_BASE_TO_CHECK = _T("WM_BASE_TO_CHECK");
static const TCHAR* const WM_CHECK_TO_CHECK_OVER = _T("WM_CHECK_TO_CHECK_OVER");
static const TCHAR* const WM_CHECK_TO_CHECK_ERROR = _T("WM_CHECK_TO_CHECK_ERROR");
static const TCHAR* const WM_CHECK_TO_BASE = _T("WM_CHECK_TO_BASE");
static const TCHAR* const WM_BASE_TO_ERROR = _T("WM_BASE_TO_ERROR");

class CSignCardApplySvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	typedef enum _dialog_group{
		DIALOG_GROUP_BASE	= 1,
		DIALOG_GROUP_OVER,
		DIALOG_GROUP_CHECK,
		DIALOG_GROUP_CHECK_OVER,
		DIALOG_GROUP_ERROR
	} DIALOG_GROUP;

	PLABEL_GROUP GetuserInfo();                        // 获取查询画面显示的用户信息                        
	int GetuserInfoCount();                            // 获取查询画面显示的用户信息个数
	void SetSex(BYTE sex);							   // 设置性别
	void SetCertifiType(BYTE cType,CString strType);   // 设置证件类型
	void SetSevType(BYTE cType,int sevType);		   // 设置服务类型
	int GetSevType();								   // 获取服务类型
	CCPUCard GetCardInfo();
	long OK();                                         // 响应OK按钮
	//bool IsPrinterException();
	void PrinterPrint();
	CSignCardApplySvc();
	~CSignCardApplySvc();
private:
	CCPUCard m_CpuCard;
	void StartReadCard();
	void OnReadCard(UINT resultCode,CRWReadCard* pRWReadCard);
	bool SaveCardInfo(LPRW_READ_CARD_RESPONSE pRWReadCardResponse);
	void OnCardError(CString errorMsg);
	long BlockCard(UINT resultCode,RW_BUSSINESS_RESPONSE response);
	void SaveSCData(SERVE_TYPE);
	LABEL_GROUP m_userInfo[USER_INFO_SIZE];			    // 申请、更新画面显示的用户信息
	LABEL_GROUP m_checkInfo[USER_INFO_CHECK_SIZE+1];	// 查询画面显示的用户信息 
	SIGNCARDAPPLY_INFO m_UserInfo;                      // 申请显示信息结构体
	QuerySignCardApply m_querryInfo;					// 查询信息结构体
	SignCardApply UserInfo;                             // 申请信息结构体
	void SetUserInfo();                                 // 设置基本信息数据构成     
	int m_sevType;										// 业务类型
	int m_LableCount;									// 显示信息行数
	CString m_checkResult;                              // 查询结果 	
};