#pragma once

#include "CParameterCommandExp.h"
#include "dxfile.h"


#define theREGI_INFO    CCTLOGRegister::GetInstance()              ///< CCTLOGCountInfo的唯一接口

/**
@brief Register信息

---------------------------------------------------------------
以下是维护数据的详细内容

Module ID		機器内部名称(MC)
0x10	        预留
0x11	        预留
0x12	        售票处理部(TH)
0x13	        预留
0x14	        预留
0x15	        预留
0x16	        预留
0x17	        预留
0x18	        预留
0x19	        预留
0x1A~0x1F	    预留

Condition:      TH
Condition	    項　目	内　容
000	            预留
～	            预留
029	            预留
030             票库1的售票张数
031             票库2的售票张数
032             廃票总张数
033             预留
034	            预留
035	            预留
～	            预留
255	            预留

---------------------------------------------------------------
以下是异常数据信息

Condition	    异常计数
000	            Total Count
～	            Total Count
255	            Total Count
---------------------------------------------------------------

以下是各数据在master中的存储情况
-------------------------------------------------------

项目名称             开始位置     BYTE数    类型       

记录开始时间             0          7       BCD        

维护数据                 7       4×256     BIN

异常数据存储个数       1031         1       BIN

异常数据               1032      6×256     BIN   
-------------------------------------------------------
*/

class PARAMETERCOMMAND_DECLSPEC  CCTLOGRegister
{
public:
	// 维护数据 Module ID
	typedef enum _maintain_module
	{
		MAINTAIN_UNUSED     = 0x00,                 ///< 未使用
		MAINTAIN_TH         = 0x12,                 ///< 售票处理部(TH)
	} MAINTAIN_MODULE;

	// 维护数据 Condition
	typedef enum _maintain_condition
	{
		MAGAZINE_ONE_CNT           = 30,            ///<  票箱1的售票张数
		MAGAZINE_TWO_CNT   	       = 31,            ///<  票箱2的售票张数
		INVALIDATED_TICKET_CNT     = 32,	        ///<  废票总张数
	} MAINTAIN_CONDITION;

	// 异常数据 结构
	typedef struct _error_info
	{
		WORD EC_code;                               // 错误信息的模块号＋位置
		ULONG count;                                // 错误信息的个数
		_error_info(){
			EC_code = 0;
			count = 0;
		}

	} ERROR_INFO;

	// 异常数据 Module ID
	typedef enum _error_module
	{
		ERROR_UNUSED        = 0x00,                 ///< 未使用
		ERROR_APLI          = 0x11,                 ///< 程序内部错误
		ERROR_TH            = 0x12,                 ///< TH错误
		ERROR_ICRW          = 0x13,                 ///< IC RW错误
	} ERROR_MODULE;

	bool Internalize(CString dataPath, CString backupPath);            // 初期化

	void GetRecordTime(_DATE_TIME&);                                   // 取得记录时间

	ULONG GetMaintDataCount(MAINTAIN_CONDITION,MAINTAIN_MODULE&);      // 得到维护数据的计数
	void UpdateMaintDataCount(MAINTAIN_CONDITION,ULONG count=1);       // 更新维护数据的计数

	bool ClearMaintData(MAINTAIN_CONDITION);                           // 清除指定的维护数据
	bool ClearAllMaintData();                                          // 清除所有的维护数据

	void GetErrDataCount(LPBYTE);                                      // 得到异常信息
	void UpdateErrDataCount(WORD);                                     // 更新异常数目

	static CCTLOGRegister&  GetInstance()
	{
		return theInstance;
	}

private:

	enum {
		REGISTER_LEN           = 3+1024+1+256*6,		     			///< 文件长度
		REGISTER_BLOCK_SIZE    = 4,										///< 计数数据的每一块的大小
		ERROR_BLOCK_SIZE       = 6,										///< maintenance数据的每一块的大小
		ERROR_INFO_SIZE        = 256*6,                                 ///< 错误信息所占的BYTE数
	};

	// 字段所在位置
	enum {
		START_TIME_POS         = 0,                                     // 记录开始时间位置
		MAINTAIN_POS           = 7,                                     // 维护数据位置
		ERROR_KINDS_POS        = 1031,                                  // 错误种类数量位置
		ERROR_INFO_POS         = 1032                                   // 错误信息位置
	};    

	static  CCTLOGRegister theInstance;

	CDXFile* m_file;
	char m_fileData[REGISTER_LEN];                                  // 当前文件内容
	CString m_dataPath;                                             // 数据主目录
	CString m_backupPath;                                           // 备份主目录

	_DATE_TIME m_registerTime;                                      // 记录时间

	ULONG m_maintCntTotal[256];                                     // 维护数据的计数
	ERROR_INFO m_errorInfo[256];                                    // 异常信息
	BYTE m_errorKindCnt;                                            // 错误种类总计

	bool SetRecordTime(_DATE_TIME);                                 // 设置记录时间

	bool ReadFile();                                                // 读取文件
	bool OverWriteFile(const char* , ULONG );                       // 覆盖文件

	CCTLOGRegister(void);
	~CCTLOGRegister();
	CCTLOGRegister(const CCTLOGRegister&);
	CCTLOGRegister& operator=(const CCTLOGRegister&);
};
