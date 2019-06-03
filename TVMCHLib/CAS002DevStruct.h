#ifndef _CASDEVSTRUCT_H_
#define _CASDEVSTRUCT_H_

// 设置金额结构
typedef struct _tSetCoinNum
{
	int uiMHopper1Num; // 主找零器2硬币个数，小于1100。	//武汉11号线专用
	int uiMHopper2Num; // 主找零器1硬币个数，小于1100。	//武汉11号线专用

	UINT uiCoinBoxCoinANum; // 硬币钱箱硬币A个数，加上硬币B小于3000。
	UINT uiCoinBoxCoinBNum; // 硬币钱箱硬币B个数，加上硬币A小于3000。

	BYTE byEscrow1Num; // 暂存器1硬币数，小于70。
	BYTE byEscrow2Num; // 暂存器2硬币数，小于70。

	BYTE byMHopper1Type; // 主找零器2硬币类型，1 = 1元硬币，2 = 5角硬币。	//武汉11号线专用
	BYTE byMHopper2Type; // 主找零器1硬币类型，1 = 1元硬币，2 = 5角硬币。	//武汉11号线专用
		
	BYTE byEjectCoinNum; // 退币个数

}tSetCoinNum;

// 图像识别模块状态结构 add by lcl 2008.11.26
typedef struct _tImageValidator
{
	int	iRunStatus;				// 运行状态       0: 暂停运行 1: 正常运行
	int	iImageCatchAreaStatus;	// 图像捕捉区状态 0: 卡币状态 1: 正常状态
	int	iLightingStatus;		// 灯光状态       0: 故障状态 1: 正常状态
	int	iBackDropStatus;		// 背景状态       0: 故障状态 1: 正常状态
	int	iImageCollectionStatus;	// 图像采集状态   0: 故障状态 1: 正常状态
	int	iIdentityStatus;		// 传感器状态     0: 故障状态 1: 正常状态
	int	iSeriesFalseCoinStatus;	// 连续假币状态   0: 故障状态 1: 正常状态
	int	iReserve1;				// 预留
	int	iReserve2;				// 预留
}tImageValidator;

// 状态结构
typedef struct _tModuleStatus
{
	bool biButton1;	// 主找零器2加币箱iButton  false：检测不到加币箱iButton, true：检测到加币箱iButton	//武汉11号线专用
	bool biButton2; // 主找零器1加币箱iButton, false：检测不到加币箱iButton, true：检测到加币箱iButton	//武汉11号线专用
	bool biButton3; // 钱箱, false：检测不到加币箱, true：检测到加币箱
	//added by ZacharLiu 2015-11-30
	bool bCoinGate; // 硬币闸门状态, false:闸门正常, true:闸门有故障
	//end added

	bool bMHopper1; // 主找零器1到位Sensor  false：检测不到HOPPER, true：检测到HOPPER
	bool bMHopper2; // 主找零器2到位Sensor  false：检测不到HOPPER, true：检测到HOPPER

	bool bMHopper1Sensor;	// 主找零器2出币传感器,true：有币,false：没币		//武汉11号线专用
	bool bMHopper2Sensor;	// 主找零器1出币传感器,true：有币,false：没币		//武汉11号线专用
		
	bool bRouter1RightSensor; // 换向器1右传感器,true：挡片不在右边, false：挡片在右边（处于回收硬币状态）
	bool bRouter1LeftSensor;	// 换向器1左传感器,true：挡片不在左边, false：挡片在左边（处于出币状态）
	bool bRouter2RightSensor; // 换向器2右传感器,true：挡片不在右边, false：挡片在右边（处于回收硬币状态）
	bool bRouter2LeftSensor;	// 换向器2左传感器,true：挡片不在左边, false：挡片在左边（处于出币状态）
	
	bool bCoinInSensor; // 投币入口传感器,true：入口无币,false：入口有币
	bool bCoinOut1Sensor;	// 投币出口传感器1,true：出口1无币,false：出口1有币
	bool bCoinOut2Sensor;	// 投币出口传感器2,true：出口2无币,false：出口2有币
	bool bDoorSensor;	// 门传感器,true：门已关,false：门没关
	bool bCamSensor;	// 凸轮传感器,true：凸轮没复位, false：凸轮己复位
	bool bPositionSensor; // 拖架位置传感器,true--传感器没有被挡(模块不到位) false--传感器被挡(模块到位)

	bool bEscrow1RightSensor; // 暂存器1右传感器, true：存币兜不在右边, false：存币兜翻到右边
	bool bEscrow1MidSensor; // 暂存器1中间传感器, true：存币兜不在中间, false：存币兜翻到中间
	bool bEscrow1LeftSensor;	// 暂存器1左传感器, true：存币兜不在左边, false：存币兜翻到左边
	bool bEscrow2RightSensor; // 暂存器2右传感器, true：存币兜不在右边, false：存币兜翻到右边
	bool bEscrow2MidSensor; // 暂存器2中间传感器, true：存币兜不在中间, false：存币兜翻到中间
	bool bEscrow2LeftSensor;	// 暂存器2左传感器, true：存币兜不在左边, false：存币兜翻到左边

	bool bMHopper1LowSensor;//主找零器1低位信号，true：不空，false：空
	bool bMHopper1HighSensor;//主找零器1高位信号，true：满币，false：不满
	bool bMHopper1SafeSensor;//主找零器1安全信号，true：卡币，false：没币
	bool bMHopper1RawSensor; // 主找零器1原始出币信号，true：有币，false：没币
	
	bool bMHopper2LowSensor;//主找零器2低位信号，true：不空，false：空
	bool bMHopper2HighSensor;//主找零器2高位信号，true：满币，false：不满
	bool bMHopper2SafeSensor;//主找零器2安全信号，true：卡币，false：没币
	bool bMHopper2RawSensor; // 主找零器2原始出币信号，true：有币，false：没币
	
	// add by Money.Chan 2007-10-29
	bool bImageControl1; // 图象识别控制1 false: 图象为真 true: 图象为假
	bool bImageControl2; // 图象识别控制2 false: 图象为真 true: 图象为假
	
	bool bCoinBoxSensor; // 硬币钱箱位置  false: 不到位 true: 到位
	
	bool bPrevSensor1; // 预留传感器1 false: 传感器被挡 true: 传感器没挡
	bool bPrevSensor2; // 预留传感器2 false: 传感器被挡 true: 传感器没挡
	// end add by Money.Chan 2007-10-29

	tSetCoinNum tCoinNum;
	tImageValidator tImageValidatorStatus; // 图像识别模块状态 Add by lcliang 2008.11.26

	char acReserve[256]; // 预留
}tModuleStatus;

// 清币结构 
typedef struct _tClearCoin
{
	BOOL bClearMHopper1; // 主找零器2清币标志		//武汉11号线专用
	BOOL bClearMHopper2; // 主找零器1清币标志		//武汉11号线专用

}tClearCoin;

// iButton数据结构
typedef struct _tiButtonData
{
	BYTE byBoxID[8]; // 钱箱ID
	BYTE byOperatorID[8]; // 操作人ID

	WORD wCoinNum; // 钱箱硬币数
	BYTE byCoinType; // 硬币类型

	BYTE byAttachTime[6]; // 放入设备时间
	BYTE byDetachTime[6]; // 取走设备时间

	char acReserve[128]; //预留
}tiButtonData;

// 模块版本号
typedef struct _tModuleVersion
{
	char acDevType[7]; // 模块型号
	char acFirmWareVersion[7]; // 固件版本号	
	char acSerialNumber[32]; // SN号
	char acImageVersion[17]; // 图像识别模块版本add by lcl 2008.11.26

}tModuleVersion;

// 审计数据
typedef struct _tAuditData
{
	UINT uiTotalRevCoinA;		// 总接收A类硬币数
	UINT uiTotalRevCoinB;		// 总接收B类硬币数
	UINT uiTotalChangeCoinA;	// 总找零A类硬币数
	UINT uiTotalChangeCoinB;	// 总找零B类硬币数
	DOUBLE ulImageRealCoin;		// 图像处理真币数
	DOUBLE ulImageFakeCoin;		// 图像处理假币数
}tAuditData;

// 找零结构
typedef struct _tChange
{
	BYTE byMHopper1Num; // 主找零器2找零数	//武汉11号线专用
	BYTE byMHopper2Num; // 主找零器1找零数	//武汉11号线专用
	
}tChange;

// 钞票结构
typedef struct _tCoinInfo
{ 
	char acCurrency[4]; // 币种
	int iCount;			// 张数
	long lDenomination; // 面额
	char cSerial;		// 钞票套数(第四或第五套'4','5')
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

// 记录硬币的流向结构
typedef struct _tCoinFlow
{
	WORD wEscrow1ToCoinOut;  // 从暂存器1到硬币出口
	WORD wEscrow1ToHopper1;  // 从暂存器1到Hopper1
	WORD wEscrow1ToCoinBox;  // 从暂存器1到硬币钱箱

	WORD wEscrow2ToCoinOut;  // 从暂存器2到硬币出口
	WORD wEscrow2ToCoinBox;  // 从暂存器2到硬币钱箱

	WORD wHopper1ToCoinOut;  // 从Hopper1到硬币出口
	WORD wHopper1ToCoinBox;  // 从Hopper1到硬币钱箱

	WORD wHopper2ToCoinOut;  // 从Hopper2到硬币出口
	WORD wHopper2ToCoinBox;  // 从Hopper2到硬币钱箱

}tCoinFlow;

// 记录Log表结构体
typedef struct _tLogTableInfo 
{
	BYTE acYearDateTable[12];	// Log年日期列表
	BYTE acMonthDateTable[12];	// Log月日期列表
	BYTE acDayDateTable[12];	// Log日日期列表
	BYTE acAddrTable[24];		// Log地址列表
	BYTE acSectorTable[12];		// Log扇区列表
	BYTE byCurrentSectorNo;		// 当前Log扇区编号
	UINT wCurrentSectorAddr;	// 当前Log扇区地址
	BYTE byCurrentDateNo;		// 当前日期编号
	BYTE acLogCRC[2];			// CRC校验

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

// Log信息存储结构
typedef struct _tLogInfoStruct
{
	UINT wLogLength;     // Log长度
	UINT wLogSectorAddr; // Log起始地址
	BYTE bySectorNo;     // Log扇区编号

	_tLogInfoStruct()
	{
		wLogLength = 0;
		wLogSectorAddr = 0;
		bySectorNo = 0;
	}

}tLogInfoStruct;


// 拒收硬币类型
typedef struct _tRejectData
{
	byte byType;		// 币种 0---接收 1---拒收
	int iDenomination;	// 拒收面额 
						// 对RMB,bit0---5角 bit1---1元，其它位保留
						// 每一位为0表示接收，1表示拒收
}tRejectData;

// 拒收硬币数据
typedef struct _tRejectInfo
{
	tRejectData sRejectData[32]; //0维 --- RMB 其它维保留
}tRejectInfo;

//读写数据结构体
typedef struct _tData
{
	char acData[64]; //读出或写入的数据
}tCAS002_Data;

//RFID 信息结构体
typedef struct _tRFIDInfo
{
	unsigned char ucPortId;		//端口号:0x01 = 加币箱2  0x02 = 加币箱1  0x03 = 回收箱1  0x04 = 回收箱2	//武汉11号线专用
	unsigned char ucIndex;		//Block/Sector 号
}tCAS002_RFIDInfo;

//added by ZachartLiu 2015-12-03
typedef struct _tCfgPara
{
	unsigned char ucTypeInfo[7];
	unsigned char ucParaValue[4];

}tCAS002C_CfgPara;
//end added

#endif	// #ifndef _CASDEVSTRUCT_H_
