#pragma once
#include "ServiceHelper.h"
#include "StatusDisplayCommand.h"

#define theSTATUS_DISPLAYER CStatusDisplayModuleHelper::GetInstance()

/**
* ����LED��ʾ����
*/
typedef enum _em_show_detail_{
	MSG_IN_SERVICE = 0,
	MSG_ONLY_SALE,
	MSG_ONLY_EXCHANGE,
	MSG_ONLY_COIN,
	MSG_ONLY_BANKNOTE,
	MSG_ONLY_QR,
	MSG_ONLY_CASH,
	MSG_NO_COIN,
	MSG_NO_BANKNOTE,
	MSG_NO_CHANGE,
	MSG_OUT_OF_SVC,
	MSG_STOP_SVC,
	MSG_INTERNET_TICKETS,
	MSG_ONLY_RECHARGE,
	MSG_ONLY_QUERY,
}SHOW_DETAILE;

// ���Զ�ӦIni�����ļ�
#define CN_ID				1
#define EN_ID				2
#define PN_ID				3

/**
@brief  LED ��ʾ��������
*/
class CStatusDisplayModuleHelper 
{
public:
	~CStatusDisplayModuleHelper(void);				// ��������
	static CStatusDisplayModuleHelper& GetInstance();
public:
	long Connect();								//�򿪴���
	long Init();
	long Close();								// �رմ���
	bool Repare();								// �����޸�����

	long showTVMStatus(WORD wbStatusCode);	//��ʾ��Ӫ״̬
	long showWorkMode();					//��ʾ��ǰ����ģʽ
	long showChargeMode();					//��ʾ��ֵģʽ
	long showPaymentMode();					//��ʾ֧��ģʽ

private:
	CStatusDisplayCommand m_StatusDisplay;
	CStatusDisplayModuleHelper();			// ���캯��
	static CStatusDisplayModuleHelper theInstance;
	void ShowCurWorkMode();					// ��ʾ��ǰģʽ
	long ShowModeFunction(int modeValue);
	DWORD m_wCurrentStatus;
};
