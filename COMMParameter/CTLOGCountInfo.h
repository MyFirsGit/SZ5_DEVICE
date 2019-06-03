#pragma once

#include "Xfile.h"
#include "afxtempl.h"
#include "afxmt.h"
#include "CParameterCommandExp.h"
#include "CommonDefine.h"
#include "delegate.h"
#include "dxfile.h"

#define theCOUNT_INFO    CCTLOGCountInfo::GetInstance()         ///< CCTLOGCountInfo的唯一接口

/**
@brief 计数信息管理master   \n
       包括票箱信息和交易计数的读取与更新
*/
class PARAMETERCOMMAND_DECLSPEC  CCTLOGCountInfo
{
public:

	typedef struct _ticketbox_id
	{
		BYTE bDeviceType;	//设备类型 BDC
		BYTE bType;			//票箱类型 HEX
		BYTE bStationID;	//车站ID   BCD
		BYTE bNum;			//票箱序号 HEX
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
			// 空箱直接返回
			CString str(_T("FFFFFFFF"));
			if(bDeviceType == 0xFF){
				return str;
			}
			// 读取出的ID需要格式化为标准 1 BDC + 1HEX+1BCD + 1HEX
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

    // 计数信息
    typedef struct _ticketbox_info {
		TICKETBOX_ID ticketBoxID;
		LONG       ulInitCount;           // 票箱初期张数
        LONG       ulCurCount;            // 票箱当前张数
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
    } TICKETBOX_INFO;

    // 计数信息
    //typedef enum _log_type
    //{
    //    LOG_DATA_UPLOAD           = 1,                 // 标准日志
    //    SEND_FAIL_MESSAGE_LOG     = 2,                 // 失败日志
    //} LOG_TYPE;

	BOOL Internalize(CString dataPath, CString backupPath);         // 初期化

	TICKETBOX_INFO & GetTicketboxAInfo();                      // 读取票箱1的计数信息
    TICKETBOX_INFO & GetTicketboxBInfo();                      // 读取票箱2的计数信息
	TICKETBOX_INFO & GetTicketboxCInfo();                      // 读取废票箱的计数信息

    WORD GetCollectionCurCount();									// 得到回收箱的当前张数
    void SetCollectionCurCount(WORD collectionTicketCount);         // 设置回收箱的当前张数

    BOOL SetTicketboxAInfo(TICKETBOX_INFO& boxInfo);                 // 设置票箱1的计数信息
    BOOL SetTicketboxBInfo(TICKETBOX_INFO& boxInfo);                 // 设置票箱2的计数信息
	BOOL SetTicketboxCInfo(TICKETBOX_INFO& boxInfo);                 // 设置废票箱的计数信息


 //   void GetAuditEndTime(_DATE_TIME&);                              // 得到审计结束时间
 //   BOOL SetAuditEndTime(_DATE_TIME);                               // 设置审计结束时间
	
	//计数器重置事件
	delegate<void(COUNTER_TYPE)> CounterReseted;

    ULONG GetUDSN();											    // 得到ACC交易计数
	bool  UpdateUDSN(BYTE);											// 更新ACC交易计数
    ULONG GetUDSNForDisplay();                                      // 得到ACC交易计数，不累加
	ULONG GetLastUsedUDSN();										// 得到最后一笔ACC交易计数
    void SetUDSNForMain(ULONG);                                     // 设置ACC交易计数 

	ULONG GetTransactionNum();                                      // 得到AFC交易计数       
    ULONG GetTransactionNumForDsp();                                // 得到AFC交易计数，不累加   
	ULONG GetLastUsedTransactionNum();								// 得到最后一笔AFC交易计数
     void SetTransactionNumForMain(ULONG);                          // 写入AFC交易计数
	
	ULONG GetEventNum();                                            // 得到event计数
    ULONG GetEventNumForDsp();                                      // 得到event计数，不累加
	ULONG GetLastUsedEventNum();									// 得到最后一笔事件业务的计数
	void SetEventNumForMain(ULONG);                                 // 写入event计数 
    
	ULONG GetMaintenceNum();                                        // 得到Maintence计数
    
	ULONG GetECTNum();                                              // 得到ECT交易计数
    ULONG GetECTNumForDsp();                                              // 得到ECT交易计数，不累加
	void  UpdateECTNum(BYTE);                                       // 更新ECT交易计数
	ULONG GetLastUsedECTNum();											// 得到最后笔业务的ECT交易计数
    void SetECTNumForMain(ULONG);                                   // 写入ECT交易
  
    DWORD GetLoginCount();											// 得到操作员计数
    bool  SetLoginCount(DWORD logDate);								// 设置操作员计数
	DWORD AccumulateLoginCount();									// 累加操作员计数

	// TH发行提速增加方法
	//void CopystrBoxBackupFile();										// 把主文件复制到备份文件
	//ULONG GetUDSNNoCopy();                                          // 得到终端设备交易计数(不备份文件)
	//ULONG GetTransactionNumNoCopy();                                // 得到transaction计数(不备份文件)

	static CCTLOGCountInfo &GetInstance()
    {
        return theInstance;
    }

private:
    enum {
	    COUNT_INFO_LEN	= 67+16,                                    // 文件长度
		PR_ERROR_MSG_MAX_LEN = 261,                                 // 异常最大文字数
    };
    // 私有成员函数
    bool SetUDSN(ULONG udsn);                                       // 设置终端设备交易计数
	bool SetUDSNNoCopy(ULONG udsn);                                 // 设置终端设备交易计数(不备份文件)
    bool SetTransactionNum(ULONG);                                  // 写入transaction计数
	bool SetTransactionNumNoCopy(ULONG);                            // 写入transaction计数(不备份文件)
    bool SetEventNum(ULONG);                                        // 写入event计数 
    bool SetMaintenceNum(ULONG);                                    // 写入Maintence计数 
    bool SetYKTNum(ULONG);                                          // 写入一卡通计数

    void UpdateMasterFile();                                        // 更新并保存文件
	void UpdateMasterFileNoCopy();                                  // 更新并保存文件(不备份文件)

    // 私有成员变量
    WORD        m_collectionTicketCount;                            // 回收箱当前张数 
    TICKETBOX_INFO  m_TicketboxAInfo;                                  // 票箱1的计数信息
    TICKETBOX_INFO  m_TicketboxBInfo;                                  // 票箱2的计数信息
	TICKETBOX_INFO  m_TicketboxCInfo;                                  // 票箱2的计数信息

    _DATE_TIME  m_auditEndTime;                                     // 审计结束时间
    ULONG       m_ulUDSN;                                           // 终端设备交易计数
    ULONG       m_transactionNum;                                   // transaction计数
    ULONG       m_currentTraNum;                                    // 存储写入前的transaction值
    ULONG       m_eventNum;                                         // event计算
    ULONG       m_currentEveNum;                                    // 存储写入前的event的值
    ULONG       m_maintenceNum;                                     // maintence计算
    ULONG       m_currentMainNum;                                   // 存储写入前的maintence的值
    ULONG       m_ECTNum;                                           // 一卡通计算
    ULONG       m_currentECTNum;                                    // 存储写入前的一卡通的值
    DWORD       m_loginCount;                                       // 操作员计数
    DWORD       m_failLogDate;                                      // 
    CDXFile      *m_file;                                            // 文件读取控制类
    char        m_fileData[COUNT_INFO_LEN];                         // 当前文件内容
    CCriticalSection m_lock;                                        // 关键区控制

    static  CCTLOGCountInfo theInstance;
    
    // 构造，析构函数
    CCTLOGCountInfo(void);
    ~CCTLOGCountInfo(void);
    CCTLOGCountInfo(const CCTLOGCountInfo&);
    CCTLOGCountInfo &operator=(const CCTLOGCountInfo&);
};
