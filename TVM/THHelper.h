#pragma once
#include "ServiceHelper.h"
#include "THMessage.h"
#include "UpgradeHelper.h"
//@brief TH Helper

class CTHHelper : public CServiceHelper
{
public:

	CTHHelper(CService& service);    // ���캯��
	~CTHHelper(void);                // ��������

	void Connect();                  // TH������������
	void Disconnect();               // TH�Ͽ���������
	void Initialize();               // ���ڻ���Ʊ��
	void Test();                     // ���Ժ��� 

	void CheckTicketBoxExist(TH_RSP_GET_DEV_STATUS* response, bool& isMagazineAExist,bool& isMagazineBExist,bool& isMagazineCExist);
	long SetWorkMode(TH_CMD_SET_WORK_MODE command);										// ���ù���ģʽ
	long CardOut(TH_CMD_CARD_OUT command, UINT* uiTransportNum);						// ��ȡƱ��
	long SendCard(UINT* uiTransportNum);												// �ͳ�Ʊ��
	long RetractCard(UINT* uiRetractNum);												// ����Ʊ��
	long GetDevStatus(TH_RSP_GET_DEV_STATUS* response);									// ȡ�豸״̬
	long Reset(TH_CMD_INIT_RESET command, UINT* uiRetractNum);								// ����
	long SetTicketBoxCountInfo(TH_BOX_TYPE inBoxID, tTIMCardNum* inCardNum);	// ����Ʊ������
	long GetTicketBoxCountInfo(TH_BOX_TYPE command, tTIMCardNum* response);	// ȡƱ������״̬
	long Init(TH_CMD_INIT_RESET command, UINT* uiRetractNum);									// ��ʼ��
	long GetVersion(TH_RSP_GET_VERSION* response);										// ��ȡ�汾��
	long TestSetting(TH_CMD_TEST_SETTING command);										// Ӳ������

	// ά���ӿ�
	bool Repare();																		// �޸���Ʊ��
	bool TryRepareTH();																	// �����޸���Ʊ��
	//bool ReadTicketBoxRFID(TH_BOX_TYPE boxType,tTIMRFIDInfo* pTimRfidInfo);				// ��ȡRFID
	//bool WriteTicketBoxRFID(TH_BOX_TYPE boxType,tTIMRFIDInfo* pTimRfidInfo);			// дRFID
	bool ReplaceTicketBox(TH_REPLACE_BOX_CMD_INFO);									// Ʊ�����

	bool GetStaticBoxId(IN TH_BOX_TYPE boxId,OUT tTIMRFIDInfo& rfidInfo);

	static int UpdateProgram();									// ����TH�̼�
private:
	long CommOpen();
	long CommClose();
	// ����
	bool IsExceptionCode(long);                                           // �Ƿ�Ϊ��Ҫ���쳣��ID

};
