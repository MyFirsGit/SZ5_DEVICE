#ifndef  __UPS_COM_TOOL__H__
#define __UPS_COM_TOOL__H__

struct _tagUPSBatteryInfo
{
	double dInputVoltageR;
	double dInputVoltageS;
	double dInputVoltageT;
	double dInputFrequency;

	double dOutputVoltageR;
	double dOutputVoltageS;
	double dOutputVoltageT;
	double dOutputFrequency;

	double dOutputLoadR;//�����������һ���ٷֱȣ�����һ������ֵ��
	double dOutputLoadS;
	double dOutputLoadT;

	double dPositiveVoltage;
	double dNegativeVoltage;

	double dTemperature;
	DWORD  dwLeftStandbyTime;
	DWORD dBatteryCapacity;//��������ٷֱȣ�0%-100%������100%��ʾΪ���

	BYTE   bySysModeK;  //ȡֵ0-9

	BYTE   byBatteryTestStatus;//ȡֵ0-7

	BYTE   byErrorCode[8]; //ÿ�����ֽڱ�ʾһ�����ϱ��롣
	BYTE   byWarningCode[8];

	bool   bInputTransformerY;//��ѹ�����͡�1��ʾ��Y�͵�ѹ��
	bool   bOutputVType; //1��ʾ�����ѹ��0��ʾ���ߵ�ѹ��
};

typedef enum
{
	UPS_SUCCESSED = 0,
	UPS_ERR_WRITE_COM,
	UPS_ERR_READ_COM,
	UPS_ERR_WAITING_DATA_OUTTIME,
	UPS_ERR_HEADER_FLAG, 
	UPS_ERR_END_FLAG,
	UPS_ERR_OTHER,
}UPS_RESULT_CODE;

#endif