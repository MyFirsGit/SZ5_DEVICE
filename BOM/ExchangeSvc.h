#pragma once
#include "ReceptionDlg.h"
#include "BOMForeService.h"

// ������ת��Ϣ
static const TCHAR* const WM_TO_EXCHANGE_BASE_DLG   = _T("WM_TO_EXCHANGE_BASE_DLG");   // �滻��������
static const TCHAR* const WM_TO_EXCHANGE_FINISH_DLG = _T("WM_TO_EXCHANGE_FINISH_DLG"); // �滻��������
static const TCHAR* const WM_TO_EXCHANGE_NOT_DLG    = _T("WM_TO_EXCHANGE_NOT_DLG");    // �滻���ɻ���
static const TCHAR* const WM_TO_EXCHANGE_WAIT_DLG   = _T("WM_TO_EXCHANGE_WAIT_DLG");   // �滻�ȴ���������

// �滻ȷ�϶Ի�������
static const TCHAR* const EXCHANGE_NEWCARD_DIALOG            = _T("ԭƱ�ѱ��������뽫�³�Ʊ�������ⲿ|IC��д���ĸ�Ӧ���ڲ���ȷ�ϼ�");
static const TCHAR* const EXCHANGE_NEWCARD_DIALOG2           = _T("���뽫�³�Ʊ�������ⲿ|IC��д���ĸ�Ӧ���ڲ���ȷ�ϼ�");

/**
@brief      �滻ҵ��Service
*/
class CExchangeSvc: public CBOMForeService
{
    DECLARE_MESSAGE_MAP()
public:
    // ������ID
    typedef enum _dialog_group{
        DIALOG_GROUP_WAIT = 1,
        DIALOG_GROUP_BASE,
        DIALOG_GROUP_FINISH,
        DIALOG_GROUP_EXCHANGE_NOT
    } DIALOG_GROUP;

    CExchangeSvc();
    ~CExchangeSvc();
	
	UINT InitExchangeInfo();                                               // ����վԱ������ʾ��Ϣ
	void OnReadCard(UINT errCode, CRWReadCard* rwReadCard); 	           // ��������
	void SetBaseDlg();                                                     // ����վԱ�˿ͻ���                                 
	UINT GetExchangeInfoSize();                                            // ���վԱ������ʾ��Ϣ����  
	PLABEL_GROUP GetExchangeInfo();                                        // ���վԱ������ʾ��Ϣ
	vector<MSG_NAME_GROUP>& GetReceptionInfo();                            // ��ó˿ͽ�����ʾ��Ϣ
	long BlockCard(RW_BUSSINESS_RESPONSE response);                        // ����    
	UINT SetReceptionInfo();                                               // ���ó˿ͽ����Ϣ���ݹ���
	void DoOldCardExchange();                                              // �滻�¿�ҵ��
	

private:
	// վԱ������Ϣ
	LABEL_GROUP m_ExchangeInfo[LABEL_GROUP_EDIT_ROWS_4];
	// �˿ͻ���
	vector<MSG_NAME_GROUP> m_TicketInfoGroup;                               // ��Ʊ��Ϣ��������
	//bool IsPrinterException();                                              // ����ӡ���Ƿ�����쳣 
	void PrinterPrint();                                                    // ��Ǯ�䲢��ӡ��Ϣ
	long DoNewCardExchange();
	bool SaveCardInfo(RW_READ_CARD_RESPONSE&);
	void SaveSCData();
	void StartReadCard(CString guidInfo=_T(""));
	
	CCPUCard* pCpuCard;	                                                     // cpu����Ϣ�ṹ��
	CULCard* pUlCard;                                                        // ul����Ϣ�ṹ��
	UINT  m_ExchangeInfoSize;                                                // վԱ������ʾ��Ϣ����  
	WORD m_TicketType;                                                       // ��Ʊ������     
	bool m_isOldCard;                                                        // �ɿ���־
	CString m_strOldNum;
	CString m_strNewNum;
	CString m_strDeposit;
	ProductCategory_t m_category;
	typedef struct cardInfo{
		CString cardNum;
		CString cardNameCn;
		CString cardNameEn;
		CString vilidateTime;
		CString balance;
		cardInfo(){
			cardNum = "--";
			cardNameCn = "--";
			cardNameEn = "--";
			vilidateTime = "--";
			balance = "--";
		}
	}CARDINFO;
	CARDINFO m_cardInfo;                                                       // ������ʾ��Ϣ����  
	
};
