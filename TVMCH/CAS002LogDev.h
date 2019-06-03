// CAS002LogDev.h: interface for the OCAS002LogDev class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAS002LOGDEV_H__FA7F1E0A_27DE_4384_9847_61302CEBE8CF__INCLUDED_)
#define AFX_CAS002LOGDEV_H__FA7F1E0A_27DE_4384_9847_61302CEBE8CF__INCLUDED_

#include "OdevBase.h"
#include "CAS002DevStruct.h"

class AFX_EXT_CLASS OCAS002LogDev
{
public:
	OCAS002LogDev();
	virtual ~OCAS002LogDev();

	int iSetCommPara(tDevReturn* p_psStatus); // 设置通讯参数	
	int iInit(tModuleStatus* p_psModule, tDevReturn* p_psStatus); // 初始化

	// 硬币处理模块功能函数
	int iGetDevStatus(tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // 取设备状态
	int iGetDevVersion(tModuleVersion* p_psDevVersion, tDevReturn* p_psStatus); // 取介质程序版本号
	int iGetVersion(char *p_pcVersion); // 获取软件版本
	
	int iStartRevCoin(tModuleStatus *p_psModuleStatus, tDevReturn *p_psStatus); // 开始接收硬币	
	int iStopRevCoin(tModuleStatus *p_psModuleStatus, tDevReturn *p_psStatus); // 结束接收硬币

	int iChange(const UINT p_wChange, tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // 自动找零	
	int iChangeEx(tChange* p_sChange, tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // 手动找零	

	int iReadCoinNum(_tCoinInfo* p_psCoinInfo, tDevReturn* p_psStatus); // 读取接收金额
	int iWriteCoinNum(tSetCoinNum* p_sCoinNum, tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // 设置金额
	
	int iClearCoinFlowNum(tCoinFlow *p_psCoinFlow, tDevReturn *p_psStatus); // 清除硬币流向的数量统计信息
	int iReadCoinFlowNum(tCoinFlow *p_psCoinFlow, tDevReturn *p_psStatus);  // 读取硬币流向的数量统计信息
	
	int iEjectCoin(tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // 退出硬币
	int iClearCoin(tClearCoin* p_sClearCoin, UINT& p_wHopper1Num, UINT& p_wHopper2Num, tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // 清币
	
	int iAcceptCoinEx(tModuleStatus* p_psModuleStatus, UINT& p_wCoinType1Num, UINT& p_wCoinType2Num, tDevReturn* p_psStatus, const BYTE p_byDir = 1); // 手动存入硬币，上层指出倒出位置
	int iAcceptCoin(tModuleStatus* p_psModuleStatus, UINT& p_wCoinType1Num, UINT& p_wCoinType2Num, tDevReturn* p_psStatus); // 自动存入硬币，存入钱箱
	
	int iDenominate(UINT p_wChange, tChange* p_psChange, tDevReturn* p_psStatus); // 逻辑配钞
	int iSaveRecord(tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // 保存记录
	int iReadAuditData(tAuditData* p_psAuditData, tDevReturn* p_psStatus); // 读审计信息
	int iReadCamInfo(UINT& p_wLoopNum, UINT& p_wReturnNum, tDevReturn* p_psStatus); // 查询凸轮转动次数及退币数

	int iBeep(BYTE p_byBeepTimes, BYTE p_byBeepLength,  BYTE p_byBeepInterval, tDevReturn* p_psStatus); // 蜂鸣器发声
	int iRouterControl(const UINT p_wDivNo, tModuleStatus *p_psModuleStatus, tDevReturn *p_psStatus); // 换向器控制
	int iEscrowControl(const UINT p_wEscrowNo, tModuleStatus* p_psModuleStatus, tDevReturn *p_psStatus); // 暂存器控制
	
	int iReadSN(BYTE p_abySerialNo[20], tDevReturn* p_psStatus); // 读SN号
	int iWriteSN(BYTE p_abySerialNo[20], tDevReturn* p_psStatus); // 写SN号

	int iReadiButtonID(const BYTE p_byiButtonNo, BYTE* p_pbyiBtnID, tDevReturn* p_psStatus); // 读iButton ID号
	int iCleariButton(const BYTE p_byiButtonNo, tDevReturn* p_psStatus); // 清除iButton所有数据
	int iReadiButton(const BYTE p_byiButtonNo, tiButtonData* p_psReadData, tDevReturn* p_psStatus); // 读iButton
	int iWriteiButton(const BYTE p_byiButtonNo, tiButtonData* p_psWriteData,  tDevReturn*  p_psStatus); // 写iButton

	int iClearLog(tDevReturn* p_psStatus);
	int iReadLog(unsigned char p_pcLogDate[3], tDevReturn* p_psStatus);
	int iGetLogTableInfo(tLogTableInfo *p_psLogTable, tDevReturn *p_psStatus);

	// 图像识别模块操作
	int iDownLoadData(char* p_pcFilePath, tDevReturn* p_psStatus); //下载数据
	int iDownLoadUpdate(tDevReturn* p_psStatus);	// 更新程序
	int iSetTime(tDevReturn* p_psStatus);	// 设置时间

	ODevBase* oGetODevBase(); // 获取 odevbase 指针

	int iSetRejectType(const tRejectInfo* p_psRejectInfo, tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // 硬币拒收控制
	
	//added by hguang 2012-04-27
	int iReadSN(tCAS002_Data* p_psData, tDevReturn* p_psStatus); // 读SN
	
	int iRFWriteBlock(const tCAS002_RFIDInfo* p_psRfInfo, const tCAS002_Data* p_psData, tDevReturn *p_psrStatus);
	int iRFReadBlock(const tCAS002_RFIDInfo* p_psRfInfo, tCAS002_Data* p_psData, tDevReturn *p_psrStatus);
	int iRFWriteSector(const tCAS002_RFIDInfo* p_psRfInfo, const tCAS002_Data* p_psData, tDevReturn *p_psrStatus);
	int iRFReadSector(const tCAS002_RFIDInfo* p_psRfInfo, tCAS002_Data* p_psData, tDevReturn *p_psrStatus);
	int iGetRFIDCardInfo(unsigned char p_ucPortId, tCAS002_Data* p_psData, tDevReturn *p_psrStatus);
	
	int iGetCoinBoxSN(const unsigned char p_ucCoinBoxId, tCAS002_Data* p_psData, tDevReturn* p_psrStatus); 
	int iSetCoinBoxSN(const unsigned char p_ucCoinBoxId, const tCAS002_Data* p_psData, tDevReturn* p_psrStatus); 
	//end added
	int iRouterControlEx(const UINT p_wDivNo, const UINT p_wDir, tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // 换向器控制

	//added by ZacharyLiu 2015-12-03
	int iGetDeviceCfgPara(BYTE p_byParaType, tCAS002C_CfgPara* p_psCfgPara, tDevReturn* p_psStatus);
	int iSetDeviceCfgPara(BYTE p_byParaType, tCAS002C_CfgPara* p_psCfgPara, tDevReturn* p_psStatus);
	
	//维护接口
	int iControlGate(BYTE p_byAction, tDevReturn* p_psStatus);
	//end added

	//added by lfan 2016-02-18
	int iSetCommParaEx(tDevReturn* p_psStatus); // 设置通讯参数,不读取版本号，仅供下载时使用
	//end added

private:
	class OCAS002LogBase* m_pCAS002LogBase;	// 逻辑层实现类指针
};

#endif // !defined(AFX_CAS002LOGDEV_H__FA7F1E0A_27DE_4384_9847_61302CEBE8CF__INCLUDED_)
