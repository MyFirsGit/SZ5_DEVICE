#include "stdafx.h"
#include "BHChangeCommand.h"
#include "BHChangeException.h"
#include "BHChangeHardwareException.h"
//#include "bh_proto.h"
#define CMD_LEN_BASIC		0x0D	// 基本命令长度
#define CMD_LEN_ENHANCED	0x001A	// 高级命令长度

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief		构造函数

 @param		BYTE FrontShutterAction  前挡板动作 0：开/关 1：没有动作
			BYTE RearShutterAction   后挡板动作 0：开/关 1：没有动作
			ParamDevInit& ref  设备初始化参数
			BOOL isEnhanced ： 是否是Enhanced版本 默认为True(是)
 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeDevInit::CBHChangeDevInit(BYTE FrontShutterAction,BYTE RearShutterAction,ParamDevInit& ref,BOOL isEnhanced)
{	
	m_FontShutterAction = FrontShutterAction;
	m_RearShutterAction	= RearShutterAction;
	ptrParaDev			= &ref;
	m_isEnhanced		= isEnhanced;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeDevInit::~CBHChangeDevInit()
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
long CBHChangeDevInit::ExecuteCommand()
{
	long lRet = 1;
	lRet = DevInitFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		发送命令，并接受反馈数据

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeDevInit::DevInitFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHChangeException::ERROR_BH_NOT_OPEN;
	}
	
	// BASIC COMMAND 发送命令
	// 默认为基本箱子
	BYTE btSendBuff[1+ 1 + 1 + CMD_LEN_BASIC + 1];
	memset(btSendBuff,0x00,sizeof(btSendBuff));

	int index = 0;
	btSendBuff[index++]	= BH_CTRL_REQUEST;														// DH0
	btSendBuff[index++]	= BH_CHANGE_INIT;														// DH1 Command
	btSendBuff[index++]	= CMD_LEN_BASIC;														// DH2 DataLen
	btSendBuff[index++]	= (m_FontShutterAction & 0x01) | ((m_RearShutterAction & 0x01) << 1);	// ODR

	// 纸币长度及厚度设置
	for(int i = 0; i < 4; i++){
		btSendBuff[index++]		= ptrParaDev->box[i].max_Len;		// 长度最大范围
		btSendBuff[index++]		= ptrParaDev->box[i].min_Len;		// 长度最小范围
		btSendBuff[index+6-i]	= ptrParaDev->box[i].thickness;		// 厚度
	}
	index+= 4;
	btSendBuff[index]			= BH_CTRL_END_OF_COMMOND;			// FS

	// ENHANCED COMMAND (预留)
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand((DWORD)sizeof(btSendBuff), btSendBuff , m_RecvBuffer, m_RecvLen);
	if (result != BHTRS_RES_OK){
		return CBHChangeException::ERROR_BH_SEND;
	}

	if(m_RecvLen > 0){
		if(m_RecvBuffer[0] == BH_CTRL_ERR_RESPONSE){
			result = m_RecvBuffer[3];//CBHChangeException::ERROR_BH_DESERIALIZE;
		}
	}
	else
		result = CBHChangeException::ERROR_BH_RECEIVE;

	return result;
}
