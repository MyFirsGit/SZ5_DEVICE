#pragma once
#include <stack>
using namespace std;
class CChangeAndInvalidBanknoteboxManageSvc : public CTVMForeService
{

	DECLARE_MESSAGE_MAP()

public:
	CChangeAndInvalidBanknoteboxManageSvc(void);
	~CChangeAndInvalidBanknoteboxManageSvc(void);


	typedef enum _dialog_group
	{
		DAILOG_CHANGE_INVALID_BOX_MNG = 1,                       
	} DIALOG_GROUP;
	// 操作类型 安装还是卸载
	typedef enum _box_operation{
		INSTALLATION,
		REMOVAL
	}BOX_OPERATION;

	// 当前箱子所处的操作状态
	typedef enum _curent_phase{
		CHOOSE_BOX,		//等待选择要操作的箱子，也是初始状态
		SET_ID,			//输入ID阶段
		INPUT_COUNT,	//等待输入数量
	}PHASE;
	// 箱子种类
	typedef enum _box_type{
		UNRECOGNIZED	= 0,
		CHANGE_BOX,	// 纸币找零箱A	
		//CHANGE_BOX_B,	// 纸币找零箱B
		//INVALID_BOX,	// 废币回收箱
		//ALL_BOX,		// 所有箱子
	}BOX_INDEX;

	// 箱子状态
	typedef enum _box_state{
		UNKNOWN_STATE,
		INSTALLED,
		PRE_INST,
		REMOVED,
		PRE_RMV,
	}BOX_STATE;

	// 箱子数据
	typedef struct _box_data{
		CString	  strID;	// 箱子ID
		int		  nCount;	// 纸币数量 废币箱的数量为0
		BOX_STATE boxState;	// 箱子状态
		_box_data()
		{
			strID	= _T("FFFFFFFF");
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
			case  PRE_INST:					clr = SOFTGREEN;	break;
			case  PRE_RMV:					clr = SOFTRED;		break;
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

		bool		bIsException;

		CString		strPreID;		// ID的固定部分
		CString		strPreInvalidID;// 废票箱固定部分
		bool		bIsPrintFailed;
		_model(){
			bIsException = false;
			strPreID.Format(_T("%02d60"),theMAINTENANCE_INFO.GetLineCode());
			strPreInvalidID.Format(_T("%02d629B%02X"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetMachineCode());
			bIsPrintFailed = false;
		}
	}Model;

	Model& GetDataModel();
	void NotifyDataModelChanged();
	void OnStart();

	/*Model	changeBoxA;
	Model	changeBoxB;
	Model	invalidBox;*/

	CString TranslateState(BOX_STATE boxState);		// 箱子状态（字符串）


private:
	int m_nRBox1;
	int m_nRBox2;
	int m_nRBox3;
	int m_nRBox4;
	Model	m_model;
	int		m_nEnterTimes;							// 键入Enter的次数统计	
	int		m_nBoxSel;								// 箱子序号
	PHASE	m_phase;								// 当前所处的操作阶段
	int		LineCode;								// 线路编号
	stack<BOX_INDEX> m_stack_ready;					// 准备就绪（可卸载）的箱子集

	void	CleanStack();							// 清空栈
	void	doInstallBox(int nIndex);
	void	SetBoxID(BOX_INDEX index, CString& strID);	// 设置指定箱子的ID
	void	SetCount(int nIndex, int nCount);
	void	doUnintallBox();
	void	WriteChangeBoxCountInfo();				// 写CountInfo文件
	void	InitializeData();						// 数据初始化
	CString ShowGuide();							// 向导文言

	PHASE	getPhase();

	bool	IsBoxCouldBeInstall(BOX_INDEX index);	// 箱子是否可被安装

	LRESULT OnKeyboardF3(WPARAM , LPARAM);
	LRESULT OnKeyboardF2(WPARAM , LPARAM); 
	LRESULT OnKeyboardEnter(WPARAM, LPARAM); 
	//LRESULT OnKeyboardNumber(WPARAM,LPARAM);

	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO m_Ex_change;
	/*CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO m_Ex_changeB;
	CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO m_Ex_invalid;*/

	void PrintBanknoteChangeReceiption();

	void InsertOperaterToDatabase(BOX_INDEX boxType);
	BankNoteAndCoinType_t TransBanknoteType(int value);

	bool CheckInputBoxID(BOX_INDEX boxType,int nBoxId);		// 检查输入的ID是否重复

	CString ReadBoxRFIDAndSetID(BOX_INDEX boxType);			// 读取并设置纸币找零箱RFID
};

