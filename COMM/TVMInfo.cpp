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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTVMInfo::CTVMInfo():CSYSInfo()
{
    try {
        m_DeviceIni = NULL;
		
        // ���ڻ�
        Initialize();

		// ��ʼ��
		m_strDataPath = _T("");
		m_strDataBackpath = _T("");
		m_strLogPath = _T("");
    }
    catch (...) {
        // �����쳣��Ϣ
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
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
@brief       [����]->[�ȴ���]��ʱ��ʱ����

@param       

@retval     int(��)

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetFinishToReadyTimerInterval()
{
	 return m_DeviceIni->GetDataInt(_T("SERVICE_STATE_TIMER"), _T("FinishToReadyTimerInterval"));

}
//////////////////////////////////////////////////////////////////////////
/**
@brief       [������]->[�ȴ���]��ʱ��ʱ����

@param       

@retval     int(��)

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetReceptToReadyTimerInterval()
{

	 return m_DeviceIni->GetDataInt(_T("SERVICE_STATE_TIMER"), _T("ReceptToReadyTimerInterval"));

}
//////////////////////////////////////////////////////////////////////////
/**
@brief       [������]->[������]��ʱ��ʱ����

@param       

@retval     int(��)

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCountingToReturnTimerInterval()
{

	return m_DeviceIni->GetDataInt(_T("SERVICE_STATE_TIMER"), _T("CountingToReturnTimerInterval"));

}
//////////////////////////////////////////////////////////////////////////
/**
@brief        [�ȴ���]->[������]��ʱ��ʱ����

@param       

@retval     int(��)

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetReadyToRestTimerInterval()
{

	return m_DeviceIni->GetDataInt(_T("SERVICE_STATE_TIMER"), _T("ReadyToRestTimerInterval"));

}

//////////////////////////////////////////////////////////////////////////
/**
@brief        [��]->[��]����ָʾ�Ƽ�ʱ��

@param       

@retval     int(��)

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetReturnIndicatorTimerInterval()
{
	return m_DeviceIni->GetDataInt(_T("SERVICE_STATE_TIMER"), _T("ReturnIndicatorTimerInterval"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        [��]->[��]���������Ƽ�ʱ��

@param       

@retval     int(��)

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetReturnLightTimerInterval()
{
	return m_DeviceIni->GetDataInt(_T("SERVICE_STATE_TIMER"), _T("ReturnLightTimerInterval"));
}
//****************************THͨѶ����**********************************************

//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹTH

@param       

@retval      bool true:ʹ��TOKEN  false:��ʹ��TOKEN

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsTHUsed()
{
	return (m_DeviceIni->GetDataInt(_T("CARD_MODULE"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡTH���ں�

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
@brief       ��ȡTHͨ�ŷ�ʽ

@param       none 

@retval      int  0:���� 1:�ʲ� 2:�ܵ� 

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTHCommuMethod()
{
	return m_DeviceIni->GetDataInt(_T("TH"), _T("CMU_METHOD"));
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect1BaudRate

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTHBaudRate()
{
    return m_DeviceIni->GetDataInt(_T("CARD_MODULE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect1MaxConnReceive

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTHMaxConnReceive()
{
    return m_DeviceIni->GetDataInt(_T("CARD_MODULE"), _T("MAX_CONN_RECEIVE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect1MaxConnSend

@param       �� 

@retval      int

@exception   ��
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
@brief       ������Ʊ��������

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
@brief        �������ղ�������

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
@brief        ��ȡƱ���ڿ���������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTHBoxMediumType()
{
	return (m_DeviceIni->GetDataInt(_T("CARD_MODULE"),_T("BOX_MEDIUM_TYPE"),2));
}

//****************************TOKENͨѶ����**********************************************

//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹTOKEN

@param       

@retval      bool true:ʹ��TOKEN  false:��ʹ��TOKEN

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsTokenUsed()
{
	return (m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡTOKEN���ں�

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
@brief       ��ȡConnect1BaudRate

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect1MaxConnReceive

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenMaxConnReceive()
{
	return m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"), _T("MAX_CONN_RECEIVE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect1MaxConnSend

@param       �� 

@retval      int

@exception   ��
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
@brief       ������Ʊ��������

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
@brief        �������ղ�������

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
@brief        ��ȡƱ���ڿ���������

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
@brief      ȡ�����ļ���Token��A���������

@param      ��

@retval     int �������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenBoxAMaxCount(){
	return (m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"),_T("TOKEN_BOX1_MAX_COUNT"),1000));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�����ļ���Token��B���������

@param      ��

@retval     int �������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenBoxBMaxCount(){
	return (m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"),_T("TOKEN_BOX2_MAX_COUNT"),1000));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�����ļ���Hopper���������

@param      ��

@retval     int Hopper�������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenHopperAMaxCount(){
	return (m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"),_T("TOKEN_HOPPER1_MAX_COUNT"),300));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�����ļ���Hopper���������

@param      ��

@retval     int Hopper�������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetTokenHopperBMaxCount(){
	return (m_DeviceIni->GetDataInt(_T("TOKEN_MODULE"),_T("TOKEN_HOPPER2_MAX_COUNT"),300));
}


//***********************CH-ini�ӿ�*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹCH

@param       

@retval      bool true:ʹ��CH  false:��ʹ��CH

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsCHUsed()
{
	return (m_DeviceIni->GetDataInt(_T("COIN_MODULE"), _T("IS_USED")) == 1);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡCH���ں�

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
@brief       ��ȡCH������

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      CH���ӿ���

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
@brief      CH���ʱ����(��λ:����)

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
@brief      ȡHopper��1Ӳ�Ҵ�����ͣ�ֻ��ΪһԪ��

@param      ��

@retval     int ѭ��������B���֣�0����Ч��1��5�ǣ�2��1Ԫ��ֻ����һԪ��Ӳ����Լ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper1Type(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER1_MONEY_TYPE"),2);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡHopper��1Ӳ�Ҵ�����ö��

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper1MaxCount(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER1_MAX_COUNT"),1000);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡHopper1����ö��

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper1NearEmpty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER1_NEAR_EMPTY"),10);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡHopper1�ѿ�ö��

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper1Empty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER1_EMPTY"),0);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡHopper2Ӳ�����ͣ���ǣ�

@param      ��

@retval     int �����������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper2Type(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER2_MONEY_TYPE"),1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡHopper2���ö��

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper2MaxCount(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER2_MAX_COUNT"),1000);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡHopper2����ֵ

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper2NearEmpty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER2_NEAR_EMPTY"),10);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡHopper2�ѿ�ö��

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHHopper2Empty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("HOPPER2_EMPTY"),0);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�ݴ�1����Ͷ������

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHTemporary1Max(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("TEMPORARY1_MAX"),35);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�ݴ�2����Ͷ������

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHTemporary2Max(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("TEMPORARY2_MAX"),35);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�����������һԪ����ö��

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetLimitOneCount(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("LIMIT_ONE_RMB_COUNT"),20);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�������������ǳ���ö��

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetLimitHalfCount(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("LIMIT_HALF_RMB_OUNTT"),20);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������A���Ӳ�����ͣ���ߣ�

@param      ��

@retval     int Ӳ�����ʹ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxAType(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX1_MONEY_TYPE"),2);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������A�����Ӳ��ö��

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxAMax(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX1_MAX"),200);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������A����ö��

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxANearEmpty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX1_NEAR_EMPTY"),10);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������A�ѿ�ö��

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxAEmpty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX1_EMPTY"),0);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������B���Ӳ�����ͣ��ұߣ�

@param      ��

@retval     int Ӳ�����ʹ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxBType(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX2_MONEY_TYPE"),2);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������B���������

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxBMax(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX2_MAX"),200);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������B����ö��

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxBNearEmpty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX2_NEAR_EMPRY"),10);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������B�ѿ�ö��

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCHChangeBoxBEmpty(){
	return m_DeviceIni->GetDataInt(_T("COIN_MODULE"),_T("CHANGE_BOX2_EMPTY"),0);
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      CH ѭ�����������(0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP)
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
//@brief      ѭ�����������ö��
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
//@brief      �ݴ���1���ö��
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
//@brief      �ݴ���2���ö��
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
//@brief      ѭ���������ö��
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
//@brief      ѭ�������佫��ö��
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
//@brief      �ӱ���1����(0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP)
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
//@brief      HOPPER1���ö��
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
//@brief      HOPPER1һ�β������ö��
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
//@brief       HOPPER1�ѿ�ö��
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
//@brief       HOPPER1����ö��
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
//@brief      �ӱ���2����(0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP)
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
//@brief      HOPPER1���ö��
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
//@brief      HOPPER2һ�β������ö��
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
//@brief       HOPPER1�ѿ�ö��
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
//@brief       HOPPER1����ö��
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
//@brief       ��ȡӲ��ר��������AӲ�һ������ȼ�˳��
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
//@brief       ��ȡӲ��ר��������AӲ�һ��ս���Ӳ�һ�������
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
//@brief       ��ȡӲ��ר��������BӲ�һ������ȼ�˳��
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
//@brief       ��ȡӲ��ר��������BӲ�һ��ս���Ӳ�һ�������
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
//@brief       ��ȡӲ��ѭ��������Ӳ�һ������ȼ�˳��
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
//@brief       ��ȡӲ��ѭ��������Ӳ�һ��ս���Ӳ�һ�������
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

//***********************CH-CoinShutter-ini�ӿ�*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹ��CH-CoinShutter

@param       

@retval      bool true:ʹ�ö�д��  false:��ʹ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsCoinShutterUsed()
{
	return (m_DeviceIni->GetDataInt(_T("COIN_SLOT_COMMUNICATE"), _T("IS_USED")) == 1);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡCoinShutter���ں�

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
@brief       ��ȡCoinShutter������

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCoinShutterBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("COIN_SLOT_COMMUNICATE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      CoinShutter���ӿ���

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
@brief      CoinShutter���ʱ����(��λ:����)

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetCoinShutterMonitorInterval()
{
	return m_DeviceIni->GetDataInt(_T("COIN_SLOT_COMMUNICATE"),_T("MONITOR_INTERVAL"),5000);
}

//***********************BH-ini�ӿ�*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹ��BH

@param       

@retval      bool true:ʹ�ö�д��  false:��ʹ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsBHUsed()
{
	return (m_DeviceIni->GetDataInt(_T("BANKNOTE_ACCEPT_MODULE"), _T("IS_USED")) == 1);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡBH���ں�

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
@brief       ��ȡBH������

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBHBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_ACCEPT_MODULE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      BH���ӿ���

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
@brief      BH���ʱ����(��λ:����)

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

//****************************��д��ͨѶ����**********************************************
//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹ�ö�д��

@param     (i)RW_TYPE rwType ��д������

@retval      bool true:ʹ�ö�д��  false:ʹ��ģ������

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsRWUsed(RW_TYPE rwType)
{
	return (m_DeviceIni->GetDataInt(GetRWTypeString(rwType), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief       ���ӿ��أ�1:��  0:�أ�

@param     (i)RW_TYPE rwType ��д������    

@retval     bool true:��  false:��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool	CTVMInfo::IsRWMonitorOpened(RW_TYPE rwType)
{
	return m_DeviceIni->GetDataInt(GetRWTypeString(rwType), _T("MONITOR_SWITCH"),0)!=0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ʱ����(��λ:����)

@param     (i)RW_TYPE rwType ��д������   

@retval     int ʱ����(��λ������) 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetRWMonitorInterval(RW_TYPE rwType)
{
	return m_DeviceIni->GetDataInt(GetRWTypeString(rwType), _T("MONITOR_INTERVAL"),30);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ��д�����ں�

@param     (i)RW_TYPE rwType ��д������

@retval      int ���ں�

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetRWCommPort(RW_TYPE rwType)
{
	return m_DeviceIni->GetDataInt(GetRWTypeString(rwType), _T("COM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ��д�����ڲ�����

@param     (i)RW_TYPE rwType ��д������

@retval      int ���ڲ�����

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetRWBaudRate(RW_TYPE rwType)
{
	return m_DeviceIni->GetDataInt(GetRWTypeString(rwType), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			��ȡ�����������Դ���

@param		(i)RW_TYPE rwType ��д������

@retval			int  �������Դ���

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetRWDownLoadCnt(RW_TYPE rwType)
{
	return m_DeviceIni->GetDataInt(GetRWTypeString(rwType), _T("FIRMWARE_DOWNLOAD_CNT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ������ʱʱ�䣨��λ:�룩-Ԥ��

@param     (i)RW_TYPE rwType ��д������

@retval      int ��ʱʱ�䣨��λ:�룩

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetRWTimeout(RW_TYPE rwType)
{
	return m_DeviceIni->GetDataInt(GetRWTypeString(rwType), _T("READ_TIMEOUT"));
}

//***********************SDSP-ini�ӿ�*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹ��SDSP

@param       

@retval      bool true:ʹ�ö�д��  false:��ʹ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsSdspUsed()
{
	return (m_DeviceIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"), _T("IS_USED")) == 1);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡSdsp���ں�

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
@brief       ��ȡSdsp������

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetSdspBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Sdsp���ӿ���

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
@brief      Sdsp���ʱ����(��λ:����)

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
@brief      Sdsp���Ȳ���

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
@brief      Sdsp�ƶ��ٶȲ���

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
@brief      Sdspͣ�����

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
@brief      Sdspͣ��ʱ��

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
@brief      Sdsp���뷽ʽ

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
@brief      Sdsp�Ƴ���ʽ

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetSdspMonitorMoveOut()
{
	return m_DeviceIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"),_T("MOVE_OUT"),4);
}

//***********************PIN-PAD-ini�ӿ�*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹ��PIN-PAD

@param       

@retval      bool true:ʹ�ö�д��  false:��ʹ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsPinPadUsed()
{
	return (m_DeviceIni->GetDataInt(_T("PIN_PAD_COMMUNICATE"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡPIN-PAD���ں�

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
@brief       ��ȡPin_Pad������

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetPinPadBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("PIN_PAD_COMMUNICATE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Pin_Pad���ӿ���

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
@brief      Pin_Pad���ʱ����(��λ:����)

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetPinPadMonitorInterval()
{
	return m_DeviceIni->GetDataInt(_T("PIN_PAD_COMMUNICATE"),_T("MONITOR_INTERVAL"),5000);
}

//***********************BankCard-ini�ӿ�*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹ��BankCard

@param       

@retval      bool true:ʹ�ö�д��  false:��ʹ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsBankCardUsed()
{
	return (m_DeviceIni->GetDataInt(_T("BANK_CARD_COMMUNICATE"), _T("IS_USED")) == 1);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡPIN-PAD���ں�

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
@brief       ��ȡPin_Pad������

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBankCardBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("BANK_CARD_COMMUNICATE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      BankCard���ӿ���

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
@brief      BankCard���ʱ����(��λ:����)

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBankCardMonitorInterval()
{
	return m_DeviceIni->GetDataInt(_T("BANK_CARD_COMMUNICATE"),_T("MONITOR_INTERVAL"),5000);
}

//***********************��ӡ���ӿ�*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹ�ô�ӡ��

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsPrinterUsed()
{
	return (m_DeviceIni->GetDataInt(_T("RP_COMMUNICATE"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect3CommPort

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetPrinterCommPort()
{
    return m_DeviceIni->GetDataInt(_T("RP_COMMUNICATE"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect3BaudRate

@param       �� 

@retval      int

@exception   ��
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
@brief				��ȡIO���ں�

@param			none

@retval				int ���ں�

@exception		none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetIOCommPort()
{
	return m_DeviceIni->GetDataInt(_T("IO_COMMUNICATE"),_T("COMM_PORT"),8);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief				��ȡIO���ڲ�����

@param			none

@retval				int ������

@exception		none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetIOBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("IO_COMMUNICATE"),_T("BAUD_RATE"),9600);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		�ж�IO�Ž�����Ƿ���

@param      none

@retval     bool true:����  false:�ر�

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
//***********************BDH-ini�ӿڣ�Ԥ����*****************************************//
//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹ��BDH��Ԥ����

@param       

@retval      bool true:ʹ�ö�д��  false:��ʹ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMInfo::IsBDHUsed()
{
	return (m_DeviceIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"), _T("IS_USED")) == 1);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡBDH���ںţ�Ԥ����

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
@brief       ��ȡBDH�����ʣ�Ԥ����

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBDHBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      BH���ӿ��أ�Ԥ����

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
@brief      BDH���ʱ����(��λ:����)��Ԥ����

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
@brief      ȡֽ��������A��ֽ�����

@param      ��

@retval     int ֽ��������ֽ����1��1Ԫ��2��5Ԫ��3��10Ԫ��4��20Ԫ��5��50Ԫ��6��100Ԫ����������ô�󣬵���Ԥ������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBHChangeBoxAType(){
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"),_T("CHANGE_BOXA_TYPE"),1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡֽ��������B��ֽ�����

@param      ��

@retval     int ֽ��������ֽ����1��1Ԫ��2��5Ԫ��3��10Ԫ��4��20Ԫ��5��50Ԫ��6��100Ԫ����������ô�󣬵���Ԥ������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBHChangeBoxBType(){
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"),_T("CHANGE_BOXB_TYPE"),1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡֽ��������ϱ��佫��ֵ

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBHChangeBoxNearFull(){
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"),_T("INVALID_BOX_NEAR_FULL"),150);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡֽ������ϱ�������ֵ

@param      ��

@retval     int ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMInfo::GetBHChangeBoxFull(){
	return m_DeviceIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"),_T("INVALID_BOX_FULL"),200);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡMTCǰ���ֽڱ���

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
@brief      ��ȡLog���λ��

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
@brief      ��ȡdata���λ��

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
@brief      ��ȡBackup���λ��

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
@brief      ���ó�ֵ��д���Ƿ�����

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
@brief      ����Smart-Card��д���Ƿ�����

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
@brief      ����Token��д���Ƿ�����

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
@brief      ����ֽ�Ҵ���ģ���Ƿ�����

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
@brief      ����ֽ������ģ���Ƿ�����

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
@brief      ����Smart-Cardģ���Ƿ�����

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
@brief      ����Tokenģ���Ƿ�����

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
@brief      ����Ӳ�Ҵ���ģ���Ƿ�����

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
@brief      ����ƾ֤��ӡ���Ƿ�����

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
@brief      ������������Ƿ�����

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
@brief      �������п�ģ���Ƿ�����

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
