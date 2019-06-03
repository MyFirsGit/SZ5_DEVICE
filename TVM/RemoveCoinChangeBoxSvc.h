#pragma once
#include "TVMForeService.h"

// Ӳ��������ж��


class CRemoveCoinChangeBoxSvc:public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
	
public:
	CRemoveCoinChangeBoxSvc(void);
	~CRemoveCoinChangeBoxSvc(void);
	
	typedef enum _dialog_group						// ������
	{
		DIALOG_GROUP_DETACH_COLLECTION_BOX = 1,

	} DIALOG_GROUP;

	typedef enum _change_box_operation_type_{
		INSTALL_CHANGE_BOX		= 1,// ��װ����
		UNINSTALL_CHANGE_BOX	= 2,// ж�ز���
	}CHANGE_BOX_OPERATION_TYPE;

	typedef enum _box_index							// ����λ��
	{
		CHANGE_BOX_A = 1,
		CHANGE_BOX_B
	}CHANGE_BOX_INDEX;

	typedef enum _box_state							// ���ӵ�״̬
	{
		ST_UNKNOWN		= 0,
		ST_INSTALLED,				// �Ѱ�װ
		ST_REMOVED,					// ��ж��
		ST_PRE_REMOVE,				// ��ж��
	}BOX_STATE;

	typedef struct _change_box_info					// ����������
	{
		CString			strID;		// ID
		BankNoteAndCoinType_t	value;		// ���
		int				nCount;		// ����
		BOX_STATE		state;		// ״̬

		_change_box_info()
		{
			strID	= _T("");
			value	= VALUE_UNKNOWN;
			nCount	= 0;
			state	= ST_UNKNOWN;
		}
		// ȡ��״̬�ַ���
		CString StateToString()
		{
			CString strState;
			switch(state)
			{
			case ST_INSTALLED:		strState = _T("�Ѱ�װ");	break;
			case ST_REMOVED:		strState = _T("��ж��");	break;
			case ST_PRE_REMOVE:		strState = _T("��ж��");	break;
			default:				strState = _T("--");		break;
			}
			return strState;
		}
		// ���ݲ�ͬ״̬ȡ�ò�ͬ����ɫ
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

	typedef struct _tagModel						// ����ģ��
	{
		CString	strAChangeBoxExID;					// ��װǰ��ID
		CString	strBChangeBoxExID;

		CString nChangeABoxExCount;					// ��װǰ����
		CString nChangeBBoxExCount;

		BOOL IsHasException;
		
		CHANGE_BOX_INFO boxAinfo;				// ������� �ܼ� ���ǲ�������չ ��Ӳ������չ�Ŀ�����Ҳ����
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

	void PrintCoinBoxExchangeReceipt();								// Ӳ��ѭ��������ӱ�ƾ֤��ӡ
	void Dataintodatabase(int collectionboxnumber);								// ���ݵ������ݿ�

protected:
	void OnStart();																// Service��������
	void NotifyDataModelChanged();
	bool IsBoxCouldBeRemoved(const CHANGE_BOX_INDEX &index);					// �ж������Ƿ���Ա�ж��
private:
	Model	m_DataModel;

	LRESULT OnKeyboardEnter(WPARAM, LPARAM);									// ���� [Enter] ��������Ϣ
	LRESULT OnKeyboardF3(WPARAM , LPARAM );										// ���� [F3/Ctrl] ��������Ϣ�����أ�
	LRESULT OnKeyboardF2(WPARAM , LPARAM );										// ���� [F2/Alt] ��������Ϣ
	void	InitializeData();													// ���ݳ�ʼ��
	void	Sendcables(CHANGE_BOX_INDEX boxIndex);								// �ϴ�����

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_A;
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO mEx_change_B;

	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_A;	
	CTCoinCountInfo::COIN_BOX_INFO mEx_hopper_B;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_A;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_B;

	void InsertCoinBoxToDatabase(int nBoxNumber);								// ж�����������
};