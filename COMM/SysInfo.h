#pragma once
#include "GetIni.h"
#include "CommonExp.h"
#include "Util.h"
#include "delegate.h"
#include "CommonDefine.h"

/**
@brief	�豸INI�����ļ�������
*/

class  COMMON_API CSYSInfo
{
public:

	int Initialize();                              // ���ڻ�
	
	//delegate<void()> Initialized;

    // [APPLICATION]-TVMϵͳ��Ϣ
    CString GetInitialVersion();						// ��ȡ����汾��Ϣ
    bool      IsTopmost();									// �Ƿ���ʾ����ǰ��
	bool     CanShowCursor();							// �Ƿ���ʾ�����
	bool	    IsFunctionKeyEnabled();				// �Ƿ����ù��ܼ�
	bool	    IsTaskManagerEnabled();				// �Ƿ��������������
	bool     IsDesktopEnabled();						// �Ƿ���ʾ����
	bool		IsUsedGuardian();							// �Ƿ����ػ�����
	int        GetStatusTimerInterval();				// ��ȡ״̬��ˢ��ʱ����
	bool     IsAutoReturnDefaultService();				// ������ʱ���˿������Ƿ��Զ�����Ĭ��ҵ��0�����Զ���1���Զ���
	bool     IsEnableClickSound();						//�Ƿ��ð��I��

	// [DISKFREESPACE]-���̿ռ����ã���λ��M��
	int GetDataDirectoryWarn();					// ������Ŀ¼����
	int GetDataDirectoryStop();					// ������Ŀ¼����
	int GetBackupDirectoryWarn();				// ��������Ŀ¼����
	int GetBackupDirectoryStop();		    	// ��������Ŀ¼����
	int GetLogDirectoryWarn();					// ��־�ļ�Ŀ¼����
	int GetLogDirectoryStop();						// ��־�ļ�Ŀ¼����
	int GetSysDirectoryWarn();					// ��־�ļ�Ŀ¼����
	int GetSysDirectoryStop();						// ��־�ļ�Ŀ¼����

	// [LOG]-��־�������
    bool IsWriteDebug();                         // �Ƿ��¼Debug��Ϣ
	int	GetStoreDay();							// ��ȡ��־��������
	void SetStoreDay(int nDays);				// ������־��������

	// 	[MACHINE]-������Ϣ
	CString GetUsedAdapterName();		// ��ȡϵͳ����ʹ�õ���������

	// [APP_PATH]-Ӧ��·������
	CString GetDataPath();								// ��ȡ������Ŀ¼
	CString GetBackupPath();							// ��ȡ����Ŀ¼
	CString GetLogPath();								// ��ȡ��־���·��
	CString GetUSBDrive();								// ��ȡUSB�̷�
	CString GetCFDrive();								// ��ȡCF���̷�
	CString GetExportPath();							// ��ȡUSB���ݵ�����Ŀ¼
	CString GetFileDownloadLocalDir();                  // ��ȡFTP��ʱ����·��
	CString GetGetReceiptLogPath();                     // ��ȡƾ֤����Ŀ¼

	int GetTransactionPackageInterval();			// ��ȡ�������ݴ��ʱ����
	int GetBusinessPackageInterval();				// ��ȡҵ�����ݴ��ʱ����

    CGetIni* m_DeviceIni;

	 // ������������
	 CSYSInfo();
	 ~CSYSInfo();
	 CSYSInfo(const CSYSInfo&);
	 CSYSInfo& operator=(const CSYSInfo&);
	 CString AddBackslash(CString sPath);                // ��Ŀ¼������ӷ�б��

protected:
  
	CString GetRWTypeString(RW_TYPE rwType);
    CString GetRfidTypeString(RFID_NO rfidId);

	CString m_strDataPath;						// ��������Ŀ¼
	CString m_strDataBackpath;					// �������ݱ���Ŀ¼
	CString m_strLogPath;						// ������־Ŀ¼
	CString m_strPrintPath;						// ��ӡ���Ŀ¼
};