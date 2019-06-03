#pragma once
#include "GetIni.h"
#include "CommonExp.h"
#include "Util.h"
#include "delegate.h"
#include "CommonDefine.h"

#define theSETTING CSetting::GetInstance()

class COMMON_API CSetting
{
public:
	static CSetting&  GetInstance();
	int Initialize();                              // ���ڻ�
	delegate<void()> Initialized;

	// [APPLICATION]
	void SetDeviceTestMode(int);				// �����豸����ģʽ
	int GetDeviceTestMode();					// ��ȡ�豸����ģʽ
	void SetLanguageMode(LANGUAGETYPE_T);					// ��������ģʽ
	__inline LANGUAGETYPE_T GetLanguageMode();			// ��ȡ����ģʽ
	delegate<void(LANGUAGETYPE_T&,LANGUAGETYPE_T&)> LanguageModeChanged;
	void SetPrintStaffID(CString);				// ���ô�ӡԱ��ID
	void SetPrintLoginTime(_DATE_TIME);			// ���ô�ӡ��¼ʱ��
	CString GetPrintStaffID();					// ��ȡ��ӡԱ��ID
	_DATE_TIME GetPrintLoginTime();				// ��ȡ��ӡ��¼ʱ��

	// [FTP]
	CString GetFileUploadFTPDir();				// ��ȡ�ļ��ϴ�·��
	void SetFileUploadFTPDir(CString);			// �����ļ��ϴ�·��

	// [TH]
	delegate<void(int&)> THModeChanged;
	int GetTHMode();							// ��ȡ��Ʊ��ģʽ
	void SetTHMode(int);						// ���ó�Ʊ��ģʽ

	// [ICRW]/[ICTW]
	int GetRWUpdateFlag(bool bIsRW = true);		// ��ȡ��д��UpdateError
	void SetRWUpdateFlag(int,bool bIsRW = true);// ���ö�д��UpdateError

	// [RFID]
	int GetRFIDWriteStatus();					// ��ȡRFID��д״̬
	void SetRFIDWriteStatus(bool isUsed);		// д��RFID��д״̬
private:
	// ���ع��캯��
	CSetting();
	~CSetting();
	CSetting(const CSetting&);
	CSetting& operator=(const CSetting&);
	CString AddBackslash(CString sPath);                // ��Ŀ¼������ӷ�б��

	static CSetting theInstance;
	CGetIni* m_settingIni;
	LANGUAGETYPE_T m_LanguageMode;
};