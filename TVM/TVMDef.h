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

extern CFTPControl*						g_pFTPControl;					//	FTP����
extern CSCControl*						g_pSCControl;					// SCͨ�ſ���

extern CRWControl*						g_pTVMRWControl;					// ��д������
//extern CRWControl*					g_pChargeRWControl;					// ��д������
//extern CRWControl*					g_pCardRWControl;					// ��д������
//extern CRWControl*					g_pTokenRWControl;					// ��д������
extern CPrinterControl*					g_pPrinterControl;				// ��ӡ������
//extern CMemController*				    g_pMemControl;					// ����ģ�����
extern CMIMControl*					    g_pMIMControl;					// ����ģ�����
extern CCHControl*						g_pCHControl;					// Ӳ��ģ�����
extern CBRHControl*						g_pBHControl;					// ֽ�ҽ���ģ�����
extern CBHChangeControl*				g_pBHChangeControl;				// ֽ������ģ�����
extern CStatusDisplayControl*		    g_pStatusDisplayControl;		// ״̬��ʾ��ģ�����
//extern CIOControl*						g_pIOControl;					//	IOģ�����

extern Data::CDataControl*				g_pDataControl;									//���ݿ�ģ�����

