#include "stdafx.h"
#include "StatusDisplayCommand.h"
#include "StatusDisplayException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief      构造函数
                                                                 
 @param      (i)无

 @retval      无

 @exception   无
*/
//////////////////////////////////////////////////////////////////////////

CStatusDisplayClose::CStatusDisplayClose(void){
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      析构函数
                                                                 
 @param      (i)无

 @retval      无

 @exception   无
*/
//////////////////////////////////////////////////////////////////////////

CStatusDisplayClose::~CStatusDisplayClose(void){
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      关闭串口功能实现
                                                                 
 @param      (i)HANDLE& hStatusDisplay 通信的设备句柄

 @retval      int \n
 0:成功 非0:失败

 @exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CStatusDisplayClose::close()
{
	COMM_RESULT nResult = RES_OK;
	if ( g_hStatusDisplay != NULL) {
		if (Uninitialize == NULL)
		{
			return CStatusDisplayException::ERROR_StatusDisplay_INVALID_DISHANDLE;
		}
		nResult = Uninitialize();
		g_hStatusDisplay = NULL;
		if (nResult != RES_OK) {
			nResult=CStatusDisplayException::ERROR_StatusDisplay_CLOSE;
		}
	}
	return nResult;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      执行命令
                                                                 
 @param      (i)无

 @retval      long   \n 
                  0 : 成功   非0 : 失败 

 @exception   
*/
//////////////////////////////////////////////////////////////////////////

long CStatusDisplayClose::ExecuteCommand(){
	long iRet = 1;
	try
	{
		iRet = close();
		if(iRet!=0)
		{
			throw CStatusDisplayException(CStatusDisplayException::DETAIL_ERROR_CODE::OTHER_ERR,
				_T(__FILE__),__LINE__);
		}else
		{
		}
	}
	catch(CSysException&e)
	{
	}

	this->OnComplete(iRet);
	return iRet;
}