#include "stdafx.h"
#include "TVMDef.h"

// ���ģ�������
CFTPControl*					g_pFTPControl = NULL;
CSCControl*						g_pSCControl = NULL;

// Ӳ��ģ�������
//CRWControl*				g_pChargeRWControl= NULL;				    // ��д������
//CRWControl*				g_pCardRWControl= NULL;					    // ��д������
//CRWControl*				g_pTokenRWControl= NULL;					// ��д������
CRWControl*					g_pTVMRWControl= NULL;				    // ��д������
CPrinterControl*				g_pPrinterControl= NULL;				        // ��ӡ������
//CMemController*				g_pMemControl= NULL;					    // ����ģ�����
CMIMControl*				g_pMIMControl= NULL;					// ����ģ�����
CCHControl*					g_pCHControl= NULL;						    // Ӳ��ģ�����
CBRHControl*				g_pBHControl= NULL;						// ֽ�ҽ���ģ�����
CBHChangeControl*			g_pBHChangeControl	= NULL;				// ֽ������ģ�����
//CIOControl*					g_pIOControl= NULL;						// IOģ�����
CStatusDisplayControl*		g_pStatusDisplayControl= NULL;		            // ״̬��ʾ��ģ�����

Data::CDataControl*			g_pDataControl = NULL;
