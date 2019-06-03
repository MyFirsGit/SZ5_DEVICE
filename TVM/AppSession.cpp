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
 //   ,m_isCPSConnected(false)                // 是否在线
 //   ,m_isCPSBinding(false)                  // 上位是否已经接通
 //   ,m_hasDeliverData(false)                // 是否有未送数据
 //   ,m_hasNewParam(false)                   // 是否从上位下载新参数
 //   ,m_isCDVersionChange(false)             // 是否有CD版本更换
 //   ,m_hasProgUpdate(false)                 // 是否有程序更新
 //   ,m_isFirstRun(false)                    // 是否第一次运行程序
 //   ,m_closeStartFlag(CS_NORMAL)            // 日终处理标志
 //   ,m_userInfo()                           // 站员信息
 //   ,m_stationNameCN()                      // 车站名称
 //   ,m_stationNameEN()                      // 车站名称

 //   ,m_VersionInfo()                        // 程序版本信息
 //   ,m_SAMisChanged()                       // SAMID是否改变
 //   ,m_InitCheckError(false)                // 初期检查失败
	//,m_initDetailErrorFlag(0)               // 初期化详细错误标志
 //   ,m_isDownload(false)                    // 是否正在FTP中
 //   ,m_needSendAR(false)                    // 是否有AR数据需要发送
	
 //   ,m_isUpdateParameter(false)             // ACC参数是否正在更新
 //   ,m_isWorkingHourChange(false)           // 是否运营日切换

{
	m_isTHConnected = false;                 // TH端口是否连接
	m_isTOKENConnected = false;              // TOKEN端口是否连接
	m_isCHConnected = false;                 // CH端口是否连接
	m_isIOConnected = false;				 // IO端口是否连接


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
@brief      获取该类的对象

@param      无

@retval     CAppSession&    CAppSession实例对象

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAppSession& CAppSession::GetInstance()
{
    return theInstance;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      判断业务模式是否被上位改变
//
//@param      void
//
//@retval     bool    \n
//true:改变 false：未改变
//
//@exception  无
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
//@brief      取得在线状态
//
//@param      void
//
//@retval     bool    \n
//            true:在线   false：离线
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsSCConnected() const
{
    return m_isSCConnected;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置在线状态
//
//@param      (i)bool CPSConnected     设置在线状态
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetSCConnected(bool SCConnected)
{
    m_isSCConnected = SCConnected;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置物理连接通信状态

@param      bool true：已连接；false:未连接

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetPhysicalConnStatus(bool bConnect){
	m_isPhysicalConnStatus = bConnect;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取物理连接状态

@param      无

@retval     bool true：已连接；false:未连接

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::GetPhysicalConnStatus(){
	return m_isPhysicalConnStatus;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置上位接通状态
//
//@param      bool isCPSBinding       上位接通状态
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetCpsBind(bool isCPSBinding)
//{
//    m_isCPSBinding = isCPSBinding;
//    // 刷新屏幕
//    theDEBUG_LOG->WriteData("CAppSession", "SetCPSConnected", __LINE__, "PostMessage MessageID=0x%X, mode=%d", SP_MODE_CHANGE, SP_MODE_CPS_CONNECTION);
//    AfxGetMainWnd()->PostMessage(SP_MODE_CHANGE, SP_MODE_CPS_BIND, NULL);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      判断上位接通状态
//
//@param      无
//
//@retval     bool    上位接通状态
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::IsCpsBind()
//{
//    return m_isCPSBinding;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置初期化失败
//
//@param      bool CheckError       初期化失败
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetInitCheckError(bool CheckError)
{
   m_InitCheckError = CheckError;
}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      判断初期化失败
//
//@param      无
//
//@retval     bool    初期化失败
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
bool CAppSession::IsInitCheckError() const
{
   return m_InitCheckError;
}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      是否存在某个初期化错误
//
//@param      (i) INIT_DETAIL_ERROR initDetailFlag
//
//@retval     bool    初期化失败
//
//@exception  无
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
//@brief      设置某个初期化错误
//
//@param      (i) INIT_DETAIL_ERROR initDetailFlag
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetInitDetailErrorFlag(INIT_DETAIL_ERROR initDetailFlag)
{
	m_initDetailErrorFlag |= initDetailFlag;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      初期化错误标志
//
//@param      (i) INIT_DETAIL_ERROR initDetailFlag
//
//@retval     void
//
//@exception  无
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
//@brief      移除某个初期化错误
//
//@param      (i) INIT_DETAIL_ERROR initDetailFlag
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::RemoveInitDetailErrorFlag(INIT_DETAIL_ERROR initDetailFlag)
//{
//	// 是否存在该错误
//	if (IsInitDetailError(initDetailFlag)) {
//		m_initDetailErrorFlag ^= initDetailFlag;
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      获取初期化错误标志
//
//@param      none
//
//@retval     DWORD
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////
DWORD CAppSession::GetInitDetailErrorFlag()
{
	return m_initDetailErrorFlag;
}

//////////////////////////////////////////////////////////////////////////
///**
//@brief      是否存在需要中止的初期化错误
//
//@param      none
//
//@retval     DWORD
//
//@exception  无
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
//@brief      设置初期化详细错误文言（预留）
//
//@param      (i) INIT_DETAIL_ERROR initDetailFlag
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetInitDetailErrorMsg(CString initDetailErrorMsg)
{
	m_initDetailErrorMsg = initDetailErrorMsg;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      获取初期化详细错误文言（预留）
//
//@param      none
//
//@retval     CString
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////
CString CAppSession::GetInitDetailErrorMsg()
{
	return m_initDetailErrorMsg;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得有无未送数据标志
//
//@param      void
//
//@retval     bool    \n
//true:有未送数据 false：无未送数据
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::HasDeliverData() const
//{
//    return m_hasDeliverData;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置有无未送数据标志
//
//@param      (i)bool hasDeliverData     设置有无未送数据
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetHasDeliverData(bool hasDeliverData)
//{
//    m_hasDeliverData = hasDeliverData;
//
//    // 刷新屏幕
//    theDEBUG_LOG->WriteData("CAppSession", "SetHasDeliverData", __LINE__, "PostMessage MessageID=0x%X, mode=%d", SP_MODE_CHANGE, SP_MODE_DELIVER_DATA);
//    AfxGetMainWnd()->PostMessage(SP_MODE_CHANGE, SP_MODE_DELIVER_DATA, NULL);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      保存版本信息电文
//
//@param      (i)const VARIABLE_DATA& binData    电文信息
//
//@retval     void
//
//@exception  无
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
//@brief      获取版本信息电文
//
//@param      (o)VARIABLE_DATA& binData    电文信息
//
//@retval     VARIABLE_DATA       电文信息 
//
//@exception  无
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
@brief      设置是否有程序更新

@param      (i)bool flag    true:有 false:没有

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetHasProgUpdate(bool flag)
{
    m_hasProgUpdate = flag;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否有程序更新

@param      无

@retval     bool  \n true:有 false:没有 

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::HasProgUpdate()
{
    return m_hasProgUpdate;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置是否正在FTP中
//
//@param      (i)bool flag    标志信息
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetIsDownload(bool flag)
//{
//    m_isDownload = flag;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      获取是否正在FTP中
//
//@param      (i)bool flag    标志信息
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::GetIsDownload() const
//{
//    return m_isDownload;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置是否正在下载参数中
//
//@param      (i)bool flag    标志信息
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetIsUpdateParam(bool flag)
//{
//    m_isUpdateParameter = flag;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      获取是否正在下载参数中
//
//@param      (i)bool flag    标志信息
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::GetIsUpdateParam() const
//{
//    return m_isUpdateParameter;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置是否运营日切换
//
//@param      (i)bool flag    标志信息
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetIsWorkingHourChange(bool flag)
//{
//    m_isWorkingHourChange = flag;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      获取是否运营日切换
//
//@param      (i)bool flag    标志信息
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::GetIsWorkingHourChange() const
//{
//    return m_isWorkingHourChange;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      判断是否有新参数（从上位下载）
//
//@param      无
//
//@retval     bool    是否有新参数（从上位下载）
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
bool CAppSession::HasNewParam() const
{
   return m_hasNewParam;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置是否是否有新参数（从上位下载）
//
//@param      bool hasNewParam       是否有新参数
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetHasNewParam(bool hasNewParam)
{
   m_hasNewParam = hasNewParam;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      保存新版本信息
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
////void CAppSession::SetNewProgVer(const PROGRAM_VERSION& program)
////{
////    m_newProgramVer = program;
////}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      获取新版本信息
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
////void CAppSession::GetNewProgVer(PROGRAM_VERSION& program)
////{
////    program = m_newProgramVer;
////}
//
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断SAMid是否改变

@param      (i)SAM_TYPE  samChange    SAM类型
@param      (i)const LPBYTE  samId           SAMid

@retval     bool    是否改变

@exception  无
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
@brief      获取SAMid是否改变

@param      无

@retval     SAM_CHANGE_FLAG    是否改变

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
SAM_ID_INFO CAppSession::GetSAMInfo()
{
    return m_SAMInfo;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置是否有CD版本更换
//
//@param      bool isCDVersionChange       是否有CD版本更换
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetIsCDVersionChange(bool isCDVersionChange)
//{
//    m_isCDVersionChange = isCDVersionChange;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      判断是否有CD版本更换
//
//@param      无
//
//@retval     bool    是否有新参数（从上位下载）
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::IsCDVersionChange() const
//{
//    return m_isCDVersionChange;
//}
//

////////////////////////////////////////////////////////////////////////////
///**
//  @brief      获取工作处理类型
//
//  @param      无
//
//  @retval     CLOSE_START_FLAG   返回当前工作处理类型
//
//  @exception 无 
//*/
////////////////////////////////////////////////////////////////////////////
CLOSE_START_FLAG CAppSession::GetCloseStartFlag() const
{
	return m_closeStartFlag;
}

////////////////////////////////////////////////////////////////////////////
///**
//  @brief      设置工作处理类型
//
//  @param      const CLOSE_START_FLAG closeStartFlag 
//
//  @retval     无
//
//  @exception  无
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
@brief      获取站员信息

@param      无

@retval     USER_INFO  返回当前站员信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
USER_INFO CAppSession::GetUserInfo() const
{
	return m_userInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否有站员登录？

@param      无

@retval     USER_INFO  返回当前站员信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::HasUserLogin(){
	return m_bIsUserLogin;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置站员登录状态

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetUserLoginStatus(bool bIsUserLogin){
	m_bIsUserLogin = bIsUserLogin;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置站员信息

@param      (i)const USER_INFO& useInfo

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetUserInfo(const USER_INFO& userInfo)
{
	m_userInfo	   = userInfo;

	CString strUserID = _T("");
    // 在顶部状态条显示站员编号
	if(userInfo.sUserID.IsEmpty()){// 登出，显示ID：N/A
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
@brief      获取当站车站名称（中文）

@param      无

@retval     CString  返回当站车站名称

@exception  无
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
@brief      设置当站车站名称

@param      (i)const CString stationNameCN      中文名
@param      (i)const CString stationNameEN      英文名

@retval     无

@exception  无
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
//@brief      取得ChildView 
//
//@param      无
//
//@retval     CChildView*, 取得ChildView
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
CChildView* CAppSession::GetChildView()
{
    return &((CMainFrm*)AfxGetMainWnd())->m_wndView;
}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得顾客画面视窗 ChildViewRec 
//
//@param      无
//
//@retval     CChildViewRec*, 取得ChildViewRec
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
CChildViewRec* CAppSession::GetChildViewRec()
{
    return &((CMainFrmRec*)theApp.GetMainWndRec())->m_wndView;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示乘客画面向导消息

@param      (i)sMsg	                向导消息
@param      (i)sEnglishMsg	        英文向导消息

@retval     无

@exception  无
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
@brief      显示乘客画面向导消息

@param      (i)sMsg	                向导消息
@param      (i)sEnglishMsg	        英文向导消息

@retval     无

@exception  无
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
//@brief      显示站员和乘客画面向导消息
//
//@param      (i)sOfficeMsg               站员向导消息
//@param      (i)sReceptionMsg	        乘客向导消息
//@param      (i)sReceptionEnglishMsg	    乘客英文向导消息
//
//@retval     无
//
//@exception  无
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
//@brief      显示站员和乘客画面向导消息
//
//@param      (i)sOfficeMsg               站员向导消息
//@param      (i)sReceptionMsg	        乘客向导消息
//@param      (i)sReceptionEnglishMsg	    乘客英文向导消息
//
//@retval     无
//
//@exception  无
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
@brief      获取业务名称（中文）

@param      (i)SERVICE_ID serviceID	ID of Service

@retval     CString 业务名称

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CAppSession::GetServiceName(SERVICE_ID serviceID,LANGUAGETYPE_T languageType)
{
    CString result = _T("");
	long languageId = 0;

    switch (serviceID) {
    case ISSUE_SVC:
        languageId = TXT_ID_ISSUE_SVC;             // 售票
        break;
    case CHARGE_SVC:
        languageId = TXT_ID_CHARGE_SVC;            // 充值
        break;
	case ISSUE_PRODUCT_SVC:
		languageId = TXT_ID_ADD_PRODUCT;            // 添加产品
		break;
    case ANALYZE_SVC:
        languageId = TXT_ID_ANALYZE_SVC;           // 分析
        break;
    case SYSTEM_SET_SVC:
        languageId = TXT_ID_SYSTEM_SET_SVC;        // 系统设定
        break;
    case DATA_MANAGE_SVC:
		languageId = TXT_ID_DATA_MANAGE_SVC;     // 数据结算
		break;
	case DATA_INPUT_SVC:
		languageId = TXT_ID_DATA_INPUT_SVC;     // 数据导入
		break;
	case DATA_OUTPUT_SVC:
        languageId = TXT_ID_DATA_OUTPUT_SVC;     // 数据导出
        break;
	case VERSION_INFO_SVC:
		languageId = TXT_ID_TVM_VERSION_INFO_SVC;	// 版本信息
		break;
    case DEVICE_TEST_SVC:
        languageId = TXT_ID_DEVICE_TEST_SVC;       // 硬件自检
        break;
    case REMOVE_EXCEPTION_SVC:
// 	case REMOVE_INIT_EXCEPTION_SVC:
//         languageId = TXT_ID_REMOVE_INIT_EXCEPTION_SVC;  // 异常解除
//         break;
    case LOGIN_SVC:
        languageId = TXT_ID_LOGIN_SVC;             // 登录
        break;
    case LOGOFF_SVC:
        languageId = TXT_ID_LOGOFF_SVC;            // 登出
        break;
    case END_SVC:
        languageId = TXT_ID_END_SVC;               // 业务结束
        break;
    case SHUTDOWN_SVC:
        languageId = TXT_ID_SHUTDOWN_SVC;          // 关机
        break;
	case EXIT_SVC:
		languageId = TXT_ID_EXIT_TOM_SVC;			//退出程序
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
@brief      设置乘客画面业务名称

@param      (i)CString nameCH   业务名称（中文）
@param      (i)CString nameEN   业务名称（英文）

@retval     无

@exception  无
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
@brief      解析向导信息

@param      (i)originalGuideMsg	        原始向导信息：（红）XXXX
@param      (o)guideMsg	                向导信息：XXXX
@param      (o)color	                显示颜色：（红）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::ParseGuideMsg(CString originalGuideMsg, CString& guideMsg, COLORREF& color)
{
	int pos = originalGuideMsg.Find(_T("）"));

	if (pos ==-1){
		pos = originalGuideMsg.Find(_T(")"));
	}
	if (pos > 0 && pos < 7) {   // 向导信息包含颜色说明
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
@brief      判断TH端口是否连接

@param      无

@retval     bool \n  true:连接  false:不连接

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsTHConnected() const
{
	return m_isTHConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置RW端口是否连接

@param      bool \n  true:连接  false:不连接

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetIsTHConnected(bool isTHConnected)
{
	m_isTHConnected = isTHConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断TH端口是否连接

@param      无

@retval     bool \n  true:连接  false:不连接

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsTOKENModuleConnected() const
{
	return m_isTOKENConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置RW端口是否连接

@param      bool \n  true:连接  false:不连接

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetIsTOKENModuleConnected(bool isTOKENConnected)
{
	m_isTOKENConnected = isTOKENConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断CH端口是否连接

@param      无

@retval     bool \n  true:连接  false:不连接

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsCHConnected() const
{
	return m_isCHConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置CH端口是否连接

@param      bool \n  true:连接  false:不连接

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetIsCHConnected(bool isCHConnected)
{
	m_isCHConnected = isCHConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断IO端口是否连接

@param      bool \n  true:连接  false:不连接

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsIOConnected() const
{
	return m_isIOConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置IO端口是否连接

@param      bool \n  true:连接  false:不连接

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetIsIOConnected(bool bIsConnected)
{
	m_isIOConnected = bIsConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断银行卡端口是否连接

@param      bool \n  true:连接  false:不连接

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsBankCardConnected() const
{
	return m_isBankCardConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置银行卡端口是否连接

@param      bool \n  true:连接  false:不连接

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetIsBankCardConnected(bool bIsConnected)
{
	m_isBankCardConnected = bIsConnected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断震动传感器报警是否开启

@param      无

@retval     bool true:开启  false：未开启

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsWarningStart()const
{
	return m_isWarningStart;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置震动传感器报警状态是否开启

@param      bool \n  true:开启  false:不开启

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetIsWarningStart(bool bIsWarningStart)
{
	m_isWarningStart = bIsWarningStart;
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief      判断是否程序第一次启动
//
//@param      无
//
//@retval     bool \n  true:是第一次启动  false:不是第一次启动
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsFirstRun() const
{
    return m_isFirstRun;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置是否程序第一次启动
//
//@param      bool \n  true:是第一次启动  false:不是第一次启动
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetIsFirstRun(bool isFirst)
{
	m_isFirstRun = isFirst;
}


////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置是否有AR数据需要发送
//
//@param      (i)bool need    是否有AR数据
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetARToSend(bool need)
//{
//    m_needSendAR = need;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      是否有AR数据需要发送
//
//@param      无
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::HasARToSend()
//{
//    return m_needSendAR;
//}
//

////////////////////////////////////////////////////////////////////////////
///**
//@brief      认证是否0x0ffa
//
//@param      (i)DWORD rsp    认证结果
//
//@retval     bool \n
//			true 是，false 不是
//
//@exception  无
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
//@brief      认证是否0x63CX
//
//@param      (i)DWORD rsp    认证结果
//
//@retval     bool \n
//			true 是，false 不是
//
//@exception  无
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
//@brief      设置前次打印员工ID
//
//@param      (i)CString printStaffID    前次打印员工ID
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetRePrintStaffID(CString printStaffID)
//{
//	m_printStaffID = printStaffID;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置前次打印登录时间
//
//@param      (i)_DATE_TIME printLoginTime    前次打印登录时间
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetRePrintLoginTime(_DATE_TIME printLoginTime)
//{
//	m_printLoginTime = printLoginTime;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置前次打印班ID
//
//@param      (i)CString printGroupID    前次打印班ID
//
//@retval     void
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CAppSession::SetRePrintGroupID(CString printGroupID)
//{
//	m_printGroupID = printGroupID;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      获取前次打印员工ID
//
//@param      none
//
//@retval     CString     前次打印员工ID
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAppSession::GetRePrintStaffID()
//{
//	return m_printStaffID;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得前次打印登录时间
//
//@param      none
//
//@retval     _DATE_TIME     前次打印登录时间
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//_DATE_TIME CAppSession::GetRePrintLoginTime()
//{
//	return m_printLoginTime;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得前次打印班ID
//
//@param      none
//
//@retval     CString     前次打印班ID
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAppSession::GetRePrintGroupID()
//{
//	return m_printGroupID;
//}


////////////////////////////////////////////////////////////////////////////
/**
@brief      取得当前乘客界面语言类型

@param      none

@retval     LANGUAGETYPE_T languageType 当前乘客界面语言类型

@exception  无
*/
////////////////////////////////////////////////////////////////////////////
LANGUAGETYPE_T CAppSession::GetCurrentClientLanguageType()
{
	return m_currentRecpLanguage;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取盲人购票所用的语言

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LANGUAGETYPE_T CAppSession::GetCurrentBlindPlayVoiceLanguageType(){
	return m_currentBlindPlayVoiceLanguage;
}
////////////////////////////////////////////////////////////////////////////
/**
@brief      设置当前乘客界面的语言类型

@param      LANGUAGETYPE_T languageType  语言类型

@retval     无

@exception  无
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
@brief      取得当前操作员界面语言类型

@param      none

@retval     LANGUAGETYPE_T languageType 当前操作员界面语言类型

@exception  无
*/
////////////////////////////////////////////////////////////////////////////
LANGUAGETYPE_T CAppSession::GetCurrentOperatorLanguageType()
{
	return m_currentOperatorLanguage;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      设置当前操作员界面的语言类型

@param      LANGUAGETYPE_T languageType  语言类型

@retval     无

@exception  无
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
@brief      设置向导栏信息

@param      (i)DWORD id 文言代码

@retval     无

@exception  无
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
@brief      设置向导栏信息

@param      (i)CString msg 文言内容

@retval     无

@exception  无
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
@brief      设置删除为空时的向导栏信息

@param      (i)CString msg 文言内容

@retval     无

@exception  无
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetGuideWhenDelete(CString msg)
{
	m_guide_delete = msg;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      获取删除为空时的向导栏信息

@param      无

@retval     CString msg 文言内容

@exception  无
*/
////////////////////////////////////////////////////////////////////////////
CString CAppSession::GetGuideWhenDelete()
{
	return m_guide_delete;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      设置向导栏的前缀

@param      int rhs 最新内容

@retval     无

@exception  无
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
@brief      获取向导栏的前缀

@param      无

@retval     CString m_guide_prefix;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CAppSession::GetPrefixGuide(){
	return m_guide_prefix;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      追加单个数字到输入内容。例如：调用函数之前，向导栏显示“Line Code: 1”；调用函数之后，向导栏显示“Line Code: 18”。

@param      int number 最新内容

@retval     无

@exception  无
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
@brief      追加单个圆点“.”到输入内容

@param      无

@retval     无

@exception  无
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
@brief      从输入文言删除单个字符（数字或圆点）

@param      无

@retval     bool  ture:输入不为空  false:输入为空

@exception  无
*/
////////////////////////////////////////////////////////////////////////////
bool CAppSession::DeleteOneCharFromGuide()
{
	bool empty_before_delete = true;

	// 删除输入内容
	if (m_guide_input != _T(""))
	{
		empty_before_delete = false;
		m_guide_input.Delete(m_guide_input.GetLength() - 1);
	}

	// 删除输入前缀
	if (empty_before_delete)
	{
		//SetPrefixToGuide(_T(""));
	}

	// 刷新内容
	if (m_guide_input != _T("") || m_guide_prefix != _T(""))
	{
		theGuideDlg.ShowGuideMsg(m_guide_prefix + m_guide_input);
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      单字删除向导栏的输入内容

@param      无

@retval     bool  ture:输入为空  false:输入不为空

@exception  无
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::DeleteAllInput()
{
	m_guide_prefix = _T("");
	m_guide_input = _T("");
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      获取向导栏的输入内容。

@param      无

@retval     int

@exception  无
*/
////////////////////////////////////////////////////////////////////////////
int CAppSession::GetInputToInt()
{
	return _ttoi(m_guide_input);
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      获取向导栏的输入内容。

@param      无

@retval     CString

@exception  无
*/
////////////////////////////////////////////////////////////////////////////
CString CAppSession::GetInputToString()
{
	return m_guide_input;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      判断前缀是否为空。

@param      无

@retval     bool

@exception  无
*/
////////////////////////////////////////////////////////////////////////////
bool CAppSession::IsEmptyPrefix()
{
	return m_guide_prefix == _T("") ? true : false;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      获取功能导航栏的内容

@param      (i)SERVICE_ID serviceID 业务ID

@retval     无

@exception  无
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
@brief      追加菜单导航级别

@param      (i)CString level 追加级别

@retval     无

@exception  无
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
@brief      启用指定按键（枚举KEYBOARD）

@param      (i)DWORD keys 指定的按键，多个按键时可以进行与操作
			单一键：KEY_NOBODY、KEY_NUMBER、KEY_DOT、KEY_CLEAR、KEY_ENTER、KEY_ESC、KEY_F1、KEY_F2、KEY_F3、KEY_F4、KEY_F5、KEY_F6
			组合键：KEY_INPUT、KEY_RETURN、KEY_PAGE

@retval     无

@exception  无
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
@brief      禁用指定按键（枚举KEYBOARD）

@param      (i)DWORD keys 
			单一键：KEY_NOBODY、KEY_NUMBER、KEY_DOT、KEY_CLEAR、KEY_ENTER、KEY_ESC、KEY_F1、KEY_F2、KEY_F3、KEY_F4、KEY_F5、KEY_F6
			组合键：KEY_INPUT、KEY_RETURN、KEY_PAGE

@retval     无

@exception  无
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
@brief      设置最大输入位数

@param      (i)int max 位数

@retval     无

@exception  无
*/
////////////////////////////////////////////////////////////////////////////
void CAppSession::SetMaxInputLength(int max)
{
	m_guide_max = (max < 0 || max >32) ? 0 : max;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      返回维护业务菜单（1级菜单或2级菜单）

@param      无

@retval     无

@exception  无
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
@brief      重置返回维护业务菜单的ID

@param      无

@retval     SERVICE_ID 通过F3退出的业务的ID

@exception  无
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
@brief      是否是返回主菜单？

@param      bool bSetStatus;   false:设置为不为返回状态

@retval     bool true:是  false:不是

@exception  无
*/
////////////////////////////////////////////////////////////////////////////
bool CAppSession::IsReturnMaintainMenu(bool bSetStatus){
	bool bTemp = m_bIsReturnMainMenu;
	m_bIsReturnMainMenu = bSetStatus;
	return bTemp;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示告警信息给站员界面

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
@brief      判断是否是盲人购票？

@param      无

@retval     bool true:是盲人购票；false：不是盲人购票

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsBlindManOperation(){
	return m_isDisableOrBlindOperation;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置是否是盲人购票

@param      bool true:是盲人购票 false:不是盲人购票

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetBlindManStatus(bool bIsBlindMan){
	m_isDisableOrBlindOperation = bIsBlindMan;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      判断是否是人工业务结束

@param      无

@retval     bool：true（是）；false（否）

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsManualCloseOperation(){
	return m_isManualCloseOperation;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      设置是否人工业务结束

@param      bool : true(人工业务结束);false(自动业务结束)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetManualCloseOperation(bool bIsManual){
	m_isManualCloseOperation = bIsManual;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      显示信息到乘客界面右上角

@param      LANGUAGE_MSG inEmMessage;需要提示的信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::ShowMessageToPassenger(LANGUAGE_MSG inEmMessage){
	m_EmShowMessage = inEmMessage;
	theTVM_STATUS_MGR.TVMStatusDispMessageChanged.Invoke();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取要显示的信息给乘客

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LANGUAGE_MSG CAppSession::GetMessageShowPassenger(){
	return m_EmShowMessage;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置运营结束标志

@param      (in)bool bIsCloseOperation;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetCloseOperationFlag(bool bIsCloseOperation){
	m_isCloseOperationFlag = bIsCloseOperation;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取运营结束标志

@param      无

@retval     (out) bool true(业务结束序列);false（不再业务结束序列中）;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::GetCloseOperationFlag()const{
	return m_isCloseOperationFlag;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置凭证打印类型

@param      const RECEIPT_PRINT_TYPE& type;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetReceiptPrintType(const RECEIPT_PRINT_TYPE& type){
	m_receipt_print_type = type;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取需要打印凭证的类型

@param      无

@retval     RECEIPT_PRINT_TYPE m_receipt_print_type;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
RECEIPT_PRINT_TYPE CAppSession::GetReceiptPrintType(){
	return m_receipt_print_type;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      硬币模块是否需要同步计数？

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAppSession::IsCoinNeedSyncMasterFile(){
	return m_bNeedSyncMasterFile;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置是否需要同步计数

@param      bool bNeed = true(需要同步) false（不需要同步）;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetCoinNeedSyncMasterFile(bool bNeed /* = false*/){
	m_bNeedSyncMasterFile = bNeed;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief     获取保存的纸币循环鼓将空值

@param      无

@retval     无

@exception  无
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
@brief     获取保存的纸币循环鼓将空值

@param      无

@retval     无

@exception  无
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

// 获取刷卡开后维修门的员工ID
int CAppSession::GetCardOpenMainDoorOperatorID() const
{
	return m_openMainDoorOperatorID;
}


// 设置刷卡开后维修门的员工ID
void CAppSession::SetCardOpenMainDoorOperatorID(int operatorID)
{
	m_openMainDoorOperatorID = operatorID;
}

// 设置充值服务器是否可用
void CAppSession::SetRechargeServerEnabled(bool bEnabled)
{
	m_bRechargeServerEnabled = bEnabled;
}


// 获取充值服务器是否可用
bool CAppSession::IsRechargeServerEnabled()
{
	return m_bRechargeServerEnabled;
}