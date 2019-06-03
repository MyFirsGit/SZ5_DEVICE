#pragma once
#include "BOMForeService.h"

// �������Ͷ���
typedef enum sr_svc_type{		
	SVC_TYPE_SUSPEND = 1,      // ��ʧ
	SVC_TYPE_RESUME = 2,		// ���
}SR_SVC_TYPE;

/**
@brief    ����ҵ�� - ��ʧ/���ҵ��
*/
class CCardSuspendAndResumeSvc : public CBOMForeService
{
public:
	CCardSuspendAndResumeSvc();
	~CCardSuspendAndResumeSvc();

	void SetSvcType(SR_SVC_TYPE svcType);	//���ù�������
	SR_SVC_TYPE GetSvcType();				//���ع�������
	void SetUserName(CString userName);					// �����û���
	void SetSexType(BYTE sexType,CString sexTypeText);									// �����Ա�
	void SetCertificateType(CERTIFICATE_TYPE certificateType,CString certificateTypeText);	//����֤������
	CERTIFICATE_TYPE GetCertificateType();
	void SetCertificateNumber(CString certificateNumber);		// ����֤������

	void SuspendCard();				//��ʧ
	void ResumeCard();				//���
	//bool IsPrinterException();		// ����ӡ���Ƿ�����쳣
	void PrinterPrint();			// ��ӡ

	void InitLableUserInfo();								// ��ʼ���û���Ϣ���ݹ���
	PLABEL_GROUP GetLableUserInfo();						// ȡ���û���Ϣ

private:
	LABEL_GROUP m_labelUserInfo[LABEL_GROUP_EDIT_ROWS_4];		// ������ʾ���û���Ϣ

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