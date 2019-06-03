#pragma once
#include "TVMForeService.h"
#include "DataHelper.h"
#define NULL_STR _T("")	// ���ַ���
/**
@brief ά��ҵ��Service
*/
class CSaleStaticSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CSaleStaticSvc();					// ���캯��
	virtual ~CSaleStaticSvc();			// ��������

	typedef enum _dialog_group {
		DIALOG_GROUP_OVERVIEW	= 1,
		DIALOG_GROUP_DETAIL
	} DIALOG_GROUP;

	// ����ͳ������ģ��
	typedef struct _sale_statci_data_model_{
		/******************ͳ��������*************************/
		CString strDateFrom;	// ��ʼͳ��ʱ��
		CString strDateTo;		// ����ͳ��ʱ��

		DWORD dwFailedAmount;		// ���Ͻ��
		DWORD dwFailedCount;		// ���ϱ���
		DWORD dwFailedTotalAmount;	// �����ܽ��

		DWORD dwSingleTicketAmount;	 // ����Ʊ���
		DWORD dwSingleTicketTrsCount;// ����Ʊ���ױ���
		DWORD dwSingleTicketCount;	 // ����Ʊ��Ʊ����

		DWORD dwSaleTotalAmount;	// ��ƱС�ƣ����
		DWORD dwSaleTotalTrsCount;	// ��ƱС�ƣ�����
		DWORD dwSaleTotalCount;		// ��ƱС�ƣ�����


		DWORD dwRechargeAmount;		// ��ֵ���
		DWORD dwRechargeTrsCount;	// ��ֵ����

		DWORD dwRechargeTotalAmount;	// ��ֵС�ƣ����
		DWORD dwRechargeTotalTraCount;	// ��ֵС�ƣ�����

		DWORD dwNetIncomAmount;			// �����루��Ҫ���Ϲ��Ͻ�

		/***************ͳ���ֽ���ϸ����**********************/

		// ����ֽ��
		DWORD dwRecvBHFace1Count;		// ��1
		DWORD dwRecvBHFace5Count;		// ��5
		DWORD dwRecvBHFace10Count;		// ��10
		DWORD dwRecvBHFace20Count;		// ��20
		DWORD dwRecvBHFace50Count;		// ��50
		DWORD dwRecvBHFace100Count;		// ��100

		DWORD dwRecvBHTotalCount;		// ֽ�ҽ���С�ƣ�����
		DWORD dwRecvBHTotalAmount;		// ֽ�ҽ���С�ƣ����

		// ����Ӳ��
		DWORD dwRecvCHFace05Count;		// ��5��
		DWORD dwRecvCHFace1Count;		// ��1Ԫ

		DWORD dwRecvCHTotalCount;		// Ӳ�ҽ���С�ƣ�ö��
		DWORD dwRecvCHTotalAmount;		// Ӳ�ҽ���С�ƣ����
		
		// Ӳ������
		DWORD dwChangeCHFace05Count;	// ��5��
		DWORD dwChangeCHFace1Count;		// ��1Ԫ

		DWORD dwChangeCHTotalCount;		// Ӳ������С�ƣ�ö��
		DWORD dwChangeCHTotalAmount;	// Ӳ������С�ƣ����
		
		// ֽ������
		DWORD dwChangeBHFace1Count;		// ��1
		DWORD dwChangeBHFace5Count;		// ��5
		DWORD dwChangeBHFace10Count;	// ��10
		DWORD dwChangeBHFace20Count;	// ��20
		DWORD dwChangeBHFace50Count;	// ��50
		DWORD dwChangeBHFace100Count;	// ��100

		DWORD dwChangeBHTotalCount;		// ֽ������С�ƣ�����
		DWORD dwChangeBHTotalAmount;	// ֽ������С�ƣ����


		// ��ʼ��
		_sale_statci_data_model_(){

			strDateFrom				= _T("");
			strDateTo				= _T("");

			dwFailedAmount			= 0;
			dwFailedCount			= 0;
			dwFailedTotalAmount		= 0;

			dwSingleTicketAmount	= 0;
			dwSingleTicketTrsCount	= 0;
			dwSingleTicketCount		= 0;

			dwSaleTotalAmount		= 0;
			dwSaleTotalTrsCount		= 0;
			dwSaleTotalCount		= 0;


			dwRechargeAmount		= 0;
			dwRechargeTrsCount		= 0;

			dwRechargeTotalAmount	= 0;
			dwRechargeTotalTraCount	= 0;

			dwNetIncomAmount		= 0;
			dwRecvBHFace1Count		= 0;
			dwRecvBHFace5Count		= 0;
			dwRecvBHFace10Count		= 0;
			dwRecvBHFace20Count		= 0;
			dwRecvBHFace50Count		= 0;
			dwRecvBHFace100Count	= 0;

			dwRecvBHTotalCount		= 0;
			dwRecvBHTotalAmount		= 0;

			dwRecvCHFace05Count		= 0;
			dwRecvCHFace1Count		= 0;

			dwRecvCHTotalCount		= 0;
			dwRecvCHTotalAmount		= 0;

			
			dwChangeCHFace05Count	= 0;
			dwChangeCHFace1Count	= 0;

			dwChangeCHTotalCount	= 0;
			dwChangeCHTotalAmount	= 0;

			
			dwChangeBHFace1Count	= 0;
			dwChangeBHFace5Count	= 0;
			dwChangeBHFace10Count	= 0;
			dwChangeBHFace20Count	= 0;
			dwChangeBHFace50Count	= 0;
			dwChangeBHFace100Count	= 0;

			dwChangeBHTotalCount	= 0;
			dwChangeBHTotalAmount	= 0;
		}
	}Model;

	Model& GetDataModel();
	void NotifyDataModelChanged();

protected:
	virtual void OnStart();                                                         // Service��������
private:
	LRESULT OnKeyboardF5(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F5] ��������Ϣ
	LRESULT OnKeyboardF6(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F6] ��������Ϣ
	virtual LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);		// ���� [Enter] ��������Ϣ��ִ�У�
	virtual LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);			// ���� [F3/Ctrl] ��������Ϣ�����أ�
	Model m_DataModel;
private:
	void InitSaleStaticOverView(vector<TransactionRecord>& vec_transInfo);	// ��ʼ������ͳ��һ��������
};