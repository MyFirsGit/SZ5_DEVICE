#pragma once
#include "BOMForeService.h"
#include "GStatic.h"
#include "SignCardApplyBaseOfficeDlg.h"
#include "signcardapplycheckofficedlg.h"
#include "CommonDefine.h"
/**
@brief ����������
*/


//�����������¼���ݽṹ��
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

	PLABEL_GROUP GetuserInfo();                        // ��ȡ��ѯ������ʾ���û���Ϣ                        
	int GetuserInfoCount();                            // ��ȡ��ѯ������ʾ���û���Ϣ����
	void SetSex(BYTE sex);							   // �����Ա�
	void SetCertifiType(BYTE cType,CString strType);   // ����֤������
	void SetSevType(BYTE cType,int sevType);		   // ���÷�������
	int GetSevType();								   // ��ȡ��������
	CCPUCard GetCardInfo();
	long OK();                                         // ��ӦOK��ť
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
	LABEL_GROUP m_userInfo[USER_INFO_SIZE];			    // ���롢���»�����ʾ���û���Ϣ
	LABEL_GROUP m_checkInfo[USER_INFO_CHECK_SIZE+1];	// ��ѯ������ʾ���û���Ϣ 
	SIGNCARDAPPLY_INFO m_UserInfo;                      // ������ʾ��Ϣ�ṹ��
	QuerySignCardApply m_querryInfo;					// ��ѯ��Ϣ�ṹ��
	SignCardApply UserInfo;                             // ������Ϣ�ṹ��
	void SetUserInfo();                                 // ���û�����Ϣ���ݹ���     
	int m_sevType;										// ҵ������
	int m_LableCount;									// ��ʾ��Ϣ����
	CString m_checkResult;                              // ��ѯ��� 	
};