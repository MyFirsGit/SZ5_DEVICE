#pragma once
#include "TVMForeService.h"

// 硬币找零箱卸载


class CRemoveCoinChangeBoxSvc:public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
	
public:
	CRemoveCoinChangeBoxSvc(void);
	~CRemoveCoinChangeBoxSvc(void);
	
	typedef enum _dialog_group						// 画面组
	{
		DIALOG_GROUP_DETACH_COLLECTION_BOX = 1,

	} DIALOG_GROUP;

	typedef enum _change_box_operation_type_{
		INSTALL_CHANGE_BOX		= 1,// 安装操作
		UNINSTALL_CHANGE_BOX	= 2,// 卸载操作
	}CHANGE_BOX_OPERATION_TYPE;

	typedef enum _box_index							// 箱子位置
	{
		CHANGE_BOX_A = 1,
		CHANGE_BOX_B
	}CHANGE_BOX_INDEX;

	typedef enum _box_state							// 箱子的状态
	{
		ST_UNKNOWN		= 0,
		ST_INSTALLED,				// 已安装
		ST_REMOVED,					// 已卸载
		ST_PRE_REMOVE,				// 待卸载
	}BOX_STATE;

	typedef struct _change_box_info					// 找零箱数据
	{
		CString			strID;		// ID
		BankNoteAndCoinType_t	value;		// 面额
		int				nCount;		// 数量
		BOX_STATE		state;		// 状态

		_change_box_info()
		{
			strID	= _T("");
			value	= VALUE_UNKNOWN;
			nCount	= 0;
			state	= ST_UNKNOWN;
		}
		// 取得状态字符串
		CString StateToString()
		{
			CString strState;
			switch(state)
			{
			case ST_INSTALLED:		strState = _T("已安装");	break;
			case ST_REMOVED:		strState = _T("已卸载");	break;
			case ST_PRE_REMOVE:		strState = _T("待卸载");	break;
			default:				strState = _T("--");		break;
			}
			return strState;
		}
		// 根据不同状态取得不同的颜色
		COLORREF GetColor()
		{
			COLORREF color = BLACK;
			switch(state)
			{
			case ST_INSTALLED:		color = GREEN;		break;
			case ST_REMOVED:		color = RED;		break;
			case ST_PRE_REMOVE:		color = RGB(200,0,0);	break;
			default:				break;
			}
			return color;
		}
	}CHANGE_BOX_INFO;

	typedef struct _tagModel						// 数据模型
	{
		CString	strAChangeBoxExID;					// 安装前的ID
		CString	strBChangeBoxExID;

		CString nChangeABoxExCount;					// 安装前数量
		CString nChangeBBoxExCount;

		BOOL IsHasException;
		
		CHANGE_BOX_INFO boxAinfo;				// 这样穷举 很简单 但是不利于扩展 但硬币箱扩展的可能性也不大
		CHANGE_BOX_INFO	boxBinfo;
		bool IsPrintFailed;	
		_tagModel()
		{
			strAChangeBoxExID = _T("");
			strBChangeBoxExID = _T("");
			nChangeABoxExCount = _T("");
			nChangeBBoxExCount = _T("");

			IsHasException = FALSE;
			IsPrintFailed  = false;
		}
	}Model;

	Model& GetDataModel();

	void PrintCoinBoxExchangeReceipt();								// 硬币循环找零箱加币凭证打印
	void Dataintodatabase(int collectionboxnumber);								// 数据导入数据库

protected:
	void OnStart();																// Service启动后处理
	void NotifyDataModelChanged();
	bool IsBoxCouldBeRemoved(const CHANGE_BOX_INDEX &index);					// 判断箱子是否可以被卸载
private:
	Model	m_DataModel;

	LRESULT OnKeyboardEnter(WPARAM, LPARAM);									// 处理 [Enter] 键按下消息
	LRESULT OnKeyboardF3(WPARAM , LPARAM );										// 处理 [F3/Ctrl] 键按下消息（返回）
	LRESULT OnKeyboardF2(WPARAM , LPARAM );										// 处理 [F2/Alt] 键按下消息
	void	InitializeData();													// 数据初始化
	void	Sendcables(CHANGE_BOX_INDEX boxIndex);								// 上传电文

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_A;
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_B;

	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_A;	
	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_B;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_A;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_B;

	void InsertCoinBoxToDatabase(int nBoxNumber);								// 卸载找零箱入库
};