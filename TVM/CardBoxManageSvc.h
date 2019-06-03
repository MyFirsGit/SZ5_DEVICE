#pragma once
#include "stdafx.h"
#include "TVMForeService.h"
#include "GStatic.h"
#include <stack>
using namespace std;

class CTicketBoxExchangeSvc:public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CTicketBoxExchangeSvc();
	virtual ~CTicketBoxExchangeSvc();
public:
	typedef enum _UE_DLG_GROUP
	{		
		UE_NOTICEINFO=1,//提示信息对话框组
	}UE_DLG_GROUP;		//枚举所有用到的对话框组

	typedef enum _dialog_group {
		DLG_BASE     = 1, //运营日期选择
		DLG_INSTALL  = 2, //凭证选择
		DLG_REMOVE   = 3, //正在打印
	} DIALOG_GROUP;

	typedef enum _box_state
	{
		UNKNOWN_STATE,
		INSTALLED,		// 已安装
		REMOVED,		// 已卸载
		PRE_INSTALL,	// 待安装
		PRE_REMOVE,		// 待卸载
		INVALID_AT,		// 非法安装
		INVALID_DT		// 非法卸载
	}BOX_STATE;

	typedef struct _US_INSTALL_INFO
	{
		CString szID;			//安装票箱的ID
		CString szQuantity;		//票张数量
		CString szSetCount;		//设置的票卡张数（未正式安装）
		CString szWastedID;		//安装的废票箱的ID
		BOX_STATE state;		//箱子状态


		_US_INSTALL_INFO()
		{
			szID=_T("FFFFFFFF");
			szQuantity=_T("0");
			szSetCount = _T("0");
			szWastedID=_T("FFFFFFFF");
		}

		CString GetStateToString()
		{
			CString strState = _T("--");
			switch(state)
			{
			case UNKNOWN_STATE: strState = _T("--");
				break;
			case INSTALLED: strState = _T("已安装");
				break;
			case PRE_INSTALL: strState = _T("待安装");
				break;
			case REMOVED: strState = _T("已卸载");
				break;
			case PRE_REMOVE: strState = _T("待卸载");
				break;
			}
			return strState;
		}
		// 不同状态对应的颜色
		COLORREF GetColorForState()
		{
			COLORREF color = BLACK;
			switch(state)
			{
			case UNKNOWN_STATE: color = BLACK;
				break;
			case INSTALLED: color = GREEN;
				break;
			case PRE_INSTALL: color = SOFTGREEN;
				break;
			case REMOVED: color = RED;
				break;
			case PRE_REMOVE: color = RED;
				break;
			}
			return color;
		}
		// 同步数量，当操作成功是，将上一步设置的数量设为实际数量
		void SyncCount()
		{
			szQuantity = szSetCount;
		}

	}US_INSTALL_INFO;			//安装信息

	typedef enum _UE_BOX_POS
	{
		UE_POS_CARD1=1,
		UE_POS_CARD2,
		UE_POS_WASTED
	}UE_BOX_POS;

	typedef enum box_moval			// 票箱模块移动
	{
		BOX_UP	 = 1,
		BOX_DOWN,
	}BOX_MOVAL;

	typedef enum _UE_CHECK_RET
	{
		UE_VALID=0,
		UE_INVALID_CARDBOXID,
		UE_INVALID_CARDQUANTITY,
		UE_INVALID_WASTEDID,
		UE_INVALID
	}UE_CHECK_RET;

	typedef enum _current_phase		// 当前所处的操作状态
	{
		PHASE_CHOOSE_BOX	=1,
		PHASE_SET_COUNT,
	}CURRENT_PHASE;

	typedef enum _operate_box		// 被操作的箱子
	{
		TICKET_BOX_A		=1,
		TICKET_BOX_B,
		INVALID_BOX,
		//BUFFER,
		ALL_BOX,
	}OPERATE_BOX;

	// 数据模型 澳门项目为何票箱管理中没用到？
	typedef struct _DATA_MODEL
	{
		US_INSTALL_INFO		BoxAinfo;		// 票箱A数据
		US_INSTALL_INFO		BoxBinfo;		// 票箱B数据
		US_INSTALL_INFO		InvalidBox;		// 废票箱数据

		bool				m_bBoxACanOperator;
		bool				m_bBoxBCanOperator;
		bool				m_bInvalidBoxCanOperator;

		bool				bIsException;

		_DATA_MODEL()
		{
			m_bBoxACanOperator = false;
			m_bBoxBCanOperator = false;
			m_bInvalidBoxCanOperator = false;
			bIsException = false;
		}
	}DATA_MODEL;

private:
	DATA_MODEL m_dataModel;
	US_INSTALL_INFO m_usinstallinfo;	//保存安装信息,执行F2的时候会检查合法性

	BOOL	m_bSelectBoxA;
	BOOL	m_bSelectWasted;

	BOOL	m_bSelectUninstallBoxA;
	BOOL	m_bSelectUninstallWasted;
protected:
	virtual void OnStart();                                                         // Service启动后处理
public:
	DATA_MODEL& getModel();															// 获取数据模型中的数据
	void PrintReceipt(int nType);

private:
	long checkInstallInfo(US_INSTALL_INFO& usinstallinfo);	//校验安装信息是否合法
	long InstallDevices();			//安装设备 票箱 废票箱
	long UpdateCardBoxDataForInstall(OPERATE_BOX opBox);			//只安装票箱
	bool installWastedCardBox();	//只安装废票箱
	long UninstallDevices();		//卸载设备 票箱 废票箱
	long UpdateCardBoxDataForRemove(OPERATE_BOX opBox);		//只卸载票箱
	bool uninstallWastedCardBox();	//只卸载废票箱

	long checkInputCardBoxID(CString szID);
	long checkInputCardBoxQuantity(CString szQuantity);
	long checkInputWastedID(CString szID);

	bool isBoxCouldBeOperated(OPERATE_BOX opBox);	// 判断箱子是否可操作
	void toNextPhase();// 切换到下一个状态
	void setCountAndState(OPERATE_BOX box, CString strCount);
	void DoRemoval(int nInput);
	void InitCardBoxData();
	bool HandleBox(TH_MOVE_BOX_CMD_INFO cmd);

	//implement base class's virtual function
public:
	virtual void		NotifyDataModelChanged();
	int  m_buffer_result;	// 缓冲区回收结果 0 未知  1 成功 2 失败

private:		
	virtual LRESULT OnKeyboardEnter(WPARAM , LPARAM);						// 处理 [Enter] 键按下消息（执行）
	virtual LRESULT OnKeyboardF2(WPARAM , LPARAM);							 // 处理 [F2/Alt] 键按下消息（保存）
	virtual LRESULT OnKeyboardF3(WPARAM , LPARAM);							 //  处理 [F3/Ctrl] 键按下消息（返回）
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );
	
	void ShowTips(OPERATE_BOX);												// 显示提示文言
	bool MoveTicketBox(OPERATE_BOX, BOX_MOVAL);								// 移动票箱模块电机
	bool m_bHasException;
	bool boxAUnset;
	bool boxBUnset;

	void InitSvcData();														// 初始化业务数据
	long ReadBoxDataAndShow(int index);										// 读取票箱(硬件)数据并显示
	bool CheckBoxStatus(OPERATE_BOX);										// 检查票箱状态

	CURRENT_PHASE m_CurrentPhase;												 // 当前所处的操作阶段
	stack<OPERATE_BOX>  m_stkOpBox;												// 存储已选择的箱子

	inline void ClearStk(){	
		while (!m_stkOpBox.empty())	{ m_stkOpBox.pop();} }						// 清空栈

	CTCardCountInfo::CARD_BOX_INFO m_Ex_BoxA;									// 操作前票箱数据
	CTCardCountInfo::CARD_BOX_INFO m_Ex_BoxB;
	CTCardCountInfo::CARD_BOX_INFO m_Ex_INLD;
	bool isBufferSelected;
};
