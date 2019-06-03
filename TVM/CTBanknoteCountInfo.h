#pragma once
#include "Xfile.h"
#include "afxtempl.h"
#include "afxmt.h"
#include "CParameterCommandExp.h"
#include "CommonDefine.h"
#include "delegate.h"
#include "dxfile.h"

#define theBANKNOTE_COUNT    CTBanknoteCountInfo::GetInstance()         ///< CTBanknoteCountInfo��Ψһ�ӿ�

/**
@brief ֽ��Ǯ�������Ϣ����������Ϣ��ȡ�����
*/

class CTBanknoteCountInfo
{
public:
	// 20171218,lichao: ��Ϊֱ��ID���ֶδ�BCD��ʽ������������ʾ�����������У�Ʊ��ID��������ת����
	typedef struct _banknote_box_id
	{
		BYTE bLineCode;
		BYTE bType;
		BYTE bNumInQu;
		BYTE bNum;
		_banknote_box_id()
		{
			bLineCode = 0xFF;
			bType = 0xFF;
			bNumInQu = 0xFF;
			bNum = 0xFF;
		}
		void Serialze(LPBYTE lpData) const
		{
			*lpData++ = bLineCode;
			*lpData++ = bType;
			*lpData++ = bNumInQu;
			*lpData++ = bNum;
		}
		void Deserialize(LPBYTE lpData)
		{
			bLineCode = *lpData++;
			bType = *lpData++;
			bNumInQu = *lpData++;
			bNum = *lpData++;
		}
		CString ToString()
		{
			CString str;
			str.Format(_T("%02X%02X%02X%02X"),bLineCode,bType,bNumInQu,bNum);
			return str;
		}

		bool operator == (const _banknote_box_id& another) const
		{
			if(bLineCode != another.bLineCode) return false;
			if(bType != another.bType) return false;
			if(bNum != another.bNum) return false;
			if (bNumInQu != another.bNumInQu) return false;
			return true;
		}
	} BANKNOTE_BOX_ID;

	// ������Ϣ(ֽ��ѭ��/����ģ��)
	typedef struct _banknote_recycler_box_info {
		LONG		ulOperatorID;		// ����Ա��ţ���ǰ������ֽ�����жƾ֤��ӡ��
		_DATE_TIME  operateTime;		// ����ʱ�䣨��ǰ������ֽ�����жƾ֤��ӡ��
		BANKNOTE_BOX_ID banknoteBoxID;
		LONG		ulCurCount;			// ����
		LONG		ulCurValue;			// ���
		LONG		ulLastLeftAmount;	// �ϴ�����
		_banknote_recycler_box_info(){
			ulOperatorID = 0;
			operateTime = _DATE_TIME();
			ulCurCount = 0;
			ulCurValue = 0;
			ulLastLeftAmount = 0;
		}
		void Serialize(LPBYTE lpData) const
		{
			memcpy(lpData,&ulOperatorID,4);
			lpData+=4;
			memcpy(lpData,&operateTime,7);
			lpData+=7;
			// �ı�֮ǰֽ�ҽ���˳��ȥ��֮ǰ��500��1000���ģ�֮ǰ����1Ԫ��5Ԫ��
			banknoteBoxID.Serialze(lpData);
			lpData+=4;
			memcpy(lpData,&ulCurCount,4);
			lpData+=4;
			memcpy(lpData,&ulCurValue,4);
			lpData+=4;
			memcpy(lpData,&ulLastLeftAmount,4);
			lpData+=4;
		}
		void Deserialize(LPBYTE lpData)
		{
			memcpy(&ulOperatorID,lpData,4);
			lpData+=4;
			memcpy(&operateTime,lpData,7);
			lpData+=7;
			banknoteBoxID.Deserialize(lpData);
			lpData+=4;
			memcpy(&ulCurCount,lpData,4);
			lpData+=4;
			memcpy(&ulCurValue,lpData,4);
			lpData+=4;
			memcpy(&ulLastLeftAmount,lpData,4);
			lpData+=4;
		}
		void CheckMinusValue()
		{
			if(ulCurCount<0){
				ulCurCount = 0;
			}
			if(ulLastLeftAmount<0){
				ulLastLeftAmount = 0;
			}
		}

		DWORD BankNotesCountAvos(){
			return ulCurCount * ulCurValue;//(ulCount_1*100 + ulCount_5*500 + ulCount_10 * 1000 + ulCount_100 * 10000 + ulCount_20 * 2000 + ulCount_50 * 5000);
		}
		DWORD BanknotesTotalCount(){
			return ulCurCount;//(ulCount_1 + ulCount_5 + ulCount_10 + ulCount_100 + ulCount_20  + ulCount_50);
		}

		void BanknoteInit(){
			ulCurCount = 0;//ulCount_1 = ulCount_5 = ulCount_10 = ulCount_20 = ulCount_50 = ulCount_100 = 0;
			banknoteBoxID = _banknote_box_id();
		}
	} BANKNOTE_RECYCLER_BOX_INFO;

	// ֽ�һ��ղ��������ṹ��
	typedef struct _banknote_box_info_{
		LONG		ulOperatorID;		// ����Ա��ţ���ǰ������ֽ�����жƾ֤��ӡ��
		_DATE_TIME  operateTime;		// ����ʱ�䣨��ǰ������ֽ�����жƾ֤��ӡ��
		BANKNOTE_BOX_ID banknoteBoxID;
		LONG		ulCount_CNY1;           // 1Ԫ
		LONG		ulCount_CNY5;			// 5Ԫ
		LONG		ulCount_CNY10;			// 10Ԫ
		LONG		ulCount_CNY20;			// 20Ԫ
		LONG		ulCount_CNY50;			// 50Ԫ
		LONG		ulCount_CNY100;			// 100Ԫ
		LONG		ulLastLeftAmount;		// �ϴ�����
		_banknote_box_info_(){
			ulOperatorID = 0;
			operateTime = _DATE_TIME();
			ulCount_CNY1 = 0;
			ulCount_CNY5 = 0;
			ulCount_CNY10 = 0;
			ulCount_CNY20 = 0;
			ulCount_CNY50 = 0;
			ulCount_CNY100 = 0;
			ulLastLeftAmount = 0;
		}

		void Serialize(LPBYTE lpData) const
		{
			memcpy(lpData,&ulOperatorID,4);
			lpData+=4;
			memcpy(lpData,&operateTime,7);
			lpData+=7;
			banknoteBoxID.Serialze(lpData);
			lpData+=4;
			memcpy(lpData,&ulCount_CNY1,4);
			lpData+=4;
			memcpy(lpData,&ulCount_CNY5,4);
			lpData+=4;
			memcpy(lpData,&ulCount_CNY10,4);
			lpData+=4;
			memcpy(lpData,&ulCount_CNY20,4);
			lpData+=4;
			memcpy(lpData,&ulCount_CNY50,4);
			lpData+=4;
			memcpy(lpData,&ulCount_CNY100,4);
			lpData+=4;
			memcpy(lpData,&ulLastLeftAmount,4);
			lpData+=4;
		}
		void Deserialize(LPBYTE lpData)
		{
			memcpy(&ulOperatorID,lpData,4);
			lpData+=4;
			memcpy(&operateTime,lpData,7);
			lpData+=7;
			banknoteBoxID.Deserialize(lpData);
			lpData+=4;
			memcpy(&ulCount_CNY1,lpData,4);
			lpData+=4;
			memcpy(&ulCount_CNY5,lpData,4);
			lpData+=4;
			memcpy(&ulCount_CNY10,lpData,4);
			lpData+=4;
			memcpy(&ulCount_CNY20,lpData,4);
			lpData+=4;
			memcpy(&ulCount_CNY50,lpData,4);
			lpData+=4;
			memcpy(&ulCount_CNY100,lpData,4);
			lpData+=4;
			memcpy(&ulLastLeftAmount,lpData,4);
			lpData+=4;
		}

		DWORD BankNotesCountAvos(){
			return ulCount_CNY1 * 100 + ulCount_CNY5 * 500 + ulCount_CNY10 * 1000 + ulCount_CNY20 * 2000 + ulCount_CNY50 * 5000 + ulCount_CNY100 * 10000;
		}
		DWORD BankntoeTotalCount(){
			return ulCount_CNY1 + ulCount_CNY5 + ulCount_CNY10 + ulCount_CNY20 + ulCount_CNY50 + ulCount_CNY100;
		}

		void BanknoteInit(){
			ulCount_CNY1 = 0;
			ulCount_CNY5 = 0;
			ulCount_CNY10 = 0;
			ulCount_CNY20 = 0;
			ulCount_CNY50 = 0;
			ulCount_CNY100 = 0;
			banknoteBoxID = _banknote_box_id();
		}

	} BANKNOTE_BOX_INFO;
	//*********************************************************************
	// Ŀǰֽ�������������ȡ�����ù��ܣ��ӿ�ʵ�֣���������ļ��Ķ�д��ʵ��
	//*********************************************************************
	BOOL	Internalize(CString dataPath, CString backupPath);				// ���ڻ�
	

	/*
		��������Ϣ
	*/
	BOOL	SetBanknoteboxInfo(BANKNOTE_BOX_INFO& boxInfo);					// ����ֽ�һ����������Ϣ
	
	BOOL	SetBanknoteRecyclerBoxAInfo(BANKNOTE_RECYCLER_BOX_INFO& boxInfo);	// ѭ����A������Ϣ
	BOOL	SetBanknoteRecyclerBoxBInfo(BANKNOTE_RECYCLER_BOX_INFO& boxInfo);	// ѭ����B������Ϣ
	BOOL	SetBanknoteRecyclerBoxCInfo(BANKNOTE_RECYCLER_BOX_INFO& boxInfo);	// ѭ����C������Ϣ
	BOOL	SetBanknoteRecyclerBoxDInfo(BANKNOTE_RECYCLER_BOX_INFO& boxInfo);	// ѭ����D������Ϣ
	
	BOOL	SetBanknoteChangeBoxInfo(BANKNOTE_RECYCLER_BOX_INFO& changeInfo);	// �����������Ϣ


	/*
		��ȡ����Ϣ
	*/
	BANKNOTE_BOX_INFO&	GetBanknoteboxInfo();								// ȡ��ֽ�һ����������Ϣ
	
	BANKNOTE_RECYCLER_BOX_INFO& GetBanknoteRecyclerBoxAInfo();				// ȡ��ֽ��ѭ����A������Ϣ
	BANKNOTE_RECYCLER_BOX_INFO& GetBanknoteRecyclerBoxBInfo();				// ȡ��ֽ��ѭ����B������Ϣ
	BANKNOTE_RECYCLER_BOX_INFO& GetBanknoteRecyclerBoxCInfo();				// ȡ��ֽ��ѭ����C������Ϣ
	BANKNOTE_RECYCLER_BOX_INFO& GetBanknoteRecyclerBoxDInfo();				// ȡ��ֽ��ѭ����D������Ϣ

	BANKNOTE_RECYCLER_BOX_INFO& GetBanknoteChangeBoxInfo();					// ȡ��ֽ�����������Ϣ

	delegate<void(BANKNOTE_BOX_INFO&)> BanknoteCounterChanged;				// Ʊ����������¼�


	static CTBanknoteCountInfo &GetInstance()
	{
		return theInstance;
	}

private:
	enum {
		BANKNOTE_BOX_COUNT					= 1,
		BANKNOTE_BOX_INFO_LEN				= 32+11,
		BANKNOTE_COUNT_INFO_FILE_LEN		= BANKNOTE_BOX_INFO_LEN*BANKNOTE_BOX_COUNT + (16+11)*5, 
		//BANKNOTE_CHANGE_INFO_LEN			= 16*5,
	};

	void UpdateMasterFile();										// ���²������ļ�
	void UpdateMasterFileNoCopy();									// ���²������ļ�(�������ļ�)

	// ˽�г�Ա����m
	/*BANKNOTE_BOX_INFO	m_BanknoteBoxInfo;							// ֽ����ļ�����Ϣ
	BANKNOTE_CHANGE_BOX_INFO	m_BanknoteChangeBoxAInfo;			// ֽ��������A������Ϣ
	BANKNOTE_CHANGE_BOX_INFO	m_BanknoteChangeBoxBInfo;			// ֽ��������B������Ϣ
	BANKNOTE_CHANGE_BOX_INFO	m_BanknoteChangeWasteInfo;			// ֽ�ҷϱ��������Ϣ*/

	BANKNOTE_RECYCLER_BOX_INFO m_RecyclerBoxAInfo;					// ѭ����A������Ϣ
	BANKNOTE_RECYCLER_BOX_INFO m_RecyclerBoxBInfo;					// ѭ����B������Ϣ
	BANKNOTE_RECYCLER_BOX_INFO m_RecyclerBoxCInfo;					// ѭ����C������Ϣ
	BANKNOTE_RECYCLER_BOX_INFO m_RecyclerBoxDInfo;					// ѭ����D������Ϣ

	BANKNOTE_RECYCLER_BOX_INFO m_ChangeBoxInfo;						// �����������Ϣ
	BANKNOTE_BOX_INFO	m_BanknoteBoxInfo;							// �����������Ϣ

	CCriticalSection	m_lock;										// �ؼ�������
	CDXFile	*m_file;												// �ļ���ȡ������

	char	m_fileData[BANKNOTE_COUNT_INFO_FILE_LEN];				// ��ǰ�ļ�����
	
	static  CTBanknoteCountInfo theInstance;
	CTBanknoteCountInfo(void);
	~CTBanknoteCountInfo(void);
	CTBanknoteCountInfo(const CTBanknoteCountInfo&);
	CTBanknoteCountInfo &operator=(const CTBanknoteCountInfo&);
};
