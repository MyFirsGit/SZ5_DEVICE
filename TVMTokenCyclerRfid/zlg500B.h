////定义通信帧常量
//#define STX		0x20
//#define ACK		0x06
//#define NAK		0x15
//#define ETX		0x03

//#define SEQNR		0
//#define COMMAND		1	
//#define STATUS		1
//#define LENGTH		2		
//#define DATA		3

////mifare error
//#define MI_OK				0
//#define MI_NOTAGERR         1
//#define MI_EMPTY			3
//#define MI_AUTHERR			4
//#define MI_PARITYERR		5
//#define MI_CODEERR			6
//
//#define MI_SERNRERR			8
//#define MI_NOTAUTHERR		10
//#define MI_BITCOUNTERR		11
//#define MI_BYTECOUNTERR		12
//#define MI_TRANSERR			14
//#define MI_WRITEERR			15
//#define MI_INCRERR			16
//#define MI_DECRERR			17
//#define MI_READERR			18
//#define MI_QUIT				30
//
////check write Error
//#define MIS_CHK_OK          0
//#define MIS_CHK_FAILED      1
//#define MIS_CHK_COMPERR     2

unsigned char __stdcall zlg500B_request(unsigned char mode,unsigned short &tagtype);
unsigned char __stdcall zlg500B_anticoll(unsigned char bcnt,unsigned long &snr);
unsigned char __stdcall zlg500B_anticoll2(unsigned char encoll,unsigned char bcnt,unsigned long &snr);
unsigned char __stdcall zlg500B_select(unsigned long snr,unsigned char &size);
unsigned char __stdcall zlg500B_authentication(unsigned char mode,unsigned char secnr);
unsigned char __stdcall zlg500B_authentication2(unsigned char mode,unsigned char secnr,unsigned char keynr);
unsigned char __stdcall zlg500B_halt(void);
unsigned char __stdcall zlg500B_read(unsigned char addr,unsigned char *data);
unsigned char __stdcall zlg500B_write(unsigned char addr,unsigned char *data);
unsigned char __stdcall zlg500B_increment(unsigned char addr,long &value);
unsigned char __stdcall zlg500B_decrement(unsigned char addr,long &value);
unsigned char __stdcall zlg500B_writeval(unsigned char addr,long value);
unsigned char __stdcall zlg500B_readval(unsigned char addr,long &value);
unsigned char __stdcall zlg500B_changepwd(unsigned char mode,unsigned char secnr,unsigned char *pwd);
unsigned char __stdcall zlg500B_restore(unsigned char addr);
unsigned char __stdcall zlg500B_transfer(unsigned char addr);
unsigned char __stdcall zlg500B_value(unsigned char mode,unsigned char addr,long &value,unsigned char trans_addr);
unsigned char __stdcall zlg500B_load_key(unsigned char mode,unsigned char secnr,unsigned char *key);
unsigned char __stdcall zlg500B_reset(unsigned char msec);
unsigned char __stdcall zlg500B_close(void);
unsigned char __stdcall zlg500B_config(void);
unsigned char __stdcall zlg500B_get_info(unsigned char *info);
unsigned char __stdcall zlg500B_check_write(unsigned char snr,unsigned char authenmode,unsigned char addr,unsigned char *data);
unsigned char __stdcall zlg500B_set_control_bit();
unsigned char __stdcall zlg500B_clr_control_bit();
unsigned char __stdcall zlg500B_buzzer(unsigned char frquence,unsigned char opentm,unsigned char closetm,unsigned char repcnt);
unsigned char __stdcall zlg500B_read_E2(unsigned char addr,unsigned char length,unsigned char *data);
unsigned char __stdcall zlg500B_write_E2(unsigned char addr,unsigned char length,unsigned char *data);
unsigned char __stdcall zlg500B_authKey(unsigned char mode,unsigned char secnr,
										unsigned char *key);


unsigned char  __stdcall zlg500B_init(int nport=1, int nbaud=9600);
void __stdcall zlg500B_exit(void);