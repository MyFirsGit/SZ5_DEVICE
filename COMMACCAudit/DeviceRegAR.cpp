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
@brief      ��ʼ���豸�Ĵ�������

@param      CString strMainPath	  ����·��
			CString strBackupPath ����·��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::Initialize(CString strMainPath,CString strBackupPath){
	m_dataPath	 = strMainPath;// + AR_MANAGE_FOLDER + AR_MANAGE_DEV_NAME;
	m_BackupPath = strBackupPath;// + AR_MANAGE_FOLDER + AR_MANAGE_DEV_NAME;
	
	if(!CDXFile::CheckAndCreateDirectory(strMainPath + AR_MANAGE_FOLDER,strBackupPath + AR_MANAGE_FOLDER))
	{
		throw CARException(CARException::FOLDER_CREATE_FAIL, _T(__FILE__), __LINE__, _T("���ػ������ļ���ʧ�ܣ�" + CString(AR_MANAGE_FOLDER))); 
	}

	Initialize();

	CString strDataFile		= m_dataPath + AR_MANAGE_FOLDER + AR_MANAGE_DEV_NAME;
	CString strBackupFile	= m_BackupPath + AR_MANAGE_FOLDER + AR_MANAGE_DEV_NAME;
	if(!CDXFile::ResuceFile(strDataFile,strBackupFile,true)){

		if(ComCheckFileExist(strDataFile) || ComCheckFileExist(strBackupFile))//�ļ����ڣ�����û�ж��ػ��ɹ������쳣
		{
			throw CARException(CARException::FILE_RESCUE_FAIL, _T(__FILE__), __LINE__, _T("���ػ��ļ�ʧ�ܣ�" + CString(AR_MANAGE_ACC_FILE_NAME))); 
		}
		SaveACCARToFile(m_deviceRegAr);//��Ȼ�Ļ��½����������ļ�
	}

	DEVICEREG_AR savedACCAR;
	LoadACCARFromFile(savedACCAR);
	AddAR(savedACCAR);
	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::Initialize(){
	m_deviceRegAr.clear();

	m_deviceRegAr[1001] = 0;	// ����ʹ�ô�����	��λʱ+1
	m_deviceRegAr[1002] = 0;	// Ʊ��A��������	��λʱ+1��AƱ�䰲װж��Ʊ���+1
	m_deviceRegAr[1003] = 0;	// Ʊ��B��������	��λʱ+1��BƱ�䰲װж����ɺ�+1
	m_deviceRegAr[1004] = 0;	// ��Ʊ��A��Ʊ����	A���Ʊ�ɹ���+1
	m_deviceRegAr[1005] = 0;	// ��Ʊ��B��Ʊ����	B���Ʊ�ɹ���+1
	m_deviceRegAr[1006] = 0;	// ������ʹ�ô���	��Ʊ����=1004+1005�ĺ�
	m_deviceRegAr[1007] = 0;	// ��Ʊ���ʹ�ô���	�ų���Ʊ������ɺ�+1
	m_deviceRegAr[1008] = 0;	// ���˵������������	���ճ�Ʊ������ɺ�+1

	m_deviceRegAr[2001] = 0;	// ����ʹ�ô�����	��λʱ+1
	m_deviceRegAr[2002] = 0;	// Ӳ�ҷּ���ת������	Ӳ��ʶ������ɺ�+1
	m_deviceRegAr[2003] = 0;	// ������ת������	�ͳ����㶯����ɺ�+1
	m_deviceRegAr[2004] = 0;	// ѭ���������������	ѭ�������������+N��N=����ö����
	m_deviceRegAr[2005] = 0;	// ������A�������	������A�����+N��N=����ö����
	m_deviceRegAr[2006] = 0;	// ������B�������	������B�����+N��N=����ö����

	m_deviceRegAr[3001] = 0;	// ����ʹ�ô���	��λʱ+1
	m_deviceRegAr[3002] = 0;	// ֽ��������A��������	ֽ��������A�����+N��N=����������
	m_deviceRegAr[3003] = 0;	// ֽ��������B��������	ֽ��������B�����+N��N=����������
		
	m_deviceRegAr[4001] = 0;	// ����ʹ�ô���	��λʱ+1
	m_deviceRegAr[4002] = 0;	// ֽ��ʶ�����	ֽ��ʶ������ɺ�+1
	m_deviceRegAr[4003] = 0;	// Ѻ�����ѹ�����	ѹ��������ɺ�+1

	/* Ʊ������AG��
	m_deviceRegAr[5001] = 0;	// ����ʹ�ô���	��λʱ+1
	m_deviceRegAr[5002] = 0;	// Ʊ��A��������	��λʱ+1��AƱ�䰲װж��Ʊ���+1
	m_deviceRegAr[5003] = 0;	// Ʊ��B��������	��λʱ+1��BƱ�䰲װж����ɺ�+1
	m_deviceRegAr[5004] = 0;	// ��Ʊ��A��Ʊ����	A��Ʊ�����ն�����ɺ�+1
	m_deviceRegAr[5005] = 0;	// ��Ʊ��B��Ʊ����	B��Ʊ�����ն�����ɺ�+1
	m_deviceRegAr[5006] = 0;	// ������ʹ�ô���	��Ʊ����=5004+5005�ĺ�
	m_deviceRegAr[5007] = 0;	// ���˵������������	���ճ�Ʊ��A�䶯����ɺ�+1
	m_deviceRegAr[5008] = 0;	// ��ڵ����ʹ�ô���	���տڿ��ڻ�ؿں������ʱ+1*/

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ�豸�Ĵ�������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
DEVICEREG_AR& DeviceRegAR::GetDeviceRegAr(){
	return m_deviceRegAr;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  ���AR���ݣ����������ļ�    

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
			throw CARException(CARException::KEY_ERR,_T(__FILE__),__LINE__,_T("����ļ�ֵ")+sError);
		}
		m_deviceRegAr[arIter->first]+=arIter->second;
	}
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   ���ļ��м���AR����   

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
@brief    ����AR���ݵ��ļ�  

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
@brief  ��AR�������л�    

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
@brief     �������������л���AR�ṹ 

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


/*********************************TH �豸�Ĵ���*************************************/
//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ʹ�ô���

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
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
@brief      Ʊ��A��������

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddThBoxAUpDownCount(int nAddCount /* = 1 */){
	m_deviceRegAr[1002] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Ʊ��B��������

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddThBoxBUpDownCount(int nAddCount /* = 1 */){
	m_deviceRegAr[1003] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ƱA��Ʊ����

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddThScratchWheelACount(int nAddCount /* = 1 */){
	m_deviceRegAr[1004] += nAddCount;
	m_deviceRegAr[1006]	 = m_deviceRegAr[1004] + m_deviceRegAr[1005];
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ƱB��Ʊ����

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddThScratchWheelBCount(int nAddCount /* = 1 */){
	m_deviceRegAr[1005] += nAddCount;
	m_deviceRegAr[1006]	 = m_deviceRegAr[1004] + m_deviceRegAr[1005];
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��Ʊ���ʹ�ô���

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddThSendOutMotorCount(int nAddCount /* = 1 */){
	m_deviceRegAr[1007] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���˵������������

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddThLinkElecFlipCount(int nAddCount /* = 1 */){
	m_deviceRegAr[1008] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//*********************************CH �豸�Ĵ���*************************/

//////////////////////////////////////////////////////////////////////////
/*
@brief      �������ô���

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddChResetCount(int nAddCount /* = 1 */){
	m_deviceRegAr[2001] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ּ���ת������

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddChSortTrayRunCount(int nAddCount /* = 1 */){
	m_deviceRegAr[2002] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������ת������

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddChTransMotorCount(int nAddCount /* = 1 */){
	m_deviceRegAr[2003] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ѭ���������������

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddChCycleChangeCount(int nAddCount /* = 1 */){
	m_deviceRegAr[2004] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������A�������

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddChSupplayBoxAChangeCount(int nAddCount /* = 1 */){
	m_deviceRegAr[2005] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������B�������

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddChSupplayBoxBChangeCount(int nAddCount /* = 1 */){
	m_deviceRegAr[2006] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}


//********************************** BHChange �豸�Ĵ���********************/

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ʹ�ô���

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddBhcResetCount(int nAddCount /* = 1 */){
	m_deviceRegAr[3001] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ��������A��������

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddBhcBoxAChangeCount(int nAddCount /* = 1 */){
	m_deviceRegAr[3002] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ��������B�߳�����

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddBhcBoxBChangeCount(int nAddCount /* = 1 */){
	m_deviceRegAr[3003] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

// ***********************************BH �豸�Ĵ���*************************/
//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ʹ�ô���

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddBhResetCount(int nAddCount /* = 1 */){
	m_deviceRegAr[4001] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ��ʶ�����

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddBhRecongnitionCount(int nAddCount /* = 1 */){
	m_deviceRegAr[4002] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Ѻ�����ѹ�����

@param      int nAddCount;�����ۼӺͣ�Ĭ��Ϊ1��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::AddBhMotorPressCount(int nAddCount /* = 1 */){
	m_deviceRegAr[4003] += nAddCount;
	SaveACCARToFile(m_deviceRegAr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��յ��ռ���������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceRegAR::ClearDeviceAR(){
	Initialize();
	SaveACCARToFile(m_deviceRegAr);
}