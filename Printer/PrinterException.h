#pragma once
#include "afx.h"
#include "sysException.h"
#include "printerexp.h"

/**
@brief	��ӡ���쳣��
*/

class PRINTER_API CPrinterException :
    public CSysException
{
public:
    static const int MODULE_ID = 10410;     // ģ���

    typedef enum _detail_error_code {
        SET_PARAM_ERR               = 1,	// �趨ֵ�������쳣
        FUNC_PARAM_ERR              = 2,	// ���������쳣
        SEQUENCE_ERR	            = 3,	// ��������˳���쳣
        FUNC_ERR_RETURN	            = 4,	// ���������쳣
        LOGIC_ERR	                = 5,	// �߼��쳣
        OTHER_ERR	                = 6,	// �쳣����

		ERROR_Printer_OPEN_COM =  201 ,       //��ʱ���޷��򿪴�ӡ�����ڴ���
		ERROR_Printer_CLOSE_COM = 202 ,       //��ʱ���رմ�ӡ�������ڴ���
		ERROR_Printer_OPEN_ERROR = 203 ,      //��ʱ���򿪴�ӡ������
		ERROR_Printer_SETTIME = 204 ,         //��ʱ�����ô�ӡ��ʱ����
		ERROR_Printer_GETSTATE = 205 ,        //��ʱ��ȡ��ӡ��dcb����
		ERROR_Printer_SETSTATE = 206 ,        //��ʱ�����ô�ӡ��dcb����
		ERROR_Printer_BAUD_INVALID = 207 ,    //��ʱ�����������ò��Ϸ�
		ERROR_Printer_OVERLAPPED_EVENT = 208, //��ʱ������OVERLAPPED�¼�ʧ��
		ERROR_Printer_NOTOPEN = 209 ,         //��ӡʱ��δ�򿪴�ӡ������
		ERROR_Printer_WRITEFILE = 210 ,       //��ӡʱ�����ӡ��д���ݴ���
		ERROR_Printer_WAIT_ABANDONED = 211,   //��ӡʱ  �����������
		ERROR_Printer_WAIT_TIMEOUT = 212,     //��ӡʱ  ��ʱ
		ERROR_Printer_NO_PAPER = 213,         //��ӡʱ����ӡ����ֽ
		ERROR_Printer_LESS_PAPER = 214,       //��ӡʱ����ӡ��ֽ����
		ERROR_Printer_CLOSE = 215,            //�ر�ʱ���رմ�ӡ�����ڴ���
    } DETAIL_ERROR_CODE;

    CPrinterException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CPrinterException(void);

private:
};
