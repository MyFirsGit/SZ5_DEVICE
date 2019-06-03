#pragma once
#include "TVMForeService.h"
#include "DataHelper.h"

/**
@brief ά��ҵ��Service
*/
class CTransactionHistorySvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CTransactionHistorySvc();					// ���캯��
	virtual ~CTransactionHistorySvc();			// ��������

	typedef enum _dialog_group {
		DIALOG_GROUP_BASE	= 1
	} DIALOG_GROUP;

	// SVC����ģ��
	typedef struct _tagModel
	{
		TransactionRecord* ptransInfo;
		BOOL bfirst;
		BOOL bend;
		CString szMtc;
		CString szEc;
		CString szException;
		CString szSolution;
		CString szExceptionTime;
		_tagModel()
		{
			ptransInfo=NULL;
			bfirst=FALSE;
			bend=FALSE;
			szMtc=(_T(""));
			szEc=(_T(""));
			szException=(_T(""));
			szSolution=(_T(""));
			szExceptionTime=_T("");
		}
	}Model;

	Model& GetDataModel();
	void NotifyDataModelChanged();


public:
	void turnPages(long lIndex);
	void updateExceptionInfo();
protected:
	virtual void OnStart();                                                         // Service��������
private:
	//LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM /*lParam*/);					// ���� [0~9] ��������Ϣ
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Enter] ��������Ϣ
	virtual LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);			//  ���� [F3/Ctrl] ��������Ϣ�����أ�
	//Model m_DataModel;
	BOOL pageDown(long lIndex);
	BOOL pageUp(long lIndex);
	Model m_DataModel;
private:
	std::vector<TransactionRecord> m_vec_transInfo;
	//std::vector<TransactionRecord>::iterator m_iter_transInfo;

	vector<CSysException> m_ExceptionList;
	vector<CSysException>::size_type m_ExceptionNum;

	int m_nMaxLoadCount;
};
