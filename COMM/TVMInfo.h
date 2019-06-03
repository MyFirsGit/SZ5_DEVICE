#pragma once
#include "GetIni.h"
#include "Util.h"
#include "delegate.h"
#include "CommonDefine.h"
#include "SysInfo.h"

#define theTVM_INFO CTVMInfo::GetInstance()

/** 
@brief   TVM.INI�ļ�������
*/
class COMMON_API CTVMInfo : public CSYSInfo
{
public:

    static CTVMInfo&  GetInstance();

	//[��ʱ����Ϣ����λ���룩]
	int GetFinishToReadyTimerInterval();	    				// [����]->[�ȴ���]��ʱ��ʱ����
	int GetReceptToReadyTimerInterval();						// [������]->[�ȴ���]��ʱ��ʱ����
	int GetCountingToReturnTimerInterval(); 					// [������]->[������]��ʱ��ʱ����
	int GetReadyToRestTimerInterval();							// [�ȴ���]->[������]��ʱ��ʱ����
	int GetReturnIndicatorTimerInterval();						// [��]->[��]����ָʾ��
	int GetReturnLightTimerInterval();							// [��]->[��]����������

	// [��д��ͨѶ����]
	bool   IsRWUsed(RW_TYPE rwType);							// ��д���Ƿ�����
	bool   IsUSBRW(RW_TYPE rwType);							// ��д���Ƿ����USBͨ��
	int     GetRWCommPort(RW_TYPE rwType);				// ��ȡ��д�����ں�
	int     GetRWBaudRate(RW_TYPE rwType);					// ��ȡ��д�����ڲ�����
	int     GetRWDownLoadCnt(RW_TYPE rwType);			// ��ȡ��д�������������Դ���
	int     GetRWTimeout(RW_TYPE rwType);					// ��ȡ��д��������ʱʱ��
	bool	 IsRWMonitorOpened(RW_TYPE rwType);			// �Ƿ�����д�����
	int     GetRWMonitorInterval(RW_TYPE rwType);		// ��д�����ʱ����

	//[������RFIDͨѶ����]
	bool IsZLGRfidUsed();														// ֽ�ҽ���ģ���Ƿ�����
	int GetZLGRfidCommPort();												// ��ȡֽ�ҽ���ģ�鴮�ں�
	int GetZLGRfidBaudRate();												// ��ȡֽ�ҽ���ģ�鴮�ڲ�����
	bool IsZLGRfidMonitorOpened();										// �Ƿ���ֽ�ҽ���ģ����
	int GetZLGRfidMonitorInterval();										// ֽ�ҽ���ģ����ʱ����

	// [ֽ�ҽ���ģ��ͨѶ����]
	bool IsBHUsed();														// ֽ�ҽ���ģ���Ƿ�����
	int GetBHCommPort();												// ��ȡֽ�ҽ���ģ�鴮�ں�
	int GetBHBaudRate();												// ��ȡֽ�ҽ���ģ�鴮�ڲ�����
	bool IsBHMonitorOpened();										// �Ƿ���ֽ�ҽ���ģ����
	int GetBHMonitorInterval();										// ֽ�ҽ���ģ����ʱ����

	// [Ʊ��ģ��ͨ�Ų���]
	int GetTHCommuMethod();
	bool IsTHUsed();
	int GetTHCommPort();
	int GetTHBaudRate();
	int GetTHMaxConnReceive();
	int GetTHMaxConnSend();
	bool IsTHMonitorOpened();
	int GetTHMonitorInterval();
	int GetTHTestRelease();
	int GetTHTestRecycle();
	int GetTHBoxMediumType();

	// [TOKENģ��ͨ�Ų���]
	bool IsTokenUsed();
	int GetTokenCommPort();
	int GetTokenBaudRate();
	int GetTokenMaxConnReceive();
	int GetTokenMaxConnSend();
	bool IsTokenTHMonitorOpened();
	int GetTokenMonitorInterval();
	int GetTokenTestRelease();
	int GetTokenTestRecycle();
	int GetTokenBoxMediumType();
	int GetTokenBoxAMaxCount();									// Token��A���װƱ����
	int GetTokenBoxBMaxCount();									// Tolen��B���װƱ����
	int GetTokenHopperAMaxCount();								// Token Hopper A���������
	int GetTokenHopperBMaxCount();								// Token Hopper B���������

	// [Ӳ��ģ�����]
	bool IsCHUsed();
	int GetCHCommPort();
	int GetCHBaudRate();
	bool IsCHMonitorOpened();
	int GetCHMonitorInterval();
	// Hopper1���
	int GetCHHopper1Type();      // Hopper B����
	int GetCHHopper1MaxCount();
	int GetCHHopper1NearEmpty();
	int GetCHHopper1Empty();
	
	// Hopper2���
	int GetCHHopper2Type();		 // Hopper A����
	int GetCHHopper2MaxCount();
	int GetCHHopper2NearEmpty();
	int GetCHHopper2Empty();
	 
	// �ݴ�
	int GetCHTemporary1Max();
	int GetCHTemporary2Max();

	// ��������
	int GetLimitOneCount();
	int GetLimitHalfCount();

	// ������A
	int GetCHChangeBoxAType();	
	int GetCHChangeBoxAMax();
	int GetCHChangeBoxANearEmpty();
	int GetCHChangeBoxAEmpty();

	// ������B
	int GetCHChangeBoxBType();
	int GetCHChangeBoxBMax();
	int GetCHChangeBoxBNearEmpty();
	int GetCHChangeBoxBEmpty();
	
	// ������֣�ݣ�ɾ��
	//int GetCHStockMoneyType();
	//int GetCHStock1Max();
	//int GetCHTemporary1Max();
	//int GetCHTemporary2Max();
	//int GetCHStockEmpty();
	//int GetCHStockNearEmpty();
	//int GetCHHopper1Type();
	//int GetCHHopper1Max();
	//int GetCHHopper1AddMax();
	//int GetCHHopper1Empty();
	//int GetCHHopper1NearEmpty();
	//int GetCHHopper2Type();
	//int GetCHHopper2Max();
	//int GetCHHopper2AddMax();
	//int GetCHHopper2Empty();
	//int GetCHHopper2NearEmpty();
	//int GetCollectCoinPriorityforHopper1();			// ��ȡӲ��ר��������AӲ�һ������ȼ�˳��
	//int GetCollectDirectionforHopper1();			// ��ȡӲ��ר��������AӲ�һ��ս���Ӳ�һ�������
	//int GetCollectCoinPriorityforHopper2();			// ��ȡӲ��ר��������BӲ�һ������ȼ�˳��
	//int GetCollectDirectionforHopper2();			// ��ȡӲ��ר��������BӲ�һ��ս���Ӳ�һ�������
	//int GetCollectCoinPriorityforStock();			// ��ȡӲ��ѭ��������Ӳ�һ������ȼ�˳��
	//int GetCollectDirectionforStock();				// ��ȡӲ��ѭ��������Ӳ�һ��ս���Ӳ�һ�������

    // [ƾ֤��ӡ��ͨѶ����]
	bool    IsPrinterUsed();
    int     GetPrinterCommPort();                 // port open
    int     GetPrinterBaudRate();                   // Baud Rate (bps)
	bool	IsPrinterMonitorOpened();
	int		GetPrinterMonitorInternal();		
	// [IOͨѶ����]
	bool IsIOUsed();										// IO�Ƿ�����
	int GetIOCommPort();								// ��ȡIO���ں�
	int GetIOBaudRate();									// ��ȡIO������
	bool IsIOMonitorOpened();						// �Ƿ�����IO���
	int GetIOMonitorInterval();						// ��ȡIO���ʱ����

	// [Ӳ�ҿڿ���ͨѶ����]
	bool IsCoinShutterUsed();
	int  GetCoinShutterCommPort();
	int  GetCoinShutterBaudRate();
	bool IsCoinShutterMonitorOpened();
	int  GetCoinShutterMonitorInterval();

	// [�������ͨѶ����]
	bool IsPinPadUsed();
	int  GetPinPadCommPort();
	int  GetPinPadBaudRate();
	bool IsPinPadMonitorOpened();
	int  GetPinPadMonitorInterval();

	// [���п�ģ��ͨѶ����]
	bool IsBankCardUsed();
	int  GetBankCardCommPort();
	int  GetBankCardBaudRate();
	bool IsBankCardMonitorOpened();
	int  GetBankCardMonitorInterval();

	// [״̬��ʾ��ͨѶ����]
	bool IsSdspUsed();
	int GetSdspCommPort();
	int GetSdspBaudRate();
	bool IsSdspMonitorOpened();
	int GetSdspMonitorInterval();
	int GetSdspMonitorBrightness();
	int GetSdspMonitorMoveSpeed();
	int GetSdspMonitorStayStyle();
	int GetSdspMonitorStayTime();
	int GetSdspMonitorMoveIn();
	int GetSdspMonitorMoveOut();

	// ֽ������ģ��ͨѶ����
	bool IsBDHUsed();
	int  GetBDHCommPort();
	int  GetBDHBaudRate();
	bool IsBDHMonitorOpened();
	int  GetBDHMonitorInterval();
	int GetBHChangeBoxAType();
	int GetBHChangeBoxBType();
	int GetBHChangeBoxNearFull();
	int GetBHChangeBoxFull();

	// [��־]
	CString GetLogPath();
	// ��ȡ�豸��������
	CString GetDeviceComponentCode(AFC_DEVICE_TYPE deviceType,DEVICE_SUBTYPE deviceSubType,long moudleID);

	// [����Ŀ¼]
	CString GetDataPath();
	CString GetBackupPath();

	// �������������ݵķ���
	// [TVM_RECHARGE_RW]
	void SetRechargeIsRWUsed(const CString&);   //��ֵ��д���Ƿ�����

	//	[TVM_CARD_RW]
	void SetSmartCardIsRWUsed(const CString&);  //Smart-Card��д���Ƿ�����

	//	[TVM_TOKEN_RW]
	void SetTokenIsRWUsed(const CString&);		//Token��д���Ƿ�����

	//	[BANKNOTE_ACCEPT_MODULE]]
	void SetBankNoteIsUsed(const CString&);		//ֽ�Ҵ���ģ���Ƿ�����

	//	[BANKNOTE_CHANGE_MODULE]]
	void SetBankNoteChangeIsUsed(const CString&);//ֽ������ģ���Ƿ�����

	//	[CARD_MODULE]
	void SetSmartCardIsUsed(const CString&);	//Smart-Cardģ���Ƿ�����

	//	[TOKEN_MODULE]
	void SetTokenIsUsed(const CString&);		//Tokenģ���Ƿ�����

	//	[COIN_MODULE]
	void SetCoinIsUsed(const CString&);			//Ӳ��ģ���Ƿ�����

	//	[RP_COMMUNICATE]
	void SetReceiptIsUsed(const CString&);		//ƾ֤��ӡ���Ƿ�����

	//	[PIN_PAD_COMMUNICATE]
	void SetPinPadIsUsed(const CString&);		//�������ģ��

	//	[BANK_CARD_COMMUNICATE]
	void SetBankCardIsUsed(const CString&);		//���п�ģ��

	int GetParameterUpdateBeginHour() const;
	int GetParameterUpdateBeginMin() const;
	int GetParameterUpdateEndHour() const;
	int GetParameterUpdateEndMin() const;
private:
    static CTVMInfo theInstance;

    // ���ع��캯��
    CTVMInfo();
    ~CTVMInfo();
    CTVMInfo(const CTVMInfo&);
    CTVMInfo& operator=(const CTVMInfo&);

	CString m_strDataPath;						// ��������Ŀ¼
	CString m_strDataBackpath;					// �������ݱ���Ŀ¼
	CString m_strLogPath;						// ������־Ŀ¼
};