#pragma once
#include "Xfile.h"
#include "afxtempl.h"
#include "afxmt.h"
#include "CParameterCommandExp.h"
#include "CommonDefine.h"
#include "delegate.h"
#include "dxfile.h"

#define theCARD_COUNT    CTCardCountInfo::GetInstance()         ///< CTCardCountInfo��Ψһ�ӿ�

/**
@brief Ʊ��������Ϣ��������Ʊ����Ϣ��ȡ�����
*/

class CTCardCountInfo
{
public:

	typedef struct _card_box_id
	{
		BYTE bDeviceType;	//�豸���� BDC
		BYTE bType;			//Ʊ������ HEX
		BYTE bStationID;	//��վID   BCD
		BYTE bNum;			//Ʊ����� HEX
		_card_box_id()
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
			*lpData = char2BCD(bStationID);
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
			if(bDeviceType == 0xFF || bType == 0XFF || bStationID == 0XFF || bNum == 0XFF){
				return str;
			}
			// ��ȡ����ID��Ҫ��ʽ��Ϊ��׼ 1 BDC + 1HEX+1BCD + 1HEX
			str.Format(_T("%02X%02d%02X%02d"),(BYTE)char2BCD(bDeviceType),bType,(BYTE)char2BCD(bStationID),bNum);
			return str;
		}

		bool operator == (const _card_box_id& another) const
		{
			if(bDeviceType != another.bDeviceType) return false;
			if(bType != another.bType) return false;
			if(bStationID != another.bStationID) return false;
			if(bNum != another.bNum) return false;
			return true;
		}
	} CARD_BOX_ID;

    // ������Ϣ
    typedef struct _ticketbox_info {
		CARD_BOX_ID ticketBoxID;
		LONG       ulInitCount;           // Ʊ���������
        LONG       ulCurCount;           // Ʊ�䵱ǰ����
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
    } CARD_BOX_INFO;

	BOOL Internalize(CString dataPath, CString backupPath);            // ���ڻ�

	BOOL SetCardboxAInfo(CARD_BOX_INFO& boxInfo);                 // ����Ʊ��A�ļ�����Ϣ
	CARD_BOX_INFO& GetCardboxAInfo();									    // ��ȡƱ��A�ļ�����Ϣ

	BOOL SetCardboxBInfo(CARD_BOX_INFO& boxInfo);                 // ����Ʊ��B�ļ�����Ϣ
    CARD_BOX_INFO& GetCardboxBInfo();                                      // ��ȡƱ��B�ļ�����Ϣ

	BOOL SetCardboxWastedInfo(CARD_BOX_INFO& boxInfo);			// ���÷�Ʊ��ĵ�ǰ����
    CARD_BOX_INFO& GetCardboxWastedInfo();								// �õ���Ʊ��ĵ�ǰ����
   
	// Ʊ����������¼�
	delegate<void(CARD_BOX_INFO&)> CardCounterChanged;


	 static CTCardCountInfo &GetInstance()
    {
        return theInstance;
    }

private:
    enum {
		CARD_BOX_COUNT					= 3,
		CARD_BOX_INFO_LEN				= 16,
	    CARD_COUNT_INFO_FILE_LEN	= CARD_BOX_INFO_LEN*CARD_BOX_COUNT, 
    };
 
	void UpdateMasterFile();                                        // ���²������ļ�
	void UpdateMasterFileNoCopy();                            // ���²������ļ�(�������ļ�)

    // ˽�г�Ա����
	CARD_BOX_INFO  m_CardboxAInfo;                                  // Ʊ��1�ļ�����Ϣ
	CARD_BOX_INFO  m_CardboxBInfo;                                  // Ʊ��2�ļ�����Ϣ
    CARD_BOX_INFO  m_CardboxWastedInfo;                         // ��Ʊ��ļ�����Ϣ

    CDXFile   *m_file;														// �ļ���ȡ������
    char        m_fileData[CARD_COUNT_INFO_FILE_LEN];// ��ǰ�ļ�����
    CCriticalSection m_lock;											// �ؼ�������

    static  CTCardCountInfo theInstance;
    
    // ���죬��������
    CTCardCountInfo(void);
    ~CTCardCountInfo(void);
    CTCardCountInfo(const CTCardCountInfo&);
    CTCardCountInfo &operator=(const CTCardCountInfo&);
};
