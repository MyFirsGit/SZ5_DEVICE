#include "stdafx.h"
#include "TVMRWHelper.h"
#include "tpuerrorinfo.h"
#include "tvmsetting.h"
#include "Sync.h"
#define PARAM_HEAD_LEN	36		// 参数头长度
/**
@brief 读写器Helper
*/
RWID CTVMRWHelper::CARD_RWID(INVALID_HANDLE_VALUE,FILTER_CARD_RW_LOG,TVM_CARD_RW);
RWID CTVMRWHelper::TOKEN_RWID(INVALID_HANDLE_VALUE,FILTER_TOKEN_RW_LOG,TVM_TOKEN_RW);
RWID CTVMRWHelper::CHARGE_RWID(INVALID_HANDLE_VALUE,FILTER_RECHARGE_RW_LOG,TVM_RECHARGE_RW);

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTVMRWHelper::CTVMRWHelper(RW_TYPE rwType):m_RWID(rwType == TVM_CARD_RW ? CARD_RWID : rwType == TVM_TOKEN_RW ? TOKEN_RWID : CHARGE_RWID )
{
	m_RWType = rwType;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTVMRWHelper::~CTVMRWHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化读写器

@param       (i)bool reset      是否reset   true:执行 TWReset, false:不执行 RWReset

@retval      无

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CTVMRWHelper::InitCheck(bool reset)
{
	try {
		// 连接读写器
		long errCode = Open();

		// 复位读写器（获取版本、获取RW的SAM ID）
		RW_RSP_STATUS status;
		if (reset) {			
			errCode = Reset(status);
		}

		// 获取TPU版本及SAM卡信息
		RW_RSP_TPU_INFO tpuInfo;
		errCode = GetTPUInfo(tpuInfo);
		if(errCode == RW_RSP_OK){
			if (m_RWType == TVM_CARD_RW){
				theAPP_SESSION.SetSAMInfo(TH_ECT_ISAM, tpuInfo.bmacISAMid);
				theAPP_SESSION.SetSAMInfo(TH_ECT_PSAM, tpuInfo.bmacPSAMid);
				theAPP_SESSION.SetSAMInfo(TH_ACC_ISAM, tpuInfo.accISAMid);
				theAPP_SESSION.SetSAMInfo(TH_ACC_PSAM, tpuInfo.accPSAMid);
			}
			/*if (m_RWType == TVM_TOKEN_RW){
			theAPP_SESSION.SetSAMInfo(RW_ECT_ISAM, tpuInfo.bmacISAMid);
			theAPP_SESSION.SetSAMInfo(RW_ECT_PSAM, tpuInfo.bmacPSAMid);
			theAPP_SESSION.SetSAMInfo(RW_ACC_ISAM, tpuInfo.accISAMid);
			theAPP_SESSION.SetSAMInfo(RW_ACC_PSAM, tpuInfo.accPSAMid);
			}
			if (m_RWType == TVM_RECHARGE_RW){
			theAPP_SESSION.SetSAMInfo(RW_ECT_ISAM, tpuInfo.bmacISAMid);
			theAPP_SESSION.SetSAMInfo(RW_ECT_PSAM, tpuInfo.bmacPSAMid);
			theAPP_SESSION.SetSAMInfo(RW_ACC_ISAM, tpuInfo.accISAMid);
			theAPP_SESSION.SetSAMInfo(RW_ACC_PSAM, tpuInfo.accPSAMid);
			}		*/	
		
			DWORD appVer = 0;
			CString strVer;
			strVer.Format(_T("%01X%02X%02X%02X%02X"),tpuInfo.softwareVersionApp[1] & 0x0F,tpuInfo.softwareVersionApp[2],tpuInfo.softwareVersionApp[3],tpuInfo.softwareVersionApp[4],tpuInfo.softwareVersionApp[5]);
			//memcpy(&appVer,tpuInfo.softwareVersionApp,4);
			appVer = _ttoi(strVer);
			theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,ConvertRWTypeToProgType(m_RWType,false),appVer);
		}
	}
	catch (CSysException) {
		//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,ConvertRWTypeToProgType(m_RWType,true),0);
		//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,ConvertRWTypeToProgType(m_RWType,false),0);
		throw;
	}
	catch (...) {
		//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,ConvertRWTypeToProgType(m_RWType,true),0);
		//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,ConvertRWTypeToProgType(m_RWType,false),0);
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       检查部件是否更换并发送部件更换电文

@param       无 

@retval      无

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CTVMRWHelper::CheckDeviceExchange()
{
	try {
		// 获取TPU设备ID
		RW_RSP_TPU_INFO tpuInfo;
		long errCode = GetTPUInfo(tpuInfo);
		if(errCode == RW_RSP_OK){
			//// 检查部件ID是否变更
			//CCTLOGDeviceInfo::DEVICE_INFO deviceInfo;
			//theDeviceInfo.GetDeviceInfo(deviceInfo);
			//if(memcmp(tpuInfo.DeviceCode,deviceInfo.TW_DEVICE_ID,sizeof(deviceInfo.TW_DEVICE_ID)) != 0){
			//	// 发送设备部件更换电文
			//	//theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(DEVICE_INNER_TPU,tpuInfo.DeviceCode,deviceInfo.TW_DEVICE_ID);
			//	theDeviceInfo.SetTWId(tpuInfo.DeviceCode);
			//}

			//// 判断否有SMA卡在位
			//RW_RSP_TPU_INFO rsp_compare;
			//if(memcmp(tpuInfo.accISAMid,rsp_compare.accISAMid,sizeof(rsp_compare.accISAMid)) == 0){
			//	//theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
			//	theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_SAM_ERROR);
			//	throw CRWHardwareException(RW_TYPE::TVM_CARD_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
			//}
			//if(memcmp(tpuInfo.accPSAMid,rsp_compare.accPSAMid,sizeof(rsp_compare.accPSAMid)) == 0){
			//	//theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
			//	theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_SAM_ERROR);
			//	throw CRWHardwareException(RW_TYPE::TVM_CARD_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
			//}
			//if(memcmp(tpuInfo.bmacPSAMid,rsp_compare.bmacPSAMid,sizeof(rsp_compare.bmacPSAMid)) == 0){
			//	//theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
			//	theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_SAM_ERROR);
			//	throw CRWHardwareException(RW_TYPE::TVM_CARD_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
			//}	

			//// 上报SAM卡状态
			////theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_NORMAL);
			//// 上报读写器状态
			//theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_NORMAL);
		}
	}
	catch (CSysException) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化读写器

@param       无 

@retval      无

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CTVMRWHelper::InitData()
{
	RW_CMD_INIT initInput;
	// 一直寻卡
	initInput.ReadTimeout = 0;
	// 设备ID
	//theMAINTENANCE_INFO.GetCurrentDevice().Serialize(initInput.DeviceId);
	CString deviceId;
	deviceId.Format(_T("%.2d%.2d%.2x%.3d"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetStationCode(),theMAINTENANCE_INFO.GetDeviecTypeID(),theMAINTENANCE_INFO.GetMachineCode());
	initInput.DeviceId = _ttoi(deviceId);
	// 设备类型
	//initInput.DeviceType = MAKEWORD(0x00,theMAINTENANCE_INFO.GetCurrentDevice().bAfcDeviceType);
	DWORD DeviceType = MAKEWORD(theMAINTENANCE_INFO.GetCurrentDevice().bAfcDeviceType,0);

	//  车站编码
	//DWORD dwLocationNum = theACC_LOCATION.GetStationLocationNumber(theMAINTENANCE_INFO.GetStationCodeWithLine());
	initInput.StationId = ComMakeLong(HIBYTE(theMAINTENANCE_INFO.GetTPUStationCode()),LOBYTE(theMAINTENANCE_INFO.GetTPUStationCode()),0,0);//dwLocationNum;

	// 运营公司或参与方
	initInput.AgentCode = 99;	//重启一卡通
	// 运营日期
	initInput.OperationDate = ComGetBusiDate();
	 // 当前日历日期
	initInput.CurrentDate = ComGetCurDate();
	// 设备测试模式
	initInput.TestMode = theMAINTENANCE_INFO.GetDeviceTestMode();	//0x00：表示正常模式；0x01：表示测试模式
	// 车站运营模式 参见ModeCode_t				
	initInput.OperationMode = theTVM_STATUS_MGR.GetOperationMode();
	// 操作员的编号,参见OperateID_t
	initInput.OperaterID = 0;//_ttoi(theAPP_SESSION.GetUserInfo().sUserID);永远都为零
	// 天线配置 01 02 03，分别表示双天线，A天线，B天线	
	initInput.AntennaConfig = 0x01;			
	//0x00：不是有障碍换乘车站；0x01：是有障碍换乘车站；
	initInput.ChangeStationSign = 0;

	// "通讯服务器IP地址, 通讯端口,网络通讯超时时间"，字符串格式，以‘\0’结尾，例如："10.8.3.45,50031,5"
	CString strYPTonlinePara = _T("");
	strYPTonlinePara.Format(_T("%s,%d,%d\0"),theMAINTENANCE_INFO.GetYPTHostID(),theMAINTENANCE_INFO.GetYPTPort(),theMAINTENANCE_INFO.GetYPTTimeOutTime());
	USES_CONVERSION;
	LPSTR YPTonlinePara = T2A(strYPTonlinePara);	
	memcpy(initInput.YPTonlinePara,YPTonlinePara,strlen(YPTonlinePara));
	CString strECTonlinePara = _T("");
	strECTonlinePara.Format(_T("%s,%d,%d\0"),theMAINTENANCE_INFO.GetECTHostID(),theMAINTENANCE_INFO.GetECTPort(),theMAINTENANCE_INFO.GetECTTimeOutTime());
	LPSTR ECTonlinePara = T2A(strECTonlinePara);	
	memcpy(initInput.YKTonlinePara,ECTonlinePara,strlen(ECTonlinePara));

	/*
	ReaderPlace：读写器在设备上的位置：0x00：不考虑类型；
	0x01：进闸Reader；
	0x02：出闸Reader；
	0x05：BOM读卡器
	0x07：TVM读卡器
	0x09：TCM读卡器
	*/
	initInput.ReaderPosition =  0x07; 
	CAfcTVMOperationParam::TPU_PARAM tpuPar;
	theAFC_TVM_OPERATION.GetTpuParam(tpuPar);
	initInput.ErrFareCtrMax = tpuPar.maxWriteErrorCnt;
	initInput.WaitTimeForCardWR = tpuPar.unwriteWaitTime;
	initInput.RetryTimesForCardWR = tpuPar.unwriteRetryTimes;

	// 获取TPU状态
	RW_RSP_STATUS Status = 0;
	GetStatus(Status);

	int iResult = RW_RSP_OK;
	bool hasInit = false;
	while(!hasInit && iResult == RW_RSP_OK){
		switch(Status){
			// 空闲状态
			case STATUE_IDLE:
				{
					iResult = Stop(Status);
					break;
				}
				// 初始状态
			case STATUE_INIT:
				{
					iResult = Reset(Status);
					break;
				}	
				// 未初始化状态
			case STATUE_NOTINIT:
				{
					iResult = InitTW(initInput,Status);
					hasInit = true;
					break;
				}			
			case STATUE_FINDED:			// 已寻到卡状态
			case STATUE_POLLCARD:		// 寻卡状态
			case STATUE_TRANSACTION:	// 交易状态			
				{
					iResult = Abort(Status);
					break;
				}
			case STATUE_GETUDDATA:
				{
					iResult = GetAndSaveUDData();
					GetStatus(Status);
					break;
				}
			default:
				{
					throw CRWException(m_RWType,CRWException::FUNC_ERR_RETURN,_T(__FILE__),__LINE__,_tl(TXT_TPU_RETURN_STATUS_ILLEGAL));
				}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       保存UD数据

@param       无 

@retval       int          0:成功  其他:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMRWHelper::GetAndSaveUDData()
{
	int iRet = -1;
	RW_BUSSINESS_RESPONSE udData;
	iRet = GetUDData(udData);
	if (iRet == RW_RSP_OK)
	{
		SaveRWUDData(udData.UDlen,udData.UDData);
	}
	else{
		int iLastErrCode = 0;
		GetErrCode(iLastErrCode);			
	}
	return iRet;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      保存读写器返回的UD交易数据

@param      (i)WORD txnLen	   读写器返回的UD数据长度
@param      (i)LPBYTE txnBuf   读写器返回的UD数据内容

@retval     int 0:成功 1:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMRWHelper::SaveRWUDData(WORD txnLen,LPBYTE txnBuf)
{
	if(txnLen <= 0 || txnBuf == NULL){
		throw CTxnDataException(CTxnDataException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);
	}
	// 获取操作员ID
	int staffId = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
	LPBYTE lpBuffer = txnBuf;
	// UD数量	
	BYTE udCount = *lpBuffer;
	lpBuffer ++;
	for(int i = 0;i<udCount;i++){
		// 交易数据记录类别
		BYTE transType = *lpBuffer;
		lpBuffer ++;
		// 交易数据记录长度
		WORD transLen = mstos(ComMakeWORD(*lpBuffer,*(lpBuffer + 1)));
		lpBuffer += 2;
		VARIABLE_DATA singleTXNData;
		singleTXNData.nLen =transLen + LEN_AFC_SYS_HEADER; 
		singleTXNData.lpData = new BYTE[singleTXNData.nLen];

		LPBYTE lpData = singleTXNData.lpData;
		memset(lpData,0x00,singleTXNData.nLen);
		// 操作员ID
		int2BCD(staffId, (char*)lpData, 3);
		lpData += 3;
		// 发生日期
		Date2BCD(ComGetBusiDate(),lpData);
		lpData += 4;
		// 消息长度
		ComRevWORD(mstos(singleTXNData.nLen),lpData);		
		lpData +=2;
		// 预留
		*lpData = 0xFF;
		lpData ++;
		// 消息正文
		memcpy(lpData,lpBuffer,transLen);
		lpBuffer += transLen;
		switch(transType){
			case 2: // ACC交易
				theACC_TXN_MGR.SaveTxnData(singleTXNData.nLen,singleTXNData.lpData);
				break;
			case 1: // ECT交易
				theECT_TXN_MGR.SaveTxnData(singleTXNData.nLen,singleTXNData.lpData);
				break;
			default:
				break;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      保存读写器返回的AR数据

@param      (i)WORD txnLen	   读写器返回的AR数据长度
@param      (i)DAT::ARDATA txnBuf   读写器返回的AR数据内容

@retval     int 0:成功 1:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTVMRWHelper::SaveRWARData(WORD &ARDataLen,DAT::ARDATA &ARData,BYTE ticketType)
{
	if(ARDataLen>0)
	{
		ACCAR ar;
		unsigned char* lpData = ARData.ARdata;
		for (int i=0;i<ARData.ARnum;i++)
		{
			WORD arKey =  MAKEWORD(*lpData,*(lpData + 1));
			lpData += 2;
			DWORD arValue = MAKELONG(MAKEWORD(*lpData, *(lpData + 1)),MAKEWORD(*(lpData + 2), *(lpData + 3)));
			lpData += 4;

			if ((arKey<15 && arKey>6)||arKey ==5){
				theDeviceAR.AddProcessStatByType(ticketType,arKey,arValue);
			}
			else{				
				ar[arKey] = (int)arValue;
			}
		}
		theACCAUDIT_MGR.AddAR(ar);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       修复读写器(初始化自检，维护中使用)

@param       无 

@retval      bool   true:成功; false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMRWHelper::RepareIni()
{
	try {
		// 关闭读写器
		Close();
		// 重新自检
		InitCheck();

		// 同步当前站点到全网最高票价
		RW_CMD_TICKET_PRICE query;
		RW_CMD_TICKET_PRICE_RES result;

		query.CardType = CARD_TYPE_SY_EPT_UL;
		query.ProductCategory = PURSE;
		query.ProductType = SJT;
		query.ProductSubType = 0;
		query.PriceType = 0x01; 
		query.Time = ComGetCurTime();

		WORD currentStation = theMAINTENANCE_INFO.GetStationCodeWithLine();
		memcpy(query.BegainStation, &currentStation, 4);
		WORD uncurrentStation = 0xFFFF;
		memcpy(query.EndStation,&uncurrentStation, 4);

		long resultCode = CARDRW_HELPER->GetTicketPrice(query, result);
		if(resultCode == RW_RSP_OK){
			theFunction_INFO.SetTicketMaxAmount(result.TicketPrice);
		}
	}
	catch (CSysException) {
		throw;
	}
	catch (...) {
		throw CRWException(m_RWType,CRWException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       修复读写器(普通业务中用)

@param       无 

@retval      bool   true:成功; false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMRWHelper::Repare()
{
	try {
		// 基本修复
		RepareIni();
		// 初始化数据
		InitData();
	}
	catch (CSysException) {
		throw;
	}
	catch (...) {
		throw CRWException(m_RWType,CRWException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发出读卡命令

@param     (i)bool IsPaidArea  付费区标志 true:付费区;false：非付费区

@param	   (i)bool IsEpPayment EP支付标志 true:EP支付读卡；false：非EP支付读卡

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMRWHelper::ReadCard(SERVICE_ID serviceId,bool IsPaidArea,bool IsEpPayment)
{
	// 读卡前中止读写器
	Abort(true);

	// 读卡
	RW_READ_CARD_REQUEST readCard;
	readCard.IsCheck = true;			//合法性检查标志	true:检查；false:不检查
	readCard.IsPaidArea = IsPaidArea;	//付费区标志		true:付费区;false：非付费区
	readCard.IsReadRecords = false;		//交易记录读取标志	true:读取;false:不读取
	readCard.PollCardMethod = ANTENNA_MARK_B;
	switch(serviceId){
			// 读卡测试
		case RW_TEST_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			readCard.PollCardMethod = ANTENNA_MARK_ALL;
			break;
			// 售票
		case ISSUE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_ISSUE;	
			readCard.PollCardMethod = ANTENNA_MARK_A;
			break;
		case SVT_ISSUE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_SVT_ISSUE;
			break;
			// 补票
		case ADJUST_SVC:
			readCard.FunctionCode = FUNCTION_CODE_ADJUST;	
			break;
			// 充值
		case CHARGE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_CHARGE;	
			break;
			// 分析
		case ANALYZE_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			readCard.IsReadRecords = true;
			break;
		case MONITOR_CARD_RW__SVC:
			readCard.FunctionCode = FUNCTION_CODE_MAINTENANCE_DOOR;
			readCard.PollCardMethod = ANTENNA_MARK_C;
			break;
		default:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			break;

	}

	CService* pCallBackService=theSERVICE_MGR.GetService(serviceId);
	if(NULL!=pCallBackService)
	{
		ReadCard(readCard,pCallBackService);
	}

	// 等待读卡命令执行
	Sleep(100);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发出读卡命令

@param     (i)bool IsPaidArea  付费区标志 true:付费区;false：非付费区
@param     (o)RW_READ_CARD_RESPONSE  读卡数据

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::TryReadCard(SERVICE_ID serviceId,RW_READ_CARD_RESPONSE& rspReadCard,bool IsPaidArea)
{
	// 读卡前中止读写器
	Abort(true);

	// 读卡
	RW_READ_CARD_REQUEST readCard;
	readCard.IsCheck = true;			//合法性检查标志	true:检查；false:不检查  注：正常售票需要改为：true，暂时测试用。
	readCard.IsPaidArea = IsPaidArea;	//付费区标志		true:付费区;false：非付费区
	readCard.IsReadRecords = false;		//交易记录读取标志	true:读取;false:不读取
	readCard.PollCardMethod = ANTENNA_MARK_B;
	switch(serviceId){
		// 读卡测试
		case RW_TEST_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			readCard.PollCardMethod = ANTENNA_MARK_ALL;
			break;
		// 售票
		case ISSUE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_ISSUE;	
			readCard.PollCardMethod = ANTENNA_MARK_A;
			break;
		case SVT_ISSUE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_SVT_ISSUE;
			break;
			// 补票
		case ADJUST_SVC:
			readCard.FunctionCode = FUNCTION_CODE_ADJUST;
			readCard.PollCardMethod = ANTENNA_MARK_A;
			break;
			// 充值
		case CHARGE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_CHARGE;	
			break;
			// 分析
		case ANALYZE_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			readCard.IsReadRecords = true;
			break;
		case MONITOR_CARD_RW__SVC:
			readCard.FunctionCode = FUNCTION_CODE_MAINTENANCE_DOOR;
			readCard.PollCardMethod = ANTENNA_MARK_C;
			break;
		default:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			break;
	}

	return TryReadCard(readCard,&rspReadCard);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      中止读写器命令

@param      (i)bool checkStatus      true:先检查读写器状态; false:不检查读写器状态

@retval     long   错误代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Abort(bool checkStatus)
{
	bool mustStop = true;
	// 读取读写器状态
	RW_RSP_STATUS status;
	if (checkStatus) {		
		GetStatus(status);
		// 空闲模式时，不须中止读写器
		if (status == STATUE_IDLE) {
			mustStop = false;
		}
	}

	long errCode = RW_RSP_OK;
	if (mustStop) {
		errCode = Abort(status);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      停止读写器读卡命令

@param      none

@retval     long   错误代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::StopReadCard()
{
	CRWStopReadCard pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      与读卡器同步参数

@param      无

@retval     bool  true:成功 false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMRWHelper::SynchronismParam()
{
	CParamHelper::VEC_PARAM_ID vecParamId;

	VERSION_ACC versionAcc;
	VERSION_ECT versionEct;
	VERSION_AFC versionAfc;
	DWORD		versionModHistory;
	vecParamId.clear();

	if(GetParameterInfo(versionAcc,versionEct,versionAfc) != 0){
		return false;
	}

	if(versionAcc.lBusinessParam != theACC_BUSINESS.GetCurMasterVer()){
		vecParamId.push_back(ACC_BUSINESS_ID);
	}
	if(versionAcc.lServerFeeParam != theACC_SERVICEFEE.GetCurMasterVer()){
		vecParamId.push_back(ACC_SERVICEFEE_ID);
	}
	if(versionAcc.lOverTimeAdjuestParam != theACC_OVERTIMEADJUEST.GetCurMasterVer()){
		vecParamId.push_back(ACC_OVERTIME_UPDATE_ID);
	}
	if(versionAcc.lDeviceControlParam != theACC_DEVICECONTROL.GetCurMasterVer()){
		vecParamId.push_back(ACC_DEVICE_ID);
	}
	if(versionAcc.lStationParam != theACC_LOCATION.GetCurMasterVer()){
		vecParamId.push_back(ACC_STATION_ID);
	}
	if(versionAcc.lPayStationParam != theACC_TOLLSTATION.GetCurMasterVer()){
		vecParamId.push_back(ACC_CHARGE_STATION_ID);
	}
	if(versionAcc.lCardAttributeParam != theACC_PRODUCT.GetCurMasterVer()){
		vecParamId.push_back(ACC_CARDATTRIBUTE_ID);
	}
	if(versionAcc.lBasePriceParam != theACC_BASEPRICE.GetCurMasterVer()){
		vecParamId.push_back(ACC_BASE_PRICE_TABLE_ID);
	}
	if(versionAcc.lPrieLevelParam != theACC_PRICELEVEL.GetCurMasterVer()){
		vecParamId.push_back(ACC_PRICE_LEVEL_TABLE_ID);
	}
	if(versionAcc.lPriceAdjuestParam != theACC_PRICEADJUEST.GetCurMasterVer()){
		vecParamId.push_back(ACC_PRICE_ADJUEST_TABLE_ID);
	}
	if(versionAcc.lTimeFloatParam != theACC_TIMEFLOAT.GetCurMasterVer()){
		vecParamId.push_back(ACC_TIME_FLOATING_TABLE_ID);
	}
	if(versionAcc.lTimeTypeParam != theACC_TIMETYPE.GetCurMasterVer()){
		vecParamId.push_back(ACC_TIME_TYPE_TABLE_ID);
	}
	if(versionAcc.lTimeDetailParam != theACC_TIMEDETAIL.GetCurMasterVer()){
		vecParamId.push_back(ACC_TTIME_TYPE_DETAIL_ID);
	}
	if(versionAcc.lDiscountParam != theACC_DISCOUNT.GetCurMasterVer()){
		vecParamId.push_back(ACC_DISSCOUNT_TABLE_ID);
	}
	//if(versionAcc.lNotServStationParam != theACC_NOTSERVSTATION.GetCurMasterVer()){
	//	vecParamId.push_back(ACC_NOT_SERVICE_STATION_ID);
	//}
	if(versionAcc.lWriteListParam != theACC_WRITELIST.GetCurMasterVer()){
		vecParamId.push_back(ACC_WRITE_LIST_ID);
	}
	if(versionAcc.lOtherCityCardParam != theACC_OTHERCITYCARD.GetCurMasterVer()){
		vecParamId.push_back(ACC_OTHER_CITY_CARD_ID);
	}
	if(versionAcc.lSingleBlackListParam != theACC_SINGLEBLACKLIST.GetCurMasterVer()){
		vecParamId.push_back(ACC_SINGLE_BLACK_LIST_ID);
	}
	if(versionAcc.lSectionBlackListParam != theACC_SECTIONBLACKLIST.GetCurMasterVer()){
		vecParamId.push_back(ACC_SECTION_BLACK_LIST_ID);
	}
	if(versionAcc.lSectionParam != theACC_SECTION.GetCurMasterVer()){
		vecParamId.push_back(ACC_SECTION_ID);
	}
	if(versionAcc.lissueCompanyParam != theACC_ISSUECOMPANY.GetCurMasterVer()){
		vecParamId.push_back(ACC_ISSUE_COMPANY_ID);
	}


	
	return UpdateParamter(vecParamId);
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      更新读写器指定参数

@param      (i)vecParamId 参数ID数组

@retval     bool  true:成功 false:失败    

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMRWHelper::UpdateParamter(CParamHelper::VEC_PARAM_ID vecParamId)
{
	bool ret = true;
	CString paramPathName = _T("");
	CString sPrefix = PARAM_PREFIX;	
	CString ParamPath = _T("");

	// 遍历参数ID
	vector<WORD>::iterator iteId = vecParamId.begin();
	while (iteId != vecParamId.end()) {
		// 取得参数文件路径
		if(*iteId ==  AFC_MODEHISTORY_ID || *iteId == AFC_DEVICECOMMONPARAM_ID || *iteId == AFC_STAFFPWDPARAM_ID
			||*iteId == AFC_ACCESSLEVELPARAM_ID /*||*iteId == AFC_TVMOPERATIONPARAM_ID*/ ||*iteId == AFC_DEVICERUNTIMEPARAM_ID){
				ParamPath = PARAM_AFC_CURRENT_DIR;
		}
		else if(
			*iteId == ACC_BUSINESS_ID				
			|| *iteId == ACC_SERVICEFEE_ID			
			|| *iteId == ACC_OVERTIME_UPDATE_ID		
			|| *iteId == ACC_DEVICE_ID				
			|| *iteId == ACC_STATION_ID				
			|| *iteId == ACC_CHARGE_STATION_ID		
			|| *iteId == ACC_SECTION_ID				
			|| *iteId == ACC_CARDATTRIBUTE_ID		
			|| *iteId == ACC_ISSUE_COMPANY_ID		
			|| *iteId == ACC_BASE_PRICE_TABLE_ID		
			|| *iteId == ACC_PRICE_LEVEL_TABLE_ID	
			|| *iteId == ACC_PRICE_ADJUEST_TABLE_ID	
			|| *iteId == ACC_TIME_FLOATING_TABLE_ID	
			|| *iteId == ACC_TIME_TYPE_TABLE_ID		
			|| *iteId == ACC_TTIME_TYPE_DETAIL_ID	
			|| *iteId == ACC_DISSCOUNT_TABLE_ID		
			|| *iteId == ACC_NOT_SERVICE_STATION_ID	
			|| *iteId == ACC_SINGLE_BLACK_LIST_ID	
			|| *iteId == ACC_SECTION_BLACK_LIST_ID	
			|| *iteId == ACC_WRITE_LIST_ID			
			|| *iteId == ACC_OTHER_CITY_CARD_ID		
		){
			ParamPath = PARAM_ACC_CURRENT_DIR;
		}
		else{
			CString  sParaID = _T("");
			sParaID.Format(_T("%.4x"),*iteId);  
			throw CRWException(m_RWType,CRWException::PARAM_NOT_EXIST,_T(__FILE__),__LINE__,_tl(TXT_RW_RETURN_PARA_ID_ERROR )+ sParaID);
		}
		paramPathName = thePARAM_HELPER.GetTheParamPathName(*iteId,ParamPath);
		if(thePARAM_HELPER.GetFileNameFromFullPath(paramPathName).GetLength()!=0){
			// 读出bin文件的信息
			VARIABLE_DATA data;
			CXFile file(paramPathName);
			file.ReadBinFile(data);

			// 需要去掉头部文件（36字节）
			if(data.nLen > PARAM_HEAD_LEN){

				// 发出RW的更新命令
				RW_PARAM_DOWNLOAD input;
				if(*iteId != AFC_MODEHISTORY_ID){
					input.fileData = data.lpData + PARAM_HEAD_LEN;
					input.size = data.nLen - PARAM_HEAD_LEN;
				}
				else{			
					input.fileData = data.lpData;
					input.size = data.nLen;
				}
				input.paramId = /*GetTPUParamID*/(*iteId);

				int iRet = DownloadParamter(input);
				if(iRet != RW_RSP_OK){
					ret = false;
					int iLastErrCode = 0;
					GetErrCode(iLastErrCode);
				}
			}
		}
		else{
			CString  sParaID = _T("");
			sParaID.Format(_T("%.4x"),*iteId);  
			throw CRWException(m_RWType,CRWException::PARAM_NOT_EXIST,_T(__FILE__),__LINE__,_tl(TXT_RW_SYNC_LOCAL_PARAMETER_DOES_NOT_EXIST) + sParaID);
		}
		iteId ++;
		//Sleep(1000);
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取读写器指定参数ID

@param      (i)vecParamId 参数ID数组

@retval     bool  true:成功 false:失败    

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CTVMRWHelper::GetTPUParamID(WORD paraID)
{
	//switch(paraID){
	//case ACC_BLACKLIST_ID:// ACC卡黑名单参数
	//	return BLACK_LIST_PARA;
	//case ACC_PRICE_ID:// ACC计价方案参数
	//	return TICKET_PRICE_PARA;
	//case ACC_STATION_ID:// ACC运营点参数
	//	return STATION_NUM_PARA;
	//case ACC_OPERATION_CONTROL_ID:// ACC运营控制参数
	//	return SERV_CONTROL_PARA;
	//case ACC_MODE_RESUME_ID:// ACC模式履歷
	//	return MODE_HISTORY_PARA;
	//case RE_APP_PARA:// 应用程序
	//	return RE_APP_PARA;
	//}
	return 0;
}

////////////////////////////////////////////原子命令//////////////////////
//////////////////////////////////////////////////////////////////////////
/**
@brief      打开读写器命令

@param      none

@retval     long   错误代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Open()
{
	RW_CMD_OPEN cmd_open;
	cmd_open.isUsb = false;
	if (m_RWType == TVM_CARD_RW)
	{
		cmd_open.port = theTVM_SETTING.GetRWComPort();//theTVM_INFO.GetRWCommPort(m_RWType);// 只用CARD类型的读卡器，郑州只有一个TPU
		cmd_open.baud = theTVM_SETTING.GetRWBaudRate();//theTVM_INFO.GetRWBaudRate(m_RWType);
	}
	else if (m_RWType == TVM_RECHARGE_RW)
	{
		cmd_open.port = theTVM_SETTING.GetChargeRWComPort();//theTVM_INFO.GetRWCommPort(m_RWType);// 只用CARD类型的读卡器，郑州只有一个TPU
		cmd_open.baud = theTVM_SETTING.GetChargeRWBaudRate();//theTVM_INFO.GetRWBaudRate(m_RWType);
	}
	CRWOpen pRwCmd(cmd_open,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      关闭读写器命令

@param      none

@retval     long   错误代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Close()
{
	CRWClose pRwCmd(m_RWID);
	long errCode =  pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      复位读写器命令

@param      (o)RW_RSP_STATUS& status 读写器状态

@retval     long   错误代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Reset(RW_RSP_STATUS& status)
{
	CRWReset pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&status);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取读写器状态命令

@param      (o)RW_RSP_STATUS& status         读写器状态

@retval     long   错误代码   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetStatus(RW_RSP_STATUS& status)
{
	CRWGetStatus pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	pRwCmd.GetRWResponse(&status);
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      中止读写器命令

@param      (o)RW_RSP_STATUS& status         读写器状态

@retval     long   错误代码   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Abort(RW_RSP_STATUS& status)
{
	CRWAbort pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&status);
	}
	return errCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      停止读写器命令

@param      (o)RW_RSP_STATUS& status         读写器状态

@retval     long   错误代码   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Stop(RW_RSP_STATUS& status)
{
	CRWStop pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&status);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置读写器时钟

@param      (i)_DATE_TIME    输入时钟
@param      (o)_DATE_TIME    设置后的TPU时钟

@retval     long   错误代码   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::SetTime(_DATE_TIME currentDateTime,_DATE_TIME& tpuDateTime)
{
	CRWSetTime pRwCmd(currentDateTime,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&tpuDateTime);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取读写器时钟

@param      (o)_DATE_TIME    TPU时钟

@retval     long   错误代码   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetTime(_DATE_TIME& tpuDateTime)
{
	CRWGetTime pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&tpuDateTime);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      计算交易认证码(MAC)

@param      (i)BYTE*   需要计算MAC的Hash数据 20字节
@param      (o)BYTE*   返回的MAC数据		 4字节	

@retval     long   错误代码   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetMac(BYTE* hashData,BYTE* mac)
{
	WORD keyVersion = 1;
	CRWGetMac pRwCmd(keyVersion,hashData,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(mac);
	}
	return errCode;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取TPU错误代码

@param      (o)int &iErrorCode    TPU详细错误代码

@retval     long    成功:0; 失败:错误应答码 

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetErrCode(int &iErrorCode)
{
	CRWGetErrCode pRwCmd(iErrorCode,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&iErrorCode);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取TPU信息

@param      (o)DAT::TPUINFO&    TPU信息

@retval     long    成功:0; 失败:错误应答码     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetTPUInfo(RW_RSP_TPU_INFO& tpuInfo)
{
	CRWGetTpuInfo pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&tpuInfo);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      程序下载命令

@param      (i)RW_CMD_FIRMWARE input    输入参数结构体

@retval     long   错误代码   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::TWFirmwareDownload(RW_CMD_FIRMWARE input)
{
	CRWFirmwareDownload pRwCmd(&input,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if(RW_RSP_OK != errCode) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化读写器命令

@param      (o)RW_RSP_STATUS& status         读写器状态

@retval     long   错误代码   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::InitTW(RW_CMD_INIT initInfo,RW_RSP_STATUS& status)
{
	CRWInit pRwCmd(initInfo,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&status);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取票价

@param      (i)RW_CMD_TICKET_PRICE ticketInfo   车票信息
@param      (o)DWORD& price						票价

@retval     long    成功:0; 失败:错误应答码     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetTicketPrice(RW_CMD_TICKET_PRICE ticketInfo,RW_CMD_TICKET_PRICE_RES& res)
{
	CRWGetTicketPrice pRwCmd(ticketInfo,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	//if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&res);
	//}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取税费接口

@param      RW_CMD_TICKET_TAX		车票及票价信息
@param      RW_CMD_TICKET_TAX_RES	反馈：税费

@retval     long   成功：0；失败：错误应答码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetTicketPriceTax(RW_CMD_TICKET_TAX ticketInfo,RW_CMD_TICKET_TAX_RES& taxRsp){
	CRWGetTicketTax getTax(ticketInfo,m_RWID);
	long errCode = getTax.ExecuteCommand();
	if(IsExceptionCode(errCode)){
		throw CRWHardwareException(m_RWType,errCode,_T(__FILE__),__LINE__);
	}
	getTax.GetRWResponse(&taxRsp);
	
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取参数信息

@param      (o)VERSION_ACC& accVersion   ACC参数版本信息
@param      (o)VERSION_ECT& ectVersion	 ECT参数版本信息
@param      (o)VERSION_AFC& afcVersion	 AFC参数版本信息

@retval     long    成功:0; 失败:错误应答码     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetParameterInfo(VERSION_ACC& accVersion,VERSION_ECT& ectVersion,VERSION_AFC& afcVersion)
{
	CRWGetParameterInfo pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		RW_RSP_PARAM_INFO paramData;
		pRwCmd.GetRWResponse(&paramData);
		int paramCnt = paramData.ParaCount;
		WORD paraID = 0;
		DWORD paraVer = 0;
		if(paramCnt>0){
			for(int i = 0;i<paramCnt;i++){
				paraID = ComMakeWORD(paramData.ParamData[i].ParaID[0],paramData.ParamData[i].ParaID[1]); 
				CString pareVersion(paramData.ParamData[i].ParaVer);
				paraVer= _ttoi(pareVersion.Mid(2,10));//(DWORD)ComMakeLong(paramData.ParamData[i].ParaVer[3],paramData.ParamData[i].ParaVer[2],paramData.ParamData[i].ParaVer[1],paramData.ParamData[i].ParaVer[0]);
				switch(paraID){
				case ACC_BUSINESS_ID:			
					accVersion.lBusinessParam = paraVer;
					break;
				case ACC_SERVICEFEE_ID:				
					accVersion.lServerFeeParam = paraVer;
					break;
				case ACC_OVERTIME_UPDATE_ID:			
					accVersion.lOverTimeAdjuestParam	= paraVer;
					break;
				case ACC_DEVICE_ID:		
					accVersion.lDeviceControlParam = paraVer;
					break;
				case ACC_STATION_ID:	
					accVersion.lStationParam = paraVer;
					break;
				case ACC_CHARGE_STATION_ID:		
					accVersion.lPayStationParam	= paraVer;
					break;
				case ACC_CARDATTRIBUTE_ID:		
					accVersion.lCardAttributeParam	= paraVer;
					break;
				case ACC_BASE_PRICE_TABLE_ID:		
					accVersion.lBasePriceParam	= paraVer;
					break;
				case ACC_PRICE_LEVEL_TABLE_ID:		
					accVersion.lPrieLevelParam	= paraVer;
					break;
				case ACC_PRICE_ADJUEST_TABLE_ID:		
					accVersion.lPriceAdjuestParam	= paraVer;
					break;
				case ACC_TIME_FLOATING_TABLE_ID:		
					accVersion.lTimeFloatParam	= paraVer;
					break;
				case ACC_TIME_TYPE_TABLE_ID:		
					accVersion.lTimeTypeParam	= paraVer;
					break;
				case ACC_TTIME_TYPE_DETAIL_ID:		
					accVersion.lTimeDetailParam	= paraVer;
					break;
				case ACC_DISSCOUNT_TABLE_ID:		
					accVersion.lDiscountParam	= paraVer;
					break;
				case ACC_NOT_SERVICE_STATION_ID:		
					accVersion.lNotServStationParam	= paraVer;
					break;
				case ACC_WRITE_LIST_ID:		
					accVersion.lWriteListParam	= paraVer;
					break;
				case ACC_OTHER_CITY_CARD_ID:		
					accVersion.lOtherCityCardParam	= paraVer;
					break;
				case ACC_SINGLE_BLACK_LIST_ID:		
					accVersion.lSingleBlackListParam	= paraVer;
					break;
				case ACC_SECTION_BLACK_LIST_ID:		
					accVersion.lSectionBlackListParam	= paraVer;
					break;
				case ACC_SECTION_ID:		
					accVersion.lSectionParam	= paraVer;
					break;
				case ACC_ISSUE_COMPANY_ID:		
					accVersion.lissueCompanyParam	= paraVer;
					break;
				default:
					try{
						CString  sParaID = _T("");
						sParaID.Format(_T("%.4x"),paraID);  
						throw CRWException(m_RWType,errCode, _T(__FILE__), __LINE__,_tl(TXT_RW_RETURN_PARA_ID_ERROR) + sParaID);
					}
					catch(CSysException& e){
						theEXCEPTION_MGR.WriteExceptionLog(e);// 此类错误只写日志
					}
												
				}
			}
		}
	}

	//// ACC 其他参数（非TPU使用）
	//accVersion.lProductParam	   = theACC_PRODUCT.GetCurMasterVer();
	//accVersion.lLocationParam	   = theACC_LOCATION.GetCurMasterVer();
	//accVersion.lFeeParam		   = theACC_FEE.GetCurMasterVer();

	return errCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      下载参数命令

@param      (i)RW_PARAM_DOWNLOAD paramInfo 参数信息

@retval     long    成功:0; 失败:错误应答码     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::DownloadParamter(RW_PARAM_DOWNLOAD paramInfo)
{
	CRWParamFirmware pRwCmd(paramInfo,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置运营模式

@param      (i)RW_CMD_SET_MODE paramInfo 控制代码
@param      (o)RW_RSP_STATUS status      读写器状态

@retval     long    成功:0; 失败:错误应答码     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::SetOperationMode(RW_CMD_SET_MODE cmdSetMode,RW_RSP_STATUS& status)
{
	CRWSetOperationMode pRwCmd(cmdSetMode,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&status);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置TPU 24小时运营或延长运营时间

@param      (i)RW_CMD_SET_TIME cmdSetMode 控制代码
@param      (o)RW_RSP_STATUS status      读写器状态

@retval     long    成功:0; 失败:错误应答码     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::SetBusinessTime(RW_CMD_SET_TIME cmdSetMode,RW_RSP_STATUS& status)
{
	CRWSetBusinessTime pRwCmd(cmdSetMode,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&status);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取UD数据

@param      (o)RW_BUSSINESS_RESPONSE& response     业务返回通用结构

@retval     long    成功:0; 失败:错误应答码     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetUDData(RW_BUSSINESS_RESPONSE& commonResponse)
{
	CRWGetUDData pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&commonResponse);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读卡命令

@param      (i)RW_READ_CARD_REQUEST cmdReadCard	   读卡命令输入参数
@param      (o)RW_READ_CARD_RESPONSE& rspReadCard  读卡返回数据结构

@retval     long    成功:0; 失败:错误应答码     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTVMRWHelper::ReadCard(RW_READ_CARD_REQUEST cmdReadCard,CService* pService)
{
	if(NULL!=pService)
	{
		CRWReadCard* pRwCmd = new CRWReadCard(cmdReadCard,m_RWID);
		pRwCmd->SetAutoDeleted(TRUE);
		pRwCmd->SetService(pService);
		pRwCmd->SetNeedCallbackService(true);
		pRwCmd->SetCommandID(RW_COMMAND_ID_READ_CARD);
		EnableRead();
		g_pTVMRWControl->DoCommand(pRwCmd,SP_EXECMODE_QUEUE);
	}else
	{
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读卡命令

@param      (i)RW_READ_CARD_REQUEST cmdReadCard	   读卡命令输入参数
@param      (o)RW_READ_CARD_RESPONSE& rspReadCard  读卡返回数据结构

@retval     long    成功:0; 失败:错误应答码     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::TryReadCard(RW_READ_CARD_REQUEST cmdReadCard,RW_READ_CARD_RESPONSE* rspReadCard)
{
	CRWTryReadCard rwCmd(cmdReadCard,m_RWID);
	long result = g_pTVMRWControl->DoCommand(&rwCmd);
	rwCmd.GetRWResponse(rspReadCard);
	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      售卡命令

@param      (i)RW_ISSUE_REQUEST issueInput        售卡输入参数结构
@param      (o)RW_BUSSINESS_RESPONSE& response     业务返回通用结构

@retval     long   错误代码   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Issue(RW_ISSUE_REQUEST issueInput, RW_BUSSINESS_RESPONSE& response)  
{
	CRWIssue pRwCmd(issueInput,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&response);
	}
	else{
		int errorCode=0;
		GetErrCode(errorCode);
	}
	return errCode;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      补票命令
//
//@param      (i)RW_CMD_ADJUST adjustInput        补票输入参数结构
//@param      (o)RW_BUSSINESS_RESPONSE& response     业务返回通用结构
//
//@retval     long   错误代码   
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::Adjust(RW_CMD_ADJUST adjustInput, RW_BUSSINESS_RESPONSE& response)  
//{
//	CRWAdjust pRwCmd(adjustInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	
//		pRwCmd.GetRWResponse(&response);
//	
//	return errCode;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      充值命令

@param      (i)RW_CMD_CHARGE chargeInput        充值输入参数结构
@param      (o)RW_BUSSINESS_RESPONSE& response     业务返回通用结构

@retval     long   错误代码   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Charge(RW_CMD_CHARGE chargeInput, RW_BUSSINESS_RESPONSE& response)  
{
	CRWCharge pRwCmd(chargeInput,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	pRwCmd.GetRWResponse(&response);
	return errCode;
}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      退款命令
//
//@param      (i)RW_CMD_REFUND refundInput        退款输入参数结构
//@param      (o)RW_BUSSINESS_RESPONSE& response     业务返回通用结构
//
//@retval     long   错误代码   
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::Refund(RW_CMD_REFUND refundInput, RW_BUSSINESS_RESPONSE& response)  
//{
//	CRWRefund pRwCmd(refundInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//	return errCode;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      锁卡命令

@param      (i)RW_CMD_BLOCK blockInput             锁卡输入参数结构
@param      (o)RW_BUSSINESS_RESPONSE& response     业务返回通用结构

@retval     long   错误代码   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Block(RW_CMD_BLOCK blockInput, RW_BUSSINESS_RESPONSE& response)  
{
	CRWBlock pRwCmd(blockInput,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&response);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      EP支付命令

@param      (i)RW_CMD_CONSUME rwCmd			交易数据
@param		(o)RW_BUSSINESS_RESPONSE& rwRsp 交易反馈

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::ConsumeEP(RW_CMD_CONSUME rwCmd,RW_BUSSINESS_RESPONSE& rwRsp){
	CRWConsumeEP consumeCmd(rwCmd,m_RWID);
	long errCode = consumeCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)){
		throw CRWHardwareException(m_RWType,errCode,_T(__FILE__),__LINE__);
	}
	if(RW_RSP_OK == errCode){
		consumeCmd.GetRWResponse(&rwRsp);
	}
	return errCode;
}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      抵消命令
//
//@param      (i)RW_CMD_BLOCK blockInput        抵消输入参数结构
//@param      (o)RW_BUSSINESS_RESPONSE& response     业务返回通用结构
//
//@retval     long   错误代码   
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::Counteract(RW_CMD_COUNTERACT conteractInput, RW_BUSSINESS_RESPONSE& response)  
//{
//	CRWCounteract pRwCmd(conteractInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief     激活命令
//
//@param      (i)RW_CMD_BLOCK blockInput        抵消输入参数结构
//@param      (o)RW_BUSSINESS_RESPONSE& response     业务返回通用结构
//
//@retval     long   错误代码   
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::Refresh(RW_CMD_ACTIVE refreshInput, RW_BUSSINESS_RESPONSE& response)
//{
//	CRWRefresh pRwCmd(refreshInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief     延期命令
//
//@param      (i)RW_CMD_DEFER deferInput        延期输入参数结构
//@param      (o)RW_BUSSINESS_RESPONSE& response     业务返回通用结构
//
//@retval     long   错误代码   
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::Defer(RW_CMD_DEFER deferInput, RW_BUSSINESS_RESPONSE& response)
//{
//	CRWDefer pRwCmd(deferInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      一卡通充值超时处理
//
//@param      (i) 
//
//
//@retval     long   错误代码   
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::ChargeOverTimeProcess(RW_CMD_CHARGEOVERTIME chargeOverTime, RW_BUSSINESS_RESPONSE& response )
//{
//	CRWChargeOverTime pRwCmd(chargeOverTime,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//
//	return errCode;
//}
//
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      一卡通售卡初检处理
//
//@param      (i) 
//
//
//@retval     long   错误代码   
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::IssueInitCheckProcess(RW_CMD_SETCHECK chargeInput,RW_CMD_RESP_SETCHECK &response)
//{
//	CRWSetCheck pRwCmd(chargeInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	if (RW_RSP_OK == errCode) {
//		pRwCmd.GetRWResponse(&response);
//	}
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief     记名卡持卡人信息修改命令
//
//@param      (i)RW_CMD_UPDATE_SIGNCARD_INFO updateSignCardInfo   记名卡参数结构
//@param      (o)RW_BUSSINESS_RESPONSE& response					业务返回通用结构
//
//@retval     long   错误代码   
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::UpdateSignCardInfo(RW_CMD_UPDATE_SIGNCARD_INFO updateSignCardInfo,RW_BUSSINESS_RESPONSE &response)
//{
//	CRWUpdateSignCardInfo pRwCmd(updateSignCardInfo,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//	return errCode;
//}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断读写器是否正常

@param      none

@retval     bool true:异常 false:正常 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMRWHelper::IsTWHasException()
{
	return theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否为需要抛异常的ID

@param      (i)long errCode      输入参数结构体

@retval     bool \n  true:异常的错误代码; false:不抛异常的

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMRWHelper::IsExceptionCode(long errCode)
{
	bool ret = false;

	switch(errCode) {
	//case RW_RSP_STATUSERROR:       // TPU状态非法
	//case RW_RSP_COMERR:            // 通讯接口类错误
	////case RW_RSP_SAMERR:		   // SAM 卡放在读卡反馈错误类中处理
	//	//case RW_RSP_PARAMERR:	   // 配置参数类错误
	//case RW_RSP_UDERR:		       // UD数据类错误
	//case RW_RSP_HARDWAREERR:	   // 硬件类错误
	//case RW_RSP_UNKNOWERR:		   // 未归类错误
	case RECV_TIMEOUT:			   // 通讯超时
	case SEND_ERR:				   //发送错误
	case OPENCOM_ERR:		   //串口打开失败
	case RECV_ERR:				   //接收失败
	case TPU_POWERDOWN:            //给TPU断电重启
		ret = true;
		break;
	default:
		break;
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读写器程序更新

@param      ()bool isMain   是否是基本部程序更新

@retval     int      0:成功  -2: 程序文件不存在   -3:程序版本无效  其他:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMRWHelper::UpdateProgram(bool isMain)
{
	int iRet = 0;
	try
	{
		CString fileName = isMain?MID_STR_BOM_RW_PROG_MAIN:MID_STR_BOM_RW_PROG_APP;
		CString pathName = m_RWType == TVM_RECHARGE_RW? TVM_CHARGE_RW_UPDATE_DIR: m_RWType == TVM_CARD_RW ? TVM_CARD_RW_UPDATE_DIR : m_RWType == TVM_TOKEN_RW ? TVM_TOKEN_RW_UPDATE_DIR:_T("");
		vector<CString> updateFiles;

		CString sAppPath;
		GetAppPath(sAppPath);

		CString updateFileFolder = sAppPath + pathName;

		SearchFile(updateFileFolder + fileName + _T("*.*"),updateFiles);
		if(updateFiles.size()<=0)
		{
			LOG("无读写器更新文件");
			//return -2;
			return 0;
		}

		for(int i=0;i<updateFiles.size();i++)
		{
			CString updateFile = updateFiles[i];
			SetFileAttributes(updateFile,FILE_ATTRIBUTE_NORMAL);
			CString upgradeVersionStr = thePARAM_HELPER.GetProgVerFromProgName(updateFile);	// 获取升级的tpu版本
			DWORD upgradeVersion = _ttoi(upgradeVersionStr);

			// 版本无效
			if(UNAVALIBLE_VERSION == upgradeVersion){
				DeleteFile(updateFile);
				LOG("读写器更新程序版本无效");
				return -3;
			}

			// 读出bin文件的信息
			VARIABLE_DATA data;
			CXFile file(updateFile);
			file.ReadBinFile(data);
			file.Close();

			DeleteFile(updateFile);

			// 发出TW的更新命令
			RW_CMD_FIRMWARE input;
			input.fileData = data.lpData;
			input.size = data.nLen;
			input.isMain = isMain;

			theAPP_SESSION.SetHasProgUpdate(true);
			theTVM_SETTING.SetCardRWUpdateFlag(1);
			if (RW_RSP_OK == TWFirmwareDownload(input)) {
				theTVM_SETTING.SetCardRWUpdateFlag(0);
				LOG("读写器程序更新成功。");
				theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,m_RWType == TVM_RECHARGE_RW? CVersionInfo::CHARGE_RW_APP:  CVersionInfo::CARD_RW_APP/* ? CVersionInfo::CARD_RW_APP : m_RWType == TVM_TOKEN_RW ? CVersionInfo::TOKEN_RW_APP:CVersionInfo::CHARGE_RW_APP*/,upgradeVersion);//更新本地版本
			}
			else{
				//CVersionInfo::PROG_TYPE progType;
				//if(isMain){
				//	progType = CVersionInfo::RW_MAIN;
				//}
				//else{
				//	progType = CVersionInfo::RW_APP;
				//}
				//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,progType,currentVersion);//更新失败，保留TPU当前实际版本。
				iRet = -1;
			}
			// 更新主控后等到120秒方可进行应用部程序更新
			if(isMain){
				Sleep(120*1000);
			}
		}
	}
	catch (CSysException) {
		throw;
	}
	catch (...) {
		throw CInnerException(MAIN_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return iRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取读写器错误信息

@param      (i)int iError   错误代码

@retval     CStringt     读写器错误信息

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CTVMRWHelper::GetTpuErrorInfo(int iError)
{
	return theTPUERROR_INFO.GetTpuErrorInfo(iError);
	/*TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath, _MAX_PATH); 
	CString fileName = CString(szAppPath) + _T("TpuErrorCode.ini");
	return gGetErrorInfofromFile(fileName, iError);*/
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取读写器错误信息

@param      (i)int iError   错误代码

@retval     CStringt     读写器错误信息

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CTVMRWHelper::gGetErrorInfofromFile(LPCTSTR pstrFile, DWORD dwErrorCode)
{

	CString pstrErrorCode(_T(""));
	pstrErrorCode.Format(_T("0x%08X"), dwErrorCode);

	if (pstrErrorCode == _T("") || pstrFile == NULL)
		return _T("");


	CString strInfo(_T(""));
	try
	{
		CString strFlag1(_T("/*")), strFlag2(_T("*/"));
		//$$CStdioFile file(pstrFile, CFile::typeText | CFile::modeRead);
		CXFile file(pstrFile);
		CString strTxt(_T(""));
		bool bReadInfo = file.ReadString(strTxt);
		while(bReadInfo)
		{
			strTxt.MakeUpper();
			strTxt.Trim();
			if ((strTxt[0] == '\\' && strTxt[1] == '\\')
				|| (strTxt[0] == '/' && strTxt[1] == '*'))
			{
				bReadInfo = file.ReadString(strTxt);
				continue;
			}

			CString strErrorCode  = pstrErrorCode;
			strErrorCode.MakeUpper();
			int nPos = strTxt.Find(strErrorCode);
			if (nPos != -1)
			{
				strInfo.Empty();
				int nPos1 = strTxt.Find(strFlag1);
				int nPos2 = strTxt.Find(strFlag2);
				if (nPos1 != -1 && nPos2 != -1) 
				{
					strInfo = strTxt.Mid(nPos1+strFlag1.GetLength(), nPos2-(nPos1+strFlag1.GetLength()));
				}
				break;
			}

			bReadInfo = file.ReadString(strTxt);
		}
		if (!bReadInfo)
		{
			strInfo = _tl(TXT_UNKOWN_ERROR);
			theDEBUG_LOG->WriteData(_T(""), _T("gGetErrorInfromFile"), __LINE__, _tl(TXT_HAVE_NOT_FOUND_ERROR_MSG));
		}
	}
	catch (CFileException e)
	{
#ifdef _DEBUG
		afxDump << _T("File could not be opened ")
			<< e.m_cause << _T("\n");
#endif
		TCHAR strError[256];
		memset(strError, 0, sizeof(strError));
		e.GetErrorMessage(strError, 256);
		theDEBUG_LOG->WriteData(_T(""), _T("gGetErrorInfromFile"), __LINE__, strError);
	}
	strInfo = _tl(LANGUAGE_RED) + strInfo +_T("\r\n")+ _T("(")+pstrErrorCode+_T(")");
	return strInfo;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  通过读写器的类型转换到程序类型    

@param RW_TYPE rwType 读写器类型
@param bool isMain 是否是主控部

@retval   PROG_TYPE 程序类型

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CVersionInfo::PROG_TYPE CTVMRWHelper::ConvertRWTypeToProgType(RW_TYPE rwType, bool isMain)
{
	CVersionInfo::PROG_TYPE programType = CVersionInfo::UNKOWN;	
	switch(rwType)
	{
	case RW_TYPE::TVM_CARD_RW:
		programType =  isMain ? CVersionInfo::CARD_RW_MAIN : CVersionInfo::CARD_RW_APP;
		break;
//	case RW_TYPE::TVM_TOKEN_RW:
//		programType =  isMain ? CVersionInfo::TOKEN_RW_MAIN : CVersionInfo::TOKEN_RW_APP;
//		break;
	case RW_TYPE::TVM_RECHARGE_RW:
		programType = isMain ? CVersionInfo::CHARGE_RW_MAIN : CVersionInfo::CHARGE_RW_APP;
		break;
	default:
		programType =  CVersionInfo::UNKOWN;
	}

	return programType;
}



////////////////////////////////////////////////////////////////////////////
///**
//@brief      替换(旧卡)交易命令
//
//@param      (i)RW_CMD_REPLACE_CARD replaceInput        替换(旧卡)输入参数结构
//@param      (o)RW_BUSSINESS_RESPONSE& response     业务返回通用结构
//
//@retval     long   错误代码   
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::ReplaceOldCard(RW_CMD_REPLACE_CARD& replaceInput,RW_BUSSINESS_RESPONSE& response)
//{
//	CRWReplaceOldCard pRwCmd(replaceInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      替换(新卡)交易命令
//
//@param      (i)RW_CMD_DEFER replaceInput        延期输入参数结构
//@param      (o)RW_BUSSINESS_RESPONSE& response     业务返回通用结构
//
//@retval     long   错误代码   
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::ReplaceNewCard(RW_CMD_REPLACE_CARD&replaceInput,RW_BUSSINESS_RESPONSE&response)
//{
//	CRWReplaceOldCard pRwCmd(replaceInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      个性化命令（个性化和个性化更新（不换卡））
//
//@param      (i)RW_CMD_PERSONALIZE_INFO personalizeInfo	个性化信息结构
//@param      (o)RW_BUSSINESS_RESPONSE& response	业务返回通用结构
//
//@retval     long   错误代码   
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::Personalize(RW_CMD_PERSONALIZE_INFO personalizeInfo,RW_BUSSINESS_RESPONSE& response)
//{
//	CRWPersonalize pRwCmd(personalizeInfo,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      个性化更新锁旧卡命令
//
//@param      (i)RW_CMD_PERSONALIZE_INFO personalizeInfo	个性化信息结构
//@param      (o)RW_BUSSINESS_RESPONSE& response	业务返回通用结构
//
//@retval     long   错误代码   
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::PersonalizeUpdateOldCard(RW_CMD_PERSONALIZE_INFO personalizeInfo,RW_BUSSINESS_RESPONSE& response)
//{
//	CRWPersonalizeUpdateOldCard pRwCmd(personalizeInfo,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      个性化更新写新卡命令
//
//@param      (i)RW_CMD_PERSONALIZE_INFO personalizeInfo	个性化信息结构
//@param      (o)RW_BUSSINESS_RESPONSE& response	业务返回通用结构
//
//@retval     long   错误代码   
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::PersonalizeUpdateNewCard(RW_CMD_PERSONALIZE_INFO personalizeInfo,RW_BUSSINESS_RESPONSE& response)
//{
//	CRWPersonalizeUpdateNewCard pRwCmd(personalizeInfo,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//
//	return errCode;
//}


DWORD WINAPI RechargeServerMoniterFunction(IN LPVOID vThreadParm){
	CTVMRWHelper* pThreadParm = reinterpret_cast<CTVMRWHelper*>(vThreadParm);

	if(NULL != pThreadParm){
		while(1){
			//// 判断是否退出线程
			//if(pThreadParm->m_bExitThread){
			//	break;
			//}

			// 检测充值服务器是否可用
			theAPP_SESSION.SetRechargeServerEnabled(false);
			bool ret1,ret2;
			ret1 = ret2 = false;

			WSADATA wsa;
			SOCKET s;
			struct sockaddr_in server;
			USES_CONVERSION;
			char* pHost = NULL;
			int nPort = 0;
			int ret = 0;
			// 检查YKT服务器
			pHost = T2A(theTVM_SETTING.GetRechargeServeYKT_IP());
			nPort = theTVM_SETTING.GetRechargeServeYKT_Port();    //当前端口
			WSAStartup(MAKEWORD(2, 2), &wsa);    //使用winsock函数之前，必须用WSAStartup函数来装入并初始化动态连接库
			server.sin_family = AF_INET;    //指定地址格式，在winsock中只能使用AF_INET
			server.sin_addr.s_addr = inet_addr(pHost); //指定被扫描的IP地址
			s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			server.sin_port = htons(nPort); //指定被扫描IP地址的端口号
			ret = connect(s, (struct sockaddr *)&server, sizeof(server));
			if(ret==0){
				ret1 = true;
				closesocket(s);
			}
			// 检查YPT服务器
			pHost = T2A(theTVM_SETTING.GetRechargeServeYPT_IP());
			nPort = theTVM_SETTING.GetRechargeServeYPT_Port();    //当前端口
			server.sin_addr.s_addr = inet_addr(pHost); //指定被扫描的IP地址
			s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			server.sin_port = htons(nPort); //指定被扫描IP地址的端口号
			ret = connect(s, (struct sockaddr *)&server, sizeof(server));
			if(ret==0){
				ret2 = true;
				closesocket(s);
			}

			if (ret1&&ret2){
				theAPP_SESSION.SetRechargeServerEnabled(true);
			}

			Sleep(10000);
		}// end of while
	}

	return RSP_CODE_OK;
}


// 充值服务器检测
long CTVMRWHelper::DoRechargeServerMoniter()
{
	// 创建充值服务器检测线程
	DWORD dwIdleThreadID = 0;
	HANDLE hIdleThread = CreateThread(
		NULL,				// Pointer to thread security attributes
		0,					// Initial thread stack size, in bytes 
		RechargeServerMoniterFunction,	// Pointer to thread function
		this,				// The argument for the new thread
		0,					// Creation flags 
		&dwIdleThreadID			// Pointer to returned thread identifier
		);

	if(hIdleThread != NULL && hIdleThread != INVALID_HANDLE_VALUE){
		CloseHandle(hIdleThread);
		hIdleThread = NULL;
		return 1;
	}

	return 0;
}

