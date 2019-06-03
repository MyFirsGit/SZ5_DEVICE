#include "stdafx.h"
#include "PrinterMonitorSvc.h"
#include "MainSvc.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CPrinterMonitorSvc::CPrinterMonitorSvc()
	:CMonitorSvc(MONITOR_PRINTER_MODULE_SVC,ROOT_SVC,theTVM_INFO.GetPrinterMonitorInternal())
{
	m_isError = false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CPrinterMonitorSvc::~CPrinterMonitorSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      监控动作函数

@param      无

@retval     bool 是否有异常发生

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CPrinterMonitorSvc::Monitor()
{
	try
	{
		PostPrinterEmptyMSG();	// 发送打印纸空消息
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
			// 设置状态
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
