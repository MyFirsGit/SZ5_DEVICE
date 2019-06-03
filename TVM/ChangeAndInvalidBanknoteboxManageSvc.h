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
	// �������� ��װ����ж��
	typedef enum _box_operation{
		INSTALLATION,
		REMOVAL
	}BOX_OPERATION;

	// ��ǰ���������Ĳ���״̬
	typedef enum _curent_phase{
		CHOOSE_BOX,		//�ȴ�ѡ��Ҫ���������ӣ�Ҳ�ǳ�ʼ״̬
		SET_ID,			//����ID�׶�
		INPUT_COUNT,	//�ȴ���������
	}PHASE;
	// ��������
	typedef enum _box_type{
		UNRECOGNIZED	= 0,
		CHANGE_BOX,	// ֽ��������A	
		//CHANGE_BOX_B,	// ֽ��������B
		//INVALID_BOX,	// �ϱһ�����
		//ALL_BOX,		// ��������
	}BOX_INDEX;

	// ����״̬
	typedef enum _box_state{
		UNKNOWN_STATE,
		INSTALLED,
		PRE_INST,
		REMOVED,
		PRE_RMV,
	}BOX_STATE;

	// ��������
	typedef struct _box_data{
		CString	  strID;	// ����ID
		int		  nCount;	// ֽ������ �ϱ��������Ϊ0
		BOX_STATE boxState;	// ����״̬
		_box_data()
		{
			strID	= _T("FFFFFFFF");
			nCount	= 0;
			boxState= UNKNOWN_STATE;
		}

		// ��ͬ״̬�Ĳ�ͬ��ɫ
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

	// ����ģ��
	typedef struct _model
	{
		BOX_DATA	changeBoxAData;	// ������A����
		//BOX_DATA	changeBoxBData;	// ������B����
		//BOX_DATA	invalidBoxData;	// �ϱ�������

		bool		bIsException;

		CString		strPreID;		// ID�Ĺ̶�����
		CString		strPreInvalidID;// ��Ʊ��̶�����
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

	CString TranslateState(BOX_STATE boxState);		// ����״̬���ַ�����


private:
	int m_nRBox1;
	int m_nRBox2;
	int m_nRBox3;
	int m_nRBox4;
	Model	m_model;
	int		m_nEnterTimes;							// ����Enter�Ĵ���ͳ��	
	int		m_nBoxSel;								// �������
	PHASE	m_phase;								// ��ǰ�����Ĳ����׶�
	int		LineCode;								// ��·���
	stack<BOX_INDEX> m_stack_ready;					// ׼����������ж�أ������Ӽ�

	void	CleanStack();							// ���ջ
	void	doInstallBox(int nIndex);
	void	SetBoxID(BOX_INDEX index, CString& strID);	// ����ָ�����ӵ�ID
	void	SetCount(int nIndex, int nCount);
	void	doUnintallBox();
	void	WriteChangeBoxCountInfo();				// дCountInfo�ļ�
	void	InitializeData();						// ���ݳ�ʼ��
	CString ShowGuide();							// ������

	PHASE	getPhase();

	bool	IsBoxCouldBeInstall(BOX_INDEX index);	// �����Ƿ�ɱ���װ

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

	bool CheckInputBoxID(BOX_INDEX boxType,int nBoxId);		// ��������ID�Ƿ��ظ�

	CString ReadBoxRFIDAndSetID(BOX_INDEX boxType);			// ��ȡ������ֽ��������RFID
};

