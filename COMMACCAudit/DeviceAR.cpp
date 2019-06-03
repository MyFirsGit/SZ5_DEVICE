#include "StdAfx.h"
#include "DeviceAR.h"
#include "Sync.h"
#include "Util.h"
#include "CTLOGCountInfo.h"
#include "CACCAuditException.h"
#include "MaintenanceInfo.h"
#include "SysInfo.h"



DeviceAR DeviceAR::theInstance;

DeviceAR::DeviceAR(void)
{
	currentFileName = m_dataPath + CString(AR_MANAGE_FOLDER) + CString(AR_MANAGE_FILE_NAME);	
	BackFileName = m_BackupPath + CString(AR_MANAGE_FOLDER) + CString(AR_MANAGE_FILE_NAME);
	sequenceFileName = m_dataPath + CString(AR_MANAGE_FOLDER) + CString(AR_MANAGE_SERIAL_NAME);
 	TicketCountInBox1 = 0;
 	TicketCountInBox2 = 0;
}

DeviceAR::~DeviceAR(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief         初始化ACC日计文件

@param      (i)CString dataPath          文件存放路径
@param      (i)CString backupPath     备份文件路径

@retval        无
*/
//////////////////////////////////////////////////////////////////////////
long DeviceAR::InitARFile(CString dataPath,CString backupPath)
{
	m_dataPath = dataPath;
	m_BackupPath = backupPath;
	// 创建AR审计目录
	ComCheckAndCreateDirectory(m_dataPath + AR_MANAGE_FOLDER);
	ComCheckAndCreateDirectory(m_BackupPath + AR_MANAGE_FOLDER);

	// 搜索文件
	// 主数据目录文件存在
	if (ComCheckFileExist(currentFileName)) {		
		if (!ComCheckFileExist(BackFileName)) { 
			if (::CopyFile(currentFileName, BackFileName, FALSE) == FALSE) { 
				throw CARException(CARException::FILE_RESCUE_FAIL, _T(__FILE__), __LINE__, _T("源文件：") + currentFileName + _T(" 目标文件：") + BackFileName); 
			} 
		} 
	}
	// 主数据目录文件不存在	
	else {
		// 备份数据目录文件存在
		if (ComCheckFileExist(BackFileName)) {
			// 恢复文件			
			if (::CopyFile(BackFileName, currentFileName, FALSE) == FALSE) { 
				throw CARException(CARException::FILE_RECOVER_OVERFAIL, _T(__FILE__), __LINE__, _T("源文件：") + BackFileName + _T(" 目标文件：") + currentFileName); 
			}
			
		} 
		// 备份数据目录文件不存在
		else {
			// 新建AR审计文件
			CFile ARFile;
			CFile ARBackFile;
			CFileException cfeError;
			if(!ARFile.Open(currentFileName,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary)) { 
				TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
				CString strException; 
				cfeError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
				strException = _T("异常："); 
				strException += strCause; 
				throw CARException(CARException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
			} 
			if(!ARBackFile.Open(BackFileName,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary)) { 
				TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
				CString strException; 
				cfeError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
				strException = _T("异常："); 
				strException += strCause; 
				throw CARException(CARException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
			} 
			BYTE lpData[AR_MANAGE_ORIGINAL_FILE_LEN];
			memset(lpData,0x00,AR_MANAGE_ORIGINAL_FILE_LEN);
			ARFile.Write(lpData,AR_MANAGE_ORIGINAL_FILE_LEN);
			ARBackFile.Write(lpData,AR_MANAGE_ORIGINAL_FILE_LEN);
			ARFile.Close();
			ARBackFile.Close();
			
		}
		
	}
	int rest;
	//获取开始交易流水号	
	if (ComCheckFileExist(sequenceFileName)) {
		CFile sequenceFile;
		CFileException sequenceError; 
		if(!sequenceFile.Open(sequenceFileName,CFile::modeReadWrite|CFile::typeBinary , &sequenceError)) { 
			TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
			CString strException; 
			sequenceError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException = _T("异常："); 
			strException += strCause; 
			throw CARException(CARException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
		} 
		sequenceFile.Seek(sizeof(long)*2,CFile::begin);
	
		sequenceFile.Read(&rest,sizeof(DEVICE_AR_REASON));
		sequenceFile.Close();
	}
	else
	{
		//保存开始交易流水号	
		BYTE UDSNStart[4];
		BYTE YKTSNStart[4];
		long m_ulUDSN = theCOUNT_INFO.GetUDSNForDisplay();
		ComRevLong(m_ulUDSN,UDSNStart);
		long m_ulEKTSN = theCOUNT_INFO.GetECTNumForDsp();
		ComRevLong(m_ulUDSN,UDSNStart);
		ComRevLong(m_ulEKTSN,YKTSNStart);
		CFile sequenceFile;
		CFileException sequenceError; 
		if(!sequenceFile.Open(sequenceFileName, CFile::modeCreate| CFile::modeWrite| CFile::typeBinary , &sequenceError)) { 
			TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
			CString strException; 
			sequenceError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException = _T("异常："); 
			strException += strCause; 
			throw CARException(CARException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
		} 
		sequenceFile.Write(UDSNStart,sizeof(UDSNStart));
		sequenceFile.Write(YKTSNStart,sizeof(YKTSNStart));
		int flag = AR_BUSINESS_DAY_START;
		sequenceFile.Write(&flag,sizeof(DEVICE_AR_REASON));
		sequenceFile.Close();
		rest = AR_BUSINESS_DAY_END;
		
	}
	// 文件属性改为可写
	SetFileAttributes(sequenceFileName, FILE_ATTRIBUTE_NORMAL);
	SetFileAttributes(currentFileName, FILE_ATTRIBUTE_NORMAL);
	SetFileAttributes(BackFileName, FILE_ATTRIBUTE_NORMAL);

	return rest;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      查找所需更新数据的位置

@param      (i)long RegistId                寄存器ID
@param      (o)long& theUpdataPosition		 更新数据的位置

@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceAR::FindPos(long RegistId,long& theUpdataPosition)
{
			switch(RegistId){
						case	SELLCOUNT:   	//累积发售张数	
							theUpdataPosition = 1*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	SELLAMOUNT:  	//累积发售金额（分）
							theUpdataPosition = 2*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	REFOUNDCOUNT:	//累积退票次数
							theUpdataPosition = 3*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	REFOUNDAMOUNT:	//累积退票金额（分）
							theUpdataPosition = 4*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	UPDATACOUNT : 	//累积更新次数
							theUpdataPosition = 5*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	UPDATAAMOUNT :	//累积更新金额（分）
							theUpdataPosition = 6*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	CHARGECOUNT : 	//累积充值次数
							theUpdataPosition = 7*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	CHARGEAMOUNT:  	//累积充值金额（分）
							theUpdataPosition = 8*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	LUCKCOUNT:   	//累积锁卡次数
							theUpdataPosition = 9*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						default:
							throw CARException(CARException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__, _T("参数错误" + currentFileName)); 
			}			

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      生成新票种AR数据信息

@param      (i)long ticketType              车票类型
@param      (i)long theUpdataPosition       待更新数据的偏移位置
@param      (i)long updatedata				数据
LPBYTE      (o)addData                      车票类型信息体 

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceAR::MakedataByTicketType(long ticketType,long theUpdataPosition,long updataData,LPBYTE addData)
{
	LPBYTE lpData = addData;

	BYTE type[4];
	ComRevLong(ticketType,lpData);

	lpData += 4;
	for(int i=4; i<AR_MANAGE_REVOLVE_BODY_LEN;i+=4)
	{
		if (theUpdataPosition == i)
		{
			ComRevLong(updataData,lpData);
			lpData += 4;
			continue;
		}
		memset(lpData,0,AR_MANAGE_UPDATE_MESSAGE_LEN);	
		lpData += 4;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     增加分票型自动检票机处理统计

@param      long ticketType              车票类型
@param      long RegistId                寄存器ID
@param      long updatedata				 数据增量

@retval     bool                         TRUE：添加成功          FALSE：失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool DeviceAR::AddProcessStatByType(long ticketType,long RegistId,long updatedata)                                        
{
	if(RegistId <= 0 || updatedata < 0){
		throw CARException(CARException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);
	}

	CFile cfFile; 
	CFileException cfeError;
	BYTE* bFileData = NULL;
	BYTE* bFileDataPtr = NULL;
	unsigned int uiFileLength = 0;

	bool retVal = false;
	bool IsTicketTypeExist = false;
	long theTicketType;
	long revolveTimes;             // 循环次数 
	long theUpdataPosition = 0;        // 待更新数据循环体内的位置

	try{
		//打开文件
		if(!cfFile.Open(currentFileName, CFile::modeReadWrite | CFile::typeBinary, &cfeError)) { 
			TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
			CString strException; 
			cfeError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException = _T("异常："); 
			strException += strCause; 
			throw CARException(CARException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
		} 
	
		//读取数据
		cfFile.SeekToBegin(); 
		uiFileLength = (unsigned int)cfFile.GetLength(); 
		bFileDataPtr = new BYTE[uiFileLength]; 
		bFileData = bFileDataPtr;
		if (bFileData == NULL) { 
			cfFile.Close(); 
			throw CARException(CARException::MALLOC_FAIL, _T(__FILE__), __LINE__, _T("分配内存失败")); 
		} 
		memset(bFileData, 0x00, uiFileLength); 
		if (cfFile.Read(bFileData, uiFileLength) != uiFileLength) { 
			cfFile.Close(); 
			delete[] bFileData; 
			bFileData = NULL; 
			throw CARException(CARException::FILE_READ_FAIL, _T(__FILE__), __LINE__, _T("文件读取异常：" + currentFileName)); 
		} 
		
		// 循环次数
		revolveTimes = ComMakeLong(bFileData[AR_MANAGE_REVOLVE_TIMES_POS],bFileData[AR_MANAGE_REVOLVE_TIMES_POS+1],
									bFileData[AR_MANAGE_REVOLVE_TIMES_POS+2],bFileData[AR_MANAGE_REVOLVE_TIMES_POS+3]);	

		// 查找所需更新数据的位置
		int cycTimes = 0;
		for (int i=0; i<revolveTimes; i++){	
			theTicketType = ComMakeLong(bFileData[AR_MANAGE_TICKET_POSITION_POS+i*AR_MANAGE_REVOLVE_BODY_LEN],bFileData[AR_MANAGE_TICKET_POSITION_POS+i*AR_MANAGE_REVOLVE_BODY_LEN+1],
										bFileData[AR_MANAGE_TICKET_POSITION_POS+i*AR_MANAGE_REVOLVE_BODY_LEN+2],bFileData[AR_MANAGE_TICKET_POSITION_POS+i*AR_MANAGE_REVOLVE_BODY_LEN+3]);	
			if (theTicketType == ticketType){
				cycTimes = i;
				IsTicketTypeExist = true;				
				break;
			}
		}
		FindPos(RegistId,theUpdataPosition);
		// 车票种类不存在
		if (IsTicketTypeExist == false){
			// 循环次数加一
			revolveTimes++;
			BYTE revolveTime[4];
			ComRevLong(revolveTimes,revolveTime);
			bFileData[AR_MANAGE_REVOLVE_TIMES_POS] = revolveTime[0];
			bFileData[AR_MANAGE_REVOLVE_TIMES_POS+1] = revolveTime[1];
			bFileData[AR_MANAGE_REVOLVE_TIMES_POS+2] = revolveTime[2];
			bFileData[AR_MANAGE_REVOLVE_TIMES_POS+3] = revolveTime[3];	
			cfFile.SeekToBegin();
			cfFile.Write(bFileData,uiFileLength);	

			//添加新的车票类型信息
			BYTE addData[AR_MANAGE_REVOLVE_BODY_LEN]; 
			MakedataByTicketType(ticketType,theUpdataPosition,updatedata,addData);
			cfFile.SeekToEnd();
			cfFile.Write(addData,AR_MANAGE_REVOLVE_BODY_LEN);
			retVal = true;
		}
		// 存在则写文件
		else{			
			BYTE pUpdataMessage[AR_MANAGE_UPDATE_MESSAGE_LEN];
			long theUpdataMessage = ComMakeLong(bFileData[AR_MANAGE_UPDATE_MESSAGE_POS+cycTimes*AR_MANAGE_REVOLVE_BODY_LEN+theUpdataPosition],
												bFileData[AR_MANAGE_UPDATE_MESSAGE_POS+cycTimes*AR_MANAGE_REVOLVE_BODY_LEN+theUpdataPosition+1],
												bFileData[AR_MANAGE_UPDATE_MESSAGE_POS+cycTimes*AR_MANAGE_REVOLVE_BODY_LEN+theUpdataPosition+2],
												bFileData[AR_MANAGE_UPDATE_MESSAGE_POS+cycTimes*AR_MANAGE_REVOLVE_BODY_LEN+theUpdataPosition+3]);
			theUpdataMessage +=  updatedata;
			ComRevLong(theUpdataMessage,pUpdataMessage);
			bFileData[AR_MANAGE_UPDATE_MESSAGE_POS+cycTimes*AR_MANAGE_REVOLVE_BODY_LEN+theUpdataPosition] = pUpdataMessage[0];
			bFileData[AR_MANAGE_UPDATE_MESSAGE_POS+cycTimes*AR_MANAGE_REVOLVE_BODY_LEN+theUpdataPosition+1] = pUpdataMessage[1];
			bFileData[AR_MANAGE_UPDATE_MESSAGE_POS+cycTimes*AR_MANAGE_REVOLVE_BODY_LEN+theUpdataPosition+2] = pUpdataMessage[2];
			bFileData[AR_MANAGE_UPDATE_MESSAGE_POS+cycTimes*AR_MANAGE_REVOLVE_BODY_LEN+theUpdataPosition+3] = pUpdataMessage[3];
			cfFile.SeekToBegin();
			cfFile.Write(bFileData,uiFileLength);
			retVal = true;
		}
		cfFile.Flush();
		cfFile.Close();
		delete [] bFileData; 
		bFileData = NULL;

		// 二重化
		if (::CopyFile(currentFileName, BackFileName, FALSE) == FALSE) { 
			throw CARException(CARException::FILE_RESCUE_FAIL, _T(__FILE__), __LINE__, _T("源文件：") + currentFileName + _T(" 目标文件：") + BackFileName); 
		} 
		return retVal;
	} 
	catch(CSysException&)
	{
		cfFile.Close();
		if (bFileData != NULL) { 
			delete [] bFileData; 
			bFileData = NULL; 
		} 
		throw; 
	}
	catch(...)
	{
		cfFile.Close();
		if (bFileData != NULL) { 
			delete [] bFileData; 
			bFileData = NULL; 
		} 
		throw CARException(CARException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__); 
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     获取审计数据报文头

@param     (o)LPBYTE ARCommHead     审计数据报文头
@param     (i)int ARFileTag         读取模式标志   1:当前运营日结束  2:当前运营日开始

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceAR::MakePackHead(LPBYTE ARCommHead,int ARFileTag){

	LPBYTE lpData = ARCommHead;
	BYTE UDSNStart[4];
	BYTE UDSNEnd[4];
	BYTE YKTSNStart[4];
	BYTE YKTSNEnd[4];

	//AR文件类型
	ComRevLong((long)ARFileTag,lpData);
	lpData+=4;

	//交易日期时间 
	long curTime = ComGetCurTimeT();
	ComRevLong(curTime,lpData);
	lpData += 4;

	// 设备ID
	*lpData = theMAINTENANCE_INFO.GetLineCode();
	lpData ++;
	*lpData = theMAINTENANCE_INFO.GetStationCode();
	lpData ++;
	*lpData = theMAINTENANCE_INFO.GetDeviceType();
	lpData ++;
	*lpData = theMAINTENANCE_INFO.GetMachineCode();
	lpData ++;

	//当前运营日开始
	if(ARFileTag == AR_BUSINESS_DAY_START){                   
		// 一票通开始序列号
		long m_ulUDSN = theCOUNT_INFO.GetUDSNForDisplay();
		ComRevLong(m_ulUDSN,lpData);
		ComRevLong(m_ulUDSN,UDSNStart);
		lpData+=4;
		// 一票通结束序列号
		ComRevLong(m_ulUDSN,lpData);		
		lpData+=4;

		// 一卡通开始序列号
		long m_ulEKTSN = theCOUNT_INFO.GetECTNumForDsp();
		ComRevLong(m_ulEKTSN,lpData);
		ComRevLong(m_ulEKTSN,YKTSNStart);
		lpData+=4;
		// 一卡通结束序列号
		ComRevLong(m_ulEKTSN,lpData);
		lpData+=4;
		
		//保存开始交易流水号						
		CFile sequenceFile;
		CFileException sequenceError; 
		if(!sequenceFile.Open(sequenceFileName, CFile::modeCreate| CFile::modeWrite| CFile::typeBinary , &sequenceError)) { 
			TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
			CString strException; 
			sequenceError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException = _T("异常："); 
			strException += strCause; 
			throw CARException(CARException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
		} 
		sequenceFile.Write(UDSNStart,sizeof(UDSNStart));
		sequenceFile.Write(YKTSNStart,sizeof(YKTSNStart));
		int flag = AR_BUSINESS_DAY_START;
		sequenceFile.Write(&flag,sizeof(DEVICE_AR_REASON));
		sequenceFile.Close();

	}
	//当前运营日结束
	else if(ARFileTag == AR_BUSINESS_DAY_END){				
		long m_ulUDSN = theCOUNT_INFO.GetUDSNForDisplay();			
		ComRevLong(m_ulUDSN,UDSNEnd);
		long m_ulEKTSN = theCOUNT_INFO.GetECTNumForDsp();			
		ComRevLong(m_ulEKTSN,YKTSNEnd);

		//获取开始交易流水号	
		CFile sequenceFile;
		CFileException sequenceError; 
		if(!sequenceFile.Open(sequenceFileName,CFile::modeReadWrite|CFile::typeBinary , &sequenceError)) { 
			TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
			CString strException; 
			sequenceError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException = _T("异常："); 
			strException += strCause; 
			throw CARException(CARException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
		} 
		sequenceFile.SeekToBegin();
		sequenceFile.Read(UDSNStart,sizeof(UDSNStart));
		sequenceFile.Read(YKTSNStart,sizeof(YKTSNStart));
		int flag = AR_BUSINESS_DAY_END;
		sequenceFile.Write(&flag,sizeof(DEVICE_AR_REASON));
		sequenceFile.Close();

		memcpy(lpData,UDSNStart,4);
		lpData += 4;
		memcpy(lpData,UDSNEnd,4);
		lpData += 4;
		memcpy(lpData,YKTSNStart,4);
		lpData += 4;
		memcpy(lpData,YKTSNEnd,4);
		lpData += 4;

	}else{
		throw  CARException(CARException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__,_T("写文件异常：") + currentFileName);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     读取寄存器审计文件数据

@param     (i)WORD ARFileTag         读取模式标志   1:当前运营日结束  2:当前运营日开始
@param     (o)VARIABLE_DATA& arMesg          审计数据

@retval    bool                   TRUE：读取成功             FALSE：读取失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool DeviceAR::ReadProcessStat(DEVICE_AR_REASON ARFileTag,VARIABLE_DATA& arMesg)    
{
	try
	{
		CFile cfFile; 
		CFileException cfeError; 
		if(!cfFile.Open(currentFileName, CFile::modeReadWrite | CFile::typeBinary, &cfeError)) { 
			TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
			CString strException; 
			cfeError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException = _T("异常："); 
			strException += strCause; 
			throw CARException(CARException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
		} 

		BYTE * bFileData = NULL;
		cfFile.SeekToBegin(); 
		int uiFileLength = (unsigned int)cfFile.GetLength(); 			

		bFileData = new BYTE[ AR_MANAGE_TAG_LEN+AR_MANAGE_COMM_HEAD_LEN + AR_MANAGE_DEVICE_TYPE_LEN + uiFileLength ]; 
		if (bFileData == NULL) { 
			cfFile.Close(); 
			throw CARException(CARException::MALLOC_FAIL, _T(__FILE__), __LINE__, _T("分配内存失败")); 
		} 
		memset(bFileData, 0x00,  AR_MANAGE_TAG_LEN+AR_MANAGE_COMM_HEAD_LEN + AR_MANAGE_DEVICE_TYPE_LEN + uiFileLength); 

		LPBYTE lpData = bFileData;
		MakePackHead(lpData,ARFileTag);
		lpData +=  (AR_MANAGE_TAG_LEN+AR_MANAGE_COMM_HEAD_LEN);

        // 设置设备类型
		ComRevLong((long)theMAINTENANCE_INFO.GetDeviceType(),lpData);
		lpData += 4;

		cfFile.SeekToBegin();
		if (cfFile.Read(lpData, uiFileLength) != uiFileLength) { 
			cfFile.Close(); 
			delete [] bFileData; 
			bFileData = NULL; 
			throw CARException(CARException::FILE_READ_FAIL, _T(__FILE__), __LINE__, _T("文件读取异常：") + currentFileName); 
		}
		else{
			cfFile.Close();			
		}

		arMesg.lpData = NULL;
		arMesg.nLen = 0;
		arMesg.lpData = bFileData;
		arMesg.nLen =  AR_MANAGE_TAG_LEN+AR_MANAGE_COMM_HEAD_LEN + AR_MANAGE_DEVICE_TYPE_LEN + uiFileLength;
		bFileData = NULL;
	}
	catch (...)
	{
		throw CARException(CARException::FILE_READ_FAIL, _T(__FILE__), __LINE__, _T("文件读取异常") + currentFileName); 
	}


}

//////////////////////////////////////////////////////////////////////////
/**
@brief    更新文件数据

@param     
@param     

@retval    

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void DeviceAR::UpdateSerialFile()
{
	//获取开始交易流水号	
	CFile sequenceFile;
	CFileException sequenceError; 
	if(!sequenceFile.Open(sequenceFileName,CFile::modeReadWrite|CFile::typeBinary , &sequenceError)) { 
		TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
		CString strException; 
		sequenceError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
		strException = _T("异常："); 
		strException += strCause; 
		throw CARException(CARException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
	}
	BYTE UDSNStart[4];
	sequenceFile.Seek(sizeof(UDSNStart)*2,CFile::begin);
	int flag = AR_BUSINESS_DAY_SHUTDOWN;
	sequenceFile.Write(&flag,sizeof(DEVICE_AR_REASON));
	sequenceFile.Close();
}