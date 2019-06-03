#include "stdafx.h"
#include "TWHelper.h"
#include "tpuerrorinfo.h"
#include "caccdevicecontrolparam.h"
#include "setting.h"

/**
@brief RW Helper
*/
RWID CTWHelper::s_TWID(INVALID_HANDLE_VALUE,FILTER_DESKTOP_RW_LOG,BOM_DESKTOP_RW);

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTWHelper::CTWHelper(CService &service):CServiceHelper(service)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTWHelper::~CTWHelper()
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
void CTWHelper::InitCheck(bool reset)
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
			theAPP_SESSION.SetSAMInfo(TH_ECT_ISAM, tpuInfo.bmacISAMid);
			theAPP_SESSION.SetSAMInfo(TH_ECT_PSAM, tpuInfo.bmacPSAMid);
			theAPP_SESSION.SetSAMInfo(TH_ACC_ISAM, tpuInfo.accISAMid);
			theAPP_SESSION.SetSAMInfo(TH_ACC_PSAM, tpuInfo.accPSAMid);
			theSOFT_VERSION.SetTHRWBootVer((char*)tpuInfo.softwareVersionMain); 
			theSOFT_VERSION.SetTHRWVersion((char*)tpuInfo.softwareVersionApp); 

			DWORD mainVer;
			memcpy(&mainVer,tpuInfo.softwareVersionMain,4);

			DWORD appVer;
			memcpy(&appVer,tpuInfo.softwareVersionApp,4);

			theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TW_MAIN,mainVer);
			theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TW_APP,appVer);
		}
	}
	catch (CSysException &e) {
		theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TW_MAIN,0);
		theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TW_APP,0);
		throw e;
	}
	catch (...) {
		theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TW_MAIN,0);
		theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TW_APP,0);
		throw CInnerException(CTWException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
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
void CTWHelper::CheckDeviceExchange()
{
	try {
		// 获取TPU设备ID
		RW_RSP_TPU_INFO tpuInfo;
		long errCode = GetTPUInfo(tpuInfo);
		if(errCode == RW_RSP_OK){
			// 检查部件ID是否变更
			CCTLOGDeviceInfo::DEVICE_INFO deviceInfo;
			theDeviceInfo.GetDeviceInfo(deviceInfo);
			if(memcmp(tpuInfo.DeviceCode,deviceInfo.TW_DEVICE_ID,sizeof(deviceInfo.TW_DEVICE_ID)) != 0){
				// 发送设备部件更换电文
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(BOM_DEVICE_INNER_TPU,tpuInfo.DeviceCode,deviceInfo.TW_DEVICE_ID);
				theDeviceInfo.SetTWId(tpuInfo.DeviceCode);
			}
		}
	}
	catch (CSysException) {
		throw;
	}
	catch (...) {
		throw CInnerException(CTWException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
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
void CTWHelper::InitData()
{
	RW_CMD_INIT initInput;
	// 一直寻卡
	initInput.ReadTimeout = 0;
	// 设备ID
	CString deviceId;
	deviceId.Format(_T("%.2d%.2d%.2x%.3d"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetStationCode(),theMAINTENANCE_INFO.GetDeviecTypeID(),theMAINTENANCE_INFO.GetMachineCode());
	initInput.DeviceId = _ttoi(deviceId);
	//initInput.DeviceId[5] = 0;
	//initInput.DeviceId[4] = 0;
	//initInput.DeviceId[3] = theMAINTENANCE_INFO.GetLineCode();		// 线路编号
	//initInput.DeviceId[2] = theMAINTENANCE_INFO.GetStationCode();	// 车站编号
	//initInput.DeviceId[1] = DEVICE_BOM;								// 设备类型 BOM 20
	//initInput.DeviceId[0] = theMAINTENANCE_INFO.GetMachineCode();	//设备编号
	// 设备类型
	initInput.DeviceType = theMAINTENANCE_INFO.GetCurrentACCDevice().deviceType;
	//  车站编码
	initInput.StationId = ComMakeLong(HIBYTE(theMAINTENANCE_INFO.GetTPUStationCode()),LOBYTE(theMAINTENANCE_INFO.GetTPUStationCode()),0,0);//dwLocationNum;
	// 运营公司或参与方
	initInput.AgentCode = 2;	//沈阳地铁2号线运营公司	02
	// 运营日期
	initInput.OperationDate = ComGetBusiDate();
	// 当前日历日期
	initInput.CurrentDate = ComGetCurDate();
	// 设备测试模式
	initInput.TestMode = theSETTING.GetDeviceTestMode();	//0x00：表示正常模式；0x01：表示测试模式
	// 车站运营模式 参见ModeCode_t				
	initInput.OperationMode = theBOM_STATUS_MGR.GetRunMode();
	// 操作员的编号,参见OperateID_t
	initInput.OperaterID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);		
	// 天线配置 0x00：单天线；0x01：双天线且天线A优先寻卡；0x02：双天线且天线B优先寻卡	
	initInput.AntennaConfig = 0x02;			

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
					throw CTWException(CTWException::FUNC_ERR_RETURN,_T(__FILE__),__LINE__,_T("TPU返回状态值非法"));
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
int CTWHelper::GetAndSaveUDData()
{
	int iRet = -1;
	RW_BUSSINESS_RESPONSE udData;
	iRet = GetUDData(udData);
	if (iRet == RW_RSP_OK)
	{
		SaveTWUDData(udData.UDlen,udData.UDData);
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
int CTWHelper::SaveTWUDData(WORD txnLen,LPBYTE txnBuf)
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
void CTWHelper::SaveTWARData(WORD &ARDataLen,DAT::ARDATA &ARData,BYTE ticketType)
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
				theBomAR.AddBOMProcessStatByType(ticketType,arKey,arValue);
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
bool CTWHelper::RepareIni()
{
	try {
		// 关闭读写器
		Close();
		// 重新自检
		InitCheck();
	}
	catch (CSysException) {
		throw;
	}
	catch (...) {
		throw CTWException(CTWException::OTHER_ERR,_T(__FILE__),__LINE__);
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
bool CTWHelper::Repare()
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
		throw CTWException(CTWException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发出读卡命令

@param     (i)bool IsPaidArea  付费区标志 true:付费区;false：非付费区

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTWHelper::ReadCard(bool IsPaidArea)
{
	// 读卡前中止读写器
	Abort(true);

	// 读卡
	RW_READ_CARD_REQUEST readCard;
	readCard.IsCheck = true;			//合法性检查标志	true:检查；false:不检查
	readCard.IsPaidArea = IsPaidArea;	//付费区标志		true:付费区;false：非付费区
	readCard.IsReadRecords = false;		//交易记录读取标志	true:读取;false:不读取
	UINT serviceId = m_service.GetServiceID();
	switch(serviceId){
		// 售票 || 预售票
		case ISSUE_SVC:
		case PREISSUE_SVC:	
			readCard.FunctionCode = FUNCTION_CODE_ISSUE;				
			break;
		// 补票
		case ADJUST_SVC:
			readCard.FunctionCode = FUNCTION_CODE_ADJUST;	
			break;
		// 充值
		case CHARGE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_CHARGE;	
			break;
		// 分析 || 读卡测试
		case ANALYZE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			readCard.IsReadRecords = true;
		case OUTRW_TEST_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			readCard.IsReadRecords = true;
			break;
		// 激活
		case REFRESH_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_REFRESH_AUTO;
			readCard.IsReadRecords = true;
			break;
		// 延期
		case DEFER_SVC:
			readCard.FunctionCode = FUNCTION_CODE_DEFER;
			break;
		// 换卡
		case EXCHANGE_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_EXCHANGE_OLD;
			readCard.IsReadRecords = true;
			break;
		// 即时退卡 | 退款申请 | 退款查询
		case REFUND_BASE_SVC:
		case REFUND_APPLY_SVC:
		case REFUND_QUERY_SVC:
			readCard.FunctionCode = FUNCTION_CODE_REFUND;
			break;
		// 抵消
		case COUNTERACT_SVC:
			readCard.FunctionCode = FUNCTION_CODE_CONTERACT;
			break;
		default:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			break;
	}

	ReadCard(readCard);

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
long CTWHelper::TryReadCard(RW_READ_CARD_RESPONSE& rspReadCard,bool IsPaidArea)
{
	// 读卡前中止读写器
	Abort(true);

	// 读卡
	RW_READ_CARD_REQUEST readCard;
	readCard.IsCheck = true;			//合法性检查标志	true:检查；false:不检查
	readCard.IsPaidArea = IsPaidArea;	//付费区标志		true:付费区;false：非付费区
	readCard.IsReadRecords = false;		//交易记录读取标志	true:读取;false:不读取
	UINT serviceId = m_service.GetServiceID();
	switch(serviceId){
		// 售票 || 预售票 || 读卡测试
		case ISSUE_SVC:
		case PREISSUE_SVC:	
		case OUTRW_TEST_SVC:
			readCard.FunctionCode = FUNCTION_CODE_ISSUE;				
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
			readCard.FunctionCode = 03;//FUNCTION_CODE_OTHER;
			readCard.IsReadRecords = true;
			break;
			// 激活
		case REFRESH_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = 03;//FUNCTION_CODE_REFRESH_AUTO;
			readCard.IsReadRecords = true;
			break;
			// 换卡
		case EXCHANGE_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = 03;//FUNCTION_CODE_EXCHANGE_OLD;
			readCard.IsReadRecords = true;
			break;
			// 退资
		case REFUND_SVC:
			readCard.FunctionCode = FUNCTION_CODE_REFUND_MONEY;
			break;
			// 抵消
		case COUNTERACT_SVC:
			readCard.FunctionCode = FUNCTION_CODE_CONTERACT;
			break;
		default:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			break;
	}

	return TryReadCard(readCard,rspReadCard);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      中止读写器命令

@param      (i)bool checkStatus      true:先检查读写器状态; false:不检查读写器状态

@retval     long   错误代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTWHelper::Abort(bool checkStatus)
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
long CTWHelper::StopReadCard()
{
	CRWStopReadCard pRwCmd(s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
bool CTWHelper::SynchronismParam()
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

	// ACC运营参数
	if(versionAcc.lBusinessParam != theACC_BUSINESS.GetCurMasterVer()){
		vecParamId.push_back(ACC_BUSINESS_ID);
	}
	// ACC行政处理手续费参数
	if(versionAcc.lServerFeeParam != theACC_SERVICEFEE.GetCurMasterVer()){
		vecParamId.push_back(ACC_SERVICEFEE_ID);
	}
	// ACC超时补交费用参数
	if(versionAcc.lOverTimeAdjuestParam != theACC_OVERTIMEADJUEST.GetCurMasterVer()){
		vecParamId.push_back(ACC_OVERTIME_UPDATE_ID);
	}
	// ACC设备控制参数
	if(versionAcc.lDeviceControlParam != theACC_DEVICECONTROL.GetCurMasterVer()){
		vecParamId.push_back(ACC_DEVICE_ID);
	}
	// ACC车站位置参数
	if(versionAcc.lStationParam != theACC_LINE.GetCurMasterVer()){
		vecParamId.push_back(ACC_STATION_ID);
	}
	// ACC计费站点参数
	if(versionAcc.lPayStationParam != theACC_TOLLSTATION.GetCurMasterVer()){
		vecParamId.push_back(ACC_CHARGE_STATION_ID);
	}
	// ACC票卡属性参数
	if(versionAcc.lCardAttributeParam != theACC_TICKET.GetCurMasterVer()){
		vecParamId.push_back(ACC_CARDATTRIBUTE_ID);
	}
	// ACC基础票价表参数
	if(versionAcc.lBasePriceParam != theACC_BASEPRICE.GetCurMasterVer()){
		vecParamId.push_back(ACC_BASE_PRICE_TABLE_ID);
	}
	// ACC票价级别表参数
	if(versionAcc.lPrieLevelParam != theACC_PRICELEVEL.GetCurMasterVer()){
		vecParamId.push_back(ACC_PRICE_LEVEL_TABLE_ID);
	}
	// ACC票价调整策略表参数
	if(versionAcc.lPriceAdjuestParam != theACC_PRICEADJUEST.GetCurMasterVer()){
		vecParamId.push_back(ACC_PRICE_ADJUEST_TABLE_ID);
	}
	// ACC时间浮动表参数
	if(versionAcc.lTimeFloatParam != theACC_TIMEFLOAT.GetCurMasterVer()){
		vecParamId.push_back(ACC_TIME_FLOATING_TABLE_ID);
	}
	// ACC时间类型表参数
	if(versionAcc.lTimeTypeParam != theACC_TIMETYPE.GetCurMasterVer()){
		vecParamId.push_back(ACC_TIME_TYPE_TABLE_ID);
	}
	// ACC时间类型明细参数
	if(versionAcc.lTimeDetailParam != theACC_TIMEDETAIL.GetCurMasterVer()){
		vecParamId.push_back(ACC_TTIME_TYPE_DETAIL_ID);
	}
	// ACC折扣率表参数
	if(versionAcc.lDiscountParam != theACC_DISCOUNT.GetCurMasterVer()){
		vecParamId.push_back(ACC_DISSCOUNT_TABLE_ID);
	}
	// ACC停运车站表
	if(versionAcc.lNotServStationParam != theACC_NOTSERVSTATION.GetCurMasterVer()){
		vecParamId.push_back(ACC_NOT_SERVICE_STATION_ID);
	}
	// ACC区段参数
	if(versionAcc.lSectionParam != theACC_SECTION.GetCurMasterVer()){
		vecParamId.push_back(ACC_SECTION_ID);
	}
	// ACC白名单参数
	if(versionAcc.lWriteListParam != theACC_WRITELIST.GetCurMasterVer()){
		vecParamId.push_back(ACC_WRITE_LIST_ID);
	}
	// ACC异地卡/城市代码对照参数
	if(versionAcc.lOtherCityCardParam != theACC_OTHERCITYCARD.GetCurMasterVer()){
		vecParamId.push_back(ACC_OTHER_CITY_CARD_ID);
	}
	// ACC单条黑名单参数
	if(versionAcc.lSingleBlackListParam != theACC_SINGLEBLACKLIST.GetCurMasterVer()){
		vecParamId.push_back(ACC_SINGLE_BLACK_LIST_ID);
	}
	// ACC区段黑名单参数
	if(versionAcc.lSectionBlackListParam != theACC_SECTIONBLACKLIST.GetCurMasterVer()){
		vecParamId.push_back(ACC_SECTION_BLACK_LIST_ID);
	}
	// ACC票卡发行商参数
	if(versionAcc.lissueCompanyParam != theACC_ISSUECOMPANY.GetCurMasterVer()){
		vecParamId.push_back(ACC_ISSUE_COMPANY_ID);
	}
	// AFC模式履历参数(因TPU问题暂时注释)
	CCTLOGSoftVer::VERSION_INFO Version_Info;
	theSOFT_VERSION.GetSoftVerData(Version_Info);
	if(versionAfc.lModeHistoryParam != Version_Info.dwModeHistory){
		vecParamId.push_back(AFC_MODEHISTORY_ID);
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
bool CTWHelper::UpdateParamter(CParamHelper::VEC_PARAM_ID vecParamId)
{
	bool ret = true;
	CString paramPathName = _T("");
	CString sPrefix = PARAM_PREFIX;	
	CString ParamPath = _T("");

	// 遍历参数ID
	vector<WORD>::iterator iteId = vecParamId.begin();
	while (iteId != vecParamId.end()) {
		// 取得参数文件路径
		if(*iteId == AFC_MODEHISTORY_ID || *iteId == AFC_DEVICECOMMONPARAM_ID || *iteId == AFC_STAFFPWDPARAM_ID
			||*iteId == AFC_ACCESSLEVELPARAM_ID ||*iteId == AFC_BOMOPERATIONPARAM_ID ||*iteId == AFC_DEVICERUNTIMEPARAM_ID){
				ParamPath = PARAM_AFC_CURRENT_DIR;
		}
		else if(*iteId == ACC_BUSINESS_ID 
			|| *iteId == ACC_SERVICEFEE_ID 
			|| *iteId == ACC_OVERTIME_UPDATE_ID 
			|| *iteId == ACC_DEVICE_ID 
			|| *iteId == ACC_STATION_ID 
			|| *iteId == ACC_CHARGE_STATION_ID 
			|| *iteId == ACC_SECTION_ID 
			|| *iteId == ACC_CARDATTRIBUTE_ID 
			|| *iteId == ACC_BASE_PRICE_TABLE_ID 
			|| *iteId == ACC_PRICE_LEVEL_TABLE_ID 
			|| *iteId == ACC_PRICE_ADJUEST_TABLE_ID 
			|| *iteId == ACC_TIME_FLOATING_TABLE_ID 
			|| *iteId == ACC_TIME_TYPE_TABLE_ID 
			|| *iteId == ACC_TTIME_TYPE_DETAIL_ID 
			|| *iteId == ACC_DISSCOUNT_TABLE_ID 
			|| *iteId == ACC_NOT_SERVICE_STATION_ID 
			|| *iteId == ACC_WRITE_LIST_ID 
			|| *iteId == ACC_OTHER_CITY_CARD_ID 
			|| *iteId == ACC_SINGLE_BLACK_LIST_ID 
			|| *iteId == ACC_SECTION_BLACK_LIST_ID
			|| *iteId == ACC_ISSUE_COMPANY_ID){
			ParamPath = PARAM_ACC_CURRENT_DIR;
		}
		else{
			CString  sParaID = _T("");
			sParaID.Format(_T("%.4x"),*iteId);  
			throw CTWException(CTWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__,_T("读写器返回的参数ID异常:" + sParaID));
		}
		paramPathName = thePARAM_HELPER.GetTheParamPathName(*iteId,ParamPath);
		if(thePARAM_HELPER.GetFileNameFromFullPath(paramPathName).GetLength()!=0){
			// 读出bin文件的信息
			VARIABLE_DATA data;
			CXFile file(paramPathName,_T(""));
			file.ReadBinFileWithoutSUM(data);

			// 发出RW的更新命令
			RW_PARAM_DOWNLOAD input;
			input.fileData = data.lpData;
			input.size = data.nLen;		
			input.paramId = *iteId;

			int iRet = DownloadParamter(input);
			if(iRet != RW_RSP_OK){
				ret = false;
				int iLastErrCode = 0;
				GetErrCode(iLastErrCode);	
			}
		}
		else{
			CString  sParaID = _T("");
			sParaID.Format(_T("%.4x"),*iteId);  
			throw CTWException(CTWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__,_T("读写器同步用本地参数不存在:" + sParaID));
		}
		iteId ++;
		//Sleep(1000);
	}
	return ret;
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
long CTWHelper::Open()
{
	RW_CMD_OPEN cmd_open;
	cmd_open.isUsb = false;
	cmd_open.port = theBOM_INFO.GetRWCommPort(false);
	cmd_open.baud = theBOM_INFO.GetRWBaudRate(false);
	CRWOpen pRwCmd(cmd_open,s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::Close()
{
	CRWClose pRwCmd(s_TWID);
	long errCode =  pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::Reset(RW_RSP_STATUS& status)
{
	CRWReset pRwCmd(s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::GetStatus(RW_RSP_STATUS& status)
{
	CRWGetStatus pRwCmd(s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::Abort(RW_RSP_STATUS& status)
{
	CRWAbort pRwCmd(s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::Stop(RW_RSP_STATUS& status)
{
	CRWStop pRwCmd(s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::SetTime(_DATE_TIME currentDateTime,_DATE_TIME& tpuDateTime)
{
	CRWSetTime pRwCmd(currentDateTime,s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::GetTime(_DATE_TIME& tpuDateTime)
{
	CRWGetTime pRwCmd(s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::GetMac(BYTE* hashData,BYTE* mac)
{
	WORD keyVersion = 1;
	CRWGetMac pRwCmd(keyVersion,hashData,s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::GetErrCode(int &iErrorCode)
{
	CRWGetErrCode pRwCmd(iErrorCode,s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::GetTPUInfo(RW_RSP_TPU_INFO& tpuInfo)
{
	CRWGetTpuInfo pRwCmd(s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::TWFirmwareDownload(RW_CMD_FIRMWARE input)
{
	CRWFirmwareDownload pRwCmd(&input,s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if(RW_RSP_OK != errCode) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::InitTW(RW_CMD_INIT initInfo,RW_RSP_STATUS& status)
{
	CRWInit pRwCmd(initInfo,s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::GetTicketPrice(RW_CMD_TICKET_PRICE ticketInfo,RW_CMD_TICKET_PRICE_RES& res)
{
	CRWGetTicketPrice pRwCmd(ticketInfo,s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	//if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&res);
	//}
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
long CTWHelper::GetParameterInfo(VERSION_ACC& accVersion,VERSION_ECT& ectVersion,VERSION_AFC& afcVersion)
{
	CRWGetParameterInfo pRwCmd(s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
				paraVer=(DWORD)ComMakeLong(paramData.ParamData[i].ParaVer[3],paramData.ParamData[i].ParaVer[2],paramData.ParamData[i].ParaVer[1],paramData.ParamData[i].ParaVer[0]);
				switch(paraID){
					case ACC_BUSINESS_ID:// ACC运营参数
						accVersion.lBusinessParam = paraVer;
						break;
					case ACC_SERVICEFEE_ID:// ACC行政处理手续费参数
						accVersion.lServerFeeParam = paraVer;
						break;
					case ACC_OVERTIME_UPDATE_ID:// ACC超时补交费用参数
						accVersion.lOverTimeAdjuestParam = paraVer;
						break;
					case ACC_DEVICE_ID:// ACC设备控制参数
						accVersion.lDeviceControlParam = paraVer;
						break;
					case ACC_STATION_ID:// ACC车站位置参数
						accVersion.lStationParam = paraVer;
						break;
					case ACC_CHARGE_STATION_ID:// ACC计费站点参数
						accVersion.lPayStationParam = paraVer;
						break;
					case ACC_SECTION_ID:// ACC区段参数
						accVersion.lSectionParam = paraVer;
						break;
					case ACC_CARDATTRIBUTE_ID:// ACC票卡属性参数
						accVersion.lCardAttributeParam = paraVer;
						break;
					case ACC_BASE_PRICE_TABLE_ID:// ACC基础票价表参数
						accVersion.lBasePriceParam = paraVer;
						break;
					case ACC_PRICE_LEVEL_TABLE_ID:// ACC票价级别表参数
						accVersion.lPrieLevelParam = paraVer;
						break;
					case ACC_PRICE_ADJUEST_TABLE_ID:// ACC票价调整策略表参数
						accVersion.lPriceAdjuestParam = paraVer;
						break;
					case ACC_TIME_FLOATING_TABLE_ID:// ACC时间浮动表参数
						accVersion.lTimeFloatParam = paraVer;
						break;
					case ACC_TIME_TYPE_TABLE_ID:// ACC时间类型表参数
						accVersion.lTimeTypeParam = paraVer;
						break;
					case ACC_TTIME_TYPE_DETAIL_ID:// ACC时间类型明细参数
						accVersion.lTimeDetailParam = paraVer;
						break;
					case ACC_DISSCOUNT_TABLE_ID:// ACC折扣率表参数
						accVersion.lDiscountParam = paraVer;
						break;
					case ACC_NOT_SERVICE_STATION_ID:// ACC停运车站表
						accVersion.lNotServStationParam = paraVer;
						break;
					case ACC_WRITE_LIST_ID:// ACC白名单参数
						accVersion.lWriteListParam = paraVer;
						break;
					case ACC_OTHER_CITY_CARD_ID:// ACC异地卡/城市代码对照参数
						accVersion.lOtherCityCardParam = paraVer;
						break;
					case ACC_SINGLE_BLACK_LIST_ID:// ACC单条黑名单参数
						accVersion.lSingleBlackListParam = paraVer;
						break;
					case ACC_SECTION_BLACK_LIST_ID:// ACC区段黑名单参数
						accVersion.lSectionBlackListParam = paraVer;
						break;
					case ACC_ISSUE_COMPANY_ID:// ACC票卡发行商参数
						accVersion.lissueCompanyParam = paraVer;
						break;
					case AFC_MODEHISTORY_ID://模式履历参数
						afcVersion.lModeHistoryParam = paraVer;
						break;
					default:
						CString  sParaID = _T("");
						sParaID.Format(_T("%.4x"),paraID);  
						//throw CTWException(errCode, _T(__FILE__), __LINE__,_T("内部读写器返回的参数ID异常:" + sParaID));						
				}
			}
		}
	}
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
long CTWHelper::DownloadParamter(RW_PARAM_DOWNLOAD paramInfo)
{
	CRWParamFirmware pRwCmd(paramInfo,s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::SetOperationMode(RW_CMD_SET_MODE cmdSetMode,RW_RSP_STATUS& status)
{
	CRWSetOperationMode pRwCmd(cmdSetMode,s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::SetBusinessTime(RW_CMD_SET_TIME cmdSetMode,RW_RSP_STATUS& status)
{
	CRWSetBusinessTime pRwCmd(cmdSetMode,s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CTWHelper::GetUDData(RW_BUSSINESS_RESPONSE& commonResponse)
{
	CRWGetUDData pRwCmd(s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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
void CTWHelper::ReadCard(RW_READ_CARD_REQUEST cmdReadCard)
{
	CRWReadCard* pRwCmd = new CRWReadCard(cmdReadCard,s_TWID);
	pRwCmd->SetAutoDeleted(TRUE);
	pRwCmd->SetService(&m_service);
	pRwCmd->SetCommandID(RW_COMMAND_ID_READ_CARD);
	g_pRWControl->DoCommand(pRwCmd,SP_EXECMODE_QUEUE);
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
long CTWHelper::TryReadCard(RW_READ_CARD_REQUEST cmdReadCard,RW_READ_CARD_RESPONSE& rspReadCard)
{
	CRWTryReadCard rwCmd(cmdReadCard,s_TWID);
	long result = g_pRWControl->DoCommand(&rwCmd);
	rwCmd.GetRWResponse(&rspReadCard);
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
long CTWHelper::Issue(RW_ISSUE_REQUEST issueInput, RW_BUSSINESS_RESPONSE& response)  
{
	CRWIssue pRwCmd(issueInput,s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
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

//////////////////////////////////////////////////////////////////////////
/**
@brief      锁卡命令

@param      (i)RW_CMD_BLOCK blockInput             锁卡输入参数结构
@param      (o)RW_BUSSINESS_RESPONSE& response     业务返回通用结构

@retval     long   错误代码   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTWHelper::Block(RW_CMD_BLOCK blockInput, RW_BUSSINESS_RESPONSE& response)  
{
	CRWBlock pRwCmd(blockInput,s_TWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CTWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&response);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断读写器是否正常

@param      none

@retval     bool true:异常 false:正常 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CTWHelper::IsTWHasException()
{
	return theEXCEPTION_MGR.HasException(CTWHardwareException::MODULE_ID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否为需要抛异常的ID

@param      (i)long errCode      输入参数结构体

@retval     bool \n  true:异常的错误代码; false:不抛异常的

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTWHelper::IsExceptionCode(long errCode)
{
	bool ret = false;

	switch(errCode) {
	case RW_RSP_STATUSERROR:       // TPU状态非法
	case RW_RSP_COMERR:            // 通讯接口类错误
	case RW_RSP_SAMERR:
		//case RW_RSP_PARAMERR:	   // 配置参数类错误
	case RW_RSP_UDERR:		       // UD数据类错误
	case RW_RSP_HARDWAREERR:	   // 硬件类错误
	case RW_RSP_UNKNOWERR:		   // 未归类错误
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
int CTWHelper::UpdateProgram(bool isMain)
{
	int iRet = 0;
	try
	{
		CString fileName = isMain?MID_STR_BOM_RW_PROG_MAIN:MID_STR_BOM_RW_PROG_APP;
		vector<CString> updateFiles;

		CString sAppPath;
		GetAppPath(sAppPath);
		CString updateFileFolder = sAppPath + BOM_TW_UPDATE_DIR;

		SearchFile(updateFileFolder + fileName + _T("*.*"),updateFiles);
		if(updateFiles.size()<=0)
		{
			return -2;
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
				return -3;
			}

			// 读出bin文件的信息
			VARIABLE_DATA data;
			CXFile file(updateFile,_T(""));
			file.ReadBinFileWithoutSUM(data);

			DeleteFile(updateFile);

			// 发出TW的更新命令
			RW_CMD_FIRMWARE input;
			input.fileData = data.lpData;
			input.size = data.nLen;
			input.isMain = isMain;

			theAPP_SESSION.SetHasProgUpdate(true);
			theSETTING.SetRWUpdateFlag(1,false);
			if (RW_RSP_OK == TWFirmwareDownload(input)) {
				theSETTING.SetRWUpdateFlag(0,false);
				//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,progType,upgradeVersion);//更新本地版本
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
CString CTWHelper::GetTpuErrorInfo(int iError)
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
CString CTWHelper::gGetErrorInfofromFile(LPCTSTR pstrFile, DWORD dwErrorCode)
{

	CString pstrErrorCode(_T(""));
	pstrErrorCode.Format(_T("0x%08X"), dwErrorCode);

	if (pstrErrorCode == "" || pstrFile == NULL)
		return _T("");


	CString strInfo(_T(""));
	try
	{
		CString strFlag1(_T("/*")), strFlag2(_T("*/"));
		CStdioFile file(pstrFile, CFile::typeText | CFile::modeRead);
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
			strInfo = _T("未知错误");
			theDEBUG_LOG->WriteData(_T(""), _T("gGetErrorInfromFile"), __LINE__, _T("没有找到相应的错误信息。"));
		}
	}
	catch (CFileException e)
	{
#ifdef _DEBUG
		afxDump << "File could not be opened "
			<< e.m_cause << "\n";
#endif
		TCHAR strError[256];
		memset(strError, 0, sizeof(strError));
		e.GetErrorMessage(strError, 256);
		theDEBUG_LOG->WriteData(_T(""), _T("gGetErrorInfromFile"), __LINE__, strError);
	}
	strInfo = _T("（红）") + strInfo +_T("\r\n")+ _T("(")+pstrErrorCode+_T(")");
	return strInfo;
}