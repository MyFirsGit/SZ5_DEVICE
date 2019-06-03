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
                                                                 
 @param      (i)��

 @retval     ��  

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CPrinterOpenBox::CPrinterOpenBox(void){
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ��������
                                                                 
 @param      (i)��

 @retval     ��  

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CPrinterOpenBox::~CPrinterOpenBox(void){
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ִ������
                                                                 
 @param      (i)��

 @retval     long  \n
               0 : �ɹ�  ��0 : ʧ��

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////
long CPrinterOpenBox::ExecuteCommand(void)
{
	long iRet = 1;
	iRet = OpenBoxFunction();
	this->OnComplete(iRet);
	return iRet;
}
//////////////////////////////////////////////////////////////////////////
/**
 @brief      ��Ǯ��
                                                                 
 @param      (i)��

 @retval     int  \n
             0 : �ɹ�  ��0 : ʧ��

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CPrinterOpenBox::OpenBoxFunction()
{
	int iRet = 1;
	char* cCmd = NULL;
	DWORD dwBytesWritten = 10;
	BOOL  bWriteState = FALSE;
	OVERLAPPED overlapped;

	//�жϴ�ӡ�������Ƿ��
	if ( m_hPrinter == NULL) {
		iRet = CPrinterException::ERROR_Printer_NOTOPEN;
		return iRet;
	}

	//����OVERLAPPED�¼�
	memset(&overlapped,0,sizeof(OVERLAPPED));
	overlapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if (overlapped.hEvent == NULL) {
		iRet = CPrinterException::ERROR_Printer_OVERLAPPED_EVENT;
	}
	
	//��Ǯ��ָ��
	cCmd = new char[10];
	memset(cCmd,0,10);
	cCmd[0] = 0x1B;
	cCmd[1] = 0x70;
	cCmd[2] = 0x30;
	cCmd[3] = 0x08;
	cCmd[4] = 0x09;

	//�������ӡ��
	bWriteState = WriteFile(m_hPrinter,cCmd,dwBytesWritten,&dwBytesWritten,&overlapped);
    if (NULL != cCmd) {
        delete [] cCmd;
        cCmd = NULL;
    }
	
	if (bWriteState != TRUE) {
		if (GetLastError() == ERROR_IO_PENDING) {
			iRet = WaitForSingleObject(overlapped.hEvent,BOX_WAIT_TIME);  //�ȴ��ź�
			switch( iRet) {
			case WAIT_ABANDONED :
				iRet = CPrinterException::ERROR_Printer_WAIT_ABANDONED ;
				break;
			case WAIT_OBJECT_0 :    //�ɹ�
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
		iRet = CPrinterException::ERROR_Printer_WRITEFILE;
		return iRet;
	}
	return iRet;
}
