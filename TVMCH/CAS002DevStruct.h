#ifndef _CASDEVSTRUCT_H_
#define _CASDEVSTRUCT_H_

// ���ý��ṹ
typedef struct _tSetCoinNum
{
	int uiMHopper1Num; // ��������2Ӳ�Ҹ�����С��1100��	//�人11����ר��
	int uiMHopper2Num; // ��������1Ӳ�Ҹ�����С��1100��	//�人11����ר��

	UINT uiCoinBoxCoinANum; // Ӳ��Ǯ��Ӳ��A����������Ӳ��BС��3000��
	UINT uiCoinBoxCoinBNum; // Ӳ��Ǯ��Ӳ��B����������Ӳ��AС��3000��

	BYTE byEscrow1Num; // �ݴ���1Ӳ������С��70��
	BYTE byEscrow2Num; // �ݴ���2Ӳ������С��70��

	BYTE byMHopper1Type; // ��������2Ӳ�����ͣ�1 = 1ԪӲ�ң�2 = 5��Ӳ�ҡ�	//�人11����ר��
	BYTE byMHopper2Type; // ��������1Ӳ�����ͣ�1 = 1ԪӲ�ң�2 = 5��Ӳ�ҡ�	//�人11����ר��
		
	BYTE byEjectCoinNum; // �˱Ҹ���

}tSetCoinNum;

// ͼ��ʶ��ģ��״̬�ṹ add by lcl 2008.11.26
typedef struct _tImageValidator
{
	int	iRunStatus;				// ����״̬       0: ��ͣ���� 1: ��������
	int	iImageCatchAreaStatus;	// ͼ��׽��״̬ 0: ����״̬ 1: ����״̬
	int	iLightingStatus;		// �ƹ�״̬       0: ����״̬ 1: ����״̬
	int	iBackDropStatus;		// ����״̬       0: ����״̬ 1: ����״̬
	int	iImageCollectionStatus;	// ͼ��ɼ�״̬   0: ����״̬ 1: ����״̬
	int	iIdentityStatus;		// ������״̬     0: ����״̬ 1: ����״̬
	int	iSeriesFalseCoinStatus;	// �����ٱ�״̬   0: ����״̬ 1: ����״̬
	int	iReserve1;				// Ԥ��
	int	iReserve2;				// Ԥ��
}tImageValidator;

// ״̬�ṹ
typedef struct _tModuleStatus
{
	bool biButton1;	// ��������2�ӱ���iButton  false����ⲻ���ӱ���iButton, true����⵽�ӱ���iButton	//�人11����ר��
	bool biButton2; // ��������1�ӱ���iButton, false����ⲻ���ӱ���iButton, true����⵽�ӱ���iButton	//�人11����ר��
	bool biButton3; // Ǯ��, false����ⲻ���ӱ���, true����⵽�ӱ���
	//added by ZacharLiu 2015-11-30
	bool bCoinGate; // Ӳ��բ��״̬, false:բ������, true:բ���й���
	//end added

	bool bMHopper1; // ��������1��λSensor  false����ⲻ��HOPPER, true����⵽HOPPER
	bool bMHopper2; // ��������2��λSensor  false����ⲻ��HOPPER, true����⵽HOPPER

	bool bMHopper1Sensor;	// ��������2���Ҵ�����,true���б�,false��û��		//�人11����ר��
	bool bMHopper2Sensor;	// ��������1���Ҵ�����,true���б�,false��û��		//�人11����ר��
		
	bool bRouter1RightSensor; // ������1�Ҵ�����,true����Ƭ�����ұ�, false����Ƭ���ұߣ����ڻ���Ӳ��״̬��
	bool bRouter1LeftSensor;	// ������1�󴫸���,true����Ƭ�������, false����Ƭ����ߣ����ڳ���״̬��
	bool bRouter2RightSensor; // ������2�Ҵ�����,true����Ƭ�����ұ�, false����Ƭ���ұߣ����ڻ���Ӳ��״̬��
	bool bRouter2LeftSensor;	// ������2�󴫸���,true����Ƭ�������, false����Ƭ����ߣ����ڳ���״̬��
	
	bool bCoinInSensor; // Ͷ����ڴ�����,true������ޱ�,false������б�
	bool bCoinOut1Sensor;	// Ͷ�ҳ��ڴ�����1,true������1�ޱ�,false������1�б�
	bool bCoinOut2Sensor;	// Ͷ�ҳ��ڴ�����2,true������2�ޱ�,false������2�б�
	bool bDoorSensor;	// �Ŵ�����,true�����ѹ�,false����û��
	bool bCamSensor;	// ͹�ִ�����,true��͹��û��λ, false��͹�ּ���λ
	bool bPositionSensor; // �ϼ�λ�ô�����,true--������û�б���(ģ�鲻��λ) false--����������(ģ�鵽λ)

	bool bEscrow1RightSensor; // �ݴ���1�Ҵ�����, true����Ҷ������ұ�, false����Ҷ������ұ�
	bool bEscrow1MidSensor; // �ݴ���1�м䴫����, true����Ҷ������м�, false����Ҷ������м�
	bool bEscrow1LeftSensor;	// �ݴ���1�󴫸���, true����Ҷ��������, false����Ҷ��������
	bool bEscrow2RightSensor; // �ݴ���2�Ҵ�����, true����Ҷ������ұ�, false����Ҷ������ұ�
	bool bEscrow2MidSensor; // �ݴ���2�м䴫����, true����Ҷ������м�, false����Ҷ������м�
	bool bEscrow2LeftSensor;	// �ݴ���2�󴫸���, true����Ҷ��������, false����Ҷ��������

	bool bMHopper1LowSensor;//��������1��λ�źţ�true�����գ�false����
	bool bMHopper1HighSensor;//��������1��λ�źţ�true�����ң�false������
	bool bMHopper1SafeSensor;//��������1��ȫ�źţ�true�����ң�false��û��
	bool bMHopper1RawSensor; // ��������1ԭʼ�����źţ�true���бң�false��û��
	
	bool bMHopper2LowSensor;//��������2��λ�źţ�true�����գ�false����
	bool bMHopper2HighSensor;//��������2��λ�źţ�true�����ң�false������
	bool bMHopper2SafeSensor;//��������2��ȫ�źţ�true�����ң�false��û��
	bool bMHopper2RawSensor; // ��������2ԭʼ�����źţ�true���бң�false��û��
	
	// add by Money.Chan 2007-10-29
	bool bImageControl1; // ͼ��ʶ�����1 false: ͼ��Ϊ�� true: ͼ��Ϊ��
	bool bImageControl2; // ͼ��ʶ�����2 false: ͼ��Ϊ�� true: ͼ��Ϊ��
	
	bool bCoinBoxSensor; // Ӳ��Ǯ��λ��  false: ����λ true: ��λ
	
	bool bPrevSensor1; // Ԥ��������1 false: ���������� true: ������û��
	bool bPrevSensor2; // Ԥ��������2 false: ���������� true: ������û��
	// end add by Money.Chan 2007-10-29

	tSetCoinNum tCoinNum;
	tImageValidator tImageValidatorStatus; // ͼ��ʶ��ģ��״̬ Add by lcliang 2008.11.26

	char acReserve[256]; // Ԥ��
}tModuleStatus;

// ��ҽṹ 
typedef struct _tClearCoin
{
	BOOL bClearMHopper1; // ��������2��ұ�־		//�人11����ר��
	BOOL bClearMHopper2; // ��������1��ұ�־		//�人11����ר��

}tClearCoin;

// iButton���ݽṹ
typedef struct _tiButtonData
{
	BYTE byBoxID[8]; // Ǯ��ID
	BYTE byOperatorID[8]; // ������ID

	WORD wCoinNum; // Ǯ��Ӳ����
	BYTE byCoinType; // Ӳ������

	BYTE byAttachTime[6]; // �����豸ʱ��
	BYTE byDetachTime[6]; // ȡ���豸ʱ��

	char acReserve[128]; //Ԥ��
}tiButtonData;

// ģ��汾��
typedef struct _tModuleVersion
{
	char acDevType[7]; // ģ���ͺ�
	char acFirmWareVersion[7]; // �̼��汾��	
	char acSerialNumber[32]; // SN��
	char acImageVersion[17]; // ͼ��ʶ��ģ��汾add by lcl 2008.11.26

}tModuleVersion;

// �������
typedef struct _tAuditData
{
	UINT uiTotalRevCoinA;		// �ܽ���A��Ӳ����
	UINT uiTotalRevCoinB;		// �ܽ���B��Ӳ����
	UINT uiTotalChangeCoinA;	// ������A��Ӳ����
	UINT uiTotalChangeCoinB;	// ������B��Ӳ����
	DOUBLE ulImageRealCoin;		// ͼ���������
	DOUBLE ulImageFakeCoin;		// ͼ����ٱ���
}tAuditData;

// ����ṹ
typedef struct _tChange
{
	BYTE byMHopper1Num; // ��������2������	//�人11����ר��
	BYTE byMHopper2Num; // ��������1������	//�人11����ר��
	
}tChange;

// ��Ʊ�ṹ
typedef struct _tCoinInfo
{ 
	char acCurrency[4]; // ����
	int iCount;			// ����
	long lDenomination; // ���
	char cSerial;		// ��Ʊ����(���Ļ������'4','5')
}tCoinInfo;

typedef struct _tCASConfig
{
	WORD wCoinTypeA;
	WORD wCoinTypeB;

	INT iHopper1Max;	// lcl 2008.11.06
	INT iHopper2Max;	// lcl 2008.11.06
	INT iHopperMin;		// lcl 2008.11.06
	BYTE byImageUsed;	// lcl 2008.11.26

	BYTE byEscrowUsed;	// lfan	2015.12.30
	BYTE byGateUsed;	// lfan 2015.12.30

	BYTE byCycleChange;
	UINT uiChangeDelay;
	UINT uiReclMaxCount; //add by lliang 16.8.19
}tCASConfig;

// ��¼Ӳ�ҵ�����ṹ
typedef struct _tCoinFlow
{
	WORD wEscrow1ToCoinOut;  // ���ݴ���1��Ӳ�ҳ���
	WORD wEscrow1ToHopper1;  // ���ݴ���1��Hopper1
	WORD wEscrow1ToCoinBox;  // ���ݴ���1��Ӳ��Ǯ��

	WORD wEscrow2ToCoinOut;  // ���ݴ���2��Ӳ�ҳ���
	WORD wEscrow2ToCoinBox;  // ���ݴ���2��Ӳ��Ǯ��

	WORD wHopper1ToCoinOut;  // ��Hopper1��Ӳ�ҳ���
	WORD wHopper1ToCoinBox;  // ��Hopper1��Ӳ��Ǯ��

	WORD wHopper2ToCoinOut;  // ��Hopper2��Ӳ�ҳ���
	WORD wHopper2ToCoinBox;  // ��Hopper2��Ӳ��Ǯ��

}tCoinFlow;

// ��¼Log��ṹ��
typedef struct _tLogTableInfo 
{
	BYTE acYearDateTable[12];	// Log�������б�
	BYTE acMonthDateTable[12];	// Log�������б�
	BYTE acDayDateTable[12];	// Log�������б�
	BYTE acAddrTable[24];		// Log��ַ�б�
	BYTE acSectorTable[12];		// Log�����б�
	BYTE byCurrentSectorNo;		// ��ǰLog�������
	UINT wCurrentSectorAddr;	// ��ǰLog������ַ
	BYTE byCurrentDateNo;		// ��ǰ���ڱ��
	BYTE acLogCRC[2];			// CRCУ��

	_tLogTableInfo()
	{
		memset(acYearDateTable, 0x00, sizeof(acYearDateTable));
		memset(acMonthDateTable, 0x00, sizeof(acMonthDateTable));
		memset(acDayDateTable, 0x00, sizeof(acDayDateTable));
		memset(acAddrTable, 0x00, sizeof(acAddrTable));
		memset(acSectorTable, 0x00, sizeof(acSectorTable));
		memset(acLogCRC, 0x00, sizeof(acLogCRC));
		wCurrentSectorAddr = 0;
		byCurrentSectorNo = 0;
		byCurrentDateNo = 0;
	}
	
}tLogTableInfo;

// Log��Ϣ�洢�ṹ
typedef struct _tLogInfoStruct
{
	UINT wLogLength;     // Log����
	UINT wLogSectorAddr; // Log��ʼ��ַ
	BYTE bySectorNo;     // Log�������

	_tLogInfoStruct()
	{
		wLogLength = 0;
		wLogSectorAddr = 0;
		bySectorNo = 0;
	}

}tLogInfoStruct;


// ����Ӳ������
typedef struct _tRejectData
{
	byte byType;		// ���� 0---���� 1---����
	int iDenomination;	// ������� 
						// ��RMB,bit0---5�� bit1---1Ԫ������λ����
						// ÿһλΪ0��ʾ���գ�1��ʾ����
}tRejectData;

// ����Ӳ������
typedef struct _tRejectInfo
{
	tRejectData sRejectData[32]; //0ά --- RMB ����ά����
}tRejectInfo;

//��д���ݽṹ��
typedef struct _tData
{
	char acData[64]; //������д�������
}tCAS002_Data;

//RFID ��Ϣ�ṹ��
typedef struct _tRFIDInfo
{
	unsigned char ucPortId;		//�˿ں�:0x01 = �ӱ���2  0x02 = �ӱ���1  0x03 = ������1  0x04 = ������2	//�人11����ר��
	unsigned char ucIndex;		//Block/Sector ��
}tCAS002_RFIDInfo;

//added by ZachartLiu 2015-12-03
typedef struct _tCfgPara
{
	unsigned char ucTypeInfo[7];
	unsigned char ucParaValue[4];

}tCAS002C_CfgPara;
//end added

#endif	// #ifndef _CASDEVSTRUCT_H_
