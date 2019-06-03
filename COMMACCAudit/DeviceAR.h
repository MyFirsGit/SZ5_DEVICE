#pragma once

#include "ACCAuditDef.h"
#include "CACCAuditException.h"
#include "Util.h"
#include "ACCAuditExp.h"


// DeviceAR的唯一接口
#define theDeviceAR DeviceAR::GetInstance()  
// 
class ACCAUDIT_DECLSPEC DeviceAR
{
public:

	static DeviceAR &GetInstance()
	{
		return theInstance;
	}
     
	void MakePackHead(LPBYTE ARCommHead,int ARFileTag);                           //获取审计数据报文头
	void FindPos(long RegistId,long& theUpdataPosition);                          // 查找所需更新数据的位置  
	void MakedataByTicketType(long ticketType,long theUpdataPosition,long updataData,LPBYTE addData);                  //添加新的车票类型信息

    long InitARFile(CString dataPath=_T(""),CString backupPath=_T(""));                                                                //初始化ACC日计文件
	bool AddProcessStatByType(long ticketType,long RegistId,long updatedata);         //增加分票型自动检票机处理统计
	bool ReadProcessStat(DEVICE_AR_REASON ARFileTag,VARIABLE_DATA& arMesg);                       //根据票型读取分票型自动检票机处理统计
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

    static DeviceAR theInstance;

	DeviceAR(void);
	~DeviceAR(void);
};
