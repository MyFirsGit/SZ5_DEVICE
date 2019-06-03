#pragma once
#include "afx.h"
#include "SysException.h"
#include "TOKENLIBExp.h"
/**
@brief	�ⲿ��д��Ӳ���쳣��
*/

class TOKENCOMLIB_DECLSPEC CTOKENHardwareException :
    public CSysException
{
public:
    static const int MODULE_ID = 12501;     // ģ���

    CTOKENHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CTOKENHardwareException(void);

	typedef enum _detail_error_code {
		TOKEN_ERR_OK						= 0x00,	// ����ִ����ȷ
		TOKEN_ERR_NO_TOKEN_AT_READ_AREA		= 0x01,	// ��token�ڶ���λ�þ���
		TOKEN_ERR_HAS_TOKEN_AT_READ_AREA	= 0x03,	// �б��ڶ���λ�þ���
		TOKEN_ERR_INVALID_PARAMETER			= 0x31,	// ����
		TOKEN_ERR_BOX_A_EMPTY				= 0x3c,	// AƱ��Ϊ��
		TOKEN_ERR_BOX_B_EMPTY				= 0x3d,	// BƱ��Ϊ��
		TOKEN_ERR_JAM_AT_OUTLET				= 0x40,	// ��token�ڳ���λ�ö���
		TOKEN_ERR_BOX_A_CARDOUT				= 0x42,	// Ʊ��A����Ʊ��
		TOKEN_ERR_BOX_B_CARDOUT				= 0x43,	// Ʊ��B����Ʊ��
		TOKEN_ERR_DEVICE_BUSY				= 0x4a,	// �豸æ
		TOKEN_ERR_CLEAN_FAILED				= 0x4b,	// ���ʧ��
		TOKEN_ERR_MOTOR_RUN					= 0x50,	// ���ת��ʧ��
		TOKEN_ERR_READER_SENSOR				= 0x61,	// ����������������
		TOKEN_ERR_OUTLET_SENSOR				= 0x62,	// ���ҿڴ���������
		TOKEN_ERR_RETRIEVE_SENSOR			= 0x63,	// ���տڴ���������
		TOKEN_ERR_MAGNET1					= 0x64,	// �����1����
		TOKEN_ERR_MAGNET2					= 0x65,	// �����2����
		TOKEN_ERR_HOPPER1					= 0x66,	// Hopper1���ڹ���
		TOKEN_ERR_HOPPER2					= 0x67,	// Hopper2���ڹ���
		TOKEN_ERR_MAGNET3					= 0x68,	// �����3����
		TOKEN_ERR_MAGNET4					= 0x69,	// �����4����
		TOKEN_ERR_MAGNET_SENSOR				= 0x6a,	// �����λ����������
		TOKEN_ERR_HOPPER1_OUT_SENSOR		= 0x6b,	// Hopper1���Ҵ���������
		TOKEN_ERR_HOPPER2_OUT_SENSOR		= 0x6c,	// Hopper2���Ҵ���������
		TOKEN_ERR_HOPPER1_CLEAN_SENSOR		= 0x6d,	// Hopper1��Ҵ���������
		TOKEN_ERR_HOPPER2_CLEAN_SENSOR		= 0x6e,	// Hopper2��Ҵ���������

		TOKEN_ERR_RFID_CARD_NOT_EXIST		= 0xA1,	//�޷���⵽�����RFID��
		TOKEN_ERR_RFID_CARD_AUTHENT			= 0xA2,	// ����RFID����֤ʧ��
		TOKEN_ERR_RFID_CARD_OPERATOR_PARAMTER = 0xA3,// ��д����RFID����������

		TOKEN_ERR_COMM						= 0xFE,	// ���ڴ���
		TOKEN_ERR_ACK						= 0xFD,	// ACK����
		TOKEN_ERR_CMD						= 0xFC,	// Cmd����
	} DETAIL_ERROR_CODE;

private:
};
