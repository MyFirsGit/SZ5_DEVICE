#pragma once
#include "CParameterCommandExp.h"
#include "dxfile.h"

/**
@brief	记名卡申请记录Master类
*/

#define theNAMED_CARD_APPLY_RRSULT    CNamedCardApplyResult::GetInstance()       // 唯一接口

class PARAMETERCOMMAND_DECLSPEC  CNamedCardApplyResult
{

public:
	bool Internalize(CString, CString);                             // 初期化
	void UpdateTheApplyResult(BYTE*,BYTE status = 0);               // 更新指定记名卡申请的审核记录(若记录不存在则添加)
	bool GetTheApplyResult(BYTE*,BYTE&);                            // 获取指定记名卡申请的审核结果

	static CNamedCardApplyResult&  GetInstance()
	{
		return theInstance;
	}

private:

	static  CNamedCardApplyResult theInstance;             // 该类的唯一接口

	enum {
		FILE_BASE_SIZE = 22,					        // 单条记录长度 21 Byte  + 1byte
		FILE_DATE_SIZE = 4,					            // 日期记录长度
	};

	typedef struct _named_card_apply_result_info
	{
		BYTE    CertificatesNum[21];      // 证件类型 + 证件编码(1Byte + 20Byte)
		BYTE    Status;					  // 审核状态
		_named_card_apply_result_info(){
			memset(CertificatesNum,0,sizeof(CertificatesNum));
			Status = 0;
		}
	} NAMED_CARD_APP_RESULT;

	vector<NAMED_CARD_APP_RESULT>   m_ResultList;       // 审核数据记录体

	CDXFile* m_File;                                 // 数据文件
	CString m_dataPath;                             // 数据主目录
	CString m_backupPath;                           // 备份主目录
	_DATE   m_saveDate;                             // 结果查询日期
    CCriticalSection m_lock;                        // 关键区控制

	void AddNamedCardApplyRecord(BYTE*,BYTE status = 0);            // 添加记名卡申请记录
	bool IsRecordExist(BYTE* );                                     // 判断记名卡申请记录是否存在
	void ClearAllRecord();                                          // 清除记名卡申请记录
	bool ReadFile();                                                // 读取文件

	void SetFileDate();								// 写入文件存储日期
	void WriteFile();                               // 写文件

	CNamedCardApplyResult(void);
	~CNamedCardApplyResult();
	CNamedCardApplyResult(const CNamedCardApplyResult&);
	CNamedCardApplyResult& operator=(const CNamedCardApplyResult&);
};
