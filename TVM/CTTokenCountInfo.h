#pragma once
#include "Xfile.h"
#include "afxtempl.h"
#include "afxmt.h"
#include "CParameterCommandExp.h"
#include "CommonDefine.h"
#include "delegate.h"
#include "dxfile.h"

#define theTOKEN_COUNT    CTTokenCountInfo::GetInstance()         ///< CTTokenCountInfo��Ψһ�ӿ�

/**
@brief Token������Ϣ��������Token����Ϣ��ȡ�����
*/

// 20171218,lichao: ��Ϊֱ��ID���ֶδ�BCD��ʽ������������ʾ�����������У�ID��������ת����
class CTTokenCountInfo
{
public:

	typedef struct _token_box_id
	{
		BYTE bLineCode;
		BYTE bType;
		WORD wNum;
		_token_box_id()
		{
			bLineCode = 0xFF;
			bType = 0xFF;
			wNum = 0xFFFF;
		}
		void Serialze(LPBYTE lpData) const
		{
			*lpData++ = bLineCode;
			*lpData++ = bType;
			memcpy(lpData, &wNum, sizeof(wNum));
			lpData+=2;
		}
		void Deserialize(LPBYTE lpData)
		{
			bLineCode = *lpData++;
			bType = *lpData++;
			wNum = ComMakeWORD(*(lpData+1),*lpData);
			lpData+=2;
		}
		CString ToString()
		{
			CString str;
			str.Format(_T("%02X%02X%04X"),bLineCode,bType,wNum);
			return str;
		}

		bool operator == (const _token_box_id& another) const
		{
			if(bLineCode != another.bLineCode) return false;
			if(bType != another.bType) return false;
			if(wNum != another.wNum) return false;
			return true;
		}
	} TOKEN_BOX_ID;

	// ������Ϣ
	typedef struct _tokenbox_info {
		LONG		ulOperatorID;		// ����Ա��ţ���ǰ�����ڲ�жƾ֤��ӡ��
		_DATE_TIME  operateTime;		// ����ʱ�䣨��ǰ�����ڲ�жƾ֤��ӡ��
		TOKEN_BOX_ID tokenBoxID;
		LONG       ulInitCount;           // Token���������
		LONG       ulCurCount;           // Token�䵱ǰ����
		LONG       ulWriteCnt;            // ���ݸ�д����     (�����ж�)
		_tokenbox_info(){
			ulOperatorID = 0;
			operateTime = _DATE_TIME();
			tokenBoxID		= TOKEN_BOX_ID();
			ulInitCount     = 0;
			ulCurCount      = 0;
			ulWriteCnt      = 0;
		}
		void Serialize(LPBYTE lpData) const
		{
			memcpy(lpData,&ulOperatorID,4);
			lpData+=4;
			memcpy(lpData,&operateTime,7);
			lpData+=7;
			tokenBoxID.Serialze(lpData);
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
			memcpy(&ulOperatorID,lpData,4);
			lpData+=4;
			memcpy(&operateTime,lpData,7);
			lpData+=7;
			tokenBoxID.Deserialize(lpData);
			lpData+=4;
			memcpy(&ulInitCount,lpData,4);
			lpData+=4;
			memcpy(&ulCurCount,lpData,4);
			lpData+=4;
			memcpy(&ulWriteCnt,lpData,4);
			lpData+=4;
		}
	} TOKEN_BOX_INFO;

	BOOL Internalize(CString dataPath, CString backupPath);            // ���ڻ�

	BOOL SetTokenboxAInfo(TOKEN_BOX_INFO& boxInfo);                 // ����Token��1�ļ�����Ϣ
	TOKEN_BOX_INFO& GetTokenboxAInfo();								// ��ȡToken��1�ļ�����Ϣ

	BOOL SetTokenboxBInfo(TOKEN_BOX_INFO& boxInfo);                 // ����Token��2�ļ�����Ϣ
	TOKEN_BOX_INFO& GetTokenboxBInfo();                             // ��ȡToken��2�ļ�����Ϣ

	BOOL SetHopperAInfo(TOKEN_BOX_INFO& hopperInfo);				// ����Hopper1�ļ�����Ϣ
	TOKEN_BOX_INFO& GetHopperAInfo();								// ��ȡHopper1�ļ�����Ϣ

	BOOL SetHopperBInfo(TOKEN_BOX_INFO& hopperInfo);				// ����Hopper2�ļ�����Ϣ
	TOKEN_BOX_INFO& GetHopperBInfo();								// ��ȡHopper2�ļ�����Ϣ

	BOOL SetTokenboxWastedInfo(TOKEN_BOX_INFO& boxInfo);			// ���÷�Token��ĵ�ǰ����
	TOKEN_BOX_INFO& GetTokenboxWastedInfo();						// �õ���Token��ĵ�ǰ����

	BOOL SetTokenboxCollectedInfo(TOKEN_BOX_INFO& boxInfo);			// ����Token������ĵ�ǰ����
	TOKEN_BOX_INFO& GetTokenboxCollectedInfo();						// �õ�Token������ĵ�ǰ����

	// Ʊ����������¼�
	delegate<void(TOKEN_BOX_INFO&)> TokenCounterChanged;

	static CTTokenCountInfo &GetInstance()
	{
		return theInstance;
	}

private:
	enum {
		TOKEN_BOX_COUNT					= 6,
		TOKEN_BOX_INFO_LEN				= 16+11,
		TOKEN_COUNT_INFO_FILE_LEN	= TOKEN_BOX_INFO_LEN*TOKEN_BOX_COUNT, 
	};

	void UpdateMasterFile();                                        // ���²������ļ�
	void UpdateMasterFileNoCopy();									// ���²������ļ�(�������ļ�)

	void UpdateHopperId();											// ����HopperID

	// ˽�г�Ա����
	TOKEN_BOX_INFO  m_TokenboxAInfo;									// Token��A�ļ�����Ϣ
	TOKEN_BOX_INFO  m_TokenboxBInfo;									// Token��B�ļ�����Ϣ
	TOKEN_BOX_INFO  m_HopperAInfo;										// Hopper��A�ļ�����Ϣ
	TOKEN_BOX_INFO  m_HopperBInfo;										// Hopper��B�ļ�����Ϣ
	TOKEN_BOX_INFO  m_TokenboxWastedInfo;								// ��Token��ļ�����Ϣ
	TOKEN_BOX_INFO  m_TokenboxCollectedInfo;							// Token������ļ�����Ϣ

	CDXFile   *m_file;														// �ļ���ȡ������
	char        m_fileData[TOKEN_COUNT_INFO_FILE_LEN];// ��ǰ�ļ�����
	CCriticalSection m_lock;											// �ؼ�������

	static  CTTokenCountInfo theInstance;

	// ���죬��������
	CTTokenCountInfo(void);
	~CTTokenCountInfo(void);
	CTTokenCountInfo(const CTTokenCountInfo&);
	CTTokenCountInfo &operator=(const CTTokenCountInfo&);
};
