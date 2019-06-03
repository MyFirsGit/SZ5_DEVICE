#include "stdafx.h"
#include "BHChangeCommand.h"
#include "BHChangeException.h"
#include "BHChangeHardwareException.h"
//#include "bh_proto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief		构造函数

 @param     ParamCountBill& val: 设置相应盒子的出钱张数，拒绝张数，重试张数
			BOOL isEnhanced ： 是否是Enhanced版本 默认为True(是)


 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeBillCount::CBHChangeBillCount(ParamCountBill& val,BOOL isEnhanced /* = TRUE */)
{	
	memset(RecvBuffer,0,sizeof(RecvBuffer));
	RecvLen = 0;
	m_isEnhanced = isEnhanced;
	if(!m_isEnhanced)
	{
		m_pCmd = new BYTE[cmd_basic];
		m_LenCmd = cmd_basic;
		memset(m_pCmd,0,cmd_basic*sizeof(BYTE));
	}
	else
		{
			m_pCmd = new BYTE[cmd_enhanced];
			m_LenCmd = cmd_enhanced;
			m_pCmd[0] = 0x60;
			m_pCmd[1] = 0x03;
			m_pCmd[2] = 0xFF;
			m_pCmd[3] = 0x00;
			m_pCmd[4] = 0x00;
			m_pCmd[5] = 0x2C;
			m_pCmd[6] = 0xFE;
			m_pCmd[7] = 0xDC;
			m_pCmd[8] = 0xBA;
			m_pCmd[9] = 0x98;
			BYTE *temp = NULL;
			int i;
			int offset = 10;
			for ( i = 0; i < 4; ++i)
			{
				temp = val.box[i].toconvert();
				m_pCmd[offset + 2*i]				 = temp[0];
				m_pCmd[offset + 2*i + 1]		     = temp[1];
				m_pCmd[offset + 2*i + 8]			 = temp[2];
				m_pCmd[offset + 2*i + 8 + 1]		 = temp[3];
				m_pCmd[offset + i + 16]			 = temp[4];
			}
			offset =  10 + i + 16;   //30
			for (i = 0;i < 4;++i)
			{
				temp = val.box[i+4].toconvert();
				m_pCmd[offset + 2*i]				 = temp[0];
				m_pCmd[offset + 2*i + 1]		     = temp[1];
				m_pCmd[offset + 2*i + 8]			 = temp[2];
				m_pCmd[offset + 2*i + 8 + 1]		 = temp[3];
				m_pCmd[offset + i + 16]			 = temp[4];
			}
			m_pCmd[cmd_enhanced - 1] = 0x1C;
		}
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeBillCount::~CBHChangeBillCount()
{
	delete [] m_pCmd;
	m_pCmd = NULL;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		执行命令

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeBillCount::ExecuteCommand()
{
	long lRet = 1;
	lRet = BillCountFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		bill count (参考翻译:钞票计数)

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeBillCount::BillCountFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHChangeException::ERROR_BH_NOT_OPEN;
	}
	
	//发送命令
	//BYTE recvbuf[RES_MAX] = { 0 }; 
	//DWORD recvlen = 0;
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand(m_LenCmd , m_pCmd, RecvBuffer, RecvLen);
	if (result != BHTRS_RES_OK){
		return CBHChangeException::ERROR_BH_SEND;
	}


	if (RecvLen > 0){
		if (0x00 != RecvBuffer[6]|| 0x00 != RecvBuffer[7])
		{
		throw CBHChangeException(RecvBuffer[6],_T(__FILE__),__LINE__);
		}
	}
	else{
		return CBHChangeException::ERROR_BH_RECEIVE;
	}


	return 0;
}
