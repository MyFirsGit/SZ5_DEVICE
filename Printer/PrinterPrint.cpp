#include "stdafx.h"
#include "PrinterCommand.h"
#include "PrinterException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ���캯��
                                                                 
 @param      (i)CStringArray* sArray ��ӡ�����ַ�������
 @param      (i)bool   bOpenBox �Ƿ��ڴ�ӡ��ͬʱ��Ǯ�䣬Ĭ�ϣ�false(����)

 @retval      ��

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CPrinterPrint::CPrinterPrint(CStringArray* sArray, bool bOpenBox){

	m_sArrPrinter = NULL;
	if ( sArray != NULL) {
		if ( sArray->GetSize() > 0 ) {
			m_sArrPrinter = new CStringArray();
			for (int i= 0;i < sArray->GetSize();i++) {
				CString sOne = sArray->GetAt(i);
				m_sArrPrinter->Add(sOne);
			}
		}
	}
	m_bOpenBox = bOpenBox;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ��������
                                                                
 @param      (i)��

 @retval      ��

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CPrinterPrint::~CPrinterPrint(void){
	delete m_sArrPrinter;
    m_sArrPrinter = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ��ӡ����
                                                                 
 @param      (i)��

 @retval      int   \n
                0 : �ɹ�  ��0 : ʧ��

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CPrinterPrint::PrintFunction()
{
	int iRet = 1;
	if (m_hPrinter == NULL || m_hPrinter == INVALID_HANDLE_VALUE) 
	{
		iRet = CPrinterException::ERROR_Printer_NOTOPEN;
		return iRet;
	}
	// ��ӡ����Ϊ�ղ��Ҳ���Ǯ��
	if (m_sArrPrinter == NULL && m_bOpenBox == false) {
		return  CPrinterException::ERROR_Printer_WRITEFILE;
	}

	DWORD  dwTotalLen = 0 ;
	char* buff = NULL;
	//�д�ӡָ��
	char cCmdRowPrint=0X0A;
	//��ָֽ��
	char cCmdPageGo[]={0x1B,0x4A,0x60};
	//��Ǯ��ָ��
	char cCmdOpenbox[] = {0x1B,0x70,0x30,0x08,0x09};
	//��ӡ�����Ի�ָ��
	char cInit[]={0x1B,0x40};
	//��ָֽ��
	char cCutPaper[]={0x1D,0x56,0x42,0x12};

	if(m_sArrPrinter != NULL){
		int cnt = (int)m_sArrPrinter->GetCount();
		int iSize = cnt*50 + 14; 
		buff = new char[iSize];
		memset(buff,0,iSize);

		//��Ǯ��ָ��
		if (m_bOpenBox == true) {
			memcpy(buff,cCmdOpenbox,sizeof(cCmdOpenbox));
			dwTotalLen += sizeof(cCmdOpenbox);
		}

		//���Ի���ӡ��
		memcpy(buff + dwTotalLen,cInit,sizeof(cInit));
		dwTotalLen += sizeof(cInit);

		//ȡ�ô�ӡ���ݼ���ӡָ��
		for (int i=0;i<cnt;i++) {
			CString sOne = _T(" ") + m_sArrPrinter->GetAt(i);
			memcpy(buff + dwTotalLen,sOne.GetBuffer(),sOne.GetLength());
			dwTotalLen += sOne.GetLength();
			if (i < cnt -1) {
				memcpy(buff + dwTotalLen,&cCmdRowPrint,1);
				dwTotalLen += 1;
			}
		};

		if (cnt > 0) {
			//������ָֽ��
			memcpy(buff + dwTotalLen,cCmdPageGo,sizeof(cCmdPageGo));
			dwTotalLen += sizeof(cCmdPageGo);

			//������ָֽ��
			memcpy(buff + dwTotalLen,cCutPaper,sizeof(cCutPaper));
			dwTotalLen += sizeof(cCutPaper);
		}
	}
	else{
		if(m_bOpenBox == true){
			 buff = new char[sizeof(cCmdOpenbox)];
			 memset(buff,0,sizeof(cCmdOpenbox));
			 //��Ǯ��ָ��
			 memcpy(buff,cCmdOpenbox,sizeof(cCmdOpenbox));
			 dwTotalLen = sizeof(cCmdOpenbox);
		}
	}
	OVERLAPPED overlapped;   //���ڴ�ӡ����OVERLAPPED �ṹ
	//����OVERLAPPED�¼�
	memset(&overlapped,0,sizeof(OVERLAPPED));
	overlapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if (overlapped.hEvent == NULL || overlapped.hEvent == INVALID_HANDLE_VALUE)
	{
		CloseHandle(overlapped.hEvent);
		iRet = CPrinterException::ERROR_Printer_OVERLAPPED_EVENT;
		return iRet;
	}
	//�������ӡ��
	DWORD dwBytesWritten = 0;
	BOOL bWriteState = WriteFile(m_hPrinter,buff,dwTotalLen,&dwBytesWritten,&overlapped);  
	if (NULL != buff) {
		delete [] buff;
		buff = NULL;
	}
   
	if(bWriteState != TRUE) {
		if (GetLastError() == ERROR_IO_PENDING) {
			iRet = WaitForSingleObject(overlapped.hEvent,Printer_WAIT_TIME);  //�ȴ��ź�
			switch( iRet) {
			  case WAIT_ABANDONED :
				iRet = CPrinterException::ERROR_Printer_WAIT_ABANDONED ;
		  		break;
			  case WAIT_OBJECT_0 : 
				iRet = 0;
				break;
			  case WAIT_TIMEOUT : 
				iRet = CPrinterException::ERROR_Printer_WAIT_TIMEOUT;
				::CancelIo(m_hPrinter);
				break;
			  case WAIT_FAILED :
				iRet =  CPrinterException::ERROR_Printer_WRITEFILE;
				break;
			}
		}
		else{
			iRet = CPrinterException::ERROR_Printer_WRITEFILE;
		}
	}
	else{
		iRet = 0;
	}
	CloseHandle(overlapped.hEvent);
	return iRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ִ������
                                                                 
 @param      (i)��

 @retval      long   \n
                 0 : �ɹ�  ��0 : ʧ��

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////
long CPrinterPrint::ExecuteCommand(void){
   long iRet = 1;
   iRet = PrintFunction();
   this->OnComplete(iRet);
   return iRet;
}