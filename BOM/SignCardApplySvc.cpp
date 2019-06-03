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
	// 初始化画面信息
    SetUserInfo();
	
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_02801_SIGN_CARD_APPLY_BASE_DLG, new CSignCardApplyBaseOfficeDlg(this));
	m_pDialogFlow->AddDialog(IDD_02803_SIGN_CARD_APPLY_CHECK_DLG, new CSignCardApplyCheckOfficeDlg(this));
	m_pDialogFlow->AddDialog(IDD_02802_SIGN_CARD_APPLY_OVER_DLG, new CSignCardApplyOverOfficeDlg(this));
	//设置乘客画面
	m_pDialogFlow->AddDialog(IDD_02801_SIGN_CARD_APPLY_DLG, new CWelcomeReceptionDlg(this));

	// 设置申请画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_02801_SIGN_CARD_APPLY_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_02801_SIGN_CARD_APPLY_DLG);

	// 设置申请结束画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_OVER, theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SUCCEED));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_OVER, IDD_02802_SIGN_CARD_APPLY_OVER_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_OVER, IDD_02801_SIGN_CARD_APPLY_DLG);

	// 设置查询画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_CHECK, theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CHECK, IDD_02803_SIGN_CARD_APPLY_CHECK_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CHECK, IDD_02801_SIGN_CARD_APPLY_DLG);

	// 设置查询结束画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_CHECK_OVER, theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CHECK_OVER, IDD_02802_SIGN_CARD_APPLY_OVER_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CHECK_OVER, IDD_02801_SIGN_CARD_APPLY_DLG);

	// 设置申请错误画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR, theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, IDD_02802_SIGN_CARD_APPLY_OVER_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, IDD_02801_SIGN_CARD_APPLY_DLG);

	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_BASE_TO_OVER,DIALOG_GROUP_OVER);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_BASE_TO_CHECK,DIALOG_GROUP_CHECK);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_BASE_TO_ERROR,DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_CHECK,WM_CHECK_TO_CHECK_OVER,DIALOG_GROUP_CHECK_OVER);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_CHECK,WM_CHECK_TO_CHECK_ERROR,DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_CHECK,WM_CHECK_TO_BASE,DIALOG_GROUP_BASE);

	//设置初始画面组
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
@brief      初始化基本信息数据构成

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSignCardApplySvc::SetUserInfo()
{
	int i=0;
	// 首次申请 || 申请变更
	if(m_sevType ==FIRST_APPLY ||m_sevType == UPDATA_APPLY) {
		// 姓名
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_NAME) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.user_name;
		i++;
		// 性别
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_SEX) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.sex==Sex_t::SEX_MALE? MALE:FEMALE;
		i++;
		// 证件类型
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_TYPE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.str_certificate_type;
		i++;
		// 证件编号
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_NUM) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.certificate_number;
		i++;
		// 证件有效期
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_VALIDATE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		if(ComCheckDateTimeValid(m_UserInfo.validate_date)==0){
			m_userInfo[i].nameRight = m_UserInfo.validate_date.ToString(_T("%.4d年%.2d月%.2d日" ));
		}
		else{
			m_userInfo[i].nameRight = "  --  年  -- 月 --  日";
		}	
		i++;
		// 电话
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_PHONE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.Phone==""?TXT_NOTHING:m_UserInfo.Phone;
		i++;
		// Email
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_EMAIL) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.Email==""?TXT_NOTHING:m_UserInfo.Email;
		i++;
		// 地址1
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_FIRST_ADDR) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.first_addr==""?TXT_NOTHING:m_UserInfo.first_addr;
		i++;
		// 地址2
		m_userInfo[i].nameLeft = add_wsp(CString(TXT_USER_SECND_ADDE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_userInfo[i].nameRight = m_UserInfo.second_addr==""?TXT_NOTHING:m_UserInfo.second_addr;
		i++;		
	}
	// 申请查询
	else if (m_sevType == CHECK_APPLY)	{
		// 证件类型
		m_checkInfo[i].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_TYPE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_checkInfo[i].nameRight = m_UserInfo.str_certificate_type;
		i++;
		// 证件编号
		m_checkInfo[i].nameLeft = add_wsp(CString(TXT_USER_CERTIFICATE_NUM) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_checkInfo[i].nameRight = m_UserInfo.certificate_number;
		i++;
		// 查询结束画面
		if (m_pDialogFlow->GetCurDialogGroupID() ==DIALOG_GROUP_CHECK_OVER){
			// 当前状态
			m_checkInfo[i].nameLeft = add_wsp(CString(TXT_USER_CURRENT_STATE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
			m_checkInfo[i].nameRight= m_checkResult==""?TXT_NOTHING:m_checkResult; 
			i++;
		}
	}
	m_LableCount = i;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      获取查询画面显示的用户信息个数

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
@brief      获取卡片信息

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CCPUCard CSignCardApplySvc::GetCardInfo()
{
	// 姓名
	m_UserInfo.user_name = m_CpuCard.personalData.Name;
	// 性别
	((CSignCardApplyBaseOfficeDlg*)m_pDialogFlow->GetDialog(IDD_02801_SIGN_CARD_APPLY_BASE_DLG))->ClickSexButton(m_CpuCard.personalData.Sex);
	// 证件类型
	((CSignCardApplyBaseOfficeDlg*)m_pDialogFlow->GetDialog(IDD_02801_SIGN_CARD_APPLY_BASE_DLG))->ClickCredentialsTypeButton(m_CpuCard.personalData.CredentialsType);
	// 证件编号
	m_UserInfo.certificate_number = m_CpuCard.personalData.CredentialsNumber;
	SetUserInfo();
	return m_CpuCard;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief    获取查询画面显示的用户信息     

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
@brief   设置性别   

@param     (i)BYTE sex 性别

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
@brief   设置证件类型   

@param     (i) BYTE cType, 证件类型代码
		   (i) CString strType    证件名称

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
@brief      获得车票信息

@param      (i)  UINT errCode              错误代码
@param      (i)  CRWReadCard* rwReadCard   读卡命令

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
		// 读卡失败
		if(resultCode != RW_RSP_OK){
			// 黑名单卡
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
			// 其他错误
			else{
				if(pRWReadCardResponse->DetailErrorCode != RW_ERROR_CARD_NO_CARD){
					OnCardError(m_pRWHelper->GetTpuErrorInfo(resultCode));			
				}
				else{
				}
			}
		}
		// 读卡成功
		else{
			// 保存读卡数据
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
@brief    保存读卡数据

@param      (i) LPRW_READ_CARD_RESPONSE pRWReadCardResponse 读卡数据

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
@brief      出错处理

@param      CString errorMsg 错误信息

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
@brief      锁卡处理

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
		// 锁卡成功
		if(RW_RSP_OK == resultCode){
			// 更新交易流水号
			theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);	

			// 更新UD数据
			if(response.UDlen != 0){
				m_pRWHelper->SaveRWUDData(response.UDlen,response.UDData);
			}

			// 更新AR数据
			if(response.ARLen != 0){
				m_pRWHelper->SaveRWARData(response.ARLen,response.ARData,response.TranResult.CardType);
			}
		}
	} 
	return resultCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     设置服务类型 

@param      (i) BYTE cType, 服务类型代码
			(i) CString strType    服务类型名称

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSignCardApplySvc::SetSevType(BYTE cType,int sevType)
{
	m_sevType = sevType;
	// 申请查询
	if(m_sevType == CHECK_APPLY&&m_pDialogFlow->GetCurDialogGroupID() !=DIALOG_GROUP_CHECK){
		m_pDialogFlow->ShowNextDialogGroup(WM_BASE_TO_CHECK); 
	}	
	// 首次申请
	if((m_sevType == FIRST_APPLY)&&m_pDialogFlow->GetCurDialogGroupID() ==DIALOG_GROUP_CHECK){
		m_pDialogFlow->ShowNextDialogGroup(WM_CHECK_TO_BASE); 
	}	
	// 申请变更
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
@brief     获取服务类型

@param      (i) BYTE cType, 服务类型代码
(i) CString strType    服务类型名称

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
@brief      响应确定按钮

@param      无

@retval      0:成功  非0：失败

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CSignCardApplySvc::OK()
{
	try{
		theSERVICE_MGR.SetState(ISSUEING);
		// 查询状态
		if(m_sevType == CHECK_APPLY&&m_pDialogFlow->GetCurDialogGroupID() ==DIALOG_GROUP_CHECK){
			// 获取录入信息
			CSignCardApplyCheckOfficeDlg* resDlg = (CSignCardApplyCheckOfficeDlg*)m_pDialogFlow->GetDialog(IDD_02803_SIGN_CARD_APPLY_CHECK_DLG);
			m_querryInfo.certificateType = m_UserInfo.certificate_type;
			m_UserInfo.certificate_number = resDlg->GetEditText(0);
			int bufSize = m_UserInfo.certificate_number.GetLength();
			memcpy(m_querryInfo.certificateID,m_UserInfo.certificate_number.GetBuffer(),bufSize<sizeof(UserInfo.certificateID)? bufSize:sizeof(UserInfo.certificateID));
			
			// 查询本地申请结果
			BYTE querry[21];
			LPBYTE lquery = querry;
			memcpy(lquery,&m_querryInfo.certificateType,1);
			lquery++;
			memcpy(lquery,m_querryInfo.certificateID,sizeof(m_querryInfo.certificateID));
			BYTE checkStatus;
			bool isChecked = theNAMED_CARD_APPLY_RRSULT.GetTheApplyResult(querry,checkStatus);
			// 本地记录不存在
			if(!isChecked){	
				// 发送查询请求
				theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->SendQuerySignCardApplyStatus(m_querryInfo);
				int count = 0;
				do{
					if(count == 12){
						checkStatus = 0;	
						// 查询失败
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
			// SC审计
			SaveSCData(CHECK_APPLY);
		}
		// 记名卡申请
		if((m_sevType == FIRST_APPLY||m_sevType ==UPDATA_APPLY)&&m_pDialogFlow->GetCurDialogGroupID() ==DIALOG_GROUP_BASE){
			// 获取录入信息
			CSignCardApplyBaseOfficeDlg* resDlg = (CSignCardApplyBaseOfficeDlg*)m_pDialogFlow->GetDialog(IDD_02801_SIGN_CARD_APPLY_BASE_DLG);
			// 姓名
			m_UserInfo.user_name = resDlg->GetEditText(0);
			int bufSize = m_UserInfo.user_name.GetLength();
			memcpy(UserInfo.name,m_UserInfo.user_name.GetBuffer(),bufSize<sizeof(UserInfo.name)? bufSize:sizeof(UserInfo.name));
			// 证件编号
			m_UserInfo.certificate_number = resDlg->GetEditText(1);
			bufSize = m_UserInfo.certificate_number.GetLength();
			memcpy(UserInfo.certificateID,m_UserInfo.certificate_number.GetBuffer(),bufSize<sizeof(UserInfo.certificateID)? bufSize:sizeof(UserInfo.certificateID));
			// 证件有效日期
			m_UserInfo.validate_date.wYear = _ttoi(resDlg->GetEditText(2));
			m_UserInfo.validate_date.biMonth = _ttoi(resDlg->GetEditText(3));
			m_UserInfo.validate_date.biDay = _ttoi(resDlg->GetEditText(4));
			m_UserInfo.validate_date.biHour=0;
			m_UserInfo.validate_date.biMinute=0;
			m_UserInfo.validate_date.biSecond=0;
			UserInfo.cardValidity = m_UserInfo.validate_date;
			// 电话
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
			// 邮箱
			m_UserInfo.Email = resDlg->GetEditText(6);
			bufSize = m_UserInfo.Email.GetLength();
			if(bufSize !=0){
				memcpy(UserInfo.email,m_UserInfo.Email.GetBuffer(),bufSize<sizeof(UserInfo.email)? bufSize:sizeof(UserInfo.email));
			}
			// 地址1
			m_UserInfo.first_addr = resDlg->GetEditText(7);
			bufSize = m_UserInfo.first_addr.GetLength();
			if(bufSize !=0){
				memcpy(UserInfo.address1,m_UserInfo.first_addr.GetBuffer(),bufSize<sizeof(UserInfo.address1)? bufSize:sizeof(UserInfo.address1));
			}
			// 地址2
			m_UserInfo.second_addr = resDlg->GetEditText(8);
			bufSize = m_UserInfo.second_addr.GetLength();
			if(bufSize !=0){
				memcpy(UserInfo.address2,m_UserInfo.second_addr.GetBuffer(),bufSize<sizeof(UserInfo.address2)? bufSize:sizeof(UserInfo.address2));
			}			
			// 首次申请
			if(m_sevType == FIRST_APPLY){
				// 保存申请记录到本地
				BYTE localContent[21];
				LPBYTE lpBillNum = localContent;
				localContent[0] = UserInfo.certificateType;
				memcpy(localContent+1,UserInfo.certificateID,sizeof(UserInfo.certificateID));
				BYTE status = 0;
				theNAMED_CARD_APPLY_RRSULT.UpdateTheApplyResult(lpBillNum,status);
				// 保存交易数据
				theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertSignCardApply(UserInfo);
				theSERVICE_MGR.SetState(FINISH);
				m_pDialogFlow->ShowNextDialogGroup(WM_BASE_TO_OVER);
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SUCCEED));
				// SC审计
				SaveSCData(FIRST_APPLY);
			}
			// 申请变更
			else{
				//写卡数据
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
					// 更新交易流水号
					theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);	
					// 更新UD数据
					if(response.UDlen != 0){
						m_pRWHelper->SaveRWUDData(response.UDlen,response.UDData);
					}
					// 更新AR数据
					if(response.ARLen != 0){
						m_pRWHelper->SaveRWARData(response.ARLen,response.ARData,m_CpuCard.applicationData.CardAppMainType);
					}
					// SC审计
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
@brief      保存一卡通充值sc数据

@param      none

@retval  	long	错误信息

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
@brief      判断打印机是否正常

@param      无

@retval     bool true:  false:异常正常

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//bool CSignCardApplySvc::IsPrinterException() 
//{
//	return m_pPRTHelper->IsPrinterHasException();
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打印数据、打开钱箱

@param      无

@retval     无

@exception  无
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
					line.Replace(_T(" 记名卡申请凭证"),_T("记名卡申请变更凭证"));
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
