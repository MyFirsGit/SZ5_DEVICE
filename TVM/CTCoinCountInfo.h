#pragma once
#include "Xfile.h"
#include "afxtempl.h"
#include "afxmt.h"
#include "CParameterCommandExp.h"
#include "CommonDefine.h"
#include "delegate.h"
#include "dxfile.h"

#define theCOIN_COUNT    CTCoinCountInfo::GetInstance()         ///< CTCoinCountInfo的唯一接口

/**
@brief 硬币钱箱计数信息管理，包括信息读取与更新
*/

// 20171218,lichao: 改为直接ID各字段存BCD格式，这样界面显示和上送数据中，ID不用再做转换。
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

	// 硬币HOPPER箱ID

	typedef struct _coin_hopper_box_id
	{
		BYTE bLineCode;
		BYTE bType;
		BYTE bStationCode;
		BYTE wNum;	// 来源与设备ID中的最后一位（假设为N），设备上左边的Hopper：3×N – 2，设备上右边的Hopper：3×N – 1，设备上循环找零Hopper：3×N
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

	// 硬币回收箱计数信息
	typedef struct _coin_collection_box_info {
		LONG		ulOperatorID;		// 操作员编号（当前仅用于拆卸凭证打印）
		_DATE_TIME  operateTime;		// 操作时间（当前仅用于拆卸凭证打印）
		COIN_BOX_ID coinBoxID;
		LONG       ulCount_1yuan;           // 1元枚数
		LONG       ulCount_5jiao;           // 5角枚数
		LONG       ulInitCount;				// 初期枚数(上次留存)
		//LONG       ulCurCount;            // 当前枚数
		//LONG       ulWriteCnt;            // 数据改写次数     (不作判断)
		_coin_collection_box_info(){
			ulOperatorID = 0;
			operateTime = _DATE_TIME();
			coinBoxID = COIN_BOX_ID();
			ulCount_1yuan = 0;           // 1元枚数
			ulCount_5jiao = 0;			 // 5角枚数
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

		// 单位：分
		long GetCurAmount(){
			return ulCount_1yuan * 100 + ulCount_5jiao * 100;
		}
	} COIN_COLLECTION_BOX_INFO;

	// 找零箱硬币计数信息（内容待定）
	typedef struct _coin_change_box_indo {
		COIN_BOX_ID coinBoxID;
		LONG		ulCount;			// 数量	
		LONG		ulValue;				// 面值
		//LONG       ulCount_1yuan;           // 1元枚数
		//LONG       ulCount_5jiao;           // 5角枚数
		LONG       ulLastLeftCount;           // 上次留存
		_coin_change_box_indo(){
			coinBoxID = COIN_BOX_ID();
			ulCount = 0;				// 数量
			ulValue = 0;				// 面值
			ulLastLeftCount = 0;		// 上次留存数量
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

	// 硬币暂存器1计数信息
	typedef struct _coin_temporary1_info {
		COIN_BOX_ID coinBoxID;
		LONG       ulCount_1yuan;           // 1元枚数
		LONG       ulCount_5jiao;           // 5角枚数
		LONG       ulWriteCnt;              // 数据改写次数     (不作判断)
		_coin_temporary1_info(){
			coinBoxID.bNum = 0x01;
			ulCount_1yuan = 0;				// 1元枚数
			ulCount_5jiao = 0;				// 5角枚数
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

	// 硬币暂存器2计数信息
	typedef struct _coin_temporary2_info {
		COIN_BOX_ID coinBoxID;
		LONG       ulCount_1yuan;           // 1元枚数
		LONG       ulCount_5jiao;           // 5角枚数
		LONG       ulWriteCnt;              // 数据改写次数     (不作判断)
		_coin_temporary2_info(){
			coinBoxID.bNum = 0x02;
			ulCount_1yuan = 0;				// 1元枚数
			ulCount_5jiao = 0;				// 5角枚数
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

	// 硬币HOPPER箱计数信息
	typedef struct _coin_box_info {
		LONG		ulOperatorID;		// 操作员编号（当前仅用于加币凭证打印）
		_DATE_TIME  operateTime;		// 操作时间（当前仅用于加币凭证打印）
		COIN_BOX_ID coinBoxID;
		LONG       ulInitCount;          // 初期枚数
		LONG       ulCurCount;           // 当前枚数
		LONG       ulWriteCnt;           // 数据改写次数     (不作判断)
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

	// 硬币补充箱计数信息
	typedef struct _coin_add_box_info {
		COIN_BOX_ID coinBoxID;
		LONG       ulInitCount;          // 初期枚数
		LONG       ulCurCount;           // 当前枚数
		LONG       ulWriteCnt;           // 数据改写次数     (不作判断)
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


	//A 箱子对应1元 
	//B 箱子对应5毛
	BOOL Internalize(CString dataPath, CString backupPath);				// 初期化

	//BOOL SetCoinAddboxAInfo(COIN_ADD_BOX_INFO& boxInfo);				// 设置硬币补充箱A的计数信息
	//COIN_ADD_BOX_INFO& GetCoinAddboxAInfo();							// 读取硬币补充箱A的计数信息

	//BOOL SetCoinAddboxBInfo(COIN_ADD_BOX_INFO& boxInfo);				// 设置硬币补充箱B的计数信息
	//COIN_ADD_BOX_INFO& GetCoinAddboxBInfo();							// 读取硬币补充箱B的计数信息

	BOOL SetCoinHopperboxAInfo(COIN_BOX_INFO& boxInfo);					// 设置硬币HOPPER箱A的计数信息(面对模块：右边)(五角)
	COIN_BOX_INFO& GetCoinHopperboxAInfo();								// 读取硬币HOPPER箱A的计数信息(面对模块：右边)(五角)

	BOOL SetCoinHopperboxBInfo(COIN_BOX_INFO& boxInfo);					// 设置硬币HOPPER箱B的计数信息(面对模块：左边)(一元)
	COIN_BOX_INFO& GetCoinHopperboxBInfo();								// 读取硬币HOPPER箱B的计数信息(面对模块：左边)(一元)

	BOOL SetCollectionCoinboxAInfo(COIN_COLLECTION_BOX_INFO& boxInfo);	// 设置硬币回收箱A的计数信息(面对模块：右边)
	COIN_COLLECTION_BOX_INFO& GetCollectionCoinboxAInfo();				// 读取硬币回收箱A的计数信息(面对模块：右边)

	BOOL SetCollectionCoinboxBInfo(COIN_COLLECTION_BOX_INFO& boxInfo);	// 设置硬币回收箱B的计数信息(面对模块：左边)
	COIN_COLLECTION_BOX_INFO& GetCollectionCoinboxBInfo();				// 读取硬币回收箱B的计数信息(面对模块：左边)

	BOOL SetChangeCoinboxAInfo(COIN_CHANGE_BOX_INFO& boxInfo);			// 设置硬币找零箱A的计数信息(面对模块：右边)
	COIN_CHANGE_BOX_INFO& GetChangeCoinboxAInfo();						// 读取硬币找零箱A的计数信息(面对模块：右边)

	BOOL SetChangeCoinboxBInfo(COIN_CHANGE_BOX_INFO& boxInfo);			// 设置硬币找零箱B的计数信息(面对模块：左边)
	COIN_CHANGE_BOX_INFO& GetChangeCoinboxBInfo();						// 读取硬币找零箱B的计数信息(面对模块：左边)

	BOOL SetCoinTemporary1Info(COIN_TEMPORARY1_INFO& boxInfo);			// 设置硬币暂存器1的计数信息
	COIN_TEMPORARY1_INFO& GetCoinTemporary1Info();						// 设置硬币暂存器1的计数信息

	BOOL SetCoinTemporary2Info(COIN_TEMPORARY2_INFO& boxInfo);			// 设置硬币暂存器2的计数信息
	COIN_TEMPORARY2_INFO& GetCoinTemporary2Info();						// 设置硬币暂存器2的计数信息

	BOOL UpdataBoxCountData(int nHopper1,int nHooper2,int nChangeBox1,int nChangeBox2,int nColl1,int nColl2);// 同步计数
	// 硬币计数变更事件
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

	void UpdateMasterFile();                                        // 更新并保存文件
	void UpdateMasterFileNoCopy();                            // 更新并保存文件(不备份文件)

	// 私有成员变量
	COIN_ADD_BOX_INFO  m_CoinAddBoxAInfo;			// 硬币补充箱A的计数信息
	COIN_ADD_BOX_INFO  m_CoinAddBoxBInfo;			// 硬币补充箱B的计数信息
	COIN_BOX_INFO  m_CoinHopperBoxAInfo;			// 硬币HOPPER箱A的计数信息
	COIN_BOX_INFO  m_CoinHopperBoxBInfo;			// 硬币HOPPER箱B的计数信息
	COIN_COLLECTION_BOX_INFO  m_CoinCollectionBoxAInfo;            // 硬币回收箱A的计数信息
	COIN_COLLECTION_BOX_INFO  m_CoinCollectionBoxBInfo;            // 硬币回收箱B的计数信息
	COIN_TEMPORARY1_INFO m_CoinTemporary1Info;					   // 硬币暂存器1的计数信息
	COIN_TEMPORARY2_INFO m_CoinTemporary2Info;					   // 硬币暂存器2的计数信息
	COIN_CHANGE_BOX_INFO	m_CoinChangeBoxAInfo;					// 硬币找零箱A的计数信息
	COIN_CHANGE_BOX_INFO	m_CoinChangeBoxBInfo;					// 硬币找零箱B的计数信息

	CDXFile   *m_file;														// 文件读取控制类
	char      m_fileData[COIN_COUNT_INFO_FILE_LEN];// 当前文件内容
	CCriticalSection m_lock;											// 关键区控制

	static  CTCoinCountInfo theInstance;

	// 构造，析构函数
	CTCoinCountInfo(void);
	~CTCoinCountInfo(void);
	CTCoinCountInfo(const CTCoinCountInfo&);
	CTCoinCountInfo &operator=(const CTCoinCountInfo&);
};
