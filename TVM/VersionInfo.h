#pragma once
#include "delegate.h";

#define theVERSION_INI CVersionInfo::GetInstance()       // CVersionInfo��Ψһ�ӿ�

/**
@brief Versioninfo.INI����Ϣ����
*/

class CVersionInfo
{
public:
	// �汾����
	typedef enum _VER_TYPE{
		CURRENT = 0,       // ��ǰ�汾
		FUTURE  = 1        // �����汾
	}VER_TYPE;

	// ��������
	typedef enum _PROG_TYPE{
		UNKOWN			= -1,
		MC				= 1,
 		TH_MAIN			= 2,
 		TH_APP			= 4,
		CARD_RW_MAIN	= 8,
		CARD_RW_APP		= 16,
		TW_MAIN	= 32,
		TW_APP	= 64,
		CHARGE_RW_MAIN	= 128,
		CHARGE_RW_APP	= 256,
		TEMPLATE		= 512,   //ģ���ļ�
		GUI				= 1024,	 //GUI�ļ�
		VOICE			= 2048,  //�����ļ�
		STATION_MAP		= 4096,	 //��·վ�����
	}PROG_TYPE;

    static CVersionInfo&  GetInstance()
    {
        return theInstance;
    }
	
	static CVersionInfo* GetInstance(CString fileName);
	static void DeleteInstance(CVersionInfo* cVersionInfo);

    int Initialize(CString fileName);                                // ���ڻ�

	CString GetMCVersionNoString();						// ��ȡMC�İ汾��

	delegate<void(VER_TYPE,PROG_TYPE,DWORD)> VersionNoChanged;		//�汾�Ÿı��¼�
    DWORD GetVersionNo(VER_TYPE,PROG_TYPE);				// ��ȡ�汾��
	void SetVersionNo(VER_TYPE,PROG_TYPE,DWORD);        // ���ð汾��

	int GetBatchNo(VER_TYPE,PROG_TYPE);					// ��ȡ���κ�
	void SetBatchNo(VER_TYPE,PROG_TYPE,int);			// �������κ�

	int GetCompanyID(VER_TYPE,PROG_TYPE);				// ��ȡ����ID
	void SetCompanyID(VER_TYPE,PROG_TYPE,int);			// ���ó���ID

	_DATE GetStartDate(VER_TYPE,PROG_TYPE);				// ��ȡ��Ч����
	void SetStartDate(VER_TYPE,PROG_TYPE,_DATE);		// ������Ч����

	int GetVersionType(VER_TYPE,PROG_TYPE);				// ��ȡ�汾����
	void SetVersionType(VER_TYPE,PROG_TYPE,int);		// ���ð汾����

	bool SwitchVersion(__in PROG_TYPE progType,__in VER_TYPE fromVerType,__in VER_TYPE toVerType,__in bool removeFromVer = true);

	bool RemoveVersion(__in PROG_TYPE progType,__in VER_TYPE verType);

private:
    static CVersionInfo theInstance;
    CGetIni* m_versionIni;

	__inline CString GetSectionName(__in PROG_TYPE progTye);

    // ���ع��캯��
    CVersionInfo();
    ~CVersionInfo();
    CVersionInfo(const CVersionInfo&);
    CVersionInfo& operator=(const CVersionInfo&);

    CString AddBackslash(CString sPath);                // ��Ŀ¼������ӷ�б��
};