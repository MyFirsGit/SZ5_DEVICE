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
@brief         ��ʼ��ACC�ռ��ļ�

@param      (i)CString dataPath          �ļ����·��
@param      (i)CString backupPath     �����ļ�·��

@retval        ��
*/
//////////////////////////////////////////////////////////////////////////
long DeviceAR::InitARFile(CString dataPath,CString backupPath)
{
	m_dataPath = dataPath;
	m_BackupPath = backupPath;
	// ����AR���Ŀ¼
	ComCheckAndCreateDirectory(m_dataPath + AR_MANAGE_FOLDER);
	ComCheckAndCreateDirectory(m_BackupPath + AR_MANAGE_FOLDER);

	// �����ļ�
	// ������Ŀ¼�ļ�����
	if (ComCheckFileExist(currentFileName)) {		
		if (!ComCheckFileExist(BackFileName)) { 
			if (::CopyFile(currentFileName, BackFileName, FALSE) == FALSE) { 
				throw CARException(CARException::FILE_RESCUE_FAIL, _T(__FILE__), __LINE__, _T("Դ�ļ���") + currentFileName + _T(" Ŀ���ļ���") + BackFileName); 
			} 
		} 
	}
	// ������Ŀ¼�ļ�������	
	else {
		// ��������Ŀ¼�ļ�����
		if (ComCheckFileExist(BackFileName)) {
			// �ָ��ļ�			
			if (::CopyFile(BackFileName, currentFileName, FALSE) == FALSE) { 
				throw CARException(CARException::FILE_RECOVER_OVERFAIL, _T(__FILE__), __LINE__, _T("Դ�ļ���") + BackFileName + _T(" Ŀ���ļ���") + currentFileName); 
			}
			
		} 
		// ��������Ŀ¼�ļ�������
		else {
			// �½�AR����ļ�
			CFile ARFile;
			CFile ARBackFile;
			CFileException cfeError;
			if(!ARFile.Open(currentFileName,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary)) { 
				TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
				CString strException; 
				cfeError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
				strException = _T("�쳣��"); 
				strException += strCause; 
				throw CARException(CARException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
			} 
			if(!ARBackFile.Open(BackFileName,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary)) { 
				TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
				CString strException; 
				cfeError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
				strException = _T("�쳣��"); 
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
	//��ȡ��ʼ������ˮ��	
	if (ComCheckFileExist(sequenceFileName)) {
		CFile sequenceFile;
		CFileException sequenceError; 
		if(!sequenceFile.Open(sequenceFileName,CFile::modeReadWrite|CFile::typeBinary , &sequenceError)) { 
			TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
			CString strException; 
			sequenceError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException = _T("�쳣��"); 
			strException += strCause; 
			throw CARException(CARException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
		} 
		sequenceFile.Seek(sizeof(long)*2,CFile::begin);
	
		sequenceFile.Read(&rest,sizeof(DEVICE_AR_REASON));
		sequenceFile.Close();
	}
	else
	{
		//���濪ʼ������ˮ��	
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
			strException = _T("�쳣��"); 
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
	// �ļ����Ը�Ϊ��д
	SetFileAttributes(sequenceFileName, FILE_ATTRIBUTE_NORMAL);
	SetFileAttributes(currentFileName, FILE_ATTRIBUTE_NORMAL);
	SetFileAttributes(BackFileName, FILE_ATTRIBUTE_NORMAL);

	return rest;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������������ݵ�λ��

@param      (i)long RegistId                �Ĵ���ID
@param      (o)long& theUpdataPosition		 �������ݵ�λ��

@retval     ��
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceAR::FindPos(long RegistId,long& theUpdataPosition)
{
			switch(RegistId){
						case	SELLCOUNT:   	//�ۻ���������	
							theUpdataPosition = 1*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	SELLAMOUNT:  	//�ۻ����۽��֣�
							theUpdataPosition = 2*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	REFOUNDCOUNT:	//�ۻ���Ʊ����
							theUpdataPosition = 3*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	REFOUNDAMOUNT:	//�ۻ���Ʊ���֣�
							theUpdataPosition = 4*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	UPDATACOUNT : 	//�ۻ����´���
							theUpdataPosition = 5*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	UPDATAAMOUNT :	//�ۻ����½��֣�
							theUpdataPosition = 6*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	CHARGECOUNT : 	//�ۻ���ֵ����
							theUpdataPosition = 7*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	CHARGEAMOUNT:  	//�ۻ���ֵ���֣�
							theUpdataPosition = 8*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						case	LUCKCOUNT:   	//�ۻ���������
							theUpdataPosition = 9*AR_MANAGE_UPDATE_MESSAGE_LEN;
							break;
						default:
							throw CARException(CARException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__, _T("��������" + currentFileName)); 
			}			

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ʊ��AR������Ϣ

@param      (i)long ticketType              ��Ʊ����
@param      (i)long theUpdataPosition       ���������ݵ�ƫ��λ��
@param      (i)long updatedata				����
LPBYTE      (o)addData                      ��Ʊ������Ϣ�� 

@retval     ��

@exception  ��
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
@brief     ���ӷ�Ʊ���Զ���Ʊ������ͳ��

@param      long ticketType              ��Ʊ����
@param      long RegistId                �Ĵ���ID
@param      long updatedata				 ��������

@retval     bool                         TRUE����ӳɹ�          FALSE��ʧ��

@exception  ��
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
	long revolveTimes;             // ѭ������ 
	long theUpdataPosition = 0;        // ����������ѭ�����ڵ�λ��

	try{
		//���ļ�
		if(!cfFile.Open(currentFileName, CFile::modeReadWrite | CFile::typeBinary, &cfeError)) { 
			TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
			CString strException; 
			cfeError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException = _T("�쳣��"); 
			strException += strCause; 
			throw CARException(CARException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
		} 
	
		//��ȡ����
		cfFile.SeekToBegin(); 
		uiFileLength = (unsigned int)cfFile.GetLength(); 
		bFileDataPtr = new BYTE[uiFileLength]; 
		bFileData = bFileDataPtr;
		if (bFileData == NULL) { 
			cfFile.Close(); 
			throw CARException(CARException::MALLOC_FAIL, _T(__FILE__), __LINE__, _T("�����ڴ�ʧ��")); 
		} 
		memset(bFileData, 0x00, uiFileLength); 
		if (cfFile.Read(bFileData, uiFileLength) != uiFileLength) { 
			cfFile.Close(); 
			delete[] bFileData; 
			bFileData = NULL; 
			throw CARException(CARException::FILE_READ_FAIL, _T(__FILE__), __LINE__, _T("�ļ���ȡ�쳣��" + currentFileName)); 
		} 
		
		// ѭ������
		revolveTimes = ComMakeLong(bFileData[AR_MANAGE_REVOLVE_TIMES_POS],bFileData[AR_MANAGE_REVOLVE_TIMES_POS+1],
									bFileData[AR_MANAGE_REVOLVE_TIMES_POS+2],bFileData[AR_MANAGE_REVOLVE_TIMES_POS+3]);	

		// ��������������ݵ�λ��
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
		// ��Ʊ���಻����
		if (IsTicketTypeExist == false){
			// ѭ��������һ
			revolveTimes++;
			BYTE revolveTime[4];
			ComRevLong(revolveTimes,revolveTime);
			bFileData[AR_MANAGE_REVOLVE_TIMES_POS] = revolveTime[0];
			bFileData[AR_MANAGE_REVOLVE_TIMES_POS+1] = revolveTime[1];
			bFileData[AR_MANAGE_REVOLVE_TIMES_POS+2] = revolveTime[2];
			bFileData[AR_MANAGE_REVOLVE_TIMES_POS+3] = revolveTime[3];	
			cfFile.SeekToBegin();
			cfFile.Write(bFileData,uiFileLength);	

			//����µĳ�Ʊ������Ϣ
			BYTE addData[AR_MANAGE_REVOLVE_BODY_LEN]; 
			MakedataByTicketType(ticketType,theUpdataPosition,updatedata,addData);
			cfFile.SeekToEnd();
			cfFile.Write(addData,AR_MANAGE_REVOLVE_BODY_LEN);
			retVal = true;
		}
		// ������д�ļ�
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

		// ���ػ�
		if (::CopyFile(currentFileName, BackFileName, FALSE) == FALSE) { 
			throw CARException(CARException::FILE_RESCUE_FAIL, _T(__FILE__), __LINE__, _T("Դ�ļ���") + currentFileName + _T(" Ŀ���ļ���") + BackFileName); 
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
@brief     ��ȡ������ݱ���ͷ

@param     (o)LPBYTE ARCommHead     ������ݱ���ͷ
@param     (i)int ARFileTag         ��ȡģʽ��־   1:��ǰ��Ӫ�ս���  2:��ǰ��Ӫ�տ�ʼ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceAR::MakePackHead(LPBYTE ARCommHead,int ARFileTag){

	LPBYTE lpData = ARCommHead;
	BYTE UDSNStart[4];
	BYTE UDSNEnd[4];
	BYTE YKTSNStart[4];
	BYTE YKTSNEnd[4];

	//AR�ļ�����
	ComRevLong((long)ARFileTag,lpData);
	lpData+=4;

	//��������ʱ�� 
	long curTime = ComGetCurTimeT();
	ComRevLong(curTime,lpData);
	lpData += 4;

	// �豸ID
	*lpData = theMAINTENANCE_INFO.GetLineCode();
	lpData ++;
	*lpData = theMAINTENANCE_INFO.GetStationCode();
	lpData ++;
	*lpData = theMAINTENANCE_INFO.GetDeviceType();
	lpData ++;
	*lpData = theMAINTENANCE_INFO.GetMachineCode();
	lpData ++;

	//��ǰ��Ӫ�տ�ʼ
	if(ARFileTag == AR_BUSINESS_DAY_START){                   
		// һƱͨ��ʼ���к�
		long m_ulUDSN = theCOUNT_INFO.GetUDSNForDisplay();
		ComRevLong(m_ulUDSN,lpData);
		ComRevLong(m_ulUDSN,UDSNStart);
		lpData+=4;
		// һƱͨ�������к�
		ComRevLong(m_ulUDSN,lpData);		
		lpData+=4;

		// һ��ͨ��ʼ���к�
		long m_ulEKTSN = theCOUNT_INFO.GetECTNumForDsp();
		ComRevLong(m_ulEKTSN,lpData);
		ComRevLong(m_ulEKTSN,YKTSNStart);
		lpData+=4;
		// һ��ͨ�������к�
		ComRevLong(m_ulEKTSN,lpData);
		lpData+=4;
		
		//���濪ʼ������ˮ��						
		CFile sequenceFile;
		CFileException sequenceError; 
		if(!sequenceFile.Open(sequenceFileName, CFile::modeCreate| CFile::modeWrite| CFile::typeBinary , &sequenceError)) { 
			TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
			CString strException; 
			sequenceError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException = _T("�쳣��"); 
			strException += strCause; 
			throw CARException(CARException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
		} 
		sequenceFile.Write(UDSNStart,sizeof(UDSNStart));
		sequenceFile.Write(YKTSNStart,sizeof(YKTSNStart));
		int flag = AR_BUSINESS_DAY_START;
		sequenceFile.Write(&flag,sizeof(DEVICE_AR_REASON));
		sequenceFile.Close();

	}
	//��ǰ��Ӫ�ս���
	else if(ARFileTag == AR_BUSINESS_DAY_END){				
		long m_ulUDSN = theCOUNT_INFO.GetUDSNForDisplay();			
		ComRevLong(m_ulUDSN,UDSNEnd);
		long m_ulEKTSN = theCOUNT_INFO.GetECTNumForDsp();			
		ComRevLong(m_ulEKTSN,YKTSNEnd);

		//��ȡ��ʼ������ˮ��	
		CFile sequenceFile;
		CFileException sequenceError; 
		if(!sequenceFile.Open(sequenceFileName,CFile::modeReadWrite|CFile::typeBinary , &sequenceError)) { 
			TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
			CString strException; 
			sequenceError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException = _T("�쳣��"); 
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
		throw  CARException(CARException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__,_T("д�ļ��쳣��") + currentFileName);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��ȡ�Ĵ�������ļ�����

@param     (i)WORD ARFileTag         ��ȡģʽ��־   1:��ǰ��Ӫ�ս���  2:��ǰ��Ӫ�տ�ʼ
@param     (o)VARIABLE_DATA& arMesg          �������

@retval    bool                   TRUE����ȡ�ɹ�             FALSE����ȡʧ��

@exception  ��
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
			strException = _T("�쳣��"); 
			strException += strCause; 
			throw CARException(CARException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
		} 

		BYTE * bFileData = NULL;
		cfFile.SeekToBegin(); 
		int uiFileLength = (unsigned int)cfFile.GetLength(); 			

		bFileData = new BYTE[ AR_MANAGE_TAG_LEN+AR_MANAGE_COMM_HEAD_LEN + AR_MANAGE_DEVICE_TYPE_LEN + uiFileLength ]; 
		if (bFileData == NULL) { 
			cfFile.Close(); 
			throw CARException(CARException::MALLOC_FAIL, _T(__FILE__), __LINE__, _T("�����ڴ�ʧ��")); 
		} 
		memset(bFileData, 0x00,  AR_MANAGE_TAG_LEN+AR_MANAGE_COMM_HEAD_LEN + AR_MANAGE_DEVICE_TYPE_LEN + uiFileLength); 

		LPBYTE lpData = bFileData;
		MakePackHead(lpData,ARFileTag);
		lpData +=  (AR_MANAGE_TAG_LEN+AR_MANAGE_COMM_HEAD_LEN);

        // �����豸����
		ComRevLong((long)theMAINTENANCE_INFO.GetDeviceType(),lpData);
		lpData += 4;

		cfFile.SeekToBegin();
		if (cfFile.Read(lpData, uiFileLength) != uiFileLength) { 
			cfFile.Close(); 
			delete [] bFileData; 
			bFileData = NULL; 
			throw CARException(CARException::FILE_READ_FAIL, _T(__FILE__), __LINE__, _T("�ļ���ȡ�쳣��") + currentFileName); 
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
		throw CARException(CARException::FILE_READ_FAIL, _T(__FILE__), __LINE__, _T("�ļ���ȡ�쳣") + currentFileName); 
	}


}

//////////////////////////////////////////////////////////////////////////
/**
@brief    �����ļ�����

@param     
@param     

@retval    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void DeviceAR::UpdateSerialFile()
{
	//��ȡ��ʼ������ˮ��	
	CFile sequenceFile;
	CFileException sequenceError; 
	if(!sequenceFile.Open(sequenceFileName,CFile::modeReadWrite|CFile::typeBinary , &sequenceError)) { 
		TCHAR   strCause[AR_ERROR_MSG_MAX_LEN]; 
		CString strException; 
		sequenceError.GetErrorMessage(strCause, sizeof(strCause), NULL); 
		strException = _T("�쳣��"); 
		strException += strCause; 
		throw CARException(CARException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
	}
	BYTE UDSNStart[4];
	sequenceFile.Seek(sizeof(UDSNStart)*2,CFile::begin);
	int flag = AR_BUSINESS_DAY_SHUTDOWN;
	sequenceFile.Write(&flag,sizeof(DEVICE_AR_REASON));
	sequenceFile.Close();
}