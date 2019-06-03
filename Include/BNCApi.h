//ֽ��ѭ��ģ��
#ifndef _BNC_API_H
#define _BNC_API_H

#include "Apidef.h"

#define MAX_BOXNUM 6

#pragma pack(push,1)


   // error code
#define BNC_ERR_CODE_NONE               0     // No error

//���ṹ��˵��
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
} tBncDevReturn;

// �豸��ϸ״̬
typedef struct tBncSysDevStatus
{
	bool bIsCollectBoxInstalled;
	bool bIsChangeBoxInstalled;
	tBncSysDevStatus(){
		bIsChangeBoxInstalled = true;
		bIsCollectBoxInstalled = true;
	}
};

//����ʼ��������Ϣ
typedef struct
{
	UINT uiOutNumber;//�˳�����
	UINT uiRetractNumber;//��������
}tBncInitNumInfo;

//����Ʊϵ�н�����Ϣ�ṹ��
typedef struct tBncSetInhibitItem
{
	int  iValue;    //����λΪ��
	bool  bAccepted; //�Ƿ����  
	tBncSetInhibitItem(){
		bAccepted = false;
	}
};
typedef struct tBncSetInhibitList
{ 
	int iNumOfItems; //�ֽ������
	tBncSetInhibitItem  *psBncCashInfo;//�ֽ������ַָ��
	tBncSetInhibitList(){
		iNumOfItems = 0;
		psBncCashInfo = NULL;
	}
};

//���ֽ���Ϣ�ṹ��
typedef struct _BncCashItem
{
    unsigned int  iValue; //���ճ�Ʊ����λΪ��
    unsigned int  iCount; //��Ʊ����
}tBncCashItem;
typedef struct _BncCashInfo
{
    int iNumOfItems; //�ֽ������
    tBncCashItem  *psBncCashInfo;
	_BncCashInfo(){
		iNumOfItems = 0;
		psBncCashInfo = NULL;
	}
}tBncCashInfo;
// Ѻ������
typedef struct tBncEnCashInfo{
	DWORD dwCashIntoAmount;

	WORD wRecyclerAIntoCount;
	WORD wRecyclerBIntoCount;
	WORD wRecyclerCIntoCount;
	WORD wRecyclerDIntoCount;

	WORD wCashBox_CNY1_IntoCount;
	WORD wCashBox_CNY5_IntoCount;
	WORD wCashBox_CNY10_IntoCount;
	WORD wCashBox_CNY20_IntoCount;
	WORD wCashBox_CNY50_IntoCount;
	WORD wCashBox_CNY100_IntoCount;

	tBncEnCashInfo(){
		dwCashIntoAmount = 0;

		wRecyclerAIntoCount = 0;
		wRecyclerBIntoCount = 0;
		wRecyclerCIntoCount = 0;
		wRecyclerDIntoCount = 0;

		wCashBox_CNY1_IntoCount = 0;
		wCashBox_CNY5_IntoCount = 0;
		wCashBox_CNY10_IntoCount = 0;
		wCashBox_CNY20_IntoCount = 0;
		wCashBox_CNY50_IntoCount = 0;
		wCashBox_CNY100_IntoCount = 0;
	}

};//Ѻ������
//��ֽ��Ǯ��״̬�ṹ��
typedef struct 
{
	int iModuleStatus; /* 0 ������1 �踴λ 2 �쳣 */
	int iBoxStatus;  /* 0 ��װ���������� 1 ���� 2 �� 3 δ��װ */
	int iInStacker; /* 0 �ޣ� 1 �г�Ʊ���ݴ� */
}tBncStatus;

//������Ǯ��ṹ�� 
typedef struct
{
	UINT uiBoxType;   //�������ͣ�0-�ϱ��� 1-1Ԫֽ����, 2-5Ԫֽ����, 3-10Ԫֽ����, 4-20Ԫֽ����, 5-50Ԫֽ����, 6-100Ԫֽ����*
	UINT uiBoxNo;     /**������ţ�0 ~ 6 ���У� 0�ŷϱ��� 1��Ϊ5Ԫ�����䣬2��Ϊ1Ԫ������ */
	UINT uiBillValue; /**��λ�֣����������ܴ��ֽ�ҵ�������0��ʾ����ȷ�������������*/
	UINT uiBillNum;   /**������ֽ�ҵ�����*/
	UINT uiBillMaxNum;/**�������ܴ���ֽ����������*/
	UINT uiBoxStatus; /**������״̬��0-������1-δ��װ��2-������3-������4-�ѿգ�5-����,��״̬Ϊģ��Ӳ������״̬��ͨ�������жϵ�״̬����λ�������ж�*/
} tBncCashBoxValueInfo;

//��ģ������������Ϣ�ṹ��
typedef  struct
{
	tBncCashBoxValueInfo pCashBoxValueInfo[MAX_BOXNUM];
	tBncCashBoxValueInfo stCashNGBoxValueInfo;
}tBncBillBoxInfo;

//���䳮�ṹ��/���Ǯ�䷴��
typedef struct
{
	UINT uiAmount;				      //����
	UINT uiDispenseNumber[MAX_BOXNUM];//ÿ��Ǯ����Ҫ��Ǯ������
	UINT uiOutNumber[MAX_BOXNUM];    //ʵ��ÿ��Ǯ���Ǯ�����������أ������ǣ�ѭ��1.ѭ��2��ѭ��3��ѭ��4��������
	UINT uiRejectNumber[MAX_BOXNUM]; //����������ÿ��Ǯ��ϳ����յ����������أ�
} tBncChangeNum;
//����ƽṹ��
typedef struct
{
	UINT uiNGBoxAccInNum;       //�ۼƽ���ϳ�������
	UINT uiBoxAccOutNum;       //�ۼƳ�������
} tBncAuditCashNum;
//��RFID��Ϣ�ṹ��
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
}tBncRfidInfo;
#pragma pack(pop)

/*****************************************************************************/
/*                                                                           */
// ���½ӿ���LIBʵ�֣���DLL��ʹ�ã���������ṩdll��ɷ���TIMģ��ʵ�֣��������
// �ṩ����Э�飬�����װ��Э�����ʵ�����½ӿ�
/* # wang feng                                                               */
/*****************************************************************************/
//��ͨ�Ŵ��ڴ�
// ����˵�������ô˽ӿڿɴ򿪵Ͳ㴮��ͨ�š�
// ����˵������23��
// 1	unsigned int	uiCommPort	In	����ͨ�ź�
// 2	unsigned int	uiBaudRate	In	������
// ����ֵ˵��
// 1	Int	0	�ɹ�
// 2	Int	1	���ڴ򿪳���
DLLIMP int  BNC_open(unsigned int uiCommPort, unsigned int uiBaudRate);

//����ʼ��
// ����˵���� ��ʼ��ģ�飬���ģ���Ӳ������ģ��ָ�����ʼ״̬�ȡ�
// ����˵�����±�
// 1	tBncInitNumInfo *	pNumInfo	Out	�˳��ͻ���Ǯ����Ϣ
// 2	tBNDDevReturn	pDevStatus	Out	            �豸����״̬
// ����ֵ˵�����±�
// 1	Int	0	�ɹ�
// 2	Int	1	�������Ϊ��
DLLIMP int BNC_Init(tBncInitNumInfo *pNumInfo, tBncDevReturn * pDevStatus);

//��ͨ�Ŵ��ڹر�
//  ����˵�������ô˽ӿڿɹرյͲ㴮��ͨ�š�
// 	����ֵ˵��
// 	1	Int	0	�ɹ�
// 	2	Int	1	�رմ���
DLLIMP int BNC_close();	

//����ȡģ��̼��汾
DLLIMP int BNC_GetVersion(char* pVersion, tBncDevReturn * p_psStatus);
// ����˵�������ڻ�ȡ����ģ���ڵ�firmware�汾�ţ����ģ���У���Ŀǰֻ���GRG��ֽ��ģ����Ч��
// ����˵��
// 1	char*	pVersion	Out	�̼��汾������У�
// 2	tBncDevReturn &	p_psStatus	Out	����״̬��Ϣ
// ����ֵ˵������27��
// 1	Int	0	�ɹ�
// 2	Int	1	�رմ���


//������ʶ����ó�Ʊ
//  ����˵�����������������ÿɽ��յĳ�Ʊ�����ڽ�ֹ���յ��泮���˳���
// 	����˵������3��
// 	1	tBncSetInhibitList *	p_psBuyTicketSetInhibitList	In	������Ʊʱ��Ʊ������Ϣ�ṹ������
// 	2	tBncSetInhibitList *	p_psRechargeSetInhibitList	In	���ó�ֵʱ��Ʊ������Ϣ�ṹ������
// 	3	tBncDevReturn *	p_psStatus	Out	����BNA���ص��߼�������
// 	����ֵ˵��
// 	1	Int	0	�ɹ�
// 	2	Int	1	�������Ϊ��
DLLIMP int  BNC_SetPrevalue(tBncSetInhibitList *p_psBuyTicketSetInhibitList, tBncSetInhibitList *p_psRechargeSetInhibitList, tBncDevReturn * p_psStatus);

//����λ
// ����˵������������������ͨ���еĳ�Ʊ����о�ָ�����������
// ����˵��
// 1	Int	iMode	In	���ͨ�����г�Ʊ������ѡ��Ĳ����� p_iResetMode �趨��������Ϊ��
 //0��������ϵͳ���ݴ����ĳ�Ʊѹ��Ǯ��,���ʧ������ͣ�������
 //1��������ϵͳ���ݴ����ĳ�Ʊ�˳�,���ʧ������ͣ�������
// Ĭ��Ϊ 0 ��ʽ
// 2	Int	iRetryTimes	In	��λ���Դ���
// 3	tBncInitNumInfo *	pNumInfo	Out	�˳��ͻ���Ǯ����Ϣ
// 4	tBncDevReturn *	p_psStatus	Out	����BNC���ص��߼�������
// ����ֵ˵��
// 	1	Int	0	�ɹ�
// 	2	Int	1	�������Ϊ��
DLLIMP int BNC_Reset(int iMode, int iRetryTimes, tBncInitNumInfo *pNumInfo,tBncDevReturn * p_psStatus);

			
//����ȡģ��״̬
//  ����˵�������ô˽ӿڿɻ�ȡģ�鵱ǰ״̬����ϸ��Ϣ�鿴���ؽṹ�塣
// 	����˵��
// 	tBncDevReturn *	p_psStatus	Out	����BNC���ص��߼�������
// 	����ֵ˵��
// 	1	Int	0	�ɹ�
// 	2	Int	1	�������Ϊ��
DLLIMP int BNC_GetStatus(tBncDevReturn * p_psStatus);

// ��ȡģ����ϸ״̬
DLLIMP int BNC_GetSysDevStatus(tBncSysDevStatus* p_psSysStatus);

typedef struct _TAcceptorState
{
	BYTE  CurrentOCCode;
	char  CurrentCode[3];
	char  SequenceNr;
	BYTE  NrOfDocumentInintermediateStorage;
	char  ResolutionMode;
	BYTE  ValidationMode;
	BYTE  WorkMode;
}TAcceptorState;
// �ṹ�巵�ص�״̬��Ϣ;�����Ӳ��������Ϣ�ַ��������ݸ�ʽ����:
// [CurrentCode],[CurrentOCCode],[NrOfDocumentInintermediateStorage],[ResolutionMode],[SequenceNr] ,[ ValidationMode] ,[WorkMode] 

//����ȡ�����Ϣ
// 	����˵������ȡ�豸�������Ϣ��
// 	����˵�����±�
// 	1	tBncAuditCashNum *	pBncAuditCashNum	Out	���������Ϣ
// 	2	tBncDevReturn *	pDevStatus	Out	����״̬��Ϣ
// 	����ֵ˵�����±�
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BNC_GetAuditData(tBncAuditCashNum *pBncAuditCashNum, tBncDevReturn *pDevStatus);

//����ȡ����������Ϣ
// 	�������壺
// 	����˵������ȡǮ��ṹ����Ϣ��
// 	����˵�����±�
// 	1	tBncBillBoxInfo *	pBillBoxInfo	Out	����Ǯ��ṹ����Ϣ
// 	2	tBncDevReturn *	pDevStatus	Out	����״̬��Ϣ
// 	����ֵ����80��
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BNC_GetBillBoxInfo(tBncBillBoxInfo *pBillBoxInfo, tBncDevReturn *pDevStatus);

//����������������Ϣ
// 	�������壺
// 	����˵������������������Ϣ������ֽ��������
// 	����˵�����±�
// 	1	UNIT	uiSlotID	In	��λID
// 	2	tBncBillBoxInfo *	pBillBoxInfo	In	����������Ϣ
// 	3	tBncDevReturn *	pDevStatus	Out	����״̬��Ϣ
// 	����ֵ���±�
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BNC_SetBillBoxInfo(UINT uiSlotID, tBncBillBoxInfo *pBillBoxInfo, tBncDevReturn *pDevStatus);


//����ʼ���ճ�Ʊ
// 	����˵������о����Ԥ�����״̬��ָʾ�ƿ�ʼ��˸��ʾ�û�Ͷ��ֽ�ҡ�ʵ���鳮���˳�����Ʊѹ���ݴ����ȹ��ܡ�
// 	����˵�����±�
// 	1	tBncDevReturn *	p_psStatus	Out	����BNC���ص��߼�������
// 	����ֵ˵�����±�
// 	1	Int	0	�ɹ�
// 	2	Int	1	�������Ϊ��
// 	��ע���������̻߳����߳���������ʹֽ��ģ�����ȴ�����ֽ�ҵĹ���״̬�����򽫽������������ǳɹ�����ֽ�һ��������߳��е���BNC_Stop����ʹ�ú����˳����С�
DLLIMP int BNC_Start(tBncDevReturn * p_psStatus);

//��ֹͣ���ճ�Ʊ
// 	����˵������оֹͣ���ճ�Ʊ��ָʾ��Ϩ��
// 	����˵�����±�
// 	1	tBncDevReturn *	p_psStatus	Out	����BNA���ص��߼�������
//      tBncCashInfo *p_psCashInfo  Out �����ѽ��ճ�Ʊ����
// 	����ֵ˵�����±�
// 	1	Int	0	�ɹ�
// 	2	Int	1	�������Ϊ��
DLLIMP int BNC_Stop(tBncEnCashInfo *p_psCashInfo,tBncDevReturn * p_psStatus);

//����ȡʶ��Ʊ��Ϣ
// 	����˵������ȡ��о���յĳ�Ʊ��Ϣ��
// 	����˵�����±�
// 	1	tBncCashInfo *	p_psCashInfo	Out	���س�Ʊ��Ϣ�ṹ�壬��ʱǮ�����Ϣ
// 	2	BYTE&	p_byOperation	Out	���һ�Ŵ���ֽ�ҵ�״̬ 
// 		0:û�г�Ʊ����; 
// 		1:�޷�ʶ��ĳ�Ʊ�˳����˳���; 
// 		2:�û����˳��ڰѳ�Ʊ������ʶ�����ȡ�������˳��ģ�ȡ��; 
// 		3:��Ʊ�����ֱ�ǿ��ק��; 
// 		4:��Ʊ������; 
// 		5:�泮�������; 
// 		6:��Ʊ�������ҳ�ʱ����ֹͣ����; 
// 		7:�˳����г�Ʊδȡ�߳�ʱ; 
// 		8:�볮�����ݴ����г�Ʊ���ڴ���; 
// 		9:Ǯ���Ѿ�ȡ��; 
// 		10:Ǯ������; 
// 		11:��о�����ڳ�ʱ������(��������); 
// 		12:δ֪��������ͣ����
// 	3	tBncDevReturn *	p_psStatus	Out	����BNC���ص��߼�������
// 
// 	����ֵ˵�����±�
// 	1	Int	0	�ɹ�
// 	2	Int	1	�������Ϊ��
// 
// 	��ע������BNC_Start��ִ�гɹ��󣬵��øú�������ȡ��ʱǮ�������һ��ֽ�ҵĴ�����Ϣ��
// 	e)	����G&D��ֽ��ģ�飬p_byOperation���Ի�ȡ0��1��3��5��
// 	f)	����MEI��ֽ��ģ�飬p_byOperation���Ի�ȡ0��1��3��5��
// 	g)	����G&D��ֽ��ģ�飬p_byOperation���Ի�ȡ0��1��5��6��7��9��10��
// 	h)	����Ǯ��������Ǯ����ȡ�ߣ���о���ϣ�δ֪������������󣬿���ͨ��BNA_ GetStatu��Ӳ��������Ϣ���жϻ�ȡ��
DLLIMP int  BNC_GetCashInfo(tBncCashInfo *p_psCashInfo, BYTE& p_byOperation, tBncDevReturn *p_psStatus);

//��ѹ��
// 	����˵�������ݴ�����ֽ��ȫ��ѹ��Ǯ�䡣
// 	����˵�����±�
// 	1	tBncDevReturn *	p_psStatus	Out	����BNC���ص��߼�������
// 	����ֵ˵�����±�
// 	1	Int	0	�ɹ�
// 	2	Int	1	�������Ϊ��
DLLIMP int  BNC_Encash(tBncDevReturn *p_psStatus);

//���˳�
// 	����˵�������ݴ�����ֽ��ȫ���˳���
// 	����˵�����±�
// 	1	tBncDevReturn *	p_psStatus	Out	����BNA���ص��߼�������
// 	����ֵ˵�����±�
// 	1	Int	0	�ɹ�
// 	2	Int	1	�������Ϊ��
DLLIMP int  BNC_Refund(tBncDevReturn *p_psStatus);


//����ȡǮ��ID
// 	����˵�������ô˽ӿڿɻ�ȡǮ��ID����ϸ��Ϣ�鿴���ؽṹ�塣
// 	����˵�����±�
// 	1	char*	p_BoxID	Out	��ȡǮ��ID
// 	2	tBncDevReturn *	p_psStatus	Out	����BNC���ص��߼�������
// 
// 	����ֵ˵�����±�
// 	1	Int	0	�ɹ�
// 	2	Int	1	�������Ϊ��
DLLIMP int BNC_GetBoxID(char * p_BoxID, tBncDevReturn * p_psStatus);

//�������䳮
// 	����˵���������䳮
// 	����˵�����±�
// 	1	tBncChangeNum *	pChangeNum	In/Out	�����ֽ����������Ϣ
// 	2	tBncDevReturn *	pDevStatus	Out	����״̬��Ϣ
// 	����ֵ˵�����±�
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BNC_ChangeBill(tBncChangeNum *pChangeNum, tBncDevReturn *pDevStatus);

//��ȡ������
// 	����˵����ȡ��������
// 	����˵�����±�
// 	1	tBncDevReturn *	p_psStatus	Out	����BNC���ص��߼�������
// 	����ֵ˵�����±�
// 	1	Int	0	�ɹ�
// 	2	Int	1	�������Ϊ��
DLLIMP int  BNC_Cancel (tBncDevReturn *p_psStatus);

//����ʼ����ֽ��������
// 	����˵�����˽ӿ����ڸ���ֽ�������䡣
// 	����˵�����±�
// 	1	UNIT	uiBoxType	In	ָ��ֽ�������������������ͣ�0-�ϱ��� 1-1Ԫֽ����, 2-5Ԫֽ����, 3-10
// 	Ԫֽ����, 4-20Ԫֽ����, 5-50Ԫֽ����, 6-100Ԫֽ����
// 	2	UINT	uiBoxNo	In	�ƶ�ֽ�����������0 ~ 6 ���У� 0�ŷϱ��� 1��Ϊ5Ԫ�����䣬2��Ϊ1Ԫ������
// 	3	tBncDevReturn *	pDevStatus	Out	����״̬��Ϣ
// 	����ֵ���±�
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BNC_StartReplaceBox(UINT uiBoxType, UINT uiBoxNo, tBncDevReturn *pDevStatus);

//  ֹͣ����ֽ��������
// 	����˵����ֹͣ����ֽ�������䡣
// 	����˵�����±�
// 	1	UNIT	uiBoxType	In	ָ��ֽ����������
// 	2	UNIT	uiBoxNo	In	ָ��ֽ���������
// 	2	tBncDevReturn *	p_psStatus	Out	����BNC���ص��߼�������
// 
// 	����ֵ˵�����±�
// 	1	Int	0	�ɹ�
// 	2	Int	1	�������Ϊ��
DLLIMP int BNC_StopReplaceBox(UINT uiBoxType, UINT uiBoxNo, tBncDevReturn *pDevStatus);

//������Ǯ��RFID��Ϣ
// 	����˵����д��Ǯ��RFID��Ϣ��
// 	����˵�����±�
// 	1	UNIT	uiSLotID	In	��λID
// 	2	tBncRfidInfo *	pRFIDInfo	In	����RFID��Ϣ
// 	3	tBncDevReturn *	pDevStatus	Out	����״̬��Ϣ
// 	����ֵ���±�
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BNC_WriteRFIDInfo(UINT uiSLotID, tBncRfidInfo * pRFIDInfo, tBncDevReturn * pDevStatus);

//����ȡǮ��RFID��Ϣ
// 	����˵������ȡǮ��RFID��Ϣ��
// 	����˵�����±�
// 	1	UNIT	uiSLotID	In	��λID
// 	2	tBncRfidInfo *	pRFIDInfo	Out	����RFID��Ϣ
// 	3	tBncDevReturn *	pDevStatus	Out	����״̬��Ϣ
// 	����ֵ˵�����±�
// 	1	int	0��ִ�гɹ���1��ִ��ʧ�ܡ�
DLLIMP int BNC_ReadRFIDInfo(UINT uiSLotID, tBncRfidInfo* pRFIDInfo, tBncDevReturn * pDevStatus);

//   ���Ǯ��
//	OUT tBncChangeNum* pBoxInfo
DLLIMP int BNC_Empty(tBncChangeNum* pBoxInfo);
#endif

