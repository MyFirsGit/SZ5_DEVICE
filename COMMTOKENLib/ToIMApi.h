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
	一、传感器状态
	bit    传感器名称				说明
	 0	票卡到天线区检测		1：故障  0：正常 
	 1	票卡到出票口检测		1：故障  0：正常 
	 2	票卡到废票口检测		1：故障  0：正常 
	 3	出票口电磁铁检测		1：故障  0：正常 
	 4	废票口电磁铁检测		1：故障  0：正常 
	 5	保留					1：故障  0：正常 
	 6	票箱B到位检测			1：故障  0：正常 
	 7	票箱A到位检测			1：故障  0：正常 

	二、HopperA状态
	bit    传感器名称				说明
	 0	HopperA 通道传感器1		1：故障  0：正常 
	 1	HopperA 通道传感器2		1：故障  0：正常 
	 2	HopperA 电磁铁传感器	1：故障  0：正常 
	 3	HopperA 空传感器		1：故障  0：正常 
	 4	HopperA 将空传感器		1：故障  0：正常 
	 5	A暂存区电磁铁传感器		1：故障  0：正常 
	 6	出币电机打开侧传感器	1：故障  0：正常 
	 7	出币电机关闭侧传感器	1：故障  0：正常 

	三、HopperB状态
	bit    传感器名称				说明
	 0	HopperB 通道传感器1		1：故障  0：正常 
	 1	HopperB 通道传感器2		1：故障  0：正常 
	 2	HopperB 电磁铁传感器	1：故障  0：正常 
	 3	HopperB 空传感器		1：故障  0：正常 
	 4	HopperB 将空传感器		1：故障  0：正常 
	 5	B暂存区电磁铁传感器		1：故障  0：正常 
	 6	A暂存区票检测传感器		1：故障  0：正常 
	 7	B暂存区票检测传感器		1：故障  0：正常 
	*/

	/*
	一、模块状态
	bit    传感器名称										说明
	 0	票箱A将空检测传感器（对射）			1：将空（传感器未挡） 0：非空（传感器被挡）
	 1	票箱A存在传感器（）					1：存在 0：不存在 
	 2	票箱B将空检测传感器（对射）			1：将空（传感器未挡） 0：非空（传感器被挡） 
	 3	票箱B存在传感器（）					1：存在 0：不存在
	 4	票箱A空检测传感器（对射）			1：空（传感器未挡） 0：非空（传感器被挡）
	 5	票箱B空检测传感器（对射）			1：空（传感器未挡） 0：非空（传感器被挡）
	 6	天线区是否有票						1：有  0：无 
	 7	保留								保留  

	二、模块状态
	bit    传感器名称										说明
	 0	保留								保留  
	 1	HopperA 是否故障					1：故障  0：正常 
	 2	HopperB 是否故障					1：故障  0：正常 
	 3	A暂存区是否有票（有暂存区模块专用） 1：有票  0：无票
	 4	B暂存区是否有票（有暂存区模块专用） 1：有票  0：无票 
	 5	保留								保留  
	 6	保留								保留  
	 7	保留								保留   
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
