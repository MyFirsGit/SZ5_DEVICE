#include "stdafx.h"
#include "BHChangeCommand.h"
#include "BHChangeException.h"
#include "BHChangeHardwareException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CMD_COMMAND_LEN		0x15		// �����
//////////////////////////////////////////////////////////////////////////
/*
@brief      Ĭ�Ϲ��캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeSendOutBill::CBHChangeSendOutBill(int nCountBoxA,int nCountBoxB){
	this->m_nCountBoxA = nCountBoxA;
	this->m_nCountBoxB = nCountBoxB;
	memset(m_RecvBuff,0x00,sizeof(m_RecvBuff));
	m_RecvLen = 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeSendOutBill::~CBHChangeSendOutBill(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ִ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeSendOutBill::ExecuteCommand(){
	long lRet = BHTRS_RES_OK;
	lRet = SendOutBill();

	if(lRet == 0){
		lRet = BillTransportation();
	}
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���Ͳ�����ֽ���ͳ�����

@param      ��

@retval     long lRet;�������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeSendOutBill::SendOutBill(){
	long lRet = BHTRS_RES_OK;
	if(m_nCountBoxA < 0 || m_nCountBoxB < 0 || 20 < m_nCountBoxA || 20 < m_nCountBoxB){//���20��
		lRet = CBHChangeHardwareException::ERR_Para;// �������ô���
	}
	else{
		BYTE SendBuff[1 + 1 + 1+ CMD_COMMAND_LEN + 1];
		memset(SendBuff,0x00,sizeof(SendBuff));

		int index = 0;
		// ƴ�շ�������
		SendBuff[index++]	= BH_CTRL_REQUEST;				// DH0
		SendBuff[index++]	= BH_CHANGE_COUNT;				// DH1
		SendBuff[index++]	= CMD_COMMAND_LEN;				// DH2
		SendBuff[index++]	= 0xE4;	// ODR(BoxA and BoxB) 11 10 01 00
									// b7              b0
									// 1|1  1|0  0|1  0|0
									// 1st  2st  3st  4st
		// N1 ~ 4
		SendBuff[index++]	= TransforIntToHEXCode(m_nCountBoxA/10);
		SendBuff[index++]	= TransforIntToHEXCode(m_nCountBoxA%10);
		SendBuff[index++]	= TransforIntToHEXCode(m_nCountBoxB/10);
		SendBuff[index++]	= TransforIntToHEXCode(m_nCountBoxB%10);

		SendBuff[index++]	= TransforIntToHEXCode(0);
		SendBuff[index++]	= TransforIntToHEXCode(0);
		SendBuff[index++]	= TransforIntToHEXCode(0);
		SendBuff[index++]	= TransforIntToHEXCode(0);
		
		// R1 ~ 4 default 3
		int nDefault = 0;
		SendBuff[index++]	= TransforIntToHEXCode(0);
		SendBuff[index++]	= TransforIntToHEXCode(nDefault);
		SendBuff[index++]	= TransforIntToHEXCode(0);
		SendBuff[index++]	= TransforIntToHEXCode(nDefault);

		SendBuff[index++]	= TransforIntToHEXCode(0);
		SendBuff[index++]	= TransforIntToHEXCode(0);
		SendBuff[index++]	= TransforIntToHEXCode(0);
		SendBuff[index++]	= TransforIntToHEXCode(0);
		// P1 ~ 4 default 3
		SendBuff[index++]	= (nDefault & 0xFF);
		SendBuff[index++]	= (nDefault & 0xFF);

		index += 2;
		SendBuff[index]		= BH_CTRL_END_OF_COMMOND;

		lRet = SendOneCommand(sizeof(SendBuff),SendBuff,m_RecvBuff,m_RecvLen,20000);

		if (lRet != BHTRS_RES_OK){
			return CBHChangeException::ERROR_BH_SEND;
		}

		if(m_RecvLen > 0){
			if(m_RecvBuff[0] == BH_CTRL_ERR_RESPONSE){
				lRet = m_RecvBuff[3];
			}

			// �����������
			m_changeDetail.Deserialize(m_RecvBuff);
		}
		else
			lRet = CBHChangeException::ERROR_BH_RECEIVE;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      żУ��ת��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BYTE CBHChangeSendOutBill::TransforIntToHEXCode(int nCount){
	switch(nCount){
	case 0: return 0x30;
	case 1: return 0xB1;
	case 2: return 0xB2;
	case 3: return 0x33;
	case 4: return 0xB4;
	case 5: return 0x35;
	case 6: return 0x36;
	case 7: return 0xB7;
	case 8: return 0xB8;
	case 9: return 0x39;
	default:
		return 0x00;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�����ķ������

@param      PBANKNOTE_CHANGE_DETAIL rsp;(����ṹָ��)

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBHChangeSendOutBill::GetChangeResponse(PBANKNOTE_CHANGE_DETAIL rsp){
	try{
		if(rsp != NULL){
			memcpy(rsp,&m_changeDetail,sizeof(m_changeDetail));
		}
	}
	catch(CSysException& e){

	}
	catch(...){

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ֽ���ͳ�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeSendOutBill::BillTransportation(){

	CBHChangeBillTransportation transportion;
	long lRet = transportion.ExecuteCommand();

	if(lRet != 0){
		return lRet;
	}

	// �ٴ��ͳ�ȥһ��
	lRet = transportion.ExecuteCommand();
	if(lRet != 0){
		return lRet;
	}

	return 0;
}