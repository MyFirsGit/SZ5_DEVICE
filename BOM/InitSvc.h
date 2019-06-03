#pragma once
#include "stdafx.h"

/**
@brief      ���ڻ�Service
*/
class CInitSvc :
    public CBOMForeService
{
	DECLARE_MESSAGE_MAP()
public:
    enum INIT_RESULT {
        UNDOWN = 0,             // δ֪״̬
        SUCCESS,                // �ɹ�
        FAIL,                   // ʧ��
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
                result = UNDOWN;
				errorInfo = "";
				dealInfo = "";
            }
    } INITIAL_INFO;

	// ���ڻ���ʾ�к�
	enum ENUM_ROW {
		ROW_DATA_CHECK = 0,
		ROW_PARAMETER,
		ROW_RW,
		ROW_TW,		
		ROW_TH,
		ROW_PRINT,
	};

    static const int MAX_INITIAL_INFO_ROW = 6;          // ���ڻ���ʾ��Ϣ����
    INITIAL_INFO m_InitialInfo[MAX_INITIAL_INFO_ROW];   // ���ڻ���ʾ��Ϣ

    CInitSvc(void);                                     // ���캯��
    ~CInitSvc(void);                                    // ��������
    virtual void OnStart();                             // Service��������
	void SetInitRessult(int index, INIT_RESULT result); // ������ʾ���ڻ����
private:
    typedef enum _dialog_group {
        DIALOG_GROUP_BASE = 1,
    } DIALOG_GROUP;

    bool Initialize();                                  // ���ڻ�
    bool InitializeController();                        // ���ڻ�Controller
    bool CheckConsistent();                             // ����һ���Լ��
    bool InitializeParameter();                         // ���ڻ�ϵͳ����
    bool InitializeRW();                                // ���ڻ���д��(RW)
	bool InitializeTW();								// ���ڻ���д��(TW)
    bool InitializePrinter();                           // ���ڻ���ӡ��(Printer)
    bool InitializeTH();								// ���ڻ���Ʊ��
	bool InitializeLPT();								// ���ڻ���Ʊ���Ž�����
	bool InitializeUPS();								// ���ڻ�UPS
    bool InitializeCPS();								// ���ڻ���λ(CPS)
    //INIT_RESULT ACCAccredit();                          // ACC��Ȩ��һ��ͨ����֤
    //INIT_RESULT BMACAccredit();                         // ��ֵ�ն���Ȩ��һ��ͨ����֤

    bool CheckCreateHomeFolder();                       // ��鴴��������Ŀ¼
    bool CheckCreateBackupFolder();                     // ��鴴������Ŀ¼
    void DeleteLog(int days);                           // ɾ��ָ������ǰ��FTP�ϴ�����־
	void DeleteChargeReceiptFile(int days);				// ɾ��ָ������ǰ�ĳ�ֵ�����ļ�
	void DeleteTpuLog(int days);                        //ɾ��ָ������ǰ����־�ļ�
	void GetLimitTpuLogDate(int days,vector<CString>& logdate);    //��ȡָ��������Tpulog��־�ļ�����
	void GetLimitLogDate(int days,vector<int>& logdate);           //��ȡָ��������log��־�ļ�����
	//CString GetInitErrorInfo(CSysException);			// ��ȡ�쳣��ʾ��Ϣ
	//void AddUnsendAudit();								// ׷��δ�ͼ���
	void DoException(CSysException& e,int index);
	void CheckSAMidChange(SAM_POS_ samPos);
private:

	void OnExceptionNotify(CSysException&,bool);
};
