#pragma once
#include "afx.h"
#include "SysException.h"
#include "BHexp.h"
/**
@brief	纸币找零硬件异常类
*/
class BH_API CBHChangeHardwareException : public CSysException
{

public:
	static const int MODULE_ID = 13201;     // 模块号

	typedef enum _emerrcode
	{
		ERR_INTERNAL1	= 0x03,		//Internal error
		ERR_INTERNAL2	= 0x04,		//Internal error

		ERR_No1stCST	= 0x10,		//No 1st Cassette
		ERR_Empty1stCST = 0x11,		//1st cassette empty
		ERR_BD1stCST	= 0x12,		//1st cassette: Error related to bill diagnosis
		
		ERR_No5thCST	= 0x14,		//No 5th Cassette
		ERR_Empty5thCST = 0x15,		//5th cassette empty
		ERR_BD5thCST	= 0x16,		//5th cassette: Error related to bill diagnosis
		
		ERR_Pick1stCST = 0x18,		//1st Cassette pick error
		ERR_Pick5thCST = 0x1C,		//5th Cassette pick error

		ERR_No2ndCST  = 0x20,		//No 2nd Cassette
		ERR_Empty2ndCST = 0x21,		//2nd cassette empty
		ERR_BD2ndCST	= 0x22,		//2nd cassette: Error related to bill diagnosis

		ERR_No6thCST  = 0x24,		//No 6th Cassette
		ERR_Empty6thCST = 0x25,		//6th cassette empty
		ERR_BD6thCST	= 0x26,		//6th cassette: Error related to bill diagnosis

		ERR_Pick2ndCST = 0x28,		 //2nd Cassette pick error
		ERR_Pick6thCST = 0x2C,		 //6th Cassette pick error

		ERR_No3rdCST  = 0x30,		//No 3rd Cassette
		ERR_Empty3rdCST = 0x31,		//3rd cassette empty
		ERR_BD3rdCST	= 0x32,		 //3rd cassette: Error related to bill diagnosis

		ERR_No7thCST  = 0x34,   //No 7th Cassette
		ERR_Empty7thCST = 0x35, //7th cassette empty
		ERR_BD7thCST	= 0x36,  //7th cassette: Error related to bill diagnosis

		ERR_Pick3rdCST = 0x38,   //3rd Cassette pick error
		ERR_Pick7thCST = 0x3C,   //7th Cassette pick error

		ERR_No4thCST  = 0x40,   //No 4th Cassette
		ERR_Empty4thCST = 0x41, //4th cassette empty
		ERR_BD4thCST	= 0x42,  //4th cassette: Error related to bill diagnosis

		ERR_No8thCST  = 0x44,   //No 8th Cassette
		ERR_Empty8thCST = 0x45, //8th cassette empty
		ERR_BD8thCST	= 0x46,  //8th cassette: Error related to bill diagnosis

		ERR_Pick4thCST = 0x48,   //4th Cassette pick error
		ERR_Pick8thCST = 0x4C,   //8th Cassette pick error

		ERR_HomePos	  = 0x50,	 //Home position error
		ERR_UpperPos  = 0x51,	 //Upper position error
		ERR_NoBill	  = 0x52,	 //No bill in pool section

		ERR_MedRemain = 0x70,	 //Medium remain
		ERR_MedOut	  = 0x76,	 //Medium pulling out

		ERR_JAM_DFLSnToDFSS = 0x78, //JAM (FDLSn～DFSS)
		ERR_JAM_DFSSToBPS = 0x7A, //JAM (DFSS～BPS)
		ERR_JAM_DFSSToREJS = 0x7B, //JAM (DFSS～REJS)
		ERR_JAM_poolsectionToEJSForEJSR = 0x7C, //JAM (pool section ～ EJSF/EJSR)
		ERR_JAM_EJSForEJSRTopoolsection = 0x7D, //JAM (EJSF/EJSR ～ pool section)

		ERR_BillLenlong = 0x82,		//Bill length error (long)
		ERR_BillLenshort = 0x83,		//Bill length error (short)
		ERR_Thickness = 0x84,		//Thickness error
		ERR_Pickfromas  = 0x85,		//Pick from another safe
		ERR_AbnormalSpac = 0x86,    //Abnormal spacing
		ERR_CountMismatch = 0x88,   //Count mismatch
		ERR_Potentiometer = 0x89,	//Potentiometer error

		ERR_FrontShutterOpen = 0xA1, //(Front) Shutter open error
		ERR_FrontShutterClose = 0xA2, //(Front) Shutter close error
		ERR_FrontNoBil		= 0xA4,   //(Front) No bills in the ejection opening
		ERR_FrontOpenShutter = 0xA7,  //(Front) Open shutter

		ERR_RearShutterOpen = 0xA9,   //(Rear) Shutter open error
		ERR_RearShutterClose = 0xAA,  //(Rear) Shutter close error
		ERR_RearNoBil		= 0xAC,   //(Rear) No bills in the ejection opening
		ERR_RearOpenShutter = 0xAF,   //(Rear) Open shutter

		ERR_RejBoxOption = 0xB0,   //Reject box (option) is no set.
		ERR_Rejboxverflow = 0xB5,  //Reject box overflow

		ERR_FrontCapBoxOption = 0xB8,   //(Front) Capture box (option) is no setup
		ERR_FrontCapBoxNoSet = 0xB9,    //(Front) Capture box is no set
		ERR_FrontCapBoxOverFlow = 0xBA, //(Front) Capture box overflow

		ERR_RearCapBoxOption = 0xBC,	//(Rear) Capture box (option) is no setup
		ERR_RearCapBoxNoSet = 0xBD,		//(Rear) Capture box is no set
		ERR_RearCapBoxOverFlow = 0xBE,	//(Rear) Capture box overflow

		ERR_Seq = 0xC0,					//Sequence error
		ERR_Download1 = 0xC1,			//Download error
		ERR_CMDEXPTRTLDLE = 0xC2,		//Command received except RT, LD and LE
		ERR_Download2 = 0xC3,			//Download error

		ERR_RASCMDundef = 0xE0,			//RAS command undefined
		ERR_ParaNotRegister = 0xE1,		//Parameter not registered

		ERR_BillInfoNotProvide = 0xE4,		//Bill information not provided
		ERR_Specification = 0xE5,			//Specification error
		ERR_ParaISOCode = 0xE6,				//Parameter ISO code error
		ERR_BillLenorThicknessInfo = 0xE8,	//Bill length/ thickness information error
		ERR_Para  = 0xEA,					//Parameter error
		ERR_FS = 0xEC,						//FS error
		ERR_CMDFormat = 0xEE,				//Command format error
		ERR_CMDNoExec = 0xEF,				//Command execution is impossible

		ERR_OverCurrent = 0xF1,				//Over current error
		ERR_AbnormalOptSetup = 0xF2,		//Option setup is unusual.
		ERR_FrontAbnormalOptSetup = 0xF3,	//(Front) Option setup is unusual.
		ERR_RearAbnormalOptSetup = 0xF4,	//(Rear) Option setup is unusual.
		ERR_LogDataChecksum = 0xF6,			//Log data check sum error
		ERR_Sensor = 0xF8,					//Sensor error
		ERR_IllegalOper = 0xFC,				//Illegal operation (F56S)
		ERR_PowerOffDuringCount = 0xFD,		//Power off during count (F56S)

		ERROR_BH_CHANGE_NOT_STANDBY				= 500,	// 模块未到位
		ERROR_BH_CHANGE_BOX_A_NOT_INSTALL		= 501,	// 找零箱A未安装
		ERROR_BH_CHANGE_BOX_B_NOT_INSTALL		= 502,	// 找零箱B未安装
		ERROR_BH_CHANGE_WASTE_BOX_NOT_INSTALL	= 503,	// 废币箱未安装
		ERROR_BH_CHANGE_BOX_EMPTY				= 504,	// 钱箱非正常空
		ERROR_BH_CHANGE_IGILL_INSTALL_BOX_A		= 505,	// 钱箱A非法安装
		ERROR_BH_CHANGE_IGILL_INSTALL_BOX_B		= 506,	// 钱箱B非法安装
		ERROR_BH_CHANGE_IGILL_INSTALL_BOX_INVALID = 507,// 废币箱非法安装
	}ERRCODE;

	CBHChangeHardwareException (long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	~CBHChangeHardwareException (void);

private:
};