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

 @param     BYTE opr: ��������
			BOOL isEnhanced �� �Ƿ���Enhanced�汾 Ĭ��ΪTrue(��)
 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeBillTransportation::CBHChangeBillTransportation()
{	
	
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeBillTransportation::~CBHChangeBillTransportation()
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
long CBHChangeBillTransportation::ExecuteCommand()
{
	long lRet = 1;
	lRet = BillTransportationFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		bill transportation (�ο�����:��Ʊ����)

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeBillTransportation::BillTransportationFunction()
{
	long lRet = 0;

	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHChangeException::ERROR_BH_NOT_OPEN;
	}
	// ��֯����
	BYTE SendBuff[5];
	memset(SendBuff,0x00,sizeof(SendBuff));

	int index = 0;
	SendBuff[index++] = BH_CTRL_REQUEST;			// DH0
	SendBuff[index++] = BH_CHANGE_TRANSPORTATION;	// DH1
	SendBuff[index++] = 0x01;						// DH2
	SendBuff[index++] = 0xF0;	// OPR  b7~b0
								// 0xF0 1111 0000
														
	SendBuff[index++] = BH_CTRL_END_OF_COMMOND;		// FS


	//��������
	
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand(sizeof(SendBuff) , SendBuff, RecvBuffer, RecvLen);
	if (result != BHTRS_RES_OK){
		return CBHChangeException::ERROR_BH_SEND;
	}

	if(RecvLen > 0){
		if(RecvBuffer[0] == BH_CTRL_ERR_RESPONSE){
			lRet = RecvBuffer[3];
		}

		// �����������
		//m_changeDetail.Deserialize(RecvBuffer);
	}
	else
		lRet = CBHChangeException::ERROR_BH_RECEIVE;


	return 0;
}
