#pragma once
#include "stdafx.h"

/**
@brief      ���ڻ�Service
*/
class CInitSvc :  public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
    enum INIT_RESULT {
        UNKNOWN = 0,			// δ֪״̬
        SUCCESS,				// �ɹ�
        FAIL,					// ʧ��
    };

    // ���ڻ���ʾ��Ϣ�ṹ
    typedef struct _INITIAL_INFO {
        CString title;          // ����
        INIT_RESULT result;     // ���
		CString errorInfo;		// ��ϸ������Ϣ
		CString dealInfo;		// ��������Ϣ
            _INITIAL_INFO()
            {
                title = "";
                result = UNKNOWN;
				errorInfo = "";
				dealInfo = "";
            }
    } INITIAL_INFO;

	// ���ڻ���ʾ�к�
	enum ENUM_ROW {
		ROW_DISK_CHECK = 0,
		ROW_DATA_INIT,
		ROW_IO_MODULE,
		ROW_CHARGE_RW,
		//ROW_TOKEN_RW,
		ROW_CARD_RW,
		//ROW_TOKEN_MODULE,
		ROW_CARD_MODULE,
		ROW_COIN_MODULE,
		ROW_BANKNOTE_MODULE,
		//ROW_BANKNOTE_CHANGE_MODULE,
		ROW_PRINT,
		//ROW_BANKCARD_MODULE,
		//ROW_PINPAD_MODULE,
		ROW_DISPLAY,
		ROW_MAX,
		ROW_INVALID,
				
	};

    INITIAL_INFO m_InitialInfo[ROW_MAX];					// ���ڻ���ʾ��Ϣ
	int m_order[ROW_MAX];											// �Ƿ��ʼ�����Գ�ʼ��������������

    CInitSvc(void);															// ���캯��
    ~CInitSvc(void);															// ��������
    virtual void OnStart();												// Service��������
	void SetInitRessult(int index, INIT_RESULT result);		// ������ʾ���ڻ����

private:	
    typedef enum _dialog_group {
        DIALOG_GROUP_BASE = 1,
    } DIALOG_GROUP;

    bool Initialize();                                       // ���ڻ�����
    bool InitializeController();                        // ���ڻ�Controller
	void StartBackService();                           // ������̨����
	
	bool CheckConsistent();                           // ����һ���Լ��
	bool CheckDiskFreeSpace();							// �����̿ռ�
	bool InitializeSystemConfig();				   // -��ʼ��ϵͳ����
	bool InitializeOSSetting();						   // -��ʼ������ϵͳ����
	bool InitializeSystemFolder();				       // -��ʼ��ϵͳĿ¼
    
    bool InitializeParameter();                        // ���ڻ�ϵͳ����
	bool InitializeSystemLog();						   // -��ʼ��ϵͳ��־
	bool InitializeSystemException();			   // -��ʼ��ϵͳ�쳣
	bool InitializeSystemMaster();				   // -��ʼ��ϵͳMaster
	bool InitializeTxnData();							   // -��ʼ����������
	bool InitializeParamData();					   // -��ʼ����������
	bool InitializeDatabase();						   // -��ʼ�����ݿ�
	bool InitializeRegistAudit();					   // -��ʼ���Ĵ�������
	bool InitializeSCAudit();							   // -��ʼ��SC�������
	bool InitializeSystemTime();						// -��ʼ��ϵͳʱ��

	// ��ʼ��Ӳ��ģ��
	bool InitializeStatusDisplay();						//��ʼ������
	bool InitializeChargeRW();							// ��ʼ����ֵ��д��
	bool InitializeCardRW();							// ��ʼ��Card��д��
	bool InitializeTokenModule();						// ��ʼ��tokenģ��
	bool InitializeCardModule();						// ��ʼ��Cardģ��
	bool InitializeCoinModule();						// ��ʼ��Ӳ��ģ��
	bool InitializeBanknoteModule();					// ��ʼ��ֽ�ҽ���ģ��
	bool InitializeBanknoteChangeModule();				// ��ʼ��ֽ������ģ��
	bool InitializePrinter();							// ���ڻ���ӡ��(Printer)
	bool InitializeIO();								// ��ʼ��IOģ��
	//bool InitializeUPS();								// ���ڻ�UPS
	void StartDeviceMonitor();							// �����豸����߳�
	
    bool InitializeCPS();									// ���ڻ���λ(CPS)
    
	// ������֤
	//INIT_RESULT ACCAccredit();                          // ACC��Ȩ��һ��ͨ����֤
    //INIT_RESULT BMACAccredit();                         // ��ֵ�ն���Ȩ��һ��ͨ����֤
private:	
    bool CheckCreateHomeFolder();													// ��鴴��������Ŀ¼
    bool CheckCreateBackupFolder();													// ��鴴������Ŀ¼
    void DeleteLog(int days);																// ɾ��ָ������ǰ��FTP�ϴ�����־
	void DeletePicture();																		// ɾ����ʱͼƬ
	void DeleteTpuLog(int days);															//ɾ��ָ������ǰ����־�ļ�
	void GetLimitTpuLogDate(int days,vector<CString>& logdate);     //��ȡָ��������Tpulog��־�ļ�����
	void GetLimitLogDate(int days,vector<int>& logdate);					//��ȡָ��������log��־�ļ�����
	//CString GetInitErrorInfo(CSysException);										// ��ȡ�쳣��ʾ��Ϣ
	void CheckSAMidChange(SAM_POSITION samPos);

private:	
	void DoException(CSysException& e,int index);
	void OnExceptionNotify(CSysException&,bool);

	void DeletePrintHistory(int days);								// ɾ����ӡ��ʷ��¼
	void GetLimitPrintLogDate(int days,CString strPath,vector<CString>& vecNeedDel,bool bIsDir = false);// ȡ��ָ��Ŀ¼��ָ������Ӧ��ɾ������֮����ļ�
};
