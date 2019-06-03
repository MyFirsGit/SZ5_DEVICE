
const int  PRINT_MOVEDOWN_ROWNUM = 0x08;   //��ӡ����ӡ�����ֽ����
const int  Printer_WAIT_TIME = 30000;     //��ӡ���ȴ�ʱ��
const int  BOX_WAIT_TIME = 5000;     //��Ǯ��ȴ�ʱ��

typedef enum _err_detail_Printer {
	Printer_SET_PARAM_ERR   = 1,	      // �趨ֵ�������쳣
	Printer_FUNC_PARAM_ERR  = 2,	      // ���������쳣
	Printer_SEQUENCE_ERR	= 3,	      // ��������˳���쳣
	Printer_FUNC_ERR_RETURN	= 4,	      // ���������쳣
	Printer_LOGIC_ERR	    = 5,	      // �߼��쳣
	Printer_OTHER_ERR	    = 6,	      // �쳣����

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
	ERROR_Printer_CLOSE = 213 ,           //�ر�ʱ���رմ�ӡ�����ڴ���
} ERR_DETAIL_Printer;

static const unsigned long  COMMAND_Printer_CONNECT = 0x5101  ; //�򿪴���
static const unsigned long  COMMAND_Printer_PRINT = 0x5102  ;   //��ӡ����
static const unsigned long  COMMAND_Printer_CLOSE = 0x5103  ;   //�رմ�ӡ��
static const unsigned long  COMMAND_Printer_OPENBOX = 0x5104  ;  //�ر�Ǯ��
