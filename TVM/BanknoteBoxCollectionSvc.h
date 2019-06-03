#pragma once
#include "TVMForeService.h"

/**
@brief ֽ�������ҵ��Service
*/
class CBanknoteBoxCollectionSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CBanknoteBoxCollectionSvc();
	virtual ~CBanknoteBoxCollectionSvc();

	// ������
	typedef enum _dialog_group
	{
		DIALOG_GROUP_BASE = 1,
	} DIALOG_GROUP;


	// ״̬��Ϣ
	typedef enum _em_operation_{
		OP_RECYCLE_NONE,		// ��ʼ��״̬
		OP_RECYCLE_ING,			// ������
		OP_RECYCLE_SUCCESS,		// ���ճɹ�
		OP_RECYCLE_FALIED,		// ����ʧ��
	}RECYCLE_TYPE;

	// SVC����ģ��
	typedef struct _tagModel
	{
		CTBanknoteCountInfo::BANKNOTE_BOX_INFO banknoteCashInfo;
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO banknoteChangeBoxInfo;

		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO cycleBoxAInfo;
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO cycleBoxBInfo;
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO cycleBoxCInfo;
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO cycleBoxDInfo;

		CTBanknoteCountInfo::BANKNOTE_BOX_INFO Ex_banknoteCashInfo;
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO Ex_banknoteChangeBoxInfo;

		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO Ex_cycleBoxAInfo;
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO Ex_cycleBoxBInfo;
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO Ex_cycleBoxCInfo;
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO Ex_cycleBoxDInfo;

		RECYCLE_TYPE cash_box;
		RECYCLE_TYPE change_box;
		RECYCLE_TYPE recycle_box_A;
		RECYCLE_TYPE recycle_box_B;
		RECYCLE_TYPE recycle_box_C;
		RECYCLE_TYPE recycle_box_D;

		bool bIsPrintFailed;													// �Ƿ��ӡʧ��?

		_tagModel(){
			bIsPrintFailed = false;
			cash_box		= OP_RECYCLE_NONE;
			change_box		= OP_RECYCLE_NONE;
			recycle_box_A	= OP_RECYCLE_NONE;
			recycle_box_B	= OP_RECYCLE_NONE;
			recycle_box_C	= OP_RECYCLE_NONE;
			recycle_box_D	= OP_RECYCLE_NONE;
		}
	} Model;

	Model& GetDataModel();
	void NotifyDataModelChanged();

	void OnStart();
	
private:
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );						// ����Enter������Ϣ
	LRESULT OnKeyboardF2(WPARAM , LPARAM );							// ����F2������Ϣ
	CString FormatAmount(long);
	Model m_DataModel;
	void PrintBanknoteCollectionReception();
};
