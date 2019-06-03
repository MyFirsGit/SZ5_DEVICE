#include "stdafx.h"
#include "UPSCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSerialPort CUPSCommand::m_com;

//////////////////////////////////////////////////////////////////////////
/**
@brief      CUPSCommand构造函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUPSCommand::CUPSCommand()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CUPSCommand析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUPSCommand::~CUPSCommand()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打开UPS串口

@param      (i)int mPort    端口号
@param      (i)int mBaud    波特率

@retval     bool    成功:true; 失败:false

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CUPSCommand::Open(int mPort, int mBaud)
{
	return m_com.Open(mPort,mBaud);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      关闭UPS串口

@param      无

@retval     bool    成功:true; 失败:false

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CUPSCommand::Close()
{
	return m_com.Close();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CUPSCommand::ExecuteCommand()
{
	return 0;//RW_RSP_OK;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     long     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CUPSCommand::IsValidCommand()
{
	return 0;//SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取UPS信息

@param      (i)int info    UPS信息

@retval     int    成功:0; 失败:

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CUPSCommand::GetUPSBatteryInfo(_tagUPSBatteryInfo &info)
{
	int nRet = -1;
	const int cnInfoLen = 110;
	const char *pchCmd = "Q6\r";
	//theUPS_LOG.WriteLogFile("", pchCmd, strlen(pchCmd), MODE_SEND);
	nRet = m_com.Write(pchCmd, strlen(pchCmd));
	if (nRet == 0){
		char chReturn[200];
		memset(chReturn, 0, sizeof(chReturn));
		Sleep(600);
		nRet = m_com.Read(chReturn,cnInfoLen);
		//theUPS_LOG.WriteLogFile("",chReturn,cnInfoLen, MODE_RECEIVE);
		if (nRet == 0){
			nRet = ParseBatteryInfo(chReturn, sizeof(chReturn), info);
		}
		else{
			nRet = UPS_ERR_READ_COM;
		}
	}
	else{
		nRet = UPS_ERR_WRITE_COM;
	}
	return nRet;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      解析UPS信息

@param      (i)char pInfo    读取的数据
@param      (i)int nInfoLen    读取数据的长度
@param      (i)int info    UPS信息

@retval     int    成功:0; 失败:

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CUPSCommand::ParseBatteryInfo(const char *pInfo, int nInfoLen, _tagUPSBatteryInfo &info)
{
	if (pInfo == NULL || nInfoLen <= 0){
		return UPS_ERR_OTHER;
	}
	int nRet = UPS_SUCCESSED;

	//一共110个bytes
	const char *pTmp = pInfo;
	if (*pTmp++ == '(')
	{
		info.dInputVoltageR = GetFloatData(pTmp, 5); //MMM.M
		pTmp += 6;
		info.dInputVoltageS = GetFloatData(pTmp, 5); //MMM.M
		pTmp += 6;
		info.dInputVoltageT = GetFloatData(pTmp, 5); //MMM.M
		pTmp += 6;
		info.dInputFrequency = GetFloatData(pTmp, 4); //NN.N
		pTmp += 5;

		info.dOutputVoltageR = GetFloatData(pTmp, 5); //PPP.P
		pTmp += 6;
		info.dOutputVoltageS = GetFloatData(pTmp, 5); //PPP.P
		pTmp += 6;
		info.dOutputVoltageT = GetFloatData(pTmp, 5); //PPP.P
		pTmp += 6;
		info.dOutputFrequency = GetFloatData(pTmp, 4); //RR.R
		pTmp += 5;

		info.dOutputLoadR = double(GetIntData(pTmp, 3))/100; //QQQ
		pTmp += 4;
		info.dOutputLoadS = double(GetIntData(pTmp, 3))/100; //QQQ
		pTmp += 4;
		info.dOutputLoadT = double(GetIntData(pTmp, 3))/100; //QQQ
		pTmp += 4;

		info.dPositiveVoltage = GetFloatData(pTmp, 5); //SSS.S
		pTmp += 6;
		info.dNegativeVoltage = GetFloatData(pTmp, 5); //VVV.V 
		pTmp += 6;

		info.dTemperature = GetFloatData(pTmp, 4); //TT.T
		pTmp += 5;

		info.dwLeftStandbyTime = GetIntData(pTmp, 5); //ttttt
		pTmp += 6;

		info.dBatteryCapacity = (DWORD)GetIntData(pTmp, 3); //CCC
		pTmp += 4;

		info.bySysModeK = *pTmp++; //K
		info.byBatteryTestStatus = *pTmp; //B
		pTmp += 2;

		memcpy(info.byErrorCode, pTmp, 8); //ffffffff
		pTmp += 9;

		memcpy(info.byWarningCode, pTmp, 8); //wwwwwwww
		pTmp += 9;

		info.bInputTransformerY = (*pTmp++ == 1) ? true : false; //Y
		info.bOutputVType = (*pTmp++ == 1) ? true : false; //O

		if (!IsEndFlag(pTmp)) //<cr>
			nRet = UPS_ERR_END_FLAG;
	}
	else{
		nRet = UPS_ERR_HEADER_FLAG;
	}

	return nRet;
}

bool CUPSCommand::IsStartFlag(const char *pInfo)
{
	return (*pInfo == '(');
}

bool CUPSCommand::IsEndFlag(const char *pInfo)
{
	//const char *pcnEndFlag = "<cr>";
	//return (pInfo[0] == '<' && pInfo[1] == 'c' && pInfo[2] == 'r' && pInfo[3] == '>');

	return (pInfo[0] == '\r') ? true : false;
} 

double CUPSCommand::GetFloatData(const char*pInfo, int nLen)
{
	char szTmp[6];
	memset(szTmp, 0, sizeof(szTmp));
	strncpy(szTmp, pInfo, nLen);
	return atof(szTmp);
}

int CUPSCommand::GetIntData(const char*pInfo, int nLen)
{
	char szTmp[6];
	memset(szTmp, 0, sizeof(szTmp));
	strncpy(szTmp, pInfo, nLen);

	return atoi(szTmp);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    　构造函数

@param      (i)int nPort 串口的端口号
@param      (i)int nBaud 串口的波特率

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

CUPSOpen::CUPSOpen(int nPort,int nBaud)
{
	this->m_nBaud = nBaud;
	this->m_nPort = nPort;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      (i)无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUPSOpen::~CUPSOpen(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      none

@retval      long    \n
0:成功   非0 : 失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
long CUPSOpen::ExecuteCommand(void)
{
	long lCompleteCode = Open( m_nPort,m_nBaud) ? 0 : 1;
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)无

@retval      无

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CUPSClose::CUPSClose(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      (i)无

@retval      无

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CUPSClose::~CUPSClose(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      (i)无

@retval      long   \n 
0 : 成功   非0 : 失败 

@exception   
*/
//////////////////////////////////////////////////////////////////////////
long CUPSClose::ExecuteCommand()
{
	long  lCompleteCode = Close();
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    　构造函数

@param      (i)int nPort 串口的端口号
@param      (i)int nBaud 串口的波特率
@param      (i)int nSize 字长

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUPSRead::CUPSRead()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      (i)无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUPSRead::~CUPSRead(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      (i)无

@retval      long    \n
0:成功   非0 : 失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
long CUPSRead::ExecuteCommand(void)
{
	long  lCompleteCode = GetUPSBatteryInfo(m_sUPSInfo);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      (i)无

@retval      _tagUPSBatteryInfo    UPS信息

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CUPSRead::GetUPSInfo(_tagUPSBatteryInfo &info)
{
	info = m_sUPSInfo;
}