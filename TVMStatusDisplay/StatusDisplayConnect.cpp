#include "stdafx.h"
#include "StatusDisplayCommand.h"
#include "StatusDisplayException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UD_SUCCESSED_RET(n)\
{\
	if(n==0)\
	{\
		return TRUE;\
	}else\
	{\
		return FALSE;\
	}\
}\

//////////////////////////////////////////////////////////////////////////
/**
@brief    　构造函数

@param      (i)int nPort 串口的端口号
@param      (i)int nBaud 串口的波特率
@param      (i)int nSize 字长

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

CStatusDisplayConnect::CStatusDisplayConnect()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      (i)无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

CStatusDisplayConnect::~CStatusDisplayConnect(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      (i)无

@retval      long    \n
0:成功   非0 : 失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////

long CStatusDisplayConnect::ExecuteCommand(void){
	long iRet=1;
	iRet = Init();
	this->OnComplete(iRet);
	return iRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    　	打开串口功能实现

@retval     int
				0：成功 非0失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CStatusDisplayConnect::Init()
{
	int iRet = 0;
	long ret = 0;
	COMM_RESULT lRet = RES_OK;
	try{
		if(g_hStatusDisplay != 0){
			if (Uninitialize == NULL)
			{
				return CStatusDisplayException::ERROR_StatusDisplay_INVALID_DISHANDLE;
			}
			if (!Uninitialize()) {
				iRet = CStatusDisplayException::ERROR_StatusDisplay_CLOSE_COM;
				g_hStatusDisplay = NULL;
				return iRet;
			}
		}
		if (Initialize == NULL)
		{
			return CStatusDisplayException::ERROR_StatusDisplay_INVALID_DISHANDLE;
		}
		lRet = Initialize();

		if(lRet != RES_OK){
			iRet = CStatusDisplayException::ERROR_StatusDisplay_OPEN_COM;
			if(iRet!=0)
			{
				throw CStatusDisplayException(CStatusDisplayException::DETAIL_ERROR_CODE::OTHER_ERR,
					_T(__FILE__),__LINE__);
			}else
			{
			}
		}
		g_hStatusDisplay = 1;
	}
	catch(CSysException&e)
	{
	}


	return iRet;
}