#include "stdafx.h"
#include "ParamMsg.h"
#include "ParamHelper.h"
#include "maintenanceinfo.h"

CParamMsg CParamMsg::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      CParamMsg构造函数
@param      无
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
CParamMsg::CParamMsg()
{
	SetAccParameterVersion();
	SetAfcParameterVersion();
	SetECTParameterVersion();
	m_wDelayMode = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CParamMsg析构函数
@param      无
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
CParamMsg::~CParamMsg()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得实例
@param      无
@retval     CParamMsg&
*/
//////////////////////////////////////////////////////////////////////////
CParamMsg& CParamMsg::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取ACC参数版本（含当前版和将来版）
@param      无
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CParamMsg::SetAccParameterVersion()
{
 //   // ACC黑名单
	//m_cur_version_acc.lBlackListParam = theACC_BLACKLIST.GetCurMasterVer();
	//m_future_version_acc.lBlackListParam = theACC_BLACKLIST.GetFutMasterVer();
 //   // ACC业务规则参数
	//m_cur_version_acc.lBusinessListParam = theACC_BUSINESS.GetCurMasterVer();
	//m_future_version_acc.lBusinessListParam = theACC_BUSINESS.GetFutMasterVer();
	//// ACC参数版本声明
	//m_cur_version_acc.lLocationParam = theACC_LOCATION.GetCurMasterVer();
	//m_future_version_acc.lLocationParam = theACC_LOCATION.GetFutMasterVer();
 //   // ACC产品参数
	//m_cur_version_acc.lProductParam = theACC_PRODUCT.GetCurMasterVer();
	//m_future_version_acc.lProductParam = theACC_PRODUCT.GetFutMasterVer();
 //   // ACC系统参数类
	//m_cur_version_acc.lSystemParam = theACC_SYSTEM.GetCurMasterVer();
	//m_future_version_acc.lSystemParam = theACC_SYSTEM.GetFutMasterVer();
 //   // ACC版本参数类
	//m_cur_version_acc.lVersionInfoParam = theACC_VERSION.GetCurMasterVer();
	//m_future_version_acc.lVersionInfoParam = theACC_VERSION.GetFutMasterVer();
 //   // 日历参数
	//m_cur_version_acc.lCalenderParam = 0;
	//m_future_version_acc.lCalenderParam = 0;
	//// 费率参数
	//m_cur_version_acc.lFareTableParam = 0;
	//m_future_version_acc.lFareTableParam = 0;
 //   // 销售参数
	//m_cur_version_acc.lSellTableParam = 0;
	//m_future_version_acc.lSellTableParam = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取一卡通参数版本
@param      无
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CParamMsg::SetECTParameterVersion()
{
	//// 一卡通卡片属性定义参数（M3）
	//m_version_ect.lCardPropertyParam = theECT_CARD_PROPERTY.GetCurMasterVer();
	//// 一卡通黑名单参数类(充值黑名单)
	//m_version_ect.lChargeBlacklistParam = theECT_BLACKLIST_CHARGE.GetCurMasterVer();
	//// 一卡通充值终端通讯参数
	//m_version_ect.lConnectionParam = theECT_CONNECTION.GetCurMasterVer();
	//// 一卡通计次卡充值业务参数
	//m_version_ect.lMultirideChargeParam = theECT_MULTIRIDE_CHARGE.GetCurMasterVer();
	//// 一卡通定期卡充值业务参数
	//m_version_ect.lPassChargeParam = theECT_PASS_CHARGE.GetCurMasterVer();
	//// 一卡通储值卡充值业务参数
	//m_version_ect.lPurseChargeParam = theECT_PURSE_CHARGE.GetCurMasterVer();
	//// 一卡通退卡业务参数
	//m_version_ect.lRefundParam = theECT_REFUND.GetCurMasterVer();
	//// 一卡通消费可用卡类型参数类
	//m_version_ect.lShopAbleCardParam = theECT_ENABLE_CARD.GetCurMasterVer();
	//// 一卡通黑名单参数类(充值黑名单)
	//m_version_ect.lShopBlacklistParam = theECT_BLACKLIST_SHOP.GetCurMasterVer();
	//// 一卡通消费终端限额参数
	//m_version_ect.lShopLimitParam = theECT_LIMIT.GetCurMasterVer();
	//// 行业间联乘参数
	//m_version_ect.lTransferParam = theECT_TRANSFER.GetCurMasterVer();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取AFC系统参数版本
@param      无
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CParamMsg::SetAfcParameterVersion()
{
	//// 一卡通业务补充参数
	//m_version_afc.lAddParam = theAFC_ECTADD.GetCurMasterVer();
	//// 设备权限参数类
	//m_version_afc.lAccessLevelParam = theAFC_ACCESS_LEVEL.GetCurMasterVer();
	//// BOM运营参数类
	//m_version_afc.lTVMOperationParam = theAFC_TVM_OPERATION.GetCurMasterVer();
	//// 设备公共参数类
	//m_version_afc.lDeviceCommonParam = theAFC_COMMON.GetCurMasterVer();
	//// 操作员参数类
	//m_version_afc.lStaffPwdParam = theAFC_STAFF.GetCurMasterVer();
	//// 设备运行时间参数
	//m_version_afc.lDeviceRunTimeParam = 0;
}

void CParamMsg::Set24HourMode(BYTE bMode)
{
	m_b24HourMode = bMode;
}

BYTE CParamMsg::Get24HourMode()
{
	return m_b24HourMode;
}

void CParamMsg::SetOperationMode(BYTE bMode)
{
	m_bOperationMode = bMode;
}

BYTE CParamMsg::GetOperationMode()
{
	return m_bOperationMode;
}

void CParamMsg::SetDelayMode(BYTE wMode)
{
	m_wDelayMode = wMode;
}

BYTE CParamMsg::GetDelayMode()
{
	return m_wDelayMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      写入parameter message

@param      (i)WORD wMsgCode			            messege code
@param 	    (i)const VARIABLE_DATA& pParamtMsg		writing data

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CParamMsg::SaveMsg(LPBYTE lpData)
{
	theDEBUG_LOG->WriteData(_T("CParamMsg"), _T("SaveMsg"), __LINE__, _T("<-"));
	try {
		LPBYTE lpMsg = lpData;
		char* pData = (char*)lpMsg;

		// 获取消息总长度
		ULONG pDataSize = 0;
		memcpy(&pDataSize, lpMsg+2, 4);

		// 取得1002参数版本号
		lpMsg += 8;
		CString csVersion = _T("");
		int iVersion = 0;
		memcpy(&iVersion, lpMsg, 4);
		csVersion.Format(_T("%d"),iVersion);

		// 删除旧版本参数文件
		CString sOldFileName = thePARAM_HELPER.GetTheParamPathName(AFC_MODEHISTORY_ID/*0x1002*/,PARAM_AFC_CURRENT_DIR);
		DeleteFile(sOldFileName);

		CString sOldBackUpFileName = thePARAM_HELPER.GetTheParamBackUpPathName(AFC_MODEHISTORY_ID/*0x1002*/,PARAM_AFC_CURRENT_DIR);
		DeleteFile(sOldBackUpFileName);

		// 写1002参数文件
		CString stationCode(_T(""));
		stationCode.Format(_T("0000%.4x"),theMAINTENANCE_INFO.GetStationCodeWithLine());
		//CString csFileName = theTVM_INFO.GetDataPath() + PARAM_AFC_CURRENT_DIR + AFC_PARAM_ID_1090/*AFC_PARAM_ID_1002*/  + _T("00000000.") + csVersion +_T(".00");
		//CString csFileNameBackup = theTVM_INFO.GetBackupPath()+ PARAM_AFC_CURRENT_DIR + AFC_PARAM_ID_1090/*AFC_PARAM_ID_1002*/ + _T("00000000.") + csVersion +_T(".00");
		//auto_ptr<CDXFile> FileMgr(new CDXFile(csFileName,csFileNameBackup));
		//FileMgr->WriteBinFile(pData, pDataSize);

		// 取参数下载目录
		TCHAR szRunPath[MAX_PATH];
		ComGetAppPath(szRunPath, MAX_PATH);
		CString csFileName = CString(szRunPath) + PM_UPDATE_DIR + AFC_PARAM_ID_1002  + _T("00000000.") + csVersion +_T(".00");
		CFile cFile;
		CFileException cfException;
		if(!cFile.Open( csFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary,&cfException)){
			TCHAR   strCause[ERROR_MSG_MAX_LEN]; 
			CString strException; 
			cfException.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException = "异常："; 
			strException += strCause; 
			throw CTxnDataException(CTxnDataException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
		}
		try{
			cFile.Write(pData,pDataSize);
			cFile.Flush();
			cFile.Close();
		}
		catch(CSysException &)
		{
			cFile.Close();
			throw;
		}
		catch(...){
			cFile.Close();
			throw CTxnDataException(CTxnDataException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__); 
		}


		theDEBUG_LOG->WriteData(_T("CParamMsg"), _T("SaveMsg"), __LINE__, _T("->"));
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
}