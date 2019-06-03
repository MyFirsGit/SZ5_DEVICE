#pragma once
#include "TVMForeService.h"

/**
@brief Ӳ�������ҵ��Service
*/
class CCoinBoxManageAddCoinToCycleChangeBoxSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageAddCoinToCycleChangeBoxSvc();
	virtual ~CCoinBoxManageAddCoinToCycleChangeBoxSvc();

	// ������
	typedef enum _dialog_group
	{
		DIALOG_GROUP_CYCLECHANGE_BOX = 1,

	} DIALOG_GROUP;

	typedef enum _FrameFlow
	{
		FirstFrame = 1,								// Ӳ�Ҽӱһ�������					
		SecondFrame,								// Ӳ�Ҽӱ��н���
		ThridTFrame									// Ӳ�Ҽӱ���ɽ���
	}FrameFlow;

	// �豸����״̬
	typedef enum _work_status{
		WORK_STATUS_READ_AMOUNT		= 0x0001,//��ȡ���
		WORK_STATUS_ACCEPT_COIN		= 0x0002,//Ѻ��
		WORK_STATUS_BEGIN_RECIVE	= 0x0004,//������

	}WORK_STATUS;

	typedef struct _CycleChangeBoxData
	{
		CString CycleChangeCoinBoxID;						// Ӳ��ѭ��������ID
		CString CycleChangeCoinBoxTye;						// Ӳ��ѭ����������ֵ
		CString CycleChangeCoinBoxQuantity;					// Ӳ��ѭ������������
		CString CycleChangeCoinBoxAddingQuantity;			// Ӳ��ѭ��������ӱ�����
		CString CycleChangeCoinBoxAfterAddingQuantity;		// Ӳ��ѭ��������ӱҺ�����
		int CycleChangeCoinBoxFrameFlowTime;				// ������ת����
		int nBeforeAddingStockCount;						// �ӱ�ǰStock����
		int nAddingCoinCount;								// Ӳ�Ҽӱ�����
		_CycleChangeBoxData()
		{
			CycleChangeCoinBoxID = _T("");
			CycleChangeCoinBoxTye = _T("");
			CycleChangeCoinBoxQuantity = _T("0");
			CycleChangeCoinBoxAddingQuantity = _T("0");
			CycleChangeCoinBoxAfterAddingQuantity = _T("0");
			CycleChangeCoinBoxFrameFlowTime = -1;
			nBeforeAddingStockCount = 0;
			nAddingCoinCount = 0;
		}
	}CycleChangeBoxData;

	typedef struct _tagModel
	{
		CycleChangeBoxData cyclechangeboxdata;
		BOOL IsHasException;
		BOOL IsPrinterSuccessfull;
		_tagModel()
		{
			IsHasException = FALSE;
			IsPrinterSuccessfull = TRUE;
		}
	}Model;

	Model& GetDataModel();
	void NotifyDataModelChanged();

	void CycleChangeHandleData();												// Ӳ��ѭ�������䴦��ID��������Ϣ
	void InitializeDataModel();													// ��ʼ������ģ��
	CString GetHopperCoinType(int hopetype);									// Ӳ��ѭ����������ֵת��
	void DoCoinAdding();														// ��ʼ����Ӳ�Ҵ���
	void StopCancleCoinAdding();												// ֹͣ����ȡ���ӱ�
	void Sendcables();															// �ϴ�����
	void PrintCoinBoxExchangeReceipt();											// Ӳ��ѭ��������ӱ�ƾ֤��ӡ
	void Dataintodatabase();													// ���ݵ������ݿ�
protected:
	void OnCoinInserted(UINT res,CCHReadReceivs* chReadReceiveAmount);			// ���յ�Ӳ�һص�����
protected:
	void OnStart();																// Service��������
	//bool OnStop();																// Serviceֹͣ����
private:

	int EnterTime;																// Enter�����´���
	int MenuCode;																// �˵����
	int AddingCount;															// �ӱ�����
	int Stop_Insert;															// ֹͣ��ȡ����־
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Enter] ��������Ϣ
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F3/Ctrl] ��������Ϣ�����أ�

	Model m_DataModel;
	long m_work_status;
};
