#pragma once
#include "FTPControl.h"
#include "SCControl.h"
#include "RWControl.h"
#include "PRT.h"
#include "THControl.h"
#include "CHControl.h"
#include "BRHControl.h"
#include "IOControl.h"
#include "StatusDisplayControl.h"
#include "DataControl.h"
#include "BHChangeControl.h"
#include "../Printer/PrinterControl.h"

extern CFTPControl*						g_pFTPControl;					//	FTP控制
extern CSCControl*						g_pSCControl;					// SC通信控制

extern CRWControl*						g_pTVMRWControl;					// 读写器控制
//extern CRWControl*					g_pChargeRWControl;					// 读写器控制
//extern CRWControl*					g_pCardRWControl;					// 读写器控制
//extern CRWControl*					g_pTokenRWControl;					// 读写器控制
extern CPrinterControl*					g_pPrinterControl;				// 打印机控制
//extern CMemController*				    g_pMemControl;					// 发卡模块控制
extern CMIMControl*					    g_pMIMControl;					// 发卡模块控制
extern CCHControl*						g_pCHControl;					// 硬币模块控制
extern CBRHControl*						g_pBHControl;					// 纸币接收模块控制
extern CBHChangeControl*				g_pBHChangeControl;				// 纸币找零模块控制
extern CStatusDisplayControl*		    g_pStatusDisplayControl;		// 状态显示器模块控制
//extern CIOControl*						g_pIOControl;					//	IO模块控制

extern Data::CDataControl*				g_pDataControl;									//数据库模块控制

