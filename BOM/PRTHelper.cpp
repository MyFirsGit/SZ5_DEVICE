#include "StdAfx.h"
#include "PRTHelper.h"
#include "bomdef.h"
#include "FTPDef.h"
#include "PrinterCommand.h"
#include "Sync.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
@brief	PRTHelper��
*/

#define thePRT_LOG CFileLog::GetInstance(FILTER_PRT_LOG)

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
PRTHelper::PRTHelper(CService &service):CServiceHelper(service)
{
	ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + _T("PrintHistory\\UnPrint"));
	ComCheckAndCreateDirectory(theBOM_INFO.GetDataPath() + _T("PrintHistory\\ChargePrint"));	
	//lastPrintFileFullPath = theBOM_INFO.GetDataPath()+"PrintHistory\\LastPrint.txt";
	m_lastChargePrintFile = theBOM_INFO.GetDataPath() + _T("PrintHistory\\ChargePrint\\lastChargePrint.txt");

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
PRTHelper::~PRTHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �жϴ�ӡ���Ƿ�����

@param      none

@retval     bool true:�쳣 false:���� 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool PRTHelper::IsPrinterHasException()
{
	return theEXCEPTION_MGR.HasException(CPrinterException::MODULE_ID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ӡ����

@param      (i)CStringArray* sArray ��ӡ����
@param      (i)bool bOpenBox = false���Ƿ��Ǯ��

@retval      ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void PRTHelper::PrinterPrint(CStringArray* sArray , bool bOpenBox)
{

	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
	int count = sArray->GetCount();
	for(int i=0;i<count;i++)
	{
		thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,sArray->GetAt(i));
	}
	if(!theBOM_INFO.IsPrinterUsed()){		
		PrinterToFile(sArray,bOpenBox);
	}
	else if(IsPrinterHasException()){
		SaveToUnPrintFile(sArray,bOpenBox);
		if(bOpenBox)
		{
			try{
				PrinterOpenBox();
			}catch(CSysException&)
			{

			}
		}
	}
	else{
		try
		{
			CheckStatus();
			CPrinterPrint pPrinterPrint(sArray, bOpenBox);
			g_pPrinterControl->DoCommand(&pPrinterPrint);
			long errCode = WaitForCommandComplete(&pPrinterPrint);
			if (E_NO_ERROR != errCode) {
				throw CPrinterException((CPrinterException::DETAIL_ERROR_CODE)errCode, _T(__FILE__), __LINE__);
			}
			//SaveToLastPrintFile(sArray);
		}
		catch(CSysException& e)
		{
			SaveToUnPrintFile(sArray,bOpenBox);
			theEXCEPTION_MGR.ProcessException(e);
		}

	}
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("->"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����ӡ��״̬

@param     ��

@retval     ��

@exception  CPrinterException
*/
//////////////////////////////////////////////////////////////////////////
void PRTHelper::CheckStatus()
{
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
	if(!theBOM_INFO.IsPrinterUsed()){		
		return ;
	}
	else{
		CPrinterGetStatus printerGetStatus;
		g_pPrinterControl->DoCommand(&printerGetStatus);
		long errCode = WaitForCommandComplete(&printerGetStatus);
		if (E_NO_ERROR != errCode) {
			throw CPrinterException((CPrinterException::DETAIL_ERROR_CODE)errCode, _T(__FILE__), __LINE__);
		}
	}
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("->"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ��ӡ��ID

@param       (o)BYTE* printerId  ��ӡ��ID  6 Byte

@retval      ��

@exception  CPrinterException
*/
//////////////////////////////////////////////////////////////////////////
long PRTHelper::GetPrinterID(BYTE* printerId)
{
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
	if(printerId == NULL){
		throw CPrinterException(CPrinterException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
	}
	if(theBOM_INFO.IsPrinterUsed()){	
		CPrinterGetInfo printerGetInfo;
		g_pPrinterControl->DoCommand(&printerGetInfo);
		long errCode = WaitForCommandComplete(&printerGetInfo);
		if(E_NO_ERROR != errCode){
			throw CPrinterException((CPrinterException::DETAIL_ERROR_CODE)errCode, _T(__FILE__), __LINE__);
		}
		else{
			printerGetInfo.GetRWResponse(printerId);
		}
		return errCode;
	}
	else{
		memset(printerId,0xFF,6);
		CStringArray *sArray = new CStringArray();
		sArray->Add(_T("Get Printer ID"));
		PrinterToFile(sArray);
		delete sArray;
		sArray = NULL;
		return E_NO_ERROR;
	}
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("->"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���Ӵ�ӡ������

@param      (i)int nPort �˿ں�
@param      (i)int nBaud ������

@retval     long

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long PRTHelper::PrinterConnect(int nPort, int nBaud)
{
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
	if(theBOM_INFO.IsPrinterUsed()){
		CPrinterConnect pPrinterCon(nPort, nBaud);
		g_pPrinterControl->DoCommand(&pPrinterCon);
		// �ȴ���Ӧ
		long errCode = WaitForCommandComplete(&pPrinterCon);

		if (E_NO_ERROR != errCode) {
			throw CPrinterException(CPrinterException::ERROR_Printer_OPEN_COM, _T(__FILE__), __LINE__);		
		}
		return errCode;
	}
	else{
		CStringArray *sArray = new CStringArray();
		sArray->Add(_T("Open Printer"));
		PrinterToFile(sArray);
		delete sArray;
		sArray = NULL;
		return E_NO_ERROR;
	}
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("->"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      �޸���ӡ���쳣

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool PRTHelper::Repare()
{
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
	PrinterClose();
	PrinterConnect(theBOM_INFO.GetPrinterCommPort(),theBOM_INFO.GetPrinterBaudRate());
	CheckStatus();
	//PrintLastPrintFile();
	PrintUnPrintFile();
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("->"));
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ӡ��ӡ���쳣ʱδ��ӡ���ı�

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void PRTHelper::PrintUnPrintFile()
{
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
	CString searchCondition = theBOM_INFO.GetDataPath() + _T("PrintHistory\\UnPrint\\*.txt");
	vector<CString> unPrintTexts;
	SearchFile(searchCondition,unPrintTexts);
	for(vector<CString>::iterator itUnPrintText = unPrintTexts.begin(); itUnPrintText!=unPrintTexts.end();itUnPrintText++)
	{
		if(itUnPrintText!=unPrintTexts.begin())
		{
			::Sleep(2500);
		}

		CString unPrintText = *itUnPrintText;

		SYNC(FILE,unPrintText);
		CStdioFile cFile;
		UINT openFlages = CFile::modeCreate|  CFile::modeNoTruncate | CFile::modeReadWrite | CFile::typeText;
		if(!cFile.Open(unPrintText,openFlages)){
			return;
		}
		CStringArray printInfos;
		CString strTxt(_T(""));
		BOOL bReadInfo = cFile.ReadString(strTxt);
		while(bReadInfo)
		{
			printInfos.Add(strTxt);
			bReadInfo = cFile.ReadString(strTxt);
		}
		cFile.Close();
		DeleteFile(unPrintText);
		PrinterPrint(&printInfos,false);
	}
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("->"));
}

//void PRTHelper::PrintLastPrintFile()
//{
//	thePRT_LOG->WriteData("",_T(__FUNCTION__),__LINE__,"<-");
//	if(!ComCheckFileExist(lastPrintFileFullPath))
//	{
//		return;
//	}
//	SYNC(FILE,lastPrintFileFullPath);
//	CStdioFile cFile;
//	UINT openFlages = CFile::modeCreate|  CFile::modeNoTruncate | CFile::modeReadWrite | CFile::typeText;
//	if(!cFile.Open(lastPrintFileFullPath,openFlages)){
//		return;
//	}
//	CStringArray printInfos;
//	CString strTxt(_T(""));
//	BOOL bReadInfo = cFile.ReadString(strTxt);
//	while(bReadInfo)
//	{
//		printInfos.Add(strTxt);
//		bReadInfo = cFile.ReadString(strTxt);
//	}
//	cFile.Close();
//	DeleteFile(lastPrintFileFullPath);
//	PrinterPrint(&printInfos,false);
//	thePRT_LOG->WriteData("",_T(__FUNCTION__),__LINE__,"->");
//}



//////////////////////////////////////////////////////////////////////////
/*
@brief     ��ӡ��δ��ӡ�쳣�ļ��� 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void PRTHelper::SaveToUnPrintFile(CStringArray* sArray,bool bOpenBox)
{
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
	CStdioFile cFile;
	UINT openFlages = CFile::modeCreate| CFile::modeReadWrite | CFile::typeText| CFile::modeNoTruncate;
	CString unPrintFileFullPath = theBOM_INFO.GetDataPath() + _T("PrintHistory\\UnPrint\\");
	unPrintFileFullPath = unPrintFileFullPath + ComGetCurTime().ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d"))+_T(".txt");
	SYNC(FILE,unPrintFileFullPath);
	if(cFile.Open(unPrintFileFullPath,openFlages)){
		cFile.SeekToEnd();
		for(int i=0;i<sArray->GetCount();i++) {
			CString strLine =  sArray->GetAt(i)  + _T("\n");
			cFile.WriteString(strLine);
		}
		cFile.Close();
	}
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("->"));
}

//void PRTHelper::SaveToLastPrintFile(CStringArray* sArray)
//{
//	thePRT_LOG->WriteData("",_T(__FUNCTION__),__LINE__,"<-");
//	CStdioFile cFile;
//	UINT openFlages = CFile::modeCreate| CFile::modeReadWrite | CFile::typeText;
//	SYNC(FILE,lastPrintFileFullPath);
//	if(cFile.Open(lastPrintFileFullPath,openFlages)){
//		cFile.SeekToEnd();
//		for(int i=0;i<sArray->GetCount();i++) {
//			CString strLine =  sArray->GetAt(i)  + "\n";
//			cFile.WriteString(strLine);
//		}
//		cFile.Close();
//	}
//	thePRT_LOG->WriteData("",_T(__FUNCTION__),__LINE__,"->");
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��鲿���Ƿ���������Ͳ�����������

@param       �� 

@retval      ��

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
void PRTHelper::CheckDeviceExchange()
{
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
	try {
		BYTE printerId[7];
		memset(printerId,0,sizeof(printerId));
		if(GetPrinterID(printerId) == E_NO_ERROR){
			// ��鲿��ID�Ƿ���
			CCTLOGDeviceInfo::DEVICE_INFO deviceInfo;
			theDeviceInfo.GetDeviceInfo(deviceInfo);
			if(memcmp(printerId,deviceInfo.PRT_DEVICE_ID,sizeof(deviceInfo.PRT_DEVICE_ID)) != 0){
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(BOM_DEVICE_PRINTER,printerId,deviceInfo.PRT_DEVICE_ID);
				theDeviceInfo.SetPRTId(printerId);
			}
		}
	}
	catch (CSysException) {
		throw;
	}
	catch (...) {
		throw CInnerException(CTWException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("->"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ǯ��

@param      (i)��

@retval     ��  \n

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void PRTHelper::PrinterOpenBox()
{
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
	if(!theBOM_INFO.IsPrinterUsed()){
		CStringArray *sArray = new CStringArray();
		sArray->Add(_T("Open CashBox"));
		PrinterToFile(sArray);	
		delete sArray;
		sArray = NULL;
	}
	else{
		CPrinterOpenBox pPrinterOpenBox;
		g_pPrinterControl->DoCommand(&pPrinterOpenBox);
		long errCode = WaitForCommandComplete(&pPrinterOpenBox);

		if (E_NO_ERROR != errCode) {
			throw CPrinterException(CPrinterException::OTHER_ERR, _T(__FILE__), __LINE__);
		}
	}
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("->"));
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �رմ�ӡ��

@param      (i)��

@retval     ��  \n

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void  PRTHelper::PrinterClose()
{
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
	if(theBOM_INFO.IsPrinterUsed()){
		CPrinterClose pPrinterClose;
		g_pPrinterControl->DoCommand(&pPrinterClose);
		long errCode = WaitForCommandComplete(&pPrinterClose);
		if (E_NO_ERROR != errCode) {
			throw CPrinterException(CPrinterException::ERROR_Printer_CLOSE_COM, _T(__FILE__), __LINE__);		
		}
	}
	else{
		CStringArray *sArray = new CStringArray();
		sArray->Add(_T("Close Printer"));
		PrinterToFile(sArray);
		delete sArray;
		sArray = NULL;
	}
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("->"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ӡ���ݵ�

@param      (i)CStringArray* sArray ��ӡ����
@param      (i)bool bOpenBox = false���Ƿ��Ǯ��

@retval      ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void PRTHelper::PrinterToFile(CStringArray* sArray, bool bOpenBox)
{
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
	CStdioFile* sFile = new CStdioFile();;
	TCHAR* pFileName = _T("C://PrintText.txt");
	CString strLine;
	SYSTEMTIME sysTime;
	CString sYear = _T(""), sMonth = _T(""), sDate = _T(""), sHour= _T(""), sMinute= _T(""), sSecond=_T(""), sMilliSec=_T("");
	::GetLocalTime(&sysTime);
	sYear.Format(_T("%d"), sysTime.wYear);
	sMonth.Format(_T("%d"), sysTime.wMonth);
	sDate.Format(_T("%d"), sysTime.wDay);
	sHour.Format(_T("%d"), sysTime.wHour);
	sMinute.Format(_T("%d"), sysTime.wMinute);
	sSecond.Format(_T("%d"), sysTime.wSecond);
	sMilliSec.Format(_T("%d"), sysTime.wMilliseconds);

	if ( sFile->Open( pFileName, CFile::modeCreate|CFile::modeNoTruncate| CFile::modeReadWrite | CFile::typeText) ) {
		sFile->SeekToEnd();
		strLine = _T("Start at ") + sYear + sMonth + sDate + _T(" ") + sHour + sMinute + sSecond + _T(".") + sMilliSec + _T("\n");
		sFile->Write((LPCTSTR)strLine, strLine.GetLength());

		for(int i=0;i<sArray->GetCount();i++) {
			sFile->SeekToEnd();
			strLine = _T("[") + sArray->GetAt(i) + _T("]") + _T("\n");
			sFile->Write((LPCTSTR)strLine, strLine.GetLength());
		}

		::GetLocalTime(&sysTime);
		sYear.Format(_T("%d"), sysTime.wYear);
		sMonth.Format(_T("%d"), sysTime.wMonth);
		sDate.Format(_T("%d"), sysTime.wDay);
		sHour.Format(_T("%d"), sysTime.wHour);
		sMinute.Format(_T("%d"), sysTime.wMinute);
		sSecond.Format(_T("%d"), sysTime.wSecond);
		sMilliSec.Format(_T("%d"), sysTime.wMilliseconds);

		sFile->SeekToEnd();
		strLine = _T("Finish at ") + sYear + sMonth + sDate + _T(" ") + sHour + sMinute + sSecond + _T(".") + sMilliSec + _T("\n\n");
		sFile->Write((LPCTSTR)strLine, strLine.GetLength());

		sFile->Close();
	}
	delete sFile;
	sFile = NULL;
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("->"));
}


void PRTHelper::SaveToChargePrintFile(CStringArray* sArray, bool bOpenBox)
{
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
	// �����ֵ���ݵ��ļ�
	CStdioFile cFile;
	UINT openFlages = CFile::modeCreate| CFile::modeReadWrite | CFile::typeText| CFile::modeNoTruncate;
	CString chargeFileFullPath = theBOM_INFO.GetDataPath() + _T("PrintHistory\\ChargePrint\\");
	chargeFileFullPath = chargeFileFullPath + ComGetCurTime().ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d"))+_T(".txt");
	SYNC(FILE,chargeFileFullPath);
	if(cFile.Open(chargeFileFullPath,openFlages)){
		cFile.SeekToEnd();
		for(int i=0;i<sArray->GetCount();i++) {
			CString strLine =  sArray->GetAt(i)  + _T("\n");
			cFile.WriteString(strLine);
		}
		cFile.Close();
	}
	// �����ֵ���ݵ����һ�ʳ�ֵ�����ļ���
	openFlages = CFile::modeCreate| CFile::modeReadWrite | CFile::typeText;
	if(cFile.Open(m_lastChargePrintFile,openFlages)){
		cFile.SeekToEnd();
		for(int i=0;i<sArray->GetCount();i++) {
			CString strLine =  sArray->GetAt(i)  + _T("\n");
			cFile.WriteString(strLine);
		}
		cFile.Close();
	}

	if(bOpenBox){
		try{
			PrinterOpenBox();
		}catch(CSysException&)
		{

		}
	}

	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("->"));
}


void PRTHelper::PrintLastChargeReceipt()
{
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
	if(!ComCheckFileExist(m_lastChargePrintFile))
	{
		return;
	}
	SYNC(FILE,m_lastChargePrintFile);
	CStdioFile cFile;
	UINT openFlages = CFile::modeCreate|  CFile::modeNoTruncate | CFile::modeReadWrite | CFile::typeText;
	if(!cFile.Open(m_lastChargePrintFile,openFlages)){
		return;
	}
	CStringArray printInfos;
	CString strTxt(_T(""));
	BOOL bReadInfo = cFile.ReadString(strTxt);
	while(bReadInfo)
	{
		printInfos.Add(strTxt);
		bReadInfo = cFile.ReadString(strTxt);
	}
	cFile.Close();
	PrinterPrint(&printInfos,false);
	thePRT_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("->"));
}