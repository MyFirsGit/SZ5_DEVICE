#include "stdafx.h"
#include "AppSession.h"
#include "mainfrm.h"
#include "mainfrmrec.h"
#include "TVM.h"
//#include "RechargeRWHelper.h"
#include "tvmsetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAppSession CAppSession::theInstance;
LANGUAGETYPE_T CAppSession::m_currentRecpLanguage;
LANGUAGETYPE_T CAppSession::m_currentOperatorLanguage;
LANGUAGETYPE_T CAppSession::m_currentBlindPlayVoiceLanguage;

CAppSession::CAppSession()
 //   ,m_isCPSConnected(false)                // �Ƿ�����
 //   ,m_isCPSBinding(false)                  // ��λ�Ƿ��Ѿ���ͨ
 //   ,m_hasDeliverData(false)                // �Ƿ���δ������
 //   ,m_hasNewParam(false)                   // �Ƿ����λ�����²���
 //   ,m_isCDVersionChange(false)             // �Ƿ���CD�汾����
 //   ,m_hasProgUpdate(false)                 // �Ƿ��г������
 //   ,m_isFirstRun(false)                    // �Ƿ��һ�����г���
 //   ,m_closeStartFlag(CS_NORMAL)            // ���մ����־
 //   ,m_userInfo()                           // վԱ��Ϣ
 //   ,m_stationNameCN()                      // ��վ����
 //   ,m_stationNameEN()                      // ��վ����

 //   ,m_VersionInfo()                        // ����汾��Ϣ
 //   ,m_SAMisChanged()                       // SAMID�Ƿ�ı�
 //   ,m_InitCheckError(false)                // ���ڼ��ʧ��
	//,m_initDetailErrorFlag(0)               // ���ڻ���ϸ�����־
 //   ,m_isDownload(false)                    // �Ƿ�����FTP��
 //   ,m_needSendAR(false)                    // �Ƿ���AR������Ҫ����
	
 //   ,m_isUpdateParameter(false)             // ACC�����Ƿ����ڸ���
 //   ,m_isWorkingHourChange(false)           // �Ƿ���Ӫ���л�

{
	m_isTHConnected = false;                 // TH�˿��Ƿ�����
	m_isTOKENConnected = false;              // TOKEN�˿��Ƿ�����
	m_isCHConnected = false;                 // CH�˿��Ƿ�����
	m_isIOConnected = false;				 // IO�˿��Ƿ�����


	m_currentRecpLanguage = LANGUAGE_ID_CHINESE; 
	m_currentOperatorLanguage = LANGUAGE_ID_CHINESE; 

	m_guide_max = 0;
	m_guide_prefix = _T("");
	m_guide_input = _T("");
	m_guide_delete = _T("");
	m_isWarningStart = false;
	m_return_svcid = UNKNOWN_SVC;
	m_isDisableOrBlindOperation = false;
	m_isPhysicalConnStatus = false;
	m_isCloseOperationFlag = false;
	m_receipt_print_type = PRINT_TYPE_NONE;
	m_bNeedSyncMasterFile = false;
	m_isFirstRun = false;
	m_bRechargeServerEnabled = false;

	m_uBNRRecyclerNearEmpty_A = 0;
	m_uBNRRecyclerNearEmpty_B = 0;
	m_uBNRRecyclerNearEmpty_C = 0;
	m_uBNRRecyclerNearEmpty_D = 0;
	m_openMainDoorOperatorID = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ����Ķ���

@param      ��

@retval     CAppSession&    CAppSessionʵ������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAppSession& CAppSession::GetInstance()
{
    return theInstance;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      �ж�ҵ��ģʽ�Ƿ���λ�ı�
//
//@param      void
//
//@retval     bool    \n
//true:�ı� false��δ�ı�
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::IsOpModeChanged()
//{
//    if (thePARAM_MSG.m_BOM_PARAM_F.opMode != thePARAM_MSG.m_BOM_PARAM_F_new.opMode
//        || thePARAM_MSG.m_BOM_PARAM_F.GetOPMix() != thePARAM_MSG.m_BOM_PARAM_F_new.GetOPMix()) {
//        return true;
//    }
//    else {
//        return false;
//    }
//}
//

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ������״̬
//
//@param      void
//
//@retval     bool    \n
//            true:����   false������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsSCConnected() const
{
    return m_isSCConnected;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��������״̬
//
//@param      (i)bool CPSConnected     ��������״̬
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetSCConnected(bool SCConnected)
{
    m_isSCConnected = SCConnected;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������������ͨ��״̬

@param      bool true�������ӣ�false:δ����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetPhysicalConnStatus(bool bConnect){
	m_isPhysicalConnStatus = bConnect;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��������״̬

@param      ��

@retval     bool true�������ӣ�false:δ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::GetPhysicalConnStatus(){
	return m_isPhysicalConnStatus;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ������λ��ͨ״̬
//
//@param      bool isCPSBinding       ��λ��ͨ״̬
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetCpsBind(bool isCPSBinding)
//{
//    m_isCPSBinding = isCPSBinding;
//    // ˢ����Ļ
//    theDEBUG_LOG->WriteData("CAppSession", "SetCPSConnected", __LINE__, "PostMessage MessageID=0x%X, mode=%d", SP_MODE_CHANGE, SP_MODE_CPS_CONNECTION);
//    AfxGetMainWnd()->PostMessage(SP_MODE_CHANGE, SP_MODE_CPS_BIND, NULL);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      �ж���λ��ͨ״̬
//
//@param      ��
//
//@retval     bool    ��λ��ͨ״̬
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::IsCpsBind()
//{
//    return m_isCPSBinding;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���ó��ڻ�ʧ��
//
//@param      bool CheckError       ���ڻ�ʧ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetInitCheckError(bool CheckError)
{
   m_InitCheckError = CheckError;
}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      �жϳ��ڻ�ʧ��
//
//@param      ��
//
//@retval     bool    ���ڻ�ʧ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
bool CAppSession::IsInitCheckError() const
{
   return m_InitCheckError;
}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      �Ƿ����ĳ�����ڻ�����
//
//@param      (i) INIT_DETAIL_ERROR initDetailFlag
//
//@retval     bool    ���ڻ�ʧ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::IsInitDetailError(INIT_DETAIL_ERROR initDetailFlag)
//{
//	bool ret = false;
//	if (initDetailFlag & GetInitDetailErrorFlag()) {
//		ret = true;
//	}
//	return ret;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ����ĳ�����ڻ�����
//
//@param      (i) INIT_DETAIL_ERROR initDetailFlag
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetInitDetailErrorFlag(INIT_DETAIL_ERROR initDetailFlag)
{
	m_initDetailErrorFlag |= initDetailFlag;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���ڻ������־
//
//@param      (i) INIT_DETAIL_ERROR initDetailFlag
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::InitDetailErrorInfo()
//{
//	m_initDetailErrorFlag = 0;
//	m_initDetailErrorMsg = "";
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      �Ƴ�ĳ�����ڻ�����
//
//@param      (i) INIT_DETAIL_ERROR initDetailFlag
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::RemoveInitDetailErrorFlag(INIT_DETAIL_ERROR initDetailFlag)
//{
//	// �Ƿ���ڸô���
//	if (IsInitDetailError(initDetailFlag)) {
//		m_initDetailErrorFlag ^= initDetailFlag;
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ȡ���ڻ������־
//
//@param      none
//
//@retval     DWORD
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////
DWORD CAppSession::GetInitDetailErrorFlag()
{
	return m_initDetailErrorFlag;
}

//////////////////////////////////////////////////////////////////////////
///**
//@brief      �Ƿ������Ҫ��ֹ�ĳ��ڻ�����
//
//@param      none
//
//@retval     DWORD
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
DWORD CAppSession::IsInitErrorOver()
{
	if (INI_ERROR & GetInitDetailErrorFlag()) {
		return 1;
	}
	if (HOME_FOLDER_ERROR & GetInitDetailErrorFlag()) {
		return 2;
	}
	if (BACKUP_FOLDER_ERROR & GetInitDetailErrorFlag()) {
		return 3;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���ó��ڻ���ϸ�������ԣ�Ԥ����
//
//@param      (i) INIT_DETAIL_ERROR initDetailFlag
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetInitDetailErrorMsg(CString initDetailErrorMsg)
{
	m_initDetailErrorMsg = initDetailErrorMsg;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ȡ���ڻ���ϸ�������ԣ�Ԥ����
//
//@param      none
//
//@retval     CString
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////
CString CAppSession::GetInitDetailErrorMsg()
{
	return m_initDetailErrorMsg;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ������δ�����ݱ�־
//
//@param      void
//
//@retval     bool    \n
//true:��δ������ false����δ������
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::HasDeliverData() const
//{
//    return m_hasDeliverData;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��������δ�����ݱ�־
//
//@param      (i)bool hasDeliverData     ��������δ������
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetHasDeliverData(bool hasDeliverData)
//{
//    m_hasDeliverData = hasDeliverData;
//
//    // ˢ����Ļ
//    theDEBUG_LOG->WriteData("CAppSession", "SetHasDeliverData", __LINE__, "PostMessage MessageID=0x%X, mode=%d", SP_MODE_CHANGE, SP_MODE_DELIVER_DATA);
//    AfxGetMainWnd()->PostMessage(SP_MODE_CHANGE, SP_MODE_DELIVER_DATA, NULL);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ����汾��Ϣ����
//
//@param      (i)const VARIABLE_DATA& binData    ������Ϣ
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SaveVersion(const VARIABLE_DATA& binData)
//{
//    m_VersionInfo.nLen = binData.nLen;
//
//    if (m_VersionInfo.lpData != NULL) {
//        delete []m_VersionInfo.lpData;
//        m_VersionInfo.lpData = NULL;
//    }
//
//    if (binData.nLen != 0) {
//        m_VersionInfo.lpData = new BYTE[binData.nLen];
//        memcpy(m_VersionInfo.lpData, binData.lpData, binData.nLen);
//    }
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ȡ�汾��Ϣ����
//
//@param      (o)VARIABLE_DATA& binData    ������Ϣ
//
//@retval     VARIABLE_DATA       ������Ϣ 
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::GetVersion(VARIABLE_DATA& binData)
//{
//    binData.nLen = m_VersionInfo.nLen;
//
//    if (m_VersionInfo.nLen != 0) {
//        binData.lpData = new BYTE[binData.nLen];
//        memcpy(binData.lpData, m_VersionInfo.lpData, binData.nLen);
//    }
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����Ƿ��г������

@param      (i)bool flag    true:�� false:û��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetHasProgUpdate(bool flag)
{
    m_hasProgUpdate = flag;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ��г������

@param      ��

@retval     bool  \n true:�� false:û�� 

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::HasProgUpdate()
{
    return m_hasProgUpdate;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      �����Ƿ�����FTP��
//
//@param      (i)bool flag    ��־��Ϣ
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetIsDownload(bool flag)
//{
//    m_isDownload = flag;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ȡ�Ƿ�����FTP��
//
//@param      (i)bool flag    ��־��Ϣ
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::GetIsDownload() const
//{
//    return m_isDownload;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      �����Ƿ��������ز�����
//
//@param      (i)bool flag    ��־��Ϣ
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetIsUpdateParam(bool flag)
//{
//    m_isUpdateParameter = flag;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ȡ�Ƿ��������ز�����
//
//@param      (i)bool flag    ��־��Ϣ
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::GetIsUpdateParam() const
//{
//    return m_isUpdateParameter;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      �����Ƿ���Ӫ���л�
//
//@param      (i)bool flag    ��־��Ϣ
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetIsWorkingHourChange(bool flag)
//{
//    m_isWorkingHourChange = flag;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ȡ�Ƿ���Ӫ���л�
//
//@param      (i)bool flag    ��־��Ϣ
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::GetIsWorkingHourChange() const
//{
//    return m_isWorkingHourChange;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      �ж��Ƿ����²���������λ���أ�
//
//@param      ��
//
//@retval     bool    �Ƿ����²���������λ���أ�
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
bool CAppSession::HasNewParam() const
{
   return m_hasNewParam;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      �����Ƿ��Ƿ����²���������λ���أ�
//
//@param      bool hasNewParam       �Ƿ����²���
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetHasNewParam(bool hasNewParam)
{
   m_hasNewParam = hasNewParam;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      �����°汾��Ϣ
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
////void CAppSession::SetNewProgVer(const PROGRAM_VERSION& program)
////{
////    m_newProgramVer = program;
////}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ȡ�°汾��Ϣ
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
////void CAppSession::GetNewProgVer(PROGRAM_VERSION& program)
////{
////    program = m_newProgramVer;
////}
//
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж�SAMid�Ƿ�ı�

@param      (i)SAM_TYPE  samChange    SAM����
@param      (i)const LPBYTE  samId           SAMid

@retval     bool    �Ƿ�ı�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetSAMInfo(SAM_TYPE type,const LPBYTE samId)
{
    switch(type) {
    case RW_ACC_PSAM:
        {
            memcpy(m_SAMInfo.rw_acc_psam_id, samId, 6);
        }
    	break;
    case RW_ACC_ISAM:
        {
            memcpy(m_SAMInfo.rw_acc_isam_id, samId, 6);
        }
        break;
    case TH_ACC_ISAM:
        {
            memcpy(m_SAMInfo.th_acc_isam_id, samId, 6);
        }
        break;
    case TH_ACC_PSAM:
        {
            memcpy(m_SAMInfo.th_acc_psam_id, samId, 6);
        }
        break;
    case RW_ECT_ISAM:
        {
            memcpy(m_SAMInfo.rw_ect_isam_id, samId, 6);
        }
        break;
    case RW_ECT_PSAM:
        {
            memcpy(m_SAMInfo.rw_ect_psam_id, samId, 6);
        }
        break;
    case TH_ECT_ISAM:
        {
            memcpy(m_SAMInfo.th_ect_isam_id, samId, 6);
        }
        break;
    case TH_ECT_PSAM:
        {
            memcpy(m_SAMInfo.th_ect_psam_id, samId, 6);
        }
        break;
    default:
        break;
    };
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSAMid�Ƿ�ı�

@param      ��

@retval     SAM_CHANGE_FLAG    �Ƿ�ı�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SAM_ID_INFO CAppSession::GetSAMInfo()
{
    return m_SAMInfo;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      �����Ƿ���CD�汾����
//
//@param      bool isCDVersionChange       �Ƿ���CD�汾����
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetIsCDVersionChange(bool isCDVersionChange)
//{
//    m_isCDVersionChange = isCDVersionChange;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      �ж��Ƿ���CD�汾����
//
//@param      ��
//
//@retval     bool    �Ƿ����²���������λ���أ�
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::IsCDVersionChange() const
//{
//    return m_isCDVersionChange;
//}
//

////////////////////////////////////////////////////////////////////////////
///**
//  @brief      ��ȡ������������
//
//  @param      ��
//
//  @retval     CLOSE_START_FLAG   ���ص�ǰ������������
//
//  @exception �� 
//*/
////////////////////////////////////////////////////////////////////////////
CLOSE_START_FLAG CAppSession::GetCloseStartFlag() const
{
	return m_closeStartFlag;
}

////////////////////////////////////////////////////////////////////////////
///**
//  @brief      ���ù�����������
//
//  @param      const CLOSE_START_FLAG closeStartFlag 
//
//  @retval     ��
//
//  @exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetCloseStartFlag(const CLOSE_START_FLAG closeStartFlag)
{
	m_closeStartFlag = closeStartFlag;
}


bool CAppSession::IsSuperUser()
{
	return _ttoi(m_userInfo.sUserID) == theFunction_INFO.GetFounderStaffID();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡվԱ��Ϣ

@param      ��

@retval     USER_INFO  ���ص�ǰվԱ��Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
USER_INFO CAppSession::GetUserInfo() const
{
	return m_userInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ���վԱ��¼��

@param      ��

@retval     USER_INFO  ���ص�ǰվԱ��Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::HasUserLogin(){
	return m_bIsUserLogin;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����վԱ��¼״̬

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetUserLoginStatus(bool bIsUserLogin){
	m_bIsUserLogin = bIsUserLogin;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����վԱ��Ϣ

@param      (i)const USER_INFO& useInfo

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetUserInfo(const USER_INFO& userInfo)
{
	m_userInfo	   = userInfo;

	CString strUserID = _T("");
    // �ڶ���״̬����ʾվԱ���
	if(userInfo.sUserID.IsEmpty()){// �ǳ�����ʾID��N/A
		m_bIsUserLogin = false;
		GetChildView()->m_pStatusDlg->SetUserID(strUserID);
	}
	else{		
		strUserID = m_userInfo.sUserID.Right(8);
		GetChildView()->m_pStatusDlg->SetUserID(strUserID);
	}
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��վ��վ���ƣ����ģ�

@param      ��

@retval     CString  ���ص�վ��վ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CAppSession::GetStationName(LANGUAGETYPE_T languageType) const
{
	switch(languageType){
		case LANGUAGE_ID_ENGLISH:
			return m_stationNameEN;
		case LANGUAGE_ID_PORTUGUESE:
			return m_stationNamePN;
		case LANGUAGE_ID_CHINESE:
			return m_stationNameCN;
		default:
			return m_stationNameCN;
	}
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      ���õ�վ��վ����

@param      (i)const CString stationNameCN      ������
@param      (i)const CString stationNameEN      Ӣ����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetStationName(const CString stationNameCN, const CString stationNameEN, const CString stationNamePN)
{
    m_stationNameCN = stationNameCN;
    m_stationNameEN = stationNameEN;
	m_stationNamePN = stationNamePN;
    if (GetChildViewRec() != NULL && GetChildViewRec()->m_pReceptionHeadDlg != NULL) {
        GetChildViewRec()->m_pReceptionHeadDlg->SetStationName(stationNameCN, stationNameEN,stationNamePN);
    }
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��ChildView 
//
//@param      ��
//
//@retval     CChildView*, ȡ��ChildView
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
CChildView* CAppSession::GetChildView()
{
    return &((CMainFrm*)AfxGetMainWnd())->m_wndView;
}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ�ù˿ͻ����Ӵ� ChildViewRec 
//
//@param      ��
//
//@retval     CChildViewRec*, ȡ��ChildViewRec
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
CChildViewRec* CAppSession::GetChildViewRec()
{
    return &((CMainFrmRec*)theApp.GetMainWndRec())->m_wndView;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ�˿ͻ�������Ϣ

@param      (i)sMsg	                ����Ϣ
@param      (i)sEnglishMsg	        Ӣ������Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::ShowReceptionGuide(DWORD msgID )
{
    //if (GetChildViewRec()->m_pReceptionGuideDlg != NULL) {
    //    GetChildViewRec()->m_pReceptionGuideDlg->ShowGuideMsg(msgID);
    //}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ�˿ͻ�������Ϣ

@param      (i)sMsg	                ����Ϣ
@param      (i)sEnglishMsg	        Ӣ������Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::ShowReceptionGuide(CString recpMsgCn, CString recpMsgEn , CString recpMsgPn)
{
	//if (GetChildViewRec()->m_pReceptionGuideDlg != NULL) {
	//	GetChildViewRec()->m_pReceptionGuideDlg->ShowGuideMsg(recpMsgCn,recpMsgEn,recpMsgPn);
	//}
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ʾվԱ�ͳ˿ͻ�������Ϣ
//
//@param      (i)sOfficeMsg               վԱ����Ϣ
//@param      (i)sReceptionMsg	        �˿�����Ϣ
//@param      (i)sReceptionEnglishMsg	    �˿�Ӣ������Ϣ
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::ShowAllGuide(DWORD officeMsgID, DWORD recpMsgID )
{
	DeleteAllInput();
    ShowOfficeGuide(officeMsgID);
    ShowReceptionGuide(recpMsgID );
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ʾվԱ�ͳ˿ͻ�������Ϣ
//
//@param      (i)sOfficeMsg               վԱ����Ϣ
//@param      (i)sReceptionMsg	        �˿�����Ϣ
//@param      (i)sReceptionEnglishMsg	    �˿�Ӣ������Ϣ
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::ShowAllGuide(CString officeMsg, CString recpMsgCn, CString recpMsgEn , CString guidePoMsg)
{
	DeleteAllInput();
    ShowOfficeGuide(officeMsg);
    ShowReceptionGuide(recpMsgCn,recpMsgEn,guidePoMsg);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡҵ�����ƣ����ģ�

@param      (i)SERVICE_ID serviceID	ID of Service

@retval     CString ҵ������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CAppSession::GetServiceName(SERVICE_ID serviceID,LANGUAGETYPE_T languageType)
{
    CString result = _T("");
	long languageId = 0;

    switch (serviceID) {
    case ISSUE_SVC:
        languageId = TXT_ID_ISSUE_SVC;             // ��Ʊ
        break;
    case CHARGE_SVC:
        languageId = TXT_ID_CHARGE_SVC;            // ��ֵ
        break;
	case ISSUE_PRODUCT_SVC:
		languageId = TXT_ID_ADD_PRODUCT;            // ��Ӳ�Ʒ
		break;
    case ANALYZE_SVC:
        languageId = TXT_ID_ANALYZE_SVC;           // ����
        break;
    case SYSTEM_SET_SVC:
        languageId = TXT_ID_SYSTEM_SET_SVC;        // ϵͳ�趨
        break;
    case DATA_MANAGE_SVC:
		languageId = TXT_ID_DATA_MANAGE_SVC;     // ���ݽ���
		break;
	case DATA_INPUT_SVC:
		languageId = TXT_ID_DATA_INPUT_SVC;     // ���ݵ���
		break;
	case DATA_OUTPUT_SVC:
        languageId = TXT_ID_DATA_OUTPUT_SVC;     // ���ݵ���
        break;
	case VERSION_INFO_SVC:
		languageId = TXT_ID_TVM_VERSION_INFO_SVC;	// �汾��Ϣ
		break;
    case DEVICE_TEST_SVC:
        languageId = TXT_ID_DEVICE_TEST_SVC;       // Ӳ���Լ�
        break;
    case REMOVE_EXCEPTION_SVC:
// 	case REMOVE_INIT_EXCEPTION_SVC:
//         languageId = TXT_ID_REMOVE_INIT_EXCEPTION_SVC;  // �쳣���
//         break;
    case LOGIN_SVC:
        languageId = TXT_ID_LOGIN_SVC;             // ��¼
        break;
    case LOGOFF_SVC:
        languageId = TXT_ID_LOGOFF_SVC;            // �ǳ�
        break;
    case END_SVC:
        languageId = TXT_ID_END_SVC;               // ҵ�����
        break;
    case SHUTDOWN_SVC:
        languageId = TXT_ID_SHUTDOWN_SVC;          // �ػ�
        break;
	case EXIT_SVC:
		languageId = TXT_ID_EXIT_TOM_SVC;			//�˳�����
		break;
    default:
        languageId = TXT_UNDEFINE;
        break;
    }

	if (languageType == LANGUAGE_ID_UNSPECIFIED){
		languageType = theMAINTENANCE_INFO.GetLanguageMode();;
	}
	switch (languageType) {
	case LANGUAGE_ID_CHINESE:
		result = _cn(languageId);             
		break;
	case LANGUAGE_ID_ENGLISH:
		result = _en(languageId);             
		break;
	/*case LANGUAGE_ID_PORTUGUESE:
		result = _po(languageId);             
		break;*/
	default:
		result = _cn(TXT_UNDEFINE);
		break;
	}

	result.Remove('^');
    return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ó˿ͻ���ҵ������

@param      (i)CString nameCH   ҵ�����ƣ����ģ�
@param      (i)CString nameEN   ҵ�����ƣ�Ӣ�ģ�

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//void CAppSession::ShowReceptionServiceName(CString nameCH, CString nameEN)
//{
//    if (GetChildViewRec() != NULL && GetChildViewRec()->m_pReceptionHeadDlg != NULL) {
//        GetChildViewRec()->m_pReceptionHeadDlg->SetStationName(nameCH, nameEN);
//    }
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������Ϣ

@param      (i)originalGuideMsg	        ԭʼ����Ϣ�����죩XXXX
@param      (o)guideMsg	                ����Ϣ��XXXX
@param      (o)color	                ��ʾ��ɫ�����죩

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::ParseGuideMsg(CString originalGuideMsg, CString& guideMsg, COLORREF& color)
{
	int pos = originalGuideMsg.Find(_T("��"));

	if (pos ==-1){
		pos = originalGuideMsg.Find(_T(")"));
	}
	if (pos > 0 && pos < 7) {   // ����Ϣ������ɫ˵��
		guideMsg = originalGuideMsg.Mid(pos+1);
		if (originalGuideMsg.Find(_opl(LANGUAGE_RED))!=-1) {
			color = RED;
		}
		else {
			color = BLACK;
		}
	}
	else {
		guideMsg = originalGuideMsg;
		color = BLACK;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж�TH�˿��Ƿ�����

@param      ��

@retval     bool \n  true:����  false:������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsTHConnected() const
{
	return m_isTHConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����RW�˿��Ƿ�����

@param      bool \n  true:����  false:������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetIsTHConnected(bool isTHConnected)
{
	m_isTHConnected = isTHConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж�TH�˿��Ƿ�����

@param      ��

@retval     bool \n  true:����  false:������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsTOKENModuleConnected() const
{
	return m_isTOKENConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����RW�˿��Ƿ�����

@param      bool \n  true:����  false:������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetIsTOKENModuleConnected(bool isTOKENConnected)
{
	m_isTOKENConnected = isTOKENConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж�CH�˿��Ƿ�����

@param      ��

@retval     bool \n  true:����  false:������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsCHConnected() const
{
	return m_isCHConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����CH�˿��Ƿ�����

@param      bool \n  true:����  false:������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetIsCHConnected(bool isCHConnected)
{
	m_isCHConnected = isCHConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж�IO�˿��Ƿ�����

@param      bool \n  true:����  false:������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsIOConnected() const
{
	return m_isIOConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����IO�˿��Ƿ�����

@param      bool \n  true:����  false:������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetIsIOConnected(bool bIsConnected)
{
	m_isIOConnected = bIsConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж����п��˿��Ƿ�����

@param      bool \n  true:����  false:������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsBankCardConnected() const
{
	return m_isBankCardConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������п��˿��Ƿ�����

@param      bool \n  true:����  false:������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetIsBankCardConnected(bool bIsConnected)
{
	m_isBankCardConnected = bIsConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��𶯴����������Ƿ���

@param      ��

@retval     bool true:����  false��δ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsWarningStart()const
{
	return m_isWarningStart;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����𶯴���������״̬�Ƿ���

@param      bool \n  true:����  false:������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetIsWarningStart(bool bIsWarningStart)
{
	m_isWarningStart = bIsWarningStart;
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief      �ж��Ƿ�����һ������
//
//@param      ��
//
//@retval     bool \n  true:�ǵ�һ������  false:���ǵ�һ������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsFirstRun() const
{
    return m_isFirstRun;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      �����Ƿ�����һ������
//
//@param      bool \n  true:�ǵ�һ������  false:���ǵ�һ������
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetIsFirstRun(bool isFirst)
{
	m_isFirstRun = isFirst;
}


////////////////////////////////////////////////////////////////////////////
///**
//@brief      �����Ƿ���AR������Ҫ����
//
//@param      (i)bool need    �Ƿ���AR����
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetARToSend(bool need)
//{
//    m_needSendAR = need;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      �Ƿ���AR������Ҫ����
//
//@param      ��
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::HasARToSend()
//{
//    return m_needSendAR;
//}
//

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��֤�Ƿ�0x0ffa
//
//@param      (i)DWORD rsp    ��֤���
//
//@retval     bool \n
//			true �ǣ�false ����
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::isRspFFA(DWORD rsp)
//{
//	bool ret = false;
//	WORD cmpRsp = (WORD)rsp;
//	if (cmpRsp == 0x0ffa) {
//		ret = true;
//	}
//	return ret;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��֤�Ƿ�0x63CX
//
//@param      (i)DWORD rsp    ��֤���
//
//@retval     bool \n
//			true �ǣ�false ����
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::isRsp63CX(DWORD rsp)
//{
//	bool ret = false;
//	WORD cmpRsp = (WORD)rsp;
//	if (cmpRsp >= 0x63C0 && cmpRsp <= 0x63CF) {
//		ret = true;
//	}
//	return ret;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ����ǰ�δ�ӡԱ��ID
//
//@param      (i)CString printStaffID    ǰ�δ�ӡԱ��ID
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetRePrintStaffID(CString printStaffID)
//{
//	m_printStaffID = printStaffID;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ����ǰ�δ�ӡ��¼ʱ��
//
//@param      (i)_DATE_TIME printLoginTime    ǰ�δ�ӡ��¼ʱ��
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetRePrintLoginTime(_DATE_TIME printLoginTime)
//{
//	m_printLoginTime = printLoginTime;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ����ǰ�δ�ӡ��ID
//
//@param      (i)CString printGroupID    ǰ�δ�ӡ��ID
//
//@retval     void
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetRePrintGroupID(CString printGroupID)
//{
//	m_printGroupID = printGroupID;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ȡǰ�δ�ӡԱ��ID
//
//@param      none
//
//@retval     CString     ǰ�δ�ӡԱ��ID
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAppSession::GetRePrintStaffID()
//{
//	return m_printStaffID;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��ǰ�δ�ӡ��¼ʱ��
//
//@param      none
//
//@retval     _DATE_TIME     ǰ�δ�ӡ��¼ʱ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//_DATE_TIME CAppSession::GetRePrintLoginTime()
//{
//	return m_printLoginTime;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��ǰ�δ�ӡ��ID
//
//@param      none
//
//@retval     CString     ǰ�δ�ӡ��ID
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAppSession::GetRePrintGroupID()
//{
//	return m_printGroupID;
//}


////////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�õ�ǰ�˿ͽ�����������

@param      none

@retval     LANGUAGETYPE_T languageType ��ǰ�˿ͽ�����������

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
LANGUAGETYPE_T CAppSession::GetCurrentClientLanguageType()
{
	return m_currentRecpLanguage;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡä�˹�Ʊ���õ�����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LANGUAGETYPE_T CAppSession::GetCurrentBlindPlayVoiceLanguageType(){
	return m_currentBlindPlayVoiceLanguage;
}
////////////////////////////////////////////////////////////////////////////
/**
@brief      ���õ�ǰ�˿ͽ������������

@param      LANGUAGETYPE_T languageType  ��������

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetCurrentClientLanguageType(LANGUAGETYPE_T languageType)
{	
	if(languageType == LANGUAGETYPE_T::LANGUAGE_ID_CANTONESE){
		m_currentBlindPlayVoiceLanguage = languageType;
		languageType = LANGUAGETYPE_T::LANGUAGE_ID_CHINESE;
	}
	else
		m_currentBlindPlayVoiceLanguage = languageType;
	LANGUAGETYPE_T preMode = m_currentRecpLanguage;
	if(languageType!=preMode){
		m_currentRecpLanguage = languageType;
		ClientLanguageModeChanged.Invoke(languageType);
		theMAINTENANCE_INFO.SetLanguageMode(languageType);
	}
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�õ�ǰ����Ա������������

@param      none

@retval     LANGUAGETYPE_T languageType ��ǰ����Ա������������

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
LANGUAGETYPE_T CAppSession::GetCurrentOperatorLanguageType()
{
	return m_currentOperatorLanguage;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ���õ�ǰ����Ա�������������

@param      LANGUAGETYPE_T languageType  ��������

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetCurrentOperatorLanguageType(LANGUAGETYPE_T languageType)
{
	LANGUAGETYPE_T preMode = m_currentOperatorLanguage;
	if(languageType!=preMode){
		m_currentOperatorLanguage = languageType;
		OperatorLanguageModeChanged.Invoke(languageType);
	}
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ����������Ϣ

@param      (i)DWORD id ���Դ���

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::ShowOfficeGuide(DWORD id)
{
	DeleteAllInput();

	CString text;
	COLORREF color;
	CString raw = _opl(id);
	theAPP_SESSION.ParseGuideMsg(raw, text, color);
	theGuideDlg.ShowGuideMsg(text, color);
	SetGuideWhenDelete(raw);
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ����������Ϣ

@param      (i)CString msg ��������

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::ShowOfficeGuide(CString msg)
{
	DeleteAllInput();

	CString text;
	COLORREF color;
	theAPP_SESSION.ParseGuideMsg(msg, text, color);
	theGuideDlg.ShowGuideMsg(text, color);
	SetGuideWhenDelete(msg);
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ����ɾ��Ϊ��ʱ��������Ϣ

@param      (i)CString msg ��������

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetGuideWhenDelete(CString msg)
{
	m_guide_delete = msg;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡɾ��Ϊ��ʱ��������Ϣ

@param      ��

@retval     CString msg ��������

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
CString CAppSession::GetGuideWhenDelete()
{
	return m_guide_delete;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ����������ǰ׺

@param      int rhs ��������

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetPrefixToGuide(CString prefix)
{
	m_guide_prefix = prefix;
	m_guide_input = _T("");
	theGuideDlg.ShowGuideMsg(m_guide_prefix + m_guide_input);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ������ǰ׺

@param      ��

@retval     CString m_guide_prefix;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CAppSession::GetPrefixGuide(){
	return m_guide_prefix;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ׷�ӵ������ֵ��������ݡ����磺���ú���֮ǰ��������ʾ��Line Code: 1�������ú���֮��������ʾ��Line Code: 18����

@param      int number ��������

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::AppendOneNumToGuide(int number)
{
	if (m_guide_input.GetLength() < m_guide_max)
	{
		CString temp;
		temp.Format(_T("%d"), number);
		m_guide_input += temp;
		theGuideDlg.ShowGuideMsg(m_guide_prefix + m_guide_input);
	}
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ׷�ӵ���Բ�㡰.������������

@param      ��

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::AppendOneDotToGuide()
{
	if (m_guide_input.GetLength() < m_guide_max)
	{
		m_guide_input += _T(".");
		theGuideDlg.ShowGuideMsg(m_guide_prefix + m_guide_input);
	}
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ����������ɾ�������ַ������ֻ�Բ�㣩

@param      ��

@retval     bool  ture:���벻Ϊ��  false:����Ϊ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
bool CAppSession::DeleteOneCharFromGuide()
{
	bool empty_before_delete = true;

	// ɾ����������
	if (m_guide_input != _T(""))
	{
		empty_before_delete = false;
		m_guide_input.Delete(m_guide_input.GetLength() - 1);
	}

	// ɾ������ǰ׺
	if (empty_before_delete)
	{
		//SetPrefixToGuide(_T(""));
	}

	// ˢ������
	if (m_guide_input != _T("") || m_guide_prefix != _T(""))
	{
		theGuideDlg.ShowGuideMsg(m_guide_prefix + m_guide_input);
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ����ɾ����������������

@param      ��

@retval     bool  ture:����Ϊ��  false:���벻Ϊ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::DeleteAllInput()
{
	m_guide_prefix = _T("");
	m_guide_input = _T("");
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�������������ݡ�

@param      ��

@retval     int

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
int CAppSession::GetInputToInt()
{
	return _ttoi(m_guide_input);
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�������������ݡ�

@param      ��

@retval     CString

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
CString CAppSession::GetInputToString()
{
	return m_guide_input;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      �ж�ǰ׺�Ƿ�Ϊ�ա�

@param      ��

@retval     bool

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
bool CAppSession::IsEmptyPrefix()
{
	return m_guide_prefix == _T("") ? true : false;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ���ܵ�����������

@param      (i)SERVICE_ID serviceID ҵ��ID

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetMenuLevel(SERVICE_ID serviceID)
{
	if (theSERVICE_MGR.GetState() == MAINTENANCE || theSERVICE_MGR.GetState() == WAIT_MAINTENANCE){
		CMainSvc* pSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
		CNavigationDlg* dlg =  (CNavigationDlg*)(pSvc->GetDialogFlow()->GetDialog(CNavigationDlg::IDD));
		dlg->ShowLevel(serviceID);
	}
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ׷�Ӳ˵���������

@param      (i)CString level ׷�Ӽ���

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::AppendMenuLevel(CString level)
{
	if (theSERVICE_MGR.GetState() == MAINTENANCE){
		CMainSvc* pSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
		CNavigationDlg* dlg =  (CNavigationDlg*)(pSvc->GetDialogFlow()->GetDialog(CNavigationDlg::IDD));
		dlg->AppendLevel(level);
	}
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ����ָ��������ö��KEYBOARD��

@param      (i)DWORD keys ָ���İ������������ʱ���Խ��������
			��һ����KEY_NOBODY��KEY_NUMBER��KEY_DOT��KEY_CLEAR��KEY_ENTER��KEY_ESC��KEY_F1��KEY_F2��KEY_F3��KEY_F4��KEY_F5��KEY_F6
			��ϼ���KEY_INPUT��KEY_RETURN��KEY_PAGE

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetKeyboard(DWORD keys)
{
	if (theSERVICE_MGR.GetState() == MAINTENANCE || theSERVICE_MGR.GetState() == WAIT_MAINTENANCE)
	{
		CMainSvc* pSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
		CNavigationDlg* dlg =  (CNavigationDlg*)(pSvc->GetDialogFlow()->GetDialog(CNavigationDlg::IDD));
		dlg->SetKeyboard(keys);
	}
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ����ָ��������ö��KEYBOARD��

@param      (i)DWORD keys 
			��һ����KEY_NOBODY��KEY_NUMBER��KEY_DOT��KEY_CLEAR��KEY_ENTER��KEY_ESC��KEY_F1��KEY_F2��KEY_F3��KEY_F4��KEY_F5��KEY_F6
			��ϼ���KEY_INPUT��KEY_RETURN��KEY_PAGE

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::DiableKeyboard(DWORD keys)
{
	if (theSERVICE_MGR.GetState() == MAINTENANCE || theSERVICE_MGR.GetState() == WAIT_MAINTENANCE)
	{
		CMainSvc* pSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
		CNavigationDlg* dlg =  (CNavigationDlg*)(pSvc->GetDialogFlow()->GetDialog(CNavigationDlg::IDD));
		dlg->DiableKeyboard(keys);
	}
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      �����������λ��

@param      (i)int max λ��

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetMaxInputLength(int max)
{
	m_guide_max = (max < 0 || max >32) ? 0 : max;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ����ά��ҵ��˵���1���˵���2���˵���

@param      ��

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::ReturnMaintainMenu()
{
	m_bIsReturnMainMenu = true;
	m_return_svcid = (SERVICE_ID)theSERVICE_MGR.GetCurService()->GetServiceID();
	AppendMenuLevel(_T(""));
	theSERVICE_MGR.StartService(MAINTENANCE_SVC);
}
void CAppSession::SetMaintainMenu(SERVICE_ID serviceID)
{
	m_maintance_svcid = serviceID;
}
SERVICE_ID CAppSession::GetMaintainMenu()
{
	return m_maintance_svcid;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ���÷���ά��ҵ��˵���ID

@param      ��

@retval     SERVICE_ID ͨ��F3�˳���ҵ���ID

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
SERVICE_ID CAppSession::ResetReturnSeviceID()
{
	SERVICE_ID svcid = m_return_svcid;
	m_return_svcid = UNKNOWN_SVC;
	return svcid;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ��Ƿ������˵���

@param      bool bSetStatus;   false:����Ϊ��Ϊ����״̬

@retval     bool true:��  false:����

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
bool CAppSession::IsReturnMaintainMenu(bool bSetStatus){
	bool bTemp = m_bIsReturnMainMenu;
	m_bIsReturnMainMenu = bSetStatus;
	return bTemp;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ�澯��Ϣ��վԱ����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::ShowStatusWaringMsg(LANGUAGE_MSG strMsg,bool bIsShow /* = true */){
	GetChildView()->m_pStatusDlg->ShowStatusMsg(strMsg,bIsShow);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ж��Ƿ���ä�˹�Ʊ��

@param      ��

@retval     bool true:��ä�˹�Ʊ��false������ä�˹�Ʊ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsBlindManOperation(){
	return m_isDisableOrBlindOperation;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �����Ƿ���ä�˹�Ʊ

@param      bool true:��ä�˹�Ʊ false:����ä�˹�Ʊ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetBlindManStatus(bool bIsBlindMan){
	m_isDisableOrBlindOperation = bIsBlindMan;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ж��Ƿ����˹�ҵ�����

@param      ��

@retval     bool��true���ǣ���false����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsManualCloseOperation(){
	return m_isManualCloseOperation;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      �����Ƿ��˹�ҵ�����

@param      bool : true(�˹�ҵ�����);false(�Զ�ҵ�����)

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetManualCloseOperation(bool bIsManual){
	m_isManualCloseOperation = bIsManual;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʾ��Ϣ���˿ͽ������Ͻ�

@param      LANGUAGE_MSG inEmMessage;��Ҫ��ʾ����Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::ShowMessageToPassenger(LANGUAGE_MSG inEmMessage){
	m_EmShowMessage = inEmMessage;
	theTVM_STATUS_MGR.TVMStatusDispMessageChanged.Invoke();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡҪ��ʾ����Ϣ���˿�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LANGUAGE_MSG CAppSession::GetMessageShowPassenger(){
	return m_EmShowMessage;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������Ӫ������־

@param      (in)bool bIsCloseOperation;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetCloseOperationFlag(bool bIsCloseOperation){
	m_isCloseOperationFlag = bIsCloseOperation;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��Ӫ������־

@param      ��

@retval     (out) bool true(ҵ���������);false������ҵ����������У�;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::GetCloseOperationFlag()const{
	return m_isCloseOperationFlag;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ƾ֤��ӡ����

@param      const RECEIPT_PRINT_TYPE& type;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetReceiptPrintType(const RECEIPT_PRINT_TYPE& type){
	m_receipt_print_type = type;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��Ҫ��ӡƾ֤������

@param      ��

@retval     RECEIPT_PRINT_TYPE m_receipt_print_type;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
RECEIPT_PRINT_TYPE CAppSession::GetReceiptPrintType(){
	return m_receipt_print_type;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Ӳ��ģ���Ƿ���Ҫͬ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsCoinNeedSyncMasterFile(){
	return m_bNeedSyncMasterFile;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �����Ƿ���Ҫͬ������

@param      bool bNeed = true(��Ҫͬ��) false������Ҫͬ����;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetCoinNeedSyncMasterFile(bool bNeed /* = false*/){
	m_bNeedSyncMasterFile = bNeed;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief     ��ȡ�����ֽ��ѭ���Ľ���ֵ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CAppSession::GetBNRRecyclerNearEmpty(int recyclerNo)
{
	UINT nearEmptyNum = 0;
	if (recyclerNo == 1){
		nearEmptyNum = m_uBNRRecyclerNearEmpty_A;
	}
	else if (recyclerNo == 2){
		nearEmptyNum = m_uBNRRecyclerNearEmpty_B;
	}	
	else if (recyclerNo == 3){
		nearEmptyNum = m_uBNRRecyclerNearEmpty_C;
	}	
	else if (recyclerNo == 4){
		nearEmptyNum = m_uBNRRecyclerNearEmpty_D;
	}
	return nearEmptyNum;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ��ȡ�����ֽ��ѭ���Ľ���ֵ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetBNRRecyclerNearEmpty(int recyclerNo, UINT nearEmptyNum)
{
	if (recyclerNo == 1){
		m_uBNRRecyclerNearEmpty_A = nearEmptyNum;
	}
	else if (recyclerNo == 2){
		m_uBNRRecyclerNearEmpty_B = nearEmptyNum;
	}	
	else if (recyclerNo == 3){
		m_uBNRRecyclerNearEmpty_C = nearEmptyNum;
	}	
	else if (recyclerNo == 4){
		m_uBNRRecyclerNearEmpty_D = nearEmptyNum;
	}	
}

// ��ȡˢ������ά���ŵ�Ա��ID
int CAppSession::GetCardOpenMainDoorOperatorID() const
{
	return m_openMainDoorOperatorID;
}


// ����ˢ������ά���ŵ�Ա��ID
void CAppSession::SetCardOpenMainDoorOperatorID(int operatorID)
{
	m_openMainDoorOperatorID = operatorID;
}

// ���ó�ֵ�������Ƿ����
void CAppSession::SetRechargeServerEnabled(bool bEnabled)
{
	m_bRechargeServerEnabled = bEnabled;
}


// ��ȡ��ֵ�������Ƿ����
bool CAppSession::IsRechargeServerEnabled()
{
	return m_bRechargeServerEnabled;
}