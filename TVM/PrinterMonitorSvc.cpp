#include "stdafx.h"
#include "PrinterMonitorSvc.h"
#include "MainSvc.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CPrinterMonitorSvc::CPrinterMonitorSvc()
	:CMonitorSvc(MONITOR_PRINTER_MODULE_SVC,ROOT_SVC,theTVM_INFO.GetPrinterMonitorInternal())
{
	m_isError = false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CPrinterMonitorSvc::~CPrinterMonitorSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ض�������

@param      ��

@retval     bool �Ƿ����쳣����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CPrinterMonitorSvc::Monitor()
{
	try
	{
		PostPrinterEmptyMSG();	// ���ʹ�ӡֽ����Ϣ
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		return false;
	}
	return true;
}

void CPrinterMonitorSvc::PostPrinterEmptyMSG()
{
	try{
		if(PRINTER_HELPER->isLessPaper() || PRINTER_HELPER->isNoPaper()){
			theTVM_STATUS_MGR.SetPrintStatus(PRINTER_STATUS::PRINTER_NOPAPER);
		}
		else{
			// ����״̬
			theTVM_STATUS_MGR.SetPrintStatus(PRINTER_STATUS::PRINTER_NORMAL);
		}
	}
	catch (CSysException& e)
	{
		theTVM_STATUS_MGR.SetPrintStatus(PRINTER_STATUS::PRINTER_ERR);

		if (!theEXCEPTION_MGR.HasException(CPrinterException::MODULE_ID))
		{
			theEXCEPTION_MGR.ProcessException(e);
		}
	}
}
