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

	double dOutputLoadR;//输出电流，是一个百分比，不是一个绝对值。
	double dOutputLoadS;
	double dOutputLoadT;

	double dPositiveVoltage;
	double dNegativeVoltage;

	double dTemperature;
	DWORD  dwLeftStandbyTime;
	DWORD dBatteryCapacity;//电池容量百分比，0%-100%，超过100%显示为最大。

	BYTE   bySysModeK;  //取值0-9

	BYTE   byBatteryTestStatus;//取值0-7

	BYTE   byErrorCode[8]; //每两个字节表示一个故障编码。
	BYTE   byWarningCode[8];

	bool   bInputTransformerY;//变压器类型。1表示是Y型电压。
	bool   bOutputVType; //1表示是相电压，0表示是线电压。
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