#include "StdAfx.h"
#include "DeviceRegAR.h"
#include "Sync.h"
#include "Util.h"
#include "CTLOGCountInfo.h"
#include "CACCAuditException.h"
#include "MaintenanceInfo.h"
#include "SysInfo.h"



DeviceRegAR DeviceRegAR::theInstance;

DeviceRegAR::DeviceRegAR(void)
{

}

DeviceRegAR::~DeviceRegAR(void)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化设备寄存器数据

@param      CString strMainPath	  数据路径
			CString strBackupPath 备份路径

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::Initialize(CString strMainPath,CString strBackupPath){
	m_dataPath	 = strMainPath;// + AR_MANAGE_FOLDER + AR_MANAGE_DEV_NAME;
	m_BackupPath = strBackupPath;// + AR_MANAGE_FOLDER + AR_MANAGE_DEV_NAME;
	
	if(!CDXFile::CheckAndCreateDirectory(strMainPath + AR_MANAGE_FOLDER,strBackupPath + AR_MANAGE_FOLDER))
	{
		throw CARException(CARException::FOLDER_CREATE_FAIL, _T(__FILE__), __LINE__, _T("二重化创建文件夹失败：" + CString(AR_MANAGE_FOLDER))); 
	}

	Initialize();

	CString strDataFile		= m_dataPath + AR_MANAGE_FOLDER + AR_MANAGE_DEV_NAME;
	CString strBackupFile	= m_BackupPath + AR_MANAGE_FOLDER + AR_MANAGE_DEV_NAME;
	if(!CDXFile::ResuceFile(strDataFile,strBackupFile,true)){

		if(ComCheckFileExist(strDataFile) || ComCheckFileExist(strBackupFile))//文件存在，并且没有二重化成功，抛异常
		{
			throw CARException(CARException::FILE_RESCUE_FAIL, _T(__FILE__), __LINE__, _T("二重化文件失败：" + CString(AR_MANAGE_ACC_FILE_NAME))); 
		}
		SaveACCARToFile(m_deviceRegAr);//不然的话新建，并保存文件
	}

	DEVICEREG_AR savedACCAR;
	LoadACCARFromFile(savedACCAR);
	AddAR(savedACCAR);
	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化数据

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::Initialize(){
	m_deviceRegAr.clear();

	m_deviceRegAr[1001] = 0;	// 整机使用次数；	复位时+1
	m_deviceRegAr[1002] = 0;	// 票箱A升降次数	复位时+1，A票箱安装卸下票箱后+1
	m_deviceRegAr[1003] = 0;	// 票箱B升降次数	复位时+1，B票箱安装卸下完成后+1
	m_deviceRegAr[1004] = 0;	// 刮票轮A刮票次数	A箱刮票成功后+1
	m_deviceRegAr[1005] = 0;	// 刮票轮B刮票次数	B箱刮票成功后+1
	m_deviceRegAr[1006] = 0;	// 传输电机使用次数	刮票次数=1004+1005的和
	m_deviceRegAr[1007] = 0;	// 出票电机使用次数	放出车票动作完成后+1
	m_deviceRegAr[1008] = 0;	// 连杆电磁铁翻动次数	回收车票动作完成后+1

	m_deviceRegAr[2001] = 0;	// 整机使用次数；	复位时+1
	m_deviceRegAr[2002] = 0;	// 硬币分拣盘转动次数	硬币识别动作完成后+1
	m_deviceRegAr[2003] = 0;	// 传输电机转动次数	送出找零动作完成后+1
	m_deviceRegAr[2004] = 0;	// 循环找零箱找零次数	循环找零箱找零后+N（N=找零枚数）
	m_deviceRegAr[2005] = 0;	// 补币箱A找零次数	补币箱A找零后+N（N=找零枚数）
	m_deviceRegAr[2006] = 0;	// 补币箱B找零次数	补币箱B找零后+N（N=找零枚数）

	m_deviceRegAr[3001] = 0;	// 整机使用次数	复位时+1
	m_deviceRegAr[3002] = 0;	// 纸币找零箱A擦钞次数	纸币找零箱A找零后+N（N=找零张数）
	m_deviceRegAr[3003] = 0;	// 纸币找零箱B擦钞次数	纸币找零箱B找零后+N（N=找零张数）
		
	m_deviceRegAr[4001] = 0;	// 整机使用次数	复位时+1
	m_deviceRegAr[4002] = 0;	// 纸币识别次数	纸币识别动作完成后+1
	m_deviceRegAr[4003] = 0;	// 押钞电机压箱次数	压钞动作完成后+1

	/* 票卡回收AG用
	m_deviceRegAr[5001] = 0;	// 整机使用次数	复位时+1
	m_deviceRegAr[5002] = 0;	// 票箱A升降次数	复位时+1，A票箱安装卸下票箱后+1
	m_deviceRegAr[5003] = 0;	// 票箱B升降次数	复位时+1，B票箱安装卸下完成后+1
	m_deviceRegAr[5004] = 0;	// 刮票轮A刮票次数	A箱票卡回收动作完成后+1
	m_deviceRegAr[5005] = 0;	// 刮票轮B刮票次数	B箱票卡回收动作完成后+1
	m_deviceRegAr[5006] = 0;	// 传输电机使用次数	刮票次数=5004+5005的和
	m_deviceRegAr[5007] = 0;	// 连杆电磁铁翻动次数	回收车票到A箱动作完成后+1
	m_deviceRegAr[5008] = 0;	// 入口电磁铁使用次数	回收口开口或关口后动作完成时+1*/

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取设备寄存器数据

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DEVICEREG_AR& DeviceRegAR::GetDeviceRegAr(){
	return m_deviceRegAr;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  添加AR数据，并保存至文件    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddAR(DEVICEREG_AR &accAR)
{
	for(DEVICEREG_AR::iterator arIter = accAR.begin();arIter!=accAR.end();arIter++)
	{
		if(m_deviceRegAr.find(arIter->first)==m_deviceRegAr.end())
		{
			CString sError;
			sError.Format(_T("%d"),arIter->first);
			throw CARException(CARException::KEY_ERR,_T(__FILE__),__LINE__,_T("错误的键值")+sError);
		}
		m_deviceRegAr[arIter->first]+=arIter->second;
	}
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   从文件中加载AR数据   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::LoadACCARFromFile(DEVICEREG_AR& accAR)
{
	VARIABLE_DATA buffer;
	CString accARFileName = m_dataPath+AR_MANAGE_FOLDER+AR_MANAGE_DEV_NAME;
	CString accARBackupFileName = m_BackupPath+AR_MANAGE_FOLDER+AR_MANAGE_DEV_NAME;
	CDXFile accARFile(accARFileName,accARBackupFileName);
	accARFile.ReadBinFile(buffer,true);
	DeserializeACCAR(accAR,buffer);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    保存AR数据到文件  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::SaveACCARToFile(DEVICEREG_AR& accAR)
{
	try{
		VARIABLE_DATA buffer;
		SerializeACCAR(accAR,buffer);
		CString accARFileName = m_dataPath+AR_MANAGE_FOLDER+AR_MANAGE_DEV_NAME;
		CString accARBackupFileName = m_BackupPath+AR_MANAGE_FOLDER+AR_MANAGE_DEV_NAME;
		CDXFile accARFile(accARFileName,accARBackupFileName);
		accARFile.WriteBinFile((const char *)buffer.lpData,buffer.nLen,true);
	}
	catch(...){

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  将AR数据序列化    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::SerializeACCAR(DEVICEREG_AR& accAR,VARIABLE_DATA& buffer)
{
	if(buffer.lpData!=NULL)
	{
		delete buffer.lpData;
		buffer.lpData = NULL;
	}
	int itemCount = accAR.size();
	buffer.nLen = sizeof(int) + itemCount * (sizeof(unsigned short)+sizeof(int));
	buffer.lpData = new BYTE[buffer.nLen];
	LPBYTE lpContent = buffer.lpData;
	memcpy(lpContent,&itemCount,sizeof(int));
	lpContent+=sizeof(int);
	for (DEVICEREG_AR::iterator itemIter = accAR.begin();itemIter!=accAR.end();itemIter++)
	{
		unsigned short key = itemIter->first;
		int value = itemIter->second;
		memcpy(lpContent,&key,sizeof(unsigned short));
		lpContent+=sizeof(unsigned short);
		memcpy(lpContent,&value,sizeof(int));
		lpContent+=sizeof(int);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     将二进流反序列化成AR结构 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::DeserializeACCAR(DEVICEREG_AR& accAR,VARIABLE_DATA& buffer)
{
	LPBYTE lpContent = buffer.lpData;
	int itemCount;
	memcpy(&itemCount,lpContent,sizeof(int));
	lpContent+=sizeof(int);
	for(int i=0;i<itemCount;i++)
	{
		unsigned short key;
		memcpy(&key,lpContent,sizeof(unsigned short));
		lpContent+=sizeof(unsigned short);
		int value;
		memcpy(&value,lpContent,sizeof(int));
		lpContent+=sizeof(int);
		accAR[key]=value;
	}
}


/*********************************TH 设备寄存器*************************************/
//////////////////////////////////////////////////////////////////////////
/*
@brief      整机使用次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddThResetCount(int nAddCount /* = 1 */){
	m_deviceRegAr[1001] += nAddCount;
	m_deviceRegAr[1002] += nAddCount;
	m_deviceRegAr[1003] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      票箱A升降次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddThBoxAUpDownCount(int nAddCount /* = 1 */){
	m_deviceRegAr[1002] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      票箱B升降次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddThBoxBUpDownCount(int nAddCount /* = 1 */){
	m_deviceRegAr[1003] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      刮票A刮票次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddThScratchWheelACount(int nAddCount /* = 1 */){
	m_deviceRegAr[1004] += nAddCount;
	m_deviceRegAr[1006]	 = m_deviceRegAr[1004] + m_deviceRegAr[1005];
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      刮票B刮票次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddThScratchWheelBCount(int nAddCount /* = 1 */){
	m_deviceRegAr[1005] += nAddCount;
	m_deviceRegAr[1006]	 = m_deviceRegAr[1004] + m_deviceRegAr[1005];
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      出票电机使用次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddThSendOutMotorCount(int nAddCount /* = 1 */){
	m_deviceRegAr[1007] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      连杆电磁铁翻动次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddThLinkElecFlipCount(int nAddCount /* = 1 */){
	m_deviceRegAr[1008] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//*********************************CH 设备寄存器*************************/

//////////////////////////////////////////////////////////////////////////
/*
@brief      整机适用次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddChResetCount(int nAddCount /* = 1 */){
	m_deviceRegAr[2001] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      分拣盘转动次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddChSortTrayRunCount(int nAddCount /* = 1 */){
	m_deviceRegAr[2002] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      传输电机转动次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddChTransMotorCount(int nAddCount /* = 1 */){
	m_deviceRegAr[2003] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      循环找零箱找零次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddChCycleChangeCount(int nAddCount /* = 1 */){
	m_deviceRegAr[2004] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      补币箱A找零次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddChSupplayBoxAChangeCount(int nAddCount /* = 1 */){
	m_deviceRegAr[2005] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      补币箱B找零次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddChSupplayBoxBChangeCount(int nAddCount /* = 1 */){
	m_deviceRegAr[2006] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}


//********************************** BHChange 设备寄存器********************/

//////////////////////////////////////////////////////////////////////////
/*
@brief      整机使用次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddBhcResetCount(int nAddCount /* = 1 */){
	m_deviceRegAr[3001] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币找零箱A擦钞次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddBhcBoxAChangeCount(int nAddCount /* = 1 */){
	m_deviceRegAr[3002] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币找零箱B檫钞次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddBhcBoxBChangeCount(int nAddCount /* = 1 */){
	m_deviceRegAr[3003] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

// ***********************************BH 设备寄存器*************************/
//////////////////////////////////////////////////////////////////////////
/*
@brief      整机使用次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddBhResetCount(int nAddCount /* = 1 */){
	m_deviceRegAr[4001] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币识别次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddBhRecongnitionCount(int nAddCount /* = 1 */){
	m_deviceRegAr[4002] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      押钞电机压箱次数

@param      int nAddCount;次数累加和（默认为1）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddBhMotorPressCount(int nAddCount /* = 1 */){
	m_deviceRegAr[4003] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      清空当日计数器数据

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::ClearDeviceAR(){
	Initialize();
	SaveACCARToFile(m_deviceRegAr);
}