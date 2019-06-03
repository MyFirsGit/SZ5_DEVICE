#pragma once
#include "afx.h"
#include "SysException.h"
#include "THCOMLIBExp.h"
/**
@brief	�ⲿ��д��Ӳ���쳣��
*/

class THCOMLIB_DECLSPEC CTHHardwareException :
    public CSysException
{
public:
    static const int MODULE_ID = 12401;     // ģ���

    CTHHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CTHHardwareException(void);

	typedef enum _detail_error_code {
		TH_HARDWERE_ERROR = 1000,
		TH_COMMOND_ERROR  = 1001,
		TH_STATUS_ERROR   = 1002,

		TH_CARD_MODULE_NOT_STANDBY = 1100,		// Ʊ��ģ��δ��λ
		TH_CARD_NOT_INSTALL		   = 1101,		// ����δ��װ
		TH_WASTED_BOX_NOT_INSTALL  = 1102,		// ��Ʊ��δ��װ
		TH_ILLGIAL_INSTALL_ERROR   = 1103,		// ���ӷǷ���װ
		TH_WASTED_BOX_ALEADY_FULL  = 1104,		// ��Ʊ������
		TH_CARD_BOX_EMPTY		   = 1105,		// Ʊ��A��B�ѿ�
		TH_ERROR_TICKET_BOX1_EMPTY = 52064,		// ָ��Ʊ��1��Ʊ��1��Ʊ�����²��ܷ���
		TH_ERROR_TICKET_BOX2_EMPTY = 52065,		// ָ��Ʊ��2��Ʊ��2��Ʊ�����²��ܷ���

		ERROR_TH_NOT_STANDBY			= 2000,		// ����δ��λ
		ERROR_TH_TICKET_BOX_NOT_INSTALL = 2001,		// Ʊ��δ��λ�쳣
		ERROR_TH_SENSOR					= 2002,		// �������쳣
		ERROR_TH_ELETROMAGNET			= 2003,		// ������쳣
		ERROR_TH_CATCHCARD				= 2004,		// ��Ʊ����
		ERROR_TH_MOTOR					= 2005,		// �������
		ERROR_TH_BOX1_COVER_CLOSE		= 2006,		// Ʊ��1�ǰ�δ��
		ERROR_TH_BOX2_COVER_CLOSE		= 2007,		// Ʊ��2�ǰ�δ��
		ERROR_TH_WASTE_BOX_NOT_INSTALL	= 2008,		// ��Ʊ�䲻��λ
		ERROR_TH_WASTE_BOX_COVER_CLOSE	= 2009,		// ��Ʊ��ǰ�δ��
		ERROR_TH_WASTE_BOX_FULL			= 2010,		// ��Ʊ����
		ERROR_TH_ILLGIAL_INSTALL		= 2011,		// Ʊ����Ʊ�䰲װ�Ƿ�
		ERROR_TH_ILLGIAL_COUNT			= 2012,		// Ʊ�䰲װ������ʵ�ʲ���
		ERROR_TH_SEND_CARD_OUT_FAILED	= 2013,		// Ʊ������ʧ��
		ERROR_TH_CYCLE_CARD_FAILED		= 2014,		// Ʊ������Ʊ��ʧ��
		ERROR_TH_CARD_BOX_EMPTY			= 2015,		// Ʊ���ѿ���
		ERROR_TH_CARD_OUT_JAM			= 2016,		// ��Ʊ�ڿ�Ʊ����
	} DETAIL_ERROR_CODE;

private:
};
