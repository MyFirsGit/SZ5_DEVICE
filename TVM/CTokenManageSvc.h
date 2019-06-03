//#pragma once
//#include "TVMForeService.h"
//#include "TemplateHelper.h"
///*
//*@brief	TOKEN箱管理服务类
//**/
//#define EMPTY_STR			_T("")
//#define LEN_INPUT_RFID			4
//#define LEN_INPUT_QUANTITY		4
//#define INVALID_ID			_T("FFFFFFFF")
//#define PRINT_ID			_T("--------");
//
//class CTokenManageSvc : public CTVMForeService
//{
//	DECLARE_MESSAGE_MAP();
//public:
//	CTokenManageSvc();		// 构造函数
//	~CTokenManageSvc();		// 析构函数
//
//	typedef enum _box_no_{
//		BOX_A			= 1,
//		BOX_B			= 2,
//	};
//
//	/**
//	*@brief 界面定义
//	**/
//	typedef enum _dlg_group_{
//		DIALOG_GROUP_MAIN	 = 1,		// 菜单选择主界面
//		DIALOG_GROUP_INSTALL,			// 安装界面
//		DIALOG_GROUP_INSTALLED,			// 安装完成界面
//		DIALOG_GROUP_UNINSTALL,			// 卸载界面
//		DIALOG_GROUP_UNINSTALLED,		// 卸载完成界面
//		DIALOG_GROUP_RECYCLE,			// 回收界面
//		DIALOG_GROUP_RECYCLED			// 回收完成界面
//	}DIALOG_GROUP;
//
//	/**
//	*@brief 箱子状态定义
//	**/
//	typedef enum _token_box_status_{
//		STATUS_UNKNOWN,					// 未知
//		STATUS_INSTALLED,				// 已安装
//		STATUS_UNINSTALL,				// 已卸下
//		STATUS_ILLEGAL_INSTALL,			// 非法安装
//		STATUS_ILLEGAL_UNINSTALL,		// 非法卸下
//		STATUS_READY_INSTALL,			// 待安装
//		STATUS_READY_UNINSTALL,			// 待卸载
//		STATUS_INSTALL_ERROR,			// 安装失败
//		STATUS_UNINSTALL_ERROR,			// 卸载失败
//		STATUS_RECYCLE_ERROR,			// 回收失败
//	}TOKEN_BOX_STATUS;
//	
//	/**
//	*@brief 操作定义
//	**/
//	typedef enum _token_operation_type_{
//		OPERATION_TYPE_NONE,				// 无操作
//		OPERATION_TYPE_INPUT_BOXA_ID,		// 输入TOKEN箱A的ID
//		OPERATION_TYPE_INPUT_BOXA_QUANTITY,	// 输入TOKEN箱A的数量
//		OPERATION_TYPE_INPUT_BOXB_ID,		// 输入TOKEN箱B的ID
//		OPERATION_TYPE_INPUT_BOXB_QUANTITY, // 输入TOKEN箱B的数量
//		OPERATION_TYPE_INPUT_INVALID_BOX_ID,// 输入废票箱的ID
//		OPERATION_TYPE_INPUT_COLLECTED_BOX_ID,// 输入回收箱的ID
//	}OPERATION_TYPE;
//
//	/**
//	*@brief 数据空间定义
//	**/
//	typedef struct _box_detail_info_{
//		int				 ulOperatorID;
//		_DATE_TIME		 operateTime;
//		CString			 strBoxID;		// 箱子ID
//		int				 nQuantity;		// 数量
//		TOKEN_BOX_STATUS emStatus;		// 状态
//		bool			 bIsSelected;	// 是否选中当前菜单
//
//
//		_box_detail_info_(){			// 初始化
//			ulOperatorID  = 0;
//			operateTime = _DATE_TIME();
//			strBoxID	= EMPTY_STR;
//			nQuantity	= 0;
//			emStatus	= STATUS_UNKNOWN;
//			bIsSelected	= false;
//		}
//
//		COLORREF GetColorForState()
//		{
//			COLORREF color = BLACK;
//			switch(emStatus)
//			{
//			case STATUS_UNKNOWN:				// 未知
//			case STATUS_UNINSTALL:				// 已卸下
//			case STATUS_ILLEGAL_INSTALL:			// 非法安装
//			case STATUS_ILLEGAL_UNINSTALL:		// 非法卸下
//			case STATUS_READY_UNINSTALL:			// 待卸载
//			case STATUS_INSTALL_ERROR:		// 安装失败
//			case STATUS_UNINSTALL_ERROR:			// 卸载失败
//			case STATUS_RECYCLE_ERROR:			// 回收失败
//				color = RED;
//				break;
//
//			case STATUS_INSTALLED:				// 已安装
//			case STATUS_READY_INSTALL:			// 待安装
//				color = GREEN;
//				break;
//
//			default:
//				break;
//			}
//			return color;
//		}
//		// 赋值
//		_box_detail_info_& operator = (const _box_detail_info_& input){
//			this->ulOperatorID  = input.ulOperatorID;
//			this->operateTime	= input.operateTime;
//			this->bIsSelected = input.bIsSelected;
//			this->emStatus    = input.emStatus;
//			this->nQuantity   = input.nQuantity;
//			this->strBoxID    = input.strBoxID;
//			return *this;
//		}
//	}TOKEN_BOX_DETAIL_INFO;
//
//	/**
//	*@brief 数据空间定义
//	**/
//	typedef struct _data_model_{
//		// 操作前
//		TOKEN_BOX_DETAIL_INFO BeforeTokenBoxA;
//		TOKEN_BOX_DETAIL_INFO BeforeTokenBoxB;
//		TOKEN_BOX_DETAIL_INFO BeforeInvalidTokenBox;
//		TOKEN_BOX_DETAIL_INFO BeforeCollectedTokenBox;
//		TOKEN_BOX_DETAIL_INFO BeforeTokenHopperA;
//		TOKEN_BOX_DETAIL_INFO BeforeTokenHopperB;
//		// 操作后
//		TOKEN_BOX_DETAIL_INFO AfterTokenBoxA;
//		TOKEN_BOX_DETAIL_INFO AfterTokenBoxB;
//		TOKEN_BOX_DETAIL_INFO AfterInvalidTokenBox;
//		TOKEN_BOX_DETAIL_INFO AfterCollectedTokenBox;
//		TOKEN_BOX_DETAIL_INFO AfterTokenHopperA;
//		TOKEN_BOX_DETAIL_INFO AfterTokenHopperB;
//		
//
//		OPERATION_TYPE		  operationType;
//
//		bool				  bPrintSuccess;				// 打印成功与否?
//		bool				  bInsertDataBaseSuccess;		// 插入数据库是否成功？
//		bool				  bIsException;					// 硬件是否有异常？
//		// 初始化
//		_data_model_(){
//			BeforeTokenBoxA			= _box_detail_info_();
//			BeforeTokenBoxB			= _box_detail_info_();
//			BeforeInvalidTokenBox	= _box_detail_info_();
//			BeforeCollectedTokenBox	= _box_detail_info_();
//			BeforeTokenHopperA		= _box_detail_info_();
//			BeforeTokenHopperB		= _box_detail_info_();
//
//			AfterTokenBoxA			= _box_detail_info_();
//			AfterTokenBoxB			= _box_detail_info_();
//			AfterInvalidTokenBox	= _box_detail_info_();
//			AfterCollectedTokenBox	= _box_detail_info_();
//			AfterTokenHopperA		= _box_detail_info_();
//			AfterTokenHopperB		= _box_detail_info_();
//			operationType			= OPERATION_TYPE_NONE;
//			bPrintSuccess			= true;
//			bInsertDataBaseSuccess	= true;
//			bIsException			= false;
//		}
//	}Model;
//
//	// 引用数据空间
//	 Model& GetDataModel() {	return m_DataModel; }
//protected:
//	virtual void NotifyDataModelChanged();
//	void DoMainOperator();											// 响应主菜单界面
//	void DoInstallOperator();										// 响应安装操作
//	void DoInstallInputOperator();									// 响应安装时输入操作
//	void DoUninstallOperator();										// 响应卸载按钮操作
//	void DoRecycleOperator(int boxNo);								// 响应回收操作
//	
//private:
//	Model m_DataModel;
//	// 消息声明
//	virtual LRESULT OnKeyboardEnter(WPARAM , LPARAM );              // 处理 [Enter] 键按下消息（执行）
//	virtual LRESULT OnKeyboardF3(WPARAM , LPARAM );                 // 处理 [F3]    键按下消息（后退）
//	virtual LRESULT OnKeyboardF2(WPARAM , LPARAM );                 // 处理 [F2]    键按下消息（保存）
//	void OnStart();													// 业务开始时初始化数据
//
//	void InitDataModel();											// 初始化数据
//	// 函数
//	void ShowPrefixGuideMsg(CString strInput);						// 显示前缀提示语
//	// 箱子状态
//	bool IsTokenBoxAStandby();										// TOKEN 箱A是否在位？
//	bool IsTokenBoxBStandby();										// TOKEN 箱B是否在位？
//	bool IsInvalidBoxStandby();										// Invalid是否在位？
//	bool IsCollectedBoxStandby();									// 回收箱是否在位？
//	// 挡板状态
//	bool IsTokenBoxAApronOpen();									// A箱挡板是否打开？
//	bool IsTokenBoxBApronOpen();									// B箱挡板是否打开？
//
//	// 执行安装
//	void DoInstallTokenBoxA();										// 安装TOKEN箱A
//	void DoInstallTokenBoxB();										// 安装TOKEN箱B
//	void DoInstallInvalidTokenBox();								// 安装废票箱
//	void DoInstallCollectedTokenBox();								// 安装回收箱
//
//	// 执行卸载
//	void DoUninstallTokenBoxA();									// 卸载TOKEN箱A
//	void DoUninstallTokenBoxB();									// 卸载TOKEN箱B
//	void DoUninstallInvalidTokenBox();								// 卸载废票箱
//	void DoUninstallCollectedTokenBox();							// 卸载回收箱
//
//	// 硬件卸载票箱
//	bool RemoveTokenBoxA();											// 硬件卸载箱子A
//	bool RemoveTokenBoxB();											// 硬件卸载箱子B
//	bool RemoveInvalidTokenBox();									// 
//	// 执行打印操作
//	bool PrintReciept();											// 打印操作
//	CString TransforPrintID(CString boxID);							// 将ID为FFFFFFFF的转换为 --------
//	bool m_bIsAutoInputRFID;										// 是否自动读取RFID
//};