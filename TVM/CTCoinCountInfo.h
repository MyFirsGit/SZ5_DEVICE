#pragma once
#include "Xfile.h"
#include "afxtempl.h"
#include "afxmt.h"
#include "CParameterCommandExp.h"
#include "CommonDefine.h"
#include "delegate.h"
#include "dxfile.h"

#define theCOIN_COUNT    CTCoinCountInfo::GetInstance()         ///< CTCoinCountInfo��Ψһ�ӿ�

/**
@brief Ӳ��Ǯ�������Ϣ����������Ϣ��ȡ�����
*/

// 20171218,lichao: ��Ϊֱ��ID���ֶδ�BCD��ʽ������������ʾ�����������У�ID��������ת����
class CTCoinCountInfo
{
public:

	typedef struct _coin_box_id
	{
		BYTE bLineCode;
		BYTE bType;
		BYTE bNumInQu;
		BYTE bNum;
		_coin_box_id()
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

		bool operator == (const _coin_box_id& another) const
		{
			if(bLineCode != another.bLineCode) return false;
			if(bType != another.bType) return false;
			if (bNumInQu != another.bNumInQu) return false;
			if(bNum != another.bNum) return false;
			return true;
		}
	} COIN_BOX_ID;

	// Ӳ��HOPPER��ID

	typedef struct _coin_hopper_box_id
	{
		BYTE bLineCode;
		BYTE bType;
		BYTE bStationCode;
		BYTE wNum;	// ��Դ���豸ID�е����һλ������ΪN�����豸����ߵ�Hopper��3��N �C 2���豸���ұߵ�Hopper��3��N �C 1���豸��ѭ������Hopper��3��N
		_coin_hopper_box_id()
		{
			bLineCode = 0xFF;
			bType = 0xFF;
			bStationCode = 0xFF;
			wNum = 0xFF;
		}
		void Serialze(LPBYTE lpData) const
		{
			*lpData++ = bLineCode;
			*lpData++ = bType;
			*lpData++ = bStationCode;
			*lpData++ = wNum;
		}
		void Deserialize(LPBYTE lpData)
		{
			bLineCode = *lpData++;
			bType = *lpData++;
			bStationCode = *lpData++;
			wNum = *lpData++;
		}
		CString ToString()
		{
			CString str;
			str.Format(_T("%02X%02X%02X%02X"),bLineCode,bType,bStationCode,wNum);
			return str;
		}

		bool operator == (const _coin_hopper_box_id& another) const
		{
			if(bLineCode != another.bLineCode) return false;
			if(bType != another.bType) return false;
			if(bStationCode != another.bStationCode) return false;
			if(wNum != another.wNum) return false;
			return true;
		}
	} COIN_HOPPER_BOX_ID;

	// Ӳ�һ����������Ϣ
	typedef struct _coin_collection_box_info {
		LONG		ulOperatorID;		// ����Ա��ţ���ǰ�����ڲ�жƾ֤��ӡ��
		_DATE_TIME  operateTime;		// ����ʱ�䣨��ǰ�����ڲ�жƾ֤��ӡ��
		COIN_BOX_ID coinBoxID;
		LONG       ulCount_1yuan;           // 1Ԫö��
		LONG       ulCount_5jiao;           // 5��ö��
		LONG       ulInitCount;				// ����ö��(�ϴ�����)
		//LONG       ulCurCount;            // ��ǰö��
		//LONG       ulWriteCnt;            // ���ݸ�д����     (�����ж�)
		_coin_collection_box_info(){
			ulOperatorID = 0;
			operateTime = _DATE_TIME();
			coinBoxID = COIN_BOX_ID();
			ulCount_1yuan = 0;           // 1Ԫö��
			ulCount_5jiao = 0;			 // 5��ö��
			ulInitCount = 0;
			//ulCurCount = 0;
			//ulWriteCnt = 0;						
		}
		void Serialize(LPBYTE lpData) const
		{
			//memcpy(lpData,&ulOperatorID,4);
			//lpData+=4;
			//memcpy(lpData,&operateTime,7);
			//lpData+=7;
			coinBoxID.Serialze(lpData);
			lpData+=4;
			memcpy(lpData,&ulCount_1yuan,4);
			lpData+=4;
			memcpy(lpData,&ulCount_5jiao,4);
			lpData+=4;
			memcpy(lpData,&ulInitCount,4);
			lpData+=4;
		}
		void Deserialize(LPBYTE lpData)
		{
			//memcpy(&ulOperatorID,lpData,4);
			//lpData+=4;
			//memcpy(&operateTime,lpData,7);
			//lpData+=7;
			coinBoxID.Deserialize(lpData);
			lpData+=4;
			memcpy(&ulCount_1yuan,lpData,4);
			lpData+=4;
			memcpy(&ulCount_5jiao,lpData,4);
			lpData+=4;
			memcpy(&ulInitCount,lpData,4);
			lpData+=4;
		}
		void CheckMinusValue()
		{
			if(ulCount_1yuan<0){
				ulCount_1yuan = 0;
			}
			if(ulCount_5jiao<0){
				ulCount_5jiao = 0;
			}
			if(ulInitCount<0){
				ulInitCount = 0;
			}
		}
		long GetCurCount(){
			return  ulCount_1yuan + ulCount_5jiao;
		}

		// ��λ����
		long GetCurAmount(){
			return ulCount_1yuan * 100 + ulCount_5jiao * 100;
		}
	} COIN_COLLECTION_BOX_INFO;

	// ������Ӳ�Ҽ�����Ϣ�����ݴ�����
	typedef struct _coin_change_box_indo {
		COIN_BOX_ID coinBoxID;
		LONG		ulCount;			// ����	
		LONG		ulValue;				// ��ֵ
		//LONG       ulCount_1yuan;           // 1Ԫö��
		//LONG       ulCount_5jiao;           // 5��ö��
		LONG       ulLastLeftCount;           // �ϴ�����
		_coin_change_box_indo(){
			coinBoxID = COIN_BOX_ID();
			ulCount = 0;				// ����
			ulValue = 0;				// ��ֵ
			ulLastLeftCount = 0;		// �ϴ���������
		}
		void Serialize(LPBYTE lpData) const
		{
			coinBoxID.Serialze(lpData);
			lpData+=4;
			memcpy(lpData,&ulCount,4);
			lpData+=4;
			memcpy(lpData,&ulValue,4);
			lpData+=4;
			memcpy(lpData,&ulLastLeftCount,4);
			lpData+=4;
		}
		void Deserialize(LPBYTE lpData)
		{
			coinBoxID.Deserialize(lpData);
			lpData+=4;
			memcpy(&ulCount,lpData,4);
			lpData+=4;
			memcpy(&ulValue,lpData,4);
			lpData+=4;
			memcpy(&ulLastLeftCount,lpData,4);
			lpData+=4;
		}
		void CheckMinusValue()
		{
			if(ulCount<0){
				ulCount = 0;
			}
			if(ulValue<0){
				ulValue = 0;
			}
			if(ulLastLeftCount<0){
				ulLastLeftCount = 0;
			}
		}
		long GetCurCount(){
			return  ulCount;
		}
	} COIN_CHANGE_BOX_INFO;

	// Ӳ���ݴ���1������Ϣ
	typedef struct _coin_temporary1_info {
		COIN_BOX_ID coinBoxID;
		LONG       ulCount_1yuan;           // 1Ԫö��
		LONG       ulCount_5jiao;           // 5��ö��
		LONG       ulWriteCnt;              // ���ݸ�д����     (�����ж�)
		_coin_temporary1_info(){
			coinBoxID.bNum = 0x01;
			ulCount_1yuan = 0;				// 1Ԫö��
			ulCount_5jiao = 0;				// 5��ö��
			ulWriteCnt = 0;						
		}
		void Serialize(LPBYTE lpData) const
		{
			coinBoxID.Serialze(lpData);
			lpData+=4;
			memcpy(lpData,&ulCount_1yuan,4);
			lpData+=4;
			memcpy(lpData,&ulCount_5jiao,4);
			lpData+=4;
			memcpy(lpData,&ulWriteCnt,4);
			lpData+=4;
		}
		void Deserialize(LPBYTE lpData)
		{
			coinBoxID.Deserialize(lpData);
			lpData+=4;
			memcpy(&ulCount_1yuan,lpData,4);
			lpData+=4;
			memcpy(&ulCount_5jiao,lpData,4);
			lpData+=4;
			memcpy(&ulWriteCnt,lpData,4);
			lpData+=4;
		}
		void CheckMinusValue()
		{
			if(ulCount_1yuan<0){
				ulCount_1yuan = 0;
			}
			if(ulCount_5jiao<0){
				ulCount_5jiao = 0;
			}
			if(ulWriteCnt<0){
				ulWriteCnt = 0;
			}
		}
	} COIN_TEMPORARY1_INFO;

	// Ӳ���ݴ���2������Ϣ
	typedef struct _coin_temporary2_info {
		COIN_BOX_ID coinBoxID;
		LONG       ulCount_1yuan;           // 1Ԫö��
		LONG       ulCount_5jiao;           // 5��ö��
		LONG       ulWriteCnt;              // ���ݸ�д����     (�����ж�)
		_coin_temporary2_info(){
			coinBoxID.bNum = 0x02;
			ulCount_1yuan = 0;				// 1Ԫö��
			ulCount_5jiao = 0;				// 5��ö��
			ulWriteCnt = 0;						
		}
		void Serialize(LPBYTE lpData) const
		{
			coinBoxID.Serialze(lpData);
			lpData+=4;
			memcpy(lpData,&ulCount_1yuan,4);
			lpData+=4;
			memcpy(lpData,&ulCount_5jiao,4);
			lpData+=4;
			memcpy(lpData,&ulWriteCnt,4);
			lpData+=4;
		}
		void Deserialize(LPBYTE lpData)
		{
			coinBoxID.Deserialize(lpData);
			lpData+=4;
			memcpy(&ulCount_1yuan,lpData,4);
			lpData+=4;
			memcpy(&ulCount_5jiao,lpData,4);
			lpData+=4;
			memcpy(&ulWriteCnt,lpData,4);
			lpData+=4;
		}
		void CheckMinusValue()
		{
			if(ulCount_1yuan<0){
				ulCount_1yuan = 0;
			}
			if(ulCount_5jiao<0){
				ulCount_5jiao = 0;
			}
			if(ulWriteCnt<0){
				ulWriteCnt = 0;
			}
		}
	} COIN_TEMPORARY2_INFO;

	// Ӳ��HOPPER�������Ϣ
	typedef struct _coin_box_info {
		LONG		ulOperatorID;		// ����Ա��ţ���ǰ�����ڼӱ�ƾ֤��ӡ��
		_DATE_TIME  operateTime;		// ����ʱ�䣨��ǰ�����ڼӱ�ƾ֤��ӡ��
		COIN_BOX_ID coinBoxID;
		LONG       ulInitCount;          // ����ö��
		LONG       ulCurCount;           // ��ǰö��
		LONG       ulWriteCnt;           // ���ݸ�д����     (�����ж�)
		_coin_box_info(){
			ulOperatorID = 0;
			operateTime = _DATE_TIME();
			ulInitCount = 0;           
			ulCurCount = 0;           
			ulWriteCnt = 0;         
		}
		void Serialize(LPBYTE lpData) const
		{
			coinBoxID.Serialze(lpData);
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
			coinBoxID.Deserialize(lpData);
			lpData+=4;
			memcpy(&ulInitCount,lpData,4);
			lpData+=4;
			memcpy(&ulCurCount,lpData,4);
			lpData+=4;
			memcpy(&ulWriteCnt,lpData,4);
			lpData+=4;
		}
		void CheckMinusValue()
		{
			if(ulInitCount<0){
				ulInitCount = 0;
			}
			if(ulCurCount<0){
				ulCurCount = 0;
			}
			if(ulWriteCnt<0){
				ulWriteCnt = 0;
			}
		}
	} COIN_BOX_INFO;

	// Ӳ�Ҳ����������Ϣ
	typedef struct _coin_add_box_info {
		COIN_BOX_ID coinBoxID;
		LONG       ulInitCount;          // ����ö��
		LONG       ulCurCount;           // ��ǰö��
		LONG       ulWriteCnt;           // ���ݸ�д����     (�����ж�)
		_coin_add_box_info(){
			ulInitCount = 0;           
			ulCurCount = 0;           
			ulWriteCnt = 0;         
		}
		void Serialize(LPBYTE lpData) const
		{
			coinBoxID.Serialze(lpData);
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
			coinBoxID.Deserialize(lpData);
			lpData+=4;
			memcpy(&ulInitCount,lpData,4);
			lpData+=4;
			memcpy(&ulCurCount,lpData,4);
			lpData+=4;
			memcpy(&ulWriteCnt,lpData,4);
			lpData+=4;
		}
		void CheckMinusValue()
		{
			if(ulInitCount<0){
				ulInitCount = 0;
			}
			if(ulCurCount<0){
				ulCurCount = 0;
			}
			if(ulWriteCnt<0){
				ulWriteCnt = 0;
			}
		}
	} COIN_ADD_BOX_INFO;


	//A ���Ӷ�Ӧ1Ԫ 
	//B ���Ӷ�Ӧ5ë
	BOOL Internalize(CString dataPath, CString backupPath);				// ���ڻ�

	//BOOL SetCoinAddboxAInfo(COIN_ADD_BOX_INFO& boxInfo);				// ����Ӳ�Ҳ�����A�ļ�����Ϣ
	//COIN_ADD_BOX_INFO& GetCoinAddboxAInfo();							// ��ȡӲ�Ҳ�����A�ļ�����Ϣ

	//BOOL SetCoinAddboxBInfo(COIN_ADD_BOX_INFO& boxInfo);				// ����Ӳ�Ҳ�����B�ļ�����Ϣ
	//COIN_ADD_BOX_INFO& GetCoinAddboxBInfo();							// ��ȡӲ�Ҳ�����B�ļ�����Ϣ

	BOOL SetCoinHopperboxAInfo(COIN_BOX_INFO& boxInfo);					// ����Ӳ��HOPPER��A�ļ�����Ϣ(���ģ�飺�ұ�)(���)
	COIN_BOX_INFO& GetCoinHopperboxAInfo();								// ��ȡӲ��HOPPER��A�ļ�����Ϣ(���ģ�飺�ұ�)(���)

	BOOL SetCoinHopperboxBInfo(COIN_BOX_INFO& boxInfo);					// ����Ӳ��HOPPER��B�ļ�����Ϣ(���ģ�飺���)(һԪ)
	COIN_BOX_INFO& GetCoinHopperboxBInfo();								// ��ȡӲ��HOPPER��B�ļ�����Ϣ(���ģ�飺���)(һԪ)

	BOOL SetCollectionCoinboxAInfo(COIN_COLLECTION_BOX_INFO& boxInfo);	// ����Ӳ�һ�����A�ļ�����Ϣ(���ģ�飺�ұ�)
	COIN_COLLECTION_BOX_INFO& GetCollectionCoinboxAInfo();				// ��ȡӲ�һ�����A�ļ�����Ϣ(���ģ�飺�ұ�)

	BOOL SetCollectionCoinboxBInfo(COIN_COLLECTION_BOX_INFO& boxInfo);	// ����Ӳ�һ�����B�ļ�����Ϣ(���ģ�飺���)
	COIN_COLLECTION_BOX_INFO& GetCollectionCoinboxBInfo();				// ��ȡӲ�һ�����B�ļ�����Ϣ(���ģ�飺���)

	BOOL SetChangeCoinboxAInfo(COIN_CHANGE_BOX_INFO& boxInfo);			// ����Ӳ��������A�ļ�����Ϣ(���ģ�飺�ұ�)
	COIN_CHANGE_BOX_INFO& GetChangeCoinboxAInfo();						// ��ȡӲ��������A�ļ�����Ϣ(���ģ�飺�ұ�)

	BOOL SetChangeCoinboxBInfo(COIN_CHANGE_BOX_INFO& boxInfo);			// ����Ӳ��������B�ļ�����Ϣ(���ģ�飺���)
	COIN_CHANGE_BOX_INFO& GetChangeCoinboxBInfo();						// ��ȡӲ��������B�ļ�����Ϣ(���ģ�飺���)

	BOOL SetCoinTemporary1Info(COIN_TEMPORARY1_INFO& boxInfo);			// ����Ӳ���ݴ���1�ļ�����Ϣ
	COIN_TEMPORARY1_INFO& GetCoinTemporary1Info();						// ����Ӳ���ݴ���1�ļ�����Ϣ

	BOOL SetCoinTemporary2Info(COIN_TEMPORARY2_INFO& boxInfo);			// ����Ӳ���ݴ���2�ļ�����Ϣ
	COIN_TEMPORARY2_INFO& GetCoinTemporary2Info();						// ����Ӳ���ݴ���2�ļ�����Ϣ

	BOOL UpdataBoxCountData(int nHopper1,int nHooper2,int nChangeBox1,int nChangeBox2,int nColl1,int nColl2);// ͬ������
	// Ӳ�Ҽ�������¼�
	delegate<void(COIN_BOX_INFO&)>				CoinBoxCounterChanged;
	delegate<void(COIN_COLLECTION_BOX_INFO&)>   CoinCollectionBoxCounterChanged;
	
	static CTCoinCountInfo &GetInstance(){
		return theInstance;
	}

private:
	enum {
		COIN_HOPPER_BOX_INFO_LEN		= 16+11,
		COIN_HOPPER_BOX_COUNT			= 2,
		COIN_COLLECTION_BOX_INFO_LEN	= 16+11,
		COIN_COLLECTION_BOX_COUNT		= 2,
		COIN_TEMPORARY_LEN				= 16,
		COIN_TEMPORARY_COUNT			= 2,
		COIN_CHANGE_BOX_INFO_LEN		= 16,
		COIN_CHANGE_BOX_COUNT			= 2,
		COIN_COUNT_INFO_FILE_LEN		= COIN_HOPPER_BOX_INFO_LEN * COIN_HOPPER_BOX_COUNT 
									+ COIN_COLLECTION_BOX_COUNT * COIN_COLLECTION_BOX_INFO_LEN
									+ COIN_TEMPORARY_LEN * COIN_TEMPORARY_COUNT
									+ COIN_CHANGE_BOX_INFO_LEN * COIN_CHANGE_BOX_COUNT,	//172
	};

	void UpdateMasterFile();                                        // ���²������ļ�
	void UpdateMasterFileNoCopy();                            // ���²������ļ�(�������ļ�)

	// ˽�г�Ա����
	COIN_ADD_BOX_INFO  m_CoinAddBoxAInfo;			// Ӳ�Ҳ�����A�ļ�����Ϣ
	COIN_ADD_BOX_INFO  m_CoinAddBoxBInfo;			// Ӳ�Ҳ�����B�ļ�����Ϣ
	COIN_BOX_INFO  m_CoinHopperBoxAInfo;			// Ӳ��HOPPER��A�ļ�����Ϣ
	COIN_BOX_INFO  m_CoinHopperBoxBInfo;			// Ӳ��HOPPER��B�ļ�����Ϣ
	COIN_COLLECTION_BOX_INFO  m_CoinCollectionBoxAInfo;            // Ӳ�һ�����A�ļ�����Ϣ
	COIN_COLLECTION_BOX_INFO  m_CoinCollectionBoxBInfo;            // Ӳ�һ�����B�ļ�����Ϣ
	COIN_TEMPORARY1_INFO m_CoinTemporary1Info;					   // Ӳ���ݴ���1�ļ�����Ϣ
	COIN_TEMPORARY2_INFO m_CoinTemporary2Info;					   // Ӳ���ݴ���2�ļ�����Ϣ
	COIN_CHANGE_BOX_INFO	m_CoinChangeBoxAInfo;					// Ӳ��������A�ļ�����Ϣ
	COIN_CHANGE_BOX_INFO	m_CoinChangeBoxBInfo;					// Ӳ��������B�ļ�����Ϣ

	CDXFile   *m_file;														// �ļ���ȡ������
	char      m_fileData[COIN_COUNT_INFO_FILE_LEN];// ��ǰ�ļ�����
	CCriticalSection m_lock;											// �ؼ�������

	static  CTCoinCountInfo theInstance;

	// ���죬��������
	CTCoinCountInfo(void);
	~CTCoinCountInfo(void);
	CTCoinCountInfo(const CTCoinCountInfo&);
	CTCoinCountInfo &operator=(const CTCoinCountInfo&);
};
