#include "stdafx.h"
#include "BHChangeCommand.h"
#include "BHChangeException.h"
#include "BHChangeHardwareException.h"
//#include "bh_proto.h"
#define CMD_LEN_BASIC		0x0D	// ���������
#define CMD_LEN_ENHANCED	0x001A	// �߼������

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief		���캯��

 @param		BYTE FrontShutterAction  ǰ���嶯�� 0����/�� 1��û�ж���
			BYTE RearShutterAction   �󵲰嶯�� 0����/�� 1��û�ж���
			ParamDevInit& ref  �豸��ʼ������
			BOOL isEnhanced �� �Ƿ���Enhanced�汾 Ĭ��ΪTrue(��)
 @retval	��

 @exception	��
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
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeDevInit::~CBHChangeDevInit()
{
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		ִ������

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
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
 @brief		������������ܷ�������

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeDevInit::DevInitFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHChangeException::ERROR_BH_NOT_OPEN;
	}
	
	// BASIC COMMAND ��������
	// Ĭ��Ϊ��������
	BYTE btSendBuff[1+ 1 + 1 + CMD_LEN_BASIC + 1];
	memset(btSendBuff,0x00,sizeof(btSendBuff));

	int index = 0;
	btSendBuff[index++]	= BH_CTRL_REQUEST;														// DH0
	btSendBuff[index++]	= BH_CHANGE_INIT;														// DH1 Command
	btSendBuff[index++]	= CMD_LEN_BASIC;														// DH2 DataLen
	btSendBuff[index++]	= (m_FontShutterAction & 0x01) | ((m_RearShutterAction & 0x01) << 1);	// ODR

	// ֽ�ҳ��ȼ��������
	for(int i = 0; i < 4; i++){
		btSendBuff[index++]		= ptrParaDev->box[i].max_Len;		// �������Χ
		btSendBuff[index++]		= ptrParaDev->box[i].min_Len;		// ������С��Χ
		btSendBuff[index+6-i]	= ptrParaDev->box[i].thickness;		// ���
	}
	index+= 4;
	btSendBuff[index]			= BH_CTRL_END_OF_COMMOND;			// FS

	// ENHANCED COMMAND (Ԥ��)
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
