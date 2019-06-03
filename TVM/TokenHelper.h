#pragma once
#include "ServiceHelper.h"
#include "TOKENMessage.h"
#include "UpgradeHelper.h"
#include "TokenCyclerRfidCommand.h"
//@brief TOKEN Helper

class CTOKENHelper : public CServiceHelper
{
public:

	CTOKENHelper(CService& service);    // ���캯��
	~CTOKENHelper(void);                // ��������

	void Connect();                  // TOKEN������������
	void Disconnect();               // TOKEN�Ͽ���������
	void Initialize();               // ���ڻ���Ʊ��
	void Test();                     // ���Ժ��� 

	void CheckTicketBoxExist(TOKEN_RSP_GET_DEV_STATUS* response, bool& isMagazineAExist,bool& isMagazineBExist,bool& isMagazineCExist,bool& isMagazineDExist);

	// �׸���API��װ
	//long SetWorkMode(TOKEN_CMD_SET_WORK_MODE command);									// ���ù���ģʽ
	long CardOut(TOKEN_BOX_NO boxNo, UINT* uiTransportNum);									// ��ȡƱ��
	long SendCard(UINT* uiTransportNum);													// �ͳ�Ʊ��
	long RetractCard(UINT* uiRetractNum);													// ����Ʊ��
	long CleanOut(TOKEN_BOX_NO boxNo);														// ����Ʊ��
	long GetDevStatus(TOKEN_RSP_GET_DEV_STATUS* response);									// ȡ�豸״̬
	long Reset();																			// ����
	//long SetTicketBoxCountInfo(TOKEN_CMD_SET_COUNT_INFO inBoxID, tTIMCardNum* inCardNum);	// ����Ʊ������
	//long GetTicketBoxCountInfo(TOKEN_CMD_GET_COUNT_INFO command, tTIMCardNum* response);	// ȡƱ������״̬
	long Init(UINT* uiRetractNum);															// ��ʼ��
	long GetVersion(TOKEN_RSP_GET_VERSION* response);										// ��ȡ�汾��
	//long TestSetting(TOKEN_CMD_TEST_SETTING command);										// Ӳ������

	// ά���ӿ�
	bool Repare();																			// �޸���Ʊ��
	bool TryRepareTOKEN();																	// �����޸���Ʊ��

	// RFID
	CString ReadTokenBoxARFID();
	CString ReadTokenBoxBRFID();
	CString ReadInvalidBoxRFID();
	CString ReadTokenBoxCollectRFID();

	static int UpdateProgram();																// ����TOKEN�̼�
private:
	long CommOpen();
	long CommClose();
	// ����
	bool IsExceptionCode(long);																// �Ƿ�Ϊ��Ҫ���쳣��ID
	// RFID
	bool ReadTicketBoxRFID(TOKEN_READ_WRITE_RFID* pTimRfidInfo);							// ��ȡRFID
	bool WriteTicketBoxRFID(TOKEN_READ_WRITE_RFID* pTimRfidInfo);							// дRFID
	// ������RFID
	long TokenCyclerRfidInit();//ÿ�ζ�д������RFIDǰ����
	long TokenCyclerRfidRead(TOKEN_READ_WRITE_RFID* pTimRfidInfo);							// TOKEN������RFID
	long TokenCyclerRfidWrite(TOKEN_READ_WRITE_RFID* pTimRfidInfo);
};
