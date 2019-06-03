#pragma once
#include "ServiceHelper.h"
#include "CHCommand.h"
#include "CHException.h"
#include "CHHardwareException.h"
#include "CHdef.h"
#include "CHAApi.h"
/**
@brief  Ӳ��ģ�������
*/

class CCoinModuleHelper
{
public:
    CCoinModuleHelper();                // ���캯��
    ~CCoinModuleHelper(void);          // ��������
	
		typedef enum _box_no
	{
		hopper1 = 1,															
		hopper2,
		small_hopper,
		collection_boxA,
		collection_boxB,
		temporary1,
		temporary2

	}Box_No;
    BOOL IsExceptionCode(long errCode);	
	
	// �����ӿ�
	long CH_Open();
	long CH_Close();
	long CH_Init();
	long CH_InitModule();
	long CH_GetStatus(tCHADevStatus* pStatus);
	long CH_Repare();
	// ����Ӳ��ģ���������Ӳ����
	long CH_SetCoinNum();

	// ���ܡ�����ӿ�
	long CH_StartReceive();													// ��ʼ����
	long CH_StartReceive(CService*);										// ��ʼ���գ������첽��ȡ������Ϣ
	long CH_StartReceive(CService*, COIN_ACCEPTABLE& coinFaceTable);		// ��ʼ���գ�ָ���������������첽��ȡ������Ϣ
	long CH_GetInsertInfo(tCHACoinValueInfo& rsp);								// ��ȡ����Ӳ����Ϣ
	long CH_GetInsertInfoAsync(CService* pCallbackSvc);						// ��ȡ����Ӳ����Ϣ��ֱ�����յ�Ӳ�һ�ֹͣ����Ϊֹ���첽���ã�
	long CH_StopInsertInfo();												// ֹͣ��ȡ����Ӳ����Ϣ
	long CH_EndReceive();													// ֹͣ����Ӳ��
	long CH_Refund();														// �˻ؽ��յ�Ӳ��
	long CH_RefundAsync(CService* pCallbackSvc);							// �˻ؽ��յ�Ӳ�ң��첽���ã�

	long CH_Accept(bool bHopperAccept = true);	
	long CH_AcceptAsync(CService* pCallbackSvc, bool bHopperAccept = true);					

	long CH_ClearCoin(tCoinBoxInfo *tBoxInfo);


	long CH_Change(WORD num_OneY, WORD num_HalfY, tCHAChangeNum* rsp);
	long CH_ChangeAsync(CService* pCallbackSvc,WORD num_OneY, WORD num_HalfY);
	long CoinShutter_Open();																			// ��2������?��??��3?�䨰?a
	long CoinShutter_Close();																			// ��2������?��??��3?1?��?
    long GetShutterStatus();																			// ��2������?��??�����䨬?

	// RFID
	CString CH_ReadRfidData(int boxNo);	// ���ӱ�ţ�0x01 = �ӱ���2  0x02 = �ӱ���1  0x03 = ������1  0x04 = ������2
	long CH_WriteRfidData(int boxNo, BYTE* rfidData);// ���ӱ�ţ�0x01 = �ӱ���2  0x02 = �ӱ���1  0x03 = ������1  0x04 = ������2

	// �����ӿ�
    void CoinDataWrite(tCHADevStatus& coin_data);														// CH??��y?������D�䨨??����?
    //void CoinDataRead(tCHADevStatus&);	
	long CH_OpenShutter();
	long CH_CloseShutter();
	long CH_RepareShutter();
	//long CH_GetShutterStatus();

private:
	bool m_bMonitor;
	tCHADevStatus m_dataLight;
	int m_nSortTrayRunCount;								// �ּ���ת������	
};
