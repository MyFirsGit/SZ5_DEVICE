#include "stdafx.h"
#include "AppSession.h"
#include "mainfrm.h"
#include "mainfrmrec.h"
#include "BOM.h"
#include "RWHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAppSession CAppSession::theInstance;

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
//	m_initDetailErrorMsg = _T("");
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
//@brief      ���ó��ڻ���ϸ��������
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
//@brief      ��ȡ���ڻ���ϸ��������
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
	return _ttoi(m_userInfo.sUserID) == theBOM_INFO.GetFounderStaffID();
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
@brief      ����վԱ��Ϣ

@param      (i)const USER_INFO& useInfo

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetUserInfo(const USER_INFO& userInfo)
{
	m_userInfo = userInfo;

    // �ڶ���״̬����ʾվԱ���
	CString strUserID = _T("");
	strUserID = m_userInfo.sUserID.Right(8);
    GetChildView()->m_pStatusDlg->SetUserID(strUserID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��վ��վ���ƣ����ģ�

@param      ��

@retval     CString  ���ص�վ��վ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CAppSession::GetStationNameCN() const
{
    return m_stationNameCN;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��վ��վ���ƣ�Ӣ�ģ�

@param      ��

@retval     CString  ���ص�վ��վ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CAppSession::GetStationNameEN() const
{
    return m_stationNameEN;
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
void CAppSession::SetStationName(const CString stationNameCN, const CString stationNameEN)
{
    m_stationNameCN = stationNameCN;
    m_stationNameEN = stationNameEN;
	//ShowReceptionServiceName(m_stationNameCN,m_stationNameEN);
    if (GetChildViewRec() != NULL && GetChildViewRec()->m_pReceptionHeadDlg != NULL) {
        GetChildViewRec()->m_pReceptionHeadDlg->SetStationName(stationNameCN, stationNameEN);
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
@brief      ��ʾվԱ��������Ϣ

@param      (i)sMsg	                ����Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::ShowOfficeGuide(CString sMsg)
{
	theGuideDlg.ShowGuideMsg(sMsg);
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
void CAppSession::ShowReceptionGuide(CString sMsg, CString sEnglishMsg)
{
    if (GetChildViewRec()->m_pReceptionGuideDlg != NULL) {
        GetChildViewRec()->m_pReceptionGuideDlg->ShowGuideMsg(sMsg, sEnglishMsg);
    }
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
void CAppSession::ShowAllGuide(CString sOfficeMsg, CString sReceptionMsg, CString sReceptionEnglishMsg)
{
    ShowOfficeGuide(sOfficeMsg);
    ShowReceptionGuide(sReceptionMsg, sReceptionEnglishMsg);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡҵ�����ƣ����ģ�

@param      (i)SERVICE_ID serviceID	ID of Service

@retval     CString ҵ������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CAppSession::GetServiceNameCH(SERVICE_ID serviceID)
{
    CString result = _T("");

    switch (serviceID) {
    case ISSUE_SVC:
        result = TXT_ISSUE_SVC_CN;             // ��Ʊ
        break;
    case ADJUST_SVC:
        result = TXT_ADJUST_SVC_CN;            // ��Ʊ
        break;
    case CHARGE_SVC:
        result = TXT_CHARGE_SVC_CN;            // ��ֵ
        break;
    case EXCHANGE_SVC:
        result = TXT_EXCHANGE_SVC_CN;          // ����
        break;
    case REFUND_SVC:
        result = TXT_REFUND_MAIN_CN;            // �˿�
        break;
    case REFRESH_SVC:
        result = TXT_REFRESH_SVC_CN;           // ����
        break;
    case ANALYZE_SVC:
        result = TXT_ANALYZE_SVC_CN;           // ����
        break;
    case OTHERS_SVC:
        result = TXT_OTHERS_SVC_CN;            // ����
        break;
	case TICKETBOX_EXCHANGE_SVC:
		result = TXT_TICKETBOX_EXCHANGE_SVC;//Ʊ�����
		break;
    case SYSTEM_SET_SVC:
        result = TXT_SYSTEM_SET_SVC_CN;        // ϵͳ�趨
        break;
    case DATA_MANAGE_SVC:
		result = TXT_DATA_MANAGE_SVC_CN;     // ���ݽ���
		break;
	case DATA_INPUT_SVC:
		result = TXT_DATA_INPUT_SVC_CN;     // ���ݵ���
		break;
	case DATA_OUTPUT_SVC:
        result = TXT_DATA_OUTPUT_SVC_CN;     // ���ݵ���
        break;
    case DEVICE_TEST_SVC:
        result = TXT_DEVICE_TEST_SVC;       // Ӳ���Լ�
        break;
    case REMOVE_EXCEPTION_SVC:
	case REMOVE_INIT_EXCEPTION_SVC:
        result = TXT_REMOVE_EXCEPTION_SVC;  // �쳣���
        break;
    case LOGIN_SVC:
        result = TXT_LOGIN_SVC;             // ��¼
        break;
    case LOGOFF_SVC:
        result = TXT_LOGOFF_SVC;            // �ǳ�
        break;
    case END_SVC:
        result = TXT_END_SVC;               // ҵ�����
        break;
    case SHUTDOWN_SVC:
        result = TXT_SHUTDOWN_SVC;          // �ػ�
        break;
	case EXIT_BOM_SVC:
		result = TXT_EXIT_BOM_SVC;			//�˳�����
		break;
	case RESTART_SVC:
		result = TXT_RESTART_SVC_CN;
		break;
    case TVMFAILURE_REFUND_SVC:
        result = TXT_TVMFAILURE_REFUND_SVC; // TVM�����˿�
        break;
    default:
        result = _T("");
        break;
    }

    return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡҵ�����ƣ�Ӣ�ģ�

@param      (i)SERVICE_ID serviceID	ID of Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CAppSession::GetServiceNameEN(SERVICE_ID serviceID)
{
    CString result = _T("");

    //switch (serviceID) {
    //case ISSUE_SVC:
    //    result = TXT_ISSUE_SVC_EN;             // ��Ʊ
    //    break;
    //case ADJUST_SVC:
    //    result = TXT_ADJUST_SVC_EN;            // ��Ʊ
    //    break;
    //case CHARGE_SVC:
    //    result = TXT_CHARGE_SVC_EN;            // ��ֵ
    //    break;
    //case UPDATE_SVC:
    //    result = TXT_UPDATE_SVC_EN;            // ����
    //    break;
    //case EXCHANGE_SVC:
    //    result = TXT_EXCHANGE_SVC_EN;          // ����
    //    break;
    //case REFUND_SVC:
    //    result = TXT_REFUND_SVC_EN;            // �˿�
    //    break;
    //case REFRESH_SVC:
    //    result = TXT_REFRESH_SVC_EN;           // ����
    //    break;
    //case ANALYZE_SVC:
    //    result = TXT_ANALYZE_SVC_EN;           // ����
    //    break;
    //case OTHERS_SVC:
    //    result = TXT_OTHERS_SVC_EN;            // ����
    //    break;

    //case TVMFAILURE_REFUND_SVC:
    //    result = TXT_TVMFAILURE_REFUND_SVC_EN; // TVM�����˿�
    //    break;
    //case PRODUCT_DELETE_SVC:
    //    result = TXT_PRODUCT_DELETE_SVC_EN;    // ��Ʒɾ��
    //    break;
    //case REVERSE_SVC:
    //    result = TXT_REVERSE_SVC_EN;           // ����
    //    break;
    //case LOCK_SVC:
    //    result = TXT_LOCK_SVC_EN;              // ����
    //    break;
        
    //default:
    //    result = _T("");
    //    break;
    //}

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

    if (pos > 0 && pos < 6) {   // ����Ϣ������ɫ˵��
        guideMsg = originalGuideMsg.Mid(pos + 2);
        CString colorStr = originalGuideMsg.Mid(0, 6);

        if (colorStr == _T("���죩")) {
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
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::IsFirstRun() const
//{
//    return m_isFirstRun;
//}
//
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
//void CAppSession::SetIsFirstRun(bool isFirst)
//{
//    m_isFirstRun = isFirst;
//}
//

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

