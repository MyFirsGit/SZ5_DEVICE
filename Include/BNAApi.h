//ֽ�ҽ���ģ��
#ifndef _BNA_API_H
#define _BNA_API_H

#include "Apidef.h"


#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push,1)
//��������״̬�����ṹ�� 
typedef  struct 
{
	//������Ϣ
	UINT uiProviderId; //ģ�鳧�ұ�ʶ
	int iType;   //�������ͣ�0-������1-���棻2-����
	int iStdErrorCode;  //ͳһά���Ĵ������
	//2.ģ���ԭʼ��Ϣ������λ������ʾά���� 
	int iErrorCode;  //������� ���ɲ�ͬģ�鳧���ṩ��  
	char acDevReturn[128]; //  Ӳ��������Ϣ 
	char acReserve[128]; //  ������Ϣ    
} tBNASTDDevReturn;	
//����Ʊϵ�н�����Ϣ�ṹ��
typedef struct _SetInhibitItem
{
	int  iValue;    //����λΪ��
	bool  bAccepted; //�Ƿ����  
}tBNASetInhibitItem;
typedef struct  _SetInhibitList
{ 
	int iNumOfItems; //�ֽ������
	tBNASetInhibitItem  *psBnaCashInfo;//�ֽ������ַָ��
} tBNASetInhibitList;
//���ֽ���Ϣ�ṹ��
typedef struct _BnaCashItem
{
	unsigned int  iValue; //���ճ�Ʊ����λΪ��
	unsigned int  iCount; //��Ʊ����
}tBNACashItem;
typedef struct _BnaCashInfo
{
	int iNumOfItems; //�ֽ������
	tBNACashItem  *psBnaCashInfo;
}tBNACashInfo;
//��ֽ�ҽ���Ǯ��״̬�ṹ��
typedef struct _BnaStatus
{
	int iModuleStatus; /* 0 ������1 �踴λ 2 �쳣 */
	int iBoxStatus;  /* 0 ��װ���������� 1 ���� 2 �� 3 δ��װ */
	int iInStacker; /* 0 �ޣ� 1 �г�Ʊ���ݴ� */
}tBNAStatus;

#pragma pack(pop)

/*****************************************************************************/
/*                                                                           */
// ���½ӿ���LIBʵ�֣���DLL��ʹ�ã���������ṩdll��ɷ���TIMģ��ʵ�֣��������
// �ṩ����Э�飬�����װ��Э�����ʵ�����½ӿ�
/* # wang feng                                                               */
/*****************************************************************************/
//����ʼ��
DLLIMP int BNA_Init(tBNASTDDevReturn*  p_psStatus);
//������ʶ����ó�Ʊ
DLLIMP int BNA_SetPrevalue(tBNASetInhibitList *p_psBuyTicketSetInhibitList, tBNASetInhibitList *p_psRechargeSetInhibitList, tBNASTDDevReturn * p_psStatus);
//����λ
DLLIMP int BNA_Reset(int iMode, int iRetryTimes, BYTE * p_byNotesRefunded, BYTE * p_byNotesEncashed,tBNASTDDevReturn* p_psStatus);
//����ʼ���ճ�Ʊ
DLLIMP int BNA_Start(tBNASTDDevReturn * p_psStatus);
//��ֹͣ���ճ�Ʊ
DLLIMP int BNA_Stop(tBNASTDDevReturn * p_psStatus);
//  ��ȡʶ��Ʊ��Ϣ
DLLIMP int BNA_GetCashInfo(tBNACashInfo *p_psCashInfo, BYTE& p_byOperation, tBNASTDDevReturn *p_psStatus);
//��ѹ��
DLLIMP int BNA_Encash(tBNASTDDevReturn *p_psStatus);
//�˳�
DLLIMP int BNA_Refund(tBNASTDDevReturn *p_psStatus);
//  ��ȡģ��״̬
DLLIMP int BNA_GetStatus(tBNAStatus *p_psBnaStatus,tBNASTDDevReturn * p_psStatus);
//	��ȡǮ��ID
DLLIMP int BNA_GetBoxID(char * p_BoxID, tBNASTDDevReturn * p_psStatus);
//��ͨ�Ŵ��ڴ�
DLLIMP int BNA_open(unsigned int uiCommPort, unsigned int uiBaudRate);
//��ͨ�Ŵ��ڹر�
DLLIMP int BNA_close();	
//����ȡģ��̼��汾
DLLIMP int BNA_GetVersion(char* pVersion, tBNASTDDevReturn * p_psStatus);

#if defined (__cplusplus)
} // end of extern "C"
#endif

#endif



