#include "StdAfx.h"
#include "RWCommands.h"
#include "DebugLog.h"
#include "SysInfo.h"
#include "DXFile.h"
#include "DCR4000Comm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define theRW_TRACE CFileLog::GetInstance(m_RWID.name)

//////////////////////////////////////////////////////////////////////////
/**
@brief      CRWCommands���캯��

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWCommands::CRWCommands(RWID& rwid):m_RWID(rwid)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CRWCommands��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWCommands::~CRWCommands()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �򿪶�д��

@param      (i)unsigned long ulport    �˿ں�
@param      (i)unsigned long ulbaud    ������
@param      (i)BOOL		bIsUsb		   �Ƿ�ʹ��USB�ӿ�ͨ��

@retval     long    �ɹ�:0; ʧ��:����Ӧ����    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::Open(BYTE ulport, DWORD ulbaud,BOOL bIsUsb)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T(""));
	SYNC(RW,m_RWID.name);
	sLogString.Format(_T("OpenReader: nPort = %d;ulSpeed = %d;bIsUSB = %d"),ulport,ulbaud,bIsUsb);

	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
		lCompleteCode = DAT::OpenReader(ulport,ulbaud,bIsUsb,m_RWID.handle);
	//}
	//else
	//{
	//	m_RWID.status = STATUE_INIT;
	//}

	sLogString = _T("");
	sLogString.Format(_T("OpenReader: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData( _T("<") + sLogString,NULL,0);
	
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �رն�д������

@param      ��         

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::Close()
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T("CloseReader"));
	SYNC(RW,m_RWID.name);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	if(NULL != m_RWID.handle){
		//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
		//{
		lCompleteCode = DAT::CloseReader(m_RWID.handle);
		//}
		//else
		//{
		//	m_RWID.status = STATUE_INIT;
		//}
	}	
	sLogString = _T("");
	sLogString.Format(_T("CloseReader: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
    return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��λ��д������

@param      (o)unsigned char  Statue ��д��״̬      

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::Reset(BYTE& status)
{  
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T("Reset_TPU"));
	SYNC(RW,m_RWID.name);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::Reset_TPU(status,m_RWID.handle);
	//}
	//else
	//{
	//	m_RWID.status = STATUE_NOTINIT;
	//}
	sLogString = _T("");
	sLogString.Format(_T("Reset_TPU: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	if(lCompleteCode == RW_RSP_OK){
		sLogString = _T("");
		sLogString.Format(_T("Reset_TPU: status = 0x%02x"),status);
		theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	}
    return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��д��״̬����

@param      (o)BYTE& status  ��д��״̬   

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::GetStatus(BYTE& status)
{  
	long lCompleteCode  = RW_RSP_OK;
	CString sLogString(_T("GetStatue"));
	SYNC(RW,m_RWID.name);		
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode= DAT::GetStatue(status,m_RWID.handle);
	//}
	//else
	//{
	//	status = m_RWID.status;
	//}
	sLogString = _T("");
	sLogString.Format(_T("GetStatue: ReturnCode = 0x%02x;status = 0x%02x"),lCompleteCode,status);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ֹ��д������

@param      (o)BYTE& status  ��д��״̬       

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::Abort(BYTE& status)
{ 
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T("Abort_TPU"));

	SYNC(RW,m_RWID.name);		
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::Abort_TPU(status,m_RWID.handle);
	//}
	//else
	//{
	//	status = STATUE_IDLE;
	//	m_RWID.status = STATUE_IDLE;
	//}
	
	sLogString = _T("");
	sLogString.Format(_T("Abort_TPU: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		sLogString = _T("");
		sLogString.Format(_T("Abort_TPU: status = 0x%02x"),status);
		theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	}
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ��д������

@param      (o)BYTE& status  ��д��״̬       

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::Stop(BYTE& status)
{ 
	CString sLogString(_T("Stop_TPU"));
	long lCompleteCode = RW_RSP_OK;

	SYNC(RW,m_RWID.name);		
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::Stop_TPU(status,m_RWID.handle);
	//}
	//else
	//{
	//	status = STATUE_NOTINIT;
	//	m_RWID.status = STATUE_NOTINIT;
	//}
	
	sLogString = _T("");
	sLogString.Format(_T("Stop_TPU: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		sLogString = _T("");
		sLogString.Format(_T("Stop_TPU: status = 0x%02x"),status);
		theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	}
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ö�д��ʱ��

@param      (i)BYTE* SetTime ���õ�ʱ��   
@param      (o)BYTE* TPUTime ���ú�TPUʱ��

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::SetTime(BYTE* SetTime,BYTE *TPUTime)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T("SetReadTimer: SetTime ="));
	SYNC(RW,m_RWID.name);		
	theRW_TRACE->WriteData(_T(">")+sLogString,SetTime,7);
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::SetReadTimer(SetTime,TPUTime,m_RWID.handle);
	//}
	//else
	//{
	//	_DATE_TIME curTime = ComGetCurTime();
	//	memcpy(TPUTime,&curTime,sizeof(_DATE_TIME));
	//}
	sLogString = _T("");
	sLogString.Format(_T("SetReadTimer: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡTPUʱ��

@param      (o)BYTE *Time      TPUʱ��,BCD��,��ʽΪ��YYYYMMDDHHMMSS 

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::GetTime(BYTE *Time)
{	
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T("GetReadTime:"));
	SYNC(RW,m_RWID.name);
	theRW_TRACE->WriteData(_T(">")+sLogString,Time,7);

	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::GetReadTime(Time,m_RWID.handle);
	//}
	//else
	//{
	//	_DATE_TIME curTime = ComGetCurTime();
	//	memcpy(Time,&curTime,sizeof(_DATE_TIME));
	//}

	sLogString = _T("");
	sLogString.Format(_T("GetReadTime: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		sLogString = _T("GetReadTime: Time =");
		theRW_TRACE->WriteData(_T("<")+sLogString,Time,7);
	}
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���㽻����֤��(MAC)

@param      (i)BYTE *KeyVer    ��Կ�汾
@param      (i)BYTE *Data      ��Ҫ����MAC��Hash����
@param      (o)BYTE *MAC	   ���ڴ�ŷ��ص�MAC����

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::GetMac(BYTE *KeyVer,BYTE *Data,BYTE* MAC)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T("CalculateMAC: KeyVer ="));
	SYNC(RW,m_RWID.name);
	theRW_TRACE->WriteData(_T(">")+sLogString,KeyVer,2);

	sLogString =_T("CalculateMAC: Data =");
	theRW_TRACE->WriteData(_T(">")+sLogString,Data,20);

	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::CalculateMAC (KeyVer,Data,MAC,m_RWID.handle);
	//}
	//else
	//{
	//		memset(MAC,0xFF,4);
	//}

	sLogString = _T("");
	sLogString.Format(_T("CalculateMAC: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		sLogString = _T("CalculateMAC: MAC =");
		theRW_TRACE->WriteData(_T("<")+sLogString,MAC,4);
	}
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡTPU�������

@param      (o)int &iErrorCode    TPU��ϸ�������

@retval     long    �ɹ�:0; ʧ��:����Ӧ���� 

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::GetErrCode(int &iErrorCode)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T("GetErrorCode:"));
	SYNC(RW,m_RWID.name);

	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::GetErrorCode(iErrorCode,m_RWID.handle);
	//}
	//else
	//{
	//		iErrorCode = 0;
	//}
	
	sLogString = _T("");
	sLogString.Format(_T("GetErrorCode: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	if(lCompleteCode == RW_RSP_OK){
		sLogString = _T("");
		sLogString.Format(_T("GetErrorCode: iErrorCode = 0x%08x"),iErrorCode);
		theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	}
	return lCompleteCode;
}


bool CRWCommands::IsDcr4000ReturnCode(int returnCode)
{
	return ((returnCode&0xFF00)==0xFF00) || ((returnCode&0xE000) == 0xE000);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡTPU��Ϣ

@param      (o)DAT::TPUINFO&    TPU��Ϣ

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::GetTPUInfo(DAT::TPUINFO &Tpuinfo)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T("GetTPUInfo:"));
	SYNC(RW,m_RWID.name);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW"))){
			lCompleteCode = DAT::GetTPUInfo(Tpuinfo,m_RWID.handle);
	//}
	//else{
	//	VARIABLE_DATA data;
	//	//ReadSimDataFile(TPU_INFO_FILE_NAME,data);
	//	memcpy(&Tpuinfo,data.lpData,sizeof(DAT::TPUINFO));
	//}
	sLogString = _T("");
	sLogString.Format(_T("GetTPUInfo: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	if(lCompleteCode == RW_RSP_OK){
		sLogString = _T("GetTPUInfo: DeviceCode =");
		theRW_TRACE->WriteData(_T("<")+sLogString,Tpuinfo.DeviceCode,sizeof(Tpuinfo.DeviceCode));

		sLogString = _T("GetTPUInfo: AppSoftVer =");
		theRW_TRACE->WriteData(_T("<")+sLogString,Tpuinfo.AppSoftVer,sizeof(Tpuinfo.AppSoftVer));

		sLogString = _T("GetTPUInfo: HardwareVer =");
		theRW_TRACE->WriteData(_T("<")+sLogString,Tpuinfo.HardwareVer,sizeof(Tpuinfo.HardwareVer));

		sLogString = _T("GetTPUInfo: BMACPSAMID =");
//		theRW_TRACE->WriteData(_T("<")+sLogString,Tpuinfo.BMACPSAMID,sizeof(Tpuinfo.BMACPSAMID));

		sLogString = _T("GetTPUInfo: BMACISAMID =");
//		theRW_TRACE->WriteData(_T("<")+sLogString,Tpuinfo.BMACISAMID,sizeof(Tpuinfo.BMACISAMID));

		sLogString = _T("GetTPUInfo: ACCPSAMID =");
//		theRW_TRACE->WriteData(_T("<")+sLogString,Tpuinfo.ACCPSAMID,sizeof(Tpuinfo.ACCPSAMID));

		sLogString = _T("GetTPUInfo: ACCISAMID =");
//		theRW_TRACE->WriteData(_T("<")+sLogString,Tpuinfo.ACCISAMID,sizeof(Tpuinfo.ACCISAMID));
	}
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡƱ��

@param      (i)DAT::TICKETINFO TicketInfo   ��Ʊ��Ϣ
@param      (o)DWORD& TicketPrice           Ʊ��

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::GetTicketPrice(DAT::TICKETINFO TicketInfo,DWORD& TicketPrice,int& DetailErrCode)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T(""));
	SYNC(RW,m_RWID.name);
	sLogString.Format(_T("GetTicketPrice: TicketInfo.CardType = %d ProductType = %d"),TicketInfo.CardType,TicketInfo.ProductType);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	sLogString = _T("");
	sLogString.Format(_T("GetTicketPrice: TicketInfo.ProductyCategory = 0x%04x ProductSubType = %d"),TicketInfo.ProductCategory,TicketInfo.ProductSubType);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	sLogString = _T("");
	sLogString.Format(_T("GetTicketPrice: TicketInfo.PriceType = %d PassengerType = %d Time ="),TicketInfo.PriceType,TicketInfo.PassengerType);
	theRW_TRACE->WriteData(_T(">")+sLogString,TicketInfo.Time,sizeof(TicketInfo.Time));

	sLogString = _T("");
	sLogString.Format(_T("GetTicketPrice: TicketInfo.BegainStation="));
	theRW_TRACE->WriteData(_T(">")+sLogString,TicketInfo.BegainStation,sizeof(TicketInfo.BegainStation));

	sLogString = _T("");
	sLogString.Format(_T("GetTicketPrice: TicketInfo.EndStation="));
	theRW_TRACE->WriteData(_T(">")+sLogString,TicketInfo.EndStation,sizeof(TicketInfo.EndStation));

	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))	{
		lCompleteCode = DAT::GetTicketPrice(TicketInfo,TicketPrice,m_RWID.handle);
	//}
	//else	{
	//	TicketPrice = 300;
	//}
	//
	sLogString = _T("");
	sLogString.Format(_T("GetTicketPrice: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK)
	{
		sLogString = _T("");
		sLogString.Format(_T("GetTicketPrice: TicketPrice = %d"),TicketPrice);
		theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	}
	else{
		if(IsDcr4000ReturnCode(lCompleteCode))
		{
			DetailErrCode = lCompleteCode;
		}
		else
		{	
			GetErrCode(DetailErrCode);
		}
		
	}
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ˰��

@param      (i)DAT::TAXINFO TicketInfo   ��Ʊ��Ϣ
@param      (o)DWORD& TicketTax           ˰��

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//long CRWCommands::GetTicketTax(DAT::TAXINFO TicketInfo,DWORD& TicketTax,int& DetailErrCode)
//{
//	long lCompleteCode = RW_RSP_OK;
//	CString sLogString(_T(""));
//	SYNC(RW,m_RWID.name);
//
//	sLogString.Format(_T("GetTicketTax: TicketInfo.CardType = %d "),TicketInfo.CardType,TicketInfo.SaleOrAddAmount);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))	{
//	lCompleteCode = DAT::GetTicketPriceTax(TicketInfo,TicketTax,m_RWID.handle);
//	//}
//	//else	{
//	//	TicketPrice = 300;
//	//}
//	//
//	sLogString = _T("");
//	sLogString.Format(_T("GetTicketTax: ReturnCode = 0x%02x"),lCompleteCode);
//	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//	if(lCompleteCode == RW_RSP_OK)
//	{
//		sLogString = _T("");
//		sLogString.Format(_T("GetTicketTax: TicketTax = %d"),TicketTax);
//		theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//	}
//	else{
//		if(IsDcr4000ReturnCode(lCompleteCode))
//		{
//			DetailErrCode = lCompleteCode;
//		}
//		else
//		{	
//			GetErrCode(DetailErrCode);
//		}
//
//	}
//	return lCompleteCode;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡTPU������Ϣ

@param      (o)WORD &ParamCount					��������
	if(theTVM_INFO.IsRWUsed())
	{
		@param      (o)DAT::PARAINFO *Paraminfo         �����ṹָ��
	}
	else
	{
	}

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::GetParameterInfo(WORD &ParamCount,DAT::PARAINFO *Paraminfo)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T("GetParaInfo"));
	SYNC(RW,m_RWID.name);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW"))){
		lCompleteCode = DAT::GetParaInfo(ParamCount,Paraminfo,m_RWID.handle);
	//}
	//else{
	//	VARIABLE_DATA data;
	//	//ReadSimDataFile(PARAM_DATA_FILE_NAME,data);
	//	memcpy(Paraminfo,data.lpData,data.nLen);
	//	ParamCount = data.nLen / sizeof(DAT::PARAINFO);

	//}
	sLogString = _T("");
	sLogString.Format(_T("GetParaInfo: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	if(lCompleteCode == RW_RSP_OK){		
		sLogString = _T("");
		sLogString.Format(_T("GetParaInfo: ParamCount = %d"),ParamCount);
		theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

		for(int i = 0; i< ParamCount;i++){
			sLogString = _T("GetParaInfo: Paraminfo->ParaID = ");
			theRW_TRACE->WriteData(_T("<")+sLogString,Paraminfo->ParaID,sizeof(Paraminfo->ParaID));
			sLogString = _T("GetParaInfo: Paraminfo->ParaVer = ");
			theRW_TRACE->WriteData(_T("<")+sLogString,Paraminfo->ParaVer,sizeof(Paraminfo->ParaVer));
			sLogString = _T("GetParaInfo: Paraminfo->TakeEffectTime = ");
			//theRW_TRACE->WriteData(_T("<")+sLogString,Paraminfo->TakeEffectTime,sizeof(Paraminfo->TakeEffectTime));
			sLogString = _T("GetParaInfo: Paraminfo->VerType = ");
			//theRW_TRACE->WriteData(_T("<")+sLogString,&Paraminfo->VerType,sizeof(Paraminfo->VerType));
			
			/*sLogString = _T("");
			sLogString.Format(_T("GetParaInfo: Paraminfo->VerType = %d"),Paraminfo->VerType);
			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);*/
			Paraminfo++;
		}
	}
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡTPU������Ϣ

@param      (i)uchar* ParaType				�����ļ�����,����Ϊ2�ֽ�
@param      (i)BYTE bContinue				��������־:0x00 ��ʾ������,0x01 ��ʾ�к�����
@param      (i)WORD PacketNumber			���ݰ����: ��Χ0x0000 ~ OxFFFF
@param      (i)WORD PacketLen				���ݰ���Ϣ����,���Ϊ1024,����1024�ֽ�ʱ�ְ�����,���һ�����ݰ�����ʵ�ʳ��ȷ���
@param      (i)BYTE* Data					������Ϣ�����ص��������ݣ����1024byte
@param      (i)WORD& ReceivePacketNumber	���ڴ�ŷ���Ӧ����ָʾӦ��ɹ��İ����

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::DownloadParamter(WORD ParaType,BYTE bContinue,WORD PacketNumber,WORD PacketLen,BYTE* Data,WORD& ReceivePacketNumber)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString = _T("Para_Downdload: ParaType =");
	SYNC(RW,m_RWID.name);
	WORD wParaType = mstos(ParaType);
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&wParaType,sizeof(wParaType));

	sLogString = _T("");
	sLogString.Format(_T("Para_Downdload: bContinue = %d PacketNumber = %d PacketLen = %d"),bContinue,PacketNumber,PacketLen);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	sLogString = _T("Para_Downdload: Data =");
	theRW_TRACE->WriteData(_T(">")+sLogString,Data,PacketLen);

	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW"))){
		lCompleteCode = DAT::Para_Downdload(ParaType,bContinue,PacketNumber,PacketLen,Data,ReceivePacketNumber,m_RWID.handle);
	//}
	//else{
	//}

	sLogString = _T("");
	sLogString.Format(_T("Para_Downdload: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK)
	{		
		sLogString = _T("");
		sLogString.Format(_T("Para_Downdload: ReceivePacketNumber = %d"),ReceivePacketNumber);
		theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	}
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡTPU������Ϣ

@param      (i)DAT::INITSTRUCT	initInfo	��ʼ����Ϣ
			typedef struct _INITSTRUCT {  
				ulong DelayTime;			// �Զ�ABORT��ʱʱ��,��λms,��ʾ��λ����������ָ��������X������û�м�⵽��Ƭ�����������Զ����ص�����״̬
				uchar DeviceCode[6];		// �豸ID
				uchar DeviceHdType;			// �豸Ӳ������ DeviceType_t+DeviceSubType_t
				uchar StationPLInfo[4];		// ��վλ����ϢStationID_t
				uchar AgentCode;			// ��Ӫ��˾����뷽 Ԥ��
				uchar OperationDate[4];		// ��Ӫ���� YYYYMMDD
				uchar CurrentDate[4];		// ��ǰ�������� YYYYMMDD
				uchar DeviceTestMode;		// �豸����ģʽ
				uchar StationOperationMode; // ��վ��Ӫģʽ �μ�ModeCode_t
				uchar OperaterID[4];		// ����Ա�ı��,�μ�OperateID_t
				uchar TransferStationFlag;  // ����վ��־ 0x00���������ϰ����˳�վ��0x01�������ϰ����˳�վ
				uchar ReaderPlace;			// ������λ��
				ushort ErrFareCtrMax;		// ��Ʊ���������������
				ushort WaitTimeForCardWR;	// δд����ȫ�Ŀ��ȴ�ʱ��
				ushort RetryTimesForCardWR;	// δд����ȫ�Ŀ����Դ���
				uchar  AntennaConfig;		// �������� 0x00�������ߣ�0x01��˫����������A����Ѱ����0x02��˫����������B����Ѱ��
				uchar Reserver[4];          // Ԥ��
			} INITSTRUCT; 
@param      (o)BYTE& status	 TPU״̬

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::InitRW(DAT::INITSTRUCT initInfo,BYTE& status)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T("Init_TPU: initInfo ="));
	SYNC(RW,m_RWID.name);

	sLogString.Format(_T("Init_TPU: initInfo.DelayTime = %d  DeviceTestMode = %d AntennaConfig = %d"),initInfo.DelayTime,initInfo.DeviceTestMode,initInfo.AntennaConfig);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	sLogString = _T("");
	sLogString.Format(_T("Init_TPU: initInfo.DeviceCode = "),initInfo.DeviceCode);
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)initInfo.DeviceCode,sizeof(initInfo.DeviceCode));

	sLogString = _T("");
	sLogString.Format(_T("Init_TPU: initInfo.DeviceHdType = "),initInfo.DeviceHdType);
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&(initInfo.DeviceHdType),sizeof(initInfo.DeviceHdType));

	sLogString = _T("");
	sLogString.Format(_T("Init_TPU: initInfo.StationPLInfo = "),initInfo.StationPLInfo);
	theRW_TRACE->WriteData(_T(">")+sLogString,initInfo.StationPLInfo,sizeof(initInfo.StationPLInfo));

	sLogString = _T("");
	sLogString.Format(_T("Init_TPU: initInfo.AgentCode = "),initInfo.AgentCode);
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&(initInfo.AgentCode),sizeof(initInfo.AgentCode));

	sLogString = _T("");
	sLogString.Format(_T("Init_TPU: initInfo.OperationDate = "),initInfo.OperationDate);
	theRW_TRACE->WriteData(_T(">")+sLogString,initInfo.OperationDate,sizeof(initInfo.OperationDate));

	sLogString = _T("");
	sLogString.Format(_T("Init_TPU: initInfo.CurrentDate = "),initInfo.CurrentDate);
	theRW_TRACE->WriteData(_T(">")+sLogString,initInfo.CurrentDate,sizeof(initInfo.CurrentDate));

	sLogString = _T("");
	sLogString.Format(_T("Init_TPU: initInfo.StationOperationMode = "),initInfo.StationOperationMode);
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&(initInfo.StationOperationMode),sizeof(initInfo.StationOperationMode));

	sLogString = _T("");
	sLogString.Format(_T("Init_TPU: initInfo.OperaterID = "),initInfo.OperaterID);
	theRW_TRACE->WriteData(_T(">")+sLogString,initInfo.OperaterID,sizeof(initInfo.OperaterID));

	sLogString = _T("");
	sLogString.Format(_T("Init_TPU: initInfo.ErrFareCtrMax = "),initInfo.ErrFareCtrMax);
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&initInfo.ErrFareCtrMax,sizeof(initInfo.ErrFareCtrMax));

	sLogString = _T("");
	sLogString.Format(_T("Init_TPU: initInfo.RetryTimesForCardWR ="),initInfo.RetryTimesForCardWR);
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&(initInfo.RetryTimesForCardWR),sizeof(initInfo.RetryTimesForCardWR));

	sLogString = _T("");
	sLogString.Format(_T("Init_TPU: initInfo.WaitTimeForCardWR = "),initInfo.WaitTimeForCardWR );
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&initInfo.WaitTimeForCardWR,sizeof(initInfo.WaitTimeForCardWR));

	//sLogString = _T("");
	//sLogString.Format(_T("Init_TPU: initInfo.ReaderPosition = "), initInfo.ReaderPosition);
	//theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&initInfo.ReaderPosition,sizeof(initInfo.ReaderPosition));

	//sLogString = _T("");
	//sLogString.Format(_T("Init_TPU: initInfo.ChangeStationSign = "),initInfo.ChangeStationSign );
	//theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&initInfo.ChangeStationSign,sizeof(initInfo.ChangeStationSign));


	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode  = DAT::Init_TPU(initInfo,status,m_RWID.handle);
	//}
	//else
	//{
	//	status = STATUE_IDLE;
	//	m_RWID.status = STATUE_IDLE;
	//}

	sLogString = _T("");
	sLogString.Format(_T("Init_TPU: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){		
		sLogString = _T("");
		sLogString.Format(_T("Init_TPU: status = %d"),status);
		theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	}
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ϣ����

@param      (i)DAT::pCardReadMisc_t  
				typedef struct CARDREADMISC{
				//CPU ��Ƭ��������
				uchar ValidCheckMark;//�Ϸ��Լ���־(1 bytes)
				uchar TransCode;//ҵ���ܴ���
				uchar AreaPaidMark;//��������־
				uchar TransTime[7];//��ǰʱ��
				uchar AntennaMark;//����Ѱ����־
				uchar ucReadFlag;//��Ʊǰ�������
				uchar ucStation[4];//��Ʊǰ��վվ��
				uchar ucGetTransLog;//��ʷ���׶�ȡ���
				}CardReadMisc_t, *pCardReadMisc_t;
@param      (o)DAT::CARDINFO&			RW����������Ϣ    
				typedef struct _RECHARGEPRODUCT
				{
				ushort ProductType;//��Ʒ����
				ushort ProductSubType;//��Ʒ������
				ulong RefoundMoney;//���˽��
				uchar RechargeMark;//�Ƿ�ɳ�ֵ
				}RECHARGEPRODUCT;

				typedef struct _CARDINFO
				{
				uchar CardPHType;//��Ʊ��������
				uchar CardInfoLen;//�����ݳ���
				uchar PayAreaReticketCode;//��������Ʊ�������
				uchar NoPayAreaReticketCode;//�Ƿ�������Ʊ�������
				ulong RepayTicketAmount;//���鲹Ʊ���
				//ѭ��
				uchar ProductCount;//��Ʒ����
				RECHARGEPRODUCT RechargeProduct[10];
				//ѭ������
				uchar SalableProductType;//���۲�Ʒ����
				uchar SalableProductCount;//���۲�Ʒ����
				uchar Reserve[20];//����
				ulong ReturnCardFee;//[4];������Ʊ�����ѽ��
				uchar isBigMoneyCard;//�Ƿ�Ϊ��
				uchar AntennaMark; // ���߱�־
				uchar CardInfo[512];//����Ϣ
				uchar ucLogNum;//ʷ�������ݼ�¼����
				uchar LogInfo[160];//ʷ��������
				}CARDINFO;
@param      (o)DWORD &DetailErrCode             TPU��ϸ�������

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::ReadCard(DAT::pCardReadMisc_t pCardReadMisc,DAT::CARDINFO &CardInfo,int &DetailErrCode)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T(""));
	SYNC(RW,m_RWID.name);

	sLogString.Format(_T("ReadCard: pCardReadMisc->ValidCheckMark =%d TransCode = %d AreaPaidMark = %d"),pCardReadMisc->ValidCheckMark,pCardReadMisc->TransCode,pCardReadMisc->AreaPaidMark);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	sLogString = _T("ReadCard: pCardReadMisc->TransTime =");
	theRW_TRACE->WriteData(_T(">")+sLogString,pCardReadMisc->TransTime,sizeof(pCardReadMisc->TransTime));

	sLogString = _T("");
	sLogString.Format(_T("ReadCard: pCardReadMisc->AntennaMark =%d ucReadFlag = %d ucGetTransLog = %d"),pCardReadMisc->AntennaMark,pCardReadMisc->ucReadFlag,pCardReadMisc->ucGetTransLog);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	sLogString = _T("ReadCard: pCardReadMisc->ucStation =");
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)pCardReadMisc->ucStation,sizeof(pCardReadMisc->ucStation));

	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::ReadCardInfo(pCardReadMisc,CardInfo,m_RWID.handle);
	//}
	//else
	//{

	//	VARIABLE_DATA srcbinInfo;
	//	if(theTVM_INFO.GetSimCardType(m_RWID.name==_T("RW")) == RW_SIM_M1_CARD_TYPE){
	//		//ReadSimDataFile(M1_CARD_FILE_NAME,srcbinInfo);			
	//	}
	//	else{
	//		//ReadSimDataFile(UL_CARD_FILE_NAME,srcbinInfo);				
	//	}
	//	memcpy(&CardInfo,srcbinInfo.lpData,sizeof(CardInfo));
	//}
	sLogString = _T("");
	sLogString.Format(_T("ReadCardInfo: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		DetailErrCode = 0;
		sLogString = _T("");
		sLogString.Format(_T("ReadCard: CardInfo.CardPHType =%d CardInfoLen = %d ucLogNum = %d NoPayAreaReticketCode = %d PayAreaReticketCode = %d"),CardInfo.CardPHType,CardInfo.CardInfoLen,CardInfo.ucLogNum,CardInfo.NoPayAreaReticketCode,CardInfo.PayAreaReticketCode);
		theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

		//sLogString = _T("");
		////sLogString.Format(_T("ReadCard: CardInfo.RepayTicketAmount =%d ReturnCardFee =%d SalableProductType =%d SalableProductCount =%d ProductCount =%d isBigMoneyCard = %d AntennaMark = %d"),CardInfo.RepayTicketAmount,CardInfo.ReturnCardFee,CardInfo.SalableProductType,CardInfo.SalableProductCount,CardInfo.ProductCount,CardInfo.isBigMoneyCard,CardInfo.AntennaMark);
		//theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
		
		/*if(CardInfo.ProductCount>0){
			BYTE * pProInfo = (LPBYTE)&CardInfo.RechargeProduct;
			for(int i = 0; i< CardInfo.ProductCount;i++){
				sLogString = _T("ReadCard: CardInfo->RechargeProductInfo = ");
				theRW_TRACE->WriteData(_T("<")+sLogString,pProInfo,ACC_RECHARGE_PRODUCT_LEN);
				pProInfo += ACC_RECHARGE_PRODUCT_LEN;
			}
		}*/

		sLogString = _T("ReadCard: CardInfo.CardInfo =");
		theRW_TRACE->WriteData(_T("<")+sLogString,CardInfo.CardInfo,CardInfo.CardInfoLen);

		if(CardInfo.ucLogNum>0){
			BYTE * pLogInfo = CardInfo.LogInfo;
			for(int i = 0; i< CardInfo.ucLogNum;i++){
				sLogString = _T("ReadCard: CardInfo->LogInfo = ");
				theRW_TRACE->WriteData(_T("<")+sLogString,pLogInfo,ACC_UL_TRANS_LOG_LEN);
				pLogInfo += ACC_UL_TRANS_LOG_LEN;
			}
		}
	}
	else{
		if(IsDcr4000ReturnCode(lCompleteCode))
		{
			DetailErrCode = lCompleteCode;
		}
		else
		{	
			GetErrCode(DetailErrCode);
		}
	}
    return  lCompleteCode;
 }

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������(Ӧ��������0xA1�ÿ��ں������У���δ������SLE��ͨ��������Ըÿ�������������)

@param      (i)DWORD &SerialNo				// UDSN��һ��ͨ������ˮ��
@param      (i)BYTE*TranDateTime			// ���׷���������ʱ�� 7BCD
@param      (o)uchar &SerAccnum			    // UDSN��һ��ͨ������ˮ���ۼ�ֵ
@param      (o)TRANRESULT &TranResult       // TPUҵ����󷵻صĽ������(ͬ��Ʊ)
@param      (o)ushort &UDDatalen			// UD���ݳ��� ����Ϊ0��ʾ�����UD���ݲ�����
@param      (o)uchar *UDData				// UD����(ͬ��Ʊ)			
@param      (o)ushort &ARDataLen			// AR���ݳ��� ����Ϊ0��ʾ�����AR���ݲ�����
@param      (o)ARDATA &ARData				// AR����(ͬ��Ʊ)

@retval     long    �ɹ�:0; ʧ��:����Ӧ����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::Block(DWORD &SerialNo,BYTE* TranDateTime, BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
						WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T(""));
	SYNC(RW,m_RWID.name);
	sLogString.Format(_T("CardBlock: SerialNo = %d TranDateTime = "),SerialNo);
	theRW_TRACE->WriteData(_T(">")+sLogString,TranDateTime,TRANS_TIME_LEN);
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::CardBlock(SerialNo,TranDateTime,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);
	//}
	//else
	//{
	//	SerAccnum = 1;
	//	memset(&TranResult,0,sizeof(TranResult));
	//	VARIABLE_DATA srcbinInfo;
	//	// UD
	//	if(theTVM_INFO.GetSimCardType(false) == RW_SIM_M1_CARD_TYPE){		
	//		//ReadSimDataFile(ACC_M1_BLOCK_UD_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	else{
	//		//ReadSimDataFile(ACC_BLOCK_UD_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	UDDatalen = srcbinInfo.nLen;
	//	memcpy(UDData,srcbinInfo.lpData,srcbinInfo.nLen);

	//	// AR
	//	if(theTVM_INFO.GetSimCardType(false) == RW_SIM_M1_CARD_TYPE){
	//		//ReadSimDataFile(ACC_M1_BLOCK_AR_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	else{
	//		//ReadSimDataFile(ACC_BLOCK_AR_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	ARDataLen = srcbinInfo.nLen;
	//	memcpy(&ARData,srcbinInfo.lpData,srcbinInfo.nLen);
	//}
	sLogString = _T("");
	sLogString.Format(_T("CardBlock: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		DetailErrCode = 0;
		sLogString = _T("");
		sLogString.Format(_T("CardBlock: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

		if(UDDatalen>0){
			sLogString = _T("CardBlock: UDData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
		}

		if(ARDataLen>0){
			sLogString = _T("");
			sLogString.Format(_T("CardBlock: ARData.ARnum = %d"),ARData.ARnum);
			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

			sLogString = _T("CardBlock: ARData.ARData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
		}

		sLogString = _T("");
		sLogString.Format(_T("CardBlock: TranResult ="));
		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
	}
	else{
		if(IsDcr4000ReturnCode(lCompleteCode))
		{
			DetailErrCode = lCompleteCode;
		}
		else
		{	
			GetErrCode(DetailErrCode);
		}
	}
	return  lCompleteCode;
}
  
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)DWORD &SerialNo				// UDSN��һ��ͨ������ˮ��
@param      (i)BYTE*TranDateTime			// ���׷���������ʱ�� 7BCD
@param      (i)BYTE CardType  		        // ��Ƭ���࣬0x01:һ��ͨS50/S70��Ƭ��0x02:һ��ͨCPU����0x11:һƱͨS50/S70��Ƭ��0x12:һƱͨUL��
@param      (i)WORD ProductyType			// ��Ʒ����
@param      (i)BYTE *ReasonCode				// ����ԭ��
@param      (o)uchar &SerAccnum			    // UDSN��һ��ͨ������ˮ���ۼ�ֵ
@param      (o)TRANRESULT &TranResult       // TPUҵ����󷵻صĽ������(ͬ��Ʊ)
@param      (o)ushort &UDDatalen			// UD���ݳ��� ����Ϊ0��ʾ�����UD���ݲ�����
@param      (o)uchar *UDData				// UD����(ͬ��Ʊ)			
@param      (o)ushort &ARDataLen			// AR���ݳ��� ����Ϊ0��ʾ�����AR���ݲ�����
@param      (o)ARDATA &ARData				// AR����(ͬ��Ʊ)

@retval     long    �ɹ�:0; ʧ��:����Ӧ����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//long CRWCommands::Counteract(DWORD &SerialNo,BYTE* TranDateTime, BYTE CardType,WORD ProductyType,BYTE* ReasonCode,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
//						     WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode)
//{
//	SYNC(RW,m_RWID.name);
//
//	CString sLogString(_T(""));
//	sLogString.Format(_T("ReverseTicket: SerialNo = %d TranDateTime = "),SerialNo);
//	theRW_TRACE->WriteData(_T(">")+sLogString,TranDateTime,TRANS_TIME_LEN);
//
//	sLogString.Format(_T("ReverseTicket: CardType = %d ProductyType = %d ReasonCode ="),CardType,ProductyType);
//	theRW_TRACE->WriteData(_T(">")+sLogString,ReasonCode,sizeof(WORD));
//	
//	long lCompleteCode = RW_RSP_OK;
//	if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
//	{
//		lCompleteCode = DAT::ReverseTicket (SerialNo,TranDateTime,CardType,ProductyType,ReasonCode,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);	
//	}
//	else
//	{
//		SerAccnum = 1;
//		memset(&TranResult,0,sizeof(TranResult));
//		VARIABLE_DATA srcbinInfo;
//		// UD		
//		//ReadSimDataFile(ACC_COUNTERACT_UD_DATA_FILE_NAME,srcbinInfo);
//		UDDatalen = srcbinInfo.nLen;
//		memcpy(UDData,srcbinInfo.lpData,srcbinInfo.nLen);
//
//		// AR
//		//ReadSimDataFile(ACC_COUNTERACT_AR_DATA_FILE_NAME,srcbinInfo);		
//		ARDataLen = srcbinInfo.nLen;
//		memcpy(&ARData,srcbinInfo.lpData,srcbinInfo.nLen);
//	}
//
//	sLogString = _T("");
//	sLogString.Format(_T("ReverseTicket: ReturnCode = 0x%02x"),lCompleteCode);
//	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//	if(lCompleteCode == RW_RSP_OK){
//		DetailErrCode = 0;
//		sLogString = _T("");
//		sLogString.Format(_T("ReverseTicket: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//		if(UDDatalen>0){
//			sLogString = _T("ReverseTicket: UDData =");
//			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
//		}
//
//		if(ARDataLen>0){
//			sLogString = _T("");
//			sLogString.Format(_T("ReverseTicket: ARData.ARnum = %d"),ARData.ARnum);
//			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//			sLogString = _T("ReverseTicket: ARData.ARData =");
//			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
//		}
//
//		sLogString = _T("");
//		sLogString.Format(_T("ReverseTicket: TranResult ="));
//		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
//	}
//	else{
//		if(IsDcr4000ReturnCode(lCompleteCode))
//		{
//			DetailErrCode = lCompleteCode;
//		}
//		else
//		{	
//			GetErrCode(DetailErrCode);
//		}
//	}
//	return  lCompleteCode;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)BYTE*TranDateTime			// ���׷���������ʱ�� 7BCD
@param      (i)BYTE CardType  		        // ��Ƭ���࣬0x01:һ��ͨS50/S70��Ƭ��0x02:һ��ͨCPU����0x11:һƱͨS50/S70��Ƭ��0x12:һƱͨUL��
@param      (i)WORD ProductyType			// ��Ʒ����
@param      (o)BYTE &Status					// ??

@retval     long    �ɹ�:0; ʧ��:����Ӧ����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::Active(DWORD &SerialNo,BYTE* TranDateTime,BYTE CardType,WORD ProductyType,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
						 WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode)
{
	SYNC(RW,m_RWID.name);

	CString sLogString(_T(""));
	sLogString.Format(_T("ActiveCard: CardType = %d ProductyType = %d TranDateTime = "),CardType,ProductyType);
	theRW_TRACE->WriteData(_T(">")+sLogString,TranDateTime,TRANS_TIME_LEN);

	long lCompleteCode = RW_RSP_OK;
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
		//lCompleteCode = DAT::ActiveCard(SerialNo,TranDateTime,CardType,ProductyType,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);
	//}
	//else
	//{
	//	//SerAccnum = 1;
	//	//memset(&TranResult,0,sizeof(TranResult));
	//	//VARIABLE_DATA srcbinInfo;
	//	//// UD	
	//	////ReadSimDataFile(ACC_ACTIVE_UD_DATA_FILE_NAME,srcbinInfo);
	//	//UDDatalen = srcbinInfo.nLen;
	//	//memcpy(UDData,srcbinInfo.lpData,srcbinInfo.nLen);

	//	//// AR
	//	////ReadSimDataFile(ACC_ACTIVE_AR_DATA_FILE_NAME,srcbinInfo);		
	//	//ARDataLen = srcbinInfo.nLen;
	//	//memcpy(&ARData,srcbinInfo.lpData,srcbinInfo.nLen);
	//}

	sLogString = _T("");
	sLogString.Format(_T("ActiveCard: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		DetailErrCode = 0;
		sLogString = _T("");
		sLogString.Format(_T("ActiveCard: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

		if(UDDatalen>0){
			sLogString = _T("ActiveCard: UDData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
		}

		if(ARDataLen>0){
			sLogString = _T("");
			sLogString.Format(_T("ActiveCard: ARData.ARnum = %d"),ARData.ARnum);
			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

			sLogString = _T("ActiveCard: ARData.ARData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
		}

		sLogString = _T("");
		sLogString.Format(_T("ActiveCard: TranResult ="));
		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
	}
	else{
		if(IsDcr4000ReturnCode(lCompleteCode))
		{
			DetailErrCode = lCompleteCode;
		}
		else
		{	
			GetErrCode(DetailErrCode);
		}
	}
	
	return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������¿���

@param      (i)DWORD		&SerialNo			// UDSN��һ��ͨ������ˮ��
@param      (i)BYTE*		TranDateTime		// ���׷���������ʱ�� 7BCD
@param      (o)BYTE			&SerAccnum			// UDSN��һ��ͨ������ˮ���ۼ�ֵ
@param      (o)TRANRESULT	&TranResult			// TPUҵ����󷵻صĽ������(ͬ��Ʊ)
@param      (o)WORD			&UDDatalen			// UD���ݳ��� ����Ϊ0��ʾ�����UD���ݲ�����
@param      (o)BYTE*		UDData				// UD����(ͬ��Ʊ)			
@param      (o)WORD			&ARDataLen			// AR���ݳ��� ����Ϊ0��ʾ�����AR���ݲ�����
@param      (o)ARDATA		&ARData				// AR����(ͬ��Ʊ)

@retval     long    �ɹ�:0; ʧ��:����Ӧ����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::ReplaceNewCard(DWORD &SerialNo,BYTE* TranDateTime,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
								 WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode)
{
	CString sLogString;
	SYNC(RW,m_RWID.name);
	sLogString.Format(_T("ReplaceNewCard: SerialNo = %d TranDateTime = "),SerialNo);
	theRW_TRACE->WriteData(_T(">")+sLogString,TranDateTime,TRANS_TIME_LEN);
	
	long lCompleteCode = RW_RSP_OK;
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
		lCompleteCode = DAT::ReplaceNewCard(SerialNo,TranDateTime,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);
	//}
	//else
	//{
	//	SerAccnum = 1;
	//	memset(&TranResult,0,sizeof(TranResult));
	//	VARIABLE_DATA srcbinInfo;
	//	// UD
	//	if(theTVM_INFO.GetSimCardType(false) == RW_SIM_M1_CARD_TYPE){		
	//		//ReadSimDataFile(ACC_M1_BLOCK_UD_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	else{
	//		//ReadSimDataFile(ACC_BLOCK_UD_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	UDDatalen = srcbinInfo.nLen;
	//	memcpy(UDData,srcbinInfo.lpData,srcbinInfo.nLen);

	//	// AR
	//	if(theTVM_INFO.GetSimCardType(false) == RW_SIM_M1_CARD_TYPE){
	//		//ReadSimDataFile(ACC_M1_BLOCK_AR_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	else{
	//		//ReadSimDataFile(ACC_BLOCK_AR_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	ARDataLen = srcbinInfo.nLen;
	//	memcpy(&ARData,srcbinInfo.lpData,srcbinInfo.nLen);
	//}

	sLogString = _T("");
	sLogString.Format(_T("CardBlock: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		DetailErrCode = 0;

		sLogString = _T("");
		sLogString.Format(_T("ReplaceNewCard: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

		if(UDDatalen>0){
			sLogString = _T("ReplaceNewCard: UDData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
		}

		if(ARDataLen>0){
			sLogString = _T("");
			sLogString.Format(_T("ReplaceNewCard: ARData.ARnum = %d"),ARData.ARnum);
			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

			sLogString = _T("ReplaceNewCard: ARData.ARData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
		}

		sLogString = _T("");
		sLogString.Format(_T("ReplaceNewCard: TranResult ="));
		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
	}
	else{
		if(IsDcr4000ReturnCode(lCompleteCode))
		{
			DetailErrCode = lCompleteCode;
		}
		else
		{	
			GetErrCode(DetailErrCode);
		}
	}
	return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������ɿ���

@param      (i)DWORD		&SerialNo			// UDSN��һ��ͨ������ˮ��
@param      (i)BYTE*		TranDateTime		// ���׷���������ʱ�� 7BCD
@param      (o)BYTE			&SerAccnum			// UDSN��һ��ͨ������ˮ���ۼ�ֵ
@param      (o)TRANRESULT	&TranResult			// TPUҵ����󷵻صĽ������(ͬ��Ʊ)
@param      (o)WORD			&UDDatalen			// UD���ݳ��� ����Ϊ0��ʾ�����UD���ݲ�����
@param      (o)BYTE*		UDData				// UD����(ͬ��Ʊ)			
@param      (o)WORD			&ARDataLen			// AR���ݳ��� ����Ϊ0��ʾ�����AR���ݲ�����
@param      (o)ARDATA		&ARData				// AR����(ͬ��Ʊ)

@retval     long    �ɹ�:0; ʧ��:����Ӧ����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::ReplaceOldCard(DWORD &SerialNo,BYTE* TranDateTime,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
								 WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode)
{
	CString sLogString;
	SYNC(RW,m_RWID.name);
	sLogString.Format(_T("ReplaceOldCard: SerialNo = %d TranDateTime = "),SerialNo);
	theRW_TRACE->WriteData(_T(">")+sLogString,TranDateTime,TRANS_TIME_LEN);

	long lCompleteCode = RW_RSP_OK;
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
		lCompleteCode = DAT::ReplaceOldCard(SerialNo,TranDateTime,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);
	//}
	//else
	//{
	//	SerAccnum = 1;
	//	memset(&TranResult,0,sizeof(TranResult));
	//	VARIABLE_DATA srcbinInfo;
	//	// UD
	//	if(theTVM_INFO.GetSimCardType(false) == RW_SIM_M1_CARD_TYPE){		
	//		//ReadSimDataFile(ACC_M1_BLOCK_UD_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	else{
	//		//ReadSimDataFile(ACC_BLOCK_UD_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	UDDatalen = srcbinInfo.nLen;
	//	memcpy(UDData,srcbinInfo.lpData,srcbinInfo.nLen);

	//	// AR
	//	if(theTVM_INFO.GetSimCardType(false) == RW_SIM_M1_CARD_TYPE){
	//		//ReadSimDataFile(ACC_M1_BLOCK_AR_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	else{
	//		//ReadSimDataFile(ACC_BLOCK_AR_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	ARDataLen = srcbinInfo.nLen;
	//	memcpy(&ARData,srcbinInfo.lpData,srcbinInfo.nLen);
	//}

	sLogString = _T("");
	sLogString.Format(_T("CardBlock: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		DetailErrCode = 0;

		sLogString = _T("");
		sLogString.Format(_T("ReplaceOldCard: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

		if(UDDatalen>0){
			sLogString = _T("ReplaceOldCard: UDData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
		}

		if(ARDataLen>0){
			sLogString = _T("");
			sLogString.Format(_T("ReplaceOldCard: ARData.ARnum = %d"),ARData.ARnum);
			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

			sLogString = _T("ReplaceOldCard: ARData.ARData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
		}

		sLogString = _T("");
		sLogString.Format(_T("ReplaceOldCard: TranResult ="));
		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
	}
	else{
		if(IsDcr4000ReturnCode(lCompleteCode))
		{
			DetailErrCode = lCompleteCode;
		}
		else
		{	
			GetErrCode(DetailErrCode);
		}
	}
	return  lCompleteCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������������޿���

@param      (i)DWORD		&SerialNo			// UDSN��һ��ͨ������ˮ��
@param      (i)BYTE*		TranDateTime		// ���׷���������ʱ�� 7BCD
@param      (o)BYTE			&SerAccnum			// UDSN��һ��ͨ������ˮ���ۼ�ֵ
@param      (o)TRANRESULT	&TranResult			// TPUҵ����󷵻صĽ������(ͬ��Ʊ)
@param      (o)WORD			&UDDatalen			// UD���ݳ��� ����Ϊ0��ʾ�����UD���ݲ�����
@param      (o)BYTE*		UDData				// UD����(ͬ��Ʊ)			
@param      (o)WORD			&ARDataLen			// AR���ݳ��� ����Ϊ0��ʾ�����AR���ݲ�����
@param      (o)ARDATA		&ARData				// AR����(ͬ��Ʊ)

@retval     long    �ɹ�:0; ʧ��:����Ӧ����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//long CRWCommands::ReplaceNoCard(DWORD &SerialNo,BYTE* TranDateTime,DAT::STRUCT_NocardOrBadcardTransData transInfo,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
//	WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode)
//{
//	CString sLogString;
//	SYNC(RW,m_RWID.name);
//	sLogString.Format(_T("ReplaceNoCard: SerialNo = %d TranDateTime = "),SerialNo);
//	theRW_TRACE->WriteData(_T(">")+sLogString,TranDateTime,TRANS_TIME_LEN);
//
//	long lCompleteCode = RW_RSP_OK;
//	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
//	//{
//	lCompleteCode = DAT::NocardOrBadcardReplaceNewCard(SerialNo,TranDateTime,transInfo,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);
//	//}
//	//else
//	//{
//	//	SerAccnum = 1;
//	//	memset(&TranResult,0,sizeof(TranResult));
//	//	VARIABLE_DATA srcbinInfo;
//	//	// UD
//	//	if(theTVM_INFO.GetSimCardType(false) == RW_SIM_M1_CARD_TYPE){		
//	//		//ReadSimDataFile(ACC_M1_BLOCK_UD_DATA_FILE_NAME,srcbinInfo);
//	//	}
//	//	else{
//	//		//ReadSimDataFile(ACC_BLOCK_UD_DATA_FILE_NAME,srcbinInfo);
//	//	}
//	//	UDDatalen = srcbinInfo.nLen;
//	//	memcpy(UDData,srcbinInfo.lpData,srcbinInfo.nLen);
//
//	//	// AR
//	//	if(theTVM_INFO.GetSimCardType(false) == RW_SIM_M1_CARD_TYPE){
//	//		//ReadSimDataFile(ACC_M1_BLOCK_AR_DATA_FILE_NAME,srcbinInfo);
//	//	}
//	//	else{
//	//		//ReadSimDataFile(ACC_BLOCK_AR_DATA_FILE_NAME,srcbinInfo);
//	//	}
//	//	ARDataLen = srcbinInfo.nLen;
//	//	memcpy(&ARData,srcbinInfo.lpData,srcbinInfo.nLen);
//	//}
//
//	sLogString = _T("");
//	sLogString.Format(_T("CardBlock: ReturnCode = 0x%02x"),lCompleteCode);
//	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//	if(lCompleteCode == RW_RSP_OK){
//		DetailErrCode = 0;
//
//		sLogString = _T("");
//		sLogString.Format(_T("ReplaceNoCard: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//		if(UDDatalen>0){
//			sLogString = _T("ReplaceNoCard: UDData =");
//			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
//		}
//
//		if(ARDataLen>0){
//			sLogString = _T("");
//			sLogString.Format(_T("ReplaceNoCard: ARData.ARnum = %d"),ARData.ARnum);
//			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//			sLogString = _T("ReplaceNoCard: ARData.ARData =");
//			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
//		}
//
//		sLogString = _T("");
//		sLogString.Format(_T("ReplaceNoCard: TranResult ="));
//		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
//	}
//	else{
//		if(IsDcr4000ReturnCode(lCompleteCode))
//		{
//			DetailErrCode = lCompleteCode;
//		}
//		else
//		{	
//			GetErrCode(DetailErrCode);
//		}
//	}
//	return  lCompleteCode;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ֵ����

@param      (i)DWORD &SerialNo				// UDSN��һ��ͨ������ˮ��
@param      (i)BYTE*TranDateTime			// ���׷���������ʱ�� 7BCD	
@param      (i)char* szLocalID				// �ն˻����
@param      (i)REVALUEINFO ChargeInfo
			typedef struct _REVALUEINFO//��ֵ
			{	
			uchar PayMethed;//���ʽ
			uchar BankCardNo[10];
			uchar RevalueType;//��ֵ���͡���Ʊ������
								0x01 = һƱͨǮ����Ʒ��
								0x02 = һƱͨ�ƴβ�Ʒ��
								0x03 = һƱͨ���ڲ�Ʒ��
								0x04 = һ������ͨ����Ǯ����Ʒ��
								0x05 = ����һ��ͨСǮ����
								0x06 = ����һ��ͨѧ��Ǯ����
								0x07 = ����һ��ͨ���ڲ�Ʒ��
			ushort CardType;  // �Ӳ�Ʒ����
			ulong Amount;     // ��ֵ�ľ��������ʱת����INTEL��
			ulong Count;      // �ƴο���ֵ�Ĵ���������ʱת����INTEL��
			ulong Period;     // ���ڿ���Ч���ӳ����ȣ���λ����(�˲�����ʱ��Ч��TPU�Լ�����)
			} REVALUEINFO;
@param      (o)uchar &SerAccnum			    // UDSN��һ��ͨ������ˮ���ۼ�ֵ
@param      (o)TRANRESULT &TranResult       // TPUҵ����󷵻صĽ������(ͬ��Ʊ)
@param      (o)ushort &UDDatalen			// UD���ݳ��� ����Ϊ0��ʾ�����UD���ݲ�����
@param      (o)uchar *UDData				// UD����(ͬ��Ʊ)			
@param      (o)ushort &ARDataLen			// AR���ݳ��� ����Ϊ0��ʾ�����AR���ݲ�����
@param      (o)ARDATA &ARData				// AR����(ͬ��Ʊ)

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::Charge(DWORD &SerialNo,BYTE* TranDateTime,char* szLocalID, DAT::REVALUEINFO ChargeInfo,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
						 WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode)
{
	SYNC(RW,m_RWID.name);

	CString sLogString(_T(""));
	sLogString.Format(_T("Revalue: SerialNo = %d TranDateTime = "),SerialNo);
	theRW_TRACE->WriteData(_T(">")+sLogString,TranDateTime,TRANS_TIME_LEN);

	//sLogString = _T("");
	////sLogString.Format(_T("Revalue: ChargeInfo.SubProductType = %d  PayMethed = %d BankCardNo = %s "),ChargeInfo.AMT_YPT_Type ,ChargeInfo.PayMethed,ChargeInfo.BankCardNo );
	//theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);


	//sLogString = _T("");
	////sLogString.Format(_T("Revalue: ChargeInfo.Period = %d RevalueType = %d "),ChargeInfo.Period,ChargeInfo.RevalueType);
	//theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	sLogString = _T("");
	sLogString.Format(_T("Revalue: ChargeInfo.Chargemoney = %d RevalueType = %d"),ChargeInfo.Amount,ChargeInfo.RevalueType);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	long lCompleteCode = RW_RSP_OK;
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::Revalue(SerialNo,TranDateTime, ChargeInfo,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);
	//}
	//else
	//{
	//	SerAccnum = 1;
	//	memset(&TranResult,0,sizeof(TranResult));
	//	VARIABLE_DATA srcbinInfo;
	//	// UD		
	//	//ReadSimDataFile(ACC_M1_CHARGE_UD_DATA_FILE_NAME,srcbinInfo);
	//	UDDatalen = srcbinInfo.nLen;
	//	memcpy(UDData,srcbinInfo.lpData,srcbinInfo.nLen);

	//	// AR
	//	//ReadSimDataFile(ACC_M1_CHARGE_AR_DATA_FILE_NAME,srcbinInfo);		
	//	ARDataLen = srcbinInfo.nLen;
	//	memcpy(&ARData,srcbinInfo.lpData,srcbinInfo.nLen);

	//}
	
	sLogString = _T("");
	sLogString.Format(_T("Revalue: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		DetailErrCode = 0;
		sLogString = _T("");
		sLogString.Format(_T("Revalue: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

		if(UDDatalen>0){
			sLogString = _T("Revalue: UDData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
		}

		if(ARDataLen>0){
			sLogString = _T("");
			sLogString.Format(_T("Revalue: ARData.ARnum = %d"),ARData.ARnum);
			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

			sLogString = _T("Revalue: ARData.ARData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
		}

		sLogString = _T("");
		sLogString.Format(_T("Revalue: TranResult ="));
		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
	}
	else{
		if(IsDcr4000ReturnCode(lCompleteCode))
		{
			DetailErrCode = lCompleteCode;
		}
		else
		{	
			GetErrCode(DetailErrCode);
		}
	}
	
	return  lCompleteCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ֵ��ʱ��������
@param      (i)DWORD SerialNo,			UDSN��һ��ͨ������ˮ��
@param      (i)byte *pTime,				���׷���������ʱ��
@param      (i)byte byDo,				�Ƿ�ʱ���г�ʱ����
@param      (i)HANDLE m_RWID.handle,			��ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
@param      (o)uchar &bySerAccnum ,		UDSN��һ��ͨ������ˮ���ۼ�ֵ
@param      (o)TRANRESULT &TranResult   TPUҵ����󷵻صĽ������
typedef struct __TRANRESULT
{
uchar RetractFlag;			// Ʊ������״̬	0x00:������;0x01:��������;0x02:��Ʊ����;0x03:�ﵽ������������;0x04:���λ���
uchar HighBlacklist;        // �߼���������־(Ԥ��) 0x00:��;0x01:��
uchar CardType;				// ��Ƭ���࣬0x01:һ��ͨS50/S70��;0x02:һ��ͨCPU��;0x11:һƱͨS50/S70��;0x12:һƱͨUL��
ushort ProductyCategory;	// ��Ʊ��Ʒ���� TicketFamily_t + ProductType
uchar CardNo[4];			// һ��ͨ��������ˮ�Ż�һƱͨ�߼�����
ulong BeforAmount;			// ����ǰ���,�ƴο���Σ����ڿ����ֶ���Ч��
ulong AfterAmount;			// ���������,�ƴο���Σ����ڿ����ֶ���Ч��
uchar ucBlockFlag;			// Ʊ������״̬	0x00:Ʊ��δ����;0x01:Ʊ��������
uchar Reserved[14];			// Ԥ��
}TRANRESULT;
@param      (o)ushort &sArgUdLen        �������UD�����ڴ�ŷ���UD���ݵĳ���
@param      (o)uchar *pArgUdBuf			�������UD�����ڴ�ŷ��ص�UD���ݣ������UD���أ�Ϊ��
@param      (o)ushort &sArgArLen		������أ����ڴ��AR���ݳ���
@param      (o)ARDATA &sttArgAr			���ڴ�ŷ��ص�AR���ݣ������AR���أ�Ϊ��

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::ChargeOverTime(DWORD SerialNo,byte *pTime,byte byDo,uchar &bySerAccnum,DAT::TRANRESULT &sttTranRlt,ushort &sArgUdLen,uchar *pArgUdBuf,ushort &sArgArLen,DAT::ARDATA &sttArgAr,int &DetailErrCode)
{
	SYNC(RW,m_RWID.name);

	CString sLogString(_T(""));
	sLogString.Format(_T("ChargeOverTime: SerialNo = %d TranDateTime = "),SerialNo);
	theRW_TRACE->WriteData(_T(">")+sLogString,pTime,TRANS_TIME_LEN);


	long lCompleteCode = RW_RSP_OK;
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::SetTimeOutDeal(SerialNo,pTime,byDo,m_RWID.handle,bySerAccnum,sttTranRlt,sArgUdLen,pArgUdBuf,sArgArLen,sttArgAr);
	//}
	//else
	//{
	//	bySerAccnum = 1;
	//	memset(&sttTranRlt,0,sizeof(sttTranRlt));
	//	VARIABLE_DATA srcbinInfo;
	//	// UD		
	//	//ReadSimDataFile(ACC_M1_CHARGE_UD_DATA_FILE_NAME,srcbinInfo);
	//	sArgUdLen = srcbinInfo.nLen;
	//	memcpy(pArgUdBuf,srcbinInfo.lpData,srcbinInfo.nLen);

	//	// AR
	//	//ReadSimDataFile(ACC_M1_CHARGE_AR_DATA_FILE_NAME,srcbinInfo);		
	//	sArgArLen = srcbinInfo.nLen;
	//	memcpy(&sttArgAr,srcbinInfo.lpData,srcbinInfo.nLen);
	//}
	sLogString = _T("");
	sLogString.Format(_T("ChargeOverTime: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		DetailErrCode = 0;
		sLogString = _T("");
		sLogString.Format(_T("Revalue: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),bySerAccnum,sArgUdLen,sArgArLen);
		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

		if(sArgUdLen>0){
			sLogString = _T("Revalue: UDData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,pArgUdBuf,sArgUdLen);
		}

		if(sArgArLen>0){
			sLogString = _T("");
			sLogString.Format(_T("Revalue: ARData.ARnum = %d"),sttArgAr.ARnum);
			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

			sLogString = _T("Revalue: ARData.ARData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,sttArgAr.ARdata,sttArgAr.ARnum*ONE_ARDATA_LEN);
		}

		sLogString = _T("");
		sLogString.Format(_T("Revalue: TranResult ="));
		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&sttTranRlt,sizeof(sttTranRlt));
	}
	else{
		if(IsDcr4000ReturnCode(lCompleteCode))
		{
			DetailErrCode = lCompleteCode;
		}
		else
		{	
			GetErrCode(DetailErrCode);
		}
	}

	return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ʊ����

@param      (i)DWORD &SerialNo				// UDSN��һ��ͨ������ˮ��
@param      (i)BYTE*TranDateTime			// ���׷���������ʱ�� 7BCD	
@param      (i)COMPENSATIONPAID AdjustInfo
			typedef struct  _COMPENSATIONPAID{//��Ʊ
			                uchar CardTrantype;0x01��//����ͨM1��0x02������ͨCPU��Ƭ����0x11��һƱͨM1��Ƭ��0x12��һƱͨUL��
							ushort ProductyCategory;//��Ʊ��Ʒ���� ProductyType;
							uchar PayMethed; 0x01:  //�ǿ��ڿۿƱ��0x02: ���ڿۿƱ�� 
							ulong TicketAmount;     //��Ʊ���
							uchar PayAttribute;     //��Ʊ�������ԣ�������Ʊ�������������ԣ�0x01 = ��������Ʊ��0x02 = �Ǹ�������Ʊ
							uchar OperationType;    //0x01 =����վѡ��δˢ��վ����0x02 =�����ѡ������,����:��ʱ,����,��Ʊ��ʱ.����0x03 =ѡ���յ�վ�����������ѽ�δˢ��վ��
							uchar BegainStation[4]; //����վ��Ʊʱ�Ľ�վ��վλ����Ϣ������ʱת����INTEL��
							uchar EndStation[4];    //����վ��Ʊʱ�ĳ�վ��վλ����Ϣ.����ʱת����INTEL��
							uchar byPayType;//֧����ʽ
							uchar BankCardNo[10];//���п���
							uchar PayCardLogicID[8];
			} COMPENSATIONPAID;
@param      (o)uchar &SerAccnum			    // UDSN��һ��ͨ������ˮ���ۼ�ֵ
@param      (o)TRANRESULT &TranResult       // TPUҵ����󷵻صĽ������(ͬ��Ʊ)
@param      (o)ushort &UDDatalen			// UD���ݳ��� ����Ϊ0��ʾ�����UD���ݲ�����
@param      (o)uchar *UDData				// UD����(ͬ��Ʊ)			
@param      (o)ushort &ARDataLen			// AR���ݳ��� ����Ϊ0��ʾ�����AR���ݲ�����
@param      (o)ARDATA &ARData				// AR����(ͬ��Ʊ)

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::Adjust(DWORD &SerialNo,BYTE* TranDateTime, DAT::COMPENSATIONPAID AdjustInfo,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
						 WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode)
{
	SYNC(RW,m_RWID.name);

	CString sLogString(_T(""));
	sLogString.Format(_T("CompensationPaid: SerialNo = %d TranDateTime = "),SerialNo);
	theRW_TRACE->WriteData(_T(">")+sLogString,TranDateTime,TRANS_TIME_LEN);

	sLogString = _T("");
	sLogString.Format(_T("CompensationPaid: AdjustInfo.CardTrantype = %d ProductyType = %d PayMethed = %d"),AdjustInfo.CardTrantype,AdjustInfo.ProductyCategory,AdjustInfo.PayMethed);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	sLogString = _T("");
	sLogString.Format(_T("CompensationPaid: AdjustInfo.TicketAmount = %d PayAttribute = %d OperationType = %d"),AdjustInfo.TicketAmount,AdjustInfo.PayAttribute,AdjustInfo.OperationType);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	sLogString = _T("CompensationPaid: AdjustInfo.BegainStation = ");
	theRW_TRACE->WriteData(_T(">")+sLogString,AdjustInfo.BegainStation,sizeof(AdjustInfo.BegainStation));

	sLogString = _T("CompensationPaid: AdjustInfo.EndStation = ");
	theRW_TRACE->WriteData(_T(">")+sLogString,AdjustInfo.EndStation,sizeof(AdjustInfo.EndStation));

	sLogString = _T("");
	//sLogString.Format(_T("CompensationPaid: AdjustInfo.PayMethed = %d AdjustInfo.byPayType = %d BankCardNo = %s "),AdjustInfo.PayMethed,AdjustInfo.byPayType, AdjustInfo.BankCardNo);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);


	long lCompleteCode = RW_RSP_OK;
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::CompensationPaid(SerialNo,TranDateTime,AdjustInfo,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);
	//}
	//else
	//{
	//	SerAccnum = 1;
	//	memset(&TranResult,0,sizeof(TranResult));
	//	VARIABLE_DATA srcbinInfo;
	//	// UD		
	//	if(theTVM_INFO.GetSimCardType() == RW_SIM_M1_CARD_TYPE){
	//		//ReadSimDataFile(ACC_M1_ADJUST_UD_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	else{
	//		//ReadSimDataFile(ACC_ADJUST_UD_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	UDDatalen = srcbinInfo.nLen;
	//	memcpy(UDData,srcbinInfo.lpData,srcbinInfo.nLen);

	//	// AR
	//	if(theTVM_INFO.GetSimCardType() == RW_SIM_M1_CARD_TYPE){
	//		//ReadSimDataFile(ACC_M1_ADJUST_AR_DATA_FILE_NAME,srcbinInfo);	
	//	}
	//	else{
	//		//ReadSimDataFile(ACC_ADJUST_AR_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	ARDataLen = srcbinInfo.nLen;
	//	memcpy(&ARData,srcbinInfo.lpData,srcbinInfo.nLen);
	//}
	sLogString = _T("");
	sLogString.Format(_T("CompensationPaid: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		DetailErrCode = 0;
		sLogString = _T("");
		sLogString.Format(_T("CompensationPaid: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

		if(UDDatalen>0){
			sLogString = _T("CompensationPaid: UDData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
		}

		if(ARDataLen>0){
			sLogString = _T("");
			sLogString.Format(_T("CompensationPaid: ARData.ARnum = %d"),ARData.ARnum);
			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

			sLogString = _T("CompensationPaid: ARData.ARData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
		}

		sLogString = _T("");
		sLogString.Format(_T("CompensationPaid: TranResult ="));
		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
	}
	else{
		if(IsDcr4000ReturnCode(lCompleteCode))
		{
			DetailErrCode = lCompleteCode;
		}
		else
		{	
			GetErrCode(DetailErrCode);
		}
	}
	
	return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ۿ�����

@param      (i)DWORD &SerialNo			// UDSN��һ��ͨ������ˮ��
@param      (i)BYTE*TranDateTime		// ���׷���������ʱ�� 7BCD	
@param      (i)SALEINFO SaleInfo,
			typedef struct _SALECARD
			{
			uchar CardApplicationType;//��Ƭ����0x01������ͨM1��0x02������ͨCPU��Ƭ����	0x11��һƱͨM1��Ƭ��0x12��һƱͨUL��
			uchar ProductyType;//��Ʊ��Ʒ���� Ǯ�������ڡ��ƴ�
			ushort ProductyCategory;//��Ʊ��Ʒ���� Ʊ�ֶ��塣����ʱת����INTEL��
			ushort ProductSubType;//��Ʊ��Ʒ������
			uchar SaleMode;//��Ʊ��ʽ0x01����ʾ����Ʊ������Ʊ��		0x02����ʾ������ֹվ����Ʊ
			uchar PassengerType;//�˿�����
			ulong TicketAmount;//��Ʊ���
			uchar BegainStation[4];//���վ�������Σ�վ��
			uchar EndStation[4];//�յ�վ�������Σ�վ��
			ushort CountTicketUseCount;//�ƴ��೵Ʊʹ�ô���
			ulong ValidityPeriod;//��Ч�ڵ�ʱ�䳤��
			uchar byPayType;//֧����ʽ
			uchar BankCardNo[10];//���п���
			uchar PayCardSerialNumber[8];//֧��Ʊ���߼�����
			}SALEINFO;
@param      (o)uchar &SerAccnum			    // UDSN��һ��ͨ������ˮ���ۼ�ֵ
@param      (o)TRANRESULT &TranResult       // TPUҵ����󷵻صĽ������
			typedef struct __TRANRESULT
			{
				uchar RetractFlag;			// Ʊ������״̬	0x00:������;0x01:��������;0x02:��Ʊ����;0x03:�ﵽ������������;0x04:���λ���
				uchar HighBlacklist;        // �߼���������־(Ԥ��) 0x00:��;0x01:��
				uchar CardType;				// ��Ƭ���࣬0x01:һ��ͨS50/S70��;0x02:һ��ͨCPU��;0x11:һƱͨS50/S70��;0x12:һƱͨUL��
				ushort ProductyCategory;	// ��Ʊ��Ʒ���� TicketFamily_t + ProductType
				uchar CardNo[4];			// һ��ͨ��������ˮ�Ż�һƱͨ�߼�����
				ulong BeforAmount;			// ����ǰ���,�ƴο���Σ����ڿ����ֶ���Ч��
				ulong AfterAmount;			// ���������,�ƴο���Σ����ڿ����ֶ���Ч��
				uchar ucBlockFlag;			// Ʊ������״̬	0x00:Ʊ��δ����;0x01:Ʊ��������
				uchar Reserved[14];			// Ԥ��
			}TRANRESULT;
@param      (o)ushort &UDDatalen			// UD���ݳ��� ����Ϊ0��ʾ�����UD���ݲ�����
@param      (o)uchar *UDData				// UD����
			1	UD���ݼ�¼����	1	HEX	
			2	UD���ݼ�¼��	
				UD���ݼ�¼��Ľṹ���£�
				���	�ֶ�	      �ֽ���	����	��ע
				2.1	  �����������	   1	    HEX	    0x01:һ��ͨ���� 0x02:һƱͨ����
				2.2	  �������ݳ���	   2	    HEX	
				2.3	  ��������			
			
@param      (o)ushort &ARDataLen			// AR���ݳ��� ����Ϊ0��ʾ�����AR���ݲ�����
@param      (o)ARDATA &ARData				// AR����
			typedef struct __ARDATA 
			{
				uchar ARnum;				// 1.AR���ݼ�¼����
				uchar ARdata[BUF_SIZE];		// 2.AR���ݼ�¼��
				���	�ֶ�	  �ֽ���	����	��ע
				2.1	 �豸�Ĵ���ID	2	    HEX	   ��TPU��ص��豸�Ĵ���ID
				2.2	 AR����������	4	    HEX	   SLE����AR�����������ۼ��豸�ڱ����AR����
			}ARDATA;
				
@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::Issue(DWORD &SerialNo,BYTE* TranDateTime,DAT::SALEINFO IssueInfo,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
						WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T(""));
	SYNC(RW,m_RWID.name);

	sLogString.Format(_T("SaleCard: SerialNo = %d TranDateTime = "), SerialNo);
	theRW_TRACE->WriteData(_T(">")+sLogString,TranDateTime,TRANS_TIME_LEN);

	sLogString = _T("");
	sLogString.Format(_T("SaleCard: ProductyCategory= %d ProductyType = %d ProductSubType = %d"),IssueInfo.ProductyCategory,IssueInfo.ProductyType,IssueInfo.ProductSubType);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	sLogString = _T("");
	sLogString.Format(_T("SaleCard: IssueInfo.SaleMode = %d PassengerType = %d TicketAmount = %d"),IssueInfo.SaleMode, IssueInfo.PassengerType, IssueInfo.TicketAmount);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	sLogString.Format(_T("SaleCard: byPayType = %d"), IssueInfo.byPayType);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	sLogString = _T("SaleCard: IssueInfo.BegainStation = ");
	theRW_TRACE->WriteData(_T(">")+sLogString, (LPBYTE)&IssueInfo.BegainStation,sizeof(IssueInfo.BegainStation));

	sLogString = _T("SaleCard: IssueInfo.EndStation = ");
	theRW_TRACE->WriteData(_T(">")+sLogString, (LPBYTE)&IssueInfo.EndStation,sizeof(IssueInfo.EndStation));


	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::SaleCard(SerialNo,TranDateTime,IssueInfo,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);
	//}
	//else
	//{
	//	SerAccnum = 1;
	//	memset(&TranResult,0,sizeof(TranResult));
	//	VARIABLE_DATA srcbinInfo;
	//	// UD		
	//	if(theTVM_INFO.GetSimCardType(false) == RW_SIM_M1_CARD_TYPE){
	//		//ReadSimDataFile(ACC_M1_ISSUE_UD_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	else{
	//		//ReadSimDataFile(ACC_ISSUE_UD_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	UDDatalen = srcbinInfo.nLen;
	//	memcpy(UDData,srcbinInfo.lpData,srcbinInfo.nLen);

	//	// AR
	//	if(theTVM_INFO.GetSimCardType(false) == RW_SIM_M1_CARD_TYPE){
	//		//ReadSimDataFile(ACC_M1_ISSUE_AR_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	else{
	//		//ReadSimDataFile(ACC_ISSUE_AR_DATA_FILE_NAME,srcbinInfo);
	//	}
	//	ARDataLen = srcbinInfo.nLen;
	//	memcpy(&ARData,srcbinInfo.lpData,srcbinInfo.nLen);	
	//}

	sLogString = _T("");
	sLogString.Format(_T("SaleCard: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		DetailErrCode = 0;
		sLogString = _T("");
		sLogString.Format(_T("SaleCard: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
		theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

		if(UDDatalen>0){
			sLogString = _T("SaleCard: UDData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
		}

		if(ARDataLen>0){
			sLogString = _T("");
			sLogString.Format(_T("SaleCard: ARData.ARnum = %d"),ARData.ARnum);
			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

			sLogString = _T("SaleCard: ARData.ARData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
		}

		sLogString = _T("");
		sLogString.Format(_T("SaleCard: TranResult ="));
		theRW_TRACE->WriteData(_T("<")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
	}
	else{
		if(IsDcr4000ReturnCode(lCompleteCode))
		{
			DetailErrCode = lCompleteCode;
		}
		else
		{	
			GetErrCode(DetailErrCode);
		}
	}
    return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ۿ���������

@param      (i)byte *pTime, ����ʱ��
@param      (i)WORD wTimeLen, ʱ��ʱ��ĳ���
@param      (i)byte byIsCheck, �Ƿ���г���
@param      (o)byte byTpuState  ����tpu��״̬��
@param      (o)int &DetailErrCode  ����tpu�Ĵ�����

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::SetCheck(byte *pTime,byte byIsCheck,byte& byTpuState,int &DetailErrCode)
{
	SYNC(RW,m_RWID.name);

	CString sLogString(_T(""));
	sLogString.Format(_T("SetCheck:TranDateTime = "));
	theRW_TRACE->WriteData(_T(">")+sLogString,pTime,TRANS_TIME_LEN);

	long lCompleteCode = RW_RSP_OK;
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::SetCheck(pTime,byIsCheck,m_RWID.handle,byTpuState);
			/*}
			else
			{
			byTpuState = m_RWID.status;
			}*/

	sLogString = _T("");
	sLogString.Format(_T("SaleCard: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		sLogString = _T("");
		sLogString.Format(_T("SaleCard: TranResult ="));
		theRW_TRACE->WriteData(_T(">")+sLogString,&byTpuState,sizeof(byTpuState));
	}
	else{
		if(IsDcr4000ReturnCode(lCompleteCode))
		{
			DetailErrCode = lCompleteCode;
		}
		else
		{	
			GetErrCode(DetailErrCode);
		}
	}

	return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �˿�����

@param      (i)DWORD &SerialNo			// UDSN��һ��ͨ������ˮ��
@param      (i)BYTE*TranDateTime		// ���׷���������ʱ�� 7BCD	
@param      (i)SURRENDERINFO SaleInfo,
			typedef struct _SURRENDERINFO {
				uchar PayMethed;				// ���ʽ(Ԥ��)
				uchar CardApplicationType;		// ��Ƭ����(Ԥ��) 0x01:һ��ͨS50/S70��Ƭ��0x02:һ��ͨCPU����0x11:һƱͨS50/S70��Ƭ��0x12:һƱͨUL��
				ushort ProductyCategory;		// ��Ʊ��Ʒ����(Ԥ��)
				uchar SurrenderFlag;			// �˿���־(Ԥ��)
				uchar YPTSurrenderOperateType;	// һƱͨ�˿���������(Ԥ��)
				uchar YKTSurrenderOperateType;	// һ��ͨ�˿���������(Ԥ��)
				uchar ReasoneCode;				// �˿�����ԭ�����(Ԥ��)
				uchar BadCardNo[16];			// ��Ƭ����ӡˢ��(Ԥ��)
				uchar CardPhyID[7];				// ������ID(Ԥ��)
				ulong RefundMoney;				// ʵ�˽��(Ԥ��)
				ulong CardBalance;				// �������(Ԥ��)
				ulong CardTransCtr;				// �����׼�����(Ԥ��)
				uchar FareProductType;			// ��Ʊ��Ʒ����(Ԥ��)
				ushort FareProductyCategory;	// ��Ʊ��Ʒ����(Ԥ��)
				ulong CardDeposit;				// ����Ѻ��(Ԥ��)
				ulong ServiceFee;				// �����(Ԥ��)
				ulong CardCost;					// ���ɱ���(Ԥ��)
			} SURRENDERINFO;
@param      (o)uchar &SerAccnum			    // UDSN��һ��ͨ������ˮ���ۼ�ֵ
@param      (o)TRANRESULT &TranResult       // TPUҵ����󷵻صĽ������(ͬ��Ʊ)
@param      (o)ushort &UDDatalen			// UD���ݳ��� ����Ϊ0��ʾ�����UD���ݲ�����
@param      (o)uchar *UDData				// UD����(ͬ��Ʊ)			
@param      (o)ushort &ARDataLen			// AR���ݳ��� ����Ϊ0��ʾ�����AR���ݲ�����
@param      (o)ARDATA &ARData				// AR����(ͬ��Ʊ)
			
@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::Refund(DWORD &SerialNo,BYTE* TranDateTime,DAT::SURRENDERINFO RefundInfo,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
						 WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode)
{
	SYNC(RW,m_RWID.name);

	CString sLogString(_T(""));
	sLogString.Format(_T("CardSurrender: SerialNo = %d TranDateTime = "),SerialNo);
	theRW_TRACE->WriteData(_T(">")+sLogString,TranDateTime,TRANS_TIME_LEN);

	sLogString = _T("CardSurrender: RefundInfo =");
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&RefundInfo,sizeof(RefundInfo));
	long lCompleteCode = RW_RSP_OK;
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::CardSurrender(SerialNo,TranDateTime,RefundInfo,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);
	//}
	//else
	//{
	//	SerAccnum = 1;
	//	memset(&TranResult,0,sizeof(TranResult));
	//	VARIABLE_DATA srcbinInfo;
	//	// UD	
	//	//ReadSimDataFile(ACC_REFUND_UD_DATA_FILE_NAME,srcbinInfo);
	//	UDDatalen = srcbinInfo.nLen;
	//	memcpy(UDData,srcbinInfo.lpData,srcbinInfo.nLen);

	//	// AR
	//	//ReadSimDataFile(ACC_REFUND_AR_DATA_FILE_NAME,srcbinInfo);		
	//	ARDataLen = srcbinInfo.nLen;
	//	memcpy(&ARData,srcbinInfo.lpData,srcbinInfo.nLen);	
	//}
	sLogString = _T("");
	sLogString.Format(_T("CardSurrender: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		DetailErrCode = 0;
		sLogString = _T("");
		sLogString.Format(_T("CardSurrender: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

		if(UDDatalen>0){
			sLogString = _T("CardSurrender: UDData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
		}

		if(ARDataLen>0){
			sLogString = _T("");
			sLogString.Format(_T("CardSurrender: ARData.ARnum = %d"),ARData.ARnum);
			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

			sLogString = _T("CardSurrender: ARData.ARData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
		}

		sLogString = _T("");
		sLogString.Format(_T("CardSurrender: TranResult ="));
		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
	}
	else{
		if(IsDcr4000ReturnCode(lCompleteCode))
		{
			DetailErrCode = lCompleteCode;
		}
		else
		{	
			GetErrCode(DetailErrCode);
		}
	}
	
	return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���س�����������

@param      (i)WORD PacketNumber ���ݰ����,��һ������0x0000��ʼ�����������ۼӣ����һ������OxFFFF
@param      (i)BYTE* Data        ������Ϣ�����ص��������ݣ����1024byte,���һ��������1024�ֽڣ���������0x00   

@retval     long    �ɹ�:0; ʧ��:����Ӧ����      

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::ProgramDownload_Main(WORD PacketNumber,BYTE* Data)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T(""));
	SYNC(RW,m_RWID.name);
	sLogString.Format(_T("APP_Firmware_Downdload: PacketNumber = %d Data ="));
	theRW_TRACE->WriteData(_T(">")+sLogString,Data,PROGRAM_DATA_BLOCK_LEN);
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::APP_Firmware_Downdload(PacketNumber,Data,m_RWID.handle);
	//}
	//else
	//{
	//}

	sLogString = _T("");
	sLogString.Format(_T("APP_Firmware_Downdload: ReturnCode = 0x%02x "),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���س����������

@param      ��

@retval     long    �ɹ�:0; ʧ��:����Ӧ����      

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::ProgramUpdate_Main()
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T("APP_Firmware_Update"));
	SYNC(RW,m_RWID.name);

	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::APP_Firmware_Update(m_RWID.handle);
	//}
	//else
	//{
	//}
	sLogString = _T("");
	sLogString.Format(_T("APP_Firmware_Update: ReturnCode = 0x%02x "),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Ӧ�ó�����������

@param      (i)WORD PacketNumber ���ݰ����,��һ������0x0000��ʼ�����������ۼӣ����һ������OxFFFF
@param      (i)BYTE* Data        ������Ϣ�����ص��������ݣ����1024byte,���һ��������1024�ֽڣ���������0x00   

@retval     long    �ɹ�:0; ʧ��:����Ӧ����      

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::ProgramDownload_App(WORD PacketNumber,BYTE* Data)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T(""));
	SYNC(RW,m_RWID.name);

	sLogString.Format(_T("EXEC_Firmware_Downdload: PacketNumber = %d Data ="),PacketNumber);
	theRW_TRACE->WriteData(_T(">")+sLogString,Data,PROGRAM_DATA_BLOCK_LEN);

	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::EXEC_Firmware_Downdload(PacketNumber,Data,m_RWID.handle);
	//}
	//else
	//{
	//}
	sLogString = _T("");
	sLogString.Format(_T("EXEC_Firmware_Downdload: ReturnCode = 0x%02x "),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Ӧ�ó����������

@param      ��

@retval     long    �ɹ�:0; ʧ��:����Ӧ����      

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::ProgramUpdate_App()
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T("EXEC_Firmware_Update"));
	SYNC(RW,m_RWID.name);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::EXEC_Firmware_Update(m_RWID.handle);
	//}
	//else
	//{
	//}
	sLogString = _T("");
	sLogString.Format(_T("EXEC_Firmware_Update: ReturnCode = 0x%02x "),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����TPU��Ӫģʽ

@param      (i)BYTE ControlCode  ���ƴ���:0x01:����ģʽ;0x02:���ģʽ
@param      (i)uchar *ModeCode   ģʽ���룬����Ϊ2���ֽ�
@param      (o)uchar &statue     TPU״̬

@retval     long    �ɹ�:0; ʧ��:����Ӧ����      

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::SetOperationMode(BYTE ControlCode,BYTE *ModeCode,BYTE &status)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T(""));
	SYNC(RW,m_RWID.name);
	sLogString.Format(_T("SetOperationMode: ControlCode = %d ModeCode = "),ControlCode);
	theRW_TRACE->WriteData(_T(">")+sLogString,ModeCode,2);
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::SetBusinessMode(ControlCode,ModeCode,status,m_RWID.handle);
	//}
	//else
	//{
	//		status = m_RWID.status;
	//}

	sLogString = _T("");
	sLogString.Format(_T("SetBusinessMode: ReturnCode = 0x%02x "),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		sLogString.Format(_T("SetBusinessMode: status = %d "),status);
		theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	}
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡUD��������

@param      (o)uchar &SerAccnum			    // UDSN��һ��ͨ������ˮ���ۼ�ֵ
@param      (o)TRANRESULT &TranResult       // TPUҵ����󷵻صĽ������(ͬ��Ʊ)
@param      (o)ushort &UDDatalen			// UD���ݳ��� ����Ϊ0��ʾ�����UD���ݲ�����
@param      (o)uchar *UDData				// UD����(ͬ��Ʊ)
@param      (o)ushort &ARDataLen			// AR���ݳ��� ����Ϊ0��ʾ�����AR���ݲ�����
@param      (o)ARDATA &ARData				// AR����(ͬ��Ʊ)

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::GetUDData(BYTE &SerAccnum,DAT::TRANRESULT &TranResult,WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T("GetUDData"));
	SYNC(RW,m_RWID.name);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
			lCompleteCode = DAT::GetUDData(SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);
	//}
	//else
	//{
	//	SerAccnum = 1;
	//	memset(&TranResult,0,sizeof(TranResult));

	//	// UD
	//	VARIABLE_DATA data;
	//	//ReadSimDataFile(UD_DATA_FILE_NAME,data);		
	//	UDDatalen = data.nLen;
	//	memcpy(UDData,data.lpData,data.nLen);
	//}
	sLogString = _T("");
	sLogString.Format(_T("GetUDData: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		sLogString = _T("");
		sLogString.Format(_T("GetUDData: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

		if(UDDatalen>0){
			sLogString = _T("GetUDData: UDData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
		}

		if(ARDataLen>0){
			sLogString = _T("");
			sLogString.Format(_T("GetUDData: ARData.ARnum = %d"),ARData.ARnum);
			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

			sLogString = _T("GetUDData: ARData.ARData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
		}

		sLogString = _T("");
		sLogString.Format(_T("GetUDData: TranResult ="));
		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
	}
	return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ĸ���ⲿ��֤����

@param      (i)Struct_AuthIssueCardMasterCard &AuthIssueCardMasterCardData //����ĸ���ⲿ��֤����
@param      (o)uchar &Statue											   //�豸״̬

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//long CRWCommands::AuthIssueCardMasterCard(DAT::Struct_AuthIssueCardMasterCard authIssueCardMasterCard,BYTE& Statue)
//{
//	SYNC(RW,m_RWID.name);	
//	CString sLogString;
//
//	// ���п�ĸ���Ŀ���
//	sLogString.Format(_T("AuthIssueCardMasterCard:ICMC_Slot = %d "),authIssueCardMasterCard.ICMC_Slot);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	// ���п�ĸ�����俨�Ŀ���
//	sLogString.Format(_T("AuthIssueCardMasterCard:ICMCT_Slot = %d "),authIssueCardMasterCard.ICMCT_Slot);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	// PIN��ĳ���
//	sLogString.Format(_T("AuthIssueCardMasterCard:ICMCT_PIN_Len = %d "),authIssueCardMasterCard.ICMCT_PIN_Len);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	//���п�ĸ�����俨��PIN��
//	sLogString.Format(_T("AuthIssueCardMasterCard:ICMCT_PIN_buff = %d "),BCD2int((char*)authIssueCardMasterCard.ICMCT_PIN_buff,2));
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	long lCompleteCode = RW_RSP_OK;
//	int  DetailErrorCode = 0;
//	//if(theTVM_INFO.IsRWUsed(m_RWID.rwtype)){
//		lCompleteCode = DAT::AuthIssueCardMasterCard(authIssueCardMasterCard,Statue,m_RWID.handle);
//		if(lCompleteCode == RW_RSP_OK){
//			DetailErrorCode = 0;
//		}
//		else{
//			if(IsDcr4000ReturnCode(lCompleteCode)){
//				DetailErrorCode = lCompleteCode;
//			}
//			else{	
//				GetErrCode(DetailErrorCode);
//			}
//		}
//	//}
//	//else{		
//	//}
//	sLogString = _T("");
//	sLogString.Format(_T("AuthIssueCardMasterCard: ReturnCode = 0x%02x"),lCompleteCode);
//	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//	return  lCompleteCode;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::ExecuteCommand()
{
	return RW_RSP_OK;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     long     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::IsValidCommand()
{
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡģ�������ļ�

@param      (i)CString			fileName    �ļ���
@param      (o)VARIABLE_DATA&	filedata	�ļ���������

@retval     void     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//void CRWCommands:://ReadSimDataFile(CString fileName, VARIABLE_DATA& filedata)
//{
//	auto_ptr<CXFile> srcFile(new CXFile(fileName));
//	if (srcFile->ReadBinFile(filedata) != CXFile::FILE_OPERATION_OK){
//		throw CRWException(CRWException::OTHER_ERR, _T(""), __LINE__);
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����TPU 24Сʱ��Ӫ���ӳ���Ӫʱ��
 
@param      (i)uchar ControlCode ���õ�ģʽ   0x01������24Сʱ��Ӫ��0x02�����24Сʱ��Ӫ��0x10�������ӳ���Ӫ��
@param      (i)ulong Time �ӳ���ʱ��

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::SetBusinessTime(uchar ControlCode,ulong Time,uchar &status)
{
	long lCompleteCode = RW_RSP_OK;
	CString sLogString(_T(""));
	SYNC(RW,m_RWID.name);
	sLogString.Format(_T("SetBusinessTime: ControlCode = %d, Time =%d"),ControlCode,Time);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
		lCompleteCode = DAT::SetBusinessTime(ControlCode,Time,status,m_RWID.handle);
	//}
	//else
	//{
	//	status = m_RWID.status;
	//}

	sLogString = _T("");
	sLogString.Format(_T("SetBusinessTime: ReturnCode = 0x%02x "),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		sLogString.Format(_T("SetBusinessTime: status = %d "),status);
		theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	}
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����

@param      (i)BYTE*	TranDateTime	 ���׷���������ʱ��		7BCD
@param      (i)WORD		DeferTime		��Ч��˳�ӵĳ��ȣ�����Ϊ��λ
@param      (i)BYTE		&Status			��д��״̬

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::Defer(DAT::STT_DEFERCARD sttDefer,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
						WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode)
{
	SYNC(RW,m_RWID.name);

	CString sLogString;
	//sLogString.Format(_T("DeferCard: DeferTime = %d TranDateTime = "),DeferTime);
	//theRW_TRACE->WriteData(_T(">")+sLogString,TranDateTime,TRANS_TIME_LEN);

	long lCompleteCode = RW_RSP_OK;
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
		lCompleteCode = DAT::Defercard(sttDefer,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);
	//}
	//else
	//{
		//SerAccnum = 1;
		//memset(&TranResult,0,sizeof(TranResult));
		//VARIABLE_DATA srcbinInfo;
		//// UD	
		////ReadSimDataFile(ACC_DEFER_UD_DATA_FILE_NAME,srcbinInfo);
		//UDDatalen = srcbinInfo.nLen;
		//memcpy(UDData,srcbinInfo.lpData,srcbinInfo.nLen);

		//// AR
		////ReadSimDataFile(ACC_DEFER_AR_DATA_FILE_NAME,srcbinInfo);		
		//ARDataLen = srcbinInfo.nLen;
		//memcpy(&ARData,srcbinInfo.lpData,srcbinInfo.nLen);
	//}

	sLogString = _T("");
	sLogString.Format(_T("Defercard: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		DetailErrCode = 0;
		sLogString = _T("");
		sLogString.Format(_T("Defercard: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

		if(UDDatalen>0){
			sLogString = _T("Defercard: UDData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
		}

		if(ARDataLen>0){
			sLogString = _T("");
			sLogString.Format(_T("Defercard: ARData.ARnum = %d"),ARData.ARnum);
			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

			sLogString = _T("Defercard: ARData.ARData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
		}

		sLogString = _T("");
		sLogString.Format(_T("Defercard: TranResult ="));
		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
	}
	else{
		if(IsDcr4000ReturnCode(lCompleteCode))
		{
			DetailErrCode = lCompleteCode;
		}
		else
		{	
			GetErrCode(DetailErrCode);
		}
	}

	return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������Ϣ�޸�

@param      (i)DAT::STT_RENMCD_CHG SignCardInfo  ��������Ϣ
@param      (o)BYTE &Status  ����tpu��״̬��
@param      (o)int &DetailErrCode  ����tpu�Ĵ�����

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::UpdateSignCardInfo(DAT::STT_RENMCD_CHG SignCardInfo,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
									 WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode)
{
	SYNC(RW,m_RWID.name);

	CString sLogString;
	sLogString.Format(_T("UpdateSignCardInfo: SignCardInfo.uUdsn = %d SignCardInfo.mDateTime = "),SignCardInfo.uUdsn);
	theRW_TRACE->WriteData(_T(">")+sLogString,SignCardInfo.mDateTime,TRANS_TIME_LEN);

	sLogString.Format(_T("UpdateSignCardInfo: SignCardInfo.byTypeDsg = %d SignCardInfo.byStaffDsg = %d"),SignCardInfo.byTypeDsg,SignCardInfo.byStaffDsg);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	sLogString = _T("UpdateSignCardInfo: SignCardInfo.szName = ");
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)SignCardInfo.szName,sizeof(SignCardInfo.szName));

	sLogString = _T("UpdateSignCardInfo: SignCardInfo.szCreditNo = ");
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)SignCardInfo.szCreditNo,sizeof(SignCardInfo.szCreditNo));

	sLogString.Format(_T("UpdateSignCardInfo: SignCardInfo.byCreditType = %d SignCardInfo.wLfAmt = %d SignCardInfo.bySex = %d SignCardInfo.byAppVer = %d"),SignCardInfo.byCreditType,SignCardInfo.wLfAmt,SignCardInfo.bySex,SignCardInfo.byAppVer);
	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	sLogString = _T("UpdateSignCardInfo: SignCardInfo.mCertificateValidity = ");
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)SignCardInfo.mCertificateValidity,sizeof(SignCardInfo.mCertificateValidity));

	sLogString = _T("UpdateSignCardInfo: SignCardInfo.mTel = ");
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)SignCardInfo.mTel,sizeof(SignCardInfo.mTel));

	sLogString = _T("UpdateSignCardInfo: SignCardInfo.mEmail = ");
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)SignCardInfo.mEmail,sizeof(SignCardInfo.mEmail));

	sLogString = _T("UpdateSignCardInfo: SignCardInfo.mAddr1 = ");
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)SignCardInfo.mAddr1,sizeof(SignCardInfo.mAddr1));

	sLogString = _T("UpdateSignCardInfo: SignCardInfo.mAddr2 = ");
	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)SignCardInfo.mAddr2,sizeof(SignCardInfo.mAddr2));

	long lCompleteCode = RW_RSP_OK;
	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
	//{
		lCompleteCode = DAT::UpdateRealNameInfo(SignCardInfo,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);
	//}
	//else
	//{
		//SerAccnum = 1;
		//memset(&TranResult,0,sizeof(TranResult));
		//VARIABLE_DATA srcbinInfo;
		//// UD	
		////ReadSimDataFile(ACC_DEFER_UD_DATA_FILE_NAME,srcbinInfo);
		//UDDatalen = srcbinInfo.nLen;
		//memcpy(UDData,srcbinInfo.lpData,srcbinInfo.nLen);

		//// AR
		////ReadSimDataFile(ACC_DEFER_AR_DATA_FILE_NAME,srcbinInfo);		
		//ARDataLen = srcbinInfo.nLen;
		//memcpy(&ARData,srcbinInfo.lpData,srcbinInfo.nLen);
	//}

	sLogString = _T("");
	sLogString.Format(_T("UpdateSignCardInfo: ReturnCode = 0x%02x"),lCompleteCode);
	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	if(lCompleteCode == RW_RSP_OK){
		DetailErrCode = 0;
		sLogString = _T("");
		sLogString.Format(_T("UpdateSignCardInfo: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);

		if(UDDatalen>0){
			sLogString = _T("UpdateSignCardInfo: UDData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
		}

		if(ARDataLen>0){
			sLogString = _T("");
			sLogString.Format(_T("UpdateSignCardInfo: ARData.ARnum = %d"),ARData.ARnum);
			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);

			sLogString = _T("UpdateSignCardInfo: ARData.ARData =");
			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
		}

		sLogString = _T("");
		sLogString.Format(_T("UpdateSignCardInfo: TranResult ="));
		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
	}
	else{
		if(IsDcr4000ReturnCode(lCompleteCode))
		{
			DetailErrCode = lCompleteCode;
		}
		else
		{	
			GetErrCode(DetailErrCode);
		}
	}

	return  lCompleteCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Ի�

@param      (i)DAT::Struct_Individuation IndividuationData  ���Ի���Ϣ
			//���Ի������贫����
			typedef struct STRUCT_Individuation
			{
			uchar OperitionMode;//��������
			ushort CorporationCode;//��˾����
			ulong PersonalNO;//Ա�����
			//�ֿ�������
			char CardHolderName[8];
			uchar CredentialsNOType;//֤����������
			uchar CredentialsNO[9];//֤������
			ulong Birthday;//����
			uchar PassengerLevel;//�˿ͼ���
			uchar Reserve[8];//Ԥ��
			uchar PayMethed;//���ʽ
			ulong PayAmount;
			uchar BankCardNo[10];
			}Struct_Individuation;
@param      (o)BYTE &Status  ����tpu��״̬��
@param      (o)int &DetailErrCode  ����tpu�Ĵ�����

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//long CRWCommands::Personalize(DWORD &SerialNo,BYTE* TranDateTime,DAT::Struct_Individuation IndividuationData,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
//									 WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode)
//{
//	SYNC(RW,m_RWID.name);
//
//	CString sLogString;
//	sLogString.Format(_T("Personalize: SerialNo = %d IndividuationData.TranDateTime = "),SerialNo,TranDateTime);
//	theRW_TRACE->WriteData(_T(">")+sLogString,TranDateTime,TRANS_TIME_LEN);
//
//	sLogString.Format(_T("Personalize: IndividuationData.OperitionMode = %d IndividuationData.CorporationCode = %d IndividuationData.PersonalNO = %d"),IndividuationData.OperitionMode,IndividuationData.CorporationCode,IndividuationData.PersonalNO );
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	sLogString = _T("Personalize: IndividuationData.CardHolderName = ");
//	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)IndividuationData.CardHolderName,8);
//
//	sLogString = _T("Personalize: IndividuationData.CredentialsNO = ");
//	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)IndividuationData.CredentialsNO,9);
//
//	sLogString.Format(_T("Personalize: IndividuationData.CredentialsNOType = %d IndividuationData.Birthday = %d IndividuationData.PassengerLevel = %d IndividuationData.PayMethed = %d IndividuationData.PayAmount = %d"),IndividuationData.CredentialsNOType,IndividuationData.Birthday,IndividuationData.PassengerLevel,IndividuationData.PayMethed,IndividuationData.PayAmount);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	sLogString = _T("Personalize: IndividuationData.BankCardNo = ");
//	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)IndividuationData.BankCardNo,10);
//
//	long lCompleteCode = RW_RSP_OK;
//	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
//	//{
//		lCompleteCode = DAT::individuation(SerialNo, TranDateTime,IndividuationData,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);
//	//}
//	//else
//	//{
//		//SerAccnum = 1;
//		//memset(&TranResult,0,sizeof(TranResult));
//		//VARIABLE_DATA srcbinInfo;
//		//// UD	
//		////ReadSimDataFile(ACC_DEFER_UD_DATA_FILE_NAME,srcbinInfo);
//		//UDDatalen = srcbinInfo.nLen;
//		//memcpy(UDData,srcbinInfo.lpData,srcbinInfo.nLen);
//
//		//// AR
//		////ReadSimDataFile(ACC_DEFER_AR_DATA_FILE_NAME,srcbinInfo);		
//		//ARDataLen = srcbinInfo.nLen;
//		//memcpy(&ARData,srcbinInfo.lpData,srcbinInfo.nLen);
//	//}
//
//	sLogString = _T("");
//	sLogString.Format(_T("Personalize: ReturnCode = 0x%02x"),lCompleteCode);
//	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//	if(lCompleteCode == RW_RSP_OK){
//		DetailErrCode = 0;
//		sLogString = _T("");
//		sLogString.Format(_T("Personalize: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//		if(UDDatalen>0){
//			sLogString = _T("Personalize: UDData =");
//			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
//		}
//
//		if(ARDataLen>0){
//			sLogString = _T("");
//			sLogString.Format(_T("Personalize: ARData.ARnum = %d"),ARData.ARnum);
//			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//			sLogString = _T("Personalize: ARData.ARData =");
//			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
//		}
//
//		sLogString = _T("");
//		sLogString.Format(_T("Personalize: TranResult ="));
//		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
//	}
//	else{
//		if(IsDcr4000ReturnCode(lCompleteCode))
//		{
//			DetailErrCode = lCompleteCode;
//		}
//		else
//		{	
//			GetErrCode(DetailErrCode);
//		}
//	}
//
//	return  lCompleteCode;
//}
//
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Ի�����д�¿�

@param      (i)DAT::Struct_IndivRenewNewCard IndividuationData   ���Ի������贫����
			typedef struct STRUCT_IndivRenewNewCard
			{
			//ulong DealNo;//������ˮ��
			//uchar DealTime[7];//����ʱ��
			uchar OperitionMode;//��������
			ushort CorporationCode;//��˾����
			ulong PersonalNO;//Ա�����
			//�ֿ�������
			char CardHolderName[8];
			uchar CredentialsNOType;//֤����������
			uchar CredentialsNO[9];//֤������
			ulong Birthday;//����
			uchar PassengerLevel;//�˿ͼ���
			uchar Reserve[8];//Ԥ��
			uchar PayMethed;//���ʽ
			ulong PayAmount;
			uchar BankCardNo[10];
            }Struct_IndivRenewNewCard;
@param      (o)BYTE &Status  ����tpu��״̬��
@param      (o)int &DetailErrCode  ����tpu�Ĵ�����

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//long CRWCommands::PersonalizeUpdateNewCard(DWORD &SerialNo,BYTE* TranDateTime,DAT::Struct_IndivRenewNewCard IndividuationData,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
//									 WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode)
//{
//	SYNC(RW,m_RWID.name);
//
//	CString sLogString;
//	sLogString.Format(_T("PersonalizeUpdateNewCard: SerialNo = %d IndividuationData.TranDateTime = "),SerialNo,TranDateTime);
//	theRW_TRACE->WriteData(_T(">")+sLogString,TranDateTime,TRANS_TIME_LEN);
//
//	sLogString.Format(_T("PersonalizeUpdateNewCard: IndividuationData.OperitionMode = %d IndividuationData.CorporationCode = %d IndividuationData.PersonalNO = %d"),IndividuationData.OperitionMode,IndividuationData.CorporationCode,IndividuationData.PersonalNO );
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	sLogString = _T("PersonalizeUpdateNewCard: IndividuationData.CardHolderName = ");
//	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)IndividuationData.CardHolderName,8);
//
//	sLogString = _T("PersonalizeUpdateNewCard: IndividuationData.CredentialsNO = ");
//	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)IndividuationData.CredentialsNO,9);
//
//	sLogString.Format(_T("PersonalizeUpdateNewCard: IndividuationData.CredentialsNOType = %d IndividuationData.Birthday = %d IndividuationData.PassengerLevel = %d IndividuationData.PayMethed = %d IndividuationData.PayAmount = %d"),IndividuationData.CredentialsNOType,IndividuationData.Birthday,IndividuationData.PassengerLevel,IndividuationData.PayMethed,IndividuationData.PayAmount);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	sLogString = _T("PersonalizeUpdateNewCard: IndividuationData.BankCardNo = ");
//	theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)IndividuationData.BankCardNo,10);
//
//	long lCompleteCode = RW_RSP_OK;
//	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
//	//{
//		lCompleteCode = DAT::IndivRenewNewCard(SerialNo, TranDateTime,IndividuationData,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);
//	//}
//	//else
//	//{
//		//SerAccnum = 1;
//		//memset(&TranResult,0,sizeof(TranResult));
//		//VARIABLE_DATA srcbinInfo;
//		//// UD	
//		////ReadSimDataFile(ACC_DEFER_UD_DATA_FILE_NAME,srcbinInfo);
//		//UDDatalen = srcbinInfo.nLen;
//		//memcpy(UDData,srcbinInfo.lpData,srcbinInfo.nLen);
//
//		//// AR
//		////ReadSimDataFile(ACC_DEFER_AR_DATA_FILE_NAME,srcbinInfo);		
//		//ARDataLen = srcbinInfo.nLen;
//		//memcpy(&ARData,srcbinInfo.lpData,srcbinInfo.nLen);
//	//}
//
//	sLogString = _T("");
//	sLogString.Format(_T("PersonalizeUpdateNewCard: ReturnCode = 0x%02x"),lCompleteCode);
//	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//	if(lCompleteCode == RW_RSP_OK){
//		DetailErrCode = 0;
//		sLogString = _T("");
//		sLogString.Format(_T("PersonalizeUpdateNewCard: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//		if(UDDatalen>0){
//			sLogString = _T("PersonalizeUpdateNewCard: UDData =");
//			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
//		}
//
//		if(ARDataLen>0){
//			sLogString = _T("");
//			sLogString.Format(_T("PersonalizeUpdateNewCard: ARData.ARnum = %d"),ARData.ARnum);
//			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//			sLogString = _T("PersonalizeUpdateNewCard: ARData.ARData =");
//			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
//		}
//
//		sLogString = _T("");
//		sLogString.Format(_T("PersonalizeUpdateNewCard: TranResult ="));
//		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
//	}
//	else{
//		if(IsDcr4000ReturnCode(lCompleteCode))
//		{
//			DetailErrCode = lCompleteCode;
//		}
//		else
//		{	
//			GetErrCode(DetailErrCode);
//		}
//	}
//
//	return  lCompleteCode;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Ի���������ɿ���

@param      (i)DWORD		&SerialNo			// UDSN��һ��ͨ������ˮ��
@param      (i)BYTE*		TranDateTime		// ���׷���������ʱ�� 7BCD
@param      (o)BYTE			&SerAccnum			// UDSN��һ��ͨ������ˮ���ۼ�ֵ
@param      (o)TRANRESULT	&TranResult			// TPUҵ����󷵻صĽ������(ͬ��Ʊ)
@param      (o)WORD			&UDDatalen			// UD���ݳ��� ����Ϊ0��ʾ�����UD���ݲ�����
@param      (o)BYTE*		UDData				// UD����(ͬ��Ʊ)			
@param      (o)WORD			&ARDataLen			// AR���ݳ��� ����Ϊ0��ʾ�����AR���ݲ�����
@param      (o)ARDATA		&ARData				// AR����(ͬ��Ʊ)

@retval     long    �ɹ�:0; ʧ��:����Ӧ����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//long CRWCommands::PersonalizeUpdateOldCard(DWORD &SerialNo,BYTE* TranDateTime,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
//								 WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode)
//{
//	CString sLogString;
//	SYNC(RW,m_RWID.name);
//	sLogString.Format(_T("PersonalizeUpdateOldCard: SerialNo = %d TranDateTime = "),SerialNo);
//	theRW_TRACE->WriteData(_T(">")+sLogString,TranDateTime,TRANS_TIME_LEN);
//
//	long lCompleteCode = RW_RSP_OK;
//	//if(theTVM_INFO.IsRWUsed(m_RWID.name==_T("RW")))
//	//{
//		lCompleteCode = DAT::IndivRenewOldCard(SerialNo,TranDateTime,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);
//	//}
//	//else
//	//{
//	//	SerAccnum = 1;
//	//	memset(&TranResult,0,sizeof(TranResult));
//	//	VARIABLE_DATA srcbinInfo;
//	//	// UD
//	//	if(theTVM_INFO.GetSimCardType(false) == RW_SIM_M1_CARD_TYPE){		
//	//		//ReadSimDataFile(ACC_M1_BLOCK_UD_DATA_FILE_NAME,srcbinInfo);
//	//	}
//	//	else{
//	//		//ReadSimDataFile(ACC_BLOCK_UD_DATA_FILE_NAME,srcbinInfo);
//	//	}
//	//	UDDatalen = srcbinInfo.nLen;
//	//	memcpy(UDData,srcbinInfo.lpData,srcbinInfo.nLen);
//
//	//	// AR
//	//	if(theTVM_INFO.GetSimCardType(false) == RW_SIM_M1_CARD_TYPE){
//	//		//ReadSimDataFile(ACC_M1_BLOCK_AR_DATA_FILE_NAME,srcbinInfo);
//	//	}
//	//	else{
//	//		//ReadSimDataFile(ACC_BLOCK_AR_DATA_FILE_NAME,srcbinInfo);
//	//	}
//	//	ARDataLen = srcbinInfo.nLen;
//	//	memcpy(&ARData,srcbinInfo.lpData,srcbinInfo.nLen);
//	//}
//
//	sLogString = _T("");
//	sLogString.Format(_T("CardBlock: ReturnCode = 0x%02x"),lCompleteCode);
//	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//	if(lCompleteCode == RW_RSP_OK){
//		DetailErrCode = 0;
//
//		sLogString = _T("");
//		sLogString.Format(_T("PersonalizeUpdateOldCard: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//		if(UDDatalen>0){
//			sLogString = _T("PersonalizeUpdateOldCard: UDData =");
//			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
//		}
//
//		if(ARDataLen>0){
//			sLogString = _T("");
//			sLogString.Format(_T("PersonalizeUpdateOldCard: ARData.ARnum = %d"),ARData.ARnum);
//			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//			sLogString = _T("PersonalizeUpdateOldCard: ARData.ARData =");
//			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
//		}
//
//		sLogString = _T("");
//		sLogString.Format(_T("PersonalizeUpdateOldCard: TranResult ="));
//		theRW_TRACE->WriteData(_T(">")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
//	}
//	else{
//		if(IsDcr4000ReturnCode(lCompleteCode))
//		{
//			DetailErrCode = lCompleteCode;
//		}
//		else
//		{	
//			GetErrCode(DetailErrCode);
//		}
//	}
//	return  lCompleteCode;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ES���н���

@param      (i)Struct_ReleaseES &ReleaseData  ES���н�����Ϣ
@param      (o)uchar &Statue  ����tpu��״̬��
@param      (o)int &DetailErrCode  ����tpu�Ĵ�����

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//long CRWCommands::ReleaseEs(ulong dwUdsn,				/*in,һ��ͨ������ˮ��*/
//							uchar *TranDateTime,			/*in,����ʱ��*/
//							DAT::Struct_ReleaseES &ReleaseData,	/*in,��Ƭ��Ϣ*/
//							uchar &SerAccnum,				/*out,������ˮ���ۼ�ֵ*/
//							DAT::TRANRESULT &TranResult,			/*out,���ص� tpu ִ��״̬*/
//							ushort &sArgUdLen,				/*out,��*/
//							uchar *pArgUdBuf,				/*out,*/
//							ushort &sArgArLen,				/*out,��*/
//							DAT::ARDATA &sttArgAr,				/*out,��*/
//							int DetailErrorCode			/*out,��*/
//							)
//{
//	SYNC(RW,m_RWID.name);	
//	CString sLogString;
//
//	// һ��ͨ������ˮ��
//	sLogString.Format(_T("ReleaseEs:dwUdsn = %lu "),dwUdsn);
//	theRW_TRACE->WriteData(_T(">") + sLogString,NULL,0);
//
//	// ����ʱ��
//	sLogString = _T("ReleaseEs:TranDateTime = ");	
//	theRW_TRACE->WriteData(_T(">") + sLogString,TranDateTime,TRANS_TIME_LEN);
//
//	// uchar OperitionMode;��������
//	sLogString.Format(_T("ReleaseEs: ReleaseData.OperitionMode = %d "),ReleaseData.OperitionMode);
//	theRW_TRACE->WriteData(_T(">") + sLogString,NULL,0);
//
//	//��ʼ�����ر����ע��	
//	if ((ReleaseData.OperitionMode == OPERATION_TYPE_INITIALIZATION) || 
//		(ReleaseData.OperitionMode == OPERATION_TYPE_REENCODE) ||
//		(ReleaseData.OperitionMode == OPERATION_TYPE_INITIALIZATION_REENCODE) ||
//		(ReleaseData.OperitionMode == OPERATION_TYPE_REENCODE_PRE_ISSUE) ||
//		(ReleaseData.OperitionMode == OPERATION_TYPE_CANCEL))
//	{			
//		// uchar FormVisionNO;��ʽ�汾��
//		sLogString.Format(_T("ReleaseEs: ReleaseData.FormVisionNO = %d "),ReleaseData.FormVisionNO);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar ManufID;����ID
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ManufID = %d "),ReleaseData.ManufID);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar KeyEdition;��Կ�汾
//		sLogString.Format(_T("ReleaseEs: ReleaseData.KeyEdition = %d "),ReleaseData.KeyEdition);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// ushort CityCode;���д���
//		sLogString.Format(_T("ReleaseEs: ReleaseData.CityCode = %d "),ReleaseData.CityCode);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// ushort TradeCode;��ҵ����
//		sLogString.Format(_T("ReleaseEs: ReleaseData.TradeCode = %d "),ReleaseData.TradeCode);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar SupplyTime[3];����ʱ��
//		char szSupplyTime[10] = {0};
//		CString strSupplyTime  = _T("");
//		BCD2PSTR((char*)ReleaseData.SupplyTime,3,szSupplyTime);
//		USES_CONVERSION;
//		sLogString.Format(_T("ReleaseEs: ReleaseData.SupplyTime = %s"),A2T(szSupplyTime));
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// ushort SupplyDatch;��������
//		sLogString.Format(_T("ReleaseEs: ReleaseData.SupplyDatch = %d "),ReleaseData.SupplyDatch);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	}	
//	//Ԥ��ֵ
//	if ((ReleaseData.OperitionMode == OPERATION_TYPE_PRE_ISSUE) || 		
//		(ReleaseData.OperitionMode == OPERATION_TYPE_REENCODE_PRE_ISSUE))
//	{
//		// uchar ReleaseID;��Ʒ������ID
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ReleaseID = %d "),ReleaseData.ReleaseID);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar CardType;��Ƭ��������
//		sLogString.Format(_T("ReleaseEs: ReleaseData.CardType = %d "),ReleaseData.CardType);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar ProductCategory;��Ʒ���
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ProductCategory = %d "),ReleaseData.ProductCategory);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar ProductCategory;��Ʒ���
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ProductCategory = %d "),ReleaseData.ProductCategory);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar ProductType;��Ʒ����
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ProductType = %d "),ReleaseData.ProductType);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar ProductSubtype;��Ʒ������
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ProductSubtype = %d "),ReleaseData.ProductSubtype);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar PassengerType;�˿�����
//		sLogString.Format(_T("ReleaseEs: ReleaseData.PassengerType = %d "),ReleaseData.PassengerType);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// ulong CardAmount;���ڽ��
//		sLogString.Format(_T("ReleaseEs: ReleaseData.CardAmount = %lu "),ReleaseData.CardAmount);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// ulong CardTimes;���ڴ���
//		sLogString.Format(_T("ReleaseEs: ReleaseData.CardTimes = %lu "),ReleaseData.CardTimes);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar ValidityPeriod;��Ч�ڼ�
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ValidityPeriod = %d "),ReleaseData.ValidityPeriod);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar ValidityPeriodUnit;��Ч�ڼ䵥λ
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ValidityPeriodUnit = %d "),ReleaseData.ValidityPeriodUnit);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// ulong CardDeposit;Ѻ��
//		sLogString.Format(_T("ReleaseEs: ReleaseData.CardDeposit = %lu "),ReleaseData.CardDeposit);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// ulong BookingAmount;��Ʊ���
//		sLogString.Format(_T("ReleaseEs: ReleaseData.BookingAmount = %lu "),ReleaseData.BookingAmount);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar VolidStartTime[7];��Ч��ʼ����
//		sLogString.Format(_T("ReleaseEs: ReleaseData.VolidStartTime = %s "),ReleaseData.VolidStartTime);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);		
//	}
//	long lCompleteCode = RW_RSP_OK;
//	//if(theTVM_INFO.IsRWUsed(m_RWID.rwtype)){
//		lCompleteCode = releaseES(dwUdsn,
//			TranDateTime,
//			ReleaseData,
//			SerAccnum,
//			TranResult,
//			sArgUdLen,
//			pArgUdBuf,
//			sArgArLen,
//			sttArgAr,			
//			m_RWID.handle);
//	//}
//	//else{		
//	//}
//	sLogString = _T("");
//	sLogString.Format(_T("ReleaseEs: ReturnCode = 0x%02x"),lCompleteCode);
//	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//	if(lCompleteCode == RW_RSP_OK){
//		DetailErrorCode = 0;
//	}
//	else{
//		if(IsDcr4000ReturnCode(lCompleteCode)){
//			DetailErrorCode = lCompleteCode;
//		}
//		else{	
//			GetErrCode(DetailErrorCode);
//		}
//	}
//
//	return  lCompleteCode;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Ի�����

@param      (i)Struct_Individuation &IndividuationData ���Ի�������Ϣ
@param      (o)uchar &Statue  ����tpu��״̬��
@param      (o)int &DetailErrCode  ����tpu�Ĵ�����

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//long CRWCommands::IndividuationPm(ulong dwUdsn,								/*in,һ��ͨ������ˮ��*/
//								  uchar *TranDateTime,						/*in,����ʱ��*/
//								  DAT::Struct_Individuation &IndividuationData,	/*in,��Ƭ��Ϣ*/
//								  uchar &SerAccnum,							/*out,������ˮ���ۼ�ֵ*/
//								  DAT::TRANRESULT &TranResult,					/*out,���ص� tpu ִ��״̬*/
//								  ushort &sArgUdLen,						/*out,��*/
//								  uchar *pArgUdBuf,							/*out,*/
//								  ushort &sArgArLen,						/*out,��*/
//								  DAT::ARDATA &sttArgAr,							/*out,��*/
//								  int DetailErrorCode						/*out,��*/
//								  )
//{
//	SYNC(RW,m_RWID.name);	
//	CString sLogString;
//
//	// һ��ͨ������ˮ��
//	sLogString.Format(_T("ReleaseEs:dwUdsn = %lu "),dwUdsn);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	// ����ʱ��
//	sLogString.Format(_T("ReleaseEs:TranDateTime = %s "),TranDateTime);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	// uchar OperitionMode;��������
//	sLogString.Format(_T("IndividuationPm: IndividuationData.OperitionMode = %c "),IndividuationData.OperitionMode);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	// ushort CorporationCode;��˾����
//	sLogString.Format(_T("IndividuationPm: IndividuationData.CorporationCode = %d "),IndividuationData.CorporationCode);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	// ulong PersonelNO;Ա�����
//	sLogString.Format(_T("IndividuationPm: IndividuationData.PersonelNO = %lu "),IndividuationData.PersonalNO);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	// uchar CardHolderName[8];�ֿ�������
//	sLogString.Format(_T("IndividuationPm: IndividuationData.CardHolderName = %s "),IndividuationData.CardHolderName);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	// uchar CredentialsNOType;֤����������
//	sLogString.Format(_T("IndividuationPm: IndividuationData.CredentialsNOType = %c "),IndividuationData.CredentialsNOType);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	// uchar CredentialsNO[9];֤������
//	sLogString.Format(_T("IndividuationPm: IndividuationData.CredentialsNO = %s "),IndividuationData.CredentialsNO);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	// ulong Birthday;����
//	sLogString.Format(_T("IndividuationPm: IndividuationData.Birthday = %lu "),IndividuationData.Birthday);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	// uchar PassengerLevel;�˿ͼ���
//	sLogString.Format(_T("IndividuationPm: IndividuationData.PassengerLevel = %c "),IndividuationData.PassengerLevel);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	long lCompleteCode = RW_RSP_OK;
//	//if(theTVM_INFO.IsRWUsed(m_RWID.rwtype)){
//		lCompleteCode = individuation(dwUdsn,
//			TranDateTime,
//			IndividuationData,
//			SerAccnum,
//			TranResult,
//			sArgUdLen,
//			pArgUdBuf,
//			sArgArLen,
//			sttArgAr,
//			m_RWID.handle);
//	//}
//	//else{
//	//}
//	sLogString = _T("");
//	sLogString.Format(_T("IndividuationPm: ReturnCode = 0x%02x"),lCompleteCode);
//	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//	if(lCompleteCode == RW_RSP_OK){
//		DetailErrorCode = 0;
//	}
//	else{
//		if(IsDcr4000ReturnCode(lCompleteCode)){
//			DetailErrorCode = lCompleteCode;
//		}
//		else{	
//			GetErrCode(DetailErrorCode);
//		}
//	}
//
//	return  lCompleteCode;
//}
//
//////////////////////////////////////////////////////////////////////////
/*
@brief      EP֧������

@param      (i)STT_DEPOSIT_EXPENSE inExpactData,
			typedef struct STRUCT_DEPOSIT_EXPENSE
			{
			DWORD uUdsn;
			byte mDateTime[7];
			DWORD uAmount;//���ѽ��
			STRUCT_DEPOSIT_EXPENSE()
			{
			uUdsn=0;
			memset(mDateTime,0,sizeof(mDateTime));
			uAmount=0;
			}
			} STT_DEPOSIT_EXPENSE;
@param      (o)uchar &SerAccnum			    // UDSN��һ��ͨ������ˮ���ۼ�ֵ
@param      (o)TRANRESULT &TranResult       // TPUҵ����󷵻صĽ������
			typedef struct __TRANRESULT
			{
			uchar RetractFlag;			// Ʊ������״̬	0x00:������;0x01:��������;0x02:��Ʊ����;0x03:�ﵽ������������;0x04:���λ���
			uchar HighBlacklist;        // �߼���������־(Ԥ��) 0x00:��;0x01:��
			uchar CardType;				// ��Ƭ���࣬0x01:һ��ͨS50/S70��;0x02:һ��ͨCPU��;0x11:һƱͨS50/S70��;0x12:һƱͨUL��
			ushort ProductyCategory;	// ��Ʊ��Ʒ���� TicketFamily_t + ProductType
			uchar CardNo[4];			// һ��ͨ��������ˮ�Ż�һƱͨ�߼�����
			ulong BeforAmount;			// ����ǰ���,�ƴο���Σ����ڿ����ֶ���Ч��
			ulong AfterAmount;			// ���������,�ƴο���Σ����ڿ����ֶ���Ч��
			uchar ucBlockFlag;			// Ʊ������״̬	0x00:Ʊ��δ����;0x01:Ʊ��������
			uchar Reserved[14];			// Ԥ��
			}TRANRESULT;
@param      (o)ushort &UDDatalen			// UD���ݳ��� ����Ϊ0��ʾ�����UD���ݲ�����
@param      (o)uchar *UDData				// UD����
1	UD���ݼ�¼����	1	HEX	
2	UD���ݼ�¼��	
UD���ݼ�¼��Ľṹ���£�
���	�ֶ�	      �ֽ���	����	��ע
2.1	  �����������	   1	    HEX	    0x01:һ��ͨ���� 0x02:һƱͨ����
2.2	  �������ݳ���	   2	    HEX	
2.3	  ��������			

@param      (o)ushort &ARDataLen			// AR���ݳ��� ����Ϊ0��ʾ�����AR���ݲ�����
@param      (o)ARDATA &ARData				// AR����
			typedef struct __ARDATA 
			{
			uchar ARnum;				// 1.AR���ݼ�¼����
			uchar ARdata[BUF_SIZE];		// 2.AR���ݼ�¼��
			���	�ֶ�	  �ֽ���	����	��ע
			2.1	 �豸�Ĵ���ID	2	    HEX	   ��TPU��ص��豸�Ĵ���ID
			2.2	 AR����������	4	    HEX	   SLE����AR�����������ۼ��豸�ڱ����AR����
			}ARDATA;
@retval     long    �ɹ�:0; ʧ��:����Ӧ����  

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//long CRWCommands::ConsumeEPPay(DWORD &SerialNo,BYTE* TranDateTime,DAT::Struct_Consume &inStConsume,BYTE &SerAccnum,DAT::TRANRESULT &TranResult, WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int& DetailErrCode){
//
//	long lCompleteCode = RW_RSP_OK;
//	CString sLogString(_T(""));
//	SYNC(RW,m_RWID.name);
//
//	sLogString.Format(_T("EPPay: SerialNo = %d TranDateTime = "),SerialNo);
//	theRW_TRACE->WriteData(_T(">")+sLogString,TranDateTime,TRANS_TIME_LEN);	
//
//	sLogString.Format(_T("EPPay: TranAmount = %d"),inStConsume.ConsumeAmount);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	// EP ֧���ۿ�
//	lCompleteCode = DAT::consume(SerialNo,TranDateTime,inStConsume,SerAccnum,TranResult,UDDatalen,UDData,ARDataLen,ARData,m_RWID.handle);	
//
//	sLogString = _T("");
//	sLogString.Format(_T("EPPay: ReturnCode = 0x%02x"),lCompleteCode);
//	theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//	if(lCompleteCode == RW_RSP_OK){
//		DetailErrCode = 0;
//		sLogString = _T("");
//		sLogString.Format(_T("EPPay: SerAccnum = %d UDDatalen =%d ARDataLen = %d"),SerAccnum,UDDatalen,ARDataLen);
//		theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//		if(UDDatalen>0){
//			sLogString = _T("EPPay: UDData =");
//			theRW_TRACE->WriteData(_T("<")+sLogString,UDData,UDDatalen);
//		}
//
//		if(ARDataLen>0){
//			sLogString = _T("");
//			sLogString.Format(_T("EPPay: ARData.ARnum = %d"),ARData.ARnum);
//			theRW_TRACE->WriteData(_T("<")+sLogString,NULL,0);
//
//			sLogString = _T("EPPay: ARData.ARData =");
//			theRW_TRACE->WriteData(_T("<")+sLogString,ARData.ARdata,ARData.ARnum*ONE_ARDATA_LEN);
//		}
//
//		sLogString = _T("");
//		sLogString.Format(_T("EPPay: TranResult ="));
//		theRW_TRACE->WriteData(_T("<")+sLogString,(LPBYTE)&TranResult,sizeof(TranResult));
//	}
//	else{
//		if(IsDcr4000ReturnCode(lCompleteCode))
//		{
//			DetailErrCode = lCompleteCode;
//		}
//		else
//		{	
//			GetErrCode(DetailErrCode);
//		}
//	}
//	return  lCompleteCode;
//}