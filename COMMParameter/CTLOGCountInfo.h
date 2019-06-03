#pragma once

#include "Xfile.h"
#include "afxtempl.h"
#include "afxmt.h"
#include "CParameterCommandExp.h"
#include "CommonDefine.h"
#include "delegate.h"
#include "dxfile.h"

#define theCOUNT_INFO    CCTLOGCountInfo::GetInstance()         ///< CCTLOGCountInfo��Ψһ�ӿ�

/**
@brief ������Ϣ����master   \n
       ����Ʊ����Ϣ�ͽ��׼����Ķ�ȡ�����
*/
class PARAMETERCOMMAND_DECLSPEC  CCTLOGCountInfo
{
public:

	typedef struct _ticketbox_id
	{
		BYTE bDeviceType;	//�豸���� BDC
		BYTE bType;			//Ʊ������ HEX
		BYTE bStationID;	//��վID   BCD
		BYTE bNum;			//Ʊ����� HEX
		_ticketbox_id()
		{
			bDeviceType = 0xff;
			bType = 0xFF;
			bStationID = 0xff;
			bNum = 0xff;
		}
		void Serialze(LPBYTE lpData,bool bScData = false) const
		{
			*lpData = BYTE2BCD(bDeviceType);
			lpData++;
			*lpData = bType;
			lpData++;
			*lpData = BYTE2BCD(bStationID);
			lpData++;
			*lpData = bNum;
			lpData++;
		}
		void Deserialize(LPBYTE lpData)
		{
			bDeviceType = BCD2int((char*)lpData,1);
			lpData++;
			bType = *lpData;
			lpData++;
			bStationID = BCD2int((char*)lpData,1);
			lpData++;
			bNum = *lpData;
			lpData++;
		}
		CString ToString()
		{
			// ����ֱ�ӷ���
			CString str(_T("FFFFFFFF"));
			if(bDeviceType == 0xFF){
				return str;
			}
			// ��ȡ����ID��Ҫ��ʽ��Ϊ��׼ 1 BDC + 1HEX+1BCD + 1HEX
			str.Format(_T("%02X%02d%02X%02d"),(BYTE)char2BCD(bDeviceType),bType,(BYTE)char2BCD(bStationID),bNum);
			return str;
		}

		bool operator == (const _ticketbox_id& another) const
		{
			if(bDeviceType != another.bDeviceType) return false;
			if(bType != another.bType) return false;
			if(bStationID != another.bStationID) return false;
			if(bNum != another.bNum) return false;
			return true;
		}
	} TICKETBOX_ID;

    // ������Ϣ
    typedef struct _ticketbox_info {
		TICKETBOX_ID ticketBoxID;
		LONG       ulInitCount;           // Ʊ���������
        LONG       ulCurCount;            // Ʊ�䵱ǰ����
        LONG       ulWriteCnt;            // ���ݸ�д����     (�����ж�)
        _ticketbox_info(){
            ulInitCount     = 0;
            ulCurCount      = 0;
            ulWriteCnt      = 0;
        }
		void Serialize(LPBYTE lpData) const
		{
			ticketBoxID.Serialze(lpData);
			lpData+=4;
			memcpy(lpData,&ulInitCount,4);
			lpData+=4;
			memcpy(lpData,&ulCurCount,4);
			lpData+=4;
			memcpy(lpData,&ulWriteCnt,4);
			lpData+=4;
		}
		void Deserialize(LPBYTE lpData)
		{
			ticketBoxID.Deserialize(lpData);
			lpData+=4;
			memcpy(&ulInitCount,lpData,4);
			lpData+=4;
			memcpy(&ulCurCount,lpData,4);
			lpData+=4;
			memcpy(&ulWriteCnt,lpData,4);
			lpData+=4;
		}
    } TICKETBOX_INFO;

    // ������Ϣ
    //typedef enum _log_type
    //{
    //    LOG_DATA_UPLOAD           = 1,                 // ��׼��־
    //    SEND_FAIL_MESSAGE_LOG     = 2,                 // ʧ����־
    //} LOG_TYPE;

	BOOL Internalize(CString dataPath, CString backupPath);         // ���ڻ�

	TICKETBOX_INFO & GetTicketboxAInfo();                      // ��ȡƱ��1�ļ�����Ϣ
    TICKETBOX_INFO & GetTicketboxBInfo();                      // ��ȡƱ��2�ļ�����Ϣ
	TICKETBOX_INFO & GetTicketboxCInfo();                      // ��ȡ��Ʊ��ļ�����Ϣ

    WORD GetCollectionCurCount();									// �õ�������ĵ�ǰ����
    void SetCollectionCurCount(WORD collectionTicketCount);         // ���û�����ĵ�ǰ����

    BOOL SetTicketboxAInfo(TICKETBOX_INFO& boxInfo);                 // ����Ʊ��1�ļ�����Ϣ
    BOOL SetTicketboxBInfo(TICKETBOX_INFO& boxInfo);                 // ����Ʊ��2�ļ�����Ϣ
	BOOL SetTicketboxCInfo(TICKETBOX_INFO& boxInfo);                 // ���÷�Ʊ��ļ�����Ϣ


 //   void GetAuditEndTime(_DATE_TIME&);                              // �õ���ƽ���ʱ��
 //   BOOL SetAuditEndTime(_DATE_TIME);                               // ������ƽ���ʱ��
	
	//�����������¼�
	delegate<void(COUNTER_TYPE)> CounterReseted;

    ULONG GetUDSN();											    // �õ�ACC���׼���
	bool  UpdateUDSN(BYTE);											// ����ACC���׼���
    ULONG GetUDSNForDisplay();                                      // �õ�ACC���׼��������ۼ�
	ULONG GetLastUsedUDSN();										// �õ����һ��ACC���׼���
    void SetUDSNForMain(ULONG);                                     // ����ACC���׼��� 

	ULONG GetTransactionNum();                                      // �õ�AFC���׼���       
    ULONG GetTransactionNumForDsp();                                // �õ�AFC���׼��������ۼ�   
	ULONG GetLastUsedTransactionNum();								// �õ����һ��AFC���׼���
     void SetTransactionNumForMain(ULONG);                          // д��AFC���׼���
	
	ULONG GetEventNum();                                            // �õ�event����
    ULONG GetEventNumForDsp();                                      // �õ�event���������ۼ�
	ULONG GetLastUsedEventNum();									// �õ����һ���¼�ҵ��ļ���
	void SetEventNumForMain(ULONG);                                 // д��event���� 
    
	ULONG GetMaintenceNum();                                        // �õ�Maintence����
    
	ULONG GetECTNum();                                              // �õ�ECT���׼���
    ULONG GetECTNumForDsp();                                              // �õ�ECT���׼��������ۼ�
	void  UpdateECTNum(BYTE);                                       // ����ECT���׼���
	ULONG GetLastUsedECTNum();											// �õ�����ҵ���ECT���׼���
    void SetECTNumForMain(ULONG);                                   // д��ECT����
  
    DWORD GetLoginCount();											// �õ�����Ա����
    bool  SetLoginCount(DWORD logDate);								// ���ò���Ա����
	DWORD AccumulateLoginCount();									// �ۼӲ���Ա����

	// TH�����������ӷ���
	//void CopystrBoxBackupFile();										// �����ļ����Ƶ������ļ�
	//ULONG GetUDSNNoCopy();                                          // �õ��ն��豸���׼���(�������ļ�)
	//ULONG GetTransactionNumNoCopy();                                // �õ�transaction����(�������ļ�)

	static CCTLOGCountInfo &GetInstance()
    {
        return theInstance;
    }

private:
    enum {
	    COUNT_INFO_LEN	= 67+16,                                    // �ļ�����
		PR_ERROR_MSG_MAX_LEN = 261,                                 // �쳣���������
    };
    // ˽�г�Ա����
    bool SetUDSN(ULONG udsn);                                       // �����ն��豸���׼���
	bool SetUDSNNoCopy(ULONG udsn);                                 // �����ն��豸���׼���(�������ļ�)
    bool SetTransactionNum(ULONG);                                  // д��transaction����
	bool SetTransactionNumNoCopy(ULONG);                            // д��transaction����(�������ļ�)
    bool SetEventNum(ULONG);                                        // д��event���� 
    bool SetMaintenceNum(ULONG);                                    // д��Maintence���� 
    bool SetYKTNum(ULONG);                                          // д��һ��ͨ����

    void UpdateMasterFile();                                        // ���²������ļ�
	void UpdateMasterFileNoCopy();                                  // ���²������ļ�(�������ļ�)

    // ˽�г�Ա����
    WORD        m_collectionTicketCount;                            // �����䵱ǰ���� 
    TICKETBOX_INFO  m_TicketboxAInfo;                                  // Ʊ��1�ļ�����Ϣ
    TICKETBOX_INFO  m_TicketboxBInfo;                                  // Ʊ��2�ļ�����Ϣ
	TICKETBOX_INFO  m_TicketboxCInfo;                                  // Ʊ��2�ļ�����Ϣ

    _DATE_TIME  m_auditEndTime;                                     // ��ƽ���ʱ��
    ULONG       m_ulUDSN;                                           // �ն��豸���׼���
    ULONG       m_transactionNum;                                   // transaction����
    ULONG       m_currentTraNum;                                    // �洢д��ǰ��transactionֵ
    ULONG       m_eventNum;                                         // event����
    ULONG       m_currentEveNum;                                    // �洢д��ǰ��event��ֵ
    ULONG       m_maintenceNum;                                     // maintence����
    ULONG       m_currentMainNum;                                   // �洢д��ǰ��maintence��ֵ
    ULONG       m_ECTNum;                                           // һ��ͨ����
    ULONG       m_currentECTNum;                                    // �洢д��ǰ��һ��ͨ��ֵ
    DWORD       m_loginCount;                                       // ����Ա����
    DWORD       m_failLogDate;                                      // 
    CDXFile      *m_file;                                            // �ļ���ȡ������
    char        m_fileData[COUNT_INFO_LEN];                         // ��ǰ�ļ�����
    CCriticalSection m_lock;                                        // �ؼ�������

    static  CCTLOGCountInfo theInstance;
    
    // ���죬��������
    CCTLOGCountInfo(void);
    ~CCTLOGCountInfo(void);
    CCTLOGCountInfo(const CCTLOGCountInfo&);
    CCTLOGCountInfo &operator=(const CCTLOGCountInfo&);
};
