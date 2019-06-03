#ifndef _LED_API_H_
#define _LED_API_H_
#pragma once
/*****************************************************************************/
/*                                                                           */
/*  Include Files                                                            */
/*                                                                           */
/*****************************************************************************/
// apidef.h�ж����˽ӿڵĵ���/����
//#include "Apidef.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __UINT
#define __UINT
typedef unsigned int UINT;
#endif

#ifndef __BYTE
#define __BYTE
typedef unsigned char BYTE;
#endif

#ifndef uchar
#define uchar	unsigned char	// 1 byte
#endif

#define				LED_NO_ERROR				1800		// ״̬����
#define				LED_ERR_INVALID_CMD			1851		// ��Ч����
#define				LED_ERR_HARDWARE_ERR		1852		// ����
#define				LED_ERR_COMPORT_ERR			1853		// �������ô���
#define				LED_ERR_TIME_OUT			1854		// ͨѶ��ʱ
#define				LED_ERR_INVALID_PARA		1855		// �����������

//#pragma pack(push,1)

//11.3.1��������״̬�����ṹ�� 
typedef  struct 
{
	//1.������Ϣ
	UINT uiProviderId; //ģ�鳧�ұ�ʶ������0x02�����0x04������0x06��ŷķ��0x05��
	UINT uiModuleType; //ģ�����ͱ�ʶ 0x09 LEDģ��
	UINT uiType; //�������ͣ�0-������1-���棻2-����
	UINT uiStdErrorCode[16];  //ͳһά���Ĵ������
	UINT uiErrorCode[16];  //������� ���ɲ�ͬģ�鳧���ṩ��  
	char acDevReturn[128]; //  Ӳ��������Ϣ 
	char acReserve[128]; //  ������Ϣ
} tLEDDevReturn;

//11.3.2��ģ��״̬�ṹ�� 
typedef  struct 
{
	UINT uiModuleStatus;  //ģ��״̬��0-������1:-����
} tLEDModuleStatus;


//11.3.4����ʾ���ݽṹ�� 
typedef  struct 
{
	UINT 		nSerialNO;	//���
	char	   	cCNText[256];		// ��Ҫ��ʾ������
	char	   	cENText[256];		// ��Ҫ��ʾ��Ӣ��
	UINT		iFontColor;// ������ɫ��0 ~ 6 �ֱ��Ӧ �졢�̡��ơ������ϡ��ࡢ�׵���ɫ��
	UINT		iStayTime;// ͣ��ʱ�䡣ͣ��ʱ�䣬0 ~ 9��0-��̣�9-���Ĭ��ֵΪ5���������/Ӣ��һ��������ʾʱ���˲�����Ч��ͬʱ������ʾ����������ɲ��ù������š��������/Ӣ�Ŀ�һ����ʾ����Ӣ���л�ʱ����������ʧ����������ʱ���ô�����������������š�
} tLED_Download_Text_Para;
//11.3.3���������ݽṹ�� 
typedef  struct 
{
	UINT 		uiTextNum;		//��Ҫ��ʾ�ļ�¼����
	tLED_Download_Text_Para* pLED_Download_Text_Para;
} tLED_Download_Para;

#pragma pack(pop)
	//11.4.1��ͨ�Ž���
	 int  LED_CommOpen(UINT uiCommPort, UINT uiBaudRate);
	//11.4.2����ʼ��
	 int  LED_Init(tLEDDevReturn*  pDevStatus);
	//11.4.3��ģ�鸴λ
	 int  LED_Reset(tLEDDevReturn*  pDevStatus);
	//11.4.4��������ʾ��Ϣ
	//���������ڽ���Ҫ��ʾ����Ϣ���ص�LED���ư��ϻ򱣴����ڴ������
	 int  LED_Download(tLED_Download_Text_Para pLED_Download_Para,tLEDDevReturn *pDevStatus);
	//11.4.5����ʾ��Ϣ
	//���������ڸ���ѡ�����Ž�������LED���ư��ϻ��ڴ�����е��ַ������ݽ�����ʾ��
	//Ҫ����ʾ�����л�ʱ����������ʾ
	//UINT	nSerialNO	In	����ʾ�����ݱ�š���tLED_Download_Text_Para:: nSerialNO��Ӧ��
	 int  LED_Display(UINT nSerialNO, tLEDDevReturn * pDevStatus);
	//11.4.6����ȡģ��״̬
	 int  LED_GetDevStatus(tLEDModuleStatus *pModuleStatus,tLEDDevReturn *pDevStatus);
	//11.4.7����ȡģ�������汾��Ϣ
	 int LED_GetVersion(char* cFirmWareVersion, char* cDriverVersion, tLEDDevReturn *pDevStatus);
	//11.4.8��ͨ�Źر�
	 int LED_CommClose();
#if defined (__cplusplus)
} // end of extern "C"
#endif




#endif