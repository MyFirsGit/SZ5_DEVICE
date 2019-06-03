#pragma once
#include "TVMForeService.h"
#include "DataHelper.h"

/**
@brief 维护业务Service
*/
class CTransactionHistorySvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CTransactionHistorySvc();					// 构造函数
	virtual ~CTransactionHistorySvc();			// 析构函数

	typedef enum _dialog_group {
		DIALOG_GROUP_BASE	= 1
	} DIALOG_GROUP;

	// SVC数据模型
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
	virtual void OnStart();                                                         // Service启动后处理
private:
	//LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM /*lParam*/);					// 处理 [0~9] 键按下消息
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Enter] 键按下消息
	virtual LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);			//  处理 [F3/Ctrl] 键按下消息（返回）
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
