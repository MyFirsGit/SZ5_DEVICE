#pragma once

#include "Xfile.h"


static const TCHAR* const TVM_REFUND_FILE_NAME				    = _T("master\\TVMRefundInfo.mst");      // BOM对TVM故障退款

#define theTVMRefund CCTVMRefund::GetInstance()       ///< CCTLOGPresellTicketInfo的唯一接口

/**
@brief 预制票信息Master
此Master文件主要存储的是预制票ID
*/
class  CCTVMRefund
{
public:

	// 预制票结构体
	typedef struct _tvm_error_ticket_id {
		CString csTVMErrorSierialNo;
		_tvm_error_ticket_id()
		{
			csTVMErrorSierialNo = "";
		}   
	}TVM_ERROR_TICKET_ID;

	bool Internalize(CString dataPath, CString backupPath);				// 初期化
	bool UpdateTVMTicketInfo(vector<TVM_ERROR_TICKET_ID>);				// 更新TVM文件
	bool CheckIsTVMTicket(CString);										// 判断是否为TVM票

	static CCTVMRefund &GetInstance()
	{
		return theInstance;
	}

private:

	enum {
		MIN_FILE_LENGTH = 4,						///< 预制票信息文件最小长度
	};

	static  CCTVMRefund theInstance;    // 该类的唯一接口

	// 私有成员函数
	void AddWriteFile(vector<TVM_ERROR_TICKET_ID>);   // 追加文件
	void SetFileDate();                             // 设置预制票文件的日期

	// 私有成员变量
	vector<TVM_ERROR_TICKET_ID>  m_preTicketNoList;   // 预制票内卡编号
	CXFile  *m_presellFile;                          // 预制票文件
	_DATE   m_curDate;                               // 预制票文件中存的日期

	CCTVMRefund(void);
	~CCTVMRefund();
	CCTVMRefund(const CCTVMRefund&);
	CCTVMRefund& operator=(const CCTVMRefund&);
};
