#include "StdAfx.h"
#include "CEctParamMaster.h"
#include "LogException.h"
#include "getini.h"
#include "ExceptionLog.h"
#include "cparamexception.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = _T(__FILE__);
#endif


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CEctParamMaster::CEctParamMaster(void):CParameterMaster()
{
	m_dataPath = "";
	m_backupPath = "";
	m_futVersion = 0;
	m_curVersion = 0;
	m_curFile = NULL;
	m_futFile = NULL;
	m_flag = true;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////// 
CEctParamMaster::~CEctParamMaster(void)
{
	try{
		if (NULL != m_curFile) {
			delete m_curFile;
			m_curFile = NULL;
		}
		if (NULL != m_futFile) {
			delete m_futFile;
			m_futFile = NULL;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief  初期化

@param  (i)CString fileName         读取的文件名
@param  (i)bool readflag            选择读取文件标志    true 当前版本文件   false 未来版本文件

@retval bool        true    成功    \n
false   失败

@exception CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CEctParamMaster::Internalize(bool readflag,CString dataPath,CString backupPath)
{
	m_dataPath = dataPath;
	m_backupPath = backupPath;
	m_curVersion = 0xFFFFFFFF;
	m_futVersion = 0xFFFFFFFF;
	m_packageHead.internalize();
	m_flag = readflag;
	GetParamFileName();	
	if ( m_curfileName == "" && m_futfileName == "" ){
		throw CParamException(CParamException::FILE_NOT_EXIST, _T(__FILE__), __LINE__);
	}
	SetCurVerType(m_curfileName);							// 设置当前参数版本类型
	SetFutVerType(m_futfileName);							// 设置未来参数版本类型

	try{
		return ReadFile();		
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}

}

/////////////////////////////////////////////////////////////////////////
/**
@brief  得到当前版本

@param  无

@retval DWORD version   黑名单版本 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
DWORD CEctParamMaster::GetCurMasterVer()
{
	return m_curVersion;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  得到future版本

@param  无

@retval DWORD version   future黑名单版本 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
DWORD CEctParamMaster::GetFutMasterVer()
{
	return m_futVersion;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  得到当前版本生效日期

@param  无

@retval _DATE 当前生效日期 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
_DATE CEctParamMaster::GetCurStartDate()
{
	return m_curStartDate;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  得到未来版本生效日期

@param  无

@retval _DATE 未来版本生效日期 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
_DATE CEctParamMaster::GetFutStartDate()
{
	return m_futStartDate;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  取得当前参数版本类型

@param  无

@retval PARAM_VERSION_TYPE_CODE 参数版本类型 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
PARAM_VERSION_TYPE_CODE CEctParamMaster::GetCurVerType()
{
	return m_curVersionType;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  取得未来参数版本类型

@param  无

@retval PARAM_VERSION_TYPE_CODE 参数版本类型 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
PARAM_VERSION_TYPE_CODE CEctParamMaster::GetFutVerType()
{
	return m_futVersionType;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  设置当前参数版本类型

@param  CString strFileName 参数文件名

@retval 无 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
void CEctParamMaster::SetCurVerType(CString strFileName)
{
	CString strTemp = strFileName.Right(2);
	if(strTemp == "00"){
		m_curVersionType = CUR_VER;
	}
	else if(strTemp == "01"){
		m_curVersionType = FUT_VER;
	}
	else if(strTemp == "10"){
		m_curVersionType = CUR_TEST;
	}
	else if(strTemp == "11"){
		m_curVersionType = FUT_TEST;
	}
	else{
		m_curVersionType = CUR_VER;
	}
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  设置未来参数版本类型

@param  CString strFileName 参数文件名

@retval 无 

@exception 
*/
//////////////////////////////////////////////////////////////////////////
void CEctParamMaster::SetFutVerType(CString strFileName)
{
	CString strTemp = strFileName.Right(2);
	if(strTemp == "00"){
		m_futVersionType = CUR_VER;
	}
	else if(strTemp == "01"){
		m_futVersionType = FUT_VER;
	}
	else if(strTemp == "10"){
		m_futVersionType = CUR_TEST;
	}
	else if(strTemp == "11"){
		m_futVersionType = FUT_TEST;
	}
	else{
		m_futVersionType = FUT_VER;
	}
}

/////////////////////////////////////////////////////////////////////////
/**
@brief  更新参数信息（暂不使用）

@param  (i)const char * proBlacklistInfo        要写入的数据
@param  (i)ULONG datalength                     要写入数据的长度
@param  (i)bool writeflag                       选择更新文件的标志 true 当前版本文件    false 未来版本文件

@retval bool  ret   true    成功
false   失败

@exception CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CEctParamMaster::UpdateMasterFile(const char *lpData, ULONG nLen, bool writeflag)
{  
	try {
		if(writeflag) {   
			if (m_curFile->WriteFile(lpData, nLen) != CXFile::FILE_OPERATION_OK) {
				throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
			}
			lpData += 8;
			m_curVersion = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
		}
		else {
			if (m_futFile->WriteFile(lpData, nLen) != CXFile::FILE_OPERATION_OK) {
				throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__);
			}
			lpData += 8;
			m_futVersion = ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3));
		}
		return true;
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取文件

@param      无

@retval     bool        \n
true:读取文件成功         false:读取文件失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CEctParamMaster::ReadFile()
{

	CString filePath = m_dataPath + PARAM_ECT_CURRENT_DIR + m_curfileName;
	CString backupPath = m_backupPath + PARAM_ECT_CURRENT_DIR + m_curfileName;
	try{
		if(m_curfileName != ""){
			// current文件
			//if (NULL == m_curFile) {
			m_curFile = new CXFile(filePath, backupPath);
			//}
			VARIABLE_DATA curbinInfo;
			// 读取文件信息到binInfo中
			if (m_curFile->ReadBinFile(curbinInfo) != CXFile::FILE_OPERATION_OK){
				throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
			}
			// 检查参数正确性
			if (CheckDataInfo(curbinInfo)) {            
				LPBYTE lpData = curbinInfo.lpData;
				// 跳过信息头
				lpData += 8;
				// 当前版本号
				m_curVersion = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData +=11;  

				// 当前版本生效日期
				m_curStartDate =  BCD2Date((char*)lpData);
				lpData+=4;

				if(m_flag) {
					// 解析包头
					ParseHead(curbinInfo.lpData);
					if(m_packageHead.sectionCount > 0){
						// 解析分段信息
						ParseSectionInfo(curbinInfo.lpData+PACKAGE_HEAD_LENS);					
						// 解析参数体
						ParseBody(curbinInfo.lpData);
					}
				}
			}
			else{  
				theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("参数有效性检查失败：") + filePath);
				return false;
			}
		}

		if(m_futfileName != ""){
			// future文件
			filePath = m_dataPath + PARAM_ECT_FUTURE_DIR + m_futfileName;
			backupPath = m_backupPath + PARAM_ECT_FUTURE_DIR + m_futfileName;   
			if (NULL == m_futFile) {
				m_futFile = new CXFile(filePath, backupPath);
			}
			VARIABLE_DATA futbinInfo;
			// 读取文件信息到binInfo中
			if (m_futFile->ReadBinFile(futbinInfo) != CXFile::FILE_OPERATION_OK){
				throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
			}
			// 检查参数正确性
			if (CheckDataInfo(futbinInfo)) {     
				// 读取文件正常，解析信息
				LPBYTE lpData = futbinInfo.lpData;
				// 跳过信息头
				lpData += 8;
				// version
				m_futVersion = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData +=11;  
				// 未来版本生效日期
				m_futStartDate =  BCD2Date((char*)lpData);
				lpData+=4;

				if(!m_flag) {
					// 解析包头
					ParseHead(futbinInfo.lpData);
					if(m_packageHead.sectionCount > 0){
						// 解析分段信息
						ParseSectionInfo(futbinInfo.lpData+PACKAGE_HEAD_LENS);					
						// 解析参数体
						ParseBody(futbinInfo.lpData);
					}
				}               
			}
			else{           
				theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("参数有效性检查失败：") + backupPath);
				return false;
			}
		}
		return true;		
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出当前文件内容（暂不使用）

@param      (o)  LPBYTE& lpData       文件内容
(i)  bool readFlag        true:读取当前版本     false:读取未来版本文件
@retval     bool \n
true    读取成功           false   读取失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CEctParamMaster::ReadMasterData(VARIABLE_DATA &binfileData, bool readFlag)
{
	try{
		if (readFlag) {
			// 读取两重化文件失败
			if (m_curFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
				throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
			}
		}
		else{               
			// 读取文件失败
			if (m_futFile->ReadBinFile(binfileData) != CXFile::FILE_OPERATION_OK) {
				throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
			}
		}

		// 读取成功
		return true;
	}
	catch(CSysException&) {
		throw ;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出当前文件的包头部分

@param      (i)LPBYTE lpData       文件内容

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CEctParamMaster::ParseHead(LPBYTE lpData)
{
	try{
		// 包格式版本
		m_packageHead.formatVersion = *lpData;
		lpData++;
		// 数据来源方
		m_packageHead.dataOrigin = *lpData;
		lpData++;
		// 数据包长度
		m_packageHead.packageLength = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		lpData += 4;
		// 数据类型代码
		m_packageHead.dataTypeCode = BCD2int((char*)lpData, 2);
		lpData += 2;
		// 版本号
		m_packageHead.versionCode = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		lpData += 4;
		// 生成时间
		m_packageHead.createDateTime = BCD2Time((char*)lpData);
		lpData += 7; 
		// 生效日期
		m_packageHead.startDate = BCD2Date((char*)lpData);;
		lpData += 4;
		// 数据分段总数
		m_packageHead.sectionCount = mstos(ComMakeWORD(*lpData,*(lpData + 1)));
		lpData+=2;
		// 预留 3 Byte
		memcpy(m_packageHead.reverse,lpData,3);
		lpData += 3;        
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,m_curfileName);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查电文内容正确性

@param      (i)const VARIABLE_DATA&      电文信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CEctParamMaster::CheckDataInfo(const VARIABLE_DATA& binfileData)
{
	// 检查电文长度
	if (binfileData.nLen < PACKAGE_HEAD_LENS) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT参数长度异常"));
		return false;
	}
	LPBYTE lpData = binfileData.lpData;

	// 包格式版本号
	lpData ++;
	// 数据来源方
	lpData ++;
	// 数据包长度
	DWORD packageLength = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
	if (packageLength != binfileData.nLen){
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT参数长度异常"));
		return false;
	}
	lpData += 4;
	// 数据类型代码
	lpData += 2;
	// 版本号
	lpData += 4;
	// 生成时间
	if (!ComCheckIsBCD((char*)lpData,7)) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT参数生成时间非BCD格式"));
		return false;
	}
	_DATE_TIME startDateTime;
	startDateTime = BCD2Time((char*)lpData);
	if (0 != ComCheckDateTimeValid(startDateTime)) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT参数生成时间不合法"));
		return false;
	}
	lpData+=7;
	// 生效日期
	if (!ComCheckIsBCD((char*)lpData,4)) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT参数生效日期非BCD格式"));
		return false;
	}
	_DATE createDate;
	createDate =  BCD2Date((char*)lpData);
	lpData+=4;
	if (0 != ComCheckDateValid(createDate)) {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT参数生效日期不合法"));
		return false;
	}
	// 数据分段总数
	DWORD sectionCount = mstos(ComMakeWORD(*lpData,*(lpData + 1)));
	lpData += 2;
	// 预留 3 Byte
	lpData += 3;
	if (sectionCount != 0){				
		// 分段结构体
		lpData += (8*sectionCount);		
		// 参数体    
		if (!CheckBodyInfo(lpData)) {
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT参数体数据BCD码错误"));
			return false;
		}		
	}
	else{
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ECT参数分段总数为0"));
		return false;
	}
	return true;  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取参数文件名

@param      (i)CString sPrefix      文件名前缀

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CEctParamMaster::GetParamFileNameSub(CString sPrefix)
{
	try{
		//取BOM数据目录
		TCHAR szRunPath[MAX_PATH];
		ComGetAppPath(szRunPath, MAX_PATH);
		CString szBOMINIFName = CString(szRunPath) + BOM_INI_FILE_NAME;
		CGetIni bomIni(szBOMINIFName);  
		m_dataPath = bomIni.GetDataString(BOM_INI_SECTION_APPLICATION, BOM_INI_KEY_DATAPATH);
		m_backupPath = bomIni.GetDataString(BOM_INI_SECTION_APPLICATION, BOM_INI_KEY_DATAPATH_BKP);
		//m_dataPath = theBOM_INFO.GetDataPath();		
		//m_backupPath = theBOM_INFO.GetBackupPath();

		m_curfileName = _T("");
		m_futfileName = _T("");

		CFileFind finder;
		CString sPath = _T("");
		CString sMainFile("");
		CString sBackupFile("");
		// 当前参数
		{
			// 主目录查找
			sPath = m_dataPath + PARAM_ECT_CURRENT_DIR;
			BOOL bContinue = finder.FindFile(sPath + sPrefix + _T("*.*"));
			while (bContinue) {
				bContinue = finder.FindNextFile();
				if (finder.IsDots()||finder.IsDirectory()) {
					continue;
				}
				else {   
					// 取出文件名
					m_curfileName = finder.GetFileName(); 
					// 二重化参数
					// 备份文件没有
					sBackupFile = m_backupPath + PARAM_ECT_CURRENT_DIR + m_curfileName;
					if(ComCheckFileExist(sBackupFile) == FALSE){
						sMainFile = m_dataPath + PARAM_ECT_CURRENT_DIR + m_curfileName;
						CopyFile(sMainFile,sBackupFile,FALSE);
					}
				}
			}
			// 备份目录查找
			if(m_curfileName == ""){
				sPath = m_backupPath + PARAM_ECT_CURRENT_DIR;
				BOOL bContinue = finder.FindFile(sPath + sPrefix + _T("*.*"));
				while (bContinue) {
					bContinue = finder.FindNextFile();
					if (finder.IsDots()||finder.IsDirectory()) {
						continue;
					}
					else {   
						// 取出文件名
						m_curfileName = finder.GetFileName(); 						
					}
				}	
				// 二重化参数
				// 主文件没有 && 备份文件有
				if(m_curfileName != ""){
					sMainFile = m_dataPath + PARAM_ECT_CURRENT_DIR + m_curfileName;
					sBackupFile = m_backupPath + PARAM_ECT_CURRENT_DIR + m_curfileName;
					CopyFile(sBackupFile,sMainFile,FALSE);
				}
			}
			finder.Close();
		}

		// 取得未来参数文件名
		{
			sPath = m_dataPath + PARAM_ECT_FUTURE_DIR;		
			BOOL bContinue = finder.FindFile(sPath + sPrefix + _T("*.*"));
			while (bContinue) {
				bContinue = finder.FindNextFile();
				if (finder.IsDots()||finder.IsDirectory()) {
					continue;
				}
				else {   
					// 取出文件名，用于比较日期
					m_futfileName = finder.GetFileName(); 		

					// 二重化参数
					// 备份文件没有
					sBackupFile = m_backupPath + PARAM_ECT_FUTURE_DIR + m_futfileName;
					if(ComCheckFileExist(sBackupFile) == FALSE){
						sMainFile = m_dataPath + PARAM_ECT_FUTURE_DIR + m_futfileName;
						CopyFile(sMainFile,sBackupFile,FALSE);
					}
				}
			}
			// 备份目录查找
			if(m_futfileName == ""){
				sPath = m_backupPath + PARAM_ECT_FUTURE_DIR;
				BOOL bContinue = finder.FindFile(sPath + sPrefix + _T("*.*"));
				while (bContinue) {
					bContinue = finder.FindNextFile();
					if (finder.IsDots()||finder.IsDirectory()) {
						continue;
					}
					else {   
						// 取出文件名
						m_futfileName = finder.GetFileName(); 						
					}
				}	
				// 二重化参数
				// 主文件没有 && 备份文件有
				if(m_futfileName != ""){
					sMainFile = m_dataPath + PARAM_ECT_FUTURE_DIR + m_futfileName;
					sBackupFile = m_backupPath + PARAM_ECT_FUTURE_DIR + m_futfileName;
					CopyFile(sBackupFile,sMainFile,FALSE);
				}
			}
			finder.Close();
		}
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}