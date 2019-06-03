#pragma once

#include "CParameterCommandExp.h"
#include "dxfile.h"


#define thePRE_TK    CCTLOGPresellTicketInfo::GetInstance()       ///< CCTLOGPresellTicketInfo的唯一接口

/**
@brief 预制票信息Master 此Master文件主要存储的是预制票ID
*/
class PARAMETERCOMMAND_DECLSPEC  CCTLOGPresellTicketInfo
{
public:

	// 预制票结构体
	typedef struct _presell_ticket_id {
		CardSerialNumber_t presellId;       // 预制票ID
		_presell_ticket_id()
		{
			presellId = 0;
		}   
	} PRESELL_TICKET_ID;

	bool Internalize(CString dataPath, CString backupPath);          // 初期化
	bool UpdatePresellTicketInfo(vector<PRESELL_TICKET_ID>);         // 更新预制票文件
	bool CheckIsPreTicket(CardSerialNumber_t);                       // 判断是否为预制票

	static CCTLOGPresellTicketInfo &GetInstance()
	{
		return theInstance;
	}

private:

	enum {
		MIN_FILE_LENGTH = 4,						///< 预制票信息文件最小长度
	};

	static  CCTLOGPresellTicketInfo theInstance;    // 该类的唯一接口

	// 私有成员函数
	void AddWriteFile(vector<PRESELL_TICKET_ID>);   // 追加文件
	void SetFileDate();                             // 设置预制票文件的日期

	// 私有成员变量
	vector<PRESELL_TICKET_ID>  m_preTicketNoList;   // 预制票内卡编号
	CDXFile  *m_presellFile;                          // 预制票文件
	_DATE   m_curDate;                               // 预制票文件中存的日期

	CCTLOGPresellTicketInfo(void);
	~CCTLOGPresellTicketInfo();
	CCTLOGPresellTicketInfo(const CCTLOGPresellTicketInfo&);
	CCTLOGPresellTicketInfo& operator=(const CCTLOGPresellTicketInfo&);
};
