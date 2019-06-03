#include "stdafx.h"
#include "CHCommand.h"
#include "CHException.h"
#include "DebugLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

unsigned int stock1_1y       = 0;
unsigned int stock2_05y      = 0;
unsigned int reject          = 0;

extern bool isCHAborted; //是否继续查询投入信息

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数-4.8.5读取接受金额

@param      (i)无

@retval     无  

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CCHReadReceivs::CCHReadReceivs(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      (i)无

@retval     无  

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CCHReadReceivs::~CCHReadReceivs(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      (i)无

@retval     long 0 : 成功  非0 : 失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
long CCHReadReceivs::ExecuteCommand(void)
{
	long iRet = 0;
	iRet = GetCH_Insert_Info();
	this->OnComplete(iRet);
	return iRet;
}

void CH_EnableInsert()
{
	isCHAborted=false;
}

void CH_DisableInsert()
{
	isCHAborted=true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取CH接收金额

@param      none

@retval     long   0 : 成功  非0 : 失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
long CCHReadReceivs::GetCH_Insert_Info()
{
	CHTRS_RESULT result = CHTRS_RES_OK;
	//BYTE iniCmd = CNTL_GET_INSERT_INFO; 
	
	//isCHAborted=false;
	while(true){
		
		if(isCHAborted){
			result = CCHException::ERROR_CH_STOP_READ_AMOUNT;
			break;
		}
		tCHADevReturn staus;
		result = CHA_GetReceiveNumber(&m_res,&staus);
		// 读取失败
		if(result != CHTRS_RES_OK){
			break;
		}
		// 中止
		else if(isCHAborted){
			result = CCHException::ERROR_CH_STOP_READ_AMOUNT;
			break;
		}
		// 读取成功
		else{
			// 判断是否有新的投入
			if(!Ch_Insert()){
				Sleep(100);
			}
			// 有新的投入
			else{
				break;
			}
		}
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断投入信息是否发生改变

@param      none

@retval     bool   0 : 是  非0 : 否

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CCHReadReceivs::Ch_Insert()
{
	bool ret = false;
	if(m_res.stCoinBoxValueInfo[0].uiCoinNum != stock1_1y){
		stock1_1y = m_res.stCoinBoxValueInfo[0].uiCoinNum;
		ret = true;
	}

	if(m_res.stCoinBoxValueInfo[1].uiCoinNum != stock2_05y){
		stock2_05y = m_res.stCoinBoxValueInfo[1].uiCoinNum;
		ret        = true;
	}
	if(m_res.uiEjectCoinNum != reject){
		reject = m_res.uiEjectCoinNum;
		ret    = true;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得执行结果

@param      (o)CH_RSP_ACCEPT_INSERT_INFO* pResponse 读取接受金额命令响应数据结构

@retval     none

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CCHReadReceivs::GetCHResponse(tCHACoinValueInfo* pResponse)
{
  try {
	  *pResponse = m_res;
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CCHException(CCHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}
