#include "stdafx.h"
#include "BHChangeCommand.h"
//#include "bh_proto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief		构造函数

 @param     BYTE opr: 操作类型
			BOOL isEnhanced ： 是否是Enhanced版本 默认为True(是)
 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeBillTransportation::CBHChangeBillTransportation()
{	
	
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeBillTransportation::~CBHChangeBillTransportation()
{

}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		执行命令

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeBillTransportation::ExecuteCommand()
{
	long lRet = 1;
	lRet = BillTransportationFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		bill transportation (参考翻译:钞票传输)

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeBillTransportation::BillTransportationFunction()
{
	long lRet = 0;

	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHChangeException::ERROR_BH_NOT_OPEN;
	}
	// 组织命令
	BYTE SendBuff[5];
	memset(SendBuff,0x00,sizeof(SendBuff));

	int index = 0;
	SendBuff[index++] = BH_CTRL_REQUEST;			// DH0
	SendBuff[index++] = BH_CHANGE_TRANSPORTATION;	// DH1
	SendBuff[index++] = 0x01;						// DH2
	SendBuff[index++] = 0xF0;	// OPR  b7~b0
								// 0xF0 1111 0000
														
	SendBuff[index++] = BH_CTRL_END_OF_COMMOND;		// FS


	//发送命令
	
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand(sizeof(SendBuff) , SendBuff, RecvBuffer, RecvLen);
	if (result != BHTRS_RES_OK){
		return CBHChangeException::ERROR_BH_SEND;
	}

	if(RecvLen > 0){
		if(RecvBuffer[0] == BH_CTRL_ERR_RESPONSE){
			lRet = RecvBuffer[3];
		}

		// 解析反馈结果
		//m_changeDetail.Deserialize(RecvBuffer);
	}
	else
		lRet = CBHChangeException::ERROR_BH_RECEIVE;


	return 0;
}
