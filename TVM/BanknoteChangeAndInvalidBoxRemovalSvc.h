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
	// �������� ��װ����ж��
	typedef enum _box_operation{
		INSTALLATION,
		REMOVAL
	}BOX_OPERATION;

	// ����λ��
	typedef enum _box_type{
		UNRECOGNIZED,
		CHANGE_BOX,	// ֽ��������A	
		//CHANGE_BOX_B,	// ֽ��������B
		//INVALID_BOX,	// �ϱһ�����
		//ALL_BOX,		// ��������
	}BOX_LOC;

	// ����״̬
	typedef enum _box_state{
		UNKNOWN_STATE,
		INSTALLED,
		WAIT_TO_BE_INSTALLED,
		REMOVED,
		WAIT_TO_BE_REMOVED,
	}BOX_STATE;

	// ��������
	typedef struct _box_data{
		BOX_LOC  boxType;  // ��������
		CString	  strID;	// ����ID
		int		  nCount;	// ֽ������ �ϱ��������Ϊ0
		BOX_STATE boxState;	// ����״̬
		_box_data()
		{
			boxType = UNRECOGNIZED;
			strID	= _T("---------");
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
			case  WAIT_TO_BE_INSTALLED:		clr = SOFTGREEN;	break;
			case  WAIT_TO_BE_REMOVED:		clr = SOFTRED;	break;
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
		bool		bIsPrintFailed;
		_model(){
			bIsPrintFailed = false;
		}
	}Model;

	Model& GetDataModel();
	void NotifyDataModelChanged();
	void OnStart();

	CString TranslateState(BOX_STATE boxState);		// ����״̬���ַ�����

	Model	changeBoxA;
	Model	changeBoxB;
	Model	invalidBox;

private:
	Model	m_model;
	int		m_nEnterTimes;						// ����Enter�Ĵ���ͳ��	
	int		m_nIndex;							// �������

	void	doInstallBox(int nIndex);
	void	setCount(int nIndex, int nCount);
	void	PrepareForRemoveBox(int nInput);
	void	WriteChangeBoxCountInfo();			// дCountInfo�ļ�
	void	SetStateOfPreRemove(BOX_LOC loc);	// ���ô�ж��״̬
	void	InitializeData();					// ���ݳ�ʼ��

	bool	IsBoxCouldBeRemoved(BOX_LOC loc);

	LRESULT OnKeyboardF3(WPARAM , LPARAM);
	LRESULT OnKeyboardF2(WPARAM , LPARAM); 
	LRESULT OnKeyboardEnter(WPARAM, LPARAM); 

	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO m_Ex_change;
	/*CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO m_Ex_changeB;
	CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO m_Ex_invalid;*/

	void PrintBanknoteChangeReceiption();		// ������ƾ֤��ӡ

	void InsertOperaterToDatabase(BOX_LOC boxType);
	BankNoteAndCoinType_t TransBanknoteType(int value);
	DEVICE_STATUS_INFO m_bhChangeInfo;
};

