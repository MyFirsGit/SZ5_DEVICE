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
 @brief      ���캯��
                                                                 
 @param      (i)��

 @retval      ��

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////

CStatusDisplayClose::CStatusDisplayClose(void){
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ��������
                                                                 
 @param      (i)��

 @retval      ��

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////

CStatusDisplayClose::~CStatusDisplayClose(void){
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      �رմ��ڹ���ʵ��
                                                                 
 @param      (i)HANDLE& hStatusDisplay ͨ�ŵ��豸���

 @retval      int \n
 0:�ɹ� ��0:ʧ��

 @exception   ��
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
 @brief      ִ������
                                                                 
 @param      (i)��

 @retval      long   \n 
                  0 : �ɹ�   ��0 : ʧ�� 

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