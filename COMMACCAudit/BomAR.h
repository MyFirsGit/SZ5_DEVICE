#pragma once

#include "ACCAuditDef.h"
#include "CACCAuditException.h"
#include "Util.h"
#include "ACCAuditExp.h"


// BomAR的唯一接口
#define theBomAR BomAR::GetInstance()  
// 
class ACCAUDIT_DECLSPEC BomAR
{
public:

	static BomAR &GetInstance()
	{
		return theInstance;
	}
     
	void MakePackHead(LPBYTE ARCommHead,int ARFileTag);                           //获取审计数据报文头
	void FindPos(long RegistId,long& theUpdataPosition);                          // 查找所需更新数据的位置  
	void MakedataByTicketType(long ticketType,long theUpdataPosition,long updataData,LPBYTE addData);                  //添加新的车票类型信息

    long InitBomARFile();                                                                //初始化ACC日计文件
	bool AddBOMProcessStatByType(long ticketType,long RegistId,long updatedata);         //增加分票型自动检票机处理统计
	bool ReadBOMProcessStat(DEVICE_AR_REASON ARFileTag,VARIABLE_DATA& arMesg);                       //根据票型读取分票型自动检票机处理统计
    void UpdateSerialFile();
	
private:
	CString m_dataPath;
	CString m_BackupPath;
	CString currentFileName;
	CString BackFileName;
	CString sequenceFileName;

	long deviceType;
	long TicketCountInBox1;
	long TicketCountInBox2;

    static BomAR theInstance;

	BomAR(void);
	~BomAR(void);
};
