#include "stdafx.h"
#include "SignCardApplySvc.h"
#include "ServiceDefinition.h"
#include "BOMForeService.h"
#include "WelcomeReceptionDlg.h"
#include "SignCardApplyBaseOfficeDlg.h"
#include "GuideInfo.h"
#include "signcardapplycheckofficedlg.h"
#include "SignCardApplyOverOfficeDlg.h"
#include "TemplateHelper.h"
#include "RWDef.h"

BEGIN_MESSAGE_MAP(CSignCardApplySvc,CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CSignCardApplySvc::CSignCardApplySvc()
:CBOMForeService(SIGNCARD_APPLY_SVC)
{
	m_checkResult= TXT_NOTHING;
	// ��ʼ��������Ϣ
    SetUserInfo();
	
	// ���ò���Ա����
	m_pDialogFlow->AddDialog(IDD_02801_SIGN_CARD_APPLY_BASE_DLG, new CSignCardApplyBaseOfficeDlg(this));
	m_pDialogFlow->AddDialog(IDD_02803_SIGN_CARD_APPLY_CHECK_DLG, new CSignCardApplyCheckOfficeDlg(this));
	m_pDialogFlow->AddDialog(IDD_02802_SIGN_CARD_APPLY_OVER_DLG, new CSignCardApplyOverOfficeDlg(this));
	//���ó˿ͻ���
	m_pDialogFlow->AddDialog(IDD_02801_SIGN_CARD_APPLY_DLG, new CWelcomeReceptionDlg(this));

	// �������뻭����
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_02801_SIGN_CARD_APPLY_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_02801_SIGN_CARD_APPLY_DLG);

	// �����������������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_OVER, theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SUCCEED));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_OVER, IDD_02802_SIGN_CARD_APPLY_OVER_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_OVER, IDD_02801_SIGN_CARD_APPLY_DLG);

	// ���ò�ѯ������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_CHECK, theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CHECK, IDD_02803_SIGN_CARD_APPLY_CHECK_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CHECK, IDD_02801_SIGN_CARD_APPLY_DLG);

	// ���ò�ѯ����������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_CHECK_OVER, theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CHECK_OVER, IDD_02802_SIGN_CARD_APPLY_OVER_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CHECK_OVER, IDD_02801_SIGN_CARD_APPLY_DLG);

	// ���������������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR, theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, IDD_02802_SIGN_CARD_APPLY_OVER_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, IDD_02801_SIGN_CARD_APPLY_DLG);

	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_BASE_TO_OVER,DIALOG_GROUP_OVER);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_BASE_TO_CHECK,DIALOG_GROUP_CHECK);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_BASE_TO_ERROR,DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_CHECK,WM_CHECK_TO_CHECK_OVER,DIALOG_GROUP_CHECK_OVER);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_CHECK,WM_CHECK_TO_CHECK_ERROR,DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_CHECK,WM_CHECK_TO_BASE,DIALOG_GROUP_BASE);

	//���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CSignCardApplySvc::~CSignCardApplySvc()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��������Ϣ���ݹ���

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSignCardApplySvc::SetUserInfo()
{
	int i=0;
	// �״����� || ������
	if(m_sevType ==FIRST_APPLY ||m_sevType == UPDATA_APPLY) {
		// ����
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_NAME) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.user_name;
		i++;
		// �Ա�
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_SEX) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.sex==Sex_t::SEX_MALE? MALE:FEMALE;
		i++;
		// ֤������
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_TYPE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.str_certificate_type;
		i++;
		// ֤�����
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_NUM) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.certificate_number;
		i++;
		// ֤����Ч��
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_VALIDATE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		if(ComCheckDateTimeValid(m_UserInfo.validate_date)==0){
			m_userInfo[i].nameRight = m_UserInfo.validate_date.ToString(_T("%.4d��%.2d��%.2d��" ));
		}
		else{
			m_userInfo[i].nameRight = "  --  ��  -- �� --  ��";
		}	
		i++;
		// �绰
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_PHONE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.Phone==""?TXT_NOTHING:m_UserInfo.Phone;
		i++;
		// Email
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_EMAIL) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.Email==""?TXT_NOTHING:m_UserInfo.Email;
		i++;
		// ��ַ1
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_FIRST_ADDR) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.first_addr==""?TXT_NOTHING:m_UserInfo.first_addr;
		i++;
		// ��ַ2
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_SECND_ADDE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.second_addr==""?TXT_NOTHING:m_UserInfo.second_addr;
		i++;		
	}
	// �����ѯ
	else if (m_sevType == CHECK_APPLY)	{
		// ֤������
		m_checkInfo[i].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_TYPE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_checkInfo[i].nameRight = m_UserInfo.str_certificate_type;
		i++;
		// ֤�����
		m_checkInfo[i].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_NUM) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_checkInfo[i].nameRight = m_UserInfo.certificate_number;
		i++;
		// ��ѯ��������
		if (m_pDialogFlow->GetCurDialogGroupID() ==DIALOG_GROUP_CHECK_OVER){
			// ��ǰ״̬
			m_checkInfo[i].nameLeft = add_wsp(CString(TXT_USER_CURRENT_STATE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
			m_checkInfo[i].nameRight= m_checkResult==""?TXT_NOTHING:m_checkResult; 
			i++;
		}
	}
	m_LableCount = i;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��ѯ������ʾ���û���Ϣ����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CSignCardApplySvc::GetuserInfoCount()
{
	return m_LableCount;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��Ƭ��Ϣ

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CCPUCard CSignCardApplySvc::GetCardInfo()
{
	// ����
	m_UserInfo.user_name = m_CpuCard.personalData.Name;
	// �Ա�
	((CSignCardApplyBaseOfficeDlg*)m_pDialogFlow->GetDialog(IDD_02801_SIGN_CARD_APPLY_BASE_DLG))->ClickSexButton(m_CpuCard.personalData.Sex);
	// ֤������
	((CSignCardApplyBaseOfficeDlg*)m_pDialogFlow->GetDialog(IDD_02801_SIGN_CARD_APPLY_BASE_DLG))->ClickCredentialsTypeButton(m_CpuCard.personalData.CredentialsType);
	// ֤�����
	m_UserInfo.certificate_number = m_CpuCard.personalData.CredentialsNumber;
	SetUserInfo();
	return m_CpuCard;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief    ��ȡ��ѯ������ʾ���û���Ϣ     

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
PLABEL_GROUP CSignCardApplySvc::GetuserInfo()
{
	SetUserInfo();
	if (m_sevType == CHECK_APPLY)
	{
		return m_checkInfo;
	}
	return m_userInfo;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief   �����Ա�   

@param     (i)BYTE sex �Ա�

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSignCardApplySvc::SetSex(BYTE sex)
{
	if (sex == SEX_MALE){
		m_userInfo[1].nameRight = MALE;
	}
	else{
		m_userInfo[1].nameRight = FEMALE;
	}
    UserInfo.gender = m_UserInfo.sex = Sex_t(sex);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief   ����֤������   

@param     (i) BYTE cType, ֤�����ʹ���
		   (i) CString strType    ֤������

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSignCardApplySvc::SetCertifiType(BYTE cType,CString strType)
{
	m_userInfo[2].nameRight = strType;
	m_checkInfo[0].nameRight = strType;
	m_UserInfo.certificate_type = UserInfo.certificateType = CERTIFICATE_TYPE(cType);
	m_UserInfo.str_certificate_type = strType;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////

void CSignCardApplySvc::StartReadCard()
{
	theSERVICE_MGR.SetState(ISSUEING);
	m_pRWHelper->ReadCard();
	INT_PTR ret = CMessageBox::Show(theAPP_SESSION.GetServiceNameCH((SERVICE_ID)this->GetServiceID()),GUIDE_WAIT_CARD,CMessageBox::BTN_CANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_180);
	if(ret == IDCANCEL)
	{
		m_pRWHelper->StopReadCard();
		((CSignCardApplyBaseOfficeDlg*)m_pDialogFlow->GetDialog(IDD_02801_SIGN_CARD_APPLY_BASE_DLG))->ClickSevTypeButton(0);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ó�Ʊ��Ϣ

@param      (i)  UINT errCode              �������
@param      (i)  CRWReadCard* rwReadCard   ��������

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CSignCardApplySvc::OnReadCard(UINT resultCode,CRWReadCard* pRWReadCard)
{
	try{
		CMessageBox::End();
		if(GetCurDialogGroupID() == DIALOG_GROUP_OVER || GetCurDialogGroupID() == DIALOG_GROUP_ERROR||GetCurDialogGroupID() == DIALOG_GROUP_CHECK_OVER||GetCurDialogGroupID() == DIALOG_GROUP_CHECK)
		{
			return;
		}
		theSERVICE_MGR.SetState(RECEPT);
		auto_ptr<RW_READ_CARD_RESPONSE> pRWReadCardResponse(new RW_READ_CARD_RESPONSE);
		pRWReadCard->GetRWResponse(pRWReadCardResponse.get());
		// ����ʧ��
		if(resultCode != RW_RSP_OK){
			// ��������
			if(resultCode == RW_RSP_ACC_CARD_BLACKLIST || resultCode == RW_RSP_ECT_CARD_BLACKLIST){
				RW_BUSSINESS_RESPONSE response;
				theSERVICE_MGR.SetState(ISSUEING);
				long errCode = BlockCard(resultCode,response); 
				theSERVICE_MGR.SetState(RECEPT);
				if(errCode != RW_RSP_OK){
					OnCardError(m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode));
				}	
				else{
					OnCardError(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_BLACK));
				}	
			}	
			// ��������
			else{
				if(pRWReadCardResponse->DetailErrorCode != RW_ERROR_CARD_NO_CARD){
					OnCardError(m_pRWHelper->GetTpuErrorInfo(resultCode));			
				}
				else{
				}
			}
		}
		// �����ɹ�
		else{
			// �����������
			if (SaveCardInfo(pRWReadCardResponse.get())){
				((CSignCardApplyBaseOfficeDlg*)m_pDialogFlow->GetDialog(IDD_02801_SIGN_CARD_APPLY_BASE_DLG))->PostMessage(WM_ACTIVATE, WA_ACTIVE, NULL);
			}
			else{
				OnCardError(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_CARDINFO_ERROR));
			}
		}
	}
	catch (CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}

}

//////////////////////////////////////////////////////////////////////////
/*
@brief    �����������

@param      (i) LPRW_READ_CARD_RESPONSE pRWReadCardResponse ��������

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSignCardApplySvc::SaveCardInfo(LPRW_READ_CARD_RESPONSE pRWReadCardResponse)
{
	if (pRWReadCardResponse->CardPHType == CPU_TICKET){	
		m_CpuCard.GetCardInfo(pRWReadCardResponse->CardInfoLen,pRWReadCardResponse->CardInfo);
		return true;
	}
	else{
		return false;
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ������

@param      CString errorMsg ������Ϣ

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSignCardApplySvc::OnCardError(CString errorMsg)
{
	m_pDialogFlow->ShowNextDialogGroup(WM_BASE_TO_ERROR);
	theAPP_SESSION.ShowOfficeGuide(errorMsg);
	theSERVICE_MGR.SetState(FINISH);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CSignCardApplySvc::BlockCard(UINT resultCode,RW_BUSSINESS_RESPONSE response)
{
	RW_CMD_BLOCK rw_cmd_block;
	if (resultCode == RW_RSP_ACC_CARD_BLACKLIST){
		rw_cmd_block.SerialNo = theCOUNT_INFO.GetUDSNForDisplay();
		rw_cmd_block.TranDateTime = ComGetCurTime();
		resultCode = m_pRWHelper->Block(rw_cmd_block,response);
		// �����ɹ�
		if(RW_RSP_OK == resultCode){
			// ���½�����ˮ��
			theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);	

			// ����UD����
			if(response.UDlen != 0){
				m_pRWHelper->SaveRWUDData(response.UDlen,response.UDData);
			}

			// ����AR����
			if(response.ARLen != 0){
				m_pRWHelper->SaveRWARData(response.ARLen,response.ARData,response.TranResult.CardType);
			}
		}
	} 
	return resultCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ���÷������� 

@param      (i) BYTE cType, �������ʹ���
			(i) CString strType    ������������

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSignCardApplySvc::SetSevType(BYTE cType,int sevType)
{
	m_sevType = sevType;
	// �����ѯ
	if(m_sevType == CHECK_APPLY&&m_pDialogFlow->GetCurDialogGroupID() !=DIALOG_GROUP_CHECK){
		m_pDialogFlow->ShowNextDialogGroup(WM_BASE_TO_CHECK); 
	}	
	// �״�����
	if((m_sevType == FIRST_APPLY)&&m_pDialogFlow->GetCurDialogGroupID() ==DIALOG_GROUP_CHECK){
		m_pDialogFlow->ShowNextDialogGroup(WM_CHECK_TO_BASE); 
	}	
	// ������
	if(m_sevType ==UPDATA_APPLY){
		if (m_pDialogFlow->GetCurDialogGroupID() ==DIALOG_GROUP_CHECK)
		{
			m_pDialogFlow->ShowNextDialogGroup(WM_CHECK_TO_BASE); 
		}		
		StartReadCard();
	}
	else if(m_sevType ==FIRST_APPLY){
		((CSignCardApplyBaseOfficeDlg*)m_pDialogFlow->GetDialog(IDD_02801_SIGN_CARD_APPLY_BASE_DLG))->PostMessage(WM_ACTIVATE, WA_ACTIVE, NULL);
	}	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ��ȡ��������

@param      (i) BYTE cType, �������ʹ���
(i) CString strType    ������������

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CSignCardApplySvc::GetSevType()
{
	return m_sevType;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ��Ӧȷ����ť

@param      ��

@retval      0:�ɹ�  ��0��ʧ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CSignCardApplySvc::OK()
{
	try{
		theSERVICE_MGR.SetState(ISSUEING);
		// ��ѯ״̬
		if(m_sevType == CHECK_APPLY&&m_pDialogFlow->GetCurDialogGroupID() ==DIALOG_GROUP_CHECK){
			// ��ȡ¼����Ϣ
			CSignCardApplyCheckOfficeDlg* resDlg = (CSignCardApplyCheckOfficeDlg*)m_pDialogFlow->GetDialog(IDD_02803_SIGN_CARD_APPLY_CHECK_DLG);
			m_querryInfo.certificateType = m_UserInfo.certificate_type;
			m_UserInfo.certificate_number = resDlg->GetEditText(0);
			int bufSize = m_UserInfo.certificate_number.GetLength();
			memcpy(m_querryInfo.certificateID,m_UserInfo.certificate_number.GetBuffer(),bufSize<sizeof(UserInfo.certificateID)? bufSize:sizeof(UserInfo.certificateID));
			
			// ��ѯ����������
			BYTE querry[21];
			LPBYTE lquery = querry;
			memcpy(lquery,&m_querryInfo.certificateType,1);
			lquery++;
			memcpy(lquery,m_querryInfo.certificateID,sizeof(m_querryInfo.certificateID));
			BYTE checkStatus;
			bool isChecked = theNAMED_CARD_APPLY_RRSULT.GetTheApplyResult(querry,checkStatus);
			// ���ؼ�¼������
			if(!isChecked){	
				// ���Ͳ�ѯ����
				theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->SendQuerySignCardApplyStatus(m_querryInfo);
				int count = 0;
				do{
					if(count == 12){
						checkStatus = 0;	
						// ��ѯʧ��
						theSERVICE_MGR.SetState(FINISH);
						m_pDialogFlow->ShowNextDialogGroup(WM_CHECK_TO_CHECK_ERROR); 
						return 1;
					}				
					Sleep(1000);
					count++;
				}while(!theNAMED_CARD_APPLY_RRSULT.GetTheApplyResult(lquery,checkStatus));
			}
			m_checkResult = theBOM_INFO.GetApplyCardState(checkStatus);
			theSERVICE_MGR.SetState(FINISH);
			m_pDialogFlow->ShowNextDialogGroup(WM_CHECK_TO_CHECK_OVER); 
			// SC���
			SaveSCData(CHECK_APPLY);
		}
		// ����������
		if((m_sevType == FIRST_APPLY||m_sevType ==UPDATA_APPLY)&&m_pDialogFlow->GetCurDialogGroupID() ==DIALOG_GROUP_BASE){
			// ��ȡ¼����Ϣ
			CSignCardApplyBaseOfficeDlg* resDlg = (CSignCardApplyBaseOfficeDlg*)m_pDialogFlow->GetDialog(IDD_02801_SIGN_CARD_APPLY_BASE_DLG);
			// ����
			m_UserInfo.user_name = resDlg->GetEditText(0);
			int bufSize = m_UserInfo.user_name.GetLength();
			memcpy(UserInfo.name,m_UserInfo.user_name.GetBuffer(),bufSize<sizeof(UserInfo.name)? bufSize:sizeof(UserInfo.name));
			// ֤�����
			m_UserInfo.certificate_number = resDlg->GetEditText(1);
			bufSize = m_UserInfo.certificate_number.GetLength();
			memcpy(UserInfo.certificateID,m_UserInfo.certificate_number.GetBuffer(),bufSize<sizeof(UserInfo.certificateID)? bufSize:sizeof(UserInfo.certificateID));
			// ֤����Ч����
			m_UserInfo.validate_date.wYear = _ttoi(resDlg->GetEditText(2));
			m_UserInfo.validate_date.biMonth = _ttoi(resDlg->GetEditText(3));
			m_UserInfo.validate_date.biDay = _ttoi(resDlg->GetEditText(4));
			m_UserInfo.validate_date.biHour=0;
			m_UserInfo.validate_date.biMinute=0;
			m_UserInfo.validate_date.biSecond=0;
			UserInfo.cardValidity = m_UserInfo.validate_date;
			// �绰
			m_UserInfo.Phone = resDlg->GetEditText(5);
			CString lpPhone = m_UserInfo.Phone;
			int i=0;
			while(lpPhone != ""){
				int temNum = _ttoi(lpPhone.Left(2));
				int2BCD(temNum,(char*)&UserInfo.phoneNumber[i++],1);
				lpPhone.Delete(0,2);
			}
			/*bufSize = m_UserInfo.Phone.GetLength();
			memcpy(UserInfo.phoneNumber,m_UserInfo.Phone.GetBuffer(),bufSize<sizeof(UserInfo.phoneNumber)? bufSize:sizeof(UserInfo.phoneNumber));*/
			// ����
			m_UserInfo.Email = resDlg->GetEditText(6);
			bufSize = m_UserInfo.Email.GetLength();
			if(bufSize !=0){
				memcpy(UserInfo.email,m_UserInfo.Email.GetBuffer(),bufSize<sizeof(UserInfo.email)? bufSize:sizeof(UserInfo.email));
			}
			// ��ַ1
			m_UserInfo.first_addr = resDlg->GetEditText(7);
			bufSize = m_UserInfo.first_addr.GetLength();
			if(bufSize !=0){
				memcpy(UserInfo.address1,m_UserInfo.first_addr.GetBuffer(),bufSize<sizeof(UserInfo.address1)? bufSize:sizeof(UserInfo.address1));
			}
			// ��ַ2
			m_UserInfo.second_addr = resDlg->GetEditText(8);
			bufSize = m_UserInfo.second_addr.GetLength();
			if(bufSize !=0){
				memcpy(UserInfo.address2,m_UserInfo.second_addr.GetBuffer(),bufSize<sizeof(UserInfo.address2)? bufSize:sizeof(UserInfo.address2));
			}			
			// �״�����
			if(m_sevType == FIRST_APPLY){
				// ���������¼������
				BYTE localContent[21];
				LPBYTE lpBillNum = localContent;
				localContent[0] = UserInfo.certificateType;
				memcpy(localContent+1,UserInfo.certificateID,sizeof(UserInfo.certificateID));
				BYTE status = 0;
				theNAMED_CARD_APPLY_RRSULT.UpdateTheApplyResult(lpBillNum,status);
				// ���潻������
				theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertSignCardApply(UserInfo);
				theSERVICE_MGR.SetState(FINISH);
				m_pDialogFlow->ShowNextDialogGroup(WM_BASE_TO_OVER);
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SUCCEED));
				// SC���
				SaveSCData(FIRST_APPLY);
			}
			// ������
			else{
				//д������
				RW_CMD_UPDATE_SIGNCARD_INFO updateSignCardInfo;
				updateSignCardInfo.CertificateType =  UserInfo.certificateType;
				memcpy(updateSignCardInfo.CertificateNo,UserInfo.certificateID,sizeof(UserInfo.certificateID));
				memcpy(updateSignCardInfo.Name,UserInfo.name,sizeof(UserInfo.name));
				updateSignCardInfo.SexType = UserInfo.gender;
				updateSignCardInfo.SerialNo = theCOUNT_INFO.GetUDSN();
				updateSignCardInfo.TranDateTime = ComGetCurTime();
				updateSignCardInfo.Deposit = m_CpuCard.applicationData.Deposit;
				updateSignCardInfo.AppVer = 0;
				updateSignCardInfo.CardholderType = m_CpuCard.personalData.CardHoldType;
				updateSignCardInfo.StaffType = m_CpuCard.personalData.StaffStatus;
				updateSignCardInfo.CertificateValidDate = UserInfo.cardValidity;
				memcpy(updateSignCardInfo.Tel,UserInfo.phoneNumber,sizeof(UserInfo.phoneNumber));
				memcpy(updateSignCardInfo.Email,UserInfo.email,sizeof(UserInfo.email));
				memcpy(updateSignCardInfo.Addr1,UserInfo.address1,sizeof(UserInfo.address1));
				memcpy(updateSignCardInfo.Addr2,UserInfo.address2,sizeof(UserInfo.address2));
				RW_BUSSINESS_RESPONSE response;
				long res = m_pRWHelper->UpdateSignCardInfo(updateSignCardInfo,response);
				if (res != RW_RSP_OK){
					OnCardError( m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode));
					return 1;
				}
				else{
					// ���½�����ˮ��
					theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);	
					// ����UD����
					if(response.UDlen != 0){
						m_pRWHelper->SaveRWUDData(response.UDlen,response.UDData);
					}
					// ����AR����
					if(response.ARLen != 0){
						m_pRWHelper->SaveRWARData(response.ARLen,response.ARData,m_CpuCard.applicationData.CardAppMainType);
					}
					// SC���
					SaveSCData(UPDATA_APPLY);
					theSERVICE_MGR.SetState(FINISH);
					m_pDialogFlow->ShowNextDialogGroup(WM_BASE_TO_OVER);
					theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_UPDATA_SUCCEED));
				}			
			}			
			PrinterPrint();  			
		}
		return 0;
	}
	catch (CSysException& e)
	{
		OnCardError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnCardError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����һ��ͨ��ֵsc����

@param      none

@retval  	long	������Ϣ

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CSignCardApplySvc::SaveSCData(SERVE_TYPE serType)
{
	SCAuditKey purchaseKey;
	purchaseKey.serviceID = SIGNCARD_APPLY_SVC;
	purchaseKey.cardIssuancer = ACC;
	purchaseKey.ticketType = m_CpuCard.applicationData.CardAppMainType;
	purchaseKey.flag = serType;

	SCAuditValue purchaseValue;
	purchaseValue.transCount = 1;
	ProductCategory_t category;
	theACC_TICKET.GetTicketCategry(m_CpuCard.applicationData.CardAppMainType,category);
	if(category == PURSE){
		purchaseValue.transAmount = 0;
	}
	if(category == MULTIRIDE){
		purchaseValue.transAmount = 0;
	}
	else if(category == PERIOD){
		// none
	}
	theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �жϴ�ӡ���Ƿ�����

@param      ��

@retval     bool true:  false:�쳣����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//bool CSignCardApplySvc::IsPrinterException() 
//{
//	return m_pPRTHelper->IsPrinterHasException();
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ӡ���ݡ���Ǯ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSignCardApplySvc::PrinterPrint()
{
	try{
		CString templateName= _T("SIGNCARD.template");
		CStringArray printArr;
		for(int i=0;i<theBOM_INFO.GetReceiptPrintTimes(_T("SIGNCARD"));i++)
		{
			CStringArray printTexts;
			theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);
			_DATE_TIME transDateTime = ComGetCurTime();
			
			for(int i = 0;i<printTexts.GetCount();i++){
				CString& line = printTexts.ElementAt(i);
				if (m_sevType == UPDATA_APPLY){
					line.Replace(_T(" ����������ƾ֤"),_T("������������ƾ֤"));
				}	
				line.Replace(_T("{USER_ID}"),theAPP_SESSION.GetUserInfo().sUserID);				
				line.Replace(_T("{USER_NAME}"),m_UserInfo.user_name);
				line.Replace(_T("{SEX}"),m_UserInfo.sex==Sex_t::SEX_MALE? MALE:FEMALE);
				line.Replace(_T("{CERTIFI_TYPE}"),m_UserInfo.str_certificate_type);
				line.Replace(_T("{CERTIFI_NUM}"),m_UserInfo.certificate_number);
				line.Replace(_T("{DEVICE_CODE}"),theMAINTENANCE_INFO.GetCurrentACCDevice().ToString());
				line.Replace(_T("{STATION_NAME}"),theAPP_SESSION.GetStationNameCN());			
				line.Replace(_T("{TRANS_TIME}"),transDateTime.ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d")));
				CStringArray printLine;
				ComPrintLineFormatForTemplate(line,printLine);
				printArr.Append(printLine);
			}
		}
		m_pPRTHelper->PrinterPrint(&printArr);

	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
}
