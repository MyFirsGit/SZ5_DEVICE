#ifndef __RWCOMMANDS_H__
#define __RWCOMMANDS_H__

#pragma once
#include "StdAfx.h"
#include "RWCOMLIBExp.h"
#include "RWCOMLIB_DEF.h"
#include "Command.h"
#include "CommBase.h"
#include "DCR4000Comm.h"
#pragma comment(lib,"DCR4000Comm.lib")
/** 
@brief   RW command 类
*/
typedef struct _tagRWID
{
	HANDLE handle;
	CString name;
	BYTE status;
	RW_TYPE rwtype;
	_tagRWID()
	{
		handle = INVALID_HANDLE_VALUE;
		name = "RW";
		status = 0;
	}
	_tagRWID(HANDLE handle,CString name,RW_TYPE rwtype)
	{
		this->handle = handle;
		this->name = name;
		this->rwtype = rwtype;
		status = 0;
	}
} RWID;

class RWCOMLIB_DECLSPEC CRWCommands : public CCommand
{		
public:
	CRWCommands(RWID& rwid);
	~CRWCommands();
	// 通用命令
	virtual long Open(BYTE,DWORD,BOOL);
	virtual long Close();
	virtual long Reset(BYTE&);
	virtual long GetStatus(BYTE&);
	virtual long Abort(BYTE&);
	virtual long Stop(BYTE&);
	virtual long SetTime(BYTE*,BYTE*);
	virtual long GetTime(BYTE*);
	virtual long GetMac(BYTE*,BYTE*,BYTE*); 
	virtual long GetErrCode(int&);
	virtual long GetTPUInfo(DAT::TPUINFO&);
	virtual long ProgramDownload_Main(WORD,BYTE*);  
	virtual long ProgramUpdate_Main();
	virtual long ProgramDownload_App(WORD,BYTE*);  
	virtual long ProgramUpdate_App();

	// 功能命令
	virtual long InitRW(DAT::INITSTRUCT,BYTE&);
	virtual long GetTicketPrice(DAT::TICKETINFO,DWORD&,int&);
	//virtual long GetTicketTax(DAT::TAXINFO TicketInfo,DWORD& TicketTax,int& DetailErrCode);
	virtual long GetParameterInfo(WORD&,DAT::PARAINFO*);
	virtual long DownloadParamter(WORD,BYTE,WORD,WORD,BYTE*,WORD&);
	virtual long SetOperationMode(BYTE ControlCode,BYTE *ModeCode,BYTE &statue);
	virtual long SetBusinessTime(uchar ControlCode,ulong Time,uchar &status);
	virtual long GetUDData(BYTE &SerAccnum,DAT::TRANRESULT &TranResult,WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData);
	//virtual long AuthIssueCardMasterCard(DAT::Struct_AuthIssueCardMasterCard,BYTE&);

	// 业务命令
	virtual long ReadCard(DAT::pCardReadMisc_t,DAT::CARDINFO&,int&);
	virtual long Issue(DWORD &SerialNo,BYTE* TranDateTime,DAT::SALEINFO SaleInfo,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
		WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int&);
	virtual long Adjust(DWORD &SerialNo,BYTE* TranDateTime, DAT::COMPENSATIONPAID info,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
		WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int&);
	virtual long Charge(DWORD &SerialNo,BYTE* TranDateTime,char* szLocalID, DAT::REVALUEINFO ChargeInfo,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
		WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int&);
	virtual long Refund(DWORD &SerialNo,BYTE* TranDateTime,DAT::SURRENDERINFO SaleInfo,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
		WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int&);
	virtual long Block(DWORD &SerialNo,BYTE* TranDateTime, BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
		WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int&);
	//virtual long Counteract(DWORD &SerialNo,BYTE* TranDateTime, BYTE CardType,WORD ProductyType,BYTE* ReasonCode,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
	//				WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int&);     
	virtual long Active(DWORD &SerialNo,BYTE* TranDateTime,BYTE CardType,WORD ProductyType,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
		WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode);
	virtual long Defer(DAT::STT_DEFERCARD sttDefer,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
		WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode);
	virtual long ReplaceNewCard(DWORD &SerialNo,BYTE* TranDateTime,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
		WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode);
	virtual long ReplaceOldCard(DWORD &SerialNo,BYTE* TranDateTime,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
		WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode);
	/*virtual long ReplaceNoCard(DWORD &SerialNo,BYTE* TranDateTime,DAT::STRUCT_NocardOrBadcardTransData transInfo,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
		WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode);*/
	virtual long ChargeOverTime(DWORD SerialNo,byte *pTime,byte byDo,uchar &bySerAccnum,DAT::TRANRESULT &sttTranRlt,ushort &sArgUdLen,uchar *pArgUdBuf,ushort &sArgArLen,DAT::ARDATA &sttArgAr,int &DetailErrCode);
	virtual long SetCheck(byte *pTime,byte byIsCheck,byte& byTpuState,int &DetailErrCode);
	virtual long UpdateSignCardInfo(DAT::STT_RENMCD_CHG SignCardInfo,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
		WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode);
	//virtual long Personalize(DWORD &SerialNo,BYTE* TranDateTime,DAT::Struct_Individuation IndividuationData,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode);
	//virtual long PersonalizeUpdateNewCard(DWORD &SerialNo,BYTE* TranDateTime,DAT::Struct_IndivRenewNewCard IndividuationData,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
		//WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode);
	//virtual long PersonalizeUpdateOldCard(DWORD &SerialNo,BYTE* TranDateTime,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
	//	WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int &DetailErrCode);
	//virtual long ReleaseEs(ulong dwUdsn,				/*in,一卡通交易流水号*/
	//	uchar *TranDateTime,			/*in,交易时间*/
	//	DAT::Struct_ReleaseES &ReleaseData,	/*in,卡片信息*/
	//	uchar &SerAccnum,				/*out,交易流水号累加值*/
	//	DAT::TRANRESULT &TranResult,			/*out,返回的 tpu 执行状态*/
	//	ushort &sArgUdLen,				/*out,　*/
	//	uchar *pArgUdBuf,				/*out,*/
	//	ushort &sArgArLen,				/*out,　*/
	//	DAT::ARDATA &sttArgAr,				/*out,　*/
	//	int DetailErrorCode		/*out,　*/
	//	);
	//virtual long IndividuationPm(ulong dwUdsn,							/*in,一卡通交易流水号*/
	//	uchar *TranDateTime,					/*in,交易时间*/
	//	DAT::Struct_Individuation &IndividuationData,/*in,卡片信息*/
	//	uchar &SerAccnum,						/*out,交易流水号累加值*/
	//	DAT::TRANRESULT &TranResult,					/*out,返回的 tpu 执行状态*/
	//	ushort &sArgUdLen,						/*out,　*/
	//	uchar *pArgUdBuf,						/*out,*/
	//	ushort &sArgArLen,						/*out,　*/
	//	DAT::ARDATA &sttArgAr,						/*out,　*/
	//	int DetailErrorCode					/*out,　*/
	//	);
	//// EP 支付命令
	//virtual long ConsumeEPPay(DWORD &SerialNo,BYTE* TranDateTime,DAT::Struct_Consume &inStConsume,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
	//	WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int&);

	bool IsDcr4000ReturnCode(int returnCode);
protected:
	virtual long ExecuteCommand();
	virtual long IsValidCommand();

	RWID& m_RWID;
	static const int BMAC_M1_INFO_LEN = 110;                  // 一卡通M1信息长度 
	static const int ACC_UL_INFO_LEN = 37;                    // UL票的信息长度
	static const int ACC_SV_INFO_LEN = 106;                   // SV票的信息长度

	static const int ACC_RECHARGE_PRODUCT_LEN = 9;           // 读卡器返回票卡当前存在的产品结构长度
	static const int ACC_UL_TRANS_LOG_LEN = 20;               // 读卡器返回单条交易记录长度
	static const int TRANS_TIME_LEN = 7;					  // 交易时间长度
	static const int ONE_ARDATA_LEN = 6;					  // 读写器返回单条寄存器数据长度
	static const int PROGRAM_DATA_BLOCK_LEN = 1024;			  // 程序下载数据块的长度

private:
	//void //ReadSimDataFile(CString fileName, VARIABLE_DATA& filedata);
};
#endif