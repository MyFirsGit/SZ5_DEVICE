#include <atlconv.h>
#include "StatusDisplayExp.h"
#ifndef _STATUSDISPLAY_DEF_H
#define _STATUSDISPLAY_DEF_H
const int  StatusDisplay_WAIT_TIME = 10;
static const unsigned long  COMMAND_StatusDisplay_CONNECT = 0x5101;
static const unsigned long  COMMAND_StatusDisplay_PRINT = 0x5102;
static const unsigned long  COMMAND_StatusDisplay_CLOSE = 0x5103;
static const unsigned long  COMMAND_StatusDisplay_OPENBOX = 0x5104;
#define UD_MAX_CONTENTLEN			11
#define MAX_SEND_BUFFER				128		// ��෢�͵���������
#define UD_MAX_HALFBUFFER	UD_MAX_CONTENTLEN + 1
#define UD_MAXBUFFER		UD_MAX_CONTENTLEN * 2 + 1
#define UD_MAX_EN_CONTENTLEN		12
#define UD_MAX_EN_LALFBUFFER UD_MAX_EN_CONTENTLEN + 1
#define UD_MAX_EN_BUFFER			UD_MAX_EN_CONTENTLEN*2 + 1
#define GB2312						936
//------------------------------------------------------------------------------
//����������
#define BX_5E1  0x0154
#define BX_5E2  0x0254
#define BX_5E3  0x0354
#define BX_5Q0P  0x1056
#define BX_5Q1P  0x1156
#define BX_5Q2P  0x1256
#define BX_6Q1  0x0166		// CQ��
#define BX_6Q2  0x0266
#define BX_6Q2L  0x0466

//----------------------------------------------------------------------

#define FRAME_SINGLE_COLOR_COUNT  23 //��ɫ�߿�ͼƬ����
#define FRAME_MULI_COLOR_COUNT  47 //��ɫ�߿�ͼƬ����

//------------------------------------------------------------------------------
// ͨѶģʽ
#define SEND_MODE_SERIALPORT  0
#define SEND_MODE_NETWORK  2
#define SEND_MODE_Server_2G  5
#define SEND_MODE_Server_3G  6
#define SEND_MODE_SAVEFILE  7
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// ��̬��������ģʽ
#define RUN_MODE_CYCLE_SHOW  0 //��̬������ѭ����ʾ��
#define RUN_MODE_SHOW_LAST_PAGE  1 //��̬��������ʾ��ɺ�ֹ��ʾ���һҳ���ݣ�
#define RUN_MODE_SHOW_CYCLE_WAITOUT_NOSHOW  2 //��̬������ѭ����ʾ�������趨ʱ���������δ����ʱ������ʾ��
#define RUN_MODE_SHOW_ORDERPLAYED_NOSHOW  4 //��̬������˳����ʾ����ʾ�����һҳ��Ͳ�����ʾ
//------------------------------------------------------------------------------

#define SCREEN_NO  3
#define SCREEN_WIDTH  256
#define SCREEN_HEIGHT 64
#define SCREEN_TYPE  4
#define SCREEN_DATADA  0
#define SCREEN_DATAOE  0
#define SCREEN_COMM  "COM1"
#define SCREEN_BAUD  57600
#define SCREEN_ROWORDER  0
#define SCREEN_FREQPAR  0
#define SCREEN_SOCKETIP  _T("192.168.0.199")
#define SCREEN_SOCKETPORT  5005
//#define m_bSendBusy  false	�˱��������ݸ����зǳ���Ҫ������ر�����
#define FAddDynamicArea  "�ڶ���-----��Ӷ�̬����"
#define FAddDYAreaFile  "������-----��̬�����ļ�����"
typedef enum _eledclr
{
	UE_RED=0x31,
	UE_GREEN,
	UE_YELLOW
}ELEDCLR;
typedef enum _dyareald
{
	UE_DYAREAID_0 = 0,
	UE_DYAREAID_1,
	UE_DYAREAID_2,
	UE_DYAREAID_3,
}DYAREAID;
typedef enum _eleddispmode
{
	UE_MOVECYCLE=0x00,
	UE_NOMOVE=0x01,
	UE_IMMEDIATE,
	UE_MOVELEFT,
	UE_MOVERIGHT,
	UE_MOVEUP,
	UE_MOVEDOWN
}ELEDDISPMODE;

typedef enum _eledmovespeed
{
	UE_LEVEL0 =0x00,
	UE_LEVEL1,
	UE_LEVEL2,
	UE_LEVEL3,
	UE_LEVEL4,
	UE_LEVEL5,
	UE_LEVEL6,
	UE_LEVEL7,
	UE_LEVEL8,
	UE_LEVEL9
}ELEDMOVESPEED;
// ����״̬��ʾ������ģʽ
typedef struct _disp_work_model_{
	ELEDDISPMODE display_mode;		// �ƶ�����
	ELEDDISPMODE exit_mode;			// �˳�����
	ELEDMOVESPEED display_speed;	// �ƶ��ٶ�
	int			nDispayTime;		// ͣ��ʱ��
	CString		strDispContent;		// ��ʾ��������
	_disp_work_model_(){
		display_mode  = UE_MOVELEFT;
		exit_mode	  = UE_MOVELEFT;
		display_speed = UE_LEVEL4;
		nDispayTime	  = 5;
		strDispContent= _T("");
	}

}DISP_WORK_MODEL;
//����߿�����
typedef struct _tdAreaFrameInfo
{
	byte btAreaFFlag;		// ����߿��־λ��ע��������ֶ�Ϊ0������������߿����Բ�����
	byte btAreaFDispStyle;	// �߿���ʾ��ʽ��0x00 �C��˸
											//0x01 �C˳ʱ��ת��
											//0x02 �C��ʱ��ת��
											//0x03 �C��˸��˳ʱ��ת��
											//0x04 �C��˸����ʱ��ת��
											//0x05 �C���̽�����˸
											//0x06 �C���̽���ת��
											//0x07 �C��ֹ���
	byte btAreaFDispSpeed;	// �߿���ʾ�ٶ�
	byte btAreaFMoveStep;	// �߿��ƶ���������λΪ�㣬�˲�����ΧΪ1~16
	byte btAreaFWidth;		// �߿���Ԫ��ȣ��˲�����ΧΪ1~8��ע���߿���Ԫ�ĳ��ȹ̶�Ϊ16
	byte btAreaFBackup[2];	// ������
	byte btAreaFUnitData;	// =AreaFWidth
	_tdAreaFrameInfo()
	{
		btAreaFFlag = 0x01;
		btAreaFDispStyle = 0x01;
		btAreaFDispSpeed = 0x02;
		btAreaFMoveStep = 0x08;
		btAreaFWidth = 0x08;
		memset(btAreaFBackup,0,2);
		btAreaFUnitData = btAreaFWidth;
	}
}AREAFRAME;
typedef struct _tdispinfo
{
	WORD wScreenNo;		// ��ʾ�����ţ��ò�����AddScreen_Dynamic�����е�nScreenNo������Ӧ
	WORD wDYAreaID;		// ��̬������
	WORD wRunMode;	// ��̬������ģʽ0-��̬������ѭ����ʾ
	WORD wTimeOut;			// ��̬�����ݳ�ʱʱ�䣬��λΪ��
	WORD wAllProRelate;		// ��Ŀ������־	1�����н�Ŀ����ʾ�ö�̬����
	char sProRelateList[16];	// ��Ŀ�����б��ý�Ŀ��ű�ʾ����Ŀ��ż���","����,��Ŀ��Ŷ���ΪLedshowTW 2013�����"P***"�е�"***"
	WORD wPlayPriority;	// ��̬���򲥷����ȼ���0���ö�̬�������첽��Ŀһ�𲥷�(��̬�����й����첽��Ŀ����Ч)�� 1���첽��Ŀֹͣ���ţ������Ÿö�̬����Ч��
	WORD wAreaX;
	WORD wAreaY;
	WORD wAreaWidth;
	WORD wAreaHeight;
	WORD wAreaFMode;		// ��̬����߿���ʾ��־0����ɫ��1����ɫ��255���ޱ߿�
	WORD wAreaFLine;		// ��̬����߿����ͣ���ɫ���ȡֵΪFRAME_SINGLE_COLOR_COUNT����ɫ���ȡֵΪ��FRAME_MULI_COLOR_COUNT
	WORD wAreaFColor;		// �߿���ʾ��ɫ��ѡ��Ϊ��ɫ�߿�����ʱ�ò�����Ч��
	WORD wAreaFStunt;		// �߿������ؼ���
							// 	0����˸��
							// 		1��˳ʱ��ת����
							// 		2����ʱ��ת����
							// 		3����˸��˳ʱ��ת���� 
							// 		4:��˸����ʱ��ת����
							// 		5�����̽�����˸��
							// 		6�����̽���ת����
							// 		7����ֹ���
	WORD wAreaFRunSpeed;	// �߿������ٶȣ�
	WORD wAreaFMoveStep;	// �߿��ƶ���������ֵȡֵ��Χ��1~8��
	_tdispinfo()
	{
		wScreenNo = SCREEN_NO;
		wDYAreaID = UE_DYAREAID_0; 
		wRunMode = UE_MOVECYCLE;
		wTimeOut = 0x0a;
		wAllProRelate = 0x00;
		strset(sProRelateList,0x00);
		wPlayPriority = 1;
		wAreaX = 0;
		wAreaY = 0;
		wAreaWidth = 0;
		wAreaHeight = 0;
		wAreaFMode = 0xff;
		wAreaFLine = 0x00;
		wAreaFColor = 0x00;
		wAreaFStunt = 0x00;
		wAreaFRunSpeed = 0x00;
		wAreaFMoveStep = 0x00;
	}
}TDISPINFO;
typedef struct _tdispinfotext
{
	WORD wScreenNo;		// ��ʾ�����ţ��ò�����AddScreen_Dynamic�����е�nScreenNo������Ӧ
	WORD wDYAreaID;		// ��̬������
	char sText[48];		// ��ӵ���Ϣ�ı��ַ���
	WORD wShowSingle;		//������Ϣ�Ƿ�����ʾ��0��������ʾ��1��������ʾ
	char sFontName[16];	// ������Ϣ��ʾ���� :"����"
	WORD wFontSize;		// ������Ϣ��ʾ������ֺ�
	WORD wBold;			// ������Ϣ�Ƿ������ʾ��0��������1��������ʾ��
	DWORD wFontColor;		// ������Ϣ��ʾ��ɫ��0~255
	WORD wStunt;			// ��̬������Ϣ�����ؼ���
							// 		00�������ʾ		11����������
							// 		01����ֹ��ʾ		12�����ҽ�������
							// 		02�����ٴ��		13�����½�������
							// 		03�������ƶ�		14������պ�
							// 		04����������		15�������
							// 		05�������ƶ� 	16����������
							// 		06����������		17����������
							// 		07����˸			18����������
							// 		08��Ʈѩ			19����������
							// 		09��ð�� 		20����������
							// 		10���м��Ƴ� 	21����������
							// 		22���������� 		31��������Ļ
							// 		23���������� 		32��������Ļ
							// 		24�����ҽ�����Ļ		33�����ұպ�
							// 		25�����½�����Ļ		34�����ҶԿ�
							// 		26����ɢ����		35�����±պ�
							// 		27��ˮƽ��ҳ 		36�����¶Կ�
							// 		28����ֱ��ҳ		37�������ƶ�
							// 		29��������Ļ 		38����������
							// 		30��������Ļ 		39�������ƶ�
							//		40����������
	WORD wRunSpeed;		//��̬������Ϣ��ʾ�ٶ������ٶȣ�1~100
	WORD wShowTime;		// ��̬������Ϣ��ʾʱ�䣻��λ��10ms

	_tdispinfotext()
	{
		wScreenNo = SCREEN_NO;
		wDYAreaID =	UE_DYAREAID_0; 
		memset(sText,0,sizeof(sText));
		wShowSingle = 1;
		strcpy(sFontName ,"����");
		wFontSize = 16;
		wBold = 0;
		wFontColor = 255;
		wStunt = 7;
		wRunSpeed = 5;
		wShowTime = 10;
	}
	void setContent(const TCHAR* tcContent)
	{
		#ifdef UNICODE
		UINT uiDetect = WideCharToMultiByte(CP_OEMCP, NULL, tcContent, -1, NULL, 0, NULL, FALSE);
		UINT uiMaxTCHAR = 0;

		bool bIsMaxContent = false;
		
		if (uiDetect > UD_MAXBUFFER){
			bIsMaxContent = true;
		}
		else{
			uiMaxTCHAR = _tcslen(tcContent);
		}

		// ��������ַ���
		if(bIsMaxContent){
			UINT  uiContentLen = 0;
			TCHAR tcTemp[MAX_SEND_BUFFER];
			memset(tcTemp,'\0',sizeof(TCHAR)*MAX_SEND_BUFFER);

			uiContentLen = _tcslen(tcContent);
			_tcsncpy(tcTemp,tcContent,uiContentLen);

			// ���ַ�תխ�ַ�
			CW2AEX<> wChange(tcTemp,GB2312);

			DWORD dwDataLen = strlen(wChange);
			memmove(sText,wChange,dwDataLen);
		}
		// �������ַ������ڣ���Ҫ������ʾ
		else{
			TCHAR tcMAxTCHAR[UD_MAXBUFFER];
			memset(tcMAxTCHAR, '\0', sizeof(TCHAR) * UD_MAXBUFFER);

			_tcsncpy(tcMAxTCHAR, tcContent, uiMaxTCHAR);

			TCHAR tcbuffer[UD_MAX_HALFBUFFER] = _T("\0");

			UINT uilen = _tcslen(tcMAxTCHAR);
			UINT uileft = uilen / 2;
			UINT uirght = uilen - uileft;

			TCHAR tcleft[UD_MAX_HALFBUFFER] = _T("\0");
			TCHAR tcright[UD_MAX_HALFBUFFER] = _T("\0");;

			_tcsncpy(tcleft, tcMAxTCHAR, uileft);
			_tcsncpy(tcright, tcMAxTCHAR + uileft, uirght);

			TCHAR tcLeftContent[UD_MAX_CONTENTLEN + 1];
			TCHAR tcRightContent[UD_MAX_CONTENTLEN + 1];

			UINT uipartlen = _tcslen(tcleft);
			UINT uiCharCount = WideCharToMultiByte(CP_OEMCP, NULL, tcleft, -1, NULL, 0, NULL, FALSE);
			CString szformat;
			CString szCount;
			szCount.Format(_T(" %ds"), UD_MAX_CONTENTLEN  - uiCharCount+1);
			szCount=_T("%")+szCount;
			szformat = szCount + _T("%s");
			_stprintf(tcbuffer, szformat, _T(""),tcleft);
			_tcsncpy_s(tcLeftContent, UD_MAX_CONTENTLEN + 1, tcbuffer, UD_MAX_CONTENTLEN);
			tcLeftContent[UD_MAX_CONTENTLEN]='\0';

			uipartlen = _tcslen(tcright);
			uiCharCount = WideCharToMultiByte(CP_OEMCP, NULL, tcright, -1, NULL, 0, NULL, FALSE);
			szformat = _T("%s% ");
			CString szcount;
			szcount.Format(_T("%ds"), UD_MAX_CONTENTLEN - uiCharCount +1);
			szformat += szcount;
			_stprintf(tcbuffer, szformat, tcright, _T(""));
			_tcsncpy_s(tcRightContent, UD_MAX_CONTENTLEN+1,tcbuffer,UD_MAX_CONTENTLEN);
			tcRightContent[UD_MAX_CONTENTLEN]='\0';

			CW2AEX<> w2aLeft(tcLeftContent,GB2312);
			CW2AEX<> w2aRight(tcRightContent,GB2312);

			int ileftchar=strlen(w2aLeft);
			int irightchar=strlen(w2aRight);
			memmove(sText,w2aLeft,ileftchar);
			memmove(sText+ileftchar,w2aRight,irightchar);
		}
#else

#endif

	}
void setContentEn(const TCHAR* tcContent)
	{
#ifdef UNICODE
		UINT uiDetect = WideCharToMultiByte(CP_OEMCP, NULL, tcContent, -1, NULL, 0, NULL, FALSE);
		UINT uiMaxTCHAR = 0;

		bool bIsMaxContent = false;

		if (uiDetect > UD_MAXBUFFER){
			bIsMaxContent = true;
		}
		else{
			uiMaxTCHAR = _tcslen(tcContent);
		}

		// ��������ַ���
		if(bIsMaxContent){
			UINT  uiContentLen = 0;
			TCHAR tcTemp[MAX_SEND_BUFFER];
			memset(tcTemp,'\0',sizeof(TCHAR)*MAX_SEND_BUFFER);

			uiContentLen = _tcslen(tcContent);
			_tcsncpy(tcTemp,tcContent,uiContentLen);

			// ���ַ�תխ�ַ�
			CW2AEX<> wChange(tcTemp,GB2312);

			DWORD dwDataLen = strlen(wChange);
			memmove(sText,wChange,dwDataLen);
		}
		// �������ַ������ڣ���Ҫ������ʾ
		else{
			TCHAR tcMAxTCHAR[UD_MAX_EN_BUFFER] = _T("\0");
			memset(tcMAxTCHAR, '\0', sizeof(TCHAR) * UD_MAXBUFFER);

			_tcsncpy(tcMAxTCHAR, tcContent, uiMaxTCHAR);

			TCHAR tcbuffer[UD_MAX_EN_LALFBUFFER] = _T("\0");

			UINT uilen = _tcslen(tcMAxTCHAR);
			UINT uileft = uilen / 2;
			UINT uirght = uilen - uileft;

			TCHAR tcleft[UD_MAX_EN_LALFBUFFER] = _T("\0");
			TCHAR tcright[UD_MAX_EN_LALFBUFFER] = _T("\0");;

			_tcsncpy(tcleft, tcMAxTCHAR, uileft);
			_tcsncpy(tcright, tcMAxTCHAR + uileft, uirght);

			TCHAR tcLeftContent[UD_MAX_EN_CONTENTLEN+1]= _T("\0");
			TCHAR tcRightContent[UD_MAX_EN_CONTENTLEN+1]= _T("\0");

			UINT uipartlen = _tcslen(tcleft);
			UINT uiCharCount = WideCharToMultiByte(CP_OEMCP, NULL, tcleft, -1, NULL, 0, NULL, FALSE);
			CString szformat;
			CString szCount;
			szCount.Format(_T(" %ds"), UD_MAX_EN_CONTENTLEN  - uiCharCount+1);
			szCount=_T("%")+szCount;
			szformat = szCount + _T("%s");
			_stprintf(tcbuffer, szformat, _T(""),tcleft);
			_tcsncpy_s(tcLeftContent, UD_MAX_EN_CONTENTLEN+1, tcbuffer, UD_MAX_EN_CONTENTLEN);
			tcLeftContent[UD_MAX_EN_CONTENTLEN]='\0';

			uipartlen = _tcslen(tcright);
			uiCharCount = WideCharToMultiByte(CP_OEMCP, NULL, tcright, -1, NULL, 0, NULL, FALSE);
			szformat = _T("%s% ");
			CString szcount;
			szcount.Format(_T("%ds"), UD_MAX_EN_CONTENTLEN - uiCharCount +1);
			szformat += szcount;
			_stprintf(tcbuffer, szformat, tcright, _T(""));
			_tcsncpy_s(tcRightContent, UD_MAX_EN_CONTENTLEN+1,tcbuffer,UD_MAX_EN_CONTENTLEN);
			tcRightContent[UD_MAX_EN_CONTENTLEN]='\0';

			CW2AEX<> w2aLeft(tcLeftContent,GB2312);
			CW2AEX<> w2aRight(tcRightContent,GB2312);

			int ileftchar=strlen(w2aLeft);
			int irightchar=strlen(w2aRight);
			memmove(sText,w2aLeft,ileftchar);
			memmove(sText+ileftchar,w2aRight,irightchar);
		}
#else

#endif

	}
}TDISPINFOTEXT;
#define UD_CLRPTR 3



/*-------------------------------------------------------------------------------
������:    Initialize
��ʼ����̬�⣻�ú���������ʾ��ͨѶ��
����:
����ֵ            :�������״̬���붨�塣
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pInitialize)();	//��ʼ����̬��

/*-------------------------------------------------------------------------------
������:    Uninitialize
��ʼ����̬�⣻�ú���������ʾ��ͨѶ��
����:
����ֵ            :�������״̬���붨�塣
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pUninitialize)();	//�ͷŶ�̬��

/*-------------------------------------------------------------------------------
������:    AddScreen
��̬���������ʾ����Ϣ���ú���������ʾ��ͨѶ��
����:
nControlType:��ʾ���Ŀ������ͺţ�Ŀǰ�ö�̬����̬��ֻ֧��BX-5E1��BX-5E2��BX-5E3��BX-5Eϵ�п�������
nScreenNo����ʾ�����ţ��ò�����LedshowTW 2013�����"��������"ģ���"����"����һ�¡�
nSendMode��ͨѶģʽ��Ŀǰ��̬����֧��0:����ͨѶ��2������ͨѶ(ֻ֧�̶ֹ�IPģʽ)��5�����浽�ļ�������ͨѶģʽ��
nWidth����ʾ����ȣ���λ�����ء�
nHeight����ʾ���߶ȣ���λ�����ء�
nScreenType����ʾ�����ͣ�1������ɫ��2��˫��ɫ��
nPixelMode���������ͣ�ֻ����ʾ������Ϊ˫��ɫʱ��Ч��1��R+G��2��G+R��
pCom��ͨѶ���ڣ�ֻ���ڴ���ͨѶʱ�ò�����Ч������"COM1"
nBaud��ͨѶ���ڲ����ʣ�ֻ���ڴ���ͨѶʱ�ò�����Ч��Ŀǰֻ֧��9600��57600���ֲ����ʡ�
pSocketIP����������IP��ַ��ֻ��������ͨѶ(�̶�IPģʽ)ģʽ�¸ò�����Ч������"192.168.0.199"
nSocketPort���������Ķ˿ڵ�ַ��ֻ��������ͨѶ(�̶�IPģʽ)ģʽ�¸ò�����Ч������5005
nServerMode     :0:������ģʽδ������1��������ģʽ������
pNetworkID      :������ģʽʱ������ID���
pServerIP       :��ת������IP��ַ
nServerPort     :��ת����������˿�
pServerAccessUser:��ת�����������û���
pServerAccessPassword:��ת��������������
pCommandDataFile�����浽�ļ���ʽʱ������������ļ����ơ�ֻ���ڱ��浽�ļ�ģʽ�¸ò�����Ч������"curCommandData.dat"
����ֵ:    �������״̬���붨�塣
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pAddScreen_Dynamic)(int nControlType, int nScreenNo, int nSendMode, int nWidth, int nHeight,
        int nScreenType, int nPixelMode, char* pCom, int nBaud, char* pSocketIP, int nSocketPort, int nStaticIpMode, int nServerMode,
        char* pBarcode, char* pNetworkID, char* pServerIP, int nServerPort, char* pServerAccessUser, char* pServerAccessPassword,
        char* pCommandDataFile);

/*-------------------------------------------------------------------------------
������:    QuerryServerDeviceList
��ѯ��ת�������豸���б���Ϣ��
�ú�������ʾ������ͨѶ
����:      nScreenNo, nSendMode: Integer
nScreenNo       :��ʾ�����ţ��ò�����AddScreen�����е�nScreenNo������Ӧ��
nSendMode       :����ʾ����ͨѶģʽ��
    0:����ģʽ��BX-5A2&RF��BX-5A4&RF�ȿ�����ΪRF��������ģʽ;
    2:����ģʽ;
    4:WiFiģʽ
    5:ONBON������-GPRS
    6:ONBON������-3G
pDeviceList       : �����ѯ���豸�б���Ϣ
    ���豸����Ϣ������ַ���, ���磺
    �豸1������ ������ ״̬ ���� ����ID
    �豸2������ ������ ״̬ ���� ����ID
    ����ַ���Ϊ��'�豸1����,�豸1������,�豸1״̬,�豸1����,�豸1����ID;�豸2����,�豸2������,�豸2״̬,�豸2����,�豸2����ID'
    �豸״̬(Byte):  $10:�豸δ֪
                    $11:�豸����
                    $12:�豸������

    �豸����(Byte): $16:�豸Ϊ2G
                    $17:�豸Ϊ3G
pDeviceCount      : ��ѯ���豸����

����ֵ            :�������״̬���붨�塣
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pQuerryServerDeviceList)(int nScreenNo, byte pDeviceList[], int &nDeviceCount);

/*-------------------------------------------------------------------------------
������:    BindServerDevice
����ת�������豸��
�ú���������ʾ������ͨѶ
����:      nScreenNo, nSendMode: Integer
nScreenNo       :��ʾ�����ţ��ò�����AddScreen�����е�nScreenNo������Ӧ��
pBarcode       :���豸�������룻
pNetworkId     :���豸������ID��

����ֵ            :�������״̬���붨�塣
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pBindServerDevice)(int nScreenNo, char* pBarcode, char* pNetworkId);

/*-------------------------------------------------------------------------------
������:    AddScreenDynamicArea
��̬����ָ����ʾ����Ӷ�̬���򣻸ú���������ʾ��ͨѶ��
����:
nScreenNo����ʾ�����ţ��ò�����AddScreen�����е�nScreenNo������Ӧ��
nDYAreaID����̬�����ţ�Ŀǰϵͳ�����5����̬���򣻸�ֵȡֵ��ΧΪ0~4;
nRunMode����̬��������ģʽ��
            0:��̬������ѭ����ʾ��
            1:��̬��������ʾ��ɺ�ֹ��ʾ���һҳ���ݣ�
            2:��̬������ѭ����ʾ�������趨ʱ���������δ����ʱ������ʾ��
            3:��̬������ѭ����ʾ�������趨ʱ���������δ����ʱ��ʾLogo��Ϣ,Logo ��Ϣ��Ϊ��̬��������һҳ��Ϣ
            4:��̬������˳����ʾ����ʾ�����һҳ��Ͳ�����ʾ
nTimeOut����̬�����ݳ�ʱʱ�䣻��λ���� 
nAllProRelate����Ŀ������־��
            1�����н�Ŀ����ʾ�ö�̬����
            0����ָ����Ŀ����ʾ�ö�̬���������̬����Ҫ�����ڽ�Ŀ��ʾ������һ������Ϊ�ա�
pProRelateList����Ŀ�����б��ý�Ŀ��ű�ʾ����Ŀ��ż���","����,��Ŀ��Ŷ���ΪLedshowTW 2013�����"P***"�е�"***"
nPlayImmediately����̬�����Ƿ���������0���ö�̬�������첽��Ŀһ�𲥷ţ�1���첽��Ŀֹͣ���ţ������Ÿö�̬����
nAreaX����̬������ʼ�����ꣻ��λ������
nAreaY����̬������ʼ�����ꣻ��λ������
nAreaWidth����̬�����ȣ���λ������
nAreaHeight����̬����߶ȣ���λ������
nAreaFMode����̬����߿���ʾ��־��0����ɫ��1����ɫ��255���ޱ߿�
nAreaFLine����̬����߿�����, ��ɫ���ȡֵΪFRAME_SINGLE_COLOR_COUNT����ɫ���ȡֵΪ��FRAME_MULI_COLOR_COUNT
nAreaFColor���߿���ʾ��ɫ��ѡ��Ϊ��ɫ�߿�����ʱ�ò�����Ч��
nAreaFStunt���߿������ؼ���
            0����˸��1��˳ʱ��ת����2����ʱ��ת����3����˸��˳ʱ��ת����
            4:��˸����ʱ��ת����5�����̽�����˸��6�����̽���ת����7����ֹ���
nAreaFRunSpeed���߿������ٶȣ�
nAreaFMoveStep���߿��ƶ���������ֵȡֵ��Χ��1~8��
����ֵ:    �������״̬���붨�塣
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pAddScreenDynamicArea)(int nScreenNo, int nDYAreaID, int nRunMode,
    int nTimeOut, int nAllProRelate, char* pProRelateList, int nPlayImmediately,
    int nAreaX, int nAreaY, int nAreaWidth, int nAreaHeight, int nAreaFMode, int nAreaFLine, int nAreaFColor,
    int nAreaFStunt, int nAreaFRunSpeed, int nAreaFMoveStep);
/*
������: DeleteScreenDynamicArea
ɾ����̬����ָ����ʾ��ָ���Ķ�̬������Ϣ���ú���������ʾ��ͨѶ��
������
nScreenNo	��ʾ�����ţ��ò�����AddScreen_Dynamic�����е�nScreenNo������Ӧ��
nDYAreaID	��̬�����ţ��ò�����AddScreenDynamicArea�����е�nDYAreaID������Ӧ
*/
typedef int(__stdcall *pDeleteScreenDynamicArea)(int nScreenNo, int nDYAreaID);

/*-------------------------------------------------------------------------------
������:    AddScreenDynamicAreaFile
��̬����ָ����ʾ����ָ����̬���������Ϣ�ļ����ú���������ʾ��ͨѶ��
����:
nScreenNo����ʾ�����ţ��ò�����AddScreen�����е�nScreenNo������Ӧ��
nDYAreaID����̬�����ţ��ò�����AddScreenDynamicArea�����е�nDYAreaID������Ӧ
pFileName����ӵ���Ϣ�ļ����ƣ�Ŀǰֻ֧��txt(֧��ANSI��UTF-8��Unicode�ȸ�ʽ����)��bmp���ļ���ʽ
nShowSingle��������Ϣ�Ƿ�����ʾ��0��������ʾ��1��������ʾ����ʾ�ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
pFontName��������Ϣ��ʾ���壻�ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nFontSize��������Ϣ��ʾ������ֺţ��ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nBold��������Ϣ�Ƿ������ʾ��0��������1��������ʾ���ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nFontColor��������Ϣ��ʾ��ɫ���ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nStunt����̬������Ϣ�����ؼ���
            00�������ʾ 
            01����ֹ��ʾ
            02�����ٴ�� 
            03�������ƶ� 
            04���������� 
            05�������ƶ� 
            06���������� 
            07����˸ 
            08��Ʈѩ 
            09��ð�� 
            10���м��Ƴ� 
            11���������� 
            12�����ҽ������� 
            13�����½������� 
            14������պ� 
            15������� 
            16���������� 
            17���������� 
            18���������� 
            19���������� 
            20���������� 
            21���������� 
            22���������� 
            23���������� 
            24�����ҽ�����Ļ 
            25�����½�����Ļ 
            26����ɢ���� 
            27��ˮƽ��ҳ 
            28����ֱ��ҳ 
            29��������Ļ 
            30��������Ļ 
            31��������Ļ 
            32��������Ļ 
            33�����ұպ� 
            34�����ҶԿ� 
            35�����±պ� 
            36�����¶Կ� 
            37�������ƶ� 
            38���������� 
            39�������ƶ� 
            40����������
nRunSpeed����̬������Ϣ�����ٶ�
nShowTime����̬������Ϣ��ʾʱ�䣻��λ��10ms
����ֵ:    �������״̬���붨�塣
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pAddScreenDynamicAreaFile)(int nScreenNo, int nDYAreaID,
    char* pFileName, int nShowSingle, char* pFontName, int nFontSize, int nBold, int nFontColor,
    int nStunt, int nRunSpeed, int nShowTime);

/*-------------------------------------------------------------------------------
������:    AddScreenDynamicAreaText
��̬����ָ����ʾ����ָ����̬���������Ϣ�ļ����ú���������ʾ��ͨѶ��
����:
nScreenNo����ʾ�����ţ��ò�����AddScreen�����е�nScreenNo������Ӧ��
nDYAreaID����̬�����ţ��ò�����AddScreenDynamicArea�����е�nDYAreaID������Ӧ
pText����ӵ���Ϣ�ļ����ƣ�Ŀǰֻ֧��txt(֧��ANSI��UTF-8��Unicode�ȸ�ʽ����)��bmp���ļ���ʽ
nShowSingle��������Ϣ�Ƿ�����ʾ��0��������ʾ��1��������ʾ����ʾ�ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
pFontName��������Ϣ��ʾ���壻�ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nFontSize��������Ϣ��ʾ������ֺţ��ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nBold��������Ϣ�Ƿ������ʾ��0��������1��������ʾ���ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nFontColor��������Ϣ��ʾ��ɫ���ò���ֻ��szFileNameΪtxt��ʽ�ĵ�ʱ����Ч��
nStunt����̬������Ϣ�����ؼ���
            00�������ʾ 
            01����ֹ��ʾ
            02�����ٴ�� 
            03�������ƶ� 
            04���������� 
            05�������ƶ� 
            06���������� 
            07����˸ 
            08��Ʈѩ 
            09��ð�� 
            10���м��Ƴ� 
            11���������� 
            12�����ҽ������� 
            13�����½������� 
            14������պ� 
            15������� 
            16���������� 
            17���������� 
            18���������� 
            19���������� 
            20���������� 
            21���������� 
            22���������� 
            23���������� 
            24�����ҽ�����Ļ 
            25�����½�����Ļ 
            26����ɢ���� 
            27��ˮƽ��ҳ 
            28����ֱ��ҳ 
            29��������Ļ 
            30��������Ļ 
            31��������Ļ 
            32��������Ļ 
            33�����ұպ� 
            34�����ҶԿ� 
            35�����±պ� 
            36�����¶Կ� 
            37�������ƶ� 
            38���������� 
            39�������ƶ� 
            40����������
nRunSpeed����̬������Ϣ�����ٶ�
nShowTime����̬������Ϣ��ʾʱ�䣻��λ��10ms
����ֵ:    �������״̬���붨�塣
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pAddScreenDynamicAreaText)(int nScreenNo, int nDYAreaID,
    char* pText, int nShowSingle, char* pFontName, int nFontSize, int nBold, int nFontColor,
    int nStunt, int nRunSpeed, int nShowTime);

/*-------------------------------------------------------------------------------
    ������:    DeleteScreen
    ɾ����̬����ָ����ʾ����������Ϣ���ú���������ʾ��ͨѶ��
    ����:
    nScreenNo����ʾ�����ţ��ò�����AddScreen�����е�nScreenNo������Ӧ��
    ����ֵ:    �������״̬���붨��
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pDeleteScreen_Dynamic)(int nScreenNo);

/*-------------------------------------------------------------------------------
    ������:    DeleteScreenDynamicAreaFile
    ɾ����̬����ָ����ʾ��ָ���Ķ�̬����ָ���ļ���Ϣ���ú���������ʾ��ͨѶ��
    ����:
    nScreenNo����ʾ�����ţ��ò�����AddScreen�����е�nScreenNo������Ӧ��
    nDYAreaID����̬�����ţ��ò�����AddScreenDynamicArea�����е�nDYAreaID������Ӧ
    nFileOrd����̬�����ָ���ļ����ļ���ţ�����Ű����ļ����˳�򣬴�0˳���������ɾ���м���ļ���������ļ�����Զ���䡣
    ����ֵ:    �������״̬���붨��
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pDeleteScreenDynamicAreaFile)(int nScreenNo, int nDYAreaID, int nFileOrd);

/*-------------------------------------------------------------------------------
    ������:    SendDynamicAreasInfoCommand
    ���Ͷ�̬����ָ����ʾ��ָ���Ķ�̬������Ϣ����ʾ�����ú�������ʾ��ͨѶ��
    ����:
    nScreenNo����ʾ�����ţ��ò�����AddScreen�����е�nScreenNo������Ӧ��
    nDelAllDYArea	��̬�������б�1��ͬʱ���Ͷ����̬����0�����͵�����̬����
	pDYAreaIDList	��̬�����ţ���nDelAllDYAreaΪ1ʱ����ֵΪ�� ������nDelAllDYAreaΪ0ʱ���ò�����AddScreenDynamicArea�����е�nDYAreaID������Ӧ��������Ӧ��̬����
    ����ֵ:    �������״̬���붨��
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pSendDynamicAreasInfoCommand)(int nScreenNo, int nDelAllDYArea, char* pDYAreaIDList);

/*-------------------------------------------------------------------------------
    ������:    SendDeleteDynamicAreasCommand
    ɾ����̬����ָ����ʾ��ָ���Ķ�̬������Ϣ��ͬʱ����ʾ��ͨѶɾ��ָ���Ķ�̬������Ϣ���ú�������ʾ��ͨѶ
    ����:
    nScreenNo����ʾ�����ţ��ò�����AddScreen�����е�nScreenNo������Ӧ��
    nDelAllDYArea	��̬�������б�1��ͬʱɾ�������̬����0��ɾ��������̬����
	pDYAreaIDList	��̬�����ţ���nDelAllDYAreaΪ1ʱ����ֵΪ�� ������nDelAllDYAreaΪ0ʱ���ò�����AddScreenDynamicArea�����е�nDYAreaID������Ӧ��ɾ����Ӧ��̬����
    ����ֵ:    �������״̬���붨��
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pSendDeleteDynamicAreasCommand)(int nScreenNo, int nDelAllDYArea, char* pDYAreaIDList);

class STATUS_DISPLAY_API CLedDispInfo
{
public:
	CLedDispInfo();
	~CLedDispInfo();
private:
	TDISPINFO m_dispInfo;		 //������ʾ����
	TDISPINFOTEXT m_dispInfoText;//������ʾ����

public:
	BOOL m_bSendBusy;//�˱��������ݸ����зǳ���Ҫ������ر�����
	int g_nSendMode;//ͨѶģʽֵ����
	
public:
	BOOL addScene(TDISPINFO);
	BOOL addSceneText(TDISPINFOTEXT);
	byte* getDispCommandBytes();
	byte* getDispTextCommandBytes();
};

#endif