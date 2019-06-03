#pragma once
#include "Xfile.h"
#include "afxtempl.h"
#include "afxmt.h"
#include "CParameterCommandExp.h"
#include "CommonDefine.h"
#include "delegate.h"
#include "dxfile.h"

#define theBANKNOTE_COUNT    CTBanknoteCountInfo::GetInstance()         ///< CTBanknoteCountInfo的唯一接口

/**
@brief 纸币钱箱计数信息管理，包括信息读取与更新
*/

class CTBanknoteCountInfo
{
public:
	// 20171218,lichao: 改为直接ID各字段存BCD格式，这样界面显示和上送数据中，票箱ID不用再做转换。
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

	// 计数信息(纸币循环/找零模块)
	typedef struct _banknote_recycler_box_info {
		LONG		ulOperatorID;		// 操作员编号（当前仅用于纸币箱拆卸凭证打印）
		_DATE_TIME  operateTime;		// 操作时间（当前仅用于纸币箱拆卸凭证打印）
		BANKNOTE_BOX_ID banknoteBoxID;
		LONG		ulCurCount;			// 张数
		LONG		ulCurValue;			// 面额
		LONG		ulLastLeftAmount;	// 上次留存
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
			// 改变之前纸币解析顺序，去掉之前的500和1000面额的，之前加上1元和5元的
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

	// 纸币回收部件计数结构体
	typedef struct _banknote_box_info_{
		LONG		ulOperatorID;		// 操作员编号（当前仅用于纸币箱拆卸凭证打印）
		_DATE_TIME  operateTime;		// 操作时间（当前仅用于纸币箱拆卸凭证打印）
		BANKNOTE_BOX_ID banknoteBoxID;
		LONG		ulCount_CNY1;           // 1元
		LONG		ulCount_CNY5;			// 5元
		LONG		ulCount_CNY10;			// 10元
		LONG		ulCount_CNY20;			// 20元
		LONG		ulCount_CNY50;			// 50元
		LONG		ulCount_CNY100;			// 100元
		LONG		ulLastLeftAmount;		// 上次留存
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
	// 目前纸币找零箱计数获取和设置功能，接口实现，具体计数文件的读写待实现
	//*********************************************************************
	BOOL	Internalize(CString dataPath, CString backupPath);				// 初期化
	

	/*
		设置类信息
	*/
	BOOL	SetBanknoteboxInfo(BANKNOTE_BOX_INFO& boxInfo);					// 设置纸币回收箱计数信息
	
	BOOL	SetBanknoteRecyclerBoxAInfo(BANKNOTE_RECYCLER_BOX_INFO& boxInfo);	// 循环鼓A计数信息
	BOOL	SetBanknoteRecyclerBoxBInfo(BANKNOTE_RECYCLER_BOX_INFO& boxInfo);	// 循环鼓B计数信息
	BOOL	SetBanknoteRecyclerBoxCInfo(BANKNOTE_RECYCLER_BOX_INFO& boxInfo);	// 循环鼓C计数信息
	BOOL	SetBanknoteRecyclerBoxDInfo(BANKNOTE_RECYCLER_BOX_INFO& boxInfo);	// 循环鼓D计数信息
	
	BOOL	SetBanknoteChangeBoxInfo(BANKNOTE_RECYCLER_BOX_INFO& changeInfo);	// 找零箱计数信息


	/*
		获取类信息
	*/
	BANKNOTE_BOX_INFO&	GetBanknoteboxInfo();								// 取得纸币回收箱计数信息
	
	BANKNOTE_RECYCLER_BOX_INFO& GetBanknoteRecyclerBoxAInfo();				// 取得纸币循环鼓A计数信息
	BANKNOTE_RECYCLER_BOX_INFO& GetBanknoteRecyclerBoxBInfo();				// 取得纸币循环鼓B计数信息
	BANKNOTE_RECYCLER_BOX_INFO& GetBanknoteRecyclerBoxCInfo();				// 取得纸币循环鼓C计数信息
	BANKNOTE_RECYCLER_BOX_INFO& GetBanknoteRecyclerBoxDInfo();				// 取得纸币循环鼓D计数信息

	BANKNOTE_RECYCLER_BOX_INFO& GetBanknoteChangeBoxInfo();					// 取得纸币找零箱的信息

	delegate<void(BANKNOTE_BOX_INFO&)> BanknoteCounterChanged;				// 票卡计数变更事件


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

	void UpdateMasterFile();										// 更新并保存文件
	void UpdateMasterFileNoCopy();									// 更新并保存文件(不备份文件)

	// 私有成员变量m
	/*BANKNOTE_BOX_INFO	m_BanknoteBoxInfo;							// 纸币箱的计数信息
	BANKNOTE_CHANGE_BOX_INFO	m_BanknoteChangeBoxAInfo;			// 纸币找零箱A计数信息
	BANKNOTE_CHANGE_BOX_INFO	m_BanknoteChangeBoxBInfo;			// 纸币找零箱B计数信息
	BANKNOTE_CHANGE_BOX_INFO	m_BanknoteChangeWasteInfo;			// 纸币废币箱计数信息*/

	BANKNOTE_RECYCLER_BOX_INFO m_RecyclerBoxAInfo;					// 循环鼓A计数信息
	BANKNOTE_RECYCLER_BOX_INFO m_RecyclerBoxBInfo;					// 循环鼓B计数信息
	BANKNOTE_RECYCLER_BOX_INFO m_RecyclerBoxCInfo;					// 循环鼓C计数信息
	BANKNOTE_RECYCLER_BOX_INFO m_RecyclerBoxDInfo;					// 循环鼓D计数信息

	BANKNOTE_RECYCLER_BOX_INFO m_ChangeBoxInfo;						// 找零箱计数信息
	BANKNOTE_BOX_INFO	m_BanknoteBoxInfo;							// 回收箱计数信息

	CCriticalSection	m_lock;										// 关键区控制
	CDXFile	*m_file;												// 文件读取控制类

	char	m_fileData[BANKNOTE_COUNT_INFO_FILE_LEN];				// 当前文件内容
	
	static  CTBanknoteCountInfo theInstance;
	CTBanknoteCountInfo(void);
	~CTBanknoteCountInfo(void);
	CTBanknoteCountInfo(const CTBanknoteCountInfo&);
	CTBanknoteCountInfo &operator=(const CTBanknoteCountInfo&);
};
