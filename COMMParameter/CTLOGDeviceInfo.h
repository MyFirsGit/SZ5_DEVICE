#pragma once
#include "CParameterCommandExp.h"
#include "dxfile.h"

#define theDeviceInfo    CCTLOGDeviceInfo::GetInstance()             // CCTLOGDeviceInfo的唯一接口

/**
@brief   设备信息
*/
class PARAMETERCOMMAND_DECLSPEC CCTLOGDeviceInfo
{
public:	
	// 设备信息
	typedef struct _device_info {	         //     TVM           TOM          ES For Card          ES For Token
		BYTE ACC_RW1_ISAM[6];                // 充值读写器      读写器           读写器              读写器1
		BYTE ACC_RW1_PSAM[6];                // 充值读写器      读写器           读写器              读写器1
		BYTE ECT_RW1_ISAM[6];                // 充值读写器      读写器           读写器（母卡）      读写器1（母卡）
		BYTE ECT_RW1_PSAM[6];                // 充值读写器      读写器           读写器（传输卡）    读写器1（传输卡）

		BYTE ACC_RW2_ISAM[6];                // Token读写器                                          读写器2
		BYTE ACC_RW2_PSAM[6];                // Token读写器                                          读写器2
		BYTE ECT_RW2_ISAM[6];                // Token读写器                                          读写器2（母卡）
		BYTE ECT_RW2_PSAM[6];                // Token读写器                                          读写器2（传输卡）

		BYTE ACC_RW3_ISAM[6];                // Card读写器                                           读写器3
		BYTE ACC_RW3_PSAM[6];                // Card读写器                                           读写器3
		BYTE ECT_RW3_ISAM[6];                // Card读写器                                           读写器3（母卡）
		BYTE ECT_RW3_PSAM[6];                // Card读写器                                           读写器3（传输卡）

		BYTE RW_DEVICE_ID[6];               // 外部读写器设备ID
		BYTE TW_DEVICE_ID[6];               // 内部读写器设备ID
		BYTE PRT_DEVICE_ID[6];              // 打印机设备ID
		BYTE TH_DEVICE_ID[6];               // 出票机设备ID

		_device_info() {
			memset(ACC_RW1_ISAM,0,6);
			memset(ACC_RW1_PSAM,0,6);
			memset(ECT_RW1_ISAM,0,6);
			memset(ECT_RW1_PSAM,0,6);

			memset(ACC_RW2_ISAM,0,6);
			memset(ACC_RW2_PSAM,0,6);
			memset(ECT_RW2_ISAM,0,6);
			memset(ECT_RW2_PSAM,0,6);

			memset(ACC_RW3_ISAM,0,6);
			memset(ACC_RW3_PSAM,0,6);
			memset(ECT_RW3_ISAM,0,6);
			memset(ECT_RW3_PSAM,0,6);

			memset(RW_DEVICE_ID, 0,6);
			memset(TW_DEVICE_ID, 0,6);
			memset(PRT_DEVICE_ID,0,6);
			memset(TH_DEVICE_ID, 0,6);
		}
	} DEVICE_INFO;

	BOOL Internalize(CString dataPath, CString backupPath);     // 初期化

	void GetDeviceInfo(DEVICE_INFO&);                           // 读取设备信息
	//RW 1
	void SetACC_RW1_ISAM(LPBYTE);            // 设置RW1的ACC ISAM卡号
	void SetACC_RW1_PSAM(LPBYTE);            // 设置RW1的ACC PSAM卡号
	void SetETC_RW1_ISAM(LPBYTE);            // 设置RW1的ACC ISAM卡号
	void SetETC_RW1_PSAM(LPBYTE);            // 设置RW1的ACC PSAM卡号
	//RW 2
	void SetACC_RW2_ISAM(LPBYTE);            // 设置RW2的ACC ISAM卡号
	void SetACC_RW2_PSAM(LPBYTE);            // 设置RW2的ACC PSAM卡号
	void SetETC_RW2_ISAM(LPBYTE);            // 设置RW2的ACC ISAM卡号
	void SetETC_RW2_PSAM(LPBYTE);            // 设置RW2的ACC PSAM卡号
	//RW 3
	void SetACC_RW3_ISAM(LPBYTE);            // 设置RW3的ACC ISAM卡号
	void SetACC_RW3_PSAM(LPBYTE);            // 设置RW3的ACC PSAM卡号
	void SetETC_RW3_ISAM(LPBYTE);            // 设置RW3的ACC ISAM卡号
	void SetETC_RW3_PSAM(LPBYTE);            // 设置RW3的ACC PSAM卡号

	void SetRWId(LPBYTE);					// 设置外部读写器设备ID
	void SetTWId(LPBYTE);					// 设置内部读写器设备ID
	void SetPRTId(LPBYTE);					// 设置打印机设备ID
	void SetTHId(LPBYTE);					// 设置出票机设备ID

	static CCTLOGDeviceInfo&  GetInstance()
	{
		return theInstance;
	}

private:
	enum {
		DEVICE_INFO_LEN = 96,	            ///< 文件长度
	};

	// 字段所在位置
	enum {
		ACC_RW1_ISAM_POS = 0,                 // 充值读写器
		ACC_RW1_PSAM_POS = 6,                 // 充值读写器
		ECT_RW1_ISAM_POS = 12,                // 充值读写器
		ECT_RW1_PSAM_POS = 18,                // 充值读写器

		ACC_RW2_ISAM_POS = 24,                // Token读写器
		ACC_RW2_PSAM_POS = 30,                // Token读写器
		ECT_RW2_ISAM_POS = 36,                // Token读写器
		ECT_RW2_PSAM_POS = 42,                // Token读写器

		ACC_RW3_ISAM_POS = 48,                // Card读写器
		ACC_RW3_PSAM_POS = 54,                // Card读写器
		ECT_RW3_ISAM_POS = 60,                // Card读写器
		ECT_RW3_PSAM_POS = 66,                // Card读写器

		RW_DEVICE_ID_POS  = 72,               // 外部读写器设备ID,
		TW_DEVICE_ID_POS  = 78,               // 内部读写器设备ID,
		PRT_DEVICE_ID_POS = 84,               // 打印机设备ID,
		TH_DEVICE_ID_POS  = 90,               // 出票机设备ID,
	};

	DEVICE_INFO    m_deviceInfo;                    // 设备信息
	CDXFile         *m_file;                         // 文件读取控制类
	char           m_fileData[DEVICE_INFO_LEN];     // 当前文件内容

	static  CCTLOGDeviceInfo theInstance;
	CCTLOGDeviceInfo(void);
	~CCTLOGDeviceInfo();
	CCTLOGDeviceInfo(const CCTLOGDeviceInfo&);
	CCTLOGDeviceInfo& operator=(const CCTLOGDeviceInfo&);
};
