#ifndef _PRT_API_H
#define _PRT_API_H

#include "Apidef.h"

#define		PRT_ERR_LESS_PAPER			1611		//��ӡ����ֽ
#define		PRT_ERR_WARING				1612		//�������
#define		PRT_ERR_INVALID_CMD			1651		//��Ч����
#define		PRT_ERR_NO_PAPER			1652		//��ӡ����ֽ
#define		PRT_ERR_CUT_ERROR			1653		//��ӡ��ֽ��
#define		PRT_ERR_ERROR				1654		//��ӡ������
#define		PRT_ERR_STUCK_PAPER			1655		//��ֽ
#define		PRT_ERR_PORT_ERR			1656		//�������ô�
#define		PRT_ERR_TIME_OUT			1657		//ͨѶ��ʱ
#define		PRT_ERR_PARAM_ERR			1658		//�����������

#pragma pack(push,1)

//������״̬�����ṹ�� 
typedef  struct 
{ 
	//1.������Ϣ
	UINT uiProviderId; //ģ�鳧�ұ�ʶ������0x02�����0x04������0x06��ŷķ��0x05��
	UINT uiModuleType; //ģ�����ͱ�ʶ 0x07��ӡ��ģ��
	UINT uiType; //�������ͣ�0-������1-���棻2-����
	UINT uiStdErrorCode[16];  //ͳһά���Ĵ������
	UINT uiErrorCode[16];  //������� ���ɲ�ͬģ�鳧���ṩ��  
	char acDevReturn[128]; //  Ӳ��������Ϣ 
	char acReserve[128]; //  ������Ϣ    
} tPRTDevReturn;

//ģ��״̬�ṹ�� 

typedef  struct 
{ 
	UINT uiPaperStatus;   //��ӡֽ״̬��0-������1-��ֽ��2-��ֽ
	UINT uiModuleStatus;  //ģ��״̬��0-������1:-����
} tPRTModuleStatus;


#pragma pack(pop)
#ifdef __cplusplus
extern "C" {
#endif

	//��ʼ��ģ�飬���ģ���Ӳ������ģ��ָ�����ʼ״̬��,�豸��������Ҫ���ñ�����
	DLLIMP int  PRT_Init(tPRTDevReturn*  pDevStatus);
	//���豸���ϻ��ά��ģʽ�˳�ʱ������֧��ά��ģʽ���������ִ�б�����
	DLLIMP int  PRT_Reset(tPRTDevReturn*  pDevStatus);
	//����˵�������������ڽ�������ַ������д�ӡ��
	//����˵�����±� 
	//	��248��PRT_Print����˵�� 
	//	���	��������	��������	����	˵��
	//	1	Char*	cPrintData	In	���ӡ�����ݡ�����/0��β��
	//	2	bool	bCutPaper	In	�Ƿ���ֽ��true����ֽ��false������ֽ��
	//	3	tPRTDevReturn *	pDevStatus 	Out	����״̬��Ϣ 
	DLLIMP int  PRT_Print(CStringArray* m_sArrPrinter,bool m_bOpenBox,bool bCutPaper,tPRTDevReturn *pDevStatus);
	//����˵�������ô˽ӿڿɻ�ȡģ�鵱ǰ״̬��
	//	����˵�����±� 
	//	��250��PRT_GetDevStatus ()����˵�� 
	//	���	��������	��������	����	˵��
	//	1	tPRTModuleStatus *	pModuleStatus 	Out	����ģ��״̬��Ϣ 
	//	2	tPRTDevReturn *	pDevStatus 	Out	����״̬��Ϣ 
	DLLIMP int  PRT_GetDevStatus(tPRTModuleStatus *pModuleStatus,tPRTDevReturn *pDevStatus);

	//����˵������ȡ�豸�����汾�š��̼��汾�š�״̬��Ϣ��
	//	����˵�����±� 
	//	��252��PRT_GetVersion ()����˵�� 
	//	���	��������	��������	����	˵��
	//	1	char	cFirmWareVersion[32]	out	���ع̼��汾��
	//	2	char	cDriverVersion[32]	out	���������汾��
	//	3	tPRTDevReturn *	pDevStatus 	Out	����״̬��Ϣ 
	DLLIMP int  PRT_GetVersion(char cFirmWareVersion[32], char cDriverVersion[32], tPRTDevReturn *pDevStatus);

	DLLIMP int  PRT_CommOpen(UINT uiCommPort, UINT uiBaudRate, CString path, CString fileName);
	DLLIMP int  PRT_CommClose();

#if defined (__cplusplus)
} // end of extern "C"
#endif

#endif
