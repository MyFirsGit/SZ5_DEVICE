#pragma once
#include "ACCAuditDef.h"
#include "CACCAuditException.h"
#include "Util.h"
#include "ACCAuditExp.h"


typedef map<unsigned short,int> DEVICEREG_AR;


// 设备寄存器唯一接口
#define theDeviceRegAR DeviceRegAR::GetInstance()  
// 
class ACCAUDIT_DECLSPEC DeviceRegAR
{
public:

	static DeviceRegAR &GetInstance()
	{
		return theInstance;
	}

	void Initialize(CString strMainPath,CString strBackupPath);

	// TH 设备寄存器
	void AddThResetCount(int nAddCount = 1);			// 整机使用次数
	void AddThBoxAUpDownCount(int nAddCount = 1);		// 票箱A升降次数
	void AddThBoxBUpDownCount(int nAddCount = 1);		// 票箱B升降次数
	void AddThScratchWheelACount(int nAddCount = 1);	// 刮票A刮票次数
	void AddThScratchWheelBCount(int nAddCount = 1);	// 刮票B刮票次数

	//void AddThTransMotorCount(int nAddCount = 1);		// 传输电机使用次数
	void AddThSendOutMotorCount(int nAddCount = 1);		// 出票电机使用次数
	void AddThLinkElecFlipCount(int nAddCount = 1);		// 连杆电磁铁翻动次数

	// CH 设备寄存器
	void AddChResetCount(int nAddCount = 1);			// 整机适用次数
	void AddChSortTrayRunCount(int nAddCount = 1);		// 分拣盘转动次数
	void AddChTransMotorCount(int nAddCount = 1);		// 传输电机转动次数
	void AddChCycleChangeCount(int nAddCount = 1);		// 循环找零箱找零次数
	void AddChSupplayBoxAChangeCount(int nAddCount = 1);// 补币箱A找零次数
	void AddChSupplayBoxBChangeCount(int nAddCount = 1);// 补币箱B找零次数

	// BHChange 设备寄存器
	void AddBhcResetCount(int nAddCount = 1);			// 整机使用次数
	void AddBhcBoxAChangeCount(int nAddCount = 1);		// 纸币找零箱A擦钞次数
	void AddBhcBoxBChangeCount(int nAddCount = 1);		// 纸币找零箱B檫钞次数

	// BH 设备寄存器
	void AddBhResetCount(int nAddCount = 1);			// 整机使用次数
	void AddBhRecongnitionCount(int nAddCount = 1);		// 纸币识别次数
	void AddBhMotorPressCount(int nAddCount = 1);		// 押钞电机压箱次数

	// 其他
	void AddAR(DEVICEREG_AR& accAR);
	DEVICEREG_AR& GetDeviceRegAr();

	void ClearDeviceAR();								// 清空AR数据
	
private:
	CString m_dataPath;
	CString m_BackupPath;

	DEVICEREG_AR m_deviceRegAr;

	static DeviceRegAR theInstance;

	void LoadACCARFromFile(DEVICEREG_AR&);
	void SaveACCARToFile(DEVICEREG_AR&);
	void SerializeACCAR(DEVICEREG_AR&,VARIABLE_DATA&);
	void DeserializeACCAR(DEVICEREG_AR&,VARIABLE_DATA&);
	void Initialize();

	DeviceRegAR(void);
	~DeviceRegAR(void);
};
