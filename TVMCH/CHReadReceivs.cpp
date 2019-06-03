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

extern bool isCHAborted; //�Ƿ������ѯͶ����Ϣ

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��-4.8.5��ȡ���ܽ��

@param      (i)��

@retval     ��  

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CCHReadReceivs::CCHReadReceivs(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)��

@retval     ��  

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CCHReadReceivs::~CCHReadReceivs(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      (i)��

@retval     long 0 : �ɹ�  ��0 : ʧ��

@exception   ��
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
@brief      ��ȡCH���ս��

@param      none

@retval     long   0 : �ɹ�  ��0 : ʧ��

@exception   ��
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
		// ��ȡʧ��
		if(result != CHTRS_RES_OK){
			break;
		}
		// ��ֹ
		else if(isCHAborted){
			result = CCHException::ERROR_CH_STOP_READ_AMOUNT;
			break;
		}
		// ��ȡ�ɹ�
		else{
			// �ж��Ƿ����µ�Ͷ��
			if(!Ch_Insert()){
				Sleep(100);
			}
			// ���µ�Ͷ��
			else{
				break;
			}
		}
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж�Ͷ����Ϣ�Ƿ����ı�

@param      none

@retval     bool   0 : ��  ��0 : ��

@exception   ��
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
@brief      ȡ��ִ�н��

@param      (o)CH_RSP_ACCEPT_INSERT_INFO* pResponse ��ȡ���ܽ��������Ӧ���ݽṹ

@retval     none

@exception   ��
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
		throw CCHException(CCHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}
