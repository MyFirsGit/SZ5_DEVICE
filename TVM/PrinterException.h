#pragma once
#include "afx.h"
#include "SysException.h"
//#include "printerexp.h"

/**
@brief	��ӡ���쳣��
*/

class CPrinterException :
    public CSysException
{
public:
    static const int MODULE_ID = 12101;     // ģ���

    typedef enum _detail_error_code {
        SET_PARAM_ERR               = 1,	// �趨ֵ�������쳣
        FUNC_PARAM_ERR              = 2,	// ���������쳣
        SEQUENCE_ERR	            = 3,	// ��������˳���쳣
        FUNC_ERR_RETURN	            = 4,	// ���������쳣
        LOGIC_ERR	                = 5,	// �߼��쳣
        OTHER_ERR	                = 6,	// �쳣����

		ERROR_Printer_LOAD_SDK =  200 ,       //����SDK����
		ERROR_Printer_OPEN_COM =  201 ,       //��ӡ�����Ӵ���
		ERROR_Printer_CLOSE_COM = 202 ,       //��ӡ���رմ���
		ERROR_Printer_SETTIME = 203 ,         //���ô�ӡ��ʱ
		ERROR_Printer_GETSTATE = 204 ,        //ȡ��ӡ��dcb����
		ERROR_Printer_SETSTATE = 205 ,        //���ô�ӡ��dcb����
		ERROR_Printer_BAUD_INVALID = 206 ,    //���������ò��Ϸ�
		ERROR_Printer_OVERLAPPED_EVENT = 207, //����OVERLAPPED�¼�ʧ��
		ERROR_Printer_NOTOPEN = 208 ,         //δ�򿪴�ӡ������
		ERROR_Printer_READFILE = 209 ,        //�Ӵ�ӡ�������ݴ���
		ERROR_Printer_WRITEFILE = 210 ,       //���ӡ��д���ݴ���
		ERROR_Printer_WAIT_ABANDONED = 211,   //�����������
		ERROR_Printer_WAIT_TIMEOUT = 212,     //��ӡʱ��ʱ
		ERROR_Printer_KNIFE_ERROR = 213,     // ��ӡ���е�����
		ERROR_Printer_PARAMTER_ERROR = 214,  // ��������
		ERROR_Printer_OTHER_ERROR = 215,     // ��ӡ����������
		ERROR_Printer_NO_PAPER = 216,		 // ��ӡ��ֽ��
		ERROR_Printer_LESS_PAPER = 217,		 // ��ӡ��ֽ����
		ERROR_Printer_HARDWARE = 218,		 // ��ӡ��Ӳ������

    } DETAIL_ERROR_CODE;

    CPrinterException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CPrinterException(void);

private:
};
