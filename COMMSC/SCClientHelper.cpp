#include "stdafx.h"
#include "SCClientHelper.h"
#include "scgenericack.h"
#include "scgenericnack.h"
#include "SCCommand.h"
#include "validation.h"
#include "scapi.h"
#include "scconnectrequest.h"
#include "scconnectrequestreply.h"
#include "scdatamsg.h"
#include "scenquirelink.h"
#include "scenquirelinkreply.h"
#include "scexception.h"
#include "SCClient.h"
#include "DebugLog.h"
#include "scdatamsgreply.h"

#define theSC_Client_Trace CFileLog::GetInstance(FILTER_SC_CLIENT_LOG)

#define __FILENAME__ _T("")

CMap<UINT,UINT,CSCClientHelper*,CSCClientHelper*> CSCClientHelper::s_CallbackObjects;

//////////////////////////////////////////////////////////////////////////
/*
@brief    ���캯��  

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSCClientHelper::CSCClientHelper()
{
	iSendTimeOutTimer=0;
	iReceiveTimeOutTimer=0;
	iEnquireLinkTimer=0;

	iSendTimeOutInterval=30;
	iReceiveTimeOutInterval=30;
	iEnquireLinkInterval=30;

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSCClientHelper::~CSCClientHelper()
{
	StopTimer();
}

/////////////////////////////////////////////////////////////////////////
/**
@brief   ��������
@param   header     ����ͷ��
@param   lpInData   ԭʼ����
@param   nInLen     ԭʼ���ݳ���
@param   lpOutData  ƴװ������
@param   nOutLen    ƴװ�����ݳ���
@retval  0
*/
//////////////////////////////////////////////////////////////////////////
long CSCClientHelper::MakePDU( CMD_HEADER& header, LPBYTE lpInData, UINT nInLen, LPBYTE lpOutData, UINT &nOutLen)
{
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	UINT length = nInLen +LEN_START_TAG_CMD_HEADER_END_TAG;
	if(header.bCrcType == 0x01)
	{
		length +=LEN_CRC;//4���ֽڵ�CRC
	}
	LPBYTE buffer = new BYTE[length];
	LPBYTE pBufferStart = buffer;
	memset(buffer, 0, length);
	// ��ʼ���
	*buffer = START_TAG;
	buffer++;
	header.Serialize(buffer);
	buffer+=sizeof(header);

	//��Ϣ����
	if(NULL != lpInData)
		memcpy(buffer, lpInData, nInLen);
	buffer += nInLen;

	// CRCУ��
	if(header.bCrcType == 0x01)
	{
		unsigned long ncrc = 0;
		ncrc = Crc32(lpInData, nInLen, ncrc);
		memcpy(buffer, &ncrc, LEN_CRC);
		buffer += LEN_CRC;
	}
	// �������
	*buffer = END_TAG;

	nOutLen = length;
	memcpy(lpOutData, pBufferStart, nOutLen); 
	if (NULL != pBufferStart)
	{
		delete[] pBufferStart;
		pBufferStart = NULL;
	}
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return 0;

}

///////////////////////////////////////////////////////////////////////////
///**
//@brief   ��Ϣ���ݷְ�����
//@param   header     ����ͷ��
//@param   lpInData   �ֱ�����
//@param   nInLen     �ְ����ݳ���
//@param   carryBlock ���ݰ�
//@retval  0
//*/
////////////////////////////////////////////////////////////////////////////
long CSCClientHelper::SplitIntoPackages(CMD_HEADER& header, LPVARIABLE_DATA body, CARRAY_PACKAGE_DATA &carryPackage)
{
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	UINT bufferLength = body->nLen +LEN_START_TAG_CMD_HEADER_END_TAG;
	if(header.bCrcType == 0x01)
	{
		bufferLength +=LEN_CRC;
	}
	LPBYTE buffer = new BYTE[bufferLength];
	// ����������ƴ�ӳ�һ����Ϣ
	long lRet = MakePDU(header, body->lpData, body->nLen, buffer, bufferLength);


	BYTE lpMsgHeader[LEN_START_TAG_CMD_HEADER]={0};
	memcpy(lpMsgHeader, buffer, LEN_START_TAG_CMD_HEADER);
	// �ְ�����
	int nPackageCount = 1;
	if (body->nLen > MAX_TCP_TRANS_SIZE)
	{
		nPackageCount = body->nLen / MAX_TCP_TRANS_SIZE + 1;
	}
	// ������
	
	if (nPackageCount == 1)
	{
		LPPACKAGE_DATA package = new PACKAGE_DATA;
		package->nLen = bufferLength;
		memcpy(package->bData, buffer, package->nLen);
		memcpy(package->bData+OFFSET_PACKAGE_LENGTH, &(package->nLen), 4);
		// �ְ���
		package->bData[OFFSET_PACKAGE_COUNT] = nPackageCount;
		// ����
		package->bData[OFFSET_PACKAGE_INDEX] = 1;

		carryPackage.SetAt(1,package);
	}else{
		for (int i = 0; i<nPackageCount; i++)
		{
			LPPACKAGE_DATA package = new PACKAGE_DATA;
			memset(package->bData, 0, MAX_TCP_TRANS_SIZE);
			if (i == nPackageCount -1)    // ���һ���ְ�
			{
				package->nLen = bufferLength - MAX_TCP_TRANS_SIZE*i;   // ��CRC32У��
				memcpy(package->bData, lpMsgHeader, LEN_START_TAG_CMD_HEADER);
				memcpy(package->bData+OFFSET_PACKAGE_LENGTH, &(package->nLen), 4);
				memcpy(package->bData+LEN_START_TAG_CMD_HEADER, buffer + i*MAX_TCP_TRANS_SIZE+LEN_START_TAG_CMD_HEADER, package->nLen-LEN_START_TAG_CMD_HEADER);    // ��CRC32У��
			}
			else                      // �ְ�֮һ
			{
				package->nLen = MAX_TCP_TRANS_SIZE + 40;   // ��CRC32У��
				memcpy(package->bData, lpMsgHeader, LEN_START_TAG_CMD_HEADER);
				memcpy(package->bData+OFFSET_PACKAGE_LENGTH, &(package->nLen), 4);
				memcpy(package->bData+LEN_START_TAG_CMD_HEADER,buffer + i*MAX_TCP_TRANS_SIZE+LEN_START_TAG_CMD_HEADER, MAX_TCP_TRANS_SIZE); // ��CRC32У��
				package->bData[package->nLen - LEN_END_TAG] = END_TAG;
				package->bData[OFFSET_CRC_FLAG] = 0x00;  // �����һ����У����Ϊ0x00
			}
			// �ְ�����
			package->bData[OFFSET_PACKAGE_COUNT] = nPackageCount;
			// ����
			package->bData[OFFSET_PACKAGE_INDEX] = i+1;
			carryPackage.SetAt(i+1,package);
		}
	}
	if (NULL != buffer)
	{
		delete[] buffer;
		buffer = NULL;
	}
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����λ����ְ�������

@param      CSCCommand* command ��λ����
@param	CARRAY_PACKAGE_DATA &carryBlock���ṹ

@retval long 0�ɹ�    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CSCClientHelper::MakePackages(CSCCommand* command,CARRAY_PACKAGE_DATA &carryBlock)
{
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	VARIABLE_DATA body;
	command->SerializeBody(body);
	long result = SplitIntoPackages(command->GetHeader(),&body,carryBlock);
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return result;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������λ���͵��ͻ��˵�һ����

@param    VARIABLE_DATA &buffer ��

@retval     bool �������ĵ����а��Ƿ��յ�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CSCClientHelper::SaveOnePackage(VARIABLE_DATA &buffer)
{
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	UINT nblockCount = *(buffer.lpData+OFFSET_PACKAGE_COUNT);
	UINT nblockNo = *(buffer.lpData+OFFSET_PACKAGE_INDEX);
	LPPACKAGE_DATA package = new PACKAGE_DATA;
	memcpy(package->bData,buffer.lpData,buffer.nLen);
	package->nLen = buffer.nLen;

	LPPACKAGE_DATA tmpPackageData = NULL;
	// ������ڣ�ɾ��
	if (currentReceiveMessage.Lookup(nblockNo, tmpPackageData))
	{
		delete tmpPackageData;
		currentReceiveMessage.RemoveKey(nblockNo);
	}
	// ���뵽MAP��
	currentReceiveMessage.SetAt(nblockNo, package);
	if(currentReceiveMessage.GetSize() >= nblockCount)
	{
		theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
		return true;
	}
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     �жϰ������Ƿ���ȷ

@param     (in) VARIABLE_DATA& package ��
@param	(out)SC_NACK_CODE& nackCode ����ԭ��

@retval    bool �Ƿ���ȷ 

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CSCClientHelper::IsPackageOK(VARIABLE_DATA& package,SC_NACK_CODE& nackCode )
{
	if(package.nLen<LEN_PACKAGE_LEAST)
	{
		nackCode = MSG_LEN_INVALIDE;
		return false;
	}
	LPBYTE lpPackageData = package.lpData;
	if(*lpPackageData!=START_TAG)
	{
		nackCode = PROTOCAL_INVALID;
		return false;
	}
	if(*(lpPackageData+package.nLen-LEN_END_TAG) != END_TAG)
	{
		nackCode = PROTOCAL_INVALID;
		return false;
	}
	lpPackageData++;
	CMD_HEADER header;
	header.Deserialize(lpPackageData);
	CMD_HEADER defaultHeader;
	if(header.lProtoclIdentify != defaultHeader.lProtoclIdentify)
	{
		nackCode = PROTOCAL_INVALID;
		return false;
	}
	if(header.bProtoclVersion!=defaultHeader.bProtoclVersion)
	{
		nackCode = PROTOCAL_VER_INVALID;
		return false;
	}
	if(header.bDataFormatVer!=defaultHeader.bDataFormatVer)
	{
		nackCode = DATA_FORMATVER_INVALIDE;
		return false;
	}
	if(header.bCipherflag != 0x00 && header.bCipherflag!=0x01&& header.bCipherflag!=0x02)
	{
		nackCode = ENCRYPT_ALG_INVALIDE;
		return false;
	}
	if(header.bCipherflag== 0x00 && header.bCipherAddLen!=0x00)
	{
		nackCode = ENCRYPT_ALG_INVALIDE;
		return false;
	}
	if(header.bCipherflag!=0x00 && header.bCipherAddLen==0x00)
	{
		nackCode = ENCRYPT_ALG_INVALIDE;
		return false;
	}
	if(header.bBlockNo>header.bBlockCnt)
	{
		nackCode = BLOCK_DATA_INVALIDE;
		return false;
	}
	if(header.bBlockNo<header.bBlockCnt)
	{
		if(header.bCrcType!=0x00) //�������һ����������CRCУ��
		{
			nackCode = CRC_ALG_INVALID;
			return false;
		}
	}
	if(package.nLen!=header.lMsgLen)
	{
		nackCode = DATA_TYPE_INVALID;
		return false;
	}
	BYTE availableDataTransferType[] = {PROTOCAL_DATA,RESERVE_DATA,TRANSACTION_DATA,OPER_DATA,CONTROL_DATA,STATUS_DATA};
	if(!CValidation::IsInRange(header.bDataTransType,availableDataTransferType))
	{
		nackCode = DATA_TYPE_INVALID;
		return false;
	}
	BYTE availableTransferCommand[] = {CMD_CONNECT_REQUEST,CMD_CONNECT_REQUEST,CMD_CONNECT_REQUEST_REPLY
		,CMD_DATA_TRANSFER,CMD_DATA_TRANSFER_REPLY,CMD_ENQUIRE_LINK,CMD_ENQUIRE_LINK_REPLY
		,CMD_GENERIC_ACK,CMD_GENERIC_NACK};
	
	if(!CValidation::IsInRange(header.nwTransId.bCmdID,availableTransferCommand))
	{
		nackCode = PROTOCAL_CMD_INVALIDE;
		return false;
	}

	//if(header.nwTransId.lDeviceID.wAfcStationID!=theApp.m_StationID)
	//{
	//	nackCode = OTHER_ERR;
	//	return false;
	//}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  ����λһ�����ĵ����еİ�ƴ��һ��

@param   (out )VARIABLE_DATA& commandMsg��������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCClientHelper::ComposePackagesToCommandMsg(VARIABLE_DATA& commandMsg)
{
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	UINT packageCount = currentReceiveMessage.GetSize();
	UINT commandMsgLength = 0;
	bool needCRC = true;
	for (int i=0;i<packageCount;i++)
	{
		LPPACKAGE_DATA package = currentReceiveMessage[i+1];
		if(package!=NULL)
		{
			UINT contentLength = package->nLen - LEN_START_TAG_CMD_HEADER_END_TAG ;
			if(package->bData[OFFSET_CRC_FLAG]!=0x00) 
			{
				contentLength-=LEN_CRC;
			}
			commandMsgLength+=contentLength;
			if(packageCount == package->bData[OFFSET_PACKAGE_INDEX])//���һ��
			{
				needCRC = package->bData[OFFSET_CRC_FLAG] !=0x00;
			}
		}
	}
	if(needCRC)
	{
		commandMsgLength+=LEN_CRC;
	}
	commandMsgLength+=LEN_START_TAG_CMD_HEADER_END_TAG;
	delete [] commandMsg.lpData;
	commandMsg.nLen = 0;
	commandMsg.lpData = new BYTE[commandMsgLength];
	UINT commandLastContentIndex = LEN_START_TAG_CMD_HEADER;
	for (int i=0;i<packageCount;i++)
	{
		LPPACKAGE_DATA package = currentReceiveMessage[i+1];
		if(package!=NULL)
		{
			UINT packageContentIndex = LEN_START_TAG_CMD_HEADER;
			UINT packageContentLength = package->nLen - LEN_START_TAG_CMD_HEADER_END_TAG ;
			if(package->bData[OFFSET_CRC_FLAG]!=0x00) 
			{
				packageContentLength-=LEN_CRC;
			}
			memcpy(commandMsg.lpData+commandLastContentIndex,package->bData+packageContentIndex,packageContentLength);
			commandLastContentIndex+=packageContentLength;
		}
	}
	LPPACKAGE_DATA lastPackage = currentReceiveMessage[packageCount];
	if(lastPackage==NULL)
	{
		throw CSCException(CSCException::OTHER_ERR,__FILENAME__,__LINE__,_T("Package is wrong"));
	}
	if(needCRC)
	{
		memcpy(commandMsg.lpData + commandLastContentIndex,lastPackage->bData + lastPackage->nLen -LEN_CRC_END_TAG,LEN_CRC );
		commandLastContentIndex+=LEN_CRC;
	}
	memcpy(commandMsg.lpData+LEN_START_TAG,lastPackage->bData+LEN_START_TAG,LEN_CMD_HEADER);//ͷ
	*commandMsg.lpData = START_TAG;//��ʼ���
	*(commandMsg.lpData+commandMsgLength-LEN_END_TAG)=END_TAG;//�������
	*(commandMsg.lpData+OFFSET_CRC_FLAG) = *(lastPackage->bData+OFFSET_CRC_FLAG);//CRC
	*(commandMsg.lpData+LEN_START_TAG) = commandMsgLength;//����
	*(commandMsg.lpData+OFFSET_PACKAGE_COUNT) = 0x01;//�ְ�����
	*(commandMsg.lpData+OFFSET_PACKAGE_INDEX) = 0x01;//�ְ���
	commandMsg.nLen = commandMsgLength;

	ClearPackagesBuffer();
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �õ����Ƿ���ȷ

@param    (in) VARIABLE_DATA &commandMsg  ��������
@param	(out)SC_NACK_CODE& nackCode ����ԭ��

@retval     bool �Ƿ���ȷ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CSCClientHelper::IsCommandMsgOK(VARIABLE_DATA &commandMsg,SC_NACK_CODE& nackCode)
{
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	if(commandMsg.lpData[OFFSET_CRC_FLAG] == 0x00)
	{
		theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
		return true;
	}
	LPBYTE crc = commandMsg.lpData + commandMsg.nLen - LEN_CRC_END_TAG;
	ULONG oldCRC = MAKELONG(MAKEWORD(*crc,*(crc+1)),MAKEWORD(*(crc+2),*(crc+3)));
	ULONG newCRC = Crc32(commandMsg.lpData+LEN_START_TAG_CMD_HEADER,commandMsg.nLen - LEN_START_TAG_CMD_HEADER_END_TAG - LEN_CRC ,0);
	if(oldCRC != newCRC)
	{
		nackCode =CRC_DATA_INVALIE;
		theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
		return false;
	}
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return true;
}



//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������ڱ��ذ��ݴ�����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCClientHelper::ClearPackagesBuffer()
{
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T(" <-"));
	POSITION pos = currentReceiveMessage.GetStartPosition();
	unsigned char bBlockNo = 0;
	LPPACKAGE_DATA lpBlock = NULL;
	while (pos != NULL)
	{
		currentReceiveMessage.GetNextAssoc(pos, bBlockNo, lpBlock);
		delete lpBlock;
		currentReceiveMessage.RemoveKey(bBlockNo);
	}
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T(" ->"));
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      �жϴ����ǲ��ǹؼ��Դ���
@param  SC_NACK_CODE &nackCode �������
@retval bool  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCClientHelper::IsKeyError(SC_NACK_CODE &nackCode)
{
	bool isKeyError = false;
	switch(nackCode)
	{
	case PROTOCAL_INVALID:
	case PROTOCAL_VER_INVALID:
	case DATA_FORMATVER_INVALIDE:
	case CRC_ALG_INVALID:
	case ENCRYPT_ALG_INVALIDE:
		isKeyError = true;
		break;
	default:
		isKeyError = false;
	}
	return isKeyError;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ó�ʱ�ļ��

@param      UINT iEnquireLinkInterval �����������λ���룩
@param      UINT iSendTimeOutInterval ���ͳ�ʱ�������λ���룩
@param      UINT iReceiveTimeOutInterval ���ճ�ʱ�������λ���룩

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCClientHelper::SetTimerInterval(UINT iEnquireLinkInterval,UINT iSendTimeOutInterval,UINT iReceiveTimeOutInterval)
{
	this->iEnquireLinkInterval = iEnquireLinkInterval;
	this->iSendTimeOutInterval = iSendTimeOutInterval;
	this->iReceiveTimeOutInterval = iReceiveTimeOutInterval;
}


/////////////////////////////////////////////////////////////////////////
/**
@brief      �������ͳ�ʱ��ʱ��
@param  ��
@retval �� 
*/
//////////////////////////////////////////////////////////////////////////
void CSCClientHelper::StartSendTimeOutTimer()
{
	if(iSendTimeOutTimer!=0)
	{
		::KillTimer(NULL,iSendTimeOutTimer);
		iSendTimeOutTimer = 0;
	}
	iSendTimeOutTimer = ::SetTimer(NULL,NULL,iSendTimeOutInterval*1000,OnSendTimeout);
	s_CallbackObjects.SetAt(iSendTimeOutTimer,this);
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ���ͳ�ʱ��ʱ��
@param  ��
@retval �� 
*/
//////////////////////////////////////////////////////////////////////////
void CSCClientHelper::StopSendTimeOutTimer()
{
	if(iSendTimeOutTimer!=0){
		::KillTimer(NULL,iSendTimeOutTimer);
		iSendTimeOutTimer = 0;
	}
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      �������ճ�ʱ��ʱ��
@param  ��
@retval �� 
*/
//////////////////////////////////////////////////////////////////////////
void CSCClientHelper::StartReceiveTimeOutTimer()
{
	if(iReceiveTimeOutTimer!=0)
	{
		::KillTimer(NULL,iReceiveTimeOutTimer);
		iReceiveTimeOutTimer = 0;
	}
	iReceiveTimeOutTimer = ::SetTimer(NULL,NULL,iReceiveTimeOutInterval*1000,OnReceiveTimeout);
	s_CallbackObjects.SetAt(iReceiveTimeOutTimer,this);
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ���ճ�ʱ��ʱ��
@param  ��
@retval �� 
*/
//////////////////////////////////////////////////////////////////////////
void CSCClientHelper::StopReceiveTimeOutTimer()
{
	::KillTimer(NULL,iReceiveTimeOutTimer);
	iReceiveTimeOutTimer = 0;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      ����������ʱ
@param  ��
@retval �� 
*/
//////////////////////////////////////////////////////////////////////////
void CSCClientHelper::StartEnquireLinkTimer()
{
	if(iEnquireLinkTimer!=0)
	{
		::KillTimer(NULL,iEnquireLinkTimer);
		iEnquireLinkTimer = 0;
	}
	iEnquireLinkTimer = ::SetTimer(NULL,NULL,iEnquireLinkInterval*1000,OnEnquireLinkTimer);
	s_CallbackObjects.SetAt(iEnquireLinkTimer,this);
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ������ʱ
@param  ��
@retval �� 
*/
//////////////////////////////////////////////////////////////////////////
void CSCClientHelper::StopEnquireLinkTimer()
{
	::KillTimer(NULL,iEnquireLinkTimer);
	iEnquireLinkTimer = 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������ʱ��

@param      STARTTIMERREASON ������ʱ��ԭ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCClientHelper::StartTimer(STARTTIMERREASON reason)
{
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T(" <- reason: %d"),reason);
	if(theClient.m_LocalCommand != NULL && !theClient.m_LocalCommand->IsReceived())
	{
		StartSendTimeOutTimer();
		return;
	}
	if(reason == RECEIVE_PART||reason == RECEIVE_NACK)
	{
		StartReceiveTimeOutTimer();
		return;
	}
	if(reason ==RECEIVE_COMPLETE)
	{
		StartEnquireLinkTimer();
		return;
	}
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T(" ->"));

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֹͣ��ʱ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCClientHelper::StopTimer()
{
	StopSendTimeOutTimer();
	StopReceiveTimeOutTimer();
	StopEnquireLinkTimer();
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      �ְ����ݽ��ճ�ʱ����
@param  hWnd      handle of window for timer messages
@param  uMsg      WM_TIMER message
@param  idEvent   timer identifier
@param  dwTime    current system time	
@retval �� 
*/
//////////////////////////////////////////////////////////////////////////
void CALLBACK CSCClientHelper::OnReceiveTimeout( HWND hWnd,UINT uMsg, UINT idEvent,DWORD dwTime	)
{
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T(" <-"));
	CSCClientHelper* instance = NULL;
	bool success = s_CallbackObjects.Lookup(idEvent,instance);
	if(success)
	{
		s_CallbackObjects.RemoveKey(idEvent);
		instance->StopReceiveTimeOutTimer();
		instance->ClearPackagesBuffer();
		instance->ReceiveTimeouted.Invoke();
	}
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      �������ݺ�δ�յ����ݳ�ʱ
@param  hWnd      handle of window for timer messages
@param  uMsg      WM_TIMER message
@param  idEvent   timer identifier
@param  dwTime    current system time	
@retval �� 
*/
//////////////////////////////////////////////////////////////////////////
void CALLBACK CSCClientHelper::OnSendTimeout( HWND hWnd,UINT uMsg, UINT idEvent,DWORD dwTime	)
{
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T(" <-"));
	CSCClientHelper* instance = NULL;
	bool success = s_CallbackObjects.Lookup(idEvent,instance);
	if(success)
	{
		instance->StopSendTimeOutTimer();
		instance->SendTimeouted.Invoke();
	}
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T(" ->"));
}


/////////////////////////////////////////////////////////////////////////
/**
@brief      ��������ʱ
@param  hWnd      handle of window for timer messages
@param  uMsg      WM_TIMER message
@param  idEvent   timer identifier
@param  dwTime    current system time	
@retval �� 
*/
//////////////////////////////////////////////////////////////////////////
void CALLBACK CSCClientHelper::OnEnquireLinkTimer( HWND hWnd,UINT uMsg, UINT idEvent,DWORD dwTime	)
{
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T(" ->"));
	CSCClientHelper* instance = NULL;
	bool success = s_CallbackObjects.Lookup(idEvent,instance);
	if(success)
	{
		instance->StopEnquireLinkTimer();
		instance->EnquireLinked.Invoke();
	}
	
	theSC_Client_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T(" ->"));
}
