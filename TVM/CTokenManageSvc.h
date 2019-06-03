//#pragma once
//#include "TVMForeService.h"
//#include "TemplateHelper.h"
///*
//*@brief	TOKEN����������
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
//	CTokenManageSvc();		// ���캯��
//	~CTokenManageSvc();		// ��������
//
//	typedef enum _box_no_{
//		BOX_A			= 1,
//		BOX_B			= 2,
//	};
//
//	/**
//	*@brief ���涨��
//	**/
//	typedef enum _dlg_group_{
//		DIALOG_GROUP_MAIN	 = 1,		// �˵�ѡ��������
//		DIALOG_GROUP_INSTALL,			// ��װ����
//		DIALOG_GROUP_INSTALLED,			// ��װ��ɽ���
//		DIALOG_GROUP_UNINSTALL,			// ж�ؽ���
//		DIALOG_GROUP_UNINSTALLED,		// ж����ɽ���
//		DIALOG_GROUP_RECYCLE,			// ���ս���
//		DIALOG_GROUP_RECYCLED			// ������ɽ���
//	}DIALOG_GROUP;
//
//	/**
//	*@brief ����״̬����
//	**/
//	typedef enum _token_box_status_{
//		STATUS_UNKNOWN,					// δ֪
//		STATUS_INSTALLED,				// �Ѱ�װ
//		STATUS_UNINSTALL,				// ��ж��
//		STATUS_ILLEGAL_INSTALL,			// �Ƿ���װ
//		STATUS_ILLEGAL_UNINSTALL,		// �Ƿ�ж��
//		STATUS_READY_INSTALL,			// ����װ
//		STATUS_READY_UNINSTALL,			// ��ж��
//		STATUS_INSTALL_ERROR,			// ��װʧ��
//		STATUS_UNINSTALL_ERROR,			// ж��ʧ��
//		STATUS_RECYCLE_ERROR,			// ����ʧ��
//	}TOKEN_BOX_STATUS;
//	
//	/**
//	*@brief ��������
//	**/
//	typedef enum _token_operation_type_{
//		OPERATION_TYPE_NONE,				// �޲���
//		OPERATION_TYPE_INPUT_BOXA_ID,		// ����TOKEN��A��ID
//		OPERATION_TYPE_INPUT_BOXA_QUANTITY,	// ����TOKEN��A������
//		OPERATION_TYPE_INPUT_BOXB_ID,		// ����TOKEN��B��ID
//		OPERATION_TYPE_INPUT_BOXB_QUANTITY, // ����TOKEN��B������
//		OPERATION_TYPE_INPUT_INVALID_BOX_ID,// �����Ʊ���ID
//		OPERATION_TYPE_INPUT_COLLECTED_BOX_ID,// ����������ID
//	}OPERATION_TYPE;
//
//	/**
//	*@brief ���ݿռ䶨��
//	**/
//	typedef struct _box_detail_info_{
//		int				 ulOperatorID;
//		_DATE_TIME		 operateTime;
//		CString			 strBoxID;		// ����ID
//		int				 nQuantity;		// ����
//		TOKEN_BOX_STATUS emStatus;		// ״̬
//		bool			 bIsSelected;	// �Ƿ�ѡ�е�ǰ�˵�
//
//
//		_box_detail_info_(){			// ��ʼ��
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
//			case STATUS_UNKNOWN:				// δ֪
//			case STATUS_UNINSTALL:				// ��ж��
//			case STATUS_ILLEGAL_INSTALL:			// �Ƿ���װ
//			case STATUS_ILLEGAL_UNINSTALL:		// �Ƿ�ж��
//			case STATUS_READY_UNINSTALL:			// ��ж��
//			case STATUS_INSTALL_ERROR:		// ��װʧ��
//			case STATUS_UNINSTALL_ERROR:			// ж��ʧ��
//			case STATUS_RECYCLE_ERROR:			// ����ʧ��
//				color = RED;
//				break;
//
//			case STATUS_INSTALLED:				// �Ѱ�װ
//			case STATUS_READY_INSTALL:			// ����װ
//				color = GREEN;
//				break;
//
//			default:
//				break;
//			}
//			return color;
//		}
//		// ��ֵ
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
//	*@brief ���ݿռ䶨��
//	**/
//	typedef struct _data_model_{
//		// ����ǰ
//		TOKEN_BOX_DETAIL_INFO BeforeTokenBoxA;
//		TOKEN_BOX_DETAIL_INFO BeforeTokenBoxB;
//		TOKEN_BOX_DETAIL_INFO BeforeInvalidTokenBox;
//		TOKEN_BOX_DETAIL_INFO BeforeCollectedTokenBox;
//		TOKEN_BOX_DETAIL_INFO BeforeTokenHopperA;
//		TOKEN_BOX_DETAIL_INFO BeforeTokenHopperB;
//		// ������
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
//		bool				  bPrintSuccess;				// ��ӡ�ɹ����?
//		bool				  bInsertDataBaseSuccess;		// �������ݿ��Ƿ�ɹ���
//		bool				  bIsException;					// Ӳ���Ƿ����쳣��
//		// ��ʼ��
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
//	// �������ݿռ�
//	 Model& GetDataModel() {	return m_DataModel; }
//protected:
//	virtual void NotifyDataModelChanged();
//	void DoMainOperator();											// ��Ӧ���˵�����
//	void DoInstallOperator();										// ��Ӧ��װ����
//	void DoInstallInputOperator();									// ��Ӧ��װʱ�������
//	void DoUninstallOperator();										// ��Ӧж�ذ�ť����
//	void DoRecycleOperator(int boxNo);								// ��Ӧ���ղ���
//	
//private:
//	Model m_DataModel;
//	// ��Ϣ����
//	virtual LRESULT OnKeyboardEnter(WPARAM , LPARAM );              // ���� [Enter] ��������Ϣ��ִ�У�
//	virtual LRESULT OnKeyboardF3(WPARAM , LPARAM );                 // ���� [F3]    ��������Ϣ�����ˣ�
//	virtual LRESULT OnKeyboardF2(WPARAM , LPARAM );                 // ���� [F2]    ��������Ϣ�����棩
//	void OnStart();													// ҵ��ʼʱ��ʼ������
//
//	void InitDataModel();											// ��ʼ������
//	// ����
//	void ShowPrefixGuideMsg(CString strInput);						// ��ʾǰ׺��ʾ��
//	// ����״̬
//	bool IsTokenBoxAStandby();										// TOKEN ��A�Ƿ���λ��
//	bool IsTokenBoxBStandby();										// TOKEN ��B�Ƿ���λ��
//	bool IsInvalidBoxStandby();										// Invalid�Ƿ���λ��
//	bool IsCollectedBoxStandby();									// �������Ƿ���λ��
//	// ����״̬
//	bool IsTokenBoxAApronOpen();									// A�䵲���Ƿ�򿪣�
//	bool IsTokenBoxBApronOpen();									// B�䵲���Ƿ�򿪣�
//
//	// ִ�а�װ
//	void DoInstallTokenBoxA();										// ��װTOKEN��A
//	void DoInstallTokenBoxB();										// ��װTOKEN��B
//	void DoInstallInvalidTokenBox();								// ��װ��Ʊ��
//	void DoInstallCollectedTokenBox();								// ��װ������
//
//	// ִ��ж��
//	void DoUninstallTokenBoxA();									// ж��TOKEN��A
//	void DoUninstallTokenBoxB();									// ж��TOKEN��B
//	void DoUninstallInvalidTokenBox();								// ж�ط�Ʊ��
//	void DoUninstallCollectedTokenBox();							// ж�ػ�����
//
//	// Ӳ��ж��Ʊ��
//	bool RemoveTokenBoxA();											// Ӳ��ж������A
//	bool RemoveTokenBoxB();											// Ӳ��ж������B
//	bool RemoveInvalidTokenBox();									// 
//	// ִ�д�ӡ����
//	bool PrintReciept();											// ��ӡ����
//	CString TransforPrintID(CString boxID);							// ��IDΪFFFFFFFF��ת��Ϊ --------
//	bool m_bIsAutoInputRFID;										// �Ƿ��Զ���ȡRFID
//};