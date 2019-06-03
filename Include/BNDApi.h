//ֽ������ģ��
#ifndef _BND_API_H
#define _BND_API_H

#include "Apidef.h"

#pragma pack(push,1)

//������״̬�ṹ�� 
typedef  struct 
{
	UINT uiType;	/*�������ͣ�0-������1-���棻2-����*/	
	UINT uiErrorType;/*�������ͣ�0-������1-���������2-Ӳ�����ϣ�3-ͨ���쳣��4-Ǯ�����*/
	int  iHandle;	/*��������0-������ 1-��ʼ�� 2-�ط����� 3-ͣ����Ϣ 4-�����Ӳ����������*/
	int  iStdErrorCode;     /*ͳһά���Ĵ������*/		
	UINT uiErrorCode;/*������루�ɲ�ͬģ�鳧���ṩ��*/
	char cReverse[128]; /*Ԥ����Ϣ*/
} tBNDDevReturn;

//����ʼ��������Ϣ
typedef struct
{
	UINT uiOutNumber;
	UINT uiRetractNumber;
}tBndInitNumInfo;

#ifndef MAX_BOXNUM
#define MAX_BOXNUM 6
#endif
//��Ǯ��״̬�ṹ��
typedef struct
{
	unsigned char uzBoxStatus[MAX_BOXNUM];
	unsigned char ucNgBoxStatus;
}tBndBoxStatus;

//������Ǯ��ṹ�� 
typedef struct
{
	UINT uiBoxType;   //�������ͣ�0-�ϱ��� 1-1Ԫֽ����, 2-5Ԫֽ����, 3-10Ԫֽ����, 4-20Ԫֽ����, 5-50Ԫֽ����, 6-100Ԫֽ����*
	UINT uiBoxNo;     /**������ţ�0 ~ 6 ���У� 0�ŷϱ��� 1��Ϊ5Ԫ�����䣬2��Ϊ1Ԫ������ */
	UINT uiBillValue; /**��λ�֣����������ܴ��ֽ�ҵ�������0��ʾ����ȷ�������������*/
	UINT uiBillNum;   /**������ֽ�ҵ�����*/
	UINT uiBillMaxNum;/**�������ܴ���ֽ����������*/
	UINT uiBoxStatus; /**������״̬��0-������1-δ��װ��2-������3-������4-�Կգ�5-����,��״̬Ϊģ��Ӳ������״̬��ͨ�������жϵ�״̬����λ�������ж�*/
} tBndCashBoxValueInfo;
//��ģ������������Ϣ�ṹ��
typedef  struct
{
	tBndCashBoxValueInfo pCashBoxValueInfo[MAX_BOXNUM];
	tBndCashBoxValueInfo stCashNGBoxValueInfo;;
}tBNDBillBoxInfo;

//���䳮�ṹ��
typedef struct
{
	UINT uiAmount;				      //����
	UINT uiDispenseNumber[MAX_BOXNUM];//ÿ��Ǯ����Ҫ��Ǯ������
	UINT uiOutNumber[MAX_BOXNUM];    //ʵ��ÿ��Ǯ���Ǯ�����������أ�
	UINT uiRejectNumber[MAX_BOXNUM]; //����������ÿ��Ǯ��ϳ����յ����������أ�
} tBNDChangeNum;
//����ƽṹ��
typedef struct
{
	UINT uiNGBoxAccInNum;       //�ۼƽ���ϳ�������
	UINT uiBoxAccOutNum;       //�ۼƳ�������
}tBndAuditCashNum;

//��Rfid��Ϣ�ṹ��
typedef struct
{
	unsigned char uzCashBoxID[4]; // Ǯ����
	unsigned char uzOperateID[3]; // ����ԱID
	unsigned char uzDeviceID[4];// �豸ID
	unsigned char ucCashBoxStatus;// Ǯ��λ��״̬
	unsigned char ucCashBoxOperateStatus;// ������Ǯ��״̬
	unsigned char ucBillType;// ���ִ���
	unsigned char usBillCnt;// ����
	unsigned int  uiBillAmount;// ֽ���ܽ��
	unsigned char ucStationCode[2];// ��վ����
	unsigned char ucInstallPosit;// ��װλ��
	unsigned int  uiBlockOperateTime;// �����ʱ��
	unsigned char ucOperateTime[7];// ������ʱ��
	unsigned short usCheckField;// У���ֶ�
}tBndRfidInfo;

#pragma pack(pop)

/*****************************************************************************/
/*                                                                           */
// ���½ӿ���LIBʵ�֣���DLL��ʹ�ã���������ṩdll��ɷ���TIMģ��ʵ�֣��������
// �ṩ����Э�飬�����װ��Э�����ʵ�����½ӿ�
/* # wang feng                                                               */
/*****************************************************************************/

//��ͨ�Ŵ��ڴ�
// 	����˵�������ô˽ӿڿɴ򿪵Ͳ㴮��ͨ�š�
// 	����˵�����±�
// 	1	unsigned int	uiCommPort	In	����ͨ�ź�
// 	2	unsigned int	uiBaudRate	In	������
// 	3	tBNDDevReturn	pDevStatus	Out	�豸����״̬
// 	����ֵ˵�����±�
// 	1	Int	0	�ɹ�
// 	2	Int	1	���ڴ򿪳���
DLLIMP int BND_CommOpen(UINT uiCommPort, UINT uiBaudRate, tBNDDevReturn *pDevStatus);

//��ͨ�Ŵ��ڹر�
// 	����˵�������ô˽ӿڿɹرյͲ㴮��ͨ�š�
// 	����˵���μ��±�
// 	1	tBNDDevReturn	pDevStatus	Out	�豸����״̬
// 	����ֵ˵�����±�
// 	1	Int	0	�ɹ�
// 	2	Int	1	�رմ���
DLLIMP int BND_CloseComm(tBNDDevReturn *pDevStatus);	


//����ʼ��
// 	����˵���� ��ʼ��ģ�飬���ģ���Ӳ������ģ��ָ�����ʼ״̬�ȡ�
// 	����˵�����±�
// 	1	tBndInitNumInfo *	pNumInfo	Out	�˳��ͻ���Ǯ����Ϣ
// 	2	tBNDDevReturn	pDevStatus	Out	            �豸����״̬
// 
// 	����ֵ˵�����±�
// 	1	Int	0	�ɹ�
// 	2	Int	1	�������Ϊ��
DLLIMP int  BND_Init(tBndInitNumInfo *pNumInfo, tBNDDevReturn * pDevStatus); 

//����λ
// 	����˵������λ��
// 	����˵�����±�
// 	1	tBndInitNumInfo*	pNumInfo	Out	�˳��ͻ���Ǯ������
// 	2	tBNDDevReturn	pDevStatus	Out	�豸����״̬
// 
// 	����ֵ˵�����±�
DLLIMP int BND_Reset(tBndInitNumInfo *pNumInfo, tBNDDevReturn *pDevStatus);

//����ȡģ��״̬
// 	����˵�������ô˽ӿڿɻ�ȡģ�鵱ǰ״̬����ϸ��Ϣ�鿴���ؽṹ�塣
// 	����˵�����±�
// 	1	tBndBoxStatus	pBndBoxStatus	Out	����Ǯ��״̬
// 	2	tBNDDevReturn *	tBNDDevReturn	Out	����BND���ص��߼�������
// 
// 	����ֵ˵�����±�
// 	1	Int	0	�ɹ�
// 	2	Int	1	�������Ϊ��
DLLIMP int BND_GetDevStatus(tBndBoxStatus *pBndBoxStatus, tBNDDevReturn * tBNDDevReturn);

//����ȡ�����Ϣ
// 	����˵������ȡ�豸�������Ϣ��
// 	����˵�����±�
// 	1	tBndAuditCashNum *	pBndAuditCashNum	Out	����������Ϣ
// 	2	tBNDDevReturn *	pDevStatus	Out	����״̬��Ϣ
// 
// 	����ֵ˵�����±�
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BND_GetAuditData(tBndAuditCashNum *pBndAuditCashNum, tBNDDevReturn *pDevStatus);

//����ȡ����������Ϣ
// 	����˵������ȡǮ��ṹ����Ϣ��
// 	����˵�����±�
// 	1	tBNDBillBoxInfo *	pBillBoxInfo	Out	����Ǯ��ṹ����Ϣ
// 	2	tBNDDevReturn *	pDevStatus	Out	����״̬��Ϣ

// 	����ֵ����
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BND_GetBillBoxInfo(tBNDBillBoxInfo *pBillBoxInfo, tBNDDevReturn *pDevStatus);


//����������������Ϣ
// 	����˵������������������Ϣ������ֽ��������
// 	����˵�����±�
// 	1	UNIT	uiSlotID	In	��λID
// 	2	tBNDBillBoxInfo *	pBillBoxInfo	In	����������Ϣ
// 	3	tBNDDevReturn *	pDevStatus	Out	����״̬��Ϣ
// 
// 	����ֵ���±�
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BND_SetBillBoxInfo(UINT uiSlotID, tBNDBillBoxInfo *pBillBoxInfo, tBNDDevReturn *pDevStatus);


//�������䳮
// 	����˵���������䳮
// 	����˵�����±�
// 	1	tBNDChangeNum *	pChangeNum	In	�����ֽ����������Ϣ
// 	2	tBNDDevReturn *	pDevStatus	Out	����״̬��Ϣ
// 
// 	����ֵ˵�����±�
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BND_ChangeBill(tBNDChangeNum *pChangeNum, tBNDDevReturn *pDevStatus);

//����ʼ����ֽ��������
// 	����˵�����˽ӿ����ڸ���ֽ�������䡣
// 	����˵�����±�
// 	��28��BND_StartReplaceBox()����˵��
// 	���	��������	��������	����/���	˵��
// 	1	UNIT	uiBoxType	In	ָ��ֽ������������
// 	2	UINT	uiBoxNo	In	�ƶ�ֽ�����������
// 	3	tBNDDevReturn *	pDevStatus	Out	����״̬��Ϣ
// 	����ֵ���±�
// 	��29��BND_StartReplaceBox()����ֵ˵��
// 	���	����	˵��
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BND_StartReplaceBox(UINT uiBoxType, UINT uiBoxNo, tBNDDevReturn *pDevStatus);

//��ֹͣ����ֽ��������
// 	����˵����ֹͣ����ֽ�������䡣
// 	����˵�����±�
// 	1	UNIT	uiBoxType	In	ָ��ֽ����������
// 	2	UNIT	uiBoxNo	In	ָ��ֽ���������
// 	2	tBNDDevReturn *	p_psStatus	Out	����BND���ص��߼�������
// 
// 	����ֵ˵�����±�
// 	1	Int	0	�ɹ�
// 	2	Int	1	�������Ϊ��
DLLIMP int BND_StopReplaceBox(UINT uiBoxType, UINT uiBoxNo, tBNDDevReturn *pDevStatus);

//����ȡ���к�
// 	����˵��������������ģ������кš�
// 	����˵�����±�
// 	1	Unsigned char*	pSn	Out	ģ������кţ�����32
// 	2	tBNDDevReturn *	pDevStatus	Out	����״̬��Ϣ
// 
// 	����ֵ˵�����±�
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BND_GetSerialNumber(unsigned char * pSn, tBNDDevReturn * pDevStatus);

//����ȡ�̼����
// 	����˵��������������ģ��Ĺ̼���š�
// 	����˵�����±�
// 	1	Unsigned char*	pVersion	Out	ģ��Ĺ̼����
// 	2	tBNDDevReturn *	pDevStatus	Out	����״̬��Ϣ
// 
// 	����ֵ˵�����±�
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BND_GetFWVerdion (unsigned char * pVersion, tBNDDevReturn * pDevStatus);

//��ȡ������
// 	����˵����ȡ�����������
// 	����˵�����±�
// 	1	tBNDDevReturn *	p_psStatus	Out	����BND���ص��߼�������
// 
// 	����ֵ˵�����±�
// 	1	Int	0	�ɹ�
// 	2	Int	1	�������Ϊ��
DLLIMP int  BND_Cancel (tBNDDevReturn *p_psStatus);

//������Ǯ��RFID��Ϣ
// 	����˵����д��Ǯ��RFID��Ϣ��
// 	����˵�����±�
// 	1	UNIT	uiSLotID	In	��λID
// 	2	tBndRfidInfo *	pRFIDInfo	In	����RFID��Ϣ
// 	3	tBNDDevReturn *	pDevStatus	Out	����״̬��Ϣ
// 
// 	����ֵ���±�
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BND_WriteRFIDInfo(UINT uiSLotID, tBndRfidInfo* pRFIDInfo, tBNDDevReturn * pDevStatus);

//����ȡǮ��RFID��Ϣ
// 	����˵������ȡǮ��RFID��Ϣ��
// 	����˵�����±�
// 	1	UNIT	uiSLotID	In	��λID
// 	2	tBndRfidInfo *	pRFIDInfo	Out	����RFID��Ϣ
// 	3	tBNDDevReturn *	pDevStatus	Out	����״̬��Ϣ
// 
// 	����ֵ˵�����±�
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BND_ReadRFIDInfo(UINT uiSLotID, tBndRfidInfo* pRFIDInfo, tBNDDevReturn * pDevStatus);

#endif
