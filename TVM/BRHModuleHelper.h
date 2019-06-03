#pragma once
#include "ServiceHelper.h"
#include "BRHMessage.h"
#include "BRHException.h"
#include "Sync.h"
#include "BRHHardwareException.h"

/**
@brief  ֽ��ģ�������
*/
class CBRHModuleHelper
{
public:
	CBRHModuleHelper();						// ���캯��
	~CBRHModuleHelper();						// ��������

	long Init(bool bRepareCount=true);				// ��ʼ��(�Ƿ��޸��������
	long Reset();									// ��λ
	long Abort();									// ֹͣ����, ���ڰ��ſ���е�ֹͣ������������ѯ

	long OpenAndValidate(BRH_STATUS& status);		// ��ʼ����ʶ��ֽ��(δʹ��)
	long OpenAndValidate(tBncCashInfo& feedback); // ��ʼ����ʶ��ֽ��(MEI)

	long StopValidation();							// ֹͣ����ʶ��ֽ��

	long Collect();									// ѹ��(ͬ��)
	//long Collect(BRH_CMD_FEEDBACK& feedback);		// ѹ����MEI,��������Ϣ��

	long ReturnAll(bool isCheckShutter = false);	// �˻�ȫ��ֽ��
	long ReturnAll(BRH_CMD_FEEDBACK& feedback);		// ��ֽ��(MEI)(δʹ��)

	long ReturnCache(bool isCheckShutter = false);	// �˻ػ�������ֽ��	

	long GetBoxID(BRH_BOX_ID& box_id);				// ��ȡǮ��ID
	//long GetBoxID(MEI_BOX_ID& box_id);				// ��ȡMEIǮ��ID
	//long Hold();									// ѹ�ݴ� ** MEI��  ֽ�ҽ���ʶ�� �� ѹ�ݴ���ͬһ������work������
	//long Clean(BRH_CLEAN_DEST dest);				// �����Ʊ
	long GetStatus(tBncDevReturn& status);		// ��ȡ״̬
	long GetSystemStatus(tBncBillBoxInfo& queryCashUnit);		// ��ȡ��ϸ״̬
	     

	long Repare(bool bMenualRepare = false);		// �쳣���
	static bool IsHWException(long errCode);		// �Ƿ�ΪӲ���쳣
	//bool IsBankNoteBoxAttached();                   // �ж�ֽ�����Ƿ�װ��
	bool IsCashBoxAttached();						// �ж�ֽ�һ������Ƿ�װ��	
	bool IsCashChangeBoxAttached();					// �ж�ֽ���������Ƿ�װ��

	void OpenAndValidateAsync(CService* pSvc,tBncSetInhibitList& bhTable);	// �첽����ʼ����ʶ��ֽ��,�ɽ��յ���ֵ�б�

	//void HoldAsync(CService* pSvc);					// �첽��ѹ�ݴ�

	void CollectAsync(CService* pSvc);				// �첽��ѹ��

	void ReturnCacheAsync(CService* pSvc);			// �첽���˻ػ�������ֽ�� MEI ��ֻ���˳��ݴ��е�ȫ��ֽ�� 

	void ReturnAllAsync(CService* pSvc);			// �첽���˻�ȫ��ֽ��  

	long TryRecoveryBanknoteModule();				// �������˳���ʽ�޸�ģ��

	long CheckModuleStatus();						// ���ģ��״̬


	// ά����

	// ���ճ�Ʊ
	long Empty(char* chBoxName,tBncChangeNum* result,bool btoFloat = false);
	// ���»��������
	long ClearCollectionUnitCount();

	// ��ȡ����������Ϣ
	map<BNR_MODULE_ID,T_XfsPhysicalCashUnit> GetEmptyFull();


	// ��Ҫ��һ��ͬ�������Ľӿ�


public://ֽ������
	//void SendOutBill(CService* pService,int nRecycBoxA,int nRecycBoxB,int nRecycBoxC,int nCountBoxD,int nChangeBox);

	void DispenseAsync(CService* pSvc, UINT32 dispenseAmount);	// �첽������
	long Dispense(UINT32 dispenseAmount);						// ��������
	long Present(UINT32* presentAmount);						// ��ʼ����
	long CancelWaitingCashTaken();								// ȡ���ȴ��û�ȡ�߳�Ʊ
	long Retract(UINT32* retractAmount);						// ���ճ�Ʊ

private:
	long CheckAndOpenBHComport();								// ��ֽ�ҽ���ģ�飬�Ѵ���رպ��ٴ򿪡�
	void ProcessErrorCode(long errCode);
};
