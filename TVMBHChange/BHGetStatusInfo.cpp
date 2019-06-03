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
 @brief		���캯��

 @param    
 BOOL isEnhanced �� �Ƿ���Enhanced�汾 Ĭ��ΪTrue(��)
 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeGetStatusInfo::CBHChangeGetStatusInfo( /*BOOL isEnhanced*/ )
{	
	m_RecvLen = 0;
	memset(m_RecvBuffer,0x00,sizeof(m_RecvBuffer));

	/*memset(RecvBuffer,0,sizeof(RecvBuffer));
	RecvLen = 0;
	m_isEnhanced = isEnhanced;
	if(!m_isEnhanced)
	{
		m_pCmd = new BYTE[cmd_basic];
		m_LenCmd = cmd_basic;
		m_pCmd[0] = 0x00;
		m_pCmd[1] = 0x01;
		m_pCmd[cmd_basic - 1] = 0x1C;
	}
	else
		{
			m_pCmd = new BYTE[cmd_enhanced];
			m_LenCmd = cmd_enhanced;
			m_pCmd[0] = 0x60;
			m_pCmd[1] = 0x02;
			m_pCmd[2] = 0xFF;
			m_pCmd[3] = 0x00;
			m_pCmd[4] = 0x00;
			m_pCmd[5] = 0x01;
			m_pCmd[6] = 0x00;
			m_pCmd[7] = 0x1C;
		}*/
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeGetStatusInfo::~CBHChangeGetStatusInfo()
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
long CBHChangeGetStatusInfo::ExecuteCommand()
{
	long lRet = 1;
	lRet = GetStatusInfoFunction();
	this->OnComplete(lRet);
	return lRet;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ȡ����

 @param		ָ��洢�������ݵĽṹ�����

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
void CBHChangeGetStatusInfo::GetResponse(PDEVICE_STATUS_INFO response)
{
	try{
		*response = m_response;
	}
	catch(...){}
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ȡ�豸״̬��Ϣ

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeGetStatusInfo::GetStatusInfoFunction()
{
	long lRet = BHTRS_RES_OK;
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHChangeException::ERROR_BH_NOT_OPEN;
	}
	
	//��������
	BYTE SendBuff[3];
	memset(SendBuff,0x00,sizeof(SendBuff));

	int index = 0;
	SendBuff[index++]	= 0x00;
	SendBuff[index++]	= BH_CHANGE_GET_STATUS;
	SendBuff[index]		= BH_CTRL_END_OF_COMMOND;

	lRet = SendOneCommand(sizeof(SendBuff),SendBuff,m_RecvBuffer,m_RecvLen);

	if(lRet != BHTRS_RES_OK){
		return CBHChangeException::ERROR_BH_SEND;
	}

	if(m_RecvLen > 0){
		if(m_RecvLen > 0){
			//������
			if(m_RecvBuffer[0] == BH_CTRL_ERR_RESPONSE){
				lRet = m_RecvBuffer[3];
			}
			// ��ȷ����
			else if(m_RecvBuffer[0] == BH_CTRL_RESPONSE){
				m_response.Deserialize(m_RecvBuffer);
			}
		}
		else
			lRet = CBHChangeException::ERROR_BH_RECEIVE;
	}
	return 0;
}
