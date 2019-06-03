#pragma once
#include "TVMForeService.h"

/**
@brief 纸币箱管理业务Service
*/
class CBanknoteBoxManageSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CBanknoteBoxManageSvc();
	virtual ~CBanknoteBoxManageSvc();

	// 画面组
	typedef enum _dialog_group
	{
		DIALOG_GROUP_BASE = 1,
		DIALOG_GROUP_OPERATION,
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
		bool raw_attached;		// 是否安装（原始状态）
		bool now_attached;		// 是否安装（现在状态）
		OPERATION opt;			// 操作类型
		_tag_banknote_box_info()
		{
			raw_id = _T("");
			now_id = _T("");
			face10 = 0;
			face20 = 0;
			face50 = 0;
			face100 = 0;
			face5 = 0;
			face1 = 0;
			raw_attached = false;
			now_attached = false;
			opt = NORMAL_OPERATION;
		}
	} BanknoteBoxInfo;

	// SVC数据模型
	typedef struct _tagModel
	{
		OperationType operation_type;			// 当前操作类型
		ShowPage show_page;						// 当前显示页
		BanknoteBoxInfo banknote_box;			// 纸币箱信息
		bool bIsSelectInstall;					// 是否选中了安装
		bool bIsSelectUninstall;				// 是否选中了卸载
		bool bIsBanknoteModuleException;
		_tagModel()
		{
			operation_type = OPERATION_UNKNOWN;
			show_page = SHOW_PAGE1;
			bIsSelectInstall = false;
			bIsSelectUninstall = false;
			bIsBanknoteModuleException = false;
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
	bool IsBoxConnected();//外部接口，获取纸币箱连接状态

	CString GetShowErrorMsg();
private:

	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);	// 处理 [Enter] 键按下消息
	LRESULT OnKeyboardF2(WPARAM , LPARAM );							// 处理F2按下消息

	void InitAttachBoxData();		// 初始化安装业务数据
	void InitDetachBoxData();		// 初始化卸载业务数据

	void DoHandleBase();			// 处理基本信息画面
	void DoHandleAttachBox();		// 安装业务处理
	void DoHandleDetachBox();		// 卸载业务处理

	bool CheckBoxID(LPBYTE boxID);  //判断纸币箱ID是否合法
	Model m_DataModel;
	vector<BANKNOTE_BOX_DATA> vecBanknote_uninstall;

	void LoadBankNoteBoxInstallReception();
	void LoadBankNoteBoxUninstallReception();
	CString  CashValueToString(BankNoteAndCoinType_t& value);
	void LoadBanknoteBoxData();
	CString m_strShowMsg;
};
