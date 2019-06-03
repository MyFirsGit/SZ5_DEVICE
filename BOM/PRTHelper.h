#pragma once

#include "ServiceHelper.h"
#include "PrinterCommand.h"

/**
@brief  打印机控制类
*/

class PRTHelper :
	public CServiceHelper
{
public:

	PRTHelper(CService& service);
	~PRTHelper();

	bool IsPrinterHasException();												 // 判断打印机是否有异常

	// 打印机相关
	void CheckDeviceExchange();													  // 检查打印机是否更换
	long PrinterConnect(int nPort, int nBaud);                                    // 连接打印机
	void PrinterPrint(CStringArray* sArray, bool bOpenBox = false);               // 打印数据
	void PrinterOpenBox();                                                        // 打开钱箱
	void PrinterClose();                                                          // 关闭打印机
	void CheckStatus();															  // 检查打印机状态
	long GetPrinterID(BYTE*);													  // 读取打印机ID
	bool Repare();																  // 修复打印机异常
	// 20170315：根据业主需求，充值不打印水单，都保存文件到未打印目录下；在其他业务中增加单据打印按钮，打印最后一笔充值单据。
	void SaveToChargePrintFile(CStringArray* sArray, bool bOpenBox);				// 打印数据保存到充值单据目录下,并开钱箱
	void PrintLastChargeReceipt();													// 打印充值单据目录下保存的最后一笔充值单据
	CString m_lastChargePrintFile;													// 最后一笔充值数据文件名称
private:
	void PrinterToFile(CStringArray* sArray,bool bOpenBox = false);				  // 打印数据到文件
	void PrintUnPrintFile();
	void SaveToUnPrintFile(CStringArray* sArray,bool bOpenBox = false);
	//void PrintLastPrintFile();
	//void SaveToLastPrintFile(CStringArray* sArray);
	//CString lastPrintFileFullPath;
};
