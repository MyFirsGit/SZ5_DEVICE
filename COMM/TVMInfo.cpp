#include "StdAfx.h"
#include "TVMInfo.h"
#include "SysException.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTVMInfo CTVMInfo::theInstance;

CTVMInfo& CTVMInfo::GetInstance()
{
		return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTVMInfo::CTVMInfo():CSYSInfo()
{
    try {
        m_DeviceIni = NULL;
		
        // 初期化
        Initialize();

		// 初始化
		m_strDataPath = _T("");
		m_strDataBackpath = _T("");
		m_strLogPath = _T("");
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
CTVMInfo::~CTVMInfo()
{
    delete m_DeviceIni;
    m_DeviceIni = NULL;
}

//****************************[SERVICE_STATE_TIMER]**********************************************
//////////////////////////////////////////////////////////////////////////
/**
@brief       [结束]->[等待中]定时器时间间隔

@param       

@retval     int(秒)

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetFinishToReadyTimerInterval()
{
	 return m_DeviceIni->GetDataInt(_T("SERVICE_STATE_TIMER"), _T("FinishToReadyTimerInterval"));

}
//////////////////////////////////////////////////////////////////////////
/**
@brief       [操作中]->[等待中]定时器时间间隔

@param       

@retval     int(秒)

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetReceptToReadyTimerInterval()
{

	 return m_DeviceIni->GetDataInt(_T("SERVICE_STATE_TIMER"), _T("ReceptToReadyTimerInterval"));

}
//////////////////////////////////////////////////////////////////////////
/**
@brief       [计数中]->[返回中]定时器时间间隔

@param       

@retval     int(秒)

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCountingToReturnTimerInterval()
{

	return m_DeviceIni->GetDataInt(_T("SERVICE_STATE_TIMER"), _T("CountingToReturnTimerInterval"));

}
//////////////////////////////////////////////////////////////////////////
/**
@brief        [等待中]->[待机中]定时器时间间隔

@param       

@retval     int(秒)

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetReadyToRestTimerInterval()
{

	return m_DeviceIni->GetDataInt(_T("SERVICE_STATE_TIMER"), _T("ReadyToRestTimerInterval"));

}

//////////////////////////////////////////////////////////////////////////
/**
@brief        [开]->[关]找零指示灯计时器

@param       

@retval     int(秒)

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetReturnIndicatorTimerInterval()
{
	return m_DeviceIni->GetDataInt(_T("SERVICE_STATE_TIMER"), _T("ReturnIndicatorTimerInterval"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        [开]->[关]找零照明灯计时器

@param       

@retval     int(秒)

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetReturnLightTimerInterval()
{
	return m_DeviceIni->GetDataInt(_T("SERVICE_STATE_TIMER"), _T("ReturnLightTimerInterval"));
}
//****************************TH通讯参数**********************************************

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使TH

@param       

@retval      bool true:使用TOKEN  false:不使用TOKEN

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsTHUsed()
{
	return (m_DeviceIni->GetDataInt(_T("CARD_MODULE"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取TH串口号

@param       none 

@retval      int

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTHCommPort()
{
    return m_DeviceIni->GetDataInt(_T("CARD_MODULE"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取TH通信方式

@param       none 

@retval      int  0:串口 1:邮槽 2:管道 

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTHCommuMethod()
{
	return m_DeviceIni->GetDataInt(_T("TH"), _T("CMU_METHOD"));
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect1BaudRate

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTHBaudRate()
{
    return m_DeviceIni->GetDataInt(_T("CARD_MODULE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect1MaxConnReceive

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTHMaxConnReceive()
{
    return m_DeviceIni->GetDataInt(_T("CARD_MODULE"), _T("MAX_CONN_RECEIVE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect1MaxConnSend

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTHMaxConnSend()
{
    return m_DeviceIni->GetDataInt(_T("CARD_MODULE"), _T("MAX_CONN_SEND"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTHMonitorInterval()
{
	return (m_DeviceIni->GetDataInt(_T("CARD_MODULE"),_T("MONITOR_INTERVAL"),5000) );
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsTHMonitorOpened()
{
	return (m_DeviceIni->GetDataInt(_T("CARD_MODULE"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief       连续发票测试张数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTHTestRelease()
{
	return (m_DeviceIni->GetDataInt(_T("CARD_MODULE"),_T("RELEASE_TEST_COUNT"),10) );
}

//////////////////////////////////////////////////////////////////////////
/*
@brief        连续回收测试张数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTHTestRecycle()
{
	return (m_DeviceIni->GetDataInt(_T("CARD_MODULE"),_T("RECYCLE_TEST_COUNT"),10) );
}

//////////////////////////////////////////////////////////////////////////
/*
@brief        获取票箱内卡介质类型

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTHBoxMediumType()
{
	return (m_DeviceIni->GetDataInt(_T("CARD_MODULE"),_T("BOX_MEDIUM_TYPE"),2));
}

//****************************TOKEN通讯参数**********************************************

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使TOKEN

@param       

@retval      bool true:使用TOKEN  false:不使用TOKEN

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsTokenUsed()
{
	return (m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取TOKEN串口号

@param       none 

@retval      int

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenCommPort()
{
	return m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect1BaudRate

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect1MaxConnReceive

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenMaxConnReceive()
{
	return m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"), _T("MAX_CONN_RECEIVE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect1MaxConnSend

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenMaxConnSend()
{
	return m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"), _T("MAX_CONN_SEND"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenMonitorInterval()
{
	return (m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"),_T("MONITOR_INTERVAL"),5000) );
}

//////////////////////////////////////////////////////////////////////////
/*
@brief       连续发票测试张数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenTestRelease()
{
	return (m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"),_T("RELEASE_TEST_COUNT"),10) );
}

//////////////////////////////////////////////////////////////////////////
/*
@brief        连续回收测试张数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenTestRecycle()
{
	return (m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"),_T("RECYCLE_TEST_COUNT"),10) );
}

//////////////////////////////////////////////////////////////////////////
/*
@brief        获取票箱内卡介质类型

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenBoxMediumType()
{
	return (m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"),_T("BOX_MEDIUM_TYPE"),3));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取配置文件中Token箱A最大库存容量

@param      无

@retval     int 库存容量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenBoxAMaxCount(){
	return (m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"),_T("TOKEN_BOX1_MAX_COUNT"),1000));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取配置文件中Token箱B最大库存容量

@param      无

@retval     int 库存容量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenBoxBMaxCount(){
	return (m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"),_T("TOKEN_BOX2_MAX_COUNT"),1000));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取配置文件中Hopper最大库存容量

@param      无

@retval     int Hopper库存容量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenHopperAMaxCount(){
	return (m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"),_T("TOKEN_HOPPER1_MAX_COUNT"),300));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取配置文件中Hopper最大库存容量

@param      无

@retval     int Hopper库存容量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenHopperBMaxCount(){
	return (m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"),_T("TOKEN_HOPPER2_MAX_COUNT"),300));
}


//***********************CH-ini接口*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使CH

@param       

@retval      bool true:使用CH  false:不使用CH

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsCHUsed()
{
	return (m_DeviceIni->GetDataInt(_T("COIN_MODULE"), _T("IS_USED")) == 1);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取CH串口号

@param       none 

@retval      int

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHCommPort()
{
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取CH波特率

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      CH监视开关

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsCHMonitorOpened()
{
	return (m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      CH监控时间间隔(单位:毫秒)

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHMonitorInterval()
{
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("MONITOR_INTERVAL"),5000);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取Hopper箱1硬币存放类型（只能为一元）

@param      无

@retval     int 循环找零箱B币种（0：无效，1：5角，2：1元（只能是一元，硬件规约））

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper1Type(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER1_MONEY_TYPE"),2);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取Hopper箱1硬币存放最大枚数

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper1MaxCount(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER1_MAX_COUNT"),1000);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取Hopper1将空枚数

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper1NearEmpty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER1_NEAR_EMPTY"),10);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取Hopper1已空枚数

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper1Empty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER1_EMPTY"),0);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取Hopper2硬币类型（五角）

@param      无

@retval     int 类型种类代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper2Type(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER2_MONEY_TYPE"),1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取Hopper2最大枚数

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper2MaxCount(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER2_MAX_COUNT"),1000);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取Hopper2将空值

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper2NearEmpty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER2_NEAR_EMPTY"),10);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取Hopper2已空枚数

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper2Empty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER2_EMPTY"),0);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取暂存1最大可投入数量

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHTemporary1Max(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("TEMPORARY1_MAX"),35);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取暂存2最大可投入数量

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHTemporary2Max(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("TEMPORARY2_MAX"),35);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取找零限制最大一元出币枚数

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetLimitOneCount(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("LIMIT_ONE_RMB_COUNT"),20);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取找零限制最大五角出币枚数

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetLimitHalfCount(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("LIMIT_HALF_RMB_OUNTT"),20);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      找零箱A存放硬币类型（左边）

@param      无

@retval     int 硬币类型代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxAType(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX1_MONEY_TYPE"),2);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      找零箱A最大存放硬币枚数

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxAMax(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX1_MAX"),200);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      找零箱A将空枚数

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxANearEmpty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX1_NEAR_EMPTY"),10);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      找零箱A已空枚数

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxAEmpty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX1_EMPTY"),0);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      找零箱B存放硬币类型（右边）

@param      无

@retval     int 硬币类型代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxBType(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX2_MONEY_TYPE"),2);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      找零箱B最大存放数量

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxBMax(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX2_MAX"),200);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      找零箱B将空枚数

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxBNearEmpty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX2_NEAR_EMPRY"),10);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      找零箱B已空枚数

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxBEmpty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX2_EMPTY"),0);
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      CH 循环找零箱币种(0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP)
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHStockMoneyType()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("STOCK_MONEY_TYPE"),1);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      循环找零箱最大枚数
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHStock1Max()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("STOCK1_MAX"),100);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      暂存器1最大枚数
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHTemporary1Max()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("TEMPORARY1_MAX"),20);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      暂存器2最大枚数
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHTemporary2Max()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("TEMPORARY2_MAX"),20);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      循环找零箱空枚数
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHStockEmpty()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("STOCK_EMPTY"));
//}
//
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      循环找零箱将空枚数
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHStockNearEmpty()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("STOCK_NEAR_EMPTY"),5);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      加币箱1币种(0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP)
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHHopper1Type()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER1_MONEY_TYPE"),1);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      HOPPER1最大枚数
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHHopper1Max()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER1_MAX"),700);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      HOPPER1一次补币最大枚数
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHHopper1AddMax()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER1_ADD_MAX"),500);
//}
//
//
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief       HOPPER1已空枚数
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHHopper1Empty()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER1_EMPTY"));
//}
//
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief       HOPPER1将空枚数
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHHopper1NearEmpty()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER1_NEAR_EMPTY"),5);
//}
//
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      加币箱2币种(0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP)
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHHopper2Type()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER2_MONEY_TYPE"),1);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      HOPPER1最大枚数
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHHopper2Max()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER2_MAX"),700);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      HOPPER2一次补币最大枚数
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHHopper2AddMax()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER2_ADD_MAX"),500);
//}
//
//
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief       HOPPER1已空枚数
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHHopper2Empty()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER2_EMPTY"));
//}
//
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief       HOPPER1将空枚数
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCHHopper2NearEmpty()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER1_NEAR_EMPTY"),5);
//}
//
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief       获取硬币专用找零箱A硬币回收优先级顺序
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCollectCoinPriorityforHopper1()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("COLLECT_COIN_PRIORITY_HOPPER1"),2);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief       获取硬币专用找零箱A硬币回收进入硬币回收箱编号
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCollectDirectionforHopper1()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("COLLECT_COIN_DIRECTION_HOPPER1"),2);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief       获取硬币专用找零箱B硬币回收优先级顺序
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCollectCoinPriorityforHopper2()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("COLLECT_COIN_PRIORITY_HOPPER2"),3);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief       获取硬币专用找零箱B硬币回收进入硬币回收箱编号
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCollectDirectionforHopper2()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("COLLECT_COIN_DIRECTION_HOPPER2"),2);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief       获取硬币循环找零箱硬币回收优先级顺序
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCollectCoinPriorityforStock()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("COLLECT_COIN_PRIORITY_STOCK"),1);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief       获取硬币循环找零箱硬币回收进入硬币回收箱编号
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//int CTVMInfo::GetCollectDirectionforStock()
//{
//	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("COLLECT_COIN_DIRECTION_STOCK"),1);
//}

//***********************CH-CoinShutter-ini接口*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用CH-CoinShutter

@param       

@retval      bool true:使用读写器  false:不使用

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsCoinShutterUsed()
{
	return (m_DeviceIni->GetDataInt(_T("COIN_SLOT_COMMUNICATE"), _T("IS_USED")) == 1);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取CoinShutter串口号

@param       none 

@retval      int

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCoinShutterCommPort()
{
	return m_DeviceIni->GetDataInt(_T("COIN_SLOT_COMMUNICATE"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取CoinShutter波特率

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCoinShutterBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("COIN_SLOT_COMMUNICATE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      CoinShutter监视开关

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsCoinShutterMonitorOpened()
{
	return (m_DeviceIni->GetDataInt(_T("COIN_SLOT_COMMUNICATE"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      CoinShutter监控时间间隔(单位:毫秒)

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCoinShutterMonitorInterval()
{
	return m_DeviceIni->GetDataInt(_T("COIN_SLOT_COMMUNICATE"),_T("MONITOR_INTERVAL"),5000);
}

//***********************BH-ini接口*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用BH

@param       

@retval      bool true:使用读写器  false:不使用

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsBHUsed()
{
	return (m_DeviceIni->GetDataInt(_T("BANKNOTE_ACCEPT_MODULE"), _T("IS_USED")) == 1);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取BH串口号

@param       none 

@retval      int

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBHCommPort()
{
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_ACCEPT_MODULE"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取BH波特率

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBHBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_ACCEPT_MODULE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      BH监视开关

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsBHMonitorOpened()
{
	return (m_DeviceIni->GetDataInt(_T("BANKNOTE_ACCEPT_MODULE"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      BH监控时间间隔(单位:毫秒)

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBHMonitorInterval()
{
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_ACCEPT_MODULE"),_T("MONITOR_INTERVAL"),5000);
}

bool CTVMInfo::IsZLGRfidUsed()
{
	return (m_DeviceIni->GetDataInt(_T("ZLGRFID_MODULE"), _T("IS_USED")) == 1);
}

int CTVMInfo::GetZLGRfidCommPort()
{
	return (m_DeviceIni->GetDataInt(_T("ZLGRFID_MODULE"), _T("COMM_PORT")) == 1);
}

int CTVMInfo::GetZLGRfidBaudRate()
{
	return (m_DeviceIni->GetDataInt(_T("ZLGRFID_MODULE"), _T("BAUD_RATE")) == 1);
}

bool CTVMInfo::IsZLGRfidMonitorOpened()
{
	return (m_DeviceIni->GetDataInt(_T("ZLGRFID_MODULE"), _T("MONITOR_SWITCH")) == 1);
}

int CTVMInfo::GetZLGRfidMonitorInterval()
{
	return (m_DeviceIni->GetDataInt(_T("ZLGRFID_MODULE"), _T("MONITOR_INTERVAL")) == 1);
}

//****************************读写器通讯参数**********************************************
//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用读写器

@param     (i)RW_TYPE rwType 读写器类型

@retval      bool true:使用读写器  false:使用模拟数据

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsRWUsed(RW_TYPE rwType)
{
	return (m_DeviceIni->GetDataInt(GetRWTypeString(rwType), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief       监视开关（1:开  0:关）

@param     (i)RW_TYPE rwType 读写器类型    

@retval     bool true:开  false:关

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool	CTVMInfo::IsRWMonitorOpened(RW_TYPE rwType)
{
	return m_DeviceIni->GetDataInt(GetRWTypeString(rwType), _T("MONITOR_SWITCH"),0)!=0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      监控时间间隔(单位:毫秒)

@param     (i)RW_TYPE rwType 读写器类型   

@retval     int 时间间隔(单位：毫秒) 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetRWMonitorInterval(RW_TYPE rwType)
{
	return m_DeviceIni->GetDataInt(GetRWTypeString(rwType), _T("MONITOR_INTERVAL"),30);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取读写器串口号

@param     (i)RW_TYPE rwType 读写器类型

@retval      int 串口号

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetRWCommPort(RW_TYPE rwType)
{
	return m_DeviceIni->GetDataInt(GetRWTypeString(rwType), _T("COM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取读写器串口波特率

@param     (i)RW_TYPE rwType 读写器类型

@retval      int 串口波特率

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetRWBaudRate(RW_TYPE rwType)
{
	return m_DeviceIni->GetDataInt(GetRWTypeString(rwType), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			获取数据下载重试次数

@param		(i)RW_TYPE rwType 读写器类型

@retval			int  下载重试次数

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetRWDownLoadCnt(RW_TYPE rwType)
{
	return m_DeviceIni->GetDataInt(GetRWTypeString(rwType), _T("FIRMWARE_DOWNLOAD_CNT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       读卡超时时间（单位:秒）-预留

@param     (i)RW_TYPE rwType 读写器类型

@retval      int 超时时间（单位:秒）

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetRWTimeout(RW_TYPE rwType)
{
	return m_DeviceIni->GetDataInt(GetRWTypeString(rwType), _T("READ_TIMEOUT"));
}

//***********************SDSP-ini接口*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用SDSP

@param       

@retval      bool true:使用读写器  false:不使用

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsSdspUsed()
{
	return (m_DeviceIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"), _T("IS_USED")) == 1);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Sdsp串口号

@param       none 

@retval      int

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetSdspCommPort()
{
	return m_DeviceIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Sdsp波特率

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetSdspBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Sdsp监视开关

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsSdspMonitorOpened()
{
	return (m_DeviceIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Sdsp监控时间间隔(单位:毫秒)

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetSdspMonitorInterval()
{
	return m_DeviceIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"),_T("MONITOR_INTERVAL"),5000);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Sdsp亮度参数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetSdspMonitorBrightness()
{
	return m_DeviceIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"),_T("BRIGHTNESS"),0);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Sdsp移动速度参数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetSdspMonitorMoveSpeed()
{
	return m_DeviceIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"),_T("MOVE_SPEED"),5);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Sdsp停留风格

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetSdspMonitorStayStyle()
{
	return m_DeviceIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"),_T("STAY_STYLE"),5);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Sdsp停留时间

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetSdspMonitorStayTime()
{
	return m_DeviceIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"),_T("STAY_TIME_LEN"),5);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Sdsp移入方式

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetSdspMonitorMoveIn()
{
	return m_DeviceIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"),_T("MOVE_IN"),4);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Sdsp移出方式

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetSdspMonitorMoveOut()
{
	return m_DeviceIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"),_T("MOVE_OUT"),4);
}

//***********************PIN-PAD-ini接口*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用PIN-PAD

@param       

@retval      bool true:使用读写器  false:不使用

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsPinPadUsed()
{
	return (m_DeviceIni->GetDataInt(_T("PIN_PAD_COMMUNICATE"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取PIN-PAD串口号

@param       none 

@retval      int

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetPinPadCommPort()
{
	return m_DeviceIni->GetDataInt(_T("PIN_PAD_COMMUNICATE"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Pin_Pad波特率

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetPinPadBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("PIN_PAD_COMMUNICATE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Pin_Pad监视开关

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsPinPadMonitorOpened()
{
	return (m_DeviceIni->GetDataInt(_T("PIN_PAD_COMMUNICATE"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Pin_Pad监控时间间隔(单位:毫秒)

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetPinPadMonitorInterval()
{
	return m_DeviceIni->GetDataInt(_T("PIN_PAD_COMMUNICATE"),_T("MONITOR_INTERVAL"),5000);
}

//***********************BankCard-ini接口*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用BankCard

@param       

@retval      bool true:使用读写器  false:不使用

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsBankCardUsed()
{
	return (m_DeviceIni->GetDataInt(_T("BANK_CARD_COMMUNICATE"), _T("IS_USED")) == 1);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取PIN-PAD串口号

@param       none 

@retval      int

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBankCardCommPort()
{
	return m_DeviceIni->GetDataInt(_T("BANK_CARD_COMMUNICATE"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Pin_Pad波特率

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBankCardBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("BANK_CARD_COMMUNICATE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      BankCard监视开关

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsBankCardMonitorOpened()
{
	return (m_DeviceIni->GetDataInt(_T("BANK_CARD_COMMUNICATE"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      BankCard监控时间间隔(单位:毫秒)

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBankCardMonitorInterval()
{
	return m_DeviceIni->GetDataInt(_T("BANK_CARD_COMMUNICATE"),_T("MONITOR_INTERVAL"),5000);
}

//***********************打印机接口*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用打印机

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsPrinterUsed()
{
	return (m_DeviceIni->GetDataInt(_T("RP_COMMUNICATE"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect3CommPort

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetPrinterCommPort()
{
    return m_DeviceIni->GetDataInt(_T("RP_COMMUNICATE"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect3BaudRate

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetPrinterBaudRate()
{
   return m_DeviceIni->GetDataInt(_T("RP_COMMUNICATE"), _T("BAUD_RATE"));
}
int	CTVMInfo::GetPrinterMonitorInternal()
{
	return m_DeviceIni->GetDataInt(_T("RP_COMMUNICATE"), _T("MONITOR_INTERVAL"));
}
bool CTVMInfo::IsPrinterMonitorOpened()
{
	return (m_DeviceIni->GetDataInt(_T("RP_COMMUNICATE"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsIOUsed()
{
		return (m_DeviceIni->GetDataInt(_T("IO_COMMUNICATE"),_T("IS_USED"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief				获取IO串口号

@param			none

@retval				int 串口号

@exception		none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetIOCommPort()
{
	return m_DeviceIni->GetDataInt(_T("IO_COMMUNICATE"),_T("COMM_PORT"),8);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief				获取IO串口波特率

@param			none

@retval				int 波特率

@exception		none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetIOBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("IO_COMMUNICATE"),_T("BAUD_RATE"),9600);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		判断IO门禁监控是否开启

@param      none

@retval     bool true:开启  false:关闭

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsIOMonitorOpened()
{
	return (m_DeviceIni->GetDataInt(_T("IO_COMMUNICATE"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetIOMonitorInterval()
{
	return m_DeviceIni->GetDataInt(_T("IO_COMMUNICATE"),_T("MONITOR_INTERVAL"),1000);
}
//***********************BDH-ini接口（预留）*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用BDH（预留）

@param       

@retval      bool true:使用读写器  false:不使用

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsBDHUsed()
{
	return (m_DeviceIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"), _T("IS_USED")) == 1);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取BDH串口号（预留）

@param       none 

@retval      int

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBDHCommPort()
{
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取BDH波特率（预留）

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBDHBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      BH监视开关（预留）

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsBDHMonitorOpened()
{
	return (m_DeviceIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      BDH监控时间间隔(单位:毫秒)（预留）

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBDHMonitorInterval()
{
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"),_T("MONITOR_INTERVAL"),5000);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取纸币找零箱A内纸币面额

@param      无

@retval     int 纸币找零箱纸币面额（1：1元，2：5元，3：10元，4：20元，5：50元，6：100元（不可能这么大，但是预留））

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBHChangeBoxAType(){
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"),_T("CHANGE_BOXA_TYPE"),1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取纸币找零箱B内纸币面额

@param      无

@retval     int 纸币找零箱纸币面额（1：1元，2：5元，3：10元，4：20元，5：50元，6：100元（不可能这么大，但是预留））

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBHChangeBoxBType(){
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"),_T("CHANGE_BOXB_TYPE"),1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取纸币找零箱废币箱将满值

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBHChangeBoxNearFull(){
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"),_T("INVALID_BOX_NEAR_FULL"),150);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取纸币找零废币箱已满值

@param      无

@retval     int 数量

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBHChangeBoxFull(){
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"),_T("INVALID_BOX_FULL"),200);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取MTC前两字节编码

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTVMInfo::GetDeviceComponentCode (AFC_DEVICE_TYPE deviceType,DEVICE_SUBTYPE deviceSubType,long moudleID)
{
	CString strMouldeID;
	strMouldeID.Format(_T("%d"),moudleID);
	CString deviceComponentCode = _T("");
	if (deviceType == DEVICE_TYPE_TVM){
		deviceComponentCode = m_DeviceIni->GetDataString(_T("TVM_COMPONENT_ID"),strMouldeID,_T(""));
	}
	else if (deviceType == DEVICE_TYPE_AVM){
		deviceComponentCode = m_DeviceIni->GetDataString(_T("AVM_COMPONENT_ID"),strMouldeID,_T(""));
	}
	return deviceComponentCode;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      获取Log存放位置

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTVMInfo::GetLogPath()
{
	if(m_strLogPath.IsEmpty()){
		m_strLogPath = m_DeviceIni->GetDataString(_T("APP_PATH"),_T("LogPath"),_T("E:\\Log\\"));
	}
	return m_strLogPath;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取data存放位置

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTVMInfo::GetDataPath()
{
	if(m_strDataPath.IsEmpty()){
		m_strDataPath = m_DeviceIni->GetDataString(_T("APP_PATH"),_T("DataPath"),_T("D:\\TVM_Data\\"));
	}
	return m_strDataPath;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取Backup存放位置

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTVMInfo::GetBackupPath()
{
	if(m_strDataBackpath.IsEmpty()){
		m_strDataBackpath = m_DeviceIni->GetDataString(_T("APP_PATH"),_T("BackupPath"),_T("E:\\TVM_Backup\\"));
	}
	return m_strDataBackpath;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置充值读写器是否启用

@param      (o)const CString& str

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMInfo::SetRechargeIsRWUsed(const CString& str)
{
	m_DeviceIni->WriteData(_T("TVM_RECHARGE_RW"),_T("IS_USED"),str);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置Smart-Card读写器是否启用

@param      (o)const CString& str

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMInfo::SetSmartCardIsRWUsed(const CString& str)
{
	m_DeviceIni->WriteData(_T("TVM_CARD_RW"),_T("IS_USED"),str);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置Token读写器是否启用

@param      (o)const CString& str

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMInfo::SetTokenIsRWUsed(const CString& str)
{
	m_DeviceIni->WriteData(_T("TVM_TOKEN_RW"),_T("IS_USED"),str);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币处理模块是否启用

@param      (o)const CString& str

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMInfo::SetBankNoteIsUsed(const CString& str)
{
	m_DeviceIni->WriteData(_T("BANKNOTE_ACCEPT_MODULE"),_T("IS_USED"),str);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币找零模块是否启用

@param      (o)const CString& str

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMInfo::SetBankNoteChangeIsUsed(const CString& str)
{
	m_DeviceIni->WriteData(_T("BANKNOTE_CHANGE_MODULE"),_T("IS_USED"),str);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置Smart-Card模块是否启用

@param      (o)const CString& str

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMInfo::SetSmartCardIsUsed(const CString& str)
{
	m_DeviceIni->WriteData(_T("CARD_MODULE"),_T("IS_USED"),str);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置Token模块是否启用

@param      (o)const CString& str

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMInfo::SetTokenIsUsed(const CString& str)
{
	m_DeviceIni->WriteData(_T("TOKEN_MODULE"),_T("IS_USED"),str);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置硬币处理模块是否启用

@param      (o)const CString& str

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMInfo::SetCoinIsUsed(const CString& str)
{
	m_DeviceIni->WriteData(_T("COIN_MODULE"),_T("IS_USED"),str);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置凭证打印机是否启用

@param      (o)const CString& str

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMInfo::SetReceiptIsUsed(const CString& str)
{
	m_DeviceIni->WriteData(_T("RP_COMMUNICATE"),_T("IS_USED"),str);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置密码键盘是否启用

@param      (o)const CString& str

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMInfo::SetPinPadIsUsed(const CString& str)
{
	m_DeviceIni->WriteData(_T("PIN_PAD_COMMUNICATE"),_T("IS_USED"),str);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置银行卡模块是否启用

@param      (o)const CString& str

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMInfo::SetBankCardIsUsed(const CString& str)
{
	m_DeviceIni->WriteData(_T("BANK_CARD_COMMUNICATE"),_T("IS_USED"),str);
}


int CTVMInfo::GetParameterUpdateBeginHour()const
{
	return m_DeviceIni->GetDataInt(_T("PARAMETER_UPDATE"),_T("Begin_hour"),8);
}

int CTVMInfo::GetParameterUpdateBeginMin()const
{
	return m_DeviceIni->GetDataInt(_T("PARAMETER_UPDATE"),_T("Begin_min"),0);
}

int CTVMInfo::GetParameterUpdateEndHour()const
{
	return m_DeviceIni->GetDataInt(_T("PARAMETER_UPDATE"),_T("End_hour"),23);
}

int CTVMInfo::GetParameterUpdateEndMin()const
{
	return m_DeviceIni->GetDataInt(_T("PARAMETER_UPDATE"),_T("End_min"),0);
}
