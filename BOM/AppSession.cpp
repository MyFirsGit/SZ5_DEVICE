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
//	m_initDetailErrorMsg = _T("");
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
//@brief      设置初期化详细错误文言
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
//@brief      获取初期化详细错误文言
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
	return _ttoi(m_userInfo.sUserID) == theBOM_INFO.GetFounderStaffID();
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
@brief      设置站员信息

@param      (i)const USER_INFO& useInfo

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::SetUserInfo(const USER_INFO& userInfo)
{
	m_userInfo = userInfo;

    // 在顶部状态条显示站员编号
	CString strUserID = _T("");
	strUserID = m_userInfo.sUserID.Right(8);
    GetChildView()->m_pStatusDlg->SetUserID(strUserID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取当站车站名称（中文）

@param      无

@retval     CString  返回当站车站名称

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CAppSession::GetStationNameCN() const
{
    return m_stationNameCN;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取当站车站名称（英文）

@param      无

@retval     CString  返回当站车站名称

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CAppSession::GetStationNameEN() const
{
    return m_stationNameEN;
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
@brief      显示站员画面向导消息

@param      (i)sMsg	                向导消息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAppSession::ShowOfficeGuide(CString sMsg)
{
	theGuideDlg.ShowGuideMsg(sMsg);
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
void CAppSession::ShowReceptionGuide(CString sMsg, CString sEnglishMsg)
{
    if (GetChildViewRec()->m_pReceptionGuideDlg != NULL) {
        GetChildViewRec()->m_pReceptionGuideDlg->ShowGuideMsg(sMsg, sEnglishMsg);
    }
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
void CAppSession::ShowAllGuide(CString sOfficeMsg, CString sReceptionMsg, CString sReceptionEnglishMsg)
{
    ShowOfficeGuide(sOfficeMsg);
    ShowReceptionGuide(sReceptionMsg, sReceptionEnglishMsg);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取业务名称（中文）

@param      (i)SERVICE_ID serviceID	ID of Service

@retval     CString 业务名称

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CAppSession::GetServiceNameCH(SERVICE_ID serviceID)
{
    CString result = _T("");

    switch (serviceID) {
    case ISSUE_SVC:
        result = TXT_ISSUE_SVC_CN;             // 售票
        break;
    case ADJUST_SVC:
        result = TXT_ADJUST_SVC_CN;            // 补票
        break;
    case CHARGE_SVC:
        result = TXT_CHARGE_SVC_CN;            // 充值
        break;
    case EXCHANGE_SVC:
        result = TXT_EXCHANGE_SVC_CN;          // 换卡
        break;
    case REFUND_SVC:
        result = TXT_REFUND_MAIN_CN;            // 退款
        break;
    case REFRESH_SVC:
        result = TXT_REFRESH_SVC_CN;           // 激活
        break;
    case ANALYZE_SVC:
        result = TXT_ANALYZE_SVC_CN;           // 分析
        break;
    case OTHERS_SVC:
        result = TXT_OTHERS_SVC_CN;            // 其他
        break;
	case TICKETBOX_EXCHANGE_SVC:
		result = TXT_TICKETBOX_EXCHANGE_SVC;//票箱更换
		break;
    case SYSTEM_SET_SVC:
        result = TXT_SYSTEM_SET_SVC_CN;        // 系统设定
        break;
    case DATA_MANAGE_SVC:
		result = TXT_DATA_MANAGE_SVC_CN;     // 数据结算
		break;
	case DATA_INPUT_SVC:
		result = TXT_DATA_INPUT_SVC_CN;     // 数据导入
		break;
	case DATA_OUTPUT_SVC:
        result = TXT_DATA_OUTPUT_SVC_CN;     // 数据导出
        break;
    case DEVICE_TEST_SVC:
        result = TXT_DEVICE_TEST_SVC;       // 硬件自检
        break;
    case REMOVE_EXCEPTION_SVC:
	case REMOVE_INIT_EXCEPTION_SVC:
        result = TXT_REMOVE_EXCEPTION_SVC;  // 异常解除
        break;
    case LOGIN_SVC:
        result = TXT_LOGIN_SVC;             // 登录
        break;
    case LOGOFF_SVC:
        result = TXT_LOGOFF_SVC;            // 登出
        break;
    case END_SVC:
        result = TXT_END_SVC;               // 业务结束
        break;
    case SHUTDOWN_SVC:
        result = TXT_SHUTDOWN_SVC;          // 关机
        break;
	case EXIT_BOM_SVC:
		result = TXT_EXIT_BOM_SVC;			//退出程序
		break;
	case RESTART_SVC:
		result = TXT_RESTART_SVC_CN;
		break;
    case TVMFAILURE_REFUND_SVC:
        result = TXT_TVMFAILURE_REFUND_SVC; // TVM故障退款
        break;
    default:
        result = _T("");
        break;
    }

    return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取业务名称（英文）

@param      (i)SERVICE_ID serviceID	ID of Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CAppSession::GetServiceNameEN(SERVICE_ID serviceID)
{
    CString result = _T("");

    //switch (serviceID) {
    //case ISSUE_SVC:
    //    result = TXT_ISSUE_SVC_EN;             // 售票
    //    break;
    //case ADJUST_SVC:
    //    result = TXT_ADJUST_SVC_EN;            // 补票
    //    break;
    //case CHARGE_SVC:
    //    result = TXT_CHARGE_SVC_EN;            // 充值
    //    break;
    //case UPDATE_SVC:
    //    result = TXT_UPDATE_SVC_EN;            // 更新
    //    break;
    //case EXCHANGE_SVC:
    //    result = TXT_EXCHANGE_SVC_EN;          // 更换
    //    break;
    //case REFUND_SVC:
    //    result = TXT_REFUND_SVC_EN;            // 退款
    //    break;
    //case REFRESH_SVC:
    //    result = TXT_REFRESH_SVC_EN;           // 激活
    //    break;
    //case ANALYZE_SVC:
    //    result = TXT_ANALYZE_SVC_EN;           // 分析
    //    break;
    //case OTHERS_SVC:
    //    result = TXT_OTHERS_SVC_EN;            // 其他
    //    break;

    //case TVMFAILURE_REFUND_SVC:
    //    result = TXT_TVMFAILURE_REFUND_SVC_EN; // TVM故障退款
    //    break;
    //case PRODUCT_DELETE_SVC:
    //    result = TXT_PRODUCT_DELETE_SVC_EN;    // 产品删除
    //    break;
    //case REVERSE_SVC:
    //    result = TXT_REVERSE_SVC_EN;           // 回退
    //    break;
    //case LOCK_SVC:
    //    result = TXT_LOCK_SVC_EN;              // 锁定
    //    break;
        
    //default:
    //    result = _T("");
    //    break;
    //}

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

    if (pos > 0 && pos < 6) {   // 向导信息包含颜色说明
        guideMsg = originalGuideMsg.Mid(pos + 2);
        CString colorStr = originalGuideMsg.Mid(0, 6);

        if (colorStr == _T("（红）")) {
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
////////////////////////////////////////////////////////////////////////////
//bool CAppSession::IsFirstRun() const
//{
//    return m_isFirstRun;
//}
//
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
//void CAppSession::SetIsFirstRun(bool isFirst)
//{
//    m_isFirstRun = isFirst;
//}
//

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

