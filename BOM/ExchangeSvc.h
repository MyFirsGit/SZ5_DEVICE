#pragma once
#include "ReceptionDlg.h"
#include "BOMForeService.h"

// 画面流转消息
static const TCHAR* const WM_TO_EXCHANGE_BASE_DLG   = _T("WM_TO_EXCHANGE_BASE_DLG");   // 替换基本画面
static const TCHAR* const WM_TO_EXCHANGE_FINISH_DLG = _T("WM_TO_EXCHANGE_FINISH_DLG"); // 替换结束画面
static const TCHAR* const WM_TO_EXCHANGE_NOT_DLG    = _T("WM_TO_EXCHANGE_NOT_DLG");    // 替换不可画面
static const TCHAR* const WM_TO_EXCHANGE_WAIT_DLG   = _T("WM_TO_EXCHANGE_WAIT_DLG");   // 替换等待读卡画面

// 替换确认对话框文言
static const TCHAR* const EXCHANGE_NEWCARD_DIALOG            = _T("原票已被锁定，请将新车票放置在外部|IC读写器的感应区内并按确认键");
static const TCHAR* const EXCHANGE_NEWCARD_DIALOG2           = _T("，请将新车票放置在外部|IC读写器的感应区内并按确认键");

/**
@brief      替换业务Service
*/
class CExchangeSvc: public CBOMForeService
{
    DECLARE_MESSAGE_MAP()
public:
    // 画面组ID
    typedef enum _dialog_group{
        DIALOG_GROUP_WAIT = 1,
        DIALOG_GROUP_BASE,
        DIALOG_GROUP_FINISH,
        DIALOG_GROUP_EXCHANGE_NOT
    } DIALOG_GROUP;

    CExchangeSvc();
    ~CExchangeSvc();
	
	UINT InitExchangeInfo();                                               // 设置站员画面显示信息
	void OnReadCard(UINT errCode, CRWReadCard* rwReadCard); 	           // 读卡处理
	void SetBaseDlg();                                                     // 设置站员乘客画面                                 
	UINT GetExchangeInfoSize();                                            // 获得站员画面显示信息行数  
	PLABEL_GROUP GetExchangeInfo();                                        // 获得站员画面显示信息
	vector<MSG_NAME_GROUP>& GetReceptionInfo();                            // 获得乘客界面显示信息
	long BlockCard(RW_BUSSINESS_RESPONSE response);                        // 锁卡    
	UINT SetReceptionInfo();                                               // 设置乘客金额信息数据构成
	void DoOldCardExchange();                                              // 替换新卡业务
	

private:
	// 站员画面信息
	LABEL_GROUP m_ExchangeInfo[LABEL_GROUP_EDIT_ROWS_4];
	// 乘客画面
	vector<MSG_NAME_GROUP> m_TicketInfoGroup;                               // 车票信息区域数据
	//bool IsPrinterException();                                              // 检查打印机是否存在异常 
	void PrinterPrint();                                                    // 打开钱箱并打印信息
	long DoNewCardExchange();
	bool SaveCardInfo(RW_READ_CARD_RESPONSE&);
	void SaveSCData();
	void StartReadCard(CString guidInfo=_T(""));
	
	CCPUCard* pCpuCard;	                                                     // cpu卡信息结构体
	CULCard* pUlCard;                                                        // ul卡信息结构体
	UINT  m_ExchangeInfoSize;                                                // 站员画面显示信息行数  
	WORD m_TicketType;                                                       // 车票子类型     
	bool m_isOldCard;                                                        // 旧卡标志
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
	CARDINFO m_cardInfo;                                                       // 界面显示信息缓存  
	
};
