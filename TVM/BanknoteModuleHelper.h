#pragma once
#include "ServiceHelper.h"
#include "BHCommand.h"
#include "BHException.h"
#include "Sync.h"
#include "BHHardwareException.h"

/**
@brief  ֽ��ģ�������
*/
class CBanknoteModuleHelper
{
public:
	CBanknoteModuleHelper();						// ���캯��
	~CBanknoteModuleHelper();						// ��������

	long Init();									// ��ʼ��
	long Reset();									// ��λ
	long Abort();									// ֹͣ����, ���ڰ��ſ���е�ֹͣ������������ѯ

	long OpenAndValidate(BH_STATUS& status);		// ��ʼ����ʶ��ֽ��
	long OpenAndValidate(BH_CMD_FEEDBACK& feedback); // ��ʼ����ʶ��ֽ��(MEI)

	long StopValidation();							// ֹͣ����ʶ��ֽ��

	long Collect();									// ѹ��
	long Collect(BH_CMD_FEEDBACK& feedback);		// ѹ����MEI,��������Ϣ��

	long ReturnAll(bool isCheckShutter = false);	// �˻�ȫ��ֽ��
	long ReturnAll(BH_CMD_FEEDBACK& feedback);		// ��ֽ��(MEI)

	long ReturnCache(bool isCheckShutter = false);	// �˻ػ�������ֽ��	

	long GetBoxID(BH_BOX_ID& box_id);				// ��ȡǮ��ID
	long GetBoxID(BRH_BOX_ID& box_id);				// ��ȡMEIǮ��ID

	long Hold();									// ѹ�ݴ� ** MEI��  ֽ�ҽ���ʶ�� �� ѹ�ݴ���ͬһ������work������


	long Clean(BH_CLEAN_DEST dest);					// �����Ʊ
	long GetStatus(BH_STATUS& status);				// ��ȡ״̬

	long Repare(bool bMenualRepare = false);		// �쳣���
	static bool IsHWException(long errCode);		// �Ƿ�ΪӲ���쳣
	bool IsBankNoteBoxAttached();                   // �ж�ֽ�����Ƿ�װ��
	

	void OpenAndValidateAsync(CService* pSvc,BH_FACE_ACEPTED_TABLE& bhTable);	// �첽����ʼ����ʶ��ֽ��,�ɽ��յ���ֵ�б�

	void HoldAsync(CService* pSvc);					// �첽��ѹ�ݴ�

	void CollectAsync(CService* pSvc);				// �첽��ѹ��

	void ReturnCacheAsync(CService* pSvc);			// �첽���˻ػ�������ֽ�� MEI ��ֻ���˳��ݴ��е�ȫ��ֽ�� 

	void ReturnAllAsync(CService* pSvc);			// �첽���˻�ȫ��ֽ��  

	long TryRecoveryBanknoteModule();				// �������˳���ʽ�޸�ģ��

private:
	long CheckAndOpenBHComport();					// ��ֽ�ҽ���ģ�飬�Ѵ���رպ��ٴ򿪡�
};
