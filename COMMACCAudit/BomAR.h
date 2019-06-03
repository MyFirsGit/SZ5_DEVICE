#pragma once

#include "ACCAuditDef.h"
#include "CACCAuditException.h"
#include "Util.h"
#include "ACCAuditExp.h"


// BomAR��Ψһ�ӿ�
#define theBomAR BomAR::GetInstance()  
// 
class ACCAUDIT_DECLSPEC BomAR
{
public:

	static BomAR &GetInstance()
	{
		return theInstance;
	}
     
	void MakePackHead(LPBYTE ARCommHead,int ARFileTag);                           //��ȡ������ݱ���ͷ
	void FindPos(long RegistId,long& theUpdataPosition);                          // ��������������ݵ�λ��  
	void MakedataByTicketType(long ticketType,long theUpdataPosition,long updataData,LPBYTE addData);                  //����µĳ�Ʊ������Ϣ

    long InitBomARFile();                                                                //��ʼ��ACC�ռ��ļ�
	bool AddBOMProcessStatByType(long ticketType,long RegistId,long updatedata);         //���ӷ�Ʊ���Զ���Ʊ������ͳ��
	bool ReadBOMProcessStat(DEVICE_AR_REASON ARFileTag,VARIABLE_DATA& arMesg);                       //����Ʊ�Ͷ�ȡ��Ʊ���Զ���Ʊ������ͳ��
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
