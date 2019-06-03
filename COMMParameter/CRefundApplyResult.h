#pragma once
#include "CParameterCommandExp.h"
#include "dxfile.h"

/**
@brief	非即时退款申请记录Master类
*/

#define theREFUND_APPLY_RRSULT    CRefundApplyResult::GetInstance()       // 唯一接口

class PARAMETERCOMMAND_DECLSPEC  CRefundApplyResult
{

public:
	typedef struct _refund_apply_result_info
	{
		CERTIFICATE_TYPE certificateType;	//证件类型
		BYTE certificateID[20];					//ASC证件编码
		BYTE ticketApplicationSerialNo[8];		//8HEX票卡应用序列号
		BYTE    ReceiptNum[10];           // 收据编号
		long	Balance;				  // 卡内余额
		long	Deposit;				  // 退押金金额
		BYTE    Status;					  // 审核状态
		_refund_apply_result_info(){
			certificateType = CERTIFICATE_TYPE::CERTIFICATE_UNSPECIFIED;
			memset(certificateID,0,sizeof(certificateID));
			memset(ticketApplicationSerialNo,0,sizeof(ticketApplicationSerialNo));
			memset(ReceiptNum,0,sizeof(ReceiptNum));
			Balance = 0;
			Deposit = 0;
			Status = 0;
		}
	} REFUND_APP_RESULT;
	bool Internalize(CString, CString);                             // 初期化
	void UpdateTheApplyResult(REFUND_APP_RESULT&);                // 更新指定退款申请的审核记录(若记录不存在则添加)
	bool GetTheApplyResult(REFUND_APP_RESULT&);                // 获取指定退款申请的审核结果

	static CRefundApplyResult&  GetInstance()
	{
		return theInstance;
	}

private:

	static  CRefundApplyResult theInstance;             // 该类的唯一接口

	enum {
		FILE_BASE_SIZE = 51,					        // 单条记录长度 4 Byte + 20 Byte + 8 Byte  + 4 byte + 4 byte + 1 byte
		FILE_DATE_SIZE = 4,					            // 日期记录长度
	};
	

	vector<REFUND_APP_RESULT>   m_ResultList;       // 审核数据记录体

	CDXFile* m_File;                                 // 数据文件
	CString m_dataPath;                             // 数据主目录
	CString m_backupPath;                           // 备份主目录
	_DATE   m_saveDate;                             // 结果查询日期
    CCriticalSection m_lock;                        // 关键区控制

	void AddRefundApplyRecord(BYTE*,long,long,BYTE status = 0);     // 添加退款申请记录
	bool IsRecordExist(BYTE* );                                     // 判断退款申请记录是否存在
	void ClearAllRecord();                                          // 清除退款申请记录
	bool ReadFile();                                                // 读取文件

	void SetFileDate();								// 写入文件存储日期
	void WriteFile();                               // 写文件
	void SaveToFile();
	CRefundApplyResult(void);
	~CRefundApplyResult();
	CRefundApplyResult(const CRefundApplyResult&);
	CRefundApplyResult& operator=(const CRefundApplyResult&);
};
