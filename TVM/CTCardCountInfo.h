#pragma once
#include "Xfile.h"
#include "afxtempl.h"
#include "afxmt.h"
#include "CParameterCommandExp.h"
#include "CommonDefine.h"
#include "delegate.h"
#include "dxfile.h"

#define theCARD_COUNT    CTCardCountInfo::GetInstance()         ///< CTCardCountInfo的唯一接口

/**
@brief 票卡计数信息管理，包括票箱信息读取与更新
*/

class CTCardCountInfo
{
public:

	typedef struct _card_box_id
	{
		BYTE bDeviceType;	//设备类型 BDC
		BYTE bType;			//票箱类型 HEX
		BYTE bStationID;	//车站ID   BCD
		BYTE bNum;			//票箱序号 HEX
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
			// 空箱直接返回
			CString str(_T("FFFFFFFF"));
			if(bDeviceType == 0xFF || bType == 0XFF || bStationID == 0XFF || bNum == 0XFF){
				return str;
			}
			// 读取出的ID需要格式化为标准 1 BDC + 1HEX+1BCD + 1HEX
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

    // 计数信息
    typedef struct _ticketbox_info {
		CARD_BOX_ID ticketBoxID;
		LONG       ulInitCount;           // 票箱初期张数
        LONG       ulCurCount;           // 票箱当前张数
        LONG       ulWriteCnt;            // 数据改写次数     (不作判断)
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

	BOOL Internalize(CString dataPath, CString backupPath);            // 初期化

	BOOL SetCardboxAInfo(CARD_BOX_INFO& boxInfo);                 // 设置票箱A的计数信息
	CARD_BOX_INFO& GetCardboxAInfo();									    // 读取票箱A的计数信息

	BOOL SetCardboxBInfo(CARD_BOX_INFO& boxInfo);                 // 设置票箱B的计数信息
    CARD_BOX_INFO& GetCardboxBInfo();                                      // 读取票箱B的计数信息

	BOOL SetCardboxWastedInfo(CARD_BOX_INFO& boxInfo);			// 设置废票箱的当前张数
    CARD_BOX_INFO& GetCardboxWastedInfo();								// 得到废票箱的当前张数
   
	// 票卡计数变更事件
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
 
	void UpdateMasterFile();                                        // 更新并保存文件
	void UpdateMasterFileNoCopy();                            // 更新并保存文件(不备份文件)

    // 私有成员变量
	CARD_BOX_INFO  m_CardboxAInfo;                                  // 票箱1的计数信息
	CARD_BOX_INFO  m_CardboxBInfo;                                  // 票箱2的计数信息
    CARD_BOX_INFO  m_CardboxWastedInfo;                         // 废票箱的计数信息

    CDXFile   *m_file;														// 文件读取控制类
    char        m_fileData[CARD_COUNT_INFO_FILE_LEN];// 当前文件内容
    CCriticalSection m_lock;											// 关键区控制

    static  CTCardCountInfo theInstance;
    
    // 构造，析构函数
    CTCardCountInfo(void);
    ~CTCardCountInfo(void);
    CTCardCountInfo(const CTCardCountInfo&);
    CTCardCountInfo &operator=(const CTCardCountInfo&);
};
