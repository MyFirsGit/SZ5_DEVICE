#pragma once

#include "CParameterCommandExp.h"
#include "dxfile.h"


#define theSOFT_VERSION    CCTLOGSoftVer::GetInstance()             ///< CCTLOGSoftVer��Ψһ�ӿ�

/**
@brief �汾��Ϣ����master   \n
       �����Ը����ְ汾��Ϣ�Ķ�ȡ�����
*/
class PARAMETERCOMMAND_DECLSPEC  CCTLOGSoftVer
{
public:	

	typedef struct _bomsoft_info{
		WORD wSerialNo;   // ���ΰ汾�� 2 BIN
		BYTE bVerType;	  // �汾����   1 BIN	
		DWORD dwVersion;  // �汾��     4 BIN	��Чֵ��Χ0x00000000~0xFFFFFFFE����ЧʱΪ0xFFFFFFFF������ʱת����INTEL��
		_bomsoft_info(){
			wSerialNo = 0;
			bVerType  = 0;
			dwVersion = 0;
		}
	}BOMSOFT_INFO;

	//�汾��Ϣ
	typedef struct _version_info {
		BYTE ulRWSAMID1[6];               // ��д��SAM ID 1 һ��ͨPSAM����
		BYTE ulRWSAMID2[6];               // ��д��SAM ID 2 һ��ͨISAM����
		BYTE ulRWSAMID3[6];               // ��д��SAM ID 3 ����PSAM����
		BYTE ulRWSAMID4[6];               // ��д��SAM ID 4 ����ISAM����
		BYTE ulOutRWSAMID1[6];            // �ⲿ��д��SAM ID 1 һ��ͨPSAM����
		BYTE ulOutRWSAMID2[6];            // �ⲿ��д��SAM ID 2 һ��ͨISAM����
		BYTE ulOutRWSAMID3[6];            // �ⲿ��д��SAM ID 3 ����PSAM����
		BYTE ulOutRWSAMID4[6];            // �ⲿ��д��SAM ID 4 ����ISAM����
		char ulAppliVersion[4];           // ���Ƴ���汾(BOM APPLI) 
		char ulTHRWVersion[4];            // TH RW�汾
		char ulICRWVersion[4];            // IC RW�汾
		char rwBootVer[4];                // �ⲿ��д��boot�汾
		char thRwBootVer[4];              // �ڲ���д��boot�汾
		//char thBootVersion[2];            // TH boot ���İ汾
		SC_VER_INFO thInfo;               // TH�汾��Ϣ
		SC_VER_INFO thBootInfo;           // TH Boot �汾
		DWORD dwModeHistory;              // ģʽ����
		BOMSOFT_INFO BomSoft_info;        // BOM�汾��Ϣ
		_version_info()
		{
			memset(ulRWSAMID1,0x00,6);
			memset(ulRWSAMID2,0x00,6);
			memset(ulRWSAMID3,0x00,6);
			memset(ulRWSAMID4,0x00,6);
			memset(ulOutRWSAMID1,0x00,6);
			memset(ulOutRWSAMID2,0x00,6);
			memset(ulOutRWSAMID3,0x00,6);
			memset(ulOutRWSAMID4,0x00,6);
			memset(ulAppliVersion,0x00,4);         
			memset(ulTHRWVersion,0x00,4);
			memset(ulICRWVersion,0x00,4);
			memset(rwBootVer,0x00,4);
			memset(thRwBootVer,0x00,4);
			dwModeHistory = 0;
		}
	} VERSION_INFO;


	BOOL Internalize(CString dataPath, CString backupPath);     // ���ڻ�

	void GetSoftVerData(VERSION_INFO &svData);                  // ��ȡ�汾��Ϣ

	void SetRWSAMID(SLOT_ID, const BYTE ulID[]);                // ���ö�д��SAM���
	void SetOutRWSAMID(SLOT_ID, const BYTE ulID[]);             // �����ⲿ��д��SAM���
	BOOL SetAPIVersion(const char[]);                           // ���ÿ��Ʋ��汾
	BOOL SetTHRWVersion(const char[]);                          // ����TH RW�汾
	BOOL SetICRWVersion(const char[]);                          // ����IC RW�汾
	void SetTHVersionInfo(SC_VER_INFO);                         // ����TH�汾��Ϣ
	void SetTHBootVersionInfo(SC_VER_INFO);                     // ����TH Boot�汾��Ϣ
	void SetRWBootVer(const char[]);                            // ����RW Boot�汾��Ϣ
	void SetTHRWBootVer(const char[]);                          // ����THRW Boot�汾��Ϣ
	void SetModeHistoryVer(DWORD ver);                          // ����ģʽ�����汾��Ϣ
	void SetBOMSoftInfo(WORD wSerialNo, BYTE verType, DWORD dwVersion);  // ����BOM����汾��Ϣ
	static CCTLOGSoftVer&  GetInstance()
	{
		return theInstance;
	}

private:
	enum {
		SOFT_VER_LEN	= 111,									  ///< �ļ�����
	};

	// �ֶ�����λ��
	enum {
		RW_SAMID_POS        = 0,                                  // ��д��          
		OUTRW_SAMID_POS     = 24,                                 // �ⲿ��д��
		API_VERSION_POS     = 48,                                 // ���Ƴ���汾
		RW_BOOT_VERSION     = 52,                                 // RW Boot�汾��Ϣ
		//TH_BOOT_VERSION     = 52,                                 // ��Ʊ����BOOT���İ汾
		RW_HARD_POS         = 56,                                 // RW�汾(IC��д��)      
		RW_SOFT_POS         = 60,                                 // RW�汾(TH��д��)
		TH_TYPE_POS         = 64,                                 // TH��ʽ
		TH_VERSION_POS      = 74,                                 // TH�汾��
		TH_SUM_POS          = 76,                                 // TH��SUMֵ
		TH_BOOT_TYPE_POS    = 80,                                 // TH��ʽ(boot)
		TH_BOOT_VERSION_POS = 90,                                 // TH�汾��(boot)
		TH_BOOT_SUM_POS     = 92,                                 // TH��SUMֵ(boot)
		TH__RW_BOOT_VERSION = 96,                                 // THRW Boot�汾��Ϣ
		MODE_HISTORY        = 100,                                // ģʽ�����汾
		BOM_SOFT_INFO       = 104                                 // BOM����汾��Ϣ
	};    

	VERSION_INFO m_svSoftVersion;                                   // �汾��Ϣ
	CDXFile       *m_file;                                           // �ļ���ȡ������
	char m_fileData[SOFT_VER_LEN];                                  // ��ǰ�ļ�����

	static  CCTLOGSoftVer theInstance;
	CCTLOGSoftVer(void);
	~CCTLOGSoftVer();
	CCTLOGSoftVer(const CCTLOGSoftVer&);
	CCTLOGSoftVer& operator=(const CCTLOGSoftVer&);
};
