#include "stdafx.h"
#include "TVMDef.h"

// 软件模块控制器
CFTPControl*					g_pFTPControl = NULL;
CSCControl*						g_pSCControl = NULL;

// 硬件模块控制器
//CRWControl*				g_pChargeRWControl= NULL;				    // 读写器控制
//CRWControl*				g_pCardRWControl= NULL;					    // 读写器控制
//CRWControl*				g_pTokenRWControl= NULL;					// 读写器控制
CRWControl*					g_pTVMRWControl= NULL;				    // 读写器控制
CPrinterControl*				g_pPrinterControl= NULL;				        // 打印机控制
//CMemController*				g_pMemControl= NULL;					    // 发卡模块控制
CMIMControl*				g_pMIMControl= NULL;					// 发卡模块控制
CCHControl*					g_pCHControl= NULL;						    // 硬币模块控制
CBRHControl*				g_pBHControl= NULL;						// 纸币接收模块控制
CBHChangeControl*			g_pBHChangeControl	= NULL;				// 纸币找零模块控制
//CIOControl*					g_pIOControl= NULL;						// IO模块控制
CStatusDisplayControl*		g_pStatusDisplayControl= NULL;		            // 状态显示器模块控制

Data::CDataControl*			g_pDataControl = NULL;
