#include "StdAfx.h"
#include "FunctionInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//static CString FUNCTION_INI_FILE_NAME = _T("TVM_Functioninfo.INI");
CFunctionInfo CFunctionInfo::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CFunctionInfo::CFunctionInfo()
{
    try {
        m_functionIni = NULL;
		// [TVM_WEB_API]
		//m_strWebMobileHost = _T("");
		//m_WebApiHost       = _T("");
		m_WebApiPort       = 0;
		//m_WebApiPwd        = _T("");
		m_WebApiLoginUrl   = _T("");					 // WEB_API  登录地址
		//m_WebApiQueryUrl   = _T("");					 // WEB_API  查询地址
		m_WebApiLockUrl    = _T("");					 // WEB_API  锁定地址
		//m_WebApiConfirmUrl = _T("");					 // WEB_API  提交地址
		//m_WebApiGetOrderUrl   = _T("");					 // WEB_API  取票码获取订单号地址
		//m_WebApiTVMStateUrl    = _T("");				 // WEB_API  查询TVM状态地址
		//m_WebApiUpdateStateUrl = _T("");				 // WEB_API  修改TVM状态地址
        // 初期化
        Initialize();
    }
    catch (...) {
        // 忽略异常信息
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CFunctionInfo::~CFunctionInfo()
{
    delete m_functionIni;
    m_functionIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化

@param       无 

@retval      int 0 成功，1 失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::Initialize() 
{
    try {
        // 若已初始化，则直接返回
        if (m_functionIni != NULL) {
            return 0;
        }

        TCHAR szAppPath[_MAX_PATH];

        if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
			return 1;
        }
	
        CString fileName = CString(szAppPath) + FUNCTION_INI_FILE_NAME;

        // check Functioninfo.INI file exist
        if (FALSE == ComCheckFileExist(fileName)) {
			return 1;
        }

        // 文件属性改为可写
        SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

        m_functionIni = new CGetIni(fileName);
		//[APPLICATION]
		m_accAuthorizeManner = m_functionIni->GetDataInt(_T("APPLICATION"), _T("ACCAuthorizeManner"),2);			// ACC认证方式（0：不认证；1：内部模式；2：外部模式）
		m_ectAuthorizeManner = m_functionIni->GetDataInt(_T("APPLICATION"), _T("ECTAuthorizeManner"),2);			// ECT认证方式（0：不认证；1：内部模式；2：外部模式）

		// [TVM_WEB_API]
		//m_strWebMobileHost = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("WebHost"));
		m_WebApiHost       = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("Host"));
		m_WebApiPort       = m_functionIni->GetDataInt(_T("TVM_WEB_API"), _T("Port"));
		//m_WebApiPwd       = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("UserPwd"));
		m_WebApiLoginUrl   = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("LoginUrl"));
		//m_WebApiQueryUrl   = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("QueryUrl"));
		m_WebApiLockUrl    = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("LockOperatorUrl"));
		/*m_WebApiConfirmUrl = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("ConfirmUrl"));
		m_WebApiGetOrderUrl   = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("GetOrderUrl"));
		m_WebApiTVMStateUrl    = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("TVMStatUrl"));
		m_WebApiUpdateStateUrl = m_functionIni->GetDataString(_T("TVM_WEB_API"), _T("UpdateStateUrl"));*/

		return 0;
   }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
		throw CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       判断指定线路按钮是否可用

@param       BYTE lineCode 线路编码

@retval      bool true:可用 ; false:不可用

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsLineEnabled(DWORD lineCode) 
{
	if(mapLineEnable.find(lineCode) == mapLineEnable.end())
	{
		CString slineCode(_T(""));
		slineCode.Format(_T("%d"),lineCode);
		DWORD lineStatus = m_functionIni->GetDataInt(_T("LINE_ENABLE"), slineCode, 1);
		mapLineEnable[lineCode] =  (lineStatus != 0);
	}
	return mapLineEnable[lineCode];
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       判断指定车站按钮是否可用

@param       WORD stationCode 车站编码

@retval      bool true:在最前面 ; false:不在最前面

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsStationEnabled(WORD stationCode)
{
	if(mapStationEnable.find(stationCode) == mapStationEnable.end())
	{
		CString sStationCode(_T(""));
		sStationCode.Format(_T("%d"),sStationCode);
		DWORD stationStatus = m_functionIni->GetDataInt(_T("STATION_ENABLE"), sStationCode, 1);
		mapStationEnable[stationCode] = (stationStatus != 0);
	}
	return mapStationEnable[stationCode];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断菜单按钮是否可用

@param       (i)WORD menuID			菜单ID
@param       (i)CString menuText	按钮文字

@retval      bool true:可见 ; false:不可见

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsMenuEnabled(WORD menuID)
{
	CString key(_T(""));
	key.Format(_T("%d"),menuID);
	DWORD menuStatus = m_functionIni->GetDataInt(_T("MENU_ENABLE"), key, 1);
	return (menuStatus != 0);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsMenuVisibled(WORD menuID)
{
	CString key(_T(""));
	key.Format(_T("%d"),menuID);
	DWORD menuStatus = m_functionIni->GetDataInt(_T("MENU_VISIBLE"), key, 1);
	return (menuStatus != 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断票种是否可发售

@param       (i)WORD TicketTypeId	票种ID
@param       (i)CString menuText	按钮文字

@retval      bool true:可 ; false:不可

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsTicketIssueEnabled(WORD TicketTypeId)
{
	CString key(_T(""));
	key.Format(_T("%d"),TicketTypeId);
	DWORD menuStatus = m_functionIni->GetDataInt(_T("ISSUE_ENABLE"), key, 0);
	return (menuStatus != 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断可用付款种类

@param       (i)WORD PayTypeId	付费方式ID

@retval      bool true:可 ; false:不可

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsPayTypeEnable(WORD ServiceID,CardType_t TicketType,int PayTypeId)
{
	
	CString PayTypeNames = _T("");

	CString key(_T(""));
	key.Format(_T("%.2d"),PayTypeId);
	switch(ServiceID){
		case ISSUE_SVC:			
			PayTypeNames =_T("ISSUE");
			break;
		case CHARGE_SVC:		
			PayTypeNames =_T("CHARGE");	
			break;
		case ISSUE_PRODUCT_SVC:
			PayTypeNames =_T("ISSUE_PRODUCT");	
			break;
	}
	switch(TicketType){
		/*case CARD_TYPE_MACAUPSSS_M1:
			PayTypeNames +=_T("_M1");
			break;*/
		//case CARD_TYPE_MACAUPSSS_CPU:
		//	PayTypeNames +=_T("_CPU");
		//	break;
		case CARD_TYPE_ACC_UL:
		case CARD_TYPE_SY_EPT_UL:
			PayTypeNames +=_T("_UL");
			break;
		case CARD_TYPE_ACC_M1:
			PayTypeNames +=_T("_M1");
			break;
	}
	return m_functionIni->GetDataInt(PayTypeNames,key,0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断付款方式是否可用

@param       (i)WORD PayTypeId	付费方式ID

@retval      bool true:可 ; false:不可

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
vector<int> CFunctionInfo::GetTicketCounts()
{
	if (m_vTicketCounts.size()!=0){
		return m_vTicketCounts;
	}

	for (int i =1;;i++ ){
		CString key(_T(""));
		key.Format(_T("%d"),i);
		int ticketCounts = m_functionIni->GetDataInt(_T("TICKET_COUNT"),key,-1);
		if (ticketCounts != -1){
			m_vTicketCounts.push_back(ticketCounts);
		}
		else{
			break;
		}
	}
	return m_vTicketCounts;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取按金额购票票价

@param      无

@retval     vector<int> m_vTicketAmout;(可售票金额集)

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
vector<int> CFunctionInfo::GetTicketAmount(){
	if (m_vTicketAmount.size()!=0){
		return m_vTicketAmount;
	}

	for (int i =1;;i++ ){
		CString key(_T(""));
		key.Format(_T("%d"),i);
		int ticketAmounts = m_functionIni->GetDataInt(_T("TICKET_AMOUNT"),key,-1);
		if (ticketAmounts == -1){
			break;
		}
		else if(ticketAmounts != 0){
			m_vTicketAmount.push_back(ticketAmounts);
		}
	}
	return m_vTicketAmount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取允许最大未送数据天数

@param       无 

@retval      UINT

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
UINT CFunctionInfo::GetLimitedDaysOfUnSendData()
{
	return m_functionIni->GetDataInt(_T("TXNDATA"),_T("UnsendMaxDay"),7);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取已送数据保留天数
//
//@param       无 
//
//@retval      UINT
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
UINT CFunctionInfo::GetLimitedDaysOfSendData()
{
	return m_functionIni->GetDataInt(_T("TXNDATA"),_T("SendedMaxDay"),10);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       设置已送数据保留天数

//@param       无 
//
//@retval      UINT
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
void CFunctionInfo::SetLimitedDaysOfSendData(int nDays)
{
	CString strTemp;
	strTemp.Format(_T("%d"),nDays);
	m_functionIni->WriteData(_T("TXNDATA"),_T("SendedMaxDay"),strTemp);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       是否启用方正员工
//
//@param       无
//
//@retval      bool true:启用 ; false:不启用
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsUseFounderStaffID()
{
	return m_functionIni->GetDataInt(_T("APPLICATION"), _T("UseFounderStaff"),0) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取方正员工ID

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::GetFounderStaffID()
{
	return m_functionIni->GetDataInt(_T("APPLICATION"), _T("FounderStaffID"), 10101028);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsFounderStaffID(int userID)
{
	return IsUseFounderStaffID() && (GetFounderStaffID() == userID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取欧姆龙员工密码

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::GetFounderStaffPassword()
{
	return m_functionIni->GetDataString(_T("APPLICATION"), _T("FounderStaffPassword"), _T("888888"));
}
////////////////////////////////////////////////////////////////////////////
//**
//@brief       获取ACC认证方式
//
//@param       无 
//
//@retval      int
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
int  CFunctionInfo::GetACCAuthorizeManner()
{
	return m_accAuthorizeManner;
}

////////////////////////////////////////////////////////////////////////////
//**
//@brief       获取ECT认证方式
//
//@param       无 
//
//@retval      int
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
int  CFunctionInfo::GetECTAuthorizeManner()
{
	return m_ectAuthorizeManner;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      TPU参数压缩标志（0：不压缩；1：需要压缩）

@param      无 

@retval     bool

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsTpuParamterZipFlag()
{
	return m_functionIni->GetDataInt(_T("APPLICATION"), _T("TpuParamterZipFlag"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      是否启用高级功能（0：不启用；1：启用）

@param      无 

@retval     bool

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsFounderOperation()
{
	return m_functionIni->GetDataInt(_T("APPLICATION"), _T("FounderOperation"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      USB认证标志（0：不认证；1：需要认证）

@param      无 

@retval     bool

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::GetUSBAuthFlag()
{
	return m_functionIni->GetDataInt(_T("APPLICATION"), _T("UsbAuthorizeManner"),0) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否删除参数协议头

@param      无

@retval     bool true:删除；false：不删除

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsDeleteParameterHead(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("IsDeleteParameterHead"),0) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否显示初始化过程

@param      无

@retval     bool true:显示；false：不显示

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsShowInitProcess(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("IsShowInitProcess"),0) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设备自动运行状态是否开启？

@param      无

@retval     bool true:开启 false : 关闭

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsEquipmentAutomaticRunningOn(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("EquipmentAutomaticRunningOnOff"),0) == 0 ? false:true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否显示设备空及将空参数值？

@param      无

@retval     bool true:显示 false : 不显示

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsShowNearlyFullEmptyParaValue(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("IsShowNearlyFullEmptyParaValue"),0) == 0 ? false:true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取最近的打印失败的条数

@param      无

@retval     (O)int 需要打印的条数

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::GetPrintLastReceiptCount(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("PrintLastReceiptCount"),16);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否显示支付方式区域

@param      无

@retval     bool true 显示; false 不显示

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsShowPaymentMethod(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("IsShowPaymentMethod"),1) == 1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否需要用户点击确认按钮

@param      无

@retval     bool true 需要; false 不需要

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsNeedPassengerClickOkButton(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("IsNeedPassengerClickOkButton"),1) == 1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      现金门未关闭时是否降级？

@param      无

@retval     bool：true降级；false：不降级

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsAutoDegradeWhenCashDoorOpen(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("IsAutoDegradeWhenCashDoorOpen"),1) == 1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否自动读取纸币找零模块RFID

@param      无

@retval     bool true(是);false(否);

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsBanknoteChangeBoxIDRead(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("BanknoteChangeBoxIDRead"),1) == 1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      最大可加载交易历史记录条数

@param      无

@retval     int nCount;数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::GetLoadMaxTransactionHistory(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("LoadMaxTransactionHistory"),20);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      操作员登录后默认业务（Service ID）

@param      无 

@retval     int（Service ID）

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsStaffDefaultService()
{
	return m_functionIni->GetDataInt(_T("APPLICATION"), _T("StaffDefaultService"));
}

//************=====RFID设置=========**************//
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsCardNormalBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("CardNormalBoxRFIDWriteable"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsCardWasteBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("CardWasteBoxRFIDWriteable"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsCoinChangeBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("CoinChangeBoxRFIDWriteable"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsCoinRecycleBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("CoinChangeBoxRFIDWriteable"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsBanknoteChangeBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("BanknoteChangeBoxRFIDWriteable"),0) == 0 ? false : true;
}
bool CFunctionInfo::IsBanknoteRecycleBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("BanknoteRecycleBoxRFIDWriteable"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsTokenNormalBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("TokenNormalBoxRFIDWriteable"),0) == 0 ? false : true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsTokenWasteBoxRFIDWriteable()
{
	return m_functionIni->GetDataInt(_T("RFID"), _T("TokenWasteBoxRFIDWriteable"),0) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否自动读取RFID

@param      无

@retval     bool true:不用手动输入; false:需要手动输入

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsAutoReadRfidWithoutManual(){
	return m_functionIni->GetDataInt(_T("APPLICATION"),_T("IsAutoReadRFIDWithoutManual"),0) == 0 ? false : true;
}

//************=====警报设定=========**************//
//////////////////////////////////////////////////////////////////////////
/*
@brief      回收动作结束报警音路径

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::IsCollectOverFile()
{
    return m_functionIni->GetDataString(_T("ALARM"), _T("CollectOverFile"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      回收动作结束报警音播放时长

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int    CFunctionInfo::IsCollectOverBuzzerTime()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("CollectOverBuzzerTime"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      回收动作结束报警音播放时间间隔

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int    CFunctionInfo::IsCollectOverBuzzerInterval()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("CollectOverBuzzerInterval"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      异常发生报警音路径

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::IsExceptionOccurFile()
{
	return m_functionIni->GetDataString(_T("ALARM"), _T("ExceptionOccurFile"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      异常发生报警音播放时长

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int    CFunctionInfo::IsExceptionOccurBuzzerTime()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("ExceptionOccurBuzzerTime"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      异常发生报警音播放时间间隔

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int    CFunctionInfo::IsExceptionOccurBuzzerInterval()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("ExceptionOccurBuzzerInterval"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      异常发生报警音路径

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::IsOperationMissFile()
{
	return m_functionIni->GetDataString(_T("ALARM"), _T("OperationMissFile"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      异常发生报警音播放时长

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int    CFunctionInfo::IsOperationMissBuzzerTime()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("OperationMissBuzzerTime"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      异常发生报警音播放时间间隔

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int    CFunctionInfo::IsOperationMissBuzzerInterval()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("OperationMissBuzzerInterval"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      自动降级发生报警音路径

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::IsDemotionOccurFile()
{
	return m_functionIni->GetDataString(_T("ALARM"), _T("OperationMissFile"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      自动降级发生报警音播放时长

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int    CFunctionInfo::IsDemotionOccurBuzzerTime()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("OperationMissBuzzerTime"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      自动降级发生报警音播放时间间隔

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::IsDemotionOccurBuzzerInterval()
{
	return m_functionIni->GetDataInt(_T("ALARM"), _T("OperationMissBuzzerInterval"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      根据业务模式代码获取业务模式对应的葡萄牙文

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getModeLanguagePN(WORD wdModeCode)
{
	CString szRet(_T(""));
	CString szWorkCode(_T(""));
	szWorkCode.Format(_T("%04x"),wdModeCode);
	CString szWorkLanguage=m_functionIni->GetDataString(_T("STATUS_DISPLAY_CONTENT_BUSINESS"),szWorkCode);
	int ifirst=szWorkLanguage.Find(_T(":"));
	if(ifirst!=-1)
	{
		int isecond=szWorkLanguage.Find(_T(":"),ifirst+1);
		if(isecond!=-1)
		{
			szRet=szWorkLanguage.Mid(isecond+1);
		}else
		{
		}
	}else
	{
	}
	return szRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      根据业务模式代码获取业务模式对应的中文文言

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getModeLanguageCN(WORD wdModeCode)
{
	CString szRet(_T(""));
	CString szWorkCode(_T(""));
	szWorkCode.Format(_T("%04x"),wdModeCode);
	CString szWorkLanguage=m_functionIni->GetDataString(_T("STATUS_DISPLAY_CONTENT_BUSINESS"),szWorkCode);
	int ifirst=szWorkLanguage.Find(_T(":"));
	if(ifirst!=-1)
	{
		szRet=szWorkLanguage.Mid(0,ifirst);
	}else
	{
	}
	return szRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      根据业务模式代码获取业务模式对应的英文文言

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getModeLanguageEN(WORD wdModeCode)
{
	CString szRet(_T(""));
	CString szWorkCode(_T(""));
	szWorkCode.Format(_T("%04x"),wdModeCode);
	CString szWorkLanguage=m_functionIni->GetDataString(_T("STATUS_DISPLAY_CONTENT_BUSINESS"),szWorkCode);
	int ifirst=szWorkLanguage.Find(_T(":"));
	if(ifirst!=-1)
	{
		/*int isecond=szWorkLanguage.Find(_T(":"),ifirst+1);
		if(isecond!=-1)
		{
			szRet=szWorkLanguage.Mid(ifirst+1,isecond-ifirst-1);
		}else
		{
		}*/
		szRet = szWorkLanguage.Mid(ifirst + 1,szWorkLanguage.GetLength() - 1);
	}else
	{
	}
	return szRet;
}

void CFunctionInfo::getLedContentFormat(CStringList& strList)
{
	for(int i=0;i<10;i++)
	{
		CString szKey(_T(""));
		szKey.Format(_T("%d"),i);
		CString szWorkLanguage=m_functionIni->GetDataString(_T("STATUS_DISPLAY"),szKey);
		strList.AddTail(szWorkLanguage);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		获取当前售票指定票箱  

@param      none

@retval     (o)int
            0:不指定；
			1：票箱A；
			2：票箱B

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::GetAvailableMagazineBox(){
	return m_functionIni->GetDataInt(_T("STATUS_ISSUE_SMARTCARD"),_T("ISSUE_FROM_MAGAZINE"),1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		乘客业务启动默认业务异常解除次数  

@param      none

@retval     (o)int
            0:不解除异常；
			>0:次数

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::GetTimesRepareException(){
	return m_functionIni->GetDataInt(_T("REPARE_EXCEPTION_TIMES"),_T("REPARE_TIMES"),1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取操作日志级别及名称

@param      WORD Key

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::GetOperationLogLevelContent(WORD Key,int index)
{
	CStringArray keyvaluearray;		// 按':'分割后字符串存放array
	CString key= _T("");
	key.Format(_T("%04x"),Key);		// 键转换
	CString operationlogcontent=m_functionIni->GetDataString(_T("OPERATION_LOG_LEVEL_CONTENT"),key);		// 获取键值
	
	if(operationlogcontent.IsEmpty()){
		return _T("");
	}
	int find_index = operationlogcontent.ReverseFind(':');			// 反向查找':'
	keyvaluearray.Add(operationlogcontent.Mid(find_index+1));
	while(find_index>0)
	{
		
		operationlogcontent=operationlogcontent.Mid(0,find_index);
		find_index=operationlogcontent.ReverseFind(':');
		keyvaluearray.Add(operationlogcontent.Mid(find_index+1));
	}
	int size = keyvaluearray.GetSize();
	
	return keyvaluearray.GetAt(index);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取本站到全网最高票价

@param      无

@retval     int nMaxAmount;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::GetTicketMaxAmount() const{
	int nMaxAmount = 0;
	nMaxAmount = m_functionIni->GetDataInt(_T("TICKET_MAX_AMOUNT"),_T("TICKET_LIMITS_AMOUNT"),500);

	if(nMaxAmount == 0){
		nMaxAmount = 500;
	}

	return nMaxAmount;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置本站到全网最高票价

@param      int nMaxAmount;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CFunctionInfo::SetTicketMaxAmount(int nMaxAmount) const {
	CString strMaxAmount;
	strMaxAmount.Format(_T("%d"),nMaxAmount);

	m_functionIni->WriteData(_T("TICKET_MAX_AMOUNT"),_T("TICKET_LIMITS_AMOUNT"),strMaxAmount);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      根据业务模式代码获取业务模式对应的语言

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getModeLanguage(WORD wdModeCode,int languageId)
{
	CString szRet(_T(""));
	CString szWorkCode(_T(""));
	szWorkCode.Format(_T("%d"),wdModeCode);
	CString szWorkLanguage=m_functionIni->GetDataString(_T("STATUS_DISPLAY_DETAIL"),szWorkCode);
	int ifirst=szWorkLanguage.Find(_T(":"));
	if(ifirst!=-1)
	{
		if(languageId == 1){
			szRet = szWorkLanguage.Mid(0,ifirst);
		}
		else{
			szRet = szWorkLanguage.Mid(ifirst + 1,szWorkLanguage.GetLength() - 1);
		}
	}
	return szRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      根据业务模式代码获取业务模式对应的语言

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getModeLanguage(WORD wdModeCode)
{
	CString szRet(_T(""));
	CString szWorkCode(_T(""));
	szWorkCode.Format(_T("%d"),wdModeCode);
	CString szWorkLanguage=m_functionIni->GetDataString(_T("STATUS_DISPLAY_DETAIL"),szWorkCode);
	return szWorkLanguage;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取led播放设置

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getLedWorkMode(){
	return m_functionIni->GetDataString(_T("STATUS_DISPLAY_DETAIL"),_T("SHOW_MODE"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      获取状态显示屏中文运行模式

@param      无

@retval     Cstring

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getCNWorkMode()
{
	return m_functionIni->GetDataString(_T("STATUS_DISPLAY_MODE"),_T("CN"),_T("FFF0425"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      获取状态显示屏英文文运行模式

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::getENWorkMode()
{
	return m_functionIni->GetDataString(_T("STATUS_DISPLAY_MODE"),_T("EN"),_T("FFF0755"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 WEB_API 密码

@param       无 

@retval      int

@exception   无
*/
////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::GetWebApiHost()
{
	return m_WebApiHost;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 WEB_API Port

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int     CFunctionInfo::GetWebApiPort()
{
	return m_WebApiPort;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 WEB_API 密码

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
//CString CFunctionInfo::GetWebApiPwd()
//{
//	return m_WebApiPwd;
//}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 WEB_API 登录地址

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::GetWebApiLoginUrl()
{
	return m_WebApiLoginUrl;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 WEB_API 查询地址

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
//CString CFunctionInfo::GetWebApiLockUrl()
//{
//	return m_WebApiLockUrl;
//}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 WEB_API 锁定地址

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CFunctionInfo::GetWebApiLockUrl()
{
	return m_WebApiLockUrl;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 WEB_API 提交地址

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
//CString CFunctionInfo::GetWebApiConfirmUrl()
//{
//	return m_WebApiConfirmUrl;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 WEB_API 取票码获取订单号地址

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
//CString CFunctionInfo::GetWebApiGetOrderUrl()
//{
//	return m_WebApiGetOrderUrl;
//}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 WEB_API 查询TVM状态地址

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
//CString CFunctionInfo::GetWebApiTVMStateUrl()
//{
//	return m_WebApiTVMStateUrl;
//}
