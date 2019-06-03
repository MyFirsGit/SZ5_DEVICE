#pragma once
#include "OperationDlg.h"

class CBanknoteCollectingBoxRemovalSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CBanknoteCollectingBoxRemovalSvc(void);
	~CBanknoteCollectingBoxRemovalSvc(void);
	typedef enum _dialog_group
	{
		DIALOG_GROUP_OPERATION = 1,
		DIALOG_GROUP_FINISH,
		DIALOG_GROUP_ERROR,
	} DIALOG_GROUP;

	// 操作类型
	enum OperationType
	{
		OPERATION_ATTACH_BOX,				// 安装纸币箱
		OPERATION_DETACH_BOX,				// 卸载纸币箱
		OPERATION_UNKNOWN,					// 未知
	};

	// 显示页（只有一页的时候无用，一般用于加币）
	enum ShowPage
	{
		SHOW_PAGE1,		// 第一页
		SHOW_PAGE2,		// 第二页
		SHOW_PAGE3,		// 第三页
	};

	typedef enum _operation
	{
		NORMAL_OPERATION,	//正常操作
		VIOLATE_INSTALL,	//非法安装
		VIOLATE_UNINSTALL	//非法卸载
	}OPERATION;

	// 纸币箱信息
	typedef struct _tag_banknote_box_info
	{
		CString raw_id;			// 原始编号
		CString now_id;			// 现在编号
		DWORD face1;
		DWORD face5;
		DWORD face10;			// 10元数量
		DWORD face20;			// 20元数量
		DWORD face50;			// 50元数量
		DWORD face100;			// 100元数量
		//DWORD face500;			// 500元数量
		//DWORD face1000;			// 1000元数量
		//bool raw_attached;		// 是否安装（原始状态）
		//bool now_attached;		// 是否安装（现在状态）
		//OPERATION opt;			// 操作类型
		_tag_banknote_box_info()
		{
			raw_id = _T("");
			now_id = _T("");
			face1 = 0;
			face5 = 0;
			face10 = 0;
			face20 = 0;
			face50 = 0;
			face100 = 0;
			//face500 = 999;
			//face1000 = 999;
			/*raw_attached = false;
			now_attached = false;
			opt = NORMAL_OPERATION;*/
		}

		void SetBanknoteCount(CTBanknoteCountInfo::BANKNOTE_BOX_INFO &info)
		{
			face1 = info.ulCount_CNY1;
			face5 = info.ulCount_CNY5;
			face10 = info.ulCount_CNY10;
			face20 = info.ulCount_CNY20;
			face50 = info.ulCount_CNY50;
			face100 = info.ulCount_CNY100;
		}

	} BanknoteBoxInfo;

	// SVC数据模型
	typedef struct _tagModel
	{
		OperationType operation_type;			// 当前操作类型
		BanknoteBoxInfo box_data;				// 纸币箱信息

		CString			strState;					// 纸币箱所处状态
		COLORREF		clrState;					// 纸币箱状态对应的颜色
		bool			bIsPrintFailed;				// 是否打印失败？
		_tagModel()
		{
			strState = _opl(TXT_BANKNOTEMGR_BOX_DETACHED);
			clrState = RED;
			operation_type = OPERATION_UNKNOWN;
			bIsPrintFailed = false;
		}
	} Model;

	Model& GetDataModel();
	void NotifyDataModelChanged();
	void OnStart(); 

	typedef struct	_BANKNOTE_BOX_DATA			//钱币交易数据
	{
		BankNoteAndCoinType_t	value;  //面值
		int						nCount; //数量
		long					lAmount;//金额
	}BANKNOTE_BOX_DATA;

	OPERATION GetOperationType();
	bool IsBoxConnected();				//外部接口，获取纸币箱连接状态

	CString GetShowErrorMsg();
private:
	Model m_DataModel;
	vector<BANKNOTE_BOX_DATA> vecBanknote_uninstall;

	bool CheckBoxID(LPBYTE boxID);						//判断纸币箱ID是否合法

	void InitDetachBoxData();							//初始化卸载业务数据
	void DoHandleDetachBox();							//卸载业务处理
	void LoadBanknoteBoxData();
	void InitializeDataModel();
	//void LoadBankNoteBoxUninstallReception();

	CString m_strShowMsg;
	CString CashValueToString(BankNoteAndCoinType_t& value);

	LRESULT OnKeyboardF3(WPARAM, LPARAM);
	LRESULT OnKeyboardF2(WPARAM , LPARAM );			// 处理F2按下消息
	LRESULT OnKeyboardEnter(WPARAM, LPARAM);		// 处理 [Enter] 键按下消息

	void LoadBankNoteBoxUninstallReception();
	CTBanknoteCountInfo::BANKNOTE_BOX_INFO m_Ex_Col;
};

