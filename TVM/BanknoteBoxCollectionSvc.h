#pragma once
#include "TVMForeService.h"

/**
@brief 纸币箱管理业务Service
*/
class CBanknoteBoxCollectionSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CBanknoteBoxCollectionSvc();
	virtual ~CBanknoteBoxCollectionSvc();

	// 画面组
	typedef enum _dialog_group
	{
		DIALOG_GROUP_BASE = 1,
	} DIALOG_GROUP;


	// 状态信息
	typedef enum _em_operation_{
		OP_RECYCLE_NONE,		// 初始化状态
		OP_RECYCLE_ING,			// 回收中
		OP_RECYCLE_SUCCESS,		// 回收成功
		OP_RECYCLE_FALIED,		// 回收失败
	}RECYCLE_TYPE;

	// SVC数据模型
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

		bool bIsPrintFailed;													// 是否打印失败?

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
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );						// 处理Enter按下消息
	LRESULT OnKeyboardF2(WPARAM , LPARAM );							// 处理F2按下消息
	CString FormatAmount(long);
	Model m_DataModel;
	void PrintBanknoteCollectionReception();
};
