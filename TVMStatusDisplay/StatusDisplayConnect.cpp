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
@brief    �����캯��

@param      (i)int nPort ���ڵĶ˿ں�
@param      (i)int nBaud ���ڵĲ�����
@param      (i)int nSize �ֳ�

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

CStatusDisplayConnect::CStatusDisplayConnect()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

CStatusDisplayConnect::~CStatusDisplayConnect(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      (i)��

@retval      long    \n
0:�ɹ�   ��0 : ʧ��

@exception   ��
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
@brief    ��	�򿪴��ڹ���ʵ��

@retval     int
				0���ɹ� ��0ʧ��

@exception  ��
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