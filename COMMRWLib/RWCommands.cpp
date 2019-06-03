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
@brief      CRWCommands构造函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRWCommands::CRWCommands(RWID& rwid):m_RWID(rwid)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CRWCommands析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRWCommands::~CRWCommands()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打开读写器

@param      (i)unsigned long ulport    端口号
@param      (i)unsigned long ulbaud    波特率
@param      (i)BOOL		bIsUsb		   是否使用USB接口通信

@retval     long    成功:0; 失败:错误应答码    

@exception  无
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
@brief      关闭读写器命令

@param      无         

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      复位读写器命令

@param      (o)unsigned char  Statue 读写器状态      

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      读取读写器状态命令

@param      (o)BYTE& status  读写器状态   

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      中止读写器命令

@param      (o)BYTE& status  读写器状态       

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      停止读写器命令

@param      (o)BYTE& status  读写器状态       

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      设置读写器时钟

@param      (i)BYTE* SetTime 设置的时间   
@param      (o)BYTE* TPUTime 设置后TPU时间

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      获取TPU时钟

@param      (o)BYTE *Time      TPU时钟,BCD码,格式为：YYYYMMDDHHMMSS 

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      计算交易认证码(MAC)

@param      (i)BYTE *KeyVer    密钥版本
@param      (i)BYTE *Data      需要计算MAC的Hash数据
@param      (o)BYTE *MAC	   用于存放返回的MAC数据

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      获取TPU错误代码

@param      (o)int &iErrorCode    TPU详细错误代码

@retval     long    成功:0; 失败:错误应答码 

@exception  无
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
@brief      获取TPU信息

@param      (o)DAT::TPUINFO&    TPU信息

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      获取票价

@param      (i)DAT::TICKETINFO TicketInfo   车票信息
@param      (o)DWORD& TicketPrice           票价

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      获取税费

@param      (i)DAT::TAXINFO TicketInfo   车票信息
@param      (o)DWORD& TicketTax           税费

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      获取TPU参数信息

@param      (o)WORD &ParamCount					参数数量
	if(theTVM_INFO.IsRWUsed())
	{
		@param      (o)DAT::PARAINFO *Paraminfo         参数结构指针
	}
	else
	{
	}

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      获取TPU参数信息

@param      (i)uchar* ParaType				参数文件类型,长度为2字节
@param      (i)BYTE bContinue				后续包标志:0x00 表示结束包,0x01 表示有后续包
@param      (i)WORD PacketNumber			数据包编号: 范围0x0000 ~ OxFFFF
@param      (i)WORD PacketLen				数据包信息长度,最大为1024,大于1024字节时分包发送,最后一个数据包按照实际长度发送
@param      (i)BYTE* Data					数据内息，下载的数据内容，最大1024byte
@param      (i)WORD& ReceivePacketNumber	用于存放返回应答中指示应答成功的包序号

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      获取TPU参数信息

@param      (i)DAT::INITSTRUCT	initInfo	初始化信息
			typedef struct _INITSTRUCT {  
				ulong DelayTime;			// 自动ABORT延时时间,单位ms,表示上位机发出读卡指令后，如果在X毫秒内没有检测到卡片，读卡器将自动返回到空闲状态
				uchar DeviceCode[6];		// 设备ID
				uchar DeviceHdType;			// 设备硬件类型 DeviceType_t+DeviceSubType_t
				uchar StationPLInfo[4];		// 车站位置信息StationID_t
				uchar AgentCode;			// 运营公司或参与方 预留
				uchar OperationDate[4];		// 运营日期 YYYYMMDD
				uchar CurrentDate[4];		// 当前日历日期 YYYYMMDD
				uchar DeviceTestMode;		// 设备测试模式
				uchar StationOperationMode; // 车站运营模式 参见ModeCode_t
				uchar OperaterID[4];		// 操作员的编号,参见OperateID_t
				uchar TransferStationFlag;  // 换乘站标志 0x00：不是有障碍换乘车站；0x01：是有障碍换乘车站
				uchar ReaderPlace;			// 读卡器位置
				ushort ErrFareCtrMax;		// 车票连续出错最大张数
				ushort WaitTimeForCardWR;	// 未写入完全的卡等待时间
				ushort RetryTimesForCardWR;	// 未写入完全的卡重试次数
				uchar  AntennaConfig;		// 天线配置 0x00：单天线；0x01：双天线且天线A优先寻卡；0x02：双天线且天线B优先寻卡
				uchar Reserver[4];          // 预留
			} INITSTRUCT; 
@param      (o)BYTE& status	 TPU状态

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      读卡信息命令

@param      (i)DAT::pCardReadMisc_t  
				typedef struct CARDREADMISC{
				//CPU 卡片基本数据
				uchar ValidCheckMark;//合法性检查标志(1 bytes)
				uchar TransCode;//业务功能代码
				uchar AreaPaidMark;//付费区标志
				uchar TransTime[7];//当前时间
				uchar AntennaMark;//天线寻卡标志
				uchar ucReadFlag;//补票前读卡标记
				uchar ucStation[4];//补票前车站站码
				uchar ucGetTransLog;//历史交易读取标记
				}CardReadMisc_t, *pCardReadMisc_t;
@param      (o)DAT::CARDINFO&			RW读卡命令信息    
				typedef struct _RECHARGEPRODUCT
				{
				ushort ProductType;//产品类型
				ushort ProductSubType;//产品子类型
				ulong RefoundMoney;//可退金额
				uchar RechargeMark;//是否可充值
				}RECHARGEPRODUCT;

				typedef struct _CARDINFO
				{
				uchar CardPHType;//车票物理类型
				uchar CardInfoLen;//卡数据长度
				uchar PayAreaReticketCode;//付费区补票建议代码
				uchar NoPayAreaReticketCode;//非服费区补票建议代码
				ulong RepayTicketAmount;//建议补票金额
				//循环
				uchar ProductCount;//产品数量
				RECHARGEPRODUCT RechargeProduct[10];
				//循环结束
				uchar SalableProductType;//可售产品类型
				uchar SalableProductCount;//可售产品数量
				uchar Reserve[20];//保留
				ulong ReturnCardFee;//[4];建议退票手续费金额
				uchar isBigMoneyCard;//是否为大额卡
				uchar AntennaMark; // 天线标志
				uchar CardInfo[512];//卡信息
				uchar ucLogNum;//史交易数据记录数量
				uchar LogInfo[160];//史交易数据
				}CARDINFO;
@param      (o)DWORD &DetailErrCode             TPU详细错误代码

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      锁卡命令(应答代码等于0xA1该卡在黑名单中，但未锁定，SLE可通过该命令对该卡进行锁卡操作)

@param      (i)DWORD &SerialNo				// UDSN或一卡通交易流水号
@param      (i)BYTE*TranDateTime			// 交易发生的日期时间 7BCD
@param      (o)uchar &SerAccnum			    // UDSN或一卡通交易流水号累加值
@param      (o)TRANRESULT &TranResult       // TPU业务处理后返回的结果数据(同售票)
@param      (o)ushort &UDDatalen			// UD数据长度 长度为0表示下面的UD数据不存在
@param      (o)uchar *UDData				// UD数据(同售票)			
@param      (o)ushort &ARDataLen			// AR数据长度 长度为0表示下面的AR数据不存在
@param      (o)ARDATA &ARData				// AR数据(同售票)

@retval     long    成功:0; 失败:错误应答码

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
@brief      抵消命令

@param      (i)DWORD &SerialNo				// UDSN或一卡通交易流水号
@param      (i)BYTE*TranDateTime			// 交易发生的日期时间 7BCD
@param      (i)BYTE CardType  		        // 卡片种类，0x01:一卡通S50/S70卡片；0x02:一卡通CPU卡；0x11:一票通S50/S70卡片；0x12:一票通UL卡
@param      (i)WORD ProductyType			// 产品种类
@param      (i)BYTE *ReasonCode				// 抵消原因
@param      (o)uchar &SerAccnum			    // UDSN或一卡通交易流水号累加值
@param      (o)TRANRESULT &TranResult       // TPU业务处理后返回的结果数据(同售票)
@param      (o)ushort &UDDatalen			// UD数据长度 长度为0表示下面的UD数据不存在
@param      (o)uchar *UDData				// UD数据(同售票)			
@param      (o)ushort &ARDataLen			// AR数据长度 长度为0表示下面的AR数据不存在
@param      (o)ARDATA &ARData				// AR数据(同售票)

@retval     long    成功:0; 失败:错误应答码

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
@brief      激活命令

@param      (i)BYTE*TranDateTime			// 交易发生的日期时间 7BCD
@param      (i)BYTE CardType  		        // 卡片种类，0x01:一卡通S50/S70卡片；0x02:一卡通CPU卡；0x11:一票通S50/S70卡片；0x12:一票通UL卡
@param      (i)WORD ProductyType			// 产品种类
@param      (o)BYTE &Status					// ??

@retval     long    成功:0; 失败:错误应答码

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
@brief      换卡命令（新卡）

@param      (i)DWORD		&SerialNo			// UDSN或一卡通交易流水号
@param      (i)BYTE*		TranDateTime		// 交易发生的日期时间 7BCD
@param      (o)BYTE			&SerAccnum			// UDSN或一卡通交易流水号累加值
@param      (o)TRANRESULT	&TranResult			// TPU业务处理后返回的结果数据(同售票)
@param      (o)WORD			&UDDatalen			// UD数据长度 长度为0表示下面的UD数据不存在
@param      (o)BYTE*		UDData				// UD数据(同售票)			
@param      (o)WORD			&ARDataLen			// AR数据长度 长度为0表示下面的AR数据不存在
@param      (o)ARDATA		&ARData				// AR数据(同售票)

@retval     long    成功:0; 失败:错误应答码

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
@brief      换卡命令（旧卡）

@param      (i)DWORD		&SerialNo			// UDSN或一卡通交易流水号
@param      (i)BYTE*		TranDateTime		// 交易发生的日期时间 7BCD
@param      (o)BYTE			&SerAccnum			// UDSN或一卡通交易流水号累加值
@param      (o)TRANRESULT	&TranResult			// TPU业务处理后返回的结果数据(同售票)
@param      (o)WORD			&UDDatalen			// UD数据长度 长度为0表示下面的UD数据不存在
@param      (o)BYTE*		UDData				// UD数据(同售票)			
@param      (o)WORD			&ARDataLen			// AR数据长度 长度为0表示下面的AR数据不存在
@param      (o)ARDATA		&ARData				// AR数据(同售票)

@retval     long    成功:0; 失败:错误应答码

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
@brief      换卡命令（坏卡或无卡）

@param      (i)DWORD		&SerialNo			// UDSN或一卡通交易流水号
@param      (i)BYTE*		TranDateTime		// 交易发生的日期时间 7BCD
@param      (o)BYTE			&SerAccnum			// UDSN或一卡通交易流水号累加值
@param      (o)TRANRESULT	&TranResult			// TPU业务处理后返回的结果数据(同售票)
@param      (o)WORD			&UDDatalen			// UD数据长度 长度为0表示下面的UD数据不存在
@param      (o)BYTE*		UDData				// UD数据(同售票)			
@param      (o)WORD			&ARDataLen			// AR数据长度 长度为0表示下面的AR数据不存在
@param      (o)ARDATA		&ARData				// AR数据(同售票)

@retval     long    成功:0; 失败:错误应答码

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
@brief      充值命令

@param      (i)DWORD &SerialNo				// UDSN或一卡通交易流水号
@param      (i)BYTE*TranDateTime			// 交易发生的日期时间 7BCD	
@param      (i)char* szLocalID				// 终端机编号
@param      (i)REVALUEINFO ChargeInfo
			typedef struct _REVALUEINFO//充值
			{	
			uchar PayMethed;//付款方式
			uchar BankCardNo[10];
			uchar RevalueType;//充值类型、及票种类型
								0x01 = 一票通钱包产品；
								0x02 = 一票通计次产品；
								0x03 = 一票通定期产品；
								0x04 = 一卡澳门通公共钱包产品；
								0x05 = 澳门一卡通小钱包；
								0x06 = 澳门一卡通学生钱包；
								0x07 = 澳门一卡通定期产品；
			ushort CardType;  // 子产品类型
			ulong Amount;     // 充值的具体金额。传输时转换成INTEL序。
			ulong Count;      // 计次卡充值的次数。传输时转换成INTEL序。
			ulong Period;     // 定期卡有效期延长长度，单位：天(此参数暂时无效，TPU自己处理)
			} REVALUEINFO;
@param      (o)uchar &SerAccnum			    // UDSN或一卡通交易流水号累加值
@param      (o)TRANRESULT &TranResult       // TPU业务处理后返回的结果数据(同售票)
@param      (o)ushort &UDDatalen			// UD数据长度 长度为0表示下面的UD数据不存在
@param      (o)uchar *UDData				// UD数据(同售票)			
@param      (o)ushort &ARDataLen			// AR数据长度 长度为0表示下面的AR数据不存在
@param      (o)ARDATA &ARData				// AR数据(同售票)

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      充值超时处理命令
@param      (i)DWORD SerialNo,			UDSN或一卡通交易流水号
@param      (i)byte *pTime,				交易发生的日期时间
@param      (i)byte byDo,				是否时进行超时处理
@param      (i)HANDLE m_RWID.handle,			标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
@param      (o)uchar &bySerAccnum ,		UDSN或一卡通交易流水号累加值
@param      (o)TRANRESULT &TranResult   TPU业务处理后返回的结果数据
typedef struct __TRANRESULT
{
uchar RetractFlag;			// 票卡回收状态	0x00:不回收;0x01:正常回收;0x02:废票回收;0x03:达到最大次数卡回收;0x04:批次回收
uchar HighBlacklist;        // 高级黑名单标志(预留) 0x00:否;0x01:是
uchar CardType;				// 卡片种类，0x01:一卡通S50/S70卡;0x02:一卡通CPU卡;0x11:一票通S50/S70卡;0x12:一票通UL卡
ushort ProductyCategory;	// 车票产品种类 TicketFamily_t + ProductType
uchar CardNo[4];			// 一卡通卡发行流水号或一票通逻辑卡号
ulong BeforAmount;			// 操作前余额,计次卡填次，定期卡该字段无效。
ulong AfterAmount;			// 操作后余额,计次卡填次，定期卡该字段无效。
uchar ucBlockFlag;			// 票卡锁定状态	0x00:票卡未锁定;0x01:票卡被锁定
uchar Reserved[14];			// 预留
}TRANRESULT;
@param      (o)ushort &sArgUdLen        如果返回UD，用于存放返回UD数据的长度
@param      (o)uchar *pArgUdBuf			如果返回UD，用于存放返回的UD数据，如果无UD返回，为空
@param      (o)ushort &sArgArLen		如果返回，用于存放AR数据长度
@param      (o)ARDATA &sttArgAr			用于存放返回的AR数据，如果无AR返回，为空

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      补票命令

@param      (i)DWORD &SerialNo				// UDSN或一卡通交易流水号
@param      (i)BYTE*TranDateTime			// 交易发生的日期时间 7BCD	
@param      (i)COMPENSATIONPAID AdjustInfo
			typedef struct  _COMPENSATIONPAID{//补票
			                uchar CardTrantype;0x01：//澳门通M1；0x02：澳门通CPU卡片卡；0x11：一票通M1卡片；0x12：一票通UL卡
							ushort ProductyCategory;//车票产品类型 ProductyType;
							uchar PayMethed; 0x01:  //非卡内扣款补票；0x02: 卡内扣款补票； 
							ulong TicketAmount;     //补票金额
							uchar PayAttribute;     //补票区域属性：描述补票发生的区域属性：0x01 = 付费区补票；0x02 = 非付费区补票
							uchar OperationType;    //0x01 =按进站选择（未刷进站）；0x02 =按金额选择（余额不足,包括:超时,超程,补票超时.）；0x03 =选择终点站，输入手续费金额（未刷出站）
							uchar BegainStation[4]; //按进站补票时的进站车站位置信息。传输时转换成INTEL序。
							uchar EndStation[4];    //按进站补票时的出站车站位置信息.传输时转换成INTEL序。
							uchar byPayType;//支付方式
							uchar BankCardNo[10];//银行卡号
							uchar PayCardLogicID[8];
			} COMPENSATIONPAID;
@param      (o)uchar &SerAccnum			    // UDSN或一卡通交易流水号累加值
@param      (o)TRANRESULT &TranResult       // TPU业务处理后返回的结果数据(同售票)
@param      (o)ushort &UDDatalen			// UD数据长度 长度为0表示下面的UD数据不存在
@param      (o)uchar *UDData				// UD数据(同售票)			
@param      (o)ushort &ARDataLen			// AR数据长度 长度为0表示下面的AR数据不存在
@param      (o)ARDATA &ARData				// AR数据(同售票)

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      售卡命令

@param      (i)DWORD &SerialNo			// UDSN或一卡通交易流水号
@param      (i)BYTE*TranDateTime		// 交易发生的日期时间 7BCD	
@param      (i)SALEINFO SaleInfo,
			typedef struct _SALECARD
			{
			uchar CardApplicationType;//卡片种类0x01：澳门通M1；0x02：澳门通CPU卡片卡；	0x11：一票通M1卡片；0x12：一票通UL卡
			uchar ProductyType;//车票产品种类 钱包、定期、计次
			ushort ProductyCategory;//车票产品类型 票种定义。传输时转换成INTEL序。
			ushort ProductSubType;//车票产品子类型
			uchar SaleMode;//售票方式0x01：表示按照票价来售票；		0x02：表示按照起止站来售票
			uchar PassengerType;//乘客类型
			ulong TicketAmount;//售票金额
			uchar BegainStation[4];//起点站（或区段）站码
			uchar EndStation[4];//终点站（或区段）站码
			ushort CountTicketUseCount;//计次类车票使用次数
			ulong ValidityPeriod;//有效期的时间长度
			uchar byPayType;//支付方式
			uchar BankCardNo[10];//银行卡号
			uchar PayCardSerialNumber[8];//支付票卡逻辑卡号
			}SALEINFO;
@param      (o)uchar &SerAccnum			    // UDSN或一卡通交易流水号累加值
@param      (o)TRANRESULT &TranResult       // TPU业务处理后返回的结果数据
			typedef struct __TRANRESULT
			{
				uchar RetractFlag;			// 票卡回收状态	0x00:不回收;0x01:正常回收;0x02:废票回收;0x03:达到最大次数卡回收;0x04:批次回收
				uchar HighBlacklist;        // 高级黑名单标志(预留) 0x00:否;0x01:是
				uchar CardType;				// 卡片种类，0x01:一卡通S50/S70卡;0x02:一卡通CPU卡;0x11:一票通S50/S70卡;0x12:一票通UL卡
				ushort ProductyCategory;	// 车票产品种类 TicketFamily_t + ProductType
				uchar CardNo[4];			// 一卡通卡发行流水号或一票通逻辑卡号
				ulong BeforAmount;			// 操作前余额,计次卡填次，定期卡该字段无效。
				ulong AfterAmount;			// 操作后余额,计次卡填次，定期卡该字段无效。
				uchar ucBlockFlag;			// 票卡锁定状态	0x00:票卡未锁定;0x01:票卡被锁定
				uchar Reserved[14];			// 预留
			}TRANRESULT;
@param      (o)ushort &UDDatalen			// UD数据长度 长度为0表示下面的UD数据不存在
@param      (o)uchar *UDData				// UD数据
			1	UD数据记录个数	1	HEX	
			2	UD数据记录体	
				UD数据记录体的结构如下：
				序号	字段	      字节数	编码	备注
				2.1	  交易数据类别	   1	    HEX	    0x01:一卡通交易 0x02:一票通交易
				2.2	  交易数据长度	   2	    HEX	
				2.3	  交易数据			
			
@param      (o)ushort &ARDataLen			// AR数据长度 长度为0表示下面的AR数据不存在
@param      (o)ARDATA &ARData				// AR数据
			typedef struct __ARDATA 
			{
				uchar ARnum;				// 1.AR数据记录个数
				uchar ARdata[BUF_SIZE];		// 2.AR数据记录体
				序号	字段	  字节数	编码	备注
				2.1	 设备寄存器ID	2	    HEX	   与TPU相关的设备寄存器ID
				2.2	 AR数据增加量	4	    HEX	   SLE根据AR数据增加量累加设备内保存的AR数据
			}ARDATA;
				
@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      售卡初检命令

@param      (i)byte *pTime, 日期时间
@param      (i)WORD wTimeLen, 时期时间的长度
@param      (i)byte byIsCheck, 是否进行初检
@param      (o)byte byTpuState  返回tpu的状态码
@param      (o)int &DetailErrCode  返回tpu的错误码

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      退款命令

@param      (i)DWORD &SerialNo			// UDSN或一卡通交易流水号
@param      (i)BYTE*TranDateTime		// 交易发生的日期时间 7BCD	
@param      (i)SURRENDERINFO SaleInfo,
			typedef struct _SURRENDERINFO {
				uchar PayMethed;				// 付款方式(预留)
				uchar CardApplicationType;		// 卡片种类(预留) 0x01:一卡通S50/S70卡片；0x02:一卡通CPU卡；0x11:一票通S50/S70卡片；0x12:一票通UL卡
				ushort ProductyCategory;		// 车票产品种类(预留)
				uchar SurrenderFlag;			// 退卡标志(预留)
				uchar YPTSurrenderOperateType;	// 一票通退卡操作类型(预留)
				uchar YKTSurrenderOperateType;	// 一卡通退卡操作类型(预留)
				uchar ReasoneCode;				// 退卡退资原因代码(预留)
				uchar BadCardNo[16];			// 卡片表面印刷号(预留)
				uchar CardPhyID[7];				// 卡物理ID(预留)
				ulong RefundMoney;				// 实退金额(预留)
				ulong CardBalance;				// 卡内余额(预留)
				ulong CardTransCtr;				// 卡交易计数器(预留)
				uchar FareProductType;			// 车票产品类型(预留)
				ushort FareProductyCategory;	// 车票产品种类(预留)
				ulong CardDeposit;				// 卡内押金(预留)
				ulong ServiceFee;				// 服务费(预留)
				ulong CardCost;					// 卡成本费(预留)
			} SURRENDERINFO;
@param      (o)uchar &SerAccnum			    // UDSN或一卡通交易流水号累加值
@param      (o)TRANRESULT &TranResult       // TPU业务处理后返回的结果数据(同售票)
@param      (o)ushort &UDDatalen			// UD数据长度 长度为0表示下面的UD数据不存在
@param      (o)uchar *UDData				// UD数据(同售票)			
@param      (o)ushort &ARDataLen			// AR数据长度 长度为0表示下面的AR数据不存在
@param      (o)ARDATA &ARData				// AR数据(同售票)
			
@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      主控程序下载命令

@param      (i)WORD PacketNumber 数据包编号,第一个包从0x0000开始，后续依次累加，最后一个包填OxFFFF
@param      (i)BYTE* Data        数据内息，下载的数据内容，最大1024byte,最后一块若不足1024字节，则后面填充0x00   

@retval     long    成功:0; 失败:错误应答码      

@exception  无
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
@brief      主控程序更新命令

@param      无

@retval     long    成功:0; 失败:错误应答码      

@exception  无
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
@brief      应用程序下载命令

@param      (i)WORD PacketNumber 数据包编号,第一个包从0x0000开始，后续依次累加，最后一个包填OxFFFF
@param      (i)BYTE* Data        数据内息，下载的数据内容，最大1024byte,最后一块若不足1024字节，则后面填充0x00   

@retval     long    成功:0; 失败:错误应答码      

@exception  无
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
@brief      应用程序更新命令

@param      无

@retval     long    成功:0; 失败:错误应答码      

@exception  无
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
@brief      设置TPU运营模式

@param      (i)BYTE ControlCode  控制代码:0x01:设置模式;0x02:解除模式
@param      (i)uchar *ModeCode   模式代码，长度为2个字节
@param      (o)uchar &statue     TPU状态

@retval     long    成功:0; 失败:错误应答码      

@exception  无
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
@brief      获取UD数据命令

@param      (o)uchar &SerAccnum			    // UDSN或一卡通交易流水号累加值
@param      (o)TRANRESULT &TranResult       // TPU业务处理后返回的结果数据(同售票)
@param      (o)ushort &UDDatalen			// UD数据长度 长度为0表示下面的UD数据不存在
@param      (o)uchar *UDData				// UD数据(同售票)
@param      (o)ushort &ARDataLen			// AR数据长度 长度为0表示下面的AR数据不存在
@param      (o)ARDATA &ARData				// AR数据(同售票)

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      发卡母卡外部认证函数

@param      (i)Struct_AuthIssueCardMasterCard &AuthIssueCardMasterCardData //发卡母卡外部认证数据
@param      (o)uchar &Statue											   //设备状态

@retval     long    成功:0; 失败:错误应答码     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//long CRWCommands::AuthIssueCardMasterCard(DAT::Struct_AuthIssueCardMasterCard authIssueCardMasterCard,BYTE& Statue)
//{
//	SYNC(RW,m_RWID.name);	
//	CString sLogString;
//
//	// 发行卡母卡的卡槽
//	sLogString.Format(_T("AuthIssueCardMasterCard:ICMC_Slot = %d "),authIssueCardMasterCard.ICMC_Slot);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	// 发行卡母卡传输卡的卡槽
//	sLogString.Format(_T("AuthIssueCardMasterCard:ICMCT_Slot = %d "),authIssueCardMasterCard.ICMCT_Slot);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	// PIN码的长度
//	sLogString.Format(_T("AuthIssueCardMasterCard:ICMCT_PIN_Len = %d "),authIssueCardMasterCard.ICMCT_PIN_Len);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	//发行卡母卡传输卡的PIN码
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
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::ExecuteCommand()
{
	return RW_RSP_OK;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     long     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CRWCommands::IsValidCommand()
{
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取模拟数据文件

@param      (i)CString			fileName    文件名
@param      (o)VARIABLE_DATA&	filedata	文件数据内容

@retval     void     

@exception  无
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
@brief      设置TPU 24小时运营或延长运营时间
 
@param      (i)uchar ControlCode 设置的模式   0x01：启动24小时运营；0x02：解除24小时运营；0x10：启动延长运营；
@param      (i)ulong Time 延长的时间

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      延期

@param      (i)BYTE*	TranDateTime	 交易发生的日期时间		7BCD
@param      (i)WORD		DeferTime		有效期顺延的长度，以天为单位
@param      (i)BYTE		&Status			读写器状态

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      记名卡信息修改

@param      (i)DAT::STT_RENMCD_CHG SignCardInfo  记名卡信息
@param      (o)BYTE &Status  返回tpu的状态码
@param      (o)int &DetailErrCode  返回tpu的错误码

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      个性化

@param      (i)DAT::Struct_Individuation IndividuationData  个性化信息
			//个性化交易需传数据
			typedef struct STRUCT_Individuation
			{
			uchar OperitionMode;//操作类型
			ushort CorporationCode;//公司编码
			ulong PersonalNO;//员工编号
			//持卡人姓名
			char CardHolderName[8];
			uchar CredentialsNOType;//证件号码类型
			uchar CredentialsNO[9];//证件号码
			ulong Birthday;//生日
			uchar PassengerLevel;//乘客级别
			uchar Reserve[8];//预留
			uchar PayMethed;//付款方式
			ulong PayAmount;
			uchar BankCardNo[10];
			}Struct_Individuation;
@param      (o)BYTE &Status  返回tpu的状态码
@param      (o)int &DetailErrCode  返回tpu的错误码

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      个性化更新写新卡

@param      (i)DAT::Struct_IndivRenewNewCard IndividuationData   个性化交易需传数据
			typedef struct STRUCT_IndivRenewNewCard
			{
			//ulong DealNo;//交易流水号
			//uchar DealTime[7];//交易时间
			uchar OperitionMode;//操作类型
			ushort CorporationCode;//公司编码
			ulong PersonalNO;//员工编号
			//持卡人姓名
			char CardHolderName[8];
			uchar CredentialsNOType;//证件号码类型
			uchar CredentialsNO[9];//证件号码
			ulong Birthday;//生日
			uchar PassengerLevel;//乘客级别
			uchar Reserve[8];//预留
			uchar PayMethed;//付款方式
			ulong PayAmount;
			uchar BankCardNo[10];
            }Struct_IndivRenewNewCard;
@param      (o)BYTE &Status  返回tpu的状态码
@param      (o)int &DetailErrCode  返回tpu的错误码

@retval     long    成功:0; 失败:错误应答码     

@exception  无
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
@brief      个性化更新命令（旧卡）

@param      (i)DWORD		&SerialNo			// UDSN或一卡通交易流水号
@param      (i)BYTE*		TranDateTime		// 交易发生的日期时间 7BCD
@param      (o)BYTE			&SerAccnum			// UDSN或一卡通交易流水号累加值
@param      (o)TRANRESULT	&TranResult			// TPU业务处理后返回的结果数据(同售票)
@param      (o)WORD			&UDDatalen			// UD数据长度 长度为0表示下面的UD数据不存在
@param      (o)BYTE*		UDData				// UD数据(同售票)			
@param      (o)WORD			&ARDataLen			// AR数据长度 长度为0表示下面的AR数据不存在
@param      (o)ARDATA		&ARData				// AR数据(同售票)

@retval     long    成功:0; 失败:错误应答码

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
@brief      ES发行交易

@param      (i)Struct_ReleaseES &ReleaseData  ES发行交易信息
@param      (o)uchar &Statue  返回tpu的状态码
@param      (o)int &DetailErrCode  返回tpu的错误码

@retval     long    成功:0; 失败:错误应答码     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//long CRWCommands::ReleaseEs(ulong dwUdsn,				/*in,一卡通交易流水号*/
//							uchar *TranDateTime,			/*in,交易时间*/
//							DAT::Struct_ReleaseES &ReleaseData,	/*in,卡片信息*/
//							uchar &SerAccnum,				/*out,交易流水号累加值*/
//							DAT::TRANRESULT &TranResult,			/*out,返回的 tpu 执行状态*/
//							ushort &sArgUdLen,				/*out,　*/
//							uchar *pArgUdBuf,				/*out,*/
//							ushort &sArgArLen,				/*out,　*/
//							DAT::ARDATA &sttArgAr,				/*out,　*/
//							int DetailErrorCode			/*out,　*/
//							)
//{
//	SYNC(RW,m_RWID.name);	
//	CString sLogString;
//
//	// 一卡通交易流水号
//	sLogString.Format(_T("ReleaseEs:dwUdsn = %lu "),dwUdsn);
//	theRW_TRACE->WriteData(_T(">") + sLogString,NULL,0);
//
//	// 交易时间
//	sLogString = _T("ReleaseEs:TranDateTime = ");	
//	theRW_TRACE->WriteData(_T(">") + sLogString,TranDateTime,TRANS_TIME_LEN);
//
//	// uchar OperitionMode;操作类型
//	sLogString.Format(_T("ReleaseEs: ReleaseData.OperitionMode = %d "),ReleaseData.OperitionMode);
//	theRW_TRACE->WriteData(_T(">") + sLogString,NULL,0);
//
//	//初始化，重编码或注销	
//	if ((ReleaseData.OperitionMode == OPERATION_TYPE_INITIALIZATION) || 
//		(ReleaseData.OperitionMode == OPERATION_TYPE_REENCODE) ||
//		(ReleaseData.OperitionMode == OPERATION_TYPE_INITIALIZATION_REENCODE) ||
//		(ReleaseData.OperitionMode == OPERATION_TYPE_REENCODE_PRE_ISSUE) ||
//		(ReleaseData.OperitionMode == OPERATION_TYPE_CANCEL))
//	{			
//		// uchar FormVisionNO;格式版本号
//		sLogString.Format(_T("ReleaseEs: ReleaseData.FormVisionNO = %d "),ReleaseData.FormVisionNO);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar ManufID;厂商ID
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ManufID = %d "),ReleaseData.ManufID);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar KeyEdition;密钥版本
//		sLogString.Format(_T("ReleaseEs: ReleaseData.KeyEdition = %d "),ReleaseData.KeyEdition);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// ushort CityCode;城市代码
//		sLogString.Format(_T("ReleaseEs: ReleaseData.CityCode = %d "),ReleaseData.CityCode);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// ushort TradeCode;行业代码
//		sLogString.Format(_T("ReleaseEs: ReleaseData.TradeCode = %d "),ReleaseData.TradeCode);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar SupplyTime[3];供货时间
//		char szSupplyTime[10] = {0};
//		CString strSupplyTime  = _T("");
//		BCD2PSTR((char*)ReleaseData.SupplyTime,3,szSupplyTime);
//		USES_CONVERSION;
//		sLogString.Format(_T("ReleaseEs: ReleaseData.SupplyTime = %s"),A2T(szSupplyTime));
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// ushort SupplyDatch;供货批次
//		sLogString.Format(_T("ReleaseEs: ReleaseData.SupplyDatch = %d "),ReleaseData.SupplyDatch);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	}	
//	//预赋值
//	if ((ReleaseData.OperitionMode == OPERATION_TYPE_PRE_ISSUE) || 		
//		(ReleaseData.OperitionMode == OPERATION_TYPE_REENCODE_PRE_ISSUE))
//	{
//		// uchar ReleaseID;产品发行商ID
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ReleaseID = %d "),ReleaseData.ReleaseID);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar CardType;卡片物理类型
//		sLogString.Format(_T("ReleaseEs: ReleaseData.CardType = %d "),ReleaseData.CardType);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar ProductCategory;产品类别
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ProductCategory = %d "),ReleaseData.ProductCategory);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar ProductCategory;产品类别
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ProductCategory = %d "),ReleaseData.ProductCategory);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar ProductType;产品类型
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ProductType = %d "),ReleaseData.ProductType);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar ProductSubtype;产品子类型
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ProductSubtype = %d "),ReleaseData.ProductSubtype);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar PassengerType;乘客类型
//		sLogString.Format(_T("ReleaseEs: ReleaseData.PassengerType = %d "),ReleaseData.PassengerType);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// ulong CardAmount;卡内金额
//		sLogString.Format(_T("ReleaseEs: ReleaseData.CardAmount = %lu "),ReleaseData.CardAmount);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// ulong CardTimes;卡内次数
//		sLogString.Format(_T("ReleaseEs: ReleaseData.CardTimes = %lu "),ReleaseData.CardTimes);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar ValidityPeriod;有效期间
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ValidityPeriod = %d "),ReleaseData.ValidityPeriod);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar ValidityPeriodUnit;有效期间单位
//		sLogString.Format(_T("ReleaseEs: ReleaseData.ValidityPeriodUnit = %d "),ReleaseData.ValidityPeriodUnit);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// ulong CardDeposit;押金
//		sLogString.Format(_T("ReleaseEs: ReleaseData.CardDeposit = %lu "),ReleaseData.CardDeposit);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// ulong BookingAmount;售票金额
//		sLogString.Format(_T("ReleaseEs: ReleaseData.BookingAmount = %lu "),ReleaseData.BookingAmount);
//		theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//		// uchar VolidStartTime[7];有效开始日期
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
@brief      个性化交易

@param      (i)Struct_Individuation &IndividuationData 个性化交易信息
@param      (o)uchar &Statue  返回tpu的状态码
@param      (o)int &DetailErrCode  返回tpu的错误码

@retval     long    成功:0; 失败:错误应答码     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//long CRWCommands::IndividuationPm(ulong dwUdsn,								/*in,一卡通交易流水号*/
//								  uchar *TranDateTime,						/*in,交易时间*/
//								  DAT::Struct_Individuation &IndividuationData,	/*in,卡片信息*/
//								  uchar &SerAccnum,							/*out,交易流水号累加值*/
//								  DAT::TRANRESULT &TranResult,					/*out,返回的 tpu 执行状态*/
//								  ushort &sArgUdLen,						/*out,　*/
//								  uchar *pArgUdBuf,							/*out,*/
//								  ushort &sArgArLen,						/*out,　*/
//								  DAT::ARDATA &sttArgAr,							/*out,　*/
//								  int DetailErrorCode						/*out,　*/
//								  )
//{
//	SYNC(RW,m_RWID.name);	
//	CString sLogString;
//
//	// 一卡通交易流水号
//	sLogString.Format(_T("ReleaseEs:dwUdsn = %lu "),dwUdsn);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	// 交易时间
//	sLogString.Format(_T("ReleaseEs:TranDateTime = %s "),TranDateTime);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//
//	// uchar OperitionMode;操作类型
//	sLogString.Format(_T("IndividuationPm: IndividuationData.OperitionMode = %c "),IndividuationData.OperitionMode);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	// ushort CorporationCode;公司编码
//	sLogString.Format(_T("IndividuationPm: IndividuationData.CorporationCode = %d "),IndividuationData.CorporationCode);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	// ulong PersonelNO;员工编号
//	sLogString.Format(_T("IndividuationPm: IndividuationData.PersonelNO = %lu "),IndividuationData.PersonalNO);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	// uchar CardHolderName[8];持卡人姓名
//	sLogString.Format(_T("IndividuationPm: IndividuationData.CardHolderName = %s "),IndividuationData.CardHolderName);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	// uchar CredentialsNOType;证件号码类型
//	sLogString.Format(_T("IndividuationPm: IndividuationData.CredentialsNOType = %c "),IndividuationData.CredentialsNOType);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	// uchar CredentialsNO[9];证件号码
//	sLogString.Format(_T("IndividuationPm: IndividuationData.CredentialsNO = %s "),IndividuationData.CredentialsNO);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	// ulong Birthday;生日
//	sLogString.Format(_T("IndividuationPm: IndividuationData.Birthday = %lu "),IndividuationData.Birthday);
//	theRW_TRACE->WriteData(_T(">")+sLogString,NULL,0);
//	// uchar PassengerLevel;乘客级别
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
@brief      EP支付命令

@param      (i)STT_DEPOSIT_EXPENSE inExpactData,
			typedef struct STRUCT_DEPOSIT_EXPENSE
			{
			DWORD uUdsn;
			byte mDateTime[7];
			DWORD uAmount;//消费金额
			STRUCT_DEPOSIT_EXPENSE()
			{
			uUdsn=0;
			memset(mDateTime,0,sizeof(mDateTime));
			uAmount=0;
			}
			} STT_DEPOSIT_EXPENSE;
@param      (o)uchar &SerAccnum			    // UDSN或一卡通交易流水号累加值
@param      (o)TRANRESULT &TranResult       // TPU业务处理后返回的结果数据
			typedef struct __TRANRESULT
			{
			uchar RetractFlag;			// 票卡回收状态	0x00:不回收;0x01:正常回收;0x02:废票回收;0x03:达到最大次数卡回收;0x04:批次回收
			uchar HighBlacklist;        // 高级黑名单标志(预留) 0x00:否;0x01:是
			uchar CardType;				// 卡片种类，0x01:一卡通S50/S70卡;0x02:一卡通CPU卡;0x11:一票通S50/S70卡;0x12:一票通UL卡
			ushort ProductyCategory;	// 车票产品种类 TicketFamily_t + ProductType
			uchar CardNo[4];			// 一卡通卡发行流水号或一票通逻辑卡号
			ulong BeforAmount;			// 操作前余额,计次卡填次，定期卡该字段无效。
			ulong AfterAmount;			// 操作后余额,计次卡填次，定期卡该字段无效。
			uchar ucBlockFlag;			// 票卡锁定状态	0x00:票卡未锁定;0x01:票卡被锁定
			uchar Reserved[14];			// 预留
			}TRANRESULT;
@param      (o)ushort &UDDatalen			// UD数据长度 长度为0表示下面的UD数据不存在
@param      (o)uchar *UDData				// UD数据
1	UD数据记录个数	1	HEX	
2	UD数据记录体	
UD数据记录体的结构如下：
序号	字段	      字节数	编码	备注
2.1	  交易数据类别	   1	    HEX	    0x01:一卡通交易 0x02:一票通交易
2.2	  交易数据长度	   2	    HEX	
2.3	  交易数据			

@param      (o)ushort &ARDataLen			// AR数据长度 长度为0表示下面的AR数据不存在
@param      (o)ARDATA &ARData				// AR数据
			typedef struct __ARDATA 
			{
			uchar ARnum;				// 1.AR数据记录个数
			uchar ARdata[BUF_SIZE];		// 2.AR数据记录体
			序号	字段	  字节数	编码	备注
			2.1	 设备寄存器ID	2	    HEX	   与TPU相关的设备寄存器ID
			2.2	 AR数据增加量	4	    HEX	   SLE根据AR数据增加量累加设备内保存的AR数据
			}ARDATA;
@retval     long    成功:0; 失败:错误应答码  

@exception  无
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
//	// EP 支付扣款
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