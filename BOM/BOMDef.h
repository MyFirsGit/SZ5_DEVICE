#pragma once

#include "FTPControl.h"
#include "RWControl.h"
#include "SCControl.h"
#include "PrinterControl.h"
#include "THControl.h"
extern CFTPControl*    g_pFTPControl;
extern CRWControl*     g_pRWControl;
extern CSCControl* g_pSCControl;
extern CPrinterControl* g_pPrinterControl;   //打印机

//extern CMemController*  g_pMemControl;
extern CMIMControl* g_pMIMControl;   //出票机