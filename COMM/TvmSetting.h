#pragma once
#include "GetIni.h"
//#include "Util.h"
#include "delegate.h"
#include "CommonDefine.h"
#include "CommonExp.h"

#define theTVM_SETTING CTvmSetting::GetInstance()

class COMMON_API CTvmSetting
{
public:
	static CTvmSetting&  GetInstance();
	int Initialize();                              // ���ڻ�
	delegate<void()> Initialized;

	// === TVMϵͳ��Ϣ ===
	int     GetDelayOperationTime();							// ��ȡ�ӳ���Ӫʱ�䣨��λ:�룩

	// === FTP�ϴ�·�� ===
	CString GetFileUploadFTPDir();				// ��ȡ�ļ��ϴ�·��
	void SetFileUploadFTPDir(CString);			// �����ļ��ϴ�·��

	// === ��־��Ϣ ===
	bool  IsProgFirstRun();						// �����Ƿ��ǳ��ΰ�װ����һ������
	void SetIsFirstRun(bool bFirst);

	int     GetUpdateFlag();							// ��ȡ������±�־
	int     GetACCUpdateFlag();					// ��ȡACC����ʧ�ܱ�־
	// [ICRW]
	int GetRechargeRWUpdateFlag();				// ��ȡ��д��UpdateError
	void SetRechargeRWUpdateFlag(int);		// ���ö�д��UpdateError
	// [CardICRW]
	int GetCardRWUpdateFlag();						// ��ȡ��д��UpdateError
	void SetCardRWUpdateFlag(int);				// ���ö�д��UpdateError
	// [TokenICRW]
	int GetTokenRWUpdateFlag();					// ��ȡ��д��UpdateError
	void SetTokenRWUpdateFlag(int);				// ���ö�д��UpdateError
	// === ʱ���趨 ===
	int GetTimeChangeValue();					// ��ȡʱ��ı��־value
	void SetTimeChangeValue(int);				// ����ʱ��ı��־value
	CString GetCurSystemTime();					// ��ȡ��ǰϵͳʱ��
	void SetCurSystemTime(CString);				// ���õ�ǰϵͳʱ��
	// === ��Ļ�����趨 ===
	int GetScreenBrightnessValue();				// ��ȡ��Ļ����
	void SetScreenBrightnessValue(int);			// ������Ļ����
	// === ����ģʽ�趨 ===
	//void SetDegradeMode(bool);					//	�����Ƿ�����������
	//bool	IsAllowDegrade();					//	�ж��Ƿ�������	

	int	GetServiceMode();						//	��ȡҵ��ģʽ
	void SetServiceMode(int);					//	����ҵ��ģʽ

	int GetPrintMode();							//	��ȡ��ӡģʽ
	void SetPrintMode(int);						//  ���ô�ӡģʽ

	int GetPayMentMode();						//	��ȡ֧��ģʽ
	void SetPayMentMode(int);					//	����֧��ģʽ

	int	GetChangeMode();						//	��ȡ����ģʽ
	void SetChangeMode(int);					//	��������ģʽ

	bool IsSCServiceOutOfService();				//  �Ƿ���λ�·���ͣ����״̬
	void SetSCServiceStatus(int);				//  ������λ�·�����״̬

	bool IsStartOpeartion();					// �Ƿ���Ӫ��ʼ��
	void SetStartOperation(bool);				// �����Ƿ��˿�ʼ

	// [ICRW]/[ICTW]
	int GetRWUpdateFlag(bool bIsRW = true);		// ��ȡ��д��UpdateError
	void SetRWUpdateFlag(int,bool bIsRW = true);// ���ö�д��UpdateError

	void SetDeviceTestMode(int);				// �����豸����ģʽ
	int GetDeviceTestMode();					// ��ȡ�豸����ģʽ

	bool IsAllowDegrade();						//  �Ƿ������Զ�����ģʽ
	void SetDeradedMode(int);					//  ���������Զ�����ģʽ

	int  GetSettleSettings();					// ��ȡһ���ջ�������
	void SetSettleSettings(int);


	int GetCardBoxLastAction();
	void SetCardBoxLastAction(int);

	int GetCollectTokenResult();
	void SetCollectTokenResult(int);

	int GetCoinBoxLastAction();
	void SetCoinBoxLastAction(int);

	int GetCollectCoinResult();
	void SetCollectCoinResult(int);

	int GetBanknoteBoxLastAction();
	void SetBanknoteBoxLastAction(int);

	int GetCollectBanknoteResult();
	void SetCollectBanknoteResult(int);

	CString GetBalanceOpTime();
	void SetBalanceOpTime(CString);

	// [TH]
	int GetTHMode();							// ��ȡ��Ʊ��ģʽ
	void SetTHMode(int);						// ���ó�Ʊ��ģʽ

	_DATE GetLastOnlineDate();					//  ��ȡ�ϴ�����ʱ�䣨�����գ�
	void SetLastOnlineDate(_DATE dtDay);		//  �����ϴ�����ʱ�䣨�����գ�

	int GetMaxOffLineDay();						//  ȡ���������Ӫ����
	void SetMaxOffLineDay(int nDays);			//  �������������Ӫ����
	/*
	     ���ں�����
	*/
	// ��д��[TVM_CHARGE_RW]
	int GetChargeRWComPort();
	int GetChargeRWBaudRate();
	// ��д�� [TVM_CARD_RW]
	int GetRWComPort();								// ��ȡ��д�����ں�
	int GetRWBaudRate();							// ��ȡ��д�����ڲ�����

	void SetRwComPort(const CString&);				// ���ö�д�����ں�
	void SetRwBaudRate(const CString&);				// ���ö�д��������

	// ֽ�ҽ���ģ��
	int GetBHComPort();								// ��ȡֽ�ҽ���ģ�鴮�ں�
	int GetBHBaudRate();							// ��ȡֽ�ҽ���ģ�鴮�ڲ�����

	void SetBHComPort(const CString&);				// ����ֽ�ҽ���ģ�鴮�ں�
	void SetBHBaudRate(const CString&);				// ����ֽ�ҽ���ģ�鲨����

	// Ʊ��ģ��
	int GetTHComPort();								// ��ȡƱ��ģ�鴮�ں�
	int GetTHBaudRate();							// ��ȡƱ��ģ�鲨����
	int GetTHUpadateBaudRate();						// ��ȡƱ��ģ�����ʱ������

	void SetTHComPort(const CString&);				// ����Ʊ��ģ�鴮�ں�
	void SetTHBaudRate(const CString&);				// ����Ʊ��ģ�鲨����

	// tokenģ��
	int GetTokenComPort();							// ��ȡƱ��ģ�鴮�ں�
	int GetTokenBaudRate();							// ��ȡƱ��ģ�鲨����
	int GetTokenUpadateBaudRate();					// ��ȡƱ��ģ�����ʱ������

	void SetTokenComPort(const CString&);				// ����Ʊ��ģ�鴮�ں�
	void SetTokenBaudRate(const CString&);				// ����Ʊ��ģ�鲨����

	// token������ģ��
	int GetTokenCyclerComPort();						// ��ȡtoken������ģ�鴮�ں�
	int GetTokenCyclerBaudRate();						// ��ȡtoken������ģ�鲨����

	void SetTokenCyclerComPort(const CString&);			// ����token������ģ�鴮�ں�
	void SetTokenCyclerBaudRate(const CString&);		// ����token������ģ�鲨����


	// Ӳ��ģ��
	int GetCHComPort();								// ��ȡӲ��ģ�鴮�ں�
	int GetCHBaudRate();							// ��ȡӲ��ģ�鲨����

	int SetCHComPort(const CString&);				// ����Ӳ��ģ�鴮�ں�
	int SetCHBaudRate(const CString&);				// ����Ӳ��ģ�鲨����

	// Ӳ��բ��ģ��
	int GetCSHComPort();
	int GetCSHBaudRate();
	int SetCSHComPort(const CString&);				// ����Ӳ��բ��ģ�鴮�ں�
	int SetCSHBaudRate(const CString&);				// ����Ӳ��բ��ģ�鲨����

	// ��ӡ��ģ��
	int GetPrinterComPort();						// ��ȡ��ӡ��ģ�鴮�ں�
	int GetPrinterBaudRate();						// ��ȡ��ӡ��ģ�鲨����

	void SetPrinterComPort(const CString&);			// ���ô�ӡ��ģ�鴮�ں�
	void SetPrinterBaudRate(const CString&);		// ���ô�ӡ��ģ�鲨����

	// IO
	int GetIOComPort();
	int GetIOBaudRate();

	void SetIOComPort(const CString&);
	void SetIOBaudRate(const CString&);

	// ״̬��ʾ��
	int GetDisplayComPort();						// ��ȡ״̬��ʾ�����ں�
	int GetDisplayBaudRate();						// ��ȡ״̬��ʾ��������

	void SetDisplayComPort(const CString&);			// ����״̬��ʾ�����ں�
	void SetDisplayBaudRate(const CString&);		// ����״̬��ʾ��������

	// ֽ������ģ��
	int  GetBanknoteChangeComPort();				// ��ȡֽ������ģ�鴮�ں�
	int  GetBanknoteChangeBaudRate();				// ��ȡֽ������ģ�鲨����

	void SetBanknoteChangeComPort(const CString&);	// ����ֽ������ģ�鴮�ں�
	void SetBanknoteChangeBaudRate(const CString&); // ����ֽ������ģ�鲨����

	// ֽ������RFID
	int GetBankchangeRFIDComPort();					// ��ȡֽ������RFID���ں�
	int GetBankchangeRFIDBaudRate();				// ��ȡֽ������RFID������

	void SetBanknotechangeRFIDComPort(const CString&); // ����ֽ������RFID���ں�
	void SetBanknotechangeRFIDBaudRate(const CString&);// ����ֽ������RFID������

	// ֽ��ѭ��ģ������
	CString GetBNRRecycleBoxAName();					// ֽ��ѭ����Aʶ����Ϣ
	UINT    GetBNRRecycleBoxAId();						// ֽ��ѭ����A��ID
	CString GetBNRRecycleBoxAMoneyType();				// ֽ��ѭ����A�Ĵ洢��������
	UINT	GetBNRRecycleBoxAAmount();					// ֽ��ѭ����A�����
	UINT	GetBNRRecycleBoxAMaxCount();				// ֽ��ѭ����A���������

	CString GetBNRRecycleBoxBName();					// ֽ��ѭ����Bʶ����Ϣ
	UINT    GetBNRRecycleBoxBId();						// ֽ��ѭ����B��ID
	CString GetBNRRecycleBoxBMoneyType();				// ֽ��ѭ����B�Ĵ洢��������
	UINT	GetBNRRecycleBoxBAmount();					// ֽ��ѭ����B�����
	UINT	GetBNRRecycleBoxBMaxCount();				// ֽ��ѭ����B���������

	CString GetBNRRecycleBoxCName();					// ֽ��ѭ����Cʶ����Ϣ
	UINT    GetBNRRecycleBoxCId();						// ֽ��ѭ����C��ID
	CString GetBNRRecycleBoxCMoneyType();				// ֽ��ѭ����C�Ĵ洢��������
	UINT	GetBNRRecycleBoxCAmount();					// ֽ��ѭ����C�����
	UINT	GetBNRRecycleBoxCMaxCount();				// ֽ��ѭ����C���������

	CString GetBNRRecycleBoxDName();					// ֽ��ѭ����Dʶ����Ϣ
	UINT    GetBNRRecycleBoxDId();						// ֽ��ѭ����D��ID
	CString GetBNRRecycleBoxDMoneyType();				// ֽ��ѭ����D�Ĵ洢��������
	UINT	GetBNRRecycleBoxDAmount();					// ֽ��ѭ����D�����
	UINT	GetBNRRecycleBoxDMaxCount();				// ֽ��ѭ����D���������

	CString GetBNRChangeBoxName();						// ֽ��������ʶ����Ϣ
	UINT    GetBNRChangeBoxId();						// ֽ���������ID
	CString GetBNRChangeBoxMoneyType();					// ֽ��������Ĵ洢��������
	UINT	GetBNRChangeBoxAmount();					// ֽ������������
	UINT	GetBNRChangeBoxMaxCount();					// ֽ����������������

	UINT	GetBNRCollectBoxId();						// ֽ�һ������ID

	UINT	GetCoinChangeBox1Id();						// Ӳ��������A id
	UINT	GetCoinChangeBox2Id();						// Ӳ��������B id
	UINT	GetCoinCysleBoxId();						// Ӳ��ѭ����id
	UINT	GetCoinCollectBoxId();						// Ӳ�һ�����ID

	// WEB
	void SetWebIdleProcessType(CString);
	int	GetWebIdleProcessType();
	void SetWebIdleProcessURL(CString);
	CString GetWebIdleProcessURL();

	// Recharge Server
	CString GetRechargeServeYKT_IP();
	int GetRechargeServeYKT_Port();
	CString GetRechargeServeYPT_IP();
	int GetRechargeServeYPT_Port();
private:
	// ���ع��캯��
	CTvmSetting();
	~CTvmSetting();
	CTvmSetting(const CTvmSetting&);
	CTvmSetting& operator=(const CTvmSetting&);
	CString AddBackslash(CString sPath);                // ��Ŀ¼������ӷ�б��

	static CTvmSetting theInstance;
	CGetIni* m_settingIni;
	
};