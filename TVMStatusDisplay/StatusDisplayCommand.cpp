#include "stdafx.h"
#include "StatusDisplayCommand.h"
#include "LEDApi.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// LED显示屏物理上20个英文字符宽度，但实际一行只能显示19个英文字符，最右边的一个无法显示出来。
#define SCREEN_LINE_WIDTH	20	

#define def_REDCLR		("(R)")
#define def_GREENCLR	("(G)")
#define def_YELLOWCLR	("(Y)")



CStatusDisplayCommand::CStatusDisplayCommand(void)
{

}

CStatusDisplayCommand::~CStatusDisplayCommand(void)
{

}

// 设置工作模式
BOOL CStatusDisplayCommand::SetWorkMode(BYTE nWM)
{ 
	int i = 0;
	BYTE TxData[255] = {0};
	TxData[0] = 0xaa;
	TxData[1] = 0x07;
	TxData[2] = 0x00;
	TxData[3] = 0x01;
	TxData[4] = 0x99;
	TxData[5] = 0x99;
	TxData[6] = 0x01;
	TxData[7] = nWM;

	BYTE adjust=TxData[1];
	for(int i=2;i<8;i++)
	{
		adjust=adjust^TxData[i];
	}
	TxData[8]=adjust;

	m_serialPort.WriteToPort(TxData,9);
	return TRUE; 
}

BOOL CStatusDisplayCommand::WriteXMode(BYTE nItem, BYTE* BCT, BYTE len)
{
	int i = 0;
	BYTE TxData[255] = {0};
	TxData[0] = 0xaa;
	TxData[1] = 0x25;
	TxData[2] = 0x00;
	TxData[3] = 0x01;
	TxData[4] = 0x99;
	TxData[5] = 0x99;
	TxData[6] = len+1;
	TxData[7] = nItem;

	BYTE adjust=TxData[1];
	for(i=2;i<8;i++){
		adjust=adjust^TxData[i];
	}

	for(i=8;i<len+8;i++){
		TxData[i] = *BCT;
		adjust=adjust^TxData[i];
		BCT++;
	}

	TxData[i]=adjust;

	m_serialPort.WriteToPort(TxData,len+9);
	return TRUE; 
}


long CStatusDisplayCommand::Open(int nPort, int nBardRet)
{
	//
	//if(m_serialPort.InitPort(nPort,38400,'N',8,1,EV_RXCHAR,30000)){
	//	m_serialPort.StartMonitoring();
	//	return 0; 
	//}

	return LED_CommOpen(nPort,nBardRet);
}

long CStatusDisplayCommand::Close()
{
	//m_serialPort.ClosePort();
	return LED_CommClose();
}

long CStatusDisplayCommand::Init(char** strExtenModeTextAray, int nCount)
{
	//WORD nShowPos = 0;
	//WORD nShowColor = 0;
	//WORD nEnterType = 0;
	//WORD nEnterSpeed = 0;
	//WORD nStayType = 0;
	//WORD nExistType = 0;
	//WORD nExistSpeed = 0;

	//int nItem = 0;
	//BYTE mbInfo[64] = {0};
	//for (int index=0;index<nCount;index++){
	//	// 位置
	//	nShowPos = index;
	//	//// 由于硬件内部有问题，多个模式写入时，扩展模式1和6会内容错乱，因此这不使用这两个模式，依次向后使用。
	//	//if(index==0){
	//	//	nShowPos = 13;
	//	//}
	//	//else if(index==5){
	//	//	nShowPos = 13+1;
	//	//}
	//	nItem= (BYTE)(nShowPos&0x00ff);
	//	CStringA strTmp = strExtenModeTextAray[index];
	//	if(strTmp.Find(def_REDCLR)>=0){
	//		strTmp.Replace(def_REDCLR, "");
	//		nShowColor = 1;
	//	}
	//	else if(strTmp.Find(def_YELLOWCLR)>=0){
	//		strTmp.Replace(def_YELLOWCLR, "");
	//		nShowColor = 0;
	//	}		
	//	else if(strTmp.Find(def_GREENCLR)>=0){
	//		strTmp.Replace(def_GREENCLR, "");
	//		nShowColor = 2;
	//	}		
	//	// 颜色（0：黄，1：红，2：绿）
	//	mbInfo[0]= (BYTE)(nShowColor&0x00ff);
	//	// 进入类型
	//	mbInfo[1]= (BYTE)(nEnterType&0x00ff);
	//	// 进入速度
	//	mbInfo[2]= (BYTE)(nEnterSpeed&0x00ff);
	//	// 停留显示
	//	mbInfo[3]= (BYTE)(nStayType&0x00ff);
	//	if(nStayType > 0){
	//		mbInfo[4] |= 0x80;
	//	}
	//	// 退出类型
	//	mbInfo[4]= (BYTE)(nExistType&0x00ff);
	//	// 退出速度
	//	mbInfo[5]= (BYTE)(nExistSpeed&0x00ff);
	//	// 显示文字的处理
	//	CStringA strCn, strEn;
	//	int nPos = strTmp.Find(":");
	//	if(nPos>0){
	//		strCn = strTmp.Left(nPos);
	//		strEn = strTmp.Mid(nPos+1);
	//		int i,j;
	//		// **** 中文文本
	//		// 补空格，以居中显示
	//		int strCnLen = strCn.GetLength();
	//		while(strCnLen<SCREEN_LINE_WIDTH-1){
	//			strCn = " " + strCn;
	//			strCnLen++;
	//			if(strCnLen>=SCREEN_LINE_WIDTH-1){
	//				break;
	//			}
	//			strCn = strCn + " ";
	//			strCnLen++;
	//		}
	//		// 使用居中后的文字
	//		char* tmpCn = strCn.GetBuffer();
	//		for(i=0; i<strCn.GetLength(); i++){
	//			mbInfo[8+i]= tmpCn[i];
	//		}
	//		for(j=i; j<20; j++){
	//			mbInfo[8+j]= 0x20;
	//		}
	//		// **** 英文文本
	//		// 补空格，以居中显示
	//		int strEnLen = strEn.GetLength();
	//		while(strEnLen<SCREEN_LINE_WIDTH-1){
	//			strEn = " " + strEn;
	//			strEnLen++;
	//			if(strEnLen>=SCREEN_LINE_WIDTH-1){
	//				break;
	//			}
	//			strEn = strEn + " ";
	//			strEnLen++;
	//		}
	//		char* tmpEn = strEn.GetBuffer();
	//		for(i=0; i<strEn.GetLength(); i++){
	//			mbInfo[28+i]= tmpEn[i];
	//		}
	//		for(j=i; j<20; j++){
	//			mbInfo[28+j]= 0x20;
	//		}
	//		WriteXMode(nItem, mbInfo, 48);
	//		Sleep(500);
	//	}
	//	else{
	//		// 扩展模式文言错误
	//		return 1;
	//	}
	//}
	
	tLEDDevReturn stRet;
	long lRet = LED_Init(&stRet);

	return lRet;
}

long CStatusDisplayCommand::GetStatus()
{
	tLEDModuleStatus stModuleStatus;
	tLEDDevReturn stDevStatus;
	long lRet = LED_GetDevStatus(&stModuleStatus,&stDevStatus);
	return lRet;
}


long CStatusDisplayCommand::ShowExtenModeText(int extenModeIndex)
{
	tLEDDevReturn stRet;
	long lRet = LED_Display(extenModeIndex,&stRet);
	return lRet;
}

/*
显示内容与init中传入的一致。
*/
long CStatusDisplayCommand::ShowFixedModeText(int fixedModeIndex)
{
	//int mWM = fixedModeIndex;
	//BYTE mbWM;
	//mbWM=(BYTE)(mWM&0x00FF);
	//mbWM += 0x10;
	//SetWorkMode(mbWM);
	return 0;
}
long CStatusDisplayCommand::DownloadTextToLed(int SerialNo,CString szCn,CString szEn,int color)
{
	tLED_Download_Text_Para pLED_Download_Text_Para;
	memset(pLED_Download_Text_Para.cCNText,0,sizeof(pLED_Download_Text_Para.cCNText));
	memset(pLED_Download_Text_Para.cENText,0,sizeof(pLED_Download_Text_Para.cENText));

	//stPara.uiTextNum = 1;
	pLED_Download_Text_Para.nSerialNO = SerialNo;
	pLED_Download_Text_Para.iFontColor = color;
	pLED_Download_Text_Para.iStayTime = 1;
	USES_CONVERSION;
	strcpy_s(pLED_Download_Text_Para.cCNText,T2A(szCn));
	strcpy_s(pLED_Download_Text_Para.cENText,T2A(szEn));
	tLEDDevReturn stRet;
	long lRet = LED_Download(pLED_Download_Text_Para,&stRet);
	//if (/*lRet == 0*/)
	//{
	//	lRet = LED_Display(pLED_Download_Text_Para.nSerialNO,&stRet);
	//}
	return lRet;
}