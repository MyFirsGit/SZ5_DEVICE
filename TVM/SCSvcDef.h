#pragma once
#include "SCAPI.h"
#include "CTLOGCountInfo.h"
#include "CommonDefine.h"
#include "CTBanknoteCountInfo.h"

//------------------�����ǻ�����ȡƱҵ����Ϣ�Լ��ṹ---------------------------------------------//
typedef enum _return_code {
	RSP_CODE_OK						= 0x00,		// �ɹ�
	RSP_CODE_ORDER_NOT_EXSIT		= 0x01,		// ����������
	RSP_CODE_ORDER_IS_LOCKED		= 0x02,		// ����������
	RSP_CODE_ORDER_IS_USED			= 0x03,		// ������ʹ��
	RSP_CODE_ORDER_IS_CONFIRMED		= 0x04,		// ������ȷ��/����
	RSP_CODE_ORDER_LOCKE_FAIL		= 0x05,		// ��������ʧ��
	RSP_CODE_ORDER_CONFIRMED_FAIL	= 0x06,		// ����ȷ��ʧ��
	RSP_CODE_CONNECT_FAIL			= 0x07,		// ͨѶʧ��
	RSP_CODE_LOGIN_FAIL				= 0x08,		// ��¼ʧ��

	RSP_CODE_ORDER_REQUEST_FAILED	= 0x09,		// ��������ʧ��
	RSP_CODE_GENER_ERWEIMA_FAILED	= 0x0A,		// ���ɶ�ά��ʧ��
	RSP_CODE_ORDER_QUERY_FAILED		= 0x0B,		// ������ѯʧ��


	//RSP_CODE_ORDER_IS_LOCKED		= 0x02,		// ����������
	//RSP_CODE_ORDER_IS_USED			= 0x03,		// ������ʹ��
	//RSP_CODE_ORDER_IS_CONFIRMED		= 0x04,		// ������ȷ��/����
	//RSP_CODE_ORDER_LOCKE_FAIL		= 0x05,		// ��������ʧ��
	//RSP_CODE_ORDER_CONFIRMED_FAIL	= 0x06,		// ����ȷ��ʧ��
	//RSP_CODE_CONNECT_FAIL			= 0x07,		// ͨѶʧ��
	//RSP_CODE_LOGIN_FAIL				= 0x08		// ��¼ʧ��

}RETURN_CODE;
//OrderInfo��������Ϣ�ṹ�壩
typedef struct _orderinfo
{
	CString csOrderID;				// ������
	_DATE_TIME dtOrderTime;			// �µ�ʱ��
	BYTE bSaleMode;	// ��Ʊ��ʽ 
	//0x01������Ʊ����Ʊ��
	//0x02��������ֹվ����Ʊ
	BYTE bCardType;	// ��Ƭ����	  
	//0x01��һ��ͨCPU��Ƭ��
	//0x02��һ��ͨM1��Ƭ��
	//0x11��һƱͨCPU��Ƭ��
	//0x12��һƱͨUL���� 
	//0x13��һƱͨM1��Ƭ��
	WORD wProductType;			// ��Ʊ��Ʒ����			
	WORD wProductSubType;		// ��Ʊ��Ʒ������	
	DWORD dwSaleAmount;			// ��Ʊ���		
	BYTE bPaymentMethod;		// ֧����ʽ����	
	//0���ֽ�1����ֵƱ��2��һ��ͨƱ��3�����ÿ�
	WORD wBegainStation;		// ���վվ��		2	BCD	��ʼ��վID	0-9999������ʼվ��Ʊʱ�����룬������0.
	WORD wEndStation;			// �յ�վվ��		2	BCD	Ŀ�ĳ�վID	0-9999������ʼվ��Ʊʱ�����룬������0.
	WORD wOrderPiece;			// ��������
	WORD wRemainPiece;			// ʣ��ɶ�����
	_orderinfo(){
		csOrderID=_T("");
		bSaleMode = 0;
		bCardType = 0;
		wProductSubType = 0;
		wProductType = 0;
		dwSaleAmount = 0;
		bPaymentMethod = 0;
		wBegainStation = 0;
		wEndStation = 0;
		wOrderPiece = 0;
		wRemainPiece = 0;
	}
}ORDER_INFO;
//_order_card_info����WEBServer�ύƱ����Ϣ��
typedef struct _order_card_info{
	_DATE_TIME TranDateTime;			// ����ʱ��
	BYTE  CardPhysicalNumber[7];		// ��Ƭ���кţ�����7�ֽ�ǰ��0
	CardSerialNumber_t CardLogicalNumber;	  // ��Ʊ�߼����
	CString strTAC;						// ������֤��
	CString	strRWSeqNum;				// ��д���������к�
	WORD	wSaleAmount;				// ���׽��
	WORD	wFee;						// 
	WORD	wTickCnt;					// ���������
	DWORD	dwRWValueCnt;				// ��д���ۼƽ��׽��
	_DATE_TIME EffTime;					// Ʊ����Чʱ��
	_DATE_TIME ExpTime;					// Ʊ��ʧЧʱ��
	BYTE SamId[6];						// SAM��ID
	CString strSamSeqNum;				// SAM�����к�
	WORD	wPhyType;					// ��Ƭ��������
	WORD	wAppType;					// ��ƬӦ������
	_order_card_info(){
		TranDateTime = _date_time();
		memset(CardPhysicalNumber,0,7);
		CardLogicalNumber = 0;
		strTAC = _T("");
		strRWSeqNum = _T("");
		EffTime = _date_time();
		ExpTime = _date_time();
		memset(SamId,0,6);
		strSamSeqNum = _T("");
		wSaleAmount = 0;
		wFee = 0;
		wTickCnt = 0;
		dwRWValueCnt = 0;
		wPhyType = 0;
		wAppType = 0;
	}
}ORDER_CARD_INFO;

//OrderProcessResult������ִ�н���ṹ�壩
typedef struct _orderprocessresult
{
	CString csOrderID;				// ������
	WORD wExchangedPiece;			// �Ѷһ�����
	WORD wRemainPiece;				// ʣ��ɶ�����	
	vector<ORDER_CARD_INFO>vecCardInfo;//Ʊ����Ϣ

	_orderprocessresult(){
		csOrderID=_T("");
		vecCardInfo.clear();
	}
}ORDER_PROCESS_RESULT;

//--------------------------------------������TVM��Ϣ������붨��-----------------------------------------//
// TVM����������Ϣ���붨��
typedef enum _msg_code_transation
{
	MSG_TRANS_RECYLE_INVALID_CARD		= 0x5005,	//  ��Ʊ���ս���
	MSG_TRANS_DEVICE_IDENTIFICATION	= 0x0608,	//  ISAM�������Ԥ����
	MSG_TRANS_CD_VERSION_STATUS		= 0x0609,	//  CD�汾�л���Ԥ����
	MSG_TRANS_DEVICE_STATUS					= 0x060D,	//  �豸״̬�����Ԥ����
	MSG_TRANS_AUDIT_REGISTER					= 0x0501,	//  AR���ݣ�Ԥ����
} MSG_CODE_TRANS;

// TVMҵ���¼������������
typedef  enum _msg_code_bussiness{
	// ҵ������
	MSG_BUSSINESS_FAILURE_REPORT					= 0x5101,   // TVM/AVM���ϵ�
	MSG_BUSSINESS_SOFTANDPROGRAM_UPDATE				= 0x5102,   // �����ͳ������
	MSG_BUSSINESS_SOFTANDPROGRAM_DOWNLOAD			= 0x5103,   // �����ͳ�������
	MSG_BUSSINESS_DEVICE_IMPORTANT_STATUS_CHANGE	= 0x5104,   // �豸��Ҫ״̬�����������ĿԤ����	
	MSG_BUSSINESS_ISAM_LOGIN_LOGOUT					= 0x5105,   // ISAMǩ��/ǩ�ˣ�������ĿԤ����
	MSG_BUSSINESS_OPERATING_LOG						= 0x5106,   // ������־
	MSG_BUSSINESS_DEVICE_ERROR						= 0x5107,   // �豸����
	MSG_BUSSINESS_DEVICE_REGISTER					= 0x5108,   // �豸�Ĵ���
	MSG_BUSSINESS_MONEYBOX_CHANGE					= 0x5109,   // Ǯ�����
	MSG_BUSSINESS_TICKETBOX_CHANGE					= 0x510C,   // Ʊ�����
	// �¼�����
	MSG_BUSSINESS_DEVICE_COMPONENT					= 0x5114,   // �豸�������ɣ�������ĿԤ����
	MSG_BUSSINESS_ACCOUNT_LOCK						= 0x5201,   // �˻�����
	MSG_BUSSINESS_UDSN_RESET						= 0x5204,   // �豸��ˮ����
	MSG_BUSSINESS_DEVICE_COMPONENT_CHANGE			= 0x5205,   // ����������������ĿԤ����
	MSG_BUSSINESS_SAM_CHANGE						= 0x520E,   // SAM����
	MSG_BUSSINESS_MAGAZINE_INFO						= 0x5207,   // Ʊ���汨��
	MSG_BUSSINESS_MONEYBOX_INFO						= 0x5208,   // Ǯ���汨��
	MSG_BUSSINESS_TVM_AUDIT							= 0x520A,   // TVM�������
	MSG_BUSSINESS_MONEYBOX_EMPTY_REPORT				= 0x520C,   // Ǯ������¼�
	MSG_BUSSINESS_PACKAGE_AUDIT						= 0x520D,   // �������������	
}MSG_CODE_BUSSINESS;

// ��������---�������ݴ���
typedef  enum _msg_code_control_command{
	MSG_SPECIFY_BLOCKNO_DATA					= 0x5201,   // �ϴ�ָ���������������
	MSG_SPECIFY_TIME_DATA						= 0x5202,   // �ϴ�ָ��ʱ����������(Ԥ��)
	MSG_DEBUG_DATA								= 0x5203,   // �ϴ��豸��������(Ԥ��)
	MSG_OPERATION_DATA							= 0x5204,   // ��Ӫģʽ��������
	MSG_DEVICE_CONTROL_COMMAND					= 0x5205,   // �豸���п�������
	MSG_24HOUR_OPERATION_DATA					= 0x5206,   // 24Сʱ��Ӫ��������
	MSG_DELAY_OPERATION_DATA					= 0x5207,   // �ӳ���Ӫʱ������
	MSG_OPERATION_START_DATA					= 0x52FA,   // ��Ӫ��ʼ����
	MSG_OPERATION_END_DATA						= 0x52F9,   // ��Ӫ��������
	MSG_FORCE_TIME_SYNC							= 0x5208,   // ǿ��ʱ��ͬ������
	MSG_ISSUE_RESTRICT							= 0x520A,   // ��Ʊ��������
	MSG_UPLOAD_PARAMETER_ID						= 0x520C,   // �ϴ������ͳ���汾
	MSG_UPDATE_PARAMETER						= 0x520D,   // �����ͳ����������
	MSG_FORCE_LOGOUT							= 0x5211,   // ǿ���˳���¼(Ԥ��)
	MSG_CHANGE_PARAMETER_ID						= 0x5212,   // �����ͳ���汾�л�����(Ԥ��)
	MSG_REQUEST_DEVICE_STATUS					= 0x5213,   // �豸״̬��ѯ(Ԥ��)
	MSG_AUTORUN_PARAMETER						= 0x5215    // �Զ����в������ÿ���
}MSG_CODE_CONTROL_COMMAND;

// ��������---�������ݴ���
typedef  enum _msg_code_control_request{
	MSG_REQUEST_LOGIN_OR_LOGOUT						= 0x5101,   // ��¼�ǳ�����
	MSG_REQUEST_TVM_HELP							= 0x5102,   // TVM��Ԯ����
	MSG_REQUEST_REFORM_PASSWORD						= 0x5103,   // �����޸�����
	MSG_REQUEST_SEND_IMIDIATLY_PARAMETER			= 0x5107,   // ��ʱ��Ч�����·�����
	MSG_REQUEST_CONNECT_AUTH						= 0x5109,   // ������֤����
	MSG_REQUEST_PERSONALIZATION						= 0x5110,   // ���Ի���Ϣ��ѯ����(Ԥ��)
	MSG_REQUEST_HISTORYINFO							= 0x5111,   // ��ʷ������Ϣ��ѯ����(Ԥ��)
}MSG_CODE_CONTROL_REQUEST;

// ��������---�������ݴ���
typedef  enum _msg_code_control_reply{
	MSG_REQUEST_LOGIN_OR_LOGOUT_REPLY				= 0x5101,   // ��¼�ǳ�������
	MSG_REQUEST_TVM_HELP_REPLY						= 0x5102,   // TVM��Ԯ������
	MSG_MSG_REFORM_PASSWORD_REPLY					= 0x5103,   // �����޸�������
	MSG_REQUEST_IMIDIATLY_PARAMETER_REPLY			= 0x5107,   // ��ʱ��Ч�����·�������
	MSG_REQUEST_CONNECT_AUTH_REPLY					= 0x5109,   // ������֤������
	MSG_PARAMETER_ID_DATA							= 0x5401,   // �豸�����ͳ���汾����
	MSG_REQUEST_PERSONALIZATION_REPLY				= 0x5110,   // ���Ի���Ϣ��ѯ������(Ԥ��)
	MSG_REQUEST_HISTORYINFO_REPLY					= 0x5111    // ��ʷ������Ϣ��ѯ������(Ԥ��)
}MSG_CODE_CONTROL_REPLY;

// ��������---֪ͨ���ݴ���
typedef  enum _msg_code_control_notic{
	MSG_ACCOUNT_UNLOCK_NOTIC						= 0x5502,	// �˻�����֪ͨ
	MSG_FTP_CONFIG_NOTIC							= 0x550C,	// FTP������Ϣ֪ͨ
	MSG_OP_END_NOTIC								= 0x550E,	// ��������֪ͨ
}MSG_CODE_CONTROL_NOTIC;

// ״̬���ݴ���
typedef  enum _msg_code_status{
	MSG_DEVICE_ALL_STATUS_DATA					= 0x3101,   // �豸����״̬����
	MSG_DEVICE_STATUS_CHANGE_DATA				= 0x3102,   // �豸���״̬����
	MSG_DEVICE_ERROR_NOTIC						= 0x3107    // �豸����״̬����
}MSG_CODE_STATUS;

//--------------------------------------������TVM��Ϣ������붨��-----------------------------------------//

//--------------------------------------������TVM��Ϣ���ȱ��붨��-----------------------------------------//
#define LEN_AFC_SYS_HEADER 10
#define  LEN_TXN_DATATYPE 17
#define LEN_PACKAGE_HEADER 28
//6.3AFC�ڲ�����
//6.3.1BOM��TVM�˿�
#define  LEN_TXN_BOM_REFUND_TVM (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+19)
//6.3.2�Ǽ�ʱ�˿�����
#define  LEN_TXN_DELAY_REFUND (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+89)
//6.3.5��Ʊ���ս���
#define  LEN_TXN_INVALID_TICKET_RECYCLE (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+20)
//6.3.6BOM��Ӫҵ����֧
#define  LEN_TXN_BOM_NONBUSINESS_INCOME (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+84)
#define LEN_TXN_SYSCOM_HDR 42
#define LEN_TXN_CARDCOM_HDR 20
#define LEN_TXN_CARDHOLDERCOM_HDR 16
#define LEN_TXN_SYSSECURITY_HDR 12
#define LEN_TXN_TICKETCOM_HDR 17
#define LEN_TXN_OPERATORCOM_HDR  4
#define LEN_TXN_FINCOM_HDR 13
#define LEN_TXN_SERCOM_HDR  5

#define LEN_TXN_NONIMMIDATELY_SURRENDER (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR +LEN_TXN_FINCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 15 +LEN_TXN_SERCOM_HDR)

#define LEN_TXN_SIGNCARD_APPLY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_OPERATORCOM_HDR+ 375+LEN_TXN_SERCOM_HDR)
#define LEN_TXN_SIGNCARD_MODIFY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_OPERATORCOM_HDR+ 375+LEN_TXN_SERCOM_HDR)
//#define LEN_TXN_REPORT_LOSS (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_CARDCOM_HDR + 32 + LEN_TXN_SYSSECURITY_HDR) // ͬLEN_TXN_BLACKLIST_CARD_REQUEST
#define LEN_TXN_CANCEL_REPORT_LOSS (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_OPERATORCOM_HDR+ 42+LEN_TXN_SERCOM_HDR)

#define LEN_TXN_ADMINISTRATION_PROCESS (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_FINCOM_HDR + LEN_TXN_OPERATORCOM_HDR+ 22+LEN_TXN_SERCOM_HDR)
#define LEN_TXN_NONIMMIDATELY_SURRENDER_APPLY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 105 + LEN_TXN_SERCOM_HDR)
#define LEN_TXN_NONIMMIDATELY_SURRENDER_APPLY_QUERY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 31 + LEN_TXN_SERCOM_HDR)
#define LEN_TXN_SIGNCARD_APPLY_QUERY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 21 + LEN_TXN_SERCOM_HDR)

//#define LEN_TXN_REPORT_LOSS_REQUEST (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_CARDCOM_HDR+32) // ��LEN_TXN_BLACKLIST_CARD_REQUEST

#define LEN_TXN_DEVICE_IDENTIFICATION	(LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_SYSSECURITY_HDR)
#define LEN_TXN_CD_VERSION_STATUS		(LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + 408 + LEN_TXN_SYSSECURITY_HDR)
#define LEN_TXN_BLACKLIST_CARD_REQUEST	(LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_CARDCOM_HDR + 32 + LEN_TXN_SYSSECURITY_HDR)
#define LEN_TXN_DEVICE_STATUS			(LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + 4 + LEN_TXN_SYSSECURITY_HDR)
#define LEN_TXN_AUDIT_REGISTER			(LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + 2408 + LEN_TXN_SYSSECURITY_HDR)

#define LEN_BUSINESS_HEADER 32
#define INX_BUSINESS_DATATYPE (LEN_BUSINESS_HEADER-7)
#define INX_BUSINESS_HEADER_LENGTH (LEN_BUSINESS_HEADER-5)
#define LEN_BUSINESS_HEADER_DATATYPE (LEN_BUSINESS_HEADER-5)
//6.4ҵ������
// TVM��AVM���ϵ�ҵ������
#define LEN_BUSINESS_FAILED_DATA (LEN_BUSINESS_HEADER + 35)
//6.4.2�����ͳ������
#define LEN_BUS_PARAMETER_SOFT_UPDATE_BASE (LEN_BUSINESS_HEADER+2)
//#6.4.3�����ͳ�������ҵ������
#define LEN_BUS_PARAMETER_SOFT_DOWNLOAD_BASE (LEN_BUSINESS_HEADER+1)
//6.4.4�豸��Ҫ״̬�ı�
#define LEN_BUS_DEVICE_IMPORTANT_STATUS_CHANG (LEN_BUSINESS_HEADER+4)
//6.4.5ISAMǩ��ǩ��
#define LEN_BUS_ISAM_LOGIN_LOGOUT (LEN_BUSINESS_HEADER+16)
//6.4.6������־
#define LEN_BUS_OPERATING_LOG (LEN_BUSINESS_HEADER + 68)
//6.4.7�豸��������
#define LEN_BUS_DEVICE_ERROR (LEN_BUSINESS_HEADER + 13)
//6.4.8�豸�Ĵ�������
#define LEN_BUS_DEVICE_REGISTER_BASE (LEN_BUSINESS_HEADER + 3)
#define LEN_BUS_DEVICE_REGISTER (LEN_BUSINESS_HEADER + 3 + 6 * 314)
//6.4.10Ʊ�����
#define LEN_BUS_TICKETBOX_CHANGE (LEN_BUSINESS_HEADER + 18)
//ֽ�������
#define LEN_BUS_BANKNOTE_BOX_CHANGE (LEN_BUSINESS_HEADER + 41)
//6.4.11�豸��������
#define COUNT_DEVICE_COMPONET 3
#define LEN_BUS_DEVICE_COMPONENT (LEN_BUSINESS_HEADER + 2 + 7*COUNT_DEVICE_COMPONET)
//6.5�¼�����
//6.5.1�˻�����
#define LEN_EVN_ACCOUNT_LOCK (LEN_BUSINESS_HEADER)
//6.5.3�豸��ˮ������
#define LEN_EVN_UDSN_RESET (LEN_BUSINESS_HEADER + 2 + 1*9)
//6.5.4��������
#define LEN_EVN_DEVICE_COMPONENT_CHANGE (LEN_BUSINESS_HEADER+13)
//6.5.5Ʊ���汨��
#define LEN_EVN_TICKETBOX_INFO_BASE  (LEN_BUSINESS_HEADER + 3)
#define LEN_EVN_TICKETBOX_INFO (LEN_BUSINESS_HEADER + 3 + 7*6)
//AVMǮ���汨��(ֽ����1)
#define LEN_EVN_AVM_MONEY_BOX (LEN_BUSINESS_HEADER + 4 + 1 + 4 + 1 + 1 + (1 + 2) * 6 + 1)
//TVMǮ���汨��(ֽ����1+ֽ��ѭ��������4 + ֽ��������1 + Ӳ����2 + ѭ������2 + ������2)
#define LEN_EVN_TVM_MONEY_BOX (LEN_BUSINESS_HEADER + 4 + 1 + 24 + 36 + 9 + 36 + 24)
//ֽ�������Ǯ��
#define LEN_EVN_CLEAR_BANKNOTES_INFO (LEN_BUSINESS_HEADER + 4 + 1 + 3 * 6 + 5)
//ֽ�����������
#define LEN_EVN_CLEAR_BANKNOTES_CHANGE_BOX_INFO (LEN_BUSINESS_HEADER + 4 + 1 + 3*2 + 5)
//Ӳ�������Ǯ��
#define LEN_EVN_CLEAR_COINS_INFO (LEN_BUSINESS_HEADER + 4 + 1 + 3*2 + 4 + 1 + 1 + 2 + 1 + 2*3 + 1 + 2 + 1 + 2)
//TVM����¼�
#define LEN_EVN_AVM_TVM_AUDIT (LEN_BUSINESS_HEADER + 14 + 78)
//6.6״̬����
#define LEN_STATUS_HEADER 11
#define LEN_STATUS_SINGLE_MODE (LEN_STATUS_HEADER + 2 + 4 * 1) // ״̬���ݳ���(����ģʽ/24Сʱ��Ӫ/�ӳ�ģʽ
#define LEN_STATUS_THREE (LEN_STATUS_HEADER + 2 + 4*3) // ����ģʽ + 24Сʱ��Ӫ + �ӳ�ģʽ
#define LEN_STATUS_FULL (LEN_STATUS_HEADER + 2 + 4 * 16 )  // ȫ״̬����
#define LEN_STATUS_TVM_FULL (LEN_STATUS_HEADER + 2 + 4 * 39) // TVMȫ״̬����
#define LEN_STATUS_AVM_FULL (LEN_STATUS_HEADER + 2 + 4 * 26) // AVMȫ״̬����
#define LEN_STATUS_HAS_ERROR (LEN_STATUS_HEADER + 7) // �쳣״̬���ĳ���
#define LEN_STATUS_CHANGE (LEN_STATUS_HEADER+4) //6.6.6�豸״̬�������

#define LEN_REQ_HEADER 2
//6.7.2��������
//6.7.2.1��½�ǳ�
#define LEN_REQ_LOGINLOGOFF  (LEN_REQ_HEADER+12)
//6.7.2.3�����޸�
#define LEN_REQ_CHANGE_PASSWORD (LEN_REQ_HEADER+17)

#define LEN_REQ_PERSONALIZATION (LEN_REQ_HEADER + 5 + 61 + 1 + 4 + 21 + 21 + 129 + 21 + 16 + 10 + 4 + 10 + 1 + 1 + 41 + 41 + 41 + 41 + 41 + 19 + 1)

//6.7.2.5�����޸�
//6.7.2.6��ʱ��Ч�����·�����
#define LEN_REQ_SEND_IMMIDIATLY_PARAMETER (LEN_REQ_HEADER+6)
//6.7.2.8������֤����
#define  LEN_REQ_CONNECT_AUTH (LEN_REQ_HEADER+11)

// ��Ԯ����
#define LEN_REQ_CALL_HELP (LEN_REQ_HEADER + 14)

//6.7.3֪ͨ����
//6.7.3.13��������֪ͨ
#define  LEN_NOTICE_OPERATION_END 4

//6.7.4��������
#define LEN_REPLY_PARAMETER_VERSION (10+15*40)

//--------------------------------------������TVM��Ϣ���ȱ��붨��-----------------------------------------//

// ����������
typedef enum _transfer_error{
	ERR_PROTOCOL_IDENTIFY        = 0x0001,   // Э���ʶ����
	ERR_PROTOCOL_VERSION         = 0x0002,   // Э��汾�Ŵ���
	ERR_DATA_FORMAT_VERSION      = 0x0003,   // ���ݸ�ʽ�汾�Ŵ���
	ERR_PROTOCOL_COMMAND         = 0x0004,   // Э���������
	ERR_DATA_TYPE                = 0x0005,   // �������ͱ������
	ERR_CRC_TYPE                 = 0x0006,   // У�����㷨����
	ERR_CRC_DATA                 = 0x0007,   // У�������
	ERR_BLOCK_DATA               = 0x0008,   // �ְ����ݴ���
	ERR_ECRYPT                   = 0x0009,   // �����㷨����
	ERR_KEY_VERSION              = 0x000A,   // ��Կ�汾����
	ERR_SAME_DATAD               = 0x000B,   // �ظ���Ϣ
	ERR_PACKAT_LENGTH            = 0x00FD,   // ���峤�ȴ���
	ERR_OTHER                    = 0x00FE    // ��������
}TRANSFER_ERROR;

// ��������
typedef enum _operation_code{
	OPERATION_LOGIN          = 0x1001, // ��¼(TVM)
	OPERATION_LOGOUT         = 0x1002, // �ǳ�(TVM)
	OPERATION_INPUT_PARAM    = 0x1101, // ��������ͳ���(TVM)
	OPERATION_OUTPUT_PARAM   = 0x1102, // ���������ͳ���
	OPERATION_INPUT_DEAL     = 0x1103, // ���뽻������
	OPERATION_OUTPUT_DEAL    = 0x1104, // ������������(TVM)
	OPERATION_INPUT_OPER     = 0x1105, // ����ҵ������
	OPERATION_OUTPUT_OPER    = 0x1106, // ����ҵ������(TVM)
	OPERATION_INPUT_DEBUG    = 0x1107, // �����������
	OPERATION_OUTPUT_DEBUG   = 0x1108, // ������������(TVM)
	OPERATION_FORCE_TIME     = 0x1411, // ǿ��ʱ��ͬ��
	OPERATION_MANUAL_TIME    = 0x1412, // �ֹ�ʱ��ͬ��
	OPERATION_24HOUR_DELAY   = 0x1701, // 24Сʱ��Ӫ���ӳ���Ӫ����
	OPERATION_DEBUG_UPLOAD   = 0x1702, // ������־�ϴ�(TVM)
	OPERATION_PARAM_EDIT     = 0x1A01, // �����༭(�༭���������·���
	OPERATION_MODE_SET       = 0x1901, // ģʽ�趨���˹������豸ģʽ��
	OPERATION_MODE_NOTIC     = 0x1902, // ģʽ֪ͨ�趨(TVM)
	OPERATION_TIME_WARNING   = 0xA101, // ʱ��ͬ������(TVM)
	OPERATION_TIME_ERROR     = 0xA102, // ʱ��ͬ���쳣(TVM)
	OPERATION_CONNECT_STATUS = 0xA103,  // ����״̬(TVM)
	OPERATION_SERVICE_STATUS_SETTINGS     = 0xA401,// ����ģʽ����(TVM)	     
	OPERATION_AISLEDIRECTION_SETTINGS    = 0xA402,// ͨ����������	    
	OPERATION_FLAP_STATUS_SETTINGS        = 0xA403,// բ��ģʽ����	    
	OPERATION_DEVICE_SETTINGS            = 0xA404,// ��������	(TVM)        
	OPERATION_NETWORK_SETTINGS           = 0xA405,// ͨѶ����	 (TVM)       
	OPERATION_DATETIME_SETTINGS       = 0xA406,// ��������(TVM)	        
	OPERATION_COM_SETTINGS               = 0xA407,// ��������	        
	OPERATION_FLAPOPENCLOSESPEED       = 0xA408,// ���ſ���ʱ������	
	OPERATION_VOLUME_SETTINGS            = 0xA409,// �豸��������  	  
	OPERATION_REMOVE_ERROR               = 0xA40A,// �쳣���(TVM)	        
	OPERATION_BALANCE_PROCESSING         = 0xA40B,// ҵ�����(TVM)	        
	OPERATION_SHUTDOWN                  = 0xA40C,// �ػ�(TVM)	            
	OPERATION_RESTART                   = 0xA40D,// ����(TVM)	            
	OPERATION_NETWORK_TEST               = 0xA501,// �������(TVM)	        
	OPERATION_RW_TEST                   = 0xA502,// ����������	(TVM)      
	OPERATION_COIN_UNIT_TEST              = 0xA503,// Ӳ��ģ�����(TVM)	    
	OPERATION_BANKNOTE_UNIT_TEST          = 0xA504,// ֽ��ģ�����	(TVM)    
	OPERATION_TOKEN_UNIT_TEST             = 0xA505,// TOKENģ�����(TVM)	    
	OPERATION_CARD_UNIT_TEST         = 0xA506,// Cardģ��(TVM)	        
	OPERATION_KEYBOARD_TEST              = 0xA507,// ά�����̲���	    
	OPERATION_DISPLAY_TEST               = 0xA508,// ��Ļ����	        
	OPERATION_SOUND_TEST                 = 0xA509,// ��������	        
	OPERATION_IO_UNIT_TEST                = 0xA50A,// IO������(TVM)	        
	OPERATION_TOKEN_RECYCLE_UNIT_TEST      = 0xA50B,// ���ղ�����	      
	OPERATION_PASSENGER_CONTROLUNIT_TEST  = 0xA50C,// ͨ���߼�����	    
	OPERATION_BANKCARD_UNIT_TEST          = 0xA50D,// ���п�ģ�����(TVM)    
	OPERATION_PINPAD_UNIT_TEST            = 0xA50E,// ������̲���(TVM)	    
	OPERATION_PRINTER_UNIT_TEST           = 0xA50F,// ��ӡģ�����(TVM)	    
	OPERATION_REPRINT_RECEIPT            = 0x1D01,// ƾ֤����(TVM)	        
	OPERATION_CARDBOX_REPLACEMENT        = 0xA601,// CardƱ�����(TVM)	    
	OPERATION_CARDBOX_INSTALLATION       = 0xA602,// CardƱ�䰲װ(TVM)	    
	OPERATION_CARDBOX_REMOVAL            = 0xA603,// CardƱ��ж��(TVM)	    
	OPERATION_TOKENBOX_REPLACEMENT       = 0xA701,// TokenƱ�����(TVM)	    
	OPERATION_TOKENBOX_INSTALLATION      = 0xA702,// TokenƱ�䰲װ(TVM)	    
	OPERATION_TOKENBOX_REMOVAL           = 0xA703,// TokenƱ��ж��(TVM)	    
	OPERATION_TOKEN_RECYCLE              = 0xA704,// Token����(TVM)	        
	OPERATION_COINBOX_REPLACEMENT        = 0xA801,// Coin���滻(TVM)	      
	OPERATION_COINBOX_INSTALLATION       = 0xA802,// Coin�䰲װ(TVM)	      
	OPERATION_COINBOX_REMOVAL            = 0xA803,// Coin��ж��(TVM)	      
	OPERATION_COIN_RECYCLE               = 0xA804,// Coin�����(TVM)	   
	OPERATION_COIN_CHANGE_INSTALL		 = 0xA805,// Coin�����䰲װ
	OPERATION_COIN_CHANGE_REMOVAL		 = 0xA806,// Coin������ж��
	OPERATION_BANKNOTEBOX_REPLACEMENT    = 0xA901,// ֽ�������(TVM)	      
	OPERATION_BANKNOTEBOX_INSTALLATION   = 0xA902,// ֽ���䰲װ(TVM)	      
	OPERATION_BANKNOTEBOX_REMOVAL        = 0xA903,// ֽ����ж��(TVM)	 
	OPERATION_CHANGE_BOX_INSTALLATION	 = 0xA904,	// ֽ�������䰲װ
	OPERATION_CHANGE_BOX_REMOVAL		 = 0xA905,	// ֽ��������ж��
	OPERATION_INVALID_BOX_INSTALLATION	 = 0xA906,	// �ϱһ����䰲װ
	OPERATION_INVALID_BOX_REMOVAL		 = 0xA907,	// �ϱһ�����ж��    
}OPERATION_CODE;

// ��������
typedef enum _operation_level{
	OPERATION_ONE_LEVEL   = 0x01, // ��ͨ����
	OPERATION_TWO_LEVEL   = 0x02, // ������Ϣ 
	OPERATION_THREE_LEVEL = 0x03, // ��Ҫ����
	OPERATION_FOUR_LEVEL  = 0x04, // ������Ϣ
	OPERATION_FIVE_LEVEL  = 0x05  // Σ�ղ���
}OPERATION_LEVEL;

// �豸�����������
typedef enum{
	DEVICE_CONTRL_CODE_SC_UNKNOW										= 0x0000, //δ֪
	DEVICE_CONTRL_CODE_SC_POWER_OFF										= 0x0101, //��Դ�ر�
	DEVICE_CONTRL_CODE_SC_RESTART_TVM									= 0x0102, //��������
	DEVICE_CONTRL_CODE_SC_OPERATION_START								= 0x0103, //��Ӫ��ʼ
	DEVICE_CONTRL_CODE_SC_OPERATION_END									= 0x0104, //��Ӫ����
	DEVICE_CONTRL_CODE_SC_SLEEP											= 0x0105, //˯��ģʽ
	DEVICE_CONTRL_CODE_SC_WAKEUP										= 0x0106, //Զ�̻���
	DEVICE_CONTRL_CODE_SC_START_SERVICE									= 0x0107, //��ʼ����
	DEVICE_CONTRL_CODE_SC_OUT_OF_SERVICE								= 0x0108, //��ͣ����
	DEVICE_CONTRL_CODE_SC_RESTART_SOFTWARE								= 0x0109, //�������
	DEVICE_CONTRL_CODE_SC_DE_OPER_ON									= 0x0301, //�������п�
	DEVICE_CONTRL_CODE_SC_DE_OPER_OFF									= 0x0302, //�������й�
	DEVICE_CONTRL_CODE_SC_BANKNOTE_ONLY									= 0x0303, //ֻ��ֽ��
	DEVICE_CONTRL_CODE_SC_COIN_ONLY										= 0x0304, //ֻ��Ӳ��
	DEVICE_CONTRL_CODE_SC_NO_CHANGE										= 0x0305, //������ģʽ
	DEVICE_CONTRL_CODE_SC_ONLY_SVT_ISSUE								= 0x0306, //ֻ��ֵ����Ʊģʽ
	DEVICE_CONTRL_CODE_SC_NOT_ISSUE										= 0x0308, //���ܷ��۵���Ʊģʽ
	DEVICE_CONTRL_CODE_SC_ONLY_ANALY									= 0x0309, //ֻ�鿨
	DEVICE_CONTRL_CODE_SC_NOT_SVT										= 0x030A, //���ܽ��մ�ֵ��
	DEVICE_CONTRL_CODE_SC_NOT_COIN										= 0x030B, //�ݲ���Ӳ��ģʽ
	DEVICE_CONTRL_CODE_SC_NOT_BANKNOTE									= 0x030C, //�ݲ���ֽ��ģʽ
	DEVICE_CONTRL_CODE_SC_RECOVER_NORMAL_MODE							= 0x030D, //�ָ�����ģʽ
	DEVICE_CONTRL_CODE_SC_NO_PRINT										= 0x030E, //�޴�ӡģʽ//��δ���壬����Ϊ�˱���
	// ����������ģʽ����
	DEVICE_CONTRL_CODE_NO_CHANGE									= 0x0800, //������
	DEVICE_CONTRL_CODE_NO_COIN_CHANGE								= 0x0801, //��ֽ�����㣬��Ӳ������
	DEVICE_CONTRL_CODE_NO_BILL_CHANGE								= 0x0802, //��Ӳ�����㣬��ֽ������
	DEVICE_CONTRL_CODE_ALL_CHANGE									= 0x0803, // ȫ��
	// ������ҵ��ģʽ��϶���
	DEVICE_CONTRL_CODE_BUSINESS										= 0x0600,  //  ҵ��ģʽ�����
	DEVICE_CONTRL_CODE_BUSINESS_CARD								= 0x0601,  //  �ۿ�
	DEVICE_CONTRL_CODE_BUSINESS_CHARGE								= 0x0602,  //  ��ֵ
	DEVICE_CONTRL_CODE_BUSINESS_QUERY								= 0x0604,  //  ��ѯ
	DEVICE_CONTRL_CODE_BUSINESS_ALL									= 0x0607,  //  ȫ��
	//DEVICE_CONTRL_CODE_BUSINESS_TOKEN								= 0x0610,  //  ��Token(Ԥ��)
	//DEVICE_CONTRL_CODE_BUSINESS_PRODUCT							= 0x0604,  //  �۲�Ʒ(Ԥ��)

	// ������֧��ģʽ��϶���
	DEVICE_CONTRL_CODE_PAYMENT										= 0x0700, // ֧��ģʽ�����
	DEVICE_CONTRL_CODE_PAYMENT_COIN									= 0x0701, // ��Ӳ��
	DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE								= 0x0702, // �ռ���
	DEVICE_CONTRL_CODE_PAYMENT_SVT									= 0x0704, // �մ�ֵ��
	DEVICE_CONTRL_CODE_PAYMENT_ALL									= 0x0707, // ȫ�գ�����ΪԤ����
	//DEVICE_CONTRL_CODE_PAYMENT_CREDIT_CARD						= 0x0710, // ���ÿ�
	//DEVICE_CONTRL_CODE_PAYMENT_BANKCARD							= 0x0702, // �����п�
	//DEVICE_CONTRL_CODE_PAYMENT_EP									= 0x0701, // ��EP
}DEVICE_CONTRL_CODE;

// Ǯ��Ʊ��λ��
typedef enum _magazine_palace{
						// Ǯ��              Ʊ��
	MAGAZINEA  = 0x01,  // ֽ�һ���			��Ʊ��
	MAGAZINEB  = 0x02,  // Ӳ������B		BƱ��
	MAGAZINEC  = 0x03,  // Ӳ������A		AƱ��
	MAGAZINED  = 0x04,	// Ӳ��HopperB		HopperB
	MAGAZINEE  = 0x05,	// Ӳ��HopperA		HopperA
	MAGAZINEF  = 0x06,	// Ӳ�һ�����B		������
	MAGAZINEG  = 0x07,	// Ӳ�һ�����A
	MAGAZINEH  = 0x08,	// ֽ�Ҳ�����
	MAGAZINEI  = 0x09,	// ֽ������A
	MAGAZINEJ  = 0x0A,	// ֽ������B
}MAGAZINE_PALACE;

// Ʊ������
typedef enum _magazine_type{
	MAGAZINE_NORMAL = 0x01, // ����Ʊ��	0x01
	MAGAZINE_CYCLE  = 0X02  // ��Ʊ��	0x02
}_MAGAZINE_TYPE;

// Ʊ��״̬
typedef enum _sc_magazine_status{
	VALID_ATTACH   = 0x01, // ������װ��
	INVALID_ATTCH  = 0x02, // �Ƿ���װ��
	VALID_DETACH   = 0x03, // ����ж�أ�
	INVALID_DETACH = 0x04  // �Ƿ�ж��
}SC_MAGAZINE_STATUS;


//--------------------------------------������TVM״̬���붨��-----------------------------------------//
typedef struct _tagTVM_STATUS_ID
{
	BYTE b1;
	BYTE b2;
	BYTE b3;
} TVM_STATUS_ID;

namespace TVM_STATUS_IDS
{
	const static _tagTVM_STATUS_ID   SERVICE_STATUS = {0x01,0x01,0x01};			// ����״̬
	const static _tagTVM_STATUS_ID   RUN_STATUS		= {0x01,0x01,0x02};			// ����״̬
	const static _tagTVM_STATUS_ID   WORK_MODE		= {0x01,0x01,0x03};         // ����ģʽ
	const static _tagTVM_STATUS_ID   OPERATION_MODE = {0x01,0x01,0x04};         // ��Ӫģʽ���μ�7.9��Ӫģʽ���롣
	const static _tagTVM_STATUS_ID   HOUR24MODE		= {0x01,0x01,0x05};         // 24Сʱ��Ӫģʽ
	const static _tagTVM_STATUS_ID   DELAY_RUN_MODE = {0x01,0x01,0x06};         // �ӳ���Ӫʱ��
	const static _tagTVM_STATUS_ID   CONNECT_STATUS = {0x01,0x01,0x07};         // ����״̬
	const static _tagTVM_STATUS_ID   TIME_DELAY		= {0x01,0x01,0x08};         // ʱ�Ӳ���
	const static _tagTVM_STATUS_ID   LOGIN_STATUS	= {0x01,0x01,0x09};         // ��¼״̬
	const static _tagTVM_STATUS_ID   DEVICE_AUTO_RUN_STATUS = {0x01,0x01,0x0A}; // �豸�Զ�����״̬	
	const static _tagTVM_STATUS_ID   DEVICE_ERR_STATUS = {0x01,0x01,0x0B};      // �豸����״̬״̬

	const static _tagTVM_STATUS_ID   CARD_BOX1_STATUS		= {0x06,0x01,0x01};			// ��ʽƱ��1��װ״̬
	const static _tagTVM_STATUS_ID   CARD_BOX1_COUNT		= {0x06,0x01,0x02};			// ��ʽƱ��1����״̬
	const static _tagTVM_STATUS_ID   CARD_BOX2_STATUS		= {0x06,0x02,0x01};			// ��ʽƱ��2��װ״̬
	const static _tagTVM_STATUS_ID   CARD_BOX2_COUNT		= {0x06,0x02,0x02};			// ��ʽƱ��2����״̬
	const static _tagTVM_STATUS_ID   CARD_CYCLEBOX_STATUS	= {0x07,0x01,0x01};			// ��ʽ��Ʊ�䰲װ״̬
	const static _tagTVM_STATUS_ID   CARD_CYCLEBOX_COUNT	= {0x07,0x01,0x02};			// ��ʽ��Ʊ������״̬

	const static _tagTVM_STATUS_ID   COIN_CHANGE_BOX1_STATUS = {0x09,0x01,0x01};		// Ӳ�Ҳ�����1��װ״̬
	const static _tagTVM_STATUS_ID   COIN_CHANGE_BOX1_COUNT  = {0x09,0x01,0x02};		// Ӳ�Ҳ�����1����״̬
	const static _tagTVM_STATUS_ID   COIN_CHANGE_BOX2_STATUS = {0x09,0x02,0x01};		// Ӳ�Ҳ�����2��װ״̬
	const static _tagTVM_STATUS_ID   COIN_CHANGE_BOX2_COUNT  = {0x09,0x02,0x02};		// Ӳ�Ҳ�����2����״̬
	const static _tagTVM_STATUS_ID   COIN_CYCLE_BOX1_STATUS  = {0x0A,0x01,0x01};		// Ӳ�һ�����1��װ״̬
	const static _tagTVM_STATUS_ID   COIN_CYCLE_BOX1_COUNT   = {0x0A,0x01,0x02};		// Ӳ�һ�����1����״̬
	const static _tagTVM_STATUS_ID   COIN_CYCLE_BOX2_STATUS  = {0x0A,0x02,0x01};		// Ӳ�һ�����2��װ״̬
	const static _tagTVM_STATUS_ID   COIN_CYCLE_BOX2_COUNT   = {0x0A,0x02,0x02};		// Ӳ�һ�����2����״̬
	const static _tagTVM_STATUS_ID   COIN_HOPPER_BOX1_COUNT  = {0x0D,0x01,0x01};		// Ӳ��Hopper������״̬
	const static _tagTVM_STATUS_ID   COIN_HOPPER_BOX2_COUNT  = {0x0D,0x02,0x01};		// Ӳ��Hopper������״̬
	//const static _tagTVM_STATUS_ID   COIN_HOPPER_BOX1_STATUS = {0x09,0x03,0x01};		// Ӳ��Hopper�䰲װ״̬���ޣ�
	//const static _tagTVM_STATUS_ID   COIN_HOPPER_BOX2_STATUS = {0x09,0x04,0x01};		// Ӳ��Hopper�䰲װ״̬���ޣ�
	//const static _tagTVM_STATUS_ID   COIN_CYCLE_HOPPER_BOX_STATUS = {0x09,0x05,0x01};	// Ӳ��ѭ������Hopper�䰲װ״̬(��)
	//const static _tagTVM_STATUS_ID   COIN_CYCLE_HOPPER_BOX_COUNT = {0x0D,0x03,0x01};	// Ӳ��ѭ������Hopper������״̬	

	const static _tagTVM_STATUS_ID	 BANKNOTE_CHANGE_BOX1_STATUS = {0x0B,0x01,0x01};	// ֽ��������1��װ״̬
	const static _tagTVM_STATUS_ID	 BANKNOTE_CHANGE_BOX1_COUNT	 = {0x0B,0x01,0x02};	// ֽ��������1����
	const static _tagTVM_STATUS_ID	 BANKNOTE_CHANGE_BOX2_STATUS = {0x0B,0x02,0x01};	// ֽ��������2��װ״̬��Ԥ����
	const static _tagTVM_STATUS_ID	 BANKNOTE_CHANGE_BOX2_COUNT	 = {0x0B,0x02,0x02};	// ֽ��������2������Ԥ����
	const static _tagTVM_STATUS_ID   BANKNOTE_CYCLE_BOX_STATUS   = {0x0C,0x01,0x01};	// ֽ�һ�����1��װ״̬
	const static _tagTVM_STATUS_ID   BANKNOTE_CYCLE_BOX_COUNT    = {0x0C,0x01,0x02};	// ֽ�һ�����1����״̬

	const static _tagTVM_STATUS_ID   SECURITY_DOOR_STATUS	= {0x0E,0x01,0x01};			// �ֽ�ȫ��״̬
	const static _tagTVM_STATUS_ID   TOUCH_SCREEN_STATUS	= {0x0F,0x01,0x01};			// ������״̬

	//const static _tagTVM_STATUS_ID   HELP_BUTTON_STATUS		= {0x11,0x01,0x01};			// ��Ԯ��ť״̬

	const static _tagTVM_STATUS_ID   RW1_STATUS				= {0x13,0x01,0x01};			// ��д��1״̬
	const static _tagTVM_STATUS_ID	 RW2_STATUS				= {0x13,0x02,0x01};			// ��д��2״̬��Ԥ����


	const static _tagTVM_STATUS_ID   RW_SAM1_TYPE			= {0x14,0x01,0x01};			// SAM1���ͣ�01: LRT SAM Normal�� 02: Macau Pass SAM Normal�� 03: LRT SAM Failure�� 04: Macau Pass SAM Failure��
	const static _tagTVM_STATUS_ID	 RW_SAM2_TYPE			= {0x14,0x02,0x01};			// SAM2����
	const static _tagTVM_STATUS_ID	 RW_SAM_1_STATUS		= {0x14,0x01,0x02};			// SAM1״̬
	const static _tagTVM_STATUS_ID	 RW_SAM_2_STATUS		= {0x14,0x02,0x02};			// SAM2״̬
	const static _tagTVM_STATUS_ID   PRINTER_STATUS			= {0x15,0x01,0x01};			// ��ӡ��
	
	const static _tagTVM_STATUS_ID   DOOR_STATUS			= {0x1C,0x01,0x01};			// ά�����ۺ�״̬
	const static _tagTVM_STATUS_ID   BANKNOTE_ACCEPT_MODULE_STATUS = {0x1E,0x01,0x01};  // ֽ�ҽ���ģ��״̬
	const static _tagTVM_STATUS_ID	 BANKNOTE_CHANGE_MODULE_STATUS = {0x1F,0x01,0x01};	// ֽ������ģ��״̬
	const static _tagTVM_STATUS_ID   CARD_MODULE_STATUS		= {0x20,0x01,0x01};			// ��ʽ����ģ��
	const static _tagTVM_STATUS_ID   COIN_MODULE_STATUS		= {0x21,0x01,0x01};			// Ӳ��ģ��״̬
	const static _tagTVM_STATUS_ID   STATUS_DISPLAY_STATUS  = {0x23,0x01,0x01};			// ״̬��ʾ��ģ��	
	const static _tagTVM_STATUS_ID   DEVICE_TRANSFER_AUDIT = {0x01,0x01,0x0C};			// �������״̬


	/*const static _tagTVM_STATUS_ID   PAYMENT_MODE_STATUS = {0x01,0x01,0x0D};    // ֧��ģʽ״̬
	const static _tagTVM_STATUS_ID   PRINT_MODE_STATUS = {0x01,0x01,0x0E};      // ��ӡģʽ״̬
	const static _tagTVM_STATUS_ID   CHANGE_MODE_STATUS = {0x01,0x01,0x0F};     // ����ģʽ״̬
	const static _tagTVM_STATUS_ID   DISC_SPACE_STATUS = {0x04,0x01,0x01};      // ���̿ռ�״̬*/

	const static _tagTVM_STATUS_ID   TOKEN_BOX1_STATUS = {0x06,0x01,0x01};	// TokenʽƱ��1��װ״̬
	const static _tagTVM_STATUS_ID   TOKEN_BOX1_COUNT =  {0x06,0x01,0x02};		// TokenʽƱ��1����״̬
	const static _tagTVM_STATUS_ID   TOKEN_BOX2_STATUS = {0x06,0x02,0x01};		// TokenʽƱ��2��װ״̬
	const static _tagTVM_STATUS_ID   TOKEN_BOX2_COUNT =  {0x06,0x02,0x02};		// TokenʽƱ��2����״̬
	const static _tagTVM_STATUS_ID   TOKEN_CYCLEBOX_STATUS = {0x07,0x01,0x01};  // Tokenʽ��Ʊ�䰲װ״̬
	const static _tagTVM_STATUS_ID   TOKEN_CYCLEBOX_COUNT = {0x07,0x01,0x02};   // Tokenʽ��Ʊ������״̬
	const static _tagTVM_STATUS_ID   TOKEN_HOPPER1_STATUS = {0x07,0x03,0x01};	// TokenʽƱ��1��װ״̬���ޣ�
	const static _tagTVM_STATUS_ID   TOKEN_HOPPER1_COUNT = {0x0D,0x04,0x02};	// TokenʽƱ��1����״̬
	const static _tagTVM_STATUS_ID   TOKEN_HOPPER2_STATUS = {0x07,0x04,0x01};	// TokenʽƱ��2��װ״̬(��)
	const static _tagTVM_STATUS_ID   TOKEN_HOPPER2_COUNT = {0x0D,0x05,0x01};	// TokenʽƱ��2����״̬
	const static _tagTVM_STATUS_ID   TOKEN_COLLBOX_STATUS = {0x07,0x02,0x01};  // Tokenʽ�����䰲װ״̬
	const static _tagTVM_STATUS_ID   TOKEN_COLLEBOX_COUNT = {0x07,0x02,0x02};   // Tokenʽ����������״̬

	/*
	const static _tagTVM_STATUS_ID   TILT_STATUS = {0x24,0x01,0x01};           // �񶯴�����״̬
	const static _tagTVM_STATUS_ID   AUTHORIZED_STATUS = {0x40,0xEF,0xEC};           // ��Ȩ����״̬
	const static _tagTVM_STATUS_ID   OFFLINE_DAYS = {0x40,0xEF,0xEA};           // �ѻ�����*/
}

// �豸����״̬���붨��
typedef enum{
	IN_SERVICE    = 0x00, // �豸��������
	WARN_SERVICE  = 0x01, // �豸�������
	OUTOF_SERVICE = 0x02, // �豸��ͣ����
	ERROR_SERVICE = 0x03, // �豸���Ϲر�
	MANT_SERVICE  = 0x04, // �豸ά�޹ر�
	SLEEP_SERVICE = 0x05, // �豸˯��ģʽ
	STOP_SERVICE  = 0x06, // �豸ֹͣ������Ӫ������
	EMER_SERVICE  = 0x07  // �豸��������
}SERVICE_STATUS;

// ����״̬���붨�� 
typedef enum{
	RUNING							= 0x00,  // ����������
	CLOSEING						= 0x01,  // ҵ��������
	START_RUNING				    = 0x02,  // ��������
	SHUTDOWNING						= 0x03,  // �ػ���
	WARNING							= 0x04,  // �豸����
	OPERATION_STARTING				= 0x05   // �豸��ʼ��Ӫ
}RUN_STATUS;

// ����ģʽ״̬���붨��
//typedef enum{
//	WORK_MODE_NONE					= 0x00,			// ��ҵ��
//	WORK_MODE_ISSUE					= 0x01,			// ֻ��Ʊ
//	WORK_MODE_RECHARGE				= 0x02,			// ֻ��ֵ
//	WORK_MODE_ALL					= 0x03			// ����ҵ��
//	//WORK_MODE_ISSUE_CARD			= 0x08,			// ֻ�ۿ�
//	//WORK_MODE_ISSUE_PRODUCT		= 0x04,			// ֻ�۲�Ʒ	
//	//WORK_MODE_QUERY				= 0x01,			// ֻ��ѯ
//}WORK_MODE;

// ��Ӫģʽ״̬���붨�� mzy
typedef enum{
	OPERATION_MODE_STATUS_UNKNOW     				= 0xFFFF,	// δ֪ģʽ
	OPERATION_MODE_STATUS_NORMAL_MODE				= 0x01,		// ����ģʽ
	OPERATION_MODE_STATUS_CLOSE_MODE			    = 0x02,		// �ر�ģʽ
	OPERATION_MODE_STATUS_EMERGENCY_MODE			= 0x06,		// ����ģʽ
	OPERATION_MODE_STATUS_FARE_FREE_MODE		    = 0x07		// ʱ�����ģʽ
}OPERATION_MODE;

// 24Сʱ��Ӫģʽ״̬���붨��
typedef enum{
	OPERATION_24HOURS_OFF				= 0x00,		// ��24Сʱ��Ӫ
	OPERATION_24HOURS_ON				= 0x01 		// 24Сʱ��Ӫ
}OPERATION_24HOURS_MODE;

// �ӳ���Ӫģʽ״̬���붨��
typedef enum{
	OPERATION_DELAY_OFF			= 0x00,		// ���ӳ���Ӫ
	OPERATION_DELAY_ON			= 0x01 		// �ӳ���Ӫ
}OPERATION_DELAY_MODE;

// ͨѶ״̬���붨��
typedef enum{
	DISCONNECT = 0x00,// ͨѶ�ж�
	CONNECTED  = 0x01,// ͨѶ����
	CONNECTING = 0x02 // ͨѶ�ָ���
}COMMUNICATION;

// ʱ�Ӳ���״̬���붨��
typedef enum{
	VALID_TIME = 0x00, // ʱ������
	WARN_TIME  = 0x01, // ʱ�Ӿ���
	ERROR_TIME = 0x02  // ʱ�ӹ���
}TIME_STATUS;

// ��¼״̬���붨��
typedef enum{
	LOGIN   = 0x00,	// ���˵�¼	��¼�ɹ����״̬
	LOGOUT  = 0x01,	// ���˵�¼	�˳���¼���״̬
	LOCKED  = 0x02	// �˻�����	�˻��������״̬�����˵�¼�ɹ���״̬�ı�
}LOGIN_STATUS;

// �豸����״̬���붨��
typedef enum{
	DEVICE_NO_ERROR			= 0x00, // �޹���
	DEVICE_ERROR			= 0x01  // �й���
}DEVICE_ERR_CODE;

// �������״̬
typedef enum{
	TRANSFER_AUDIT_STATUS_WARNING		= 0x00,	// ����
	TRANSFER_AUDIT_STATUS_NORMAL		= 0x01,	// ����	
	TRANSFER_AUDIT_STATUS_CLOSE			= 0x02	// ����	
}TRANSFER_AUDIT_STATUS;

// ���̿ռ�״̬���붨��
typedef enum{
	DISK_SPACE_NORMAL			= 0x00, // ����
	DISK_SPACE_FULL				= 0x01,  // ��
	DISK_SPACE_NEAR_FULL		= 0x02  // ����
}DISK_SPACE_CODE;

// Ʊ��/Ǯ�䰲װ״̬���붨��
typedef enum{
	STATUS_MAGAZINE_SET								= 0x00,		// �Ѱ�װ
	STATUS_MAGAZINE_INVALIDE_ATACH					= 0x01,		// �Ƿ���װ
	STATUS_MAGAZINE_UNSET							= 0x02,    // δ��װ
	STATUS_MAGAZINE_UNREADY							= 0x03,    // ����λ
}STATUS_MAGAZINE;

// Ʊ��/Ǯ��/Hopper������״̬���붨��
typedef enum{
	MAG_NUM_NORMAL			    = 0x00,	// ����
	MAG_NUM_EMPTY				= 0x01,	// ��
	MAG_NUM_NEAR_EMPTY			= 0x02,	// ����
	MAG_NUM_FULL				= 0x03,	// ��
	MAG_NUM_NEAR_FULL			= 0x04	// ����
}MAGAZINE_NUM_STATUS;

// �ֽ�ȫ��״̬���ݲ�ʹ�ã�
typedef enum{
	SECURITY_DOOR_STATUS_NORMAL_OPEN		= 0x00,	// ������
	SECURITY_DOOR_STATUS_NORMAL_CLOSE		= 0x01,	// �����ر�
	SECURITY_DOOR_STATUS_INVALIDE_OPEN		= 0x02	// �쳣��
}SECURITY_DOOR_STATUS;					

// ������״̬���ݲ�ʹ�ã�
typedef enum{
	TOUCH_SCREEN_STATUS_NORMAL			= 0x00,	// ����
	TOUCH_SCREEN_STATUS_ERROR			= 0x01,	// ����
}TOUCH_SCREEN_STATUS;					

// ��Ԯ��ť״̬
typedef enum{
	ASSISTANCE_BUTTON_STATUS_UNPRESSED		= 0x00,	// ����
	ASSISTANCE_BUTTON_STATUS_PRESSED		= 0x01,	// δ����
}ASSISTANCE_BUTTON_STATUS;			

// ��д��״̬���붨��
typedef enum{
	RW_NORMAL			= 0x00,		// ��д������
	RW_ERROR			= 0x01,		// ��д������
	RW_LOSS				= 0x02,		// ��д����ʧ
	RW_SAM_ERROR		= 0x03		// ��д��SAM����
}STATUS_RW;

// SAM��״̬���붨��
typedef enum{
	SAM_STATUS_LRT_NORMAL					= 0x01,		// LRT SAM������
	SAM_STATUS_LRT_FAILURE					= 0x02,		// LRT SAM���쳣
	SAM_STATUS_MACAUPASS_NORMAL				= 0x03,		// ����ͨSAM������
	SAM_STATUS_MACAUPASS_FAILURE			= 0x04		// ����ͨSAM���쳣
}SAM_STATUS;

// ��ӡ��״̬���붨��
typedef enum{
	PRINTER_NORMAL   = 0x00,  // ��ӡ����������
	PRINTER_ERR      = 0x01,  // ��ӡ��ͨ�Ź���
	PRINTER_SHUTDOWN = 0x02,  // ��ӡ���ϵ�
	PRINTER_NOPAPER  = 0x03	  // ��ӡ��ȱֽ
}PRINTER_STATUS;

// UPS״̬���붨��
typedef enum{
	UPS_STATUS_OPEN			= 0x00,  // UPS��
	UPS_STATUS_CLOSE			= 0x01,  // UPS��
	UPS_STATUS_WARNING	= 0x02   // UPS������
}UPS_STATUS;

// ά����״̬���붨��
typedef enum{
	DOOR_NORMAL_OPEN  = 0x00, // ά����������
	DOOR_NORMAL_CLOSE = 0x01, // ά���������ر�
	DOOR_ILLEGLE_OPEN = 0x02, // ά���ŷǷ���
	DOOR_CLOSE_UNLOCK = 0x03  // ά���Źر�δ��
}DOOR_STATUS;

// ֽ��/Ӳ��/��ʽ����/Tokenʽ����/״̬��ʾ��ģ��״̬���붨��
typedef enum{
	HARDWARE_MODULE_NORMAL				= 0x00,  // �豸����
	HARDWARE_MODULE_UNSET				= 0x01,  // �豸����λ
	HARDWARE_MODULE_NO_CONNECT			= 0x02,  // �豸ͨ��ʧ��
	HARDWARE_MODULE_FAILURE				= 0x03,  // �豸����
	HARDWARE_MODULE_JAM					= 0x04	 // �豸���һ�Ʊ
}HARDWARE_MODULE_STATUS;

// �񶯴�����״̬���붨��
typedef enum{
	TILT_STATUS_OFF		= 0x00,  // ����
	TILT_STATUS_ON     = 0x01   // ����
}TILT_STATUS;

// ��Ȩ����״̬���붨��
typedef enum{
	AUTHORIZED_STATUS_NORMAL			= 0x00,  // ����
	AUTHORIZED_STATUS_ABNORMAL     = 0x01   // ά���ų�������
}AUTHORIZED_STATUS;

// �豸�Զ�����״̬���붨��
typedef enum{
	AUTORUN_STOP  = 0x00, // �豸�Զ�����״̬ͣ��
	AUTORUN_START = 0x01  // �豸�Զ�����״̬����
}DEVICE_AUTORUN;

// SAM����֤״̬��Ԥ����
typedef enum{
	ISAM_UNAUTH = 0x00, // ISAM״̬δ��֤
	ISAM_AUTHED = 0x01  // ISAM״̬����֤
}SAM_AUTH_STATUS;

//--------------------------------------������TVM״̬���붨��-----------------------------------------//

//--------------------------------------�������Զ������-----------------------------------------//
// SAMλ�ñ��붨��
typedef enum{
	RECHARGE_RW_YPT_ISAM		= 0x01,// ��ֵ��д��һƱͨISAM
	RECHARGE_RW_YPT_PSAM		= 0x02,// ��ֵ��д��һƱͨPSAM
	RECHARGE_RW_YKT_ISAM		= 0x03,// ��ֵ��д��һ��ͨISAM	
	RECHARGE_RW_YKT_PSAM		= 0x04,// ��ֵ��д��һ��ͨPSAM
	CARD_RW_YPT_ISAM				= 0x05,// Ʊ����д��һƱͨISAM
	CARD_RW_YPT_PSAM				= 0x06,// Ʊ����д��һƱͨPSAM
	CARD_RW_YKT_ISAM				= 0x07,// Ʊ����д��һ��ͨISAM
	CARD_RW_YKT_PSAM				= 0x08,// Ʊ����д��һ��ͨPSAM
	TOKEN_RW_YPT_ISAM				= 0x09,// Token��д��һƱͨISAM
	TOKEN_RW_YPT_PSAM			= 0x0A,// Token��д��һƱͨPSAM
	TOKEN_RW_YKT_ISAM				= 0x0B,// Token��д��һ��ͨISAM
	TOKEN_RW_YKT_PSAM			= 0x0C// Token��д��һ��ͨPSAM
}SAM_POSITION;

// SAM��ǩ��ǩ��״̬��Ԥ����
typedef enum _auth_type{
	LOGON_IN        = 0x01,		// ǩ��
	LOGON_OUT       = 0x02,		// ǩ��
	DISCONNECT_AUTH = 0x03       // ������֤
}AUTH_TYPE;

// ��ǰ��ЧCD�汾
typedef enum _cd_version{
	VERSION_FIRST = 1,
	VERSION_SECOND = 2
}CD_VERSION;

typedef enum{
	SRC_NOT_BUT_NEW_HAVE = 0,	//����Ʊ��֮ǰû��Ʊ�䣬֮��װ���µ�Ʊ��
	SRC_HAVE_BUT_NEW_NOT,		//����Ʊ��֮ǰ����Ʊ�䣬֮��û�а�װ���µ�Ʊ�䣬 ��ж��Ʊ�䣬δ��װ��Ʊ�䡣
	SRC_NEW_SAME,				//����Ʊ��֮ǰ����Ʊ�䣬֮��װ��Ʊ����ԭ����һ��, ��Ʊ��û�и���
	SRC_NEW_DIFFER,				//����Ʊ��֮ǰ����Ʊ�䣬֮��װ��Ʊ����ԭ���Ĳ�һ���� ��������Ʊ�䡣
	SRC_NEW_ALL_NOT,			//����ǰ�󶼲�����Ʊ�䡣
	SRC_NEW_DIFFER_NUM			//����ǰ��������һ��
} OP_BOX_STATUS;

typedef enum{
	UPLOAD_COMPLETE    = 0x0001, // �ļ��ϴ�����
	PARAMETER_COMPLETE = 0x0002, // �����������
	DAY_AUDIT_COMPLETE = 0x0003, // �մ�����ƽ���
	DATA_SYNC_COMPLETE = 0x0004  // ��λͬ���������
}NOTICE_TYPE;


//--------------------------------------�����ǽ���������ṹ����-----------------------------------------//
// ACC�豸�Ĵ�������
typedef struct _txn_audit_register{
	AfcSysHdr_t afcSysHdr;			// AFCϵͳרҵͷ
	SysComHdr_t sysComHdr;			// ϵͳ����ͷ
	_txn_audit_register(){
	}
}TXN_AUDIT_REGISTER;

// CD�汾�л�
typedef struct _txn_event_cd_version_status{
	AfcSysHdr_t afcSysHdr;			// AFCϵͳרҵͷ
	SysComHdr_t sysComHdr;			// ϵͳ����ͷ
	DWORD cdGenerationId;
	DWORD numberOfEntries;
	DWORD system_params;              
	DWORD service_provider_params;
	DWORD card_actionlist;
	DWORD product_actionlist;
	DWORD sam_hotlist;
	DWORD calendars;
	DWORD fare_tables;
	DWORD fare_patterns;
	DWORD fare_codes;
	DWORD day_types;
	DWORD location_names;
	DWORD product_type_params;
	DWORD participant_params;
	DWORD mifare_params;
	DWORD high_security_blacklist;
	DWORD locations;
	DWORD business_rules;
	DWORD card_batch_withdraw_list;
	_txn_event_cd_version_status(){
		cdGenerationId = 0;
		numberOfEntries = 0;
		system_params = 0;
		service_provider_params = 0;
		card_actionlist = 0;
		product_actionlist = 0;
		sam_hotlist = 0;
		calendars = 0;
		fare_tables = 0;
		fare_patterns = 0;
		fare_codes = 0;
		day_types = 0;
		location_names = 0;
		product_type_params = 0;
		participant_params = 0;
		mifare_params = 0;
		high_security_blacklist = 0;
		locations = 0;
		business_rules = 0;
		card_batch_withdraw_list = 0;
	}
}TXN_EVENT_CD_VERSION_STATUS;

// �豸״̬
typedef struct _txn_event_device_status
{
	AfcSysHdr_t afcSysHdr;			// AFCϵͳרҵͷ
	SysComHdr_t sysComHdr;			// ϵͳ����ͷ
	BYTE deviceStatus;				// �豸״̬  1��In service��2��Out of service��255��δ���塣
	_txn_event_device_status(){
		deviceStatus = 0;
	}
}TXN_EVENT_DEVICE_STATUS;

// �豸��ʶ
typedef struct _txn_event_device_identification
{
	AfcSysHdr_t afcSysHdr;			// AFCϵͳרҵͷ
	SysComHdr_t sysComHdr;			// ϵͳ����ͷ
	_txn_event_device_identification(){
	}
}TXN_EVENT_DEVICE_IDENTIFICATION;

// ��Ʊ���ս���
typedef struct _invalid_ticket_recycletrade{
	TransComHdr_t transComHdr;
	DWORD cardSerialNo;			// 4 BIN �����к�
	BYTE cardPhysicalType;      // 1 BIN ����������
	DWORD issueId;			    // 4 BIN ��������
	WORD productType;			// 2 BIN ��Ʊ��Ʒ����
	BYTE reson;                 // 1 BIN �Ͽ�����ԭ��
	CCTLOGCountInfo::TICKETBOX_ID magazineId;     // 4 1BCD+1BIN+2BIN Ʊ��ID
	_invalid_ticket_recycletrade()
	{
		cardSerialNo = 0;
		cardPhysicalType = 0;
		issueId = 0;
	}
}INVALIDE_TICKET_RECYCLETRADE;

// ����Ա�����޸�����
typedef struct _change_staffpassword
{
	DWORD StaffID;        // ���޸������Ա��ID
	CString sOldPassword;    // ������
	CString sNewPassword;    // �޸ĺ�����
	BYTE ciperOldPwd[6];
	BYTE ciperNewPwd[6];
	_change_staffpassword()
	{
		StaffID = 0;
		sOldPassword = "";
		sNewPassword = "";
		memset(ciperOldPwd, 0, 6);
		memset(ciperNewPwd, 0, 6);
	}
}CHANGE_STAFFPASSWORD;

// �����ͳ�����£�ҵ�����ݣ�
typedef struct _parameter_update{
	WORD parameterId;  // �����ͳ���ID	2	BCD	����С�����˳������
	WORD providerId;   // �豸��Ӧ��ID 	2	BIN	����ʱ��Ҫת����INTEL�� �Գ�����Ч���Բ�����Ч��������ʱ��ȫ0
	WORD softwareNo;   // ������κ� 	2	BIN	����ʱ��Ҫת����INTEL�򡣶Գ�����Ч���Բ�����Ч��������ʱ��ȫ0
	BYTE versionType;  // �汾����	1	BIN	0x00��ʽ������ǰ�汾��0x01��ʽ���������汾��0x10���Բ�����ǰ�汾��	0x11���Բ��������汾
	DWORD oldVersion;  // �ɰ汾��	4	BIN	����ʱ��Ҫת����INTEL�� �л�ǰ�Ĳ���/�����ļ��İ汾��
	DWORD oldStartDate;// ��Ч����	4	BCD	�ɰ汾����Ч���ڣ�YYYYMMDD
	DWORD newVersion;  // �°汾��	4	BIN	����ʱ��Ҫת����INTEL�� �л������/�����ļ��İ汾��
	DWORD newStartDate;// ��Ч����	4	BCD	�°汾����Ч���ڣ�YYYYMMDD
	_parameter_update()
	{
		parameterId = 0;
		providerId = 0;
		softwareNo = 0;
		versionType = 0;
		oldVersion = 0;
		oldStartDate = 0;
		newVersion = 0;
		newStartDate = 0;
	}
}PARAMETER_UPDATE;

// �����ͳ������أ�ҵ�����ݣ�
typedef struct _parameter_download{
	WORD parameterId;    // 2 BCD �����ͳ���ID ����С�����˳������
	WORD providerId;     // 2 BIN �豸��Ӧ��ID
	WORD softwareNo;     // 2 BIN ������κ�
	BYTE versionType;    // 1 BIN �汾���� 0x00��ʽ������ǰ�汾��0x01��ʽ���������汾��0x10���Բ�����ǰ�汾��	0x11���Բ��������汾
	DWORD version;       // 4 BIN �汾
	_DATE_TIME DateTime; // 7 BCD ����ʱ��
	BYTE file[20];       // 20 ASCII �ļ�����
	BYTE result;         // 1 BIN ���ؽ�� 0x00���سɹ���0x01����ʧ�ܣ�0x02�޷�������0x03��������

	_parameter_download(){
		parameterId = 0;
		providerId = 0;
		softwareNo = 0;
		versionType = 0;
		version = 0;
		result = 0;
		memset(file, 0x00, 20);
	}
}PARAMETER_DOWNLOAD;


// ������־
typedef struct _operator_log{
	CString csUserID;
	OPERATION_CODE operCode;    // ��������	2  1BIN+1BIN
	OPERATION_LEVEL operLevel;	// ��������	1  BIN
	BYTE result;	            // �������	1  BIN
	BYTE operDetail[32];	    // ��������	32 ASCII
	BYTE operTip[32];	        // ��ע	    32 ASCII
	_operator_log(){
		csUserID = "";
		result  = 0;
		memset(operDetail, 0x00, 32);
		memset(operTip, 0x00, 32);
	}
}OPERATOR_LOG;

// �豸��������
typedef struct _device_errordata{
	BYTE partType;					  // 1 BIN �豸��������
	BYTE  bDeviceType;            // 1 BIN ��������                          
	BYTE devicePartID[3];         // 3 BIN ����ID
	WORD deviceDeviderId;         // 2 BIN �豸��Ӧ��ID
	BYTE maintanCode[4];	          // 4 1BIN+1BIN+1BIN+1BIN ���ϴ���
	BYTE ecode[2];	                  // 2 1BIN+1BIN ά�޴���
	_device_errordata()
	{
		memset(devicePartID, 0, 3);
		memset(maintanCode, 0, 4);
		memset(ecode, 0, 2);
	}
}DEVICE_ERRORDATA;

// Ʊ�佻��
typedef struct _magazine_change{
	TICKETBOX_OPERATION_TYPE operType;             // 1 BIN ��������
	CCTLOGCountInfo::TICKETBOX_ID magazineId;              // 4 1BCD+1BIN+2BIN Ʊ��ID
	MAGAZINE_PALACE magazne;			 // 1 BIN Ʊ��λ��ID
	BYTE  cardType;						 // 1 BIN Ʊ����������
	DWORD providerId;					 // 4 BIN Ʊ��������ID
	WORD cardProductType;				 // 2 BIN ��Ʊ��Ʒ����
	BYTE preProperty;					 // 1 BIN Ԥ��ֵ����
	BYTE childproductId;                 // 1 BIN ������ƷID
	WORD CardCount;                      // 2 BIN Ʊ������
	SC_MAGAZINE_STATUS magazineStatus;      // 1 BIN Ʊ��״̬
	_magazine_change()
	{
		cardType = 0;
		providerId = 0;
		cardProductType = 0;
		preProperty = 0;
		childproductId = 0;
		CardCount = 0;
	}
}MAGAZINE_CHANGE;

// TVM/AVM���ϵ�ҵ������
typedef struct _tvm_avm_business_failed_data{
	DEVICE_ID	DeviceID;
	_DATE_TIME	DateTime;
	DWORD		DeviceAfcNum;
	DWORD		TransactionNum;
	DWORD		RefoundAmount;
	BYTE		FailedReason[12];
	
	_tvm_avm_business_failed_data(){
		DeviceID = _device_id();
		DateTime = _date_time();
		DeviceAfcNum = 0;
		TransactionNum = 0;
		RefoundAmount = 0;
		memset(FailedReason,0,sizeof(FailedReason));
	}

}BUSINESS_FAILED_DATA;

// Ǯ��ID��ֽ�ң�Ӳ�ң�
typedef struct cash_box_id
{
	BYTE  btLineCode;
	BYTE  btType;
	WORD  wNum;
	cash_box_id()
	{
		btLineCode  = 0xFF;
		btType      = 0xFF;
		wNum       = 0xFFFF;
	}
	void Serialze(LPBYTE lpData) const
	{
		if(btLineCode!=0xFF)
		{
			int2BCD(btLineCode,(char*)lpData,1);
		}
		else
		{
			*lpData = btLineCode;
		}
		lpData++;
		*lpData = btType;
		lpData++;
		memcpy(lpData,&wNum,2);
		lpData+=2;
	}
	void Deserialize(LPBYTE lpData)
	{
		if(*lpData == 0xFF)
		{
			btLineCode = *lpData;
		}
		else
		{
			btLineCode = BCD2int((char*)lpData,1);
		}
		lpData++;
		btType = *lpData;
		lpData++;
		memcpy(&wNum,lpData,2);
		lpData+=2;
	}
	CString ToString()
	{
		CString str;
		str.Format(_T("%02X%02X%04X"),(BYTE)char2BCD(btLineCode),btType,wNum);
		return str;
	}
	bool operator == (const cash_box_id& another) const
	{
		if(btLineCode != another.btLineCode) return false;
		if(btType != another.btType) return false;
		if(wNum != another.wNum) return false;
		return true;
	}
} CASH_BOX_ID;//
////Ǯ��λ��
//typedef enum  cash_box_position
//{
//	BANKNOTE_COLL_POS		=  0x01, 
//	COIN_CHANGE_BOX_A_POS,
//	COIN_CHANGE_BOX_B_POS,
//	COIN_HOPPER_BOX_POS,
//	COIN_COLL_BOX_POS,
///*	BANKNOTE_INVALID_POS,*/
//	BANKNOTE_CHANGE_BOX_POS,
//	BANKNOTE_RECYCLE_BOX_A_POS,
//	BANKNOTE_RECYCLE_BOX_B_POS,
//	BANKNOTE_RECYCLE_BOX_C_POS,
//	BANKNOTE_RECYCLE_BOX_D_POS,
//}CASH_BOX_POSITION;

//Ǯ��״̬
//typedef enum _cash_box_state
//{
//	NORMAL_INSTALL    = 0x01, //������װ
//	ILLEGAL_INSTALL   = 0x02, //�Ƿ���װ
//	NORMAL_UNINSTALL  = 0x03, //����ж��
//	ILLEGAL_UNINSTALL = 0x04, //�Ƿ�ж��
//	UNKNOW_STATE      = 0xFF  //δ֪״̬
//}CASH_BOX_STATE;

//Ǯ�Ҵ��� + Ǯ������
typedef struct cash_type_count 
{
	BankNoteAndCoinType_t cash_type; //���ִ��� 1BIN
	WORD                  cash_count;//�������� 2BIN
	cash_type_count(){
		cash_type  = VALUE_UNKNOWN;
		cash_count = 0;
	}
}CASH_TYPE_COUNT;

//Ǯ���������
typedef enum  _handle_type
{
	INSTALL     = 0x01,//��װ
	UNINSTALL   = 0x02,//ж��
	ADD         = 0x03,//����
	BOX_RECYCLE     = 0x04 //����
}BOX_HANDLE_TYPE;

// 4.2.16   Ǯ�����ҵ������
typedef struct cashbox_change
{
	BOX_HANDLE_TYPE       handleType;  //1 �������� 1BIN
	BYTE COIN_BANKNOTE_BOX_ID[4];  //4 Ǯ��ID   1BCD+1BIN+2BIN
	BYTE       position;  //1 Ǯ��λ�� 1BIN
	SC_MAGAZINE_STATUS         State;  //1 Ǯ��״̬ 1BIN
	BYTE                     btCount;  //1 ��������
	vector<CASH_TYPE_COUNT>  vecCash;  //���ִ���+��������

	cashbox_change()
	{
		
	}
	
	// Ӳ��Hopper��ID
	void SetHopperBoxID(BYTE btLineCode, BYTE btType, BYTE bStationCode, BYTE wNum)
	{
		COIN_BANKNOTE_BOX_ID[0] = btLineCode;
		COIN_BANKNOTE_BOX_ID[1] = btType;
		COIN_BANKNOTE_BOX_ID[2] = bStationCode;
		COIN_BANKNOTE_BOX_ID[3] = wNum;
	}

	//ƴ��Ǯ��ID
	void SetBoxID(BYTE btLineCode, BYTE btType,BYTE bNumInGrop, BYTE bNum)
	{
		COIN_BANKNOTE_BOX_ID[0] = /*char2BCD*/(btLineCode);
		COIN_BANKNOTE_BOX_ID[1] = /*char2BCD*/(btType);
		COIN_BANKNOTE_BOX_ID[2] = /*char2BCD*/(bNumInGrop);	//(wNum >>0x08) & 0x00ff;
		COIN_BANKNOTE_BOX_ID[3] = /*char2BCD*/(bNum);	//wNum & 0x00ff;
	}
	void Serialze(LPBYTE lpData) const
	{
		*lpData = COIN_BANKNOTE_BOX_ID[0];
		lpData++;
		*lpData = COIN_BANKNOTE_BOX_ID[1];
		lpData++;
		*lpData = COIN_BANKNOTE_BOX_ID[2];
		lpData++;
		*lpData = COIN_BANKNOTE_BOX_ID[3];
		lpData++;

	}

	//����ֽ������䰲װ
	void BanNoteBox_Initial()
	{
		CASH_TYPE_COUNT cash;
		cash.cash_type = Banknote1Yuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		cash.cash_type = Banknote5Yuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		cash.cash_type = Banknote10Yuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		cash.cash_type = Banknote20Yuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		cash.cash_type = Banknote50Yuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		cash.cash_type = Banknote100Yuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
	}

	//��Ӳ����
	void CoinBox_Initial()
	{
		CASH_TYPE_COUNT cash;
		cash.cash_type = CoinHalfYuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		cash.cash_type = Coin1Yuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		/*cash.cash_type = Coin5MOP;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		cash.cash_type = Coin50AVOS;
		cash.cash_count = 0;
		vecCash.push_back(cash);*/
	}

	//��ȡ����������
	BYTE GetSumCurrency()
	{
		int nSum = 0;
		vector<CASH_TYPE_COUNT> ::iterator iter = vecCash.begin();
		while (iter != vecCash.end())
		{
			if((*iter).cash_count > 0)
			{
				nSum++;
				iter++;
			}
			else{
				vecCash.erase(iter);// ����Ϊ�ղ��ϱ���
			}			
		}
		//LPBYTE lpSum;
		//memcpy(lpSum,&nSum,1);
		BYTE  bSum = (BYTE) (0xff & nSum);
		return bSum;
	}

}CASHBOX_CHANGE;


// ISAMǩ����ǩ��
typedef struct _isam_auth
{
	BYTE samid[6];        // 6 BCD ISAM����
	DWORD participantId;  // 4 BIN ���뷽ID
	AUTH_TYPE auth_type;  // 1 BIN ����״̬
	BYTE authResult;      // 1 BIN ��֤���
	DWORD amount;         // 4 BIN ��Ȩ���
	_DATE_TIME datetime;  // 7 BCD ��ȨʧЧʱ��
	_isam_auth()
	{
		memset(samid, 0x00, sizeof samid);
		participantId = 0;
		authResult = 0;
		amount = 0;
	}
}ISAM_AUTH;

typedef struct _Personalization
{
	char cardHolderTitle[5];	//�ֿ���ͷ��
	char cardHolderName[61];	//�ֿ�������
	BYTE languagePreference;	//�ֿ���ѡ����д��Ʊ��������
	_DATE dateOfBirth;			//����
	char cardholderPhoneDay[21];//�ֿ����ռ�绰
	char cardholderPhoneHome[21];//�ֿ��˼�ͥ�绰
	char cardholderEmail[129];	//�ֿ��˵����ʼ�
	char cardholderClue[21];	//�ֿ���������ʾ
	char cardholderPassword[16];//�ֿ�������
	char staffID[30];			//�ֿ��˲���Ա��
	BYTE sex;					//�Ա� 1:Male 2:Female
	BYTE photePrinted;			//���������Ƿ���ӡ����Ƭ 1:��ӡ��; 2:δӡ��
	char address1[129];			//��ϸ��ַ1
	char address2[129];			//��ϸ��ַ2
	char cardholderNumber[19];	//�ֿ����ṩ�ĸ������
	BYTE cardholderNumberType;	//����ֿ�ʹ�õĸ����������	
	DWORD cardSerialNumber;	//�ֿ����ṩ�ĸ�������߼�����	

	_Personalization()
	{
		memset(cardHolderTitle,0,sizeof(cardHolderTitle));
		languagePreference = 0;
		memset(cardHolderName,0,sizeof(cardHolderName));
		memset(cardholderPhoneDay,0,sizeof(cardholderPhoneDay));
		memset(cardholderPhoneHome,0,sizeof(cardholderPhoneHome));
		memset(cardholderEmail,0,sizeof(cardholderEmail));
		memset(cardholderClue,0,sizeof(cardholderClue));
		memset(cardholderPassword,0,sizeof(cardholderPassword));
		memset(staffID,0,sizeof(staffID));
		sex = 0;
		photePrinted = 0;
		memset(address1,0,sizeof(address1));
		memset(address2,0,sizeof(address2));
		memset(cardholderNumber,0,sizeof(cardholderNumber));
		cardholderNumberType = 0;
		cardSerialNumber = 0;
	}

}Personalization,PersonalizationQuery;

typedef struct _HistoryTranceInfo
{
	_DATE_TIME TransactionDateTime;
	WORD TransactionStation;
	DWORD TransactionDevice;
	BYTE TransactionType;
	DWORD TransactionValue;
	DWORD RemainingValue;
	BYTE PaymentMethod;
	DWORD PaymentValue;

	_HistoryTranceInfo()
	{
		TransactionStation = 0;
		TransactionDevice = 0;
		TransactionType = 0;
		TransactionValue = 0;
		RemainingValue = 0;
		PaymentMethod = 0;
		PaymentValue = 0;
	}
	bool operator < (const _HistoryTranceInfo& another) const
	{
		if(TransactionDateTime < another.TransactionDateTime) return true;
		return false;
	}
	bool operator > (const _HistoryTranceInfo & another) const
	{
		if(TransactionDateTime > another.TransactionDateTime) return true;		
		return false;
	}
	bool operator == (const _HistoryTranceInfo& another) const
	{
		if(TransactionDateTime == another.TransactionDateTime) return true;		
		return false;
	}
}HistoryTranceInfo;

typedef struct _HistoryProductInfo
{
	DWORD cardIssuerId;	        // ������ID
	DWORD cardSerialNumber;	    // �߼�����
	WORD ProductType;           // ��Ʒ����
	WORD ProductVariantType;    // ��Ʒ������
	DWORD TransactionCount;
    vector<HistoryTranceInfo> TransactionRecord;

	_HistoryProductInfo()
	{
		cardIssuerId = 0;
		cardSerialNumber = 0;
		ProductType = 0;
		ProductVariantType = 0;
		TransactionCount = 0;
		TransactionRecord.clear();
	}
}HistoryProductInfo;

typedef struct _HistoryQueryProductType
{
	WORD ProductType;           // ��Ʒ����
	WORD ProductVariantType;    // ��Ʒ������

	_HistoryQueryProductType()
	{
		ProductType = 0;
		ProductVariantType = 0;
	}
}HistoryQueryProductType;

typedef struct _HistoryInfoQuery
{
	DWORD cardIssuerId;	        // ������ID
	DWORD cardSerialNumber;	    // �߼�����
	_DATE_TIME StartDate;		// 7 BCD ������ʼ����
	_DATE_TIME EndDate;			// 7 BCD ���׽������ڣ�	EndDate �C Start <=31��
	DWORD ProdutCount;			// Ʊ����Ʒ����
    _HistoryQueryProductType ProductInfo[10];

	_HistoryInfoQuery()
	{
		cardIssuerId = 0;
		cardSerialNumber = 0;
		ProdutCount = 0;
	}
}HistoryInfoQuery;

// �������
typedef struct _component{
	BYTE  bDeviceType;   // 1 BIN �豸��������
	DWORD dwCompoentID;  // 3 BIN �������
}COMPONENT;


// ����ģʽ����
typedef enum _work_mode_{
	WORK_MODE_BANKNOTE_ONLY			= 0x20,//ֻ��ֽ��ģʽ
	WORK_MODE_COIN_ONLY				= 0x21,//ֻ��Ӳ��ģʽ
	WORK_MODE_NO_CHANGE				= 0x22,//������ģʽ
	WORK_MODE_SVT_ONLY				= 0x23,//ֻ��ֵ����Ʊģʽ
	WORK_MODE_NO_ISSUE				= 0x24,//����Ʊģʽ
	WORK_MODE_ANALY_ONLY			= 0x25,//ֻ�鿨ģʽ
	WORK_MODE_NO_SVT				= 0x26,//���ܽ��մ�ֵ��ģʽ
	WORK_MODE_NO_COIN_ACCEPT		= 0x27,//�ݲ���Ӳ��ģʽ
	WORK_MODE_NO_BANKNOTE_ACCEPT	= 0x28,//�ݲ���ֽ��ģʽ
	WORK_MODE_NORMALLY				= 0x2F,//����ģʽ
}WORK_MODE;
