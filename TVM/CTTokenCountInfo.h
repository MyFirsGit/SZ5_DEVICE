#pragma once
#include "Xfile.h"
#include "afxtempl.h"
#include "afxmt.h"
#include "CParameterCommandExp.h"
#include "CommonDefine.h"
#include "delegate.h"
#include "dxfile.h"

#define theTOKEN_COUNT    CTTokenCountInfo::GetInstance()         ///< CTTokenCountInfo的唯一接口

/**
@brief Token计数信息管理，包括Token箱信息读取与更新
*/

// 20171218,lichao: 改为直接ID各字段存BCD格式，这样界面显示和上送数据中，ID不用再做转换。
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

	// 计数信息
	typedef struct _tokenbox_info {
		LONG		ulOperatorID;		// 操作员编号（当前仅用于拆卸凭证打印）
		_DATE_TIME  operateTime;		// 操作时间（当前仅用于拆卸凭证打印）
		TOKEN_BOX_ID tokenBoxID;
		LONG       ulInitCount;           // Token箱初期张数
		LONG       ulCurCount;           // Token箱当前张数
		LONG       ulWriteCnt;            // 数据改写次数     (不作判断)
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

	BOOL Internalize(CString dataPath, CString backupPath);            // 初期化

	BOOL SetTokenboxAInfo(TOKEN_BOX_INFO& boxInfo);                 // 设置Token箱1的计数信息
	TOKEN_BOX_INFO& GetTokenboxAInfo();								// 读取Token箱1的计数信息

	BOOL SetTokenboxBInfo(TOKEN_BOX_INFO& boxInfo);                 // 设置Token箱2的计数信息
	TOKEN_BOX_INFO& GetTokenboxBInfo();                             // 读取Token箱2的计数信息

	BOOL SetHopperAInfo(TOKEN_BOX_INFO& hopperInfo);				// 设置Hopper1的计数信息
	TOKEN_BOX_INFO& GetHopperAInfo();								// 读取Hopper1的计数信息

	BOOL SetHopperBInfo(TOKEN_BOX_INFO& hopperInfo);				// 设置Hopper2的计数信息
	TOKEN_BOX_INFO& GetHopperBInfo();								// 读取Hopper2的计数信息

	BOOL SetTokenboxWastedInfo(TOKEN_BOX_INFO& boxInfo);			// 设置废Token箱的当前张数
	TOKEN_BOX_INFO& GetTokenboxWastedInfo();						// 得到废Token箱的当前张数

	BOOL SetTokenboxCollectedInfo(TOKEN_BOX_INFO& boxInfo);			// 设置Token回收箱的当前张数
	TOKEN_BOX_INFO& GetTokenboxCollectedInfo();						// 得到Token回收箱的当前张数

	// 票卡计数变更事件
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

	void UpdateMasterFile();                                        // 更新并保存文件
	void UpdateMasterFileNoCopy();									// 更新并保存文件(不备份文件)

	void UpdateHopperId();											// 更新HopperID

	// 私有成员变量
	TOKEN_BOX_INFO  m_TokenboxAInfo;									// Token箱A的计数信息
	TOKEN_BOX_INFO  m_TokenboxBInfo;									// Token箱B的计数信息
	TOKEN_BOX_INFO  m_HopperAInfo;										// Hopper箱A的计数信息
	TOKEN_BOX_INFO  m_HopperBInfo;										// Hopper箱B的计数信息
	TOKEN_BOX_INFO  m_TokenboxWastedInfo;								// 废Token箱的计数信息
	TOKEN_BOX_INFO  m_TokenboxCollectedInfo;							// Token回收箱的计数信息

	CDXFile   *m_file;														// 文件读取控制类
	char        m_fileData[TOKEN_COUNT_INFO_FILE_LEN];// 当前文件内容
	CCriticalSection m_lock;											// 关键区控制

	static  CTTokenCountInfo theInstance;

	// 构造，析构函数
	CTTokenCountInfo(void);
	~CTTokenCountInfo(void);
	CTTokenCountInfo(const CTTokenCountInfo&);
	CTTokenCountInfo &operator=(const CTTokenCountInfo&);
};
