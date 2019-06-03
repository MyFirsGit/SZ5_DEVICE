#pragma once
#include "ServiceHelper.h"
#include "PRT.h"

/**
@brief  打印机控制类
*/

class CPrinterHelper
{
public:

	CPrinterHelper();
	~CPrinterHelper();

public:
	bool IsPrinterHasException();				// 判断打印机是否有异常
	long PrinterConnect();						// 连接打印机
	long PrinterClose();						// 关闭打印机
	long CheckStatus();							// 检查打印机状态（可检测缺纸，无纸，切刀位置异常，压纸棒位置异常，RAM存储器异常）
	long GetPrinterID(BYTE*);					// 读取打印机硬件序列号（预留）
	bool Repare();								//修复打印机异常

	long PrintReceiptByBmp (CStringArray* sArray, bool bIsSaveFile,bool bIsPrintLogo = true);  // 将凭条文本合成图片并打印
	long PrintReceiptByText(CStringArray* sArray,  bool bIsSaveFile,bool bIsPrintLogo = true);   // 按文本格式打印凭条（不支持葡萄牙文）
	long PrintLastUnprintReceipt();				// 打印因打印机故障产生的未打印的凭条

	BOOL isNoPaper();	//wu zhi
	BOOL isLessPaper();	//shao zhi
	BOOL isRamError();//ram shi fou zhengchang		//郑州不支持
	BOOL isKnifePosError();//qie zhi dao wei zhi shi fou zheng que		//郑州不支持
	BOOL isBlackSplitPosNotUnderKnife();//hei tiao wei zhi shi fou zai qie dao xia fang 郑州不支持
	BOOL isStickPosError();//ya zhi bang wei zhi shi fou da kai		//郑州不支持




	/*
	*  Software use
	*/
	RECEIPT_PRINT_TYPE GetPrintTypeFromFileName(const CString&);											// 通过打印文件的名称解析操作类型
public:
	void SaveToPrintHistoryFile(CStringArray* sArray);                                                       // 打印文本到未打印目录
private:
	long GetPrinterStatus();
	void PrinterToFile(CStringArray* sArray);                                                               // 打印文本到调试文件

	CString SaveToUnPrintDir(CStringArray* sArray);															// 保存打印文本为未打印状态
	void MovePrintFileToHistory(CString strFilePathName);													// 移动打印记录到打印历史
	CString m_strNeedPrintPath;																				// 需要打印的文件路径

	CString GetNeedPrintFileName();																			// 获取需要生成的文件的名称

	BOOL PrintLastUnprintFromPath(CString strFilePath);														// 打印指定目录下的未打印的文件

	
};
