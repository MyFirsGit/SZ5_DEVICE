#pragma once
#include "stdafx.h"
#include "ServiceHelper.h"
#include "RWMessage.h"
#include "VersionInfo.h"
#include "PrinterHelper.h"
#include "SCAuditData.h"
#include "DataHelper.h"

/**
@brief  ��ӡ�����Ϣ��
*/
class CAuditHelper : public CServiceHelper	
{
public:

	typedef struct _product_trans_data					//Ʊ����������
	{
		ProductType		product;		// ��Ʒ����
		int				nCount;			// ����
		long			lAmount;		// ���
		_product_trans_data()
		{
			product	= UNKNOWPRODUCT;
			nCount	= 0;
			lAmount = 0;
		}
	}PRODUCT_TRANS_DATA , *PPRODUCT_TRANS_DATA;

	typedef struct	_CASH_DATA							//�ֽ�������
	{
		BankNoteAndCoinType_t	value;  //��ֵ
		int						nCount; //����
		long					lAmount;//���
		_CASH_DATA()
		{
			value	= VALUE_UNKNOWN;
			nCount	= 0;
			lAmount = 0;
		}
	}CASH_DATA, *PCASH_DATA;


	typedef enum _cash_box_type
	{
		COMMON_BOX,
		RECYCLE_CHANGE_BOX,
		CHANGE_BOX_A,
		CHANGE_BOX_B,
		CYCLE_BOX_A,
		CYCLE_BOX_B,
		COLLECTION_BOX_A,
		COLLECTION_BOX_B,
	}CASH_BOX_TYPE;

	typedef struct _CASH_BOX_DATA
	{
		CASH_BOX_TYPE type;				//��������(��ͨ�䣬ѭ������...)
		CString			nBoxID;			//����ID
		BankNoteAndCoinType_t value;	//��ֵ
		int			amount;				//����
		_CASH_BOX_DATA()
		{
			type = COMMON_BOX;
			nBoxID = _T("--------");
			value = VALUE_UNKNOWN;
			amount = 0;
		}

	}CASH_BOX_DATA, *PCASH_BOX_DATA;
	
	typedef enum _change_box_operation_type_{
		INSTALL_CHANGE_BOX		= 1,// ��װ����
		UNINSTALL_CHANGE_BOX	= 2,// ж�ز���
	}CHANGE_BOX_OPERATION_TYPE;

	typedef struct _FAULT_TRANS_DATA	//���Ͻ���
	{
		int			nFaultCardCount;	//����ʧ��
		long		lFaultCardAmount;
		int			nFaultTokenCount;	//��Tokenʧ��
		long		lFaultTokenAmount;
		int			nFaultChangeCount;	//�������
		long		lFaultChangeAmount;
		int			nFaultDCPayCount;	//���ÿ�֧������
		long		lFaultDCPayAmount;
		int			nFaultBCPayCount;	//���п�֧������
		long		lFaultBCPayAmount;
		int			nFaultEPCount;		//EP֧������
		long		lFaultEPAmount;
		int			nFaultCount;		//���Ϻϼ�
		long		lFaultAmount;
		_FAULT_TRANS_DATA()
		{
			nFaultCardCount = 0;	//����ʧ��
			lFaultCardAmount = 0;
			nFaultTokenCount = 0;	//��Tokenʧ��
			lFaultTokenAmount = 0;
			nFaultChangeCount = 0;	//�������
			lFaultChangeAmount = 0;
			nFaultDCPayCount = 0;	//���ÿ�֧������
			lFaultDCPayAmount = 0;
			nFaultBCPayCount = 0;	//���п�֧������
			lFaultBCPayAmount = 0;
			nFaultEPCount = 0;		//EP֧������
			lFaultEPAmount = 0;
			nFaultCount = 0;		//���Ϻϼ�
			lFaultAmount = 0;
		}
	}FAULT_TRANS_DATA;

	typedef enum _PayMent								// ֧����ʽ
	{
		CASH							= 1,
		EP								= 2,
		BANK_CARD				= 5,
		CREDIT_CARD			= 7
	}PAYMENT_TYPE;
	typedef enum _transaction_type						// ��������
	{
		CARD_ISSUE			=  2,
		/*CARD_ISSUE			=  2,
		PRODUCT_ISSUE		=  3,*/
		RECHARGE			=  4,
	}TRANSACTION_TYPE;
	
	typedef enum _exceion_trans
	{
		//TOKEN_FAILURE		= 1,
		CARD_FAILURE		= 1,
		COIN_CHANGE_FAILURE	= 3,
		NOTE_CHANGE_FAILURE	= 4,
		CREDIT_FAILURE		= 5,
		BANK_FAILURE		= 6,
		EP_FAILURE			= 7,
		WRITE_CARD_FAILED   = 10,
	}EXCEPTION_TRANS;
	typedef struct _balance_data
	{
		_DATE_TIME	dtOriginTime;		//��ʼʱ��
		_DATE_TIME	dtCurrentTime;		//����ʱ��
		/*	�ṹ���ͣ�
				map<ProductType, pair<int, int>>
				map<��Ʒ�������ƣ� pair<�������������׽��>> 

				vector<CASH_DATA>
				vector<<��ֵ,���������>>

				vector<CASH_BOX_DATA>
				vector<<����ID����ֵ������>>

				map<EXCEPTION_TRANS, pair<int, int>> mapException;
				map<������������, pair<�������������׽��>>
		*/
		
		map<ProductType, pair<int, int>> mapIssue;			//��ֵ��������
		map<ProductType, pair<int, int>> mapRecharge;		//�۲�Ʒ��������
		map<ProductType, pair<int, int>> mapIssuePdt;

		map<ProductType, pair<int, int>> mapCashIssue;		//�ֽ���Ʊ��������	
		map<ProductType, pair<int, int>> mapCashRecharge;	//�ֽ��ֵ��������
		map<ProductType, pair<int, int>> mapCashProdIssue;	//�ֽ��۲�Ʒ��������

		vector<CASH_DATA> vecCoinReceived;					//Ӳ�ҽ���
		vector<CASH_DATA> vecNotesReceived;					//ֽ�ҽ���
		vector<CASH_DATA> vecCoinReleased;					//Ӳ������
		map<BankNoteAndCoinType_t,pair<int,int>> mapBanknoteChange;	// ֽ������

		vector<CASH_BOX_DATA> vecCoinAdded;					//Ӳ�Ҳ��䣨�����ˣ�
		map<BankNoteAndCoinType_t,int>	mapCoinAdded;		//Ӳ�Ҳ���
		vector<CASH_BOX_DATA> vecBanknoteAdded;				//ֽ�������䲹�䣨�����ˣ�
		map<BankNoteAndCoinType_t,int>  mapBanknotAdded;	//ֽ�������䲹��
		vector<CASH_BOX_DATA> vecBanknoteChangeInfo;		//ֽ��������ϸ
		vector<CASH_BOX_DATA> vecCoinCollection;			//Ӳ�һ���
		vector<CASH_DATA> vecCashCollection;				//ֽ�һ���
		vector<CASH_DATA> vecCoinCollectionBox;				//Ӳ�һ��գ��������ʱ��Σ�
		
		map<ProductType, pair<int, int>> mapDCIssue;		//���ÿ��ۿ�
		map<ProductType, pair<int, int>> mapDCRecharge;		//���ÿ���ֵ
		map<ProductType, pair<int, int>> mapDCIssuePdt;		//���ÿ��۲�Ʒ

		map<ProductType, pair<int, int>> mapBCIssue;		//���п��ۿ�
		map<ProductType, pair<int, int>> mapBCRecharge;		//���п���ֵ
		map<ProductType, pair<int, int>> mapBCIssuePdt;		//���п��۲�Ʒ

		int			nCashAcceptSum;
		int			nCashChangeSum;
		int			nBnChangeCount;		// ֽ����������

		int			nEpPayCount;		// ��Ʊ����
		long		lEpPayAmount;		// ��Ʊ���

		int			nSumSale;			// ���۽��
		int			nQRSumSale;			// ȡƱ���
		int			nCashSum;			// �ֽ��׽��
		int			nCreditSum;			// ���п����׽��
		int			nBankSum;			// ���ÿ����׽��

		int			nNoteCollection;	// ֽ�һ���
		int			nCoinCollection;	// Ӳ�һ���

		// ֽ�Ҵ���
		vector<CASH_BOX_DATA> vec_BanknoteRecyclerReservInfo;
		vector<CASH_BOX_DATA> vec_BanknoteChangeReservInfo;
		vector<CASH_DATA> vec_BanknoteCollectionReservInfo;

		// Ӳ������
		vector<CASH_BOX_DATA> vec_CoinReservInfo;

		map<EXCEPTION_TRANS, pair<int, int>> mapException;	//���Ͻ���
		int nSumExceptionCount, nSumExceptionAmount;
		_DATE_TIME	dtOperationTime;	// ����ʱ��
		int		nOperatorID;			// ����ԱID
		CString	station_name_CN;		// ��վ�����У�
		CString	station_name_EN;		// ��վ����Ӣ��
		CString	station_name_PN;		// ��վ�����ϣ�
		int		nDeviceID;				// ����ԱID

		_balance_data()
		{	
			nCashAcceptSum = 0;
			nCashChangeSum = 0;
			nBnChangeCount  = 0;
			nCashSum		= 0;
			nCreditSum		= 0;
			nBankSum		= 0;
			nEpPayCount		= 0;
			lEpPayAmount	= 0;
			nOperatorID		= 0;
			nNoteCollection = 0;
			nCoinCollection = 0;
			nSumSale		= 0;
			nQRSumSale		= 0;
			mapBanknoteChange.clear();
			mapIssue.clear();
			mapRecharge.clear();
			mapIssuePdt.clear();

			mapCashIssue.clear();
			mapCashRecharge.clear();
			mapCashProdIssue.clear();

			vecCoinReceived.clear();
			vecNotesReceived.clear();
			vecCoinReleased.clear();

			vecCoinAdded.clear();
			vecCoinCollection.clear();
			vecCashCollection.clear();
			vecCoinCollectionBox.clear();

			mapDCIssue.clear();
			mapDCRecharge.clear();
			mapDCIssuePdt.clear();

			mapBCIssue.clear();
			mapBCRecharge.clear();
			mapBCIssuePdt.clear();


			station_name_CN = _T("");
			station_name_EN = _T("");
			station_name_PN = _T("");
			nSumExceptionCount = 0;
			nSumExceptionAmount = 0;
		}

	}BALANCE_DATA;

	CAuditHelper(CService& service);													// ���캯��
	~CAuditHelper();																	// ��������

	BALANCE_DATA LoadBalanceData();														// ��ȡ��������
	int PrintBalanceBill(bool bIsPrint = true);											// ҵ������ģ�廯,ҵ�ᵥ�ݴ�ӡ����
private:
	CString  GetProductName(ProductType prodtype);										// ��Ʒ����	ת	�ַ���
	CString  CashValueToString(BankNoteAndCoinType_t& value);							// �ֽ���ֵ	ת	�ַ�����������ʾ��
	int		 FormatCashToINT(BankNoteAndCoinType_t cash);								// �ֽ���ֵ	ת	����(��������)
	int DeviceType;																		// �豸����
	pair<CString, CString> GetSum(map<ProductType, pair<int, int>> mapData);			// �������
	pair<CString, CString> GetSum( vector<CASH_DATA>& vec);							// �������
	pair<CString, CString> GetSum( vector<CASH_BOX_DATA>& vec);						// �������

	DWORD TransTypeToValue(int value);													// ת��Ӳ������Ϊֵ
	void SendTVMAuditData();															// ����������ݵ�������
	PurchaseData m_curPurchase;
	BALANCE_DATA m_BalanceData;
	CString FromInt(int);
	BankNoteAndCoinType_t TransCoinType(int value);										// ��ֵת��
	BankNoteAndCoinType_t TransBanknoteType(int value);									// ת��ֽ������
	//void PrintAuditReport(SALESTAT_KIND kind, PurchaseData& purchaseData);               //��ӡ�����Ϣ
	//CString GetPrintLineInfo(CString strTitle,int number,int amount);
	//CString GetPrintLineInfo(unsigned short ticketID,int number,int amount,bool isTotal,bool isYKT,bool isAdjust = false);
	//void PrintTicketInfo(CStringArray& printTicketTexts, map<WORD,SCAuditValue>& summary);
	//CString ComPrintLineFormatForTemplate(CString& PrintLine);


};
