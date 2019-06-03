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
@brief   RW command ��
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
	// ͨ������
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

	// ��������
	virtual long InitRW(DAT::INITSTRUCT,BYTE&);
	virtual long GetTicketPrice(DAT::TICKETINFO,DWORD&,int&);
	//virtual long GetTicketTax(DAT::TAXINFO TicketInfo,DWORD& TicketTax,int& DetailErrCode);
	virtual long GetParameterInfo(WORD&,DAT::PARAINFO*);
	virtual long DownloadParamter(WORD,BYTE,WORD,WORD,BYTE*,WORD&);
	virtual long SetOperationMode(BYTE ControlCode,BYTE *ModeCode,BYTE &statue);
	virtual long SetBusinessTime(uchar ControlCode,ulong Time,uchar &status);
	virtual long GetUDData(BYTE &SerAccnum,DAT::TRANRESULT &TranResult,WORD &UDDatalen,BYTE* UDData,WORD &ARDataLen,DAT::ARDATA &ARData);
	//virtual long AuthIssueCardMasterCard(DAT::Struct_AuthIssueCardMasterCard,BYTE&);

	// ҵ������
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
	//virtual long ReleaseEs(ulong dwUdsn,				/*in,һ��ͨ������ˮ��*/
	//	uchar *TranDateTime,			/*in,����ʱ��*/
	//	DAT::Struct_ReleaseES &ReleaseData,	/*in,��Ƭ��Ϣ*/
	//	uchar &SerAccnum,				/*out,������ˮ���ۼ�ֵ*/
	//	DAT::TRANRESULT &TranResult,			/*out,���ص� tpu ִ��״̬*/
	//	ushort &sArgUdLen,				/*out,��*/
	//	uchar *pArgUdBuf,				/*out,*/
	//	ushort &sArgArLen,				/*out,��*/
	//	DAT::ARDATA &sttArgAr,				/*out,��*/
	//	int DetailErrorCode		/*out,��*/
	//	);
	//virtual long IndividuationPm(ulong dwUdsn,							/*in,һ��ͨ������ˮ��*/
	//	uchar *TranDateTime,					/*in,����ʱ��*/
	//	DAT::Struct_Individuation &IndividuationData,/*in,��Ƭ��Ϣ*/
	//	uchar &SerAccnum,						/*out,������ˮ���ۼ�ֵ*/
	//	DAT::TRANRESULT &TranResult,					/*out,���ص� tpu ִ��״̬*/
	//	ushort &sArgUdLen,						/*out,��*/
	//	uchar *pArgUdBuf,						/*out,*/
	//	ushort &sArgArLen,						/*out,��*/
	//	DAT::ARDATA &sttArgAr,						/*out,��*/
	//	int DetailErrorCode					/*out,��*/
	//	);
	//// EP ֧������
	//virtual long ConsumeEPPay(DWORD &SerialNo,BYTE* TranDateTime,DAT::Struct_Consume &inStConsume,BYTE &SerAccnum,DAT::TRANRESULT &TranResult,
	//	WORD &UDDatalen,BYTE *UDData,WORD &ARDataLen,DAT::ARDATA &ARData,int&);

	bool IsDcr4000ReturnCode(int returnCode);
protected:
	virtual long ExecuteCommand();
	virtual long IsValidCommand();

	RWID& m_RWID;
	static const int BMAC_M1_INFO_LEN = 110;                  // һ��ͨM1��Ϣ���� 
	static const int ACC_UL_INFO_LEN = 37;                    // ULƱ����Ϣ����
	static const int ACC_SV_INFO_LEN = 106;                   // SVƱ����Ϣ����

	static const int ACC_RECHARGE_PRODUCT_LEN = 9;           // ����������Ʊ����ǰ���ڵĲ�Ʒ�ṹ����
	static const int ACC_UL_TRANS_LOG_LEN = 20;               // ���������ص������׼�¼����
	static const int TRANS_TIME_LEN = 7;					  // ����ʱ�䳤��
	static const int ONE_ARDATA_LEN = 6;					  // ��д�����ص����Ĵ������ݳ���
	static const int PROGRAM_DATA_BLOCK_LEN = 1024;			  // �����������ݿ�ĳ���

private:
	//void //ReadSimDataFile(CString fileName, VARIABLE_DATA& filedata);
};
#endif