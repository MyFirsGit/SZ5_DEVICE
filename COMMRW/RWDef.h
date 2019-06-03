#pragma once

#include "CommonDefine.h"
#include "RWException.h"

//typedef enum _rw_rsp_code {
//   RW_RSP_OK               = 0x00,         // ����ɹ����
//   // DLL�Զ������
//   RW_RSP_UNKNOWN          = 0xEF,         // δ֪����
//   RW_RSP_OVERTIME         = 0xEE,         // ͨ�ų�ʱ����
//   RW_RSP_USERCANCEL       = 0xED,         // �û�ȡ��
//   RW_RSP_BADRSPCOMMAND    = 0xEC,         // Ӧ���е��������������벻һ��
//   RW_RSP_WRITECOMERROR    = 0xEB,         // д����ʧ��
//// �豸��/ͨ�������
//   RW_RSP_CHECKERROR       = 0x01,         // У��ʹ���
//   RW_RSP_BADCOMMAND       = 0x02,         // ���������
//   RW_RSP_BADPARAM         = 0x03,         // ���������
//   RW_RSP_STATUSERROR      = 0x04,         // ��д��״̬��
//   RW_RSP_BURSTERROR       = 0x26,         // ��д�������쳣
//   RW_RSP_READERERROR      = 0x70,         // ��д������

//   // ��Ƭ�����
//   RW_RSP_AUTHENERROR      = 0x10,         // ��֤ʧ��
//   RW_RSP_READERROR        = 0x11,         // ��������
//   RW_RSP_WRITEERROR       = 0x12,         // д������	
//   RW_RSP_CARDREMOVED      = 0x13,         // �޿�����������п��ƶ�
//   RW_RSP_NOCARD           = 0x14,         // ��ʱδѰ����
//   RW_RSP_INVALIDFLAG      = 0x15,         // ����Ľ���վ��ʶ
//   RW_RSP_NORECORD         = 0x16,         // �޽��׼�¼
//   RW_RSP_ENOUGH           = 0x17,         // ��������
//   RW_RSP_LOCKERROR        = 0x19,         // ����ʧ��
//   RW_RSP_DATADAMAGED      = 0x1F,         // �����ݱ��ƻ�
//RW_RSP_BLACKCARD        = 0x20,			// ��������
//RW_RSP_NOUSE            = 0x21,         // ��Ƭδ����
//   RW_RSP_SECTAUTHERR      = 0x22,         // ������֤����
//   RW_RSP_MAC1ERR          = 0x23,         // MAC1��֤����
//   RW_RSP_MAC2ERR          = 0x24,         // MAC2��֤����
//   RW_RSP_INVALID          = 0x25,         // ��Ч��
//RW_RSP_PURSE_ERROR      = 0x27,			// Ǯ���쳣
//   RW_RSP_PINERROR         = 0x61,         // ����ԱPIN����
//   RW_RSP_ANTICOLL         = 0x62,         // �ڸ�Ӧ������ͬʱ���ֶ࿨
//   RW_RSP_ERRCARD          = 0xF8,         // ָ������û�п�Ƭ���߿�Ƭ����
//   // SAM�������
//   RW_RSP_SAMERR           = 0xB0,         // SAM������
//   RW_RSP_BMACPSAMERR      = 0xB1,         // һ��ͨPSAM����֤����
//   RW_RSP_BMACISAMERR      = 0xB2,         // һ��ͨISAM����֤����
//   RW_RSP_ACCPSAMERR       = 0xB3,         // ACC PSAM����֤����
//   RW_RSP_ACCISAMERR       = 0xB4,         // ACC ISAM����֤����
//// DLL�����
//RW_RSP_DLLCMDERROR		= 0xFC,			// �ⲿIC��д�������쳣
//RW_RSP_DLLERROR			= 0xFD,			// �ⲿIC��д��DLL�쳣
//RW_RSP_ABORTERROR		= 0xFE,			// �ⲿIC��д����ʱ��Abort��
//RW_RSP_CHARGEERROR      = 0xFB,			// ��ֵʧ��
//};

//typedef enum _rw_err_detail {
//    //RW_SET_PARAM_ERR            = 1,	///< �趨ֵ�������쳣
//    //RW_FUNC_PARAM_ERR           = 2,	///< ���������쳣
//    //RW_SEQUENCE_ERR	            = 3,	///< ��������˳���쳣
//    //RW_FUNC_ERR_RETURN	        = 4,	///< ���������쳣
//    //RW_LOGIC_ERR	            = 5,	///< �߼��쳣
//    //RW_OTHER_ERR	            = 6,	///< �쳣����
//
//    RW_ERR_COM_OPEN             = 3001, ///< ���ڴ�ʧ��
//    //RW_ERR_BUFF                 = 3002, ///< �������ݴ��� 
//    RW_ERR_CARD_CHANGED         = 3003, // д��ǰ���ֿ���ͬ
//}RW_ERR_DETAIL;
//
//typedef enum _rw_first_in_flag {
//    RW_IS_FIRST_IN              = 0x01,     // ���״ν�վ
//    RW_NOT_FIRST_IN             = 0x00      // �����״ν�վ
//}RW_FIRST_IN_FLAG;
//

// ��д�������ļ���
static const TCHAR* const MID_STR_BOM_RW_PROG_APP   = _T("*4109*"); // RWӦ�ó���
static const TCHAR* const MID_STR_BOM_RW_PROG_MAIN  = _T("*4110*"); // RW���س���

// RW������ṹ
typedef struct _rw_cmd_open
{
	BYTE port;                                  // ���ں�
	DWORD baud;                                 // ͨѶ����
	BOOL  isUsb;								// �Ƿ�ʹ��USB�ӿ�
	_rw_cmd_open()
	{
		port = 0;
		baud = 0;
		isUsb = FALSE;
	}
} RW_CMD_OPEN, *LPRW_CMD_OPEN;

// ���������������
typedef struct _rw_cmd_prog_firmware
{
	LPBYTE      fileData;                   // Firmware����,�Ӹ����ļ��ж�ȡ
	int         size;                       // �ļ��Ĵ�С  
	bool        isMain;                     // �Ƿ�Ϊ���������£�true:Main, false:Exec

	_rw_cmd_prog_firmware(){
		fileData = NULL;
		size     = 0;
		isMain   = false;
	}
} RW_CMD_FIRMWARE,*LPRW_CMD_FIRMWARE;

// ���������������
typedef struct _rw_cmd_param_firmware
{
	int downloadRetryCnt;			// ����ʧ�����Դ��� 
	LPBYTE      fileData;              // ��������,�Ӳ����ļ��ж�ȡ
	int         size;							// �ļ��Ĵ�С  
	WORD		paramId;				// ����ID
	_rw_cmd_param_firmware(){
		downloadRetryCnt = 3;
		fileData = NULL;
		size     = 0;
		paramId = 0;
	}
} RW_PARAM_DOWNLOAD,*LPRW_PARAM_DOWNLOAD;

// ������Ӫģʽ�������
typedef struct _rw_cmd_set_mode
{
	BYTE ControlCode;  //���ƴ���:0x01:����ģʽ;0x02:���ģʽ
	WORD ModeCode;	   //ģʽ���룬����Ϊ2���ֽ�
	_rw_cmd_set_mode(){
		ControlCode = 0;
		ModeCode    = 0;
	}
} RW_CMD_SET_MODE,*LPRW_CMD_SET_MODE;

// ������Ӫģʽ�������
typedef struct _rw_cmd_set_time
{
	BYTE ControlCode;  //���ƴ���:0x01������24Сʱ��Ӫ��0x02�����24Сʱ��Ӫ��0x10�������ӳ���Ӫ��
	ulong Time;	       //�ӳ�ʱ��
	_rw_cmd_set_time(){
		ControlCode = 0;
		Time    = 0;
	}
} RW_CMD_SET_TIME,*LPRW_CMD_SET_TIME;

// RW��ʼ����������ṹ
typedef struct _rw_cmd_init {  
	DWORD ReadTimeout;				// �Զ�ABORT��ʱʱ��,��λms,��ʾ��λ����������ָ��������X������û�м�⵽��Ƭ�����������Զ����ص�����״̬
	DWORD DeviceId;					// �豸ID
	WORD DeviceType;				// �豸����HardwareType_t
	DWORD StationId;				// ��վλ����Ϣ LocationNumber_t
	DWORD AgentCode;				// ��Ӫ��˾����뷽 ParticipantId_t
	_DATE OperationDate;			// ��Ӫ���� YYYYMMDD
	_DATE CurrentDate;				// ��ǰ�������� YYYYMMDD
	BYTE TestMode;					// �豸����ģʽ
	WORD OperationMode;				// ��վ��Ӫģʽ �μ�EventId_t.
	DWORD OperaterID;				// ����Ա�ı��,  3	BCD	
	BYTE AntennaConfig;			    // �������� 0x00�������ߣ�0x01��˫����������A����Ѱ����0x02��˫����������B����Ѱ��
	BYTE ChangeStationSign;
	BYTE ReaderPosition;
	WORD ErrFareCtrMax;//���������������
	WORD WaitTimeForCardWR;//
	WORD RetryTimesForCardWR;
	BYTE YPTonlinePara[50];
	BYTE YKTonlinePara[50];

	_rw_cmd_init(){
		ReadTimeout = 5000;	
		DeviceId = 0;
		//memset(DeviceId,0,4);
		DeviceType = 0;
		StationId = 0;
		TestMode = 0;
		OperationMode = 0;
		OperaterID = 0;	
		AntennaConfig = 0;
		AgentCode = 0;
		ChangeStationSign = 0;
		ReaderPosition = 0;
		ErrFareCtrMax = 0;
		WaitTimeForCardWR = 0;
		RetryTimesForCardWR = 0;
		memset(YKTonlinePara,0x00,sizeof(YKTonlinePara));
		memset(YPTonlinePara,0x00,sizeof(YPTonlinePara));
	}
} RW_CMD_INIT, *LPRW_CMD_INIT;

// RW��ȡƱ������ṹ
typedef struct _rw_cmd_get_price
{
	BYTE CardType;          // ��Ƭ������
	BYTE ProductCategory;	// ��Ʊ��Ʒ������
	WORD ProductType;		// ��Ʒ����
	WORD ProductSubType;    // ��Ʊ��Ʒ�������ͣ�Ԥ����
	BYTE PriceType;         // Ҫ��ѯ��Ʊ�����ͣ�0x01����ʾ������Ʊ�۱��ѯƱ�ۣ�0x02����ʾ������Ʊ�۱��ѯƱ�ۣ���Ԥ���� 
	BYTE PassengerType;
	_DATE_TIME Time;		// ָ����ѯƱ��ʱ������ʹ�õ�ʱ��
	BYTE BegainStation[4];
	BYTE EndStation[4];	

	_rw_cmd_get_price()
	{
		CardType = 0;
		ProductCategory = 0;
		ProductType = 0;
		ProductSubType = 0;
		PriceType = 0;
		PassengerType = 0;
		memset(BegainStation,0,sizeof(BegainStation));
		memset(EndStation,0,sizeof(EndStation));
	}
} RW_CMD_TICKET_PRICE, *LPRW_CMD_TICKET_PRICE;

// RW��ȡƱ��������ṹ
typedef struct _rw_cmd_get_price_res
{
	DWORD TicketPrice;		// Ʊ��
	int DetailErrorCode;			//��ϸ�������

	_rw_cmd_get_price_res()
	{
		TicketPrice = 0;
		DetailErrorCode = 0;
	}
} RW_CMD_TICKET_PRICE_RES, *LPRW_CMD_TICKET_PRICE_RES;

//��ȡ˰������ṹ
typedef struct _rw_cmd_get_tax 
{
	uchar CardType;			//0x01������ͨM1��0x02������ͨCPU��Ƭ����0x11��һƱͨM1��Ƭ��0x12��һƱͨUL��
	ulong SaleOrAddAmount;	//��Ʊ���ֵ���

	_rw_cmd_get_tax()
	{
		CardType = 0;
		SaleOrAddAmount = 0;
	}
} RW_CMD_TICKET_TAX;

// RW��ȡ˰��������ṹ
typedef struct _rw_cmd_get_tax_res
{
	DWORD TicketTax;		// ˰��
	int DetailErrorCode;	//��ϸ�������

	_rw_cmd_get_tax_res()
	{
		TicketTax = 0;
		DetailErrorCode = 0;
	}
} RW_CMD_TICKET_TAX_RES, *LPRW_CMD_TICKET_TAX_RES;



// RW�����������
typedef struct _rw_read_card_request
{
	int readCardInterval;		// �������ʱ��(��λ������)
	bool IsCheck;				// �Ϸ��Լ���־	true:��飻false:�����
	BYTE FunctionCode;			// ҵ���ܴ���		0x01����վ��0x02����վ��0x03���ۿ���0x04����Ʊ��0x05����ֵ��0x06���˿���0x07���滻�ɿ���0x08���滻�¿���0x09�����0x10�����ڣ� 0x11���Զ���ֵ���룻0x12��������0x13����ң� 0x14�����Ի���0x15�����Ի���Ϣ���¾ɿ���0x16�����Ի���Ϣ�����¿���0x1F��Ʊ����ʼ����0x2f��Ʊ�����У�0x3f���ر��룻0xFF������
	bool IsPaidArea;			// ��������־		true:������;false���Ǹ�������
	bool IsReadRecords;			// ���׼�¼��ȡ��־	true:��ȡ;false:����ȡ
	BYTE PollCardMethod;		// ����Ѱ����ʽ		0x01:˫����ͬʱѰ����0x02��ֻ��A����Ѱ����0x03��ֻ��B����Ѱ��
	_rw_read_card_request()
	{
		readCardInterval = 100;
		IsCheck = true;
		FunctionCode = 0xFF;			
		IsPaidArea = false;	
		IsReadRecords = false;
		PollCardMethod = 0;
	}
}RW_READ_CARD_REQUEST, *LPRW_READ_CARD_REQUEST;

// RW�ۿ�����ṹ
typedef struct _rw_issue_request
{
	DWORD SerialNo;				// ������ˮ��
	_DATE_TIME TranDateTime;	// ����ʱ��
	BYTE CardType;				// ��Ƭ���� 0x11��һƱͨM1��Ƭ��0x12��һƱͨUL��
	BYTE ProductyCategory;		// ��Ʊ��Ʒ����(�ƴΣ����ڣ�Ǯ��)
	WORD ProductType;			// ��Ʊ��Ʒ����
	WORD ProductSubType;		// ��Ʊ��Ʒ������
	BYTE SaleMode;				// ��Ʊ��ʽ
	BYTE PassengerType;         // �˿�����
	DWORD SaleAmount;			// ��Ʊ���
	DWORD BegainStation;		// ���վվ��	
	DWORD EndStation;			// �յ�վվ��
	WORD MultiRideTicketCount;  // �ƴ�Ʊʹ�ô���
	WORD ValidityPeriod;        // ��Ч�ڵ�ʱ�䳤��
	BYTE PaymentMethod;			// ֧����ʽ
	BYTE BankCardNum[10];	    // ���п���
	BYTE TicketNum[8];			// ֧������

	_rw_issue_request()
	{
		Initialize();
	}
	void Initialize()
	{
		SerialNo = 0;			
		CardType = 0;				
		ProductyCategory = 0;	
		ProductType = 0;			
		ProductSubType = 0;		
		SaleMode = 0;			
		PassengerType = 0;       
		SaleAmount = 0;			
		BegainStation = 0;			
		EndStation = 0;			
		MultiRideTicketCount = 0;  
		ValidityPeriod = 0;
		PaymentMethod = 0;	
		memset(BankCardNum,0,10);
		memset(TicketNum,0,8);
	}
}RW_ISSUE_REQUEST, *LPRW_ISSUE_REQUEST;

// RW��Ʊ����ṹ
typedef struct _rw_cmd_adjust
{
	DWORD SerialNo;				// ������ˮ��
	_DATE_TIME TranDateTime;	// ����ʱ��
	BYTE CardType;				// ��Ƭ���� 0x01:һ��ͨS50/S70��Ƭ;0x02:һ��ͨCPU��;0x11:һƱͨS50/S70��Ƭ;0x12:һƱͨUL����0x13��һƱͨCPU����
	WORD ProductType;			// ��Ʊ��Ʒ����
	BYTE AdjustMethod;			// ��Ʊ���ʽ 0x01:�ǿ��ڿۿƱ;0x02:���ڿۿƱ
	DWORD AdjustAmount;			// ��Ʊ���   	
	BYTE AdjustArea;			// ��Ʊ���� 0x01:��������Ʊ;0x02:�Ǹ�������Ʊ
	BYTE AdjustOperType;		// ��Ʊ�������� 0x01:����վѡ��δˢ��վ��;0x02:�����ѡ�����㣬������ʱ�����̡���Ʊ��ʱ��;0x03:ѡ���յ�վ�����������ѽ�δˢ��վ��
	DWORD BegainStation;			// ���վվ��	
	DWORD EndStation;			// �յ�վվ��
	BYTE PaymentMethod;			// ֧����ʽ
	BYTE BankCardNum[10];	    // ���п���
	_rw_cmd_adjust()
	{
		SerialNo = 0;
		CardType = 0;
		ProductType = 0;
		AdjustMethod = 0;
		PaymentMethod = 0;
		AdjustAmount = 0;
		AdjustArea = 0;
		AdjustOperType = 0;
		BegainStation = 0;			
		EndStation = 0;	
		memset(BankCardNum,0,10);
	}
} RW_CMD_ADJUST,*LPRW_CMD_ADJUST;

// RW��ֵ����ṹ
typedef struct _rw_cmd_charge
{
	DWORD SerialNo;				// ������ˮ��
	_DATE_TIME TranDateTime;	// ����ʱ��
	BYTE CardType;				// ������
	BYTE ChargeType;			// ��ֵ����
	WORD ChargeProductType;     // ��ֵ��Ʒ����
	WORD ChargeSubProductType;	// ��ֵ�Ӳ�Ʒ����
	DWORD ChargeAmount;			// ��ֵ���
	DWORD ChargeCount;			// ��ֵ����(�ƴο���)
	BYTE PaymentMethod;			// ֧����ʽ
	BYTE BankCardNum[10];	    // ���п���
	_rw_cmd_charge()
	{
		SerialNo = 0;
		PaymentMethod = 0;
		CardType = 0;
		ChargeType = 0;
		ChargeProductType = 0;
		ChargeSubProductType = 0;
		ChargeAmount = 0;
		ChargeCount = 0;
		memset(BankCardNum,0,10);
	}
} RW_CMD_CHARGE,*LPRW_CMD_CHARGE;

// RW��ֵ��ʱ����ṹ
typedef struct _rw_cmd_chargeovertime 
{
	DWORD SerialNo;				// ������ˮ��
	_DATE_TIME TranDateTime;	// ����ʱ��
	BYTE byDo;			        // �Ƿ�ʱ���г�ʱ����
	_rw_cmd_chargeovertime()
	{
		SerialNo = 0;		
		byDo = 0;
	}
} RW_CMD_CHARGEOVERTIME,*LPRW_CMD_CHARGEOVERTIME;

//  RW��Ʊ��������ṹ
typedef struct _rw_cmd_setcheck 
{
	_DATE_TIME TranDateTime;	// ����ʱ��
	BYTE byDo;			        // �Ƿ�ʱ���г��촦��
	_rw_cmd_setcheck()
	{
		byDo = 0;
	}
} RW_CMD_SETCHECK,*LPRW_CMD_SETCHECK;

//  RW��Ʊ��������ظ��ṹ
typedef struct _rw_resp_cmd_setcheck 
{
	byte byTpuState;            // ����tpu��״̬��
	int DetailErrCode;          // ����tpu�Ĵ�����
	_rw_resp_cmd_setcheck()
	{
		byTpuState = 0;
		DetailErrCode =0;
	}
} RW_CMD_RESP_SETCHECK,*LPRW_CMD_RESP_SETCHECK;

typedef struct _SURRENDERPRODUCT
{
	ushort ProductType;//��Ʊ��Ʒ����
	ushort ProductSubType;//��Ʊ��Ʒ������
	uchar SurrenderReasonCode;
	ulong RefundMoney;
	_SURRENDERPRODUCT()
	{
		ProductType=0;
		ProductSubType=0;
		SurrenderReasonCode=0;
		RefundMoney=0;
	}
}SURRENDERPRODUCT;

// RW�˿�����ṹ
typedef struct _rw_cmd_refund
{
	DWORD SerialNo;				// ������ˮ��
	_DATE_TIME TranDateTime;	// ����ʱ��
	BYTE CardPHType;
	BYTE CardAppType;
	BYTE ReasonCode;
	DWORD RefundAmount;
	DWORD DepositAmount;
	BYTE ReceptionNum[10];
	_rw_cmd_refund()
	{
		SerialNo = 0;
		CardPHType = 0;
		CardAppType = 0;
		ReasonCode = 0;
		RefundAmount = 0;
		DepositAmount = 0;
		memset(ReceptionNum,0,10);
	}
}RW_CMD_REFUND,*LPRW_CMD_REFUND;

// RW��������ṹ
typedef struct _rw_cmd_block
{
	DWORD SerialNo;				// ������ˮ��
	_DATE_TIME TranDateTime;	// ����ʱ��
	_rw_cmd_block()
	{
		SerialNo = 0;	
	}
}RW_CMD_BLOCK,*LPRW_CMD_BLOCK;

// RW��������ṹ
typedef struct _rw_cmd_counteract
{
	DWORD SerialNo;				// ������ˮ��
	_DATE_TIME TranDateTime;	// ����ʱ��
	BYTE CardType;	  		    // ��Ƭ���࣬0x01:һ��ͨS50/S70��Ƭ��0x02:һ��ͨCPU����0x11:һƱͨS50/S70��Ƭ��0x12:һƱͨUL��
	WORD ProductyType;			// ��Ʒ����
	WORD ReasonCode;			// ����ԭ�����
	_rw_cmd_counteract()
	{
		SerialNo = 0;	
		CardType = 0;
		ProductyType = 0;
		ReasonCode = 0;
	}
}RW_CMD_COUNTERACT,*LPRW_CMD_COUNTERACT;

// RW EP֧������ṹ
typedef struct _rw_cmd_consume{
	DWORD		SerialNo;		// ������ˮ��
	_DATE_TIME	TranDateTime;	// ����ʱ��
	DWORD		ConsumeAmount;  // ���׽��
	_rw_cmd_consume(){
		SerialNo = 0;
		ConsumeAmount = 0;
	}
}RW_CMD_CONSUME;

//////////////////////////////////////////////����Ϊ��д���������ݽṹ����/////////////////////////////////////////
// RW����״̬����
typedef BYTE RW_RSP_STATUS; 

// RW��ȡTPU��Ϣ����Ӧ�����ݽṹ
typedef struct _rw_rsp_tpu_info
{
	BYTE DeviceCode[4];
	BYTE hardwareVersion[2];                               // Ӳ���汾
	BYTE softwareVersionApp[6];                            // Ӧ�ò�����汾
	BYTE softwareVersionMain[4];                           // ����������汾
	BYTE accPSAMid[6];
	BYTE accISAMid[6];
	BYTE bmacPSAMid[6];
	BYTE bmacISAMid[6];

	_rw_rsp_tpu_info()
	{
		memset(DeviceCode,0,6);
		memset(hardwareVersion,0,2);
		memset(softwareVersionApp,0,6);
		memset(softwareVersionMain,0,4);
		memset(accPSAMid,0,6);
		memset(accISAMid,0,6);
		memset(bmacPSAMid,0,6);
		memset(bmacISAMid,0,6);
	}
} RW_RSP_TPU_INFO, *LPRW_RSP_TPU_INFO;

// ��ȡ�����汾�����ṹ
typedef struct _rw_rsp_param_info
{
	WORD			ParaCount;                  // ��������
	DAT::PARAINFO	ParamData[25];                  // ��������ָ�룬��д������

	_rw_rsp_param_info(){
		ParaCount = 0;
		memset(ParamData,0,sizeof(ParamData));
	}
} RW_RSP_PARAM_INFO,*LPRW_RSP_PARAM_INFO;

// RW����������Ʒ��Ϣ�ṹ
typedef struct _RECHARGEPRODUCT
{
	WORD ProductType;//��Ʒ����
	WORD ProductSubType;//��Ʒ������
	ulong RefoundMoney;//���˽��
	bool RechargeMark;//�Ƿ�ɳ�ֵ   false�����ɳ�ֵ��true���ɳ�ֵ
}RECHARGEPRODUCT;

// RW����������Ϣ
typedef struct _rw_read_card_response
{
	int DetailErrorCode;			//��ϸ�������
	BYTE CardPHType;				//��Ʊ�������� 0:Ԥ��	1:Ultralight  2:Mifare1K  3:CPUƱ��
	WORD CardInfoLen;				//��Ƭ��Ϣ�ĳ���
	BYTE PaidAreaAdjustCode;		//��������Ʊ�������
	BYTE UnPaidAreaAdjustCode;		//�Ƿ�������Ʊ�������
	DWORD AdjustAmount;				//���鲹Ʊ���

	BYTE ProductCount;//��Ʒ����
	RECHARGEPRODUCT RechargeProduct[10];
	BYTE SalableProductType;        //���۲�Ʒ����
	//��BITλ�������ӵ͵���λ���δ���Ǯ�����ƴΣ�����
	//Ǯ����00000001 =0x01
	//�ƴΣ�00000010 = 0x02
	//Ǯ��+�ƴ� = 00000011 = 0x03
	//���ڣ�00000100 = 0x04
	//Ǯ��+���� = 00000101 = 0x05
	//Ǯ��+�ƴ�+���� = 00000111 = 0x07

	BYTE SalableProductCount;       //���۲�Ʒ����
	BYTE AdjustPaymentMethod;		//��Ʊʱ֧����ʽ
	DWORD ReturnCardFee;            //������Ʊ�����ѽ��
	bool  isBigMoneyCard;			//�Ƿ�Ϊ�� true:�� false:��

	BYTE CardInfo[RW_MSG_CARD_INFO_LEN];	//����Ϣ,���ݲ�ͬ�����ͷ��ز�ͬ����Ϣ
	BYTE LogCount;							//���׼�¼����
	BYTE LogInfo[256];		//���׼�¼����



	_rw_read_card_response()
	{
		DetailErrorCode = 0;
		CardPHType = 0;
		CardInfoLen = 0;
		PaidAreaAdjustCode = 0;	
		UnPaidAreaAdjustCode = 0;	
		ProductCount = 0;
		AdjustAmount = 0;		
		ReturnCardFee = 0;			
		isBigMoneyCard = false;	
		memset(RechargeProduct,0,sizeof(RECHARGEPRODUCT)*10); 
		SalableProductType = 0;
		SalableProductCount = 0;
		memset(CardInfo,0,RW_MSG_CARD_INFO_LEN); 
		LogCount = 0;
		memset(LogInfo,0,256);
	}
} RW_READ_CARD_RESPONSE, *LPRW_READ_CARD_RESPONSE;



// RWҵ����ͨ�ýṹ
typedef struct _rw_bussiness_response
{
	int  DetailErrorCode;			// ��ϸ�������
	BYTE SerialAddnum;			    // UDSN��һ��ͨ������ˮ���ۼ�ֵ
	DAT::TRANRESULT TranResult;		// TPUҵ����󷵻صĽ������(ͬ��Ʊ)
	WORD UDlen;						// UD���ݳ��� ����Ϊ0��ʾ�����UD���ݲ�����
	BYTE UDData[BUF_SIZE];			// UD����
	WORD ARLen;						// AR���ݳ��� ����Ϊ0��ʾ�����AR���ݲ�����
	DAT::ARDATA ARData;				// AR����
	_rw_bussiness_response()
	{
		SerialAddnum = 0;	
		UDlen = 0;					
		memset(UDData,0,sizeof(UDData));	
		ARData.ARnum = 0;	
		memset(ARData.ARdata,0,sizeof(ARData.ARdata));
	}
} RW_BUSSINESS_RESPONSE, *LPRW_BUSSINESS_RESPONSE;

// RW�������ݽṹ��
typedef struct _rw_cmd_active
{
	_DATE_TIME TranDateTime;            // ���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
	uchar Category;                 // ��Ƭ���࣬������Ƭ��Ӧ�ô����0x01��һ��ͨS50/S70��Ƭ��0x02��һ��ͨCPU����0x11��һƱͨS50/S70��Ƭ��0x12��һƱͨUL��
	ushort ProductyCategory;        // ��Ʊ��Ʒ����	
	_rw_cmd_active()
	{
		Category = 0;
		ProductyCategory = 0;	
	}
} RW_CMD_ACTIVE, *LPRW_CMD_ACTIVE;

// RW�������ݽṹ��
typedef struct _rw_cmd_defer
{
	DWORD SerialNo;						// ������ˮ��
	_DATE_TIME TranDateTime;            // ���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
	DWORD ServiceFee;					// ������
	WORD DeferTime;						// ��Ч��˳�ӵĳ��ȣ�����Ϊ��λ
	_rw_cmd_defer()
	{
		SerialNo = 0;
		ServiceFee = 0;
		DeferTime = 0;
	}
} RW_CMD_DEFER, *LPRW_CMD_DEFER;

//  RW�滻���ݽṹ��
typedef struct _rw_cmd_replace_card
{

	_DATE_TIME TranDateTime;             
	DWORD Serialnum;			    // UDSN��һ��ͨ������ˮ��	
	_rw_cmd_replace_card()
	{
		Serialnum = 0;	
	}
} RW_CMD_REPLACE_CARD, *LPRW_CMD_REPLACE_CARD;

//  RW�޿�/�����滻���ݽṹ��
typedef struct _rw_cmd_no_card_replace
{
	typedef struct _ProductDetailInfo
	{
		WORD ProductType;
		WORD ProductSubType;
		BYTE ProductCategory;
		_DATE_TIME BeginTime;
		BYTE TypeOfExpiryDate;
		WORD DurationOfExpiryDate;
		BYTE ActivedFlag;
		BYTE ProductStatus;
		BYTE ProductIssuerId;
		DWORD Balance;
		WORD TransactionSerialNo;
		WORD BeginSection;
		WORD EndSection;
		DWORD SamId;
		WORD CountNumber;
		BYTE PriorityOfUse;
	}ProductDetailInfo;

	typedef struct _ProductInfo
	{
		BYTE ProductCount;
		ProductDetailInfo m_ProductDetailInfo[3];
		WORD CompanyId;						
		DWORD StaffId;
		char PassengerName[8];//???
		BYTE CertificateType;
		BYTE CertificateNum[9];
		_DATE PassengerBirth;
		BYTE TravelStastus;
		BYTE LastUseProduct;
		WORD EntryStation;
		_DATE_TIME EntryTime;
		WORD LastUseStation;
		_DATE_TIME LastUseTime;
	}ProductInfo;

	DWORD Serialnum;					// 
	_DATE_TIME TranDateTime;
	ProductInfo transData;
}RW_CMD_NO_CARD_REPLACE;

// RW��������Ϣ�޸�����
typedef struct _rw_cmd_update_signcard_info
{
	DWORD SerialNo;						// ������ˮ��
	_DATE_TIME TranDateTime;			// ����ʱ��
	BYTE CardholderType;				// �ֿ������ͱ�ʶ
	BYTE StaffType;						// �ֿ���ְ����ʶ
	BYTE Name[20];						// �ֿ�������
	BYTE CertificateNo[32];				// �ֿ���֤������
	BYTE CertificateType;				// �ֿ���֤������
	WORD Deposit;						// Ѻ����֣�
	BYTE SexType;						// �ֿ����Ա�
	BYTE AppVer;						// Ӧ�ð汾
	_DATE_TIME CertificateValidDate;	// ֤����Ч��
	BYTE Tel[10];						// �ֿ��˵绰	10 	BCD	
	BYTE Email[60];						// �ֿ���Email	60	ASCII
	BYTE Addr1[128];					// �ֿ��˵�ַ1	128	ASCII
	BYTE Addr2[128];					// �ֿ��˵�ַ2	128	ASCII
	_rw_cmd_update_signcard_info()
	{
		SerialNo = 0;
		CardholderType = 0;
		StaffType = 0;
		memset(Name,0,20);
		memset(CertificateNo,0,32);
		CertificateType = 0;
		Deposit = 0;
		SexType = 0;
		AppVer = 0;
		memset(Tel,0,10);
		memset(Email,0,60);
		memset(Addr1,0,128);
		memset(Addr2,0,128);
	}
}RW_CMD_UPDATE_SIGNCARD_INFO,*LPRW_CMD_UPDATE_SIGNCARD_INFO;

// RW���Ի���Ϣ����
typedef struct _rw_cmd_personalize_info
{
	DWORD SerialNo;						// ������ˮ��		4	BIN ACC�����Ψһ�Ľ�����š���1��ʼ�������ۼӣ���Χ1��4294967295���ﵽ���ֵ���1��ʼ������ʱת����INTEL��
	_DATE_TIME TranDateTime;			// ����ʱ��			7	BCD	���׷���������ʱ��
	BYTE OperationType;					// ��������			1	BIN 1�����Ի���ԭ�����£����ף�2�����Ի�����-���ɿ�����������Ϊ2ʱ6��16�ֶ������壩��3�����Ի�����-д�¿�
	BYTE CorporationCode[2];				// ��˾����			2	BCD	0001��9999
	BYTE StaffNumber[4];					// Ա�����			4	BCD	00000001 �� 99999999
	BYTE CardHolderName[8];				// �ֿ�������		8	ASCII	���4����
	BYTE CertificateType;				// ֤������			1	BIN	1�����֤��2������
	BYTE CertificateID[9];				// ֤�����			9	BCD	���֤����
	_DATE Birthday;				// ����				4	BCD	YYYYMMDD
	BYTE PassengerLevel;				// �˿ͼ���Ԥ����	1	BIN	�̶�0
	DWORD PaymentAmount;				// ֧�����			4	BIN	
	BYTE PaymentMethod;					// ֧����ʽ			1	BIN	�μ���Macau-06 System Initial Values .doc����PaymentMethod_t�Ķ��塣
	BYTE BankCarkNumber[10];			// ���п���			10	BCD	֧�����п�����
	_rw_cmd_personalize_info()
	{
		SerialNo = 0;
		OperationType = 0;
		memset(CorporationCode, 0, sizeof(CorporationCode));
		memset(StaffNumber, 0, sizeof(StaffNumber));
		memset(CardHolderName,0,61);
		CertificateType = 0;
		memset(CertificateID,0,18);
		PassengerLevel = 0;
		PaymentAmount = 0;
		PaymentMethod = 0;
		memset(BankCarkNumber,0,10);
	}
}RW_CMD_PERSONALIZE_INFO,*LPRW_CMD_PERSONALIZE_INFO;

// ����һƱͨMAC�ṹ��
typedef struct _rw_cmd_calculate_mac
{
	WORD keyVersion;           // ����MAC��Ҫ����Կ�汾
	BYTE macCalculate[20];     // ����MAC��Ҫ��Hash����
	_rw_cmd_calculate_mac(){
		keyVersion = 0;
		memset(macCalculate,0,20);
	}
}RW_CMD_CALCULATE_MAC,*LPRW_CMD_CALCULATE_MAC;

// ES��PMд����Ϣ
typedef struct _rw_cmd_es_pm_write_card_info{
	DWORD DealNo;						// ������ˮ��
	_DATE_TIME DealTime;				// ����ʱ��
	BYTE OperitionMode;					// ��������
	//һ���ֶ����ڳ�ʼ�����ر����ע��
	BYTE FormVisionNO;					// ��ʽ�汾��
	BYTE ManufID;						// ����ID
	BYTE KeyEdition;					// ��Կ�汾
	WORD CityCode;						// ���д���
	WORD TradeCode;						// ��ҵ����
	BYTE SupplyTime[3];					// ����ʱ��
	WORD SupplyDatch;					// ��������
	DWORD CardLogicalNumber;			// �߼�����	4	BIN	��Ʊ�߼���� 
	BYTE Reserve1[4];					// Ԥ��

	//�����ֶ�����Ԥ��ֵ
	BYTE ReleaseID;						// ��Ʒ������ID
	BYTE CardType;						// ��Ƭ��������
	BYTE ProductCategory;				// ��Ʒ���
	BYTE ProductType;					// ��Ʒ����
	BYTE ProductSubtype;				// ��Ʒ������
	BYTE PassengerType;					// �˿�����
	DWORD CardAmount;					// ���ڽ��
	DWORD CardTimes;					// ���ڴ���
	BYTE ValidityPeriod;				// ��Ч�ڼ�
	BYTE ValidityPeriodUnit;			// ��Ч�ڼ䵥λ
	DWORD CardDeposit;					// Ѻ��
	DWORD BookingAmount;				// ��Ʊ���
	_DATE_TIME VolidStartTime;			// ��Ч��ʼ����
	BYTE Reserve2[8];					// Ԥ��
	// �������ڸ��Ի�����
	WORD CorporationCode;				// ��˾����
	DWORD PersonalNO;					// Ա�����
	BYTE CardHolderName[8];				// �ֿ�������
	BYTE CredentialsNOType;				// ֤����������
	BYTE CredentialsNO[9];				// ֤������
	DWORD Birthday;						// ����
	BYTE PassengerLevel;				// �˿ͼ���
	BYTE Reserve[8];					// Ԥ��
	_rw_cmd_es_pm_write_card_info()
	{
		DealNo = 0;						// ������ˮ��
		OperitionMode = 0;					// ��������
		//һ���ֶ����ڳ�ʼ�����ر����ע��
		FormVisionNO = 0;					// ��ʽ�汾��
		ManufID = 0;						// ����ID
		KeyEdition = 0;					// ��Կ�汾
		CityCode = 0;						// ���д���
		TradeCode = 0;						// ��ҵ����
		memset(SupplyTime,0x00,3);
		SupplyDatch = 0;					// ��������
		CardLogicalNumber = 0;				// �߼����
		memset(Reserve1,0xff,4);
		//�����ֶ�����Ԥ��ֵ
		ReleaseID = 0;						// ��Ʒ������ID
		CardType = 0;						// ��Ƭ��������
		ProductCategory = 0;				// ��Ʒ���
		ProductType = 0;					// ��Ʒ����
		ProductSubtype = 0;				// ��Ʒ������
		PassengerType = 0;					// �˿�����
		CardAmount = 0;					// ���ڽ��
		CardTimes = 0;					// ���ڴ���
		ValidityPeriod = 0;				// ��Ч�ڼ�
		ValidityPeriodUnit = 0;			// ��Ч�ڼ䵥λ
		CardDeposit = 0;					// Ѻ��
		BookingAmount = 0;				// ��Ʊ���
		memset(Reserve2,0xff,8);
		// �������ڸ��Ի�����
		CorporationCode = 0;				// ��˾����
		PersonalNO = 0;					// Ա�����
		memset(CardHolderName,0,8);
		CredentialsNOType = 0;				// ֤����������
		memset(CredentialsNO,0,9);
		Birthday = 0;						// ����
		PassengerLevel = 0;				// �˿ͼ���
		memset(Reserve,0xff,8);
	}
}RW_CMD_ES_PM_WRITE_CARD_INFO;

//����ĸ���ⲿ��֤����
typedef struct _rw_authissue_card_master_card_param
{
	BYTE ICMC_Slot;//���п�ĸ���Ŀ���
	BYTE ICMCT_Slot;//���п�ĸ�����俨�Ŀ���
	BYTE ICMCT_PIN_Len;//PIN��ĳ���
	BYTE ICMCT_PIN_buff[2];//���п�ĸ�����俨��PIN��
}AuthIssueCardMasterCardParam;

// SAM ��λ�ö���
typedef enum _Sam_card_pos_{
	SAM_TYPE_NONE = 0,		// ��SAM��		0x00
	SAM_TYPE_YPT_ISAM,		// һƱͨISAM	0x01
	SAM_TYPE_YPT_PASM,		// һƱͨPSAM	0x02
	SAM_TYPE_YKT_ISAM,		// һ��ͨISAM	0x03
	SAM_TYPE_YKT_PSAM,		// һ��ͨPSAM	0x04
}SAM_POS_DEF;