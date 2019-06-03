#ifndef _TOIMAPI_H_
#define _TOIMAPI_H_
/*****************************************************************************/
/*                                                                           */
/*    Copyright (C) - LEGATE Intelligent Equipment - All rights reserved     */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*  Except if expressly provided in a dedicated License Agreement, you are   */
/*  not authorized to:                                                       */
/*                                                                           */
/*  1. Use, copy, modify or transfer this software component, module or      */
/*  product, including any accompanying electronic or paper documentation    */
/*  (together, the "Software").                                              */
/*                                                                           */
/*  2. Remove any product identification, copyright, proprietary notices or  */
/*  labels from the Software.                                                */
/*                                                                           */
/*  3. Modify, reverse engineer, decompile, disassemble or otherwise attempt */
/*  to reconstruct or discover the source code, or any parts of it, from the */
/*  binaries of the Software.                                                */
/*                                                                           */
/*  4. Create derivative works based on the Software (e.g. incorporating the */
/*  Software in another software or commercial product or service without a  */
/*  proper license).                                                         */
/*                                                                           */
/*  By installing or using the "Software", you confirm your acceptance of the*/
/*  hereabove terms and conditions.                                          */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/*  History:                                                                 */
/*****************************************************************************/
/*  Date       * Author          * Changes                                   */
/*****************************************************************************/
/*  2014-02-24 * yong.wang       * Creation of the file                      */
/*             *                 *                                           */
/*****************************************************************************/


/*****************************************************************************/
/*                                                                           */
/*  Include Files                                                            */
/*                                                                           */
/*****************************************************************************/

#if defined (__cplusplus)
extern "C"
{
#endif

/*****************************************************************************/
/*                                                                           */
/*  Definitions                                                              */
/*                                                                           */
/*****************************************************************************/
// type
#ifndef uchar
#define uchar unsigned char
#endif
#ifndef ushort
#define ushort unsigned short
#endif
#ifndef uint
#define uint unsigned int
#endif

namespace TOKEN_NAMESPACE
{
	// ToIM API return values
	#define RESULT_SUCCESS            's'
	#define RESULT_FAILURE            'e'
	#define RESULT_WARNING            'w'

	#define ERR_COM_CONFIG       0xFE    //Common configuration Error
	#define ERR_COM_ACK          0xFD    //ACK Error
	#define ERR_COM_CMD          0xFC    //Respond Cmd Error

	#define MODULE_HAD_TEMP      1       //Had Temporary storage area module
	#define MODULE_NONE_TEMP     2       //None Temporary storage area module



	/* 
	һ��������״̬
	bit    ����������				˵��
	 0	Ʊ�������������		1������  0������ 
	 1	Ʊ������Ʊ�ڼ��		1������  0������ 
	 2	Ʊ������Ʊ�ڼ��		1������  0������ 
	 3	��Ʊ�ڵ�������		1������  0������ 
	 4	��Ʊ�ڵ�������		1������  0������ 
	 5	����					1������  0������ 
	 6	Ʊ��B��λ���			1������  0������ 
	 7	Ʊ��A��λ���			1������  0������ 

	����HopperA״̬
	bit    ����������				˵��
	 0	HopperA ͨ��������1		1������  0������ 
	 1	HopperA ͨ��������2		1������  0������ 
	 2	HopperA �����������	1������  0������ 
	 3	HopperA �մ�����		1������  0������ 
	 4	HopperA ���մ�����		1������  0������ 
	 5	A�ݴ��������������		1������  0������ 
	 6	���ҵ���򿪲ഫ����	1������  0������ 
	 7	���ҵ���رղഫ����	1������  0������ 

	����HopperB״̬
	bit    ����������				˵��
	 0	HopperB ͨ��������1		1������  0������ 
	 1	HopperB ͨ��������2		1������  0������ 
	 2	HopperB �����������	1������  0������ 
	 3	HopperB �մ�����		1������  0������ 
	 4	HopperB ���մ�����		1������  0������ 
	 5	B�ݴ��������������		1������  0������ 
	 6	A�ݴ���Ʊ��⴫����		1������  0������ 
	 7	B�ݴ���Ʊ��⴫����		1������  0������ 
	*/

	/*
	һ��ģ��״̬
	bit    ����������										˵��
	 0	Ʊ��A���ռ�⴫���������䣩			1�����գ�������δ���� 0���ǿգ�������������
	 1	Ʊ��A���ڴ���������					1������ 0�������� 
	 2	Ʊ��B���ռ�⴫���������䣩			1�����գ�������δ���� 0���ǿգ������������� 
	 3	Ʊ��B���ڴ���������					1������ 0��������
	 4	Ʊ��A�ռ�⴫���������䣩			1���գ�������δ���� 0���ǿգ�������������
	 5	Ʊ��B�ռ�⴫���������䣩			1���գ�������δ���� 0���ǿգ�������������
	 6	�������Ƿ���Ʊ						1����  0���� 
	 7	����								����  

	����ģ��״̬
	bit    ����������										˵��
	 0	����								����  
	 1	HopperA �Ƿ����					1������  0������ 
	 2	HopperB �Ƿ����					1������  0������ 
	 3	A�ݴ����Ƿ���Ʊ�����ݴ���ģ��ר�ã� 1����Ʊ  0����Ʊ
	 4	B�ݴ����Ƿ���Ʊ�����ݴ���ģ��ר�ã� 1����Ʊ  0����Ʊ 
	 5	����								����  
	 6	����								����  
	 7	����								����   
	*/

	//sensor status
	typedef struct
	{
		uchar ucSensorStatus;
		uchar ucHopperAStatus;
		uchar ucHopperBStatus;
	}SensorStatus;

	// status
	typedef struct
	{
		uchar ucSensorStatus;
		uchar ucSubModuleStatus;
	}ModuleStatus;

	// module infomation
	typedef struct
	{
		uchar ucModNum[8];
		uchar ucFwVer[7];
	}ModuleInfo;

	typedef struct
	{
		uchar ucModNum[16];
		uchar ucFwVer[16];
	}HopperModuleInfo;

	// token box number
	typedef struct
	{
		ushort TokenBoxA;
		ushort TokenBoxB;
		ushort TokenBoxInvalid;
	}TokenBoxNum;

	// token box SN
	typedef struct
	{
		uchar sSN[14];
	}TokenBoxSN;

	// block data
	typedef struct
	{
		uchar BlockData[16];
	}BlockData;

	// sector data
	typedef struct
	{
		uchar SectorData[48];
	}SectorData;

	/*****************************************************************************/
	/*                                                                           */
	/*  Variable Declarations                                                    */
	/*                                                                           */
	/*****************************************************************************/

	/*****************************************************************************/
	/*                                                                           */
	/*  Function Declarations                                                    */
	/*                                                                           */
	/*****************************************************************************/
	extern int ToIM_CommOpen(const char *sCommPort, uint uiBaudRate);

	extern int ToIM_CommClose(void);

	extern int ToIM_Init(uchar ucActionMode,uchar & ucErrorCode, ModuleStatus & xModuleStatus, uchar &ucTokenNum);

	extern int ToIM_ReadStatus(uchar & ucErrorCode, ModuleStatus & xModuleStatus);

	//extern int ToIM_CleanPass(uchar & ucErrorCode, ModuleStatus & xModuleStatus, uchar & ucTokenNum);

	extern int ToIM_TokenToAntenna(uchar ucBoxNo, uchar & ucErrorCode, ModuleStatus & xModuleStatus, uchar & ucTokenNum);

	extern int ToIM_TokenToOutlet(uchar & ucErrorCode, ModuleStatus & xModuleStatus, uchar & ucTokenNum);

	extern int ToIM_TokenRetrieve(uchar & ucErrorCode, ModuleStatus & xModuleStatus, uchar & ucTokenNum);

	extern int ToIM_Reset(uchar &ucErrorCode);

	extern int ToIM_ReadVersion(uchar & ucErrorCode, ModuleInfo & xModuleInfo);

	extern int ToIM_CleanOut(uchar cBoxNo, uchar & ucErrorCode);

	extern int ToIM_StopCleanOut(uchar cBoxNo, uchar &ucErrorCode);

	extern int ToIM_GetNumOfCleanOut(uchar cBoxNo, uchar & ucErrorCode,ushort & usCoNum);

	//extern int ToIM_ReadTokenBoxSN(uchar cBoxNo, uchar & ucErrorCode, TokenBoxSN & xTokenBoxSN);

	extern int ToIM_Set_Modue_Type(uchar &ucErrorCode, uchar ucModuleType);

	extern int ToIM_Get_Modue_Type(uchar &ucErrorCode, uchar &ucModuleType);

	extern int ToIM_RfidWriteBlock(uchar ucRfidNo, uchar ucBlockNo,BlockData xBlockData, uchar &ucErrorCode);

	extern int ToIM_RfidReadBlock(uchar ucRfidNo, uchar ucBlockNo, uchar &ucErrorCode,BlockData &xBlockData);

	extern int ToIM_RfidWriteSector(uchar ucRfidNo, uchar ucSectorNo, uchar ucLen,SectorData xSectorData,uchar &ucErrorCode);

	extern int ToIM_RfidReadSector(uchar ucRfidNo, uchar ucSectorNo, uchar &ucErrorCode,SectorData &xSectorData);

	extern int ToIM_RfidReadID(uchar ucRfidNo, uchar &ucErrorCode, uchar* uiRfidID);

	extern int ToIM_SensorCheck(uchar &ucErrorCode, char *sensorData);

	extern int ToIM_GetHopperVersion(uchar &ucErrorCode, HopperModuleInfo &xModuleInfo);

	extern int ToIM_ControlTemporary(uchar mode, uchar &ucErrorCode);

	extern int ToIM_SetHopperQuantity(uchar HopperQuantity, uchar &ucErrorCode);

	extern int ToIM_GetHopperQuantity(uchar &HopperQuantity, uchar &ucErrorCode);

	extern int ToIM_SetTemporarySensor(uchar TemporarySensor, uchar &ucErrorCode);

	extern int ToIM_GetTemporarySensor(uchar &TemporarySensor, uchar &ucErrorCode);

	extern int ToIM_SetAntennaSensorPosition(uchar AntennaSensorPosition, uchar &ucErrorCode);

	extern int ToIM_GetAntennaSensorPosition(uchar &AntennaSensorPosition, uchar &ucErrorCode);

	extern int ToIM_CleanBoxToken(uchar cBoxNo, uchar & ucErrorCode,ushort & usCoNumA,ushort & usCoNumB);
}

#if defined (__cplusplus)
} // end of extern "C"
#endif

#endif // _TOIMAPI_H_
