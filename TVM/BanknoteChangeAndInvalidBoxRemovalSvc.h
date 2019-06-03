#pragma once

class CBanknoteChangeAndInvalidBoxRemovalSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CBanknoteChangeAndInvalidBoxRemovalSvc(void);
	~CBanknoteChangeAndInvalidBoxRemovalSvc(void);

	typedef enum _dialog_group
	{
		DAILOG_CHANGE_INVALID_BOX_REMOVAL = 1,                       
	} DIALOG_GROUP;
	// 操作类型 安装还是卸载
	typedef enum _box_operation{
		INSTALLATION,
		REMOVAL
	}BOX_OPERATION;

	// 箱子位置
	typedef enum _box_type{
		UNRECOGNIZED,
		CHANGE_BOX,	// 纸币找零箱A	
		//CHANGE_BOX_B,	// 纸币找零箱B
		//INVALID_BOX,	// 废币回收箱
		//ALL_BOX,		// 所有箱子
	}BOX_LOC;

	// 箱子状态
	typedef enum _box_state{
		UNKNOWN_STATE,
		INSTALLED,
		WAIT_TO_BE_INSTALLED,
		REMOVED,
		WAIT_TO_BE_REMOVED,
	}BOX_STATE;

	// 箱子数据
	typedef struct _box_data{
		BOX_LOC  boxType;  // 箱子种类
		CString	  strID;	// 箱子ID
		int		  nCount;	// 纸币数量 废币箱的数量为0
		BOX_STATE boxState;	// 箱子状态
		_box_data()
		{
			boxType = UNRECOGNIZED;
			strID	= _T("---------");
			nCount	= 0;
			boxState= UNKNOWN_STATE;
		}

		// 不同状态的不同颜色
		COLORREF GetColor()
		{
			COLORREF clr = BLACK;
			switch(boxState)
			{
			case  INSTALLED:				clr = GREEN;	break;
			case  REMOVED:					clr = RED;			break;
			case  WAIT_TO_BE_INSTALLED:		clr = SOFTGREEN;	break;
			case  WAIT_TO_BE_REMOVED:		clr = SOFTRED;	break;
			default:						clr = BLACK;		break;
			}
			return clr;
		}

	}BOX_DATA;

	// 数据模型
	typedef struct _model
	{
		BOX_DATA	changeBoxAData;	// 找零箱A数据
		//BOX_DATA	changeBoxBData;	// 找零箱B数据
		//BOX_DATA	invalidBoxData;	// 废币箱数据
		bool		bIsPrintFailed;
		_model(){
			bIsPrintFailed = false;
		}
	}Model;

	Model& GetDataModel();
	void NotifyDataModelChanged();
	void OnStart();

	CString TranslateState(BOX_STATE boxState);		// 箱子状态（字符串）

	Model	changeBoxA;
	Model	changeBoxB;
	Model	invalidBox;

private:
	Model	m_model;
	int		m_nEnterTimes;						// 键入Enter的次数统计	
	int		m_nIndex;							// 箱子序号

	void	doInstallBox(int nIndex);
	void	setCount(int nIndex, int nCount);
	void	PrepareForRemoveBox(int nInput);
	void	WriteChangeBoxCountInfo();			// 写CountInfo文件
	void	SetStateOfPreRemove(BOX_LOC loc);	// 设置待卸载状态
	void	InitializeData();					// 数据初始化

	bool	IsBoxCouldBeRemoved(BOX_LOC loc);

	LRESULT OnKeyboardF3(WPARAM , LPARAM);
	LRESULT OnKeyboardF2(WPARAM , LPARAM); 
	LRESULT OnKeyboardEnter(WPARAM, LPARAM); 

	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO m_Ex_change;
	/*CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO m_Ex_changeB;
	CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO m_Ex_invalid;*/

	void PrintBanknoteChangeReceiption();		// 找零箱凭证打印

	void InsertOperaterToDatabase(BOX_LOC boxType);
	BankNoteAndCoinType_t TransBanknoteType(int value);
	DEVICE_STATUS_INFO m_bhChangeInfo;
};

