#include "StdAfx.h"
#include "TOKENCommands.h"
#include "DebugLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define theTOKEN_TRACE CFileLog::GetInstance(FILTER_TOKEN_LOG)

//////////////////////////////////////////////////////////////////////////
/**
@brief      CTOKENCommands���캯��

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCommands::CTOKENCommands()
{
	hInst = NULL;
	hInst = ::LoadLibraryA("ToIMApi.dll");
	if (hInst == NULL)
	{
		CString sLogString(_T(""));
		sLogString.Format(_T("ToIMApi.dll����ʧ�ܣ�"));
		theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);
	}
	else
	{
		TIM_CommOpen = (pToIM_CommOpen)GetProcAddress(hInst,"ToIM_CommOpen");
		TIM_CommClose = (pToIM_CommClose)GetProcAddress(hInst,"ToIM_CommClose");
		TIM_Init =  (pToIM_Init)GetProcAddress(hInst,"ToIM_Init");
		TIM_Reset = (poTIM_Reset)GetProcAddress(hInst,"ToIM_Reset");
		TIM_GetVersion = (pToIM_ReadVersion)GetProcAddress(hInst,"ToIM_ReadVersion");
		TIM_GetDevStatus = (pToIM_GetDevStatus)GetProcAddress(hInst,"ToIM_ReadStatus");
		TIM_CardOut = (pToIM_TokenToAntenna)GetProcAddress(hInst,"ToIM_TokenToAntenna");
		TIM_SendCard = (pToIM_TokenToOutlet)GetProcAddress(hInst,"ToIM_TokenToOutlet");
		TIM_RetractCard = (pToIM_TokenRetrieve)GetProcAddress(hInst,"ToIM_TokenRetrieve");
		TIM_CleanOut = (pToIM_CleanOut)GetProcAddress(hInst,"ToIM_CleanOut");
		TIM_StopCleanOut = (pToIM_StopCleanOut)GetProcAddress(hInst,"ToIM_StopCleanOut");
		TIM_GetNumOfCleanOut = (pToIM_GetNumOfCleanOut)GetProcAddress(hInst,"ToIM_GetNumOfCleanOut");
		TIM_RfidWriteBlock = (pToIM_RfidWriteBlock)GetProcAddress(hInst,"ToIM_RfidWriteBlock");
		TIM_RfidReadBlock = (pToIM_RfidReadBlock)GetProcAddress(hInst,"ToIM_RfidReadBlock");
		TIM_RfidWriteSector = (pToIM_RfidWriteSector)GetProcAddress(hInst,"ToIM_RfidWriteSector");
		TIM_RfidReadSector = (pToIM_RfidReadSector)GetProcAddress(hInst,"RfidReadSector");
		TIM_RfidReadID = (pToIM_RfidReadID)GetProcAddress(hInst,"ToIM_RfidReadID");
		TIM_SensorCheck = (pToIM_SensorCheck)GetProcAddress(hInst,"ToIM_SensorCheck");
		TIM_GetHopperVersion = (pToIM_GetHopperVersion)GetProcAddress(hInst,"ToIM_GetHopperVersion");
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CTOKENCommands��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCommands::~CTOKENCommands()
{
}

// �׸���API��װ
//////////////////////////////////////////////////////////////////////////
/**
@brief      1.1.5 ͨѶ����

@param      (i)UINT uiCommPort    �˿ں�
@param      (i)UINT uiBaudRate    ������

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::CommOpen(UINT uiCommPort, UINT uiBaudRate)
{
	long lCompleteCode = TOKEN_RSP_OK;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_CommOpen: uiCommPort = %d; uiBaudRate = %d"), uiCommPort, uiBaudRate);
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);
	
	CStringA strCommPort;
	strCommPort.Format("COM%d", uiCommPort);
	lCompleteCode = TIM_CommOpen(strCommPort.GetBuffer(), uiBaudRate);
	
	sLogString = _T("");
	sLogString.Format(_T("TIM_CommOpen: ReturnCode = 0x%02x"),lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	
    return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      1.1.6 ģ���ʼ��

@param      (i)char cClearMode    Ʊ������ʽ
@param      (o)UINT* uiRetractNum    ���β�������Ŀ�Ƭ��

@retval     long    �ɹ�:0; ʧ��:���ش�����  

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::Init(uchar ucActionMode, uchar &ucRetriveNum)
{
	long lCompleteCode = TOKEN_RSP_OK;
	//uchar ucActionMode = 1;
	uchar ucErrorCode = 0;
	TOKEN_NAMESPACE::ModuleStatus xModuleStatus;
	uchar ucTokenNum = 0;

	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_Init: cClearMode = %d"), ucActionMode);
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);
	
	int retCode = TIM_Init(ucActionMode, ucErrorCode, xModuleStatus, ucRetriveNum);

	sLogString.Format(_T("TIM_Init: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);
	if (lCompleteCode==TOKEN_RSP_NG){
		sLogString = _T("");
		sLogString.Format(_T("TIM_GetVersion: ucRetriveNum = %d"), ucRetriveNum);
		theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	}

	sLogString.Format(_T("TIM_Init: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	
    return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      1.1.7 ģ�鸴λ

@param      void

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::Reset()
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;

	CString sLogString(_T("TIM_Reset:"));
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);
	
	int retCode = TIM_Reset(ucErrorCode);
	
	sLogString.Format(_T("TIM_Reset: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);

	sLogString.Format(_T("TIM_Reset: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	
    return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      1.1.8 ��ȡ�豸�����汾��Ϣ

@param      ModuleInfo & xModuleInfo
@param      

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::GetVersion(TOKEN_NAMESPACE::ModuleInfo & xModuleInfo)
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;
	
	CString sLogString(_T("TIM_GetVersion"));
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);
	
	int retCode = TIM_GetVersion(ucErrorCode, xModuleInfo);

	sLogString.Format(_T("TIM_GetVersion: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);
	if (lCompleteCode==TOKEN_RSP_NG){
		sLogString = _T("");
		sLogString.Format(_T("TIM_GetVersion: xModuleInfo.ucFwVer = %s, xModuleInfo.ucFwVer = %s"), xModuleInfo.ucModNum, xModuleInfo.ucFwVer);
		theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	}

	sLogString.Format(_T("TIM_GetVersion: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	
    return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      1.1.9 ��ȡ�豸״̬

@param      void

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::GetDevStatus(TOKEN_NAMESPACE::ModuleStatus& xModuleStatus)
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;

	CString sLogString(_T("TIM_GetDevStatus"));
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);
	
	int retCode = TIM_GetDevStatus(ucErrorCode, xModuleStatus);

	sLogString.Format(_T("TIM_GetDevStatus: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);

	sLogString.Format(_T("TIM_GetDevStatus: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	
    return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      1.1.10 ��ȡƱ��

@param      (i)char cBoxNo    Ʊ���
@param      (o)uchar & ucTokenNum

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::CardOut(uchar ucBoxNo, uchar & ucTokenNum)
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;
	TOKEN_NAMESPACE::ModuleStatus xModuleStatus;

	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_CardOut: ucBoxNo = %d"), ucBoxNo);
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);
	
	int retCode = TIM_CardOut(ucBoxNo, ucErrorCode, xModuleStatus, ucTokenNum);

	sLogString.Format(_T("TIM_CardOut: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);
	if (lCompleteCode==TOKEN_RSP_OK){
		sLogString = _T("");
		sLogString.Format(_T("TIM_CardOut: ucTokenNum = %d"),ucTokenNum);
		theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	}

	sLogString.Format(_T("TIM_CardOut: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	
    return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      1.1.11 �ͳ�Ʊ��

@param      (o)uchar& ucTokenNum    ���β������͵Ŀ�Ƭ��

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::SendCard(uchar & ucTokenNum)
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;
	TOKEN_NAMESPACE::ModuleStatus xModuleStatus;

	CString sLogString(_T("TIM_SendCard"));
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);
	
	int retCode = TIM_SendCard(ucErrorCode, xModuleStatus, ucTokenNum);
	sLogString.Format(_T("TIM_SendCard: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);
	if (lCompleteCode==TOKEN_RSP_OK){
		sLogString = _T("");
		sLogString.Format(_T("TIM_SendCard: ucTokenNum = %d"),ucTokenNum);
		theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	}

	sLogString.Format(_T("TIM_SendCard: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	
    return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      1.1.12 ����Ʊ��

@param      (o)UINT* uiRetractNum    ���λ��յĿ�Ƭ��

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::RetractCard(uchar & ucTokenNum)
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;
	TOKEN_NAMESPACE::ModuleStatus xModuleStatus;

	CString sLogString(_T("TIM_RetractCard"));
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);
	
	int retCode = TIM_RetractCard(ucErrorCode, xModuleStatus, ucTokenNum);
	sLogString.Format(_T("TIM_RetractCard: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);
	if (lCompleteCode==TOKEN_RSP_OK){
		sLogString = _T("");
		sLogString.Format(_T("TIM_RetractCard: ucTokenNum = %d"),ucTokenNum);
		theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	}

	sLogString.Format(_T("TIM_RetractCard: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	
    return  lCompleteCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      1.1.20 �ر�ͨѶ

@param      ��

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::CommClose()
{
	long lCompleteCode = TOKEN_RSP_OK;
	
	CString sLogString(_T("TIM_CommClose"));
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);
	
	lCompleteCode = TIM_CommClose();
	
	sLogString = _T("");
	sLogString.Format(_T("TIM_CommClose: lCompleteCode = 0x%02x"),lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	
    return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      1.1.21 ���Ʊ�䣨���豸�ڵ�token������գ�

@param      (i)uchar cBoxNo   Ʊ���� 

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::CleanOut(uchar cBoxNo)
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_CleanOut: cBoxNo = %d"), cBoxNo);
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);
	
	int retCode = TIM_CleanOut(cBoxNo, ucErrorCode);
	sLogString.Format(_T("TIM_CleanOut: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);

	sLogString.Format(_T("TIM_CleanOut: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	
    return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      1.1.21 ֹͣ���Ʊ��

@param      (i)uchar cBoxNo   Ʊ���� 

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::StopCleanOut(uchar cBoxNo)
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_StopCleanOut: cBoxNo = %d"), cBoxNo);
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	int retCode = TIM_StopCleanOut(cBoxNo, ucErrorCode);
	sLogString.Format(_T("TIM_StopCleanOut: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);

	sLogString.Format(_T("TIM_StopCleanOut: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return  lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      1.1.21 ��ȡƱ���������

@param      (i)uchar cBoxNo   Ʊ����

@param      (i)uchar cBoxNo   Ʊ����

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::GetCleanOutOfNum(uchar cBoxNo, ushort &usCoNum)
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_GetNumOfCleanOut: cBoxNo = %d"), cBoxNo);
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	int retCode = TIM_GetNumOfCleanOut(cBoxNo, ucErrorCode, usCoNum);
	sLogString.Format(_T("TIM_GetNumOfCleanOut: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);
	if (lCompleteCode==TOKEN_RSP_OK){
		sLogString = _T("");
		sLogString.Format(_T("TIM_GetNumOfCleanOut: usCoNum = %d"),usCoNum);
		theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	}

	sLogString.Format(_T("TIM_GetNumOfCleanOut: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return  lCompleteCode;
}


////////////////////////////////////////////////////////////////////////////
/*
@brief      ���ÿ�RFID����

@param      IN uchar ucRfidNo RFID�˿ں�
			IN uchar ucBlockNo Block�� 
			IN BlockData xBlockData Block����

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::RfidWriteBlock(uchar ucRfidNo, uchar ucBlockNo, TOKEN_NAMESPACE::BlockData xBlockData)
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_RfidWriteBlock: ucRfidNo = %d, ucBlockNo = %d, xBlockData = %016x"), ucRfidNo, ucBlockNo, (BYTE*)&xBlockData);
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	int retCode = TIM_RfidWriteBlock(ucRfidNo, ucBlockNo, xBlockData, ucErrorCode);
	sLogString.Format(_T("TIM_RfidWriteBlock: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);

	sLogString.Format(_T("TIM_RfidWriteBlock: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return  lCompleteCode;
}


////////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��RFID����

@param      IN uchar ucRfidNo RFID�˿ں�
			IN uchar ucBlockNo Block�� 
			OUT BlockData xBlockData& Block����

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::RfidReadBlock(uchar ucRfidNo, uchar ucBlockNo, TOKEN_NAMESPACE::BlockData& xBlockData)
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;
	USES_CONVERSION;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_RfidReadBlock: ucRfidNo = %d, ucBlockNo = %d"), ucRfidNo, ucBlockNo);
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	int retCode = TIM_RfidReadBlock(ucRfidNo, ucBlockNo, ucErrorCode, xBlockData);
	sLogString.Format(_T("TIM_RfidReadBlock: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);
	if (lCompleteCode==TOKEN_RSP_OK){
		sLogString = _T("");
		sLogString.Format(_T("TIM_RfidReadBlock: xBlockData = %s"), A2T((char*)&xBlockData));
		theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	}

	sLogString.Format(_T("TIM_RfidReadBlock: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return  lCompleteCode;
}




////////////////////////////////////////////////////////////////////////////
/*
@brief      ��������RFID����

@param      IN uchar ucRfidNo RFID�˿ں�
			IN uchar ucSectorNo Sector�� 
			IN uchar ucLen	���ݳ���
			IN SectorData xSectorData ��������

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::RfidWriteSector(uchar ucRfidNo, uchar ucSectorNo, uchar ucLen, TOKEN_NAMESPACE::SectorData xSectorData)
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;
	CString sLogString(_T(""));
	sLogString.Format(_T("RfidWriteSector: ucRfidNo = %d, ucSectorNo = %d, ucLen = %d, xSectorData = %048x"), ucRfidNo, ucSectorNo, ucLen, (BYTE*)&xSectorData);
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	int retCode = TIM_RfidWriteSector(ucRfidNo, ucSectorNo, ucLen, xSectorData, ucErrorCode);
	sLogString.Format(_T("TIM_RfidWriteSector: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);

	sLogString.Format(_T("TIM_RfidWriteSector: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return  lCompleteCode;
}


////////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ����RFID����

@param      IN uchar ucRfidNo RFID�˿ں�
			IN uchar ucSectorNo Sector�� 
			IN uchar ucLen	���ݳ���
		IN SectorData xSectorData ��������

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::RfidReadSector(uchar ucRfidNo, uchar ucSectorNo, TOKEN_NAMESPACE::SectorData xSectorData)
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_RfidReadSector: ucRfidNo = %d, ucSectorNo = %d"), ucRfidNo, ucSectorNo);
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	int retCode = TIM_RfidReadSector(ucRfidNo, ucSectorNo, ucErrorCode, xSectorData);
	sLogString.Format(_T("TIM_RfidReadSector: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);
	if (lCompleteCode==TOKEN_RSP_OK){
		sLogString = _T("");
		sLogString.Format(_T("TIM_RfidReadSector: xSectorData = %048x"), (BYTE*)&xSectorData);
		theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	}

	sLogString.Format(_T("TIM_RfidReadSector: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return  lCompleteCode;
}



////////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ����RFID����

@param      IN uchar ucRfidNo RFID�˿ں�
			IN uchar* uiRfidID 4�ֽ�ID����

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::RfidReadID(uchar ucRfidNo, uchar* uiRfidID)
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_RfidReadID: ucRfidNo = %d"), ucRfidNo);
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	int retCode = TIM_RfidReadID(ucRfidNo, ucErrorCode, uiRfidID);
	sLogString.Format(_T("TIM_RfidReadID: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);
	if (lCompleteCode==TOKEN_RSP_OK){
		sLogString = _T("");
		sLogString.Format(_T("TIM_RfidReadID: uiRfidID = %04x"), uiRfidID);
		theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	}

	sLogString.Format(_T("TIM_RfidReadID: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return  lCompleteCode;
}


////////////////////////////////////////////////////////////////////////////
/*
@brief      ���������

@param      OUT char* SensorData	3���ֽڴ�������Ϣ

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::SensorCheck(char* sSensorData)
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;
	CString sLogString(_T("TIM_SensorCheck:"));
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	int retCode = TIM_SensorCheck(ucErrorCode, sSensorData);
	sLogString.Format(_T("TIM_SensorCheck: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);
	if (lCompleteCode==TOKEN_RSP_OK){
		sLogString = _T("");
		sLogString.Format(_T("TIM_SensorCheck: sSensorData = %03x"), sSensorData);
		theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	}

	sLogString.Format(_T("TIM_SensorCheck: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return  lCompleteCode;
}



////////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡHopper�汾

@param      OUT HopperModuleInfo& xModuleInfo	Hopper�汾��Ϣ

@retval     long    �ɹ�:0; ʧ��:1    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::GetHopperVersion(TOKEN_NAMESPACE::HopperModuleInfo& xModuleInfo)
{
	long lCompleteCode = TOKEN_RSP_OK;
	uchar ucErrorCode = 0;
	CString sLogString(_T("TIM_GetHopperVersion:"));
	theTOKEN_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	int retCode = TIM_GetHopperVersion(ucErrorCode, xModuleInfo);
	sLogString.Format(_T("TIM_GetHopperVersion: retCode = 0x%02x, ucErrorCode = 0x%02x"), retCode, ucErrorCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	lCompleteCode = DealReturnCode(retCode, ucErrorCode);
	if (lCompleteCode==TOKEN_RSP_OK){
		sLogString = _T("");
		sLogString.Format(_T("TIM_GetHopperVersion: xModuleInfo = %032x"), (BYTE*)&xModuleInfo);
		theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	}

	sLogString.Format(_T("TIM_GetHopperVersion: lCompleteCode = 0x%02x"), lCompleteCode);
	theTOKEN_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return  lCompleteCode;
}


////////////////////////////////////////////////////////////////////////////
///**
//@brief      ����������
//
//@param      long lReturnCode    ��������         
//
//@retval     long    �ɹ�:0; ʧ��:����Ӧ����         
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
long CTOKENCommands::DealReturnCode(long retCode, uchar ucErrorCode)
{
	long lRet = 0;
	if ('s'==retCode){
		lRet = TOKEN_RSP_OK;
	}
	else/* if ('e'==retCode || 'w'==retCode)*/{
		if (ucErrorCode!=0){
			lRet = ucErrorCode;
		}
		else{
			lRet = CTOKENHardwareException::TOKEN_ERR_ACK;
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::ExecuteCommand()
{
	return TOKEN_RSP_OK;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     long     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENCommands::IsValidCommand()
{
	return SP_SUCCESS;
}
