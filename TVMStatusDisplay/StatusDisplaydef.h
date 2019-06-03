#include <atlconv.h>
#include "StatusDisplayExp.h"
#ifndef _STATUSDISPLAY_DEF_H
#define _STATUSDISPLAY_DEF_H
const int  StatusDisplay_WAIT_TIME = 10;
static const unsigned long  COMMAND_StatusDisplay_CONNECT = 0x5101;
static const unsigned long  COMMAND_StatusDisplay_PRINT = 0x5102;
static const unsigned long  COMMAND_StatusDisplay_CLOSE = 0x5103;
static const unsigned long  COMMAND_StatusDisplay_OPENBOX = 0x5104;
#define UD_MAX_CONTENTLEN			11
#define MAX_SEND_BUFFER				128		// 最多发送的数据数量
#define UD_MAX_HALFBUFFER	UD_MAX_CONTENTLEN + 1
#define UD_MAXBUFFER		UD_MAX_CONTENTLEN * 2 + 1
#define UD_MAX_EN_CONTENTLEN		12
#define UD_MAX_EN_LALFBUFFER UD_MAX_EN_CONTENTLEN + 1
#define UD_MAX_EN_BUFFER			UD_MAX_EN_CONTENTLEN*2 + 1
#define GB2312						936
//------------------------------------------------------------------------------
//控制器类型
#define BX_5E1  0x0154
#define BX_5E2  0x0254
#define BX_5E3  0x0354
#define BX_5Q0P  0x1056
#define BX_5Q1P  0x1156
#define BX_5Q2P  0x1256
#define BX_6Q1  0x0166		// CQ用
#define BX_6Q2  0x0266
#define BX_6Q2L  0x0466

//----------------------------------------------------------------------

#define FRAME_SINGLE_COLOR_COUNT  23 //纯色边框图片个数
#define FRAME_MULI_COLOR_COUNT  47 //花色边框图片个数

//------------------------------------------------------------------------------
// 通讯模式
#define SEND_MODE_SERIALPORT  0
#define SEND_MODE_NETWORK  2
#define SEND_MODE_Server_2G  5
#define SEND_MODE_Server_3G  6
#define SEND_MODE_SAVEFILE  7
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// 动态区域运行模式
#define RUN_MODE_CYCLE_SHOW  0 //动态区数据循环显示；
#define RUN_MODE_SHOW_LAST_PAGE  1 //动态区数据显示完成后静止显示最后一页数据；
#define RUN_MODE_SHOW_CYCLE_WAITOUT_NOSHOW  2 //动态区数据循环显示，超过设定时间后数据仍未更新时不再显示；
#define RUN_MODE_SHOW_ORDERPLAYED_NOSHOW  4 //动态区数据顺序显示，显示完最后一页后就不再显示
//------------------------------------------------------------------------------

#define SCREEN_NO  3
#define SCREEN_WIDTH  256
#define SCREEN_HEIGHT 64
#define SCREEN_TYPE  4
#define SCREEN_DATADA  0
#define SCREEN_DATAOE  0
#define SCREEN_COMM  "COM1"
#define SCREEN_BAUD  57600
#define SCREEN_ROWORDER  0
#define SCREEN_FREQPAR  0
#define SCREEN_SOCKETIP  _T("192.168.0.199")
#define SCREEN_SOCKETPORT  5005
//#define m_bSendBusy  false	此变量在数据更新中非常重要，请务必保留。
#define FAddDynamicArea  "第二步-----添加动态区域"
#define FAddDYAreaFile  "第三步-----动态区域文件属性"
typedef enum _eledclr
{
	UE_RED=0x31,
	UE_GREEN,
	UE_YELLOW
}ELEDCLR;
typedef enum _dyareald
{
	UE_DYAREAID_0 = 0,
	UE_DYAREAID_1,
	UE_DYAREAID_2,
	UE_DYAREAID_3,
}DYAREAID;
typedef enum _eleddispmode
{
	UE_MOVECYCLE=0x00,
	UE_NOMOVE=0x01,
	UE_IMMEDIATE,
	UE_MOVELEFT,
	UE_MOVERIGHT,
	UE_MOVEUP,
	UE_MOVEDOWN
}ELEDDISPMODE;

typedef enum _eledmovespeed
{
	UE_LEVEL0 =0x00,
	UE_LEVEL1,
	UE_LEVEL2,
	UE_LEVEL3,
	UE_LEVEL4,
	UE_LEVEL5,
	UE_LEVEL6,
	UE_LEVEL7,
	UE_LEVEL8,
	UE_LEVEL9
}ELEDMOVESPEED;
// 定义状态显示屏工作模式
typedef struct _disp_work_model_{
	ELEDDISPMODE display_mode;		// 移动方向
	ELEDDISPMODE exit_mode;			// 退出方向
	ELEDMOVESPEED display_speed;	// 移动速度
	int			nDispayTime;		// 停留时间
	CString		strDispContent;		// 显示包含内容
	_disp_work_model_(){
		display_mode  = UE_MOVELEFT;
		exit_mode	  = UE_MOVELEFT;
		display_speed = UE_LEVEL4;
		nDispayTime	  = 5;
		strDispContent= _T("");
	}

}DISP_WORK_MODEL;
//区域边框属性
typedef struct _tdAreaFrameInfo
{
	byte btAreaFFlag;		// 区域边框标志位，注：如果此字段为0，则以下区域边框属性不发送
	byte btAreaFDispStyle;	// 边框显示方式：0x00 –闪烁
											//0x01 –顺时针转动
											//0x02 –逆时针转动
											//0x03 –闪烁加顺时针转动
											//0x04 –闪烁加逆时针转动
											//0x05 –红绿交替闪烁
											//0x06 –红绿交替转动
											//0x07 –静止打出
	byte btAreaFDispSpeed;	// 边框显示速度
	byte btAreaFMoveStep;	// 边框移动步长，单位为点，此参数范围为1~16
	byte btAreaFWidth;		// 边框组元宽度，此参数范围为1~8，注：边框组元的长度固定为16
	byte btAreaFBackup[2];	// 备用字
	byte btAreaFUnitData;	// =AreaFWidth
	_tdAreaFrameInfo()
	{
		btAreaFFlag = 0x01;
		btAreaFDispStyle = 0x01;
		btAreaFDispSpeed = 0x02;
		btAreaFMoveStep = 0x08;
		btAreaFWidth = 0x08;
		memset(btAreaFBackup,0,2);
		btAreaFUnitData = btAreaFWidth;
	}
}AREAFRAME;
typedef struct _tdispinfo
{
	WORD wScreenNo;		// 显示屏屏号，该参数与AddScreen_Dynamic函数中的nScreenNo参数对应
	WORD wDYAreaID;		// 动态区域编号
	WORD wRunMode;	// 动态区运行模式0-动态区数据循环显示
	WORD wTimeOut;			// 动态区数据超时时间，单位为秒
	WORD wAllProRelate;		// 节目关联标志	1：所有节目都显示该动态区域
	char sProRelateList[16];	// 节目关联列表用节目编号表示；节目编号间用","隔开,节目编号定义为LedshowTW 2013软件中"P***"中的"***"
	WORD wPlayPriority;	// 动态区域播放优先级；0：该动态区域与异步节目一起播放(动态区域有关联异步节目才有效)； 1：异步节目停止播放，仅播放该动态区域效。
	WORD wAreaX;
	WORD wAreaY;
	WORD wAreaWidth;
	WORD wAreaHeight;
	WORD wAreaFMode;		// 动态区域边框显示标志0：纯色；1：花色；255：无边框
	WORD wAreaFLine;		// 动态区域边框类型，出色最大取值为FRAME_SINGLE_COLOR_COUNT；花色最大取值为：FRAME_MULI_COLOR_COUNT
	WORD wAreaFColor;		// 边框显示颜色；选择为纯色边框类型时该参数有效；
	WORD wAreaFStunt;		// 边框运行特技；
							// 	0：闪烁；
							// 		1：顺时针转动；
							// 		2：逆时针转动；
							// 		3：闪烁加顺时针转动； 
							// 		4:闪烁加逆时针转动；
							// 		5：红绿交替闪烁；
							// 		6：红绿交替转动；
							// 		7：静止打出
	WORD wAreaFRunSpeed;	// 边框运行速度；
	WORD wAreaFMoveStep;	// 边框移动步长；该值取值范围：1~8；
	_tdispinfo()
	{
		wScreenNo = SCREEN_NO;
		wDYAreaID = UE_DYAREAID_0; 
		wRunMode = UE_MOVECYCLE;
		wTimeOut = 0x0a;
		wAllProRelate = 0x00;
		strset(sProRelateList,0x00);
		wPlayPriority = 1;
		wAreaX = 0;
		wAreaY = 0;
		wAreaWidth = 0;
		wAreaHeight = 0;
		wAreaFMode = 0xff;
		wAreaFLine = 0x00;
		wAreaFColor = 0x00;
		wAreaFStunt = 0x00;
		wAreaFRunSpeed = 0x00;
		wAreaFMoveStep = 0x00;
	}
}TDISPINFO;
typedef struct _tdispinfotext
{
	WORD wScreenNo;		// 显示屏屏号，该参数与AddScreen_Dynamic函数中的nScreenNo参数对应
	WORD wDYAreaID;		// 动态区域编号
	char sText[48];		// 添加的信息文本字符串
	WORD wShowSingle;		//文字信息是否单行显示；0：多行显示；1：单行显示
	char sFontName[16];	// 文字信息显示字体 :"黑体"
	WORD wFontSize;		// 文字信息显示字体的字号
	WORD wBold;			// 文字信息是否粗体显示；0：正常；1：粗体显示；
	DWORD wFontColor;		// 文字信息显示颜色；0~255
	WORD wStunt;			// 动态区域信息运行特技；
							// 		00：随机显示		11：左右移入
							// 		01：静止显示		12：左右交叉移入
							// 		02：快速打出		13：上下交叉移入
							// 		03：向左移动		14：画卷闭合
							// 		04：向左连移		15：画卷打开
							// 		05：向上移动 	16：向左拉伸
							// 		06：向上连移		17：向右拉伸
							// 		07：闪烁			18：向上拉伸
							// 		08：飘雪			19：向下拉伸
							// 		09：冒泡 		20：向左镭射
							// 		10：中间移出 	21：向右镭射
							// 		22：向上镭射 		31：向上拉幕
							// 		23：向下镭射 		32：向下拉幕
							// 		24：左右交叉拉幕		33：左右闭合
							// 		25：上下交叉拉幕		34：左右对开
							// 		26：分散左拉		35：上下闭合
							// 		27：水平百页 		36：上下对开
							// 		28：垂直百页		37：向右移动
							// 		29：向左拉幕 		38：向右连移
							// 		30：向右拉幕 		39：向下移动
							//		40：向下连移
	WORD wRunSpeed;		//动态区域信息显示速度运行速度；1~100
	WORD wShowTime;		// 动态区域信息显示时间；单位：10ms

	_tdispinfotext()
	{
		wScreenNo = SCREEN_NO;
		wDYAreaID =	UE_DYAREAID_0; 
		memset(sText,0,sizeof(sText));
		wShowSingle = 1;
		strcpy(sFontName ,"黑体");
		wFontSize = 16;
		wBold = 0;
		wFontColor = 255;
		wStunt = 7;
		wRunSpeed = 5;
		wShowTime = 10;
	}
	void setContent(const TCHAR* tcContent)
	{
		#ifdef UNICODE
		UINT uiDetect = WideCharToMultiByte(CP_OEMCP, NULL, tcContent, -1, NULL, 0, NULL, FALSE);
		UINT uiMaxTCHAR = 0;

		bool bIsMaxContent = false;
		
		if (uiDetect > UD_MAXBUFFER){
			bIsMaxContent = true;
		}
		else{
			uiMaxTCHAR = _tcslen(tcContent);
		}

		// 超过最大字符数
		if(bIsMaxContent){
			UINT  uiContentLen = 0;
			TCHAR tcTemp[MAX_SEND_BUFFER];
			memset(tcTemp,'\0',sizeof(TCHAR)*MAX_SEND_BUFFER);

			uiContentLen = _tcslen(tcContent);
			_tcsncpy(tcTemp,tcContent,uiContentLen);

			// 宽字符转窄字符
			CW2AEX<> wChange(tcTemp,GB2312);

			DWORD dwDataLen = strlen(wChange);
			memmove(sText,wChange,dwDataLen);
		}
		// 在正常字符数以内，需要居中显示
		else{
			TCHAR tcMAxTCHAR[UD_MAXBUFFER];
			memset(tcMAxTCHAR, '\0', sizeof(TCHAR) * UD_MAXBUFFER);

			_tcsncpy(tcMAxTCHAR, tcContent, uiMaxTCHAR);

			TCHAR tcbuffer[UD_MAX_HALFBUFFER] = _T("\0");

			UINT uilen = _tcslen(tcMAxTCHAR);
			UINT uileft = uilen / 2;
			UINT uirght = uilen - uileft;

			TCHAR tcleft[UD_MAX_HALFBUFFER] = _T("\0");
			TCHAR tcright[UD_MAX_HALFBUFFER] = _T("\0");;

			_tcsncpy(tcleft, tcMAxTCHAR, uileft);
			_tcsncpy(tcright, tcMAxTCHAR + uileft, uirght);

			TCHAR tcLeftContent[UD_MAX_CONTENTLEN + 1];
			TCHAR tcRightContent[UD_MAX_CONTENTLEN + 1];

			UINT uipartlen = _tcslen(tcleft);
			UINT uiCharCount = WideCharToMultiByte(CP_OEMCP, NULL, tcleft, -1, NULL, 0, NULL, FALSE);
			CString szformat;
			CString szCount;
			szCount.Format(_T(" %ds"), UD_MAX_CONTENTLEN  - uiCharCount+1);
			szCount=_T("%")+szCount;
			szformat = szCount + _T("%s");
			_stprintf(tcbuffer, szformat, _T(""),tcleft);
			_tcsncpy_s(tcLeftContent, UD_MAX_CONTENTLEN + 1, tcbuffer, UD_MAX_CONTENTLEN);
			tcLeftContent[UD_MAX_CONTENTLEN]='\0';

			uipartlen = _tcslen(tcright);
			uiCharCount = WideCharToMultiByte(CP_OEMCP, NULL, tcright, -1, NULL, 0, NULL, FALSE);
			szformat = _T("%s% ");
			CString szcount;
			szcount.Format(_T("%ds"), UD_MAX_CONTENTLEN - uiCharCount +1);
			szformat += szcount;
			_stprintf(tcbuffer, szformat, tcright, _T(""));
			_tcsncpy_s(tcRightContent, UD_MAX_CONTENTLEN+1,tcbuffer,UD_MAX_CONTENTLEN);
			tcRightContent[UD_MAX_CONTENTLEN]='\0';

			CW2AEX<> w2aLeft(tcLeftContent,GB2312);
			CW2AEX<> w2aRight(tcRightContent,GB2312);

			int ileftchar=strlen(w2aLeft);
			int irightchar=strlen(w2aRight);
			memmove(sText,w2aLeft,ileftchar);
			memmove(sText+ileftchar,w2aRight,irightchar);
		}
#else

#endif

	}
void setContentEn(const TCHAR* tcContent)
	{
#ifdef UNICODE
		UINT uiDetect = WideCharToMultiByte(CP_OEMCP, NULL, tcContent, -1, NULL, 0, NULL, FALSE);
		UINT uiMaxTCHAR = 0;

		bool bIsMaxContent = false;

		if (uiDetect > UD_MAXBUFFER){
			bIsMaxContent = true;
		}
		else{
			uiMaxTCHAR = _tcslen(tcContent);
		}

		// 超过最大字符数
		if(bIsMaxContent){
			UINT  uiContentLen = 0;
			TCHAR tcTemp[MAX_SEND_BUFFER];
			memset(tcTemp,'\0',sizeof(TCHAR)*MAX_SEND_BUFFER);

			uiContentLen = _tcslen(tcContent);
			_tcsncpy(tcTemp,tcContent,uiContentLen);

			// 宽字符转窄字符
			CW2AEX<> wChange(tcTemp,GB2312);

			DWORD dwDataLen = strlen(wChange);
			memmove(sText,wChange,dwDataLen);
		}
		// 在正常字符数以内，需要居中显示
		else{
			TCHAR tcMAxTCHAR[UD_MAX_EN_BUFFER] = _T("\0");
			memset(tcMAxTCHAR, '\0', sizeof(TCHAR) * UD_MAXBUFFER);

			_tcsncpy(tcMAxTCHAR, tcContent, uiMaxTCHAR);

			TCHAR tcbuffer[UD_MAX_EN_LALFBUFFER] = _T("\0");

			UINT uilen = _tcslen(tcMAxTCHAR);
			UINT uileft = uilen / 2;
			UINT uirght = uilen - uileft;

			TCHAR tcleft[UD_MAX_EN_LALFBUFFER] = _T("\0");
			TCHAR tcright[UD_MAX_EN_LALFBUFFER] = _T("\0");;

			_tcsncpy(tcleft, tcMAxTCHAR, uileft);
			_tcsncpy(tcright, tcMAxTCHAR + uileft, uirght);

			TCHAR tcLeftContent[UD_MAX_EN_CONTENTLEN+1]= _T("\0");
			TCHAR tcRightContent[UD_MAX_EN_CONTENTLEN+1]= _T("\0");

			UINT uipartlen = _tcslen(tcleft);
			UINT uiCharCount = WideCharToMultiByte(CP_OEMCP, NULL, tcleft, -1, NULL, 0, NULL, FALSE);
			CString szformat;
			CString szCount;
			szCount.Format(_T(" %ds"), UD_MAX_EN_CONTENTLEN  - uiCharCount+1);
			szCount=_T("%")+szCount;
			szformat = szCount + _T("%s");
			_stprintf(tcbuffer, szformat, _T(""),tcleft);
			_tcsncpy_s(tcLeftContent, UD_MAX_EN_CONTENTLEN+1, tcbuffer, UD_MAX_EN_CONTENTLEN);
			tcLeftContent[UD_MAX_EN_CONTENTLEN]='\0';

			uipartlen = _tcslen(tcright);
			uiCharCount = WideCharToMultiByte(CP_OEMCP, NULL, tcright, -1, NULL, 0, NULL, FALSE);
			szformat = _T("%s% ");
			CString szcount;
			szcount.Format(_T("%ds"), UD_MAX_EN_CONTENTLEN - uiCharCount +1);
			szformat += szcount;
			_stprintf(tcbuffer, szformat, tcright, _T(""));
			_tcsncpy_s(tcRightContent, UD_MAX_EN_CONTENTLEN+1,tcbuffer,UD_MAX_EN_CONTENTLEN);
			tcRightContent[UD_MAX_EN_CONTENTLEN]='\0';

			CW2AEX<> w2aLeft(tcLeftContent,GB2312);
			CW2AEX<> w2aRight(tcRightContent,GB2312);

			int ileftchar=strlen(w2aLeft);
			int irightchar=strlen(w2aRight);
			memmove(sText,w2aLeft,ileftchar);
			memmove(sText+ileftchar,w2aRight,irightchar);
		}
#else

#endif

	}
}TDISPINFOTEXT;
#define UD_CLRPTR 3



/*-------------------------------------------------------------------------------
过程名:    Initialize
初始化动态库；该函数不与显示屏通讯。
参数:
返回值            :详见返回状态代码定义。
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pInitialize)();	//初始化动态库

/*-------------------------------------------------------------------------------
过程名:    Uninitialize
初始化动态库；该函数不与显示屏通讯。
参数:
返回值            :详见返回状态代码定义。
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pUninitialize)();	//释放动态库

/*-------------------------------------------------------------------------------
过程名:    AddScreen
向动态库中添加显示屏信息；该函数不与显示屏通讯。
参数:
nControlType:显示屏的控制器型号，目前该动态区域动态库只支持BX-5E1、BX-5E2、BX-5E3等BX-5E系列控制器。
nScreenNo：显示屏屏号；该参数与LedshowTW 2013软件中"设置屏参"模块的"屏号"参数一致。
nSendMode：通讯模式；目前动态库中支持0:串口通讯；2：网络通讯(只支持固定IP模式)；5：保存到文件等三种通讯模式。
nWidth：显示屏宽度；单位：像素。
nHeight：显示屏高度；单位：像素。
nScreenType：显示屏类型；1：单基色；2：双基色。
nPixelMode：点阵类型，只有显示屏类型为双基色时有效；1：R+G；2：G+R。
pCom：通讯串口，只有在串口通讯时该参数有效；例："COM1"
nBaud：通讯串口波特率，只有在串口通讯时该参数有效；目前只支持9600、57600两种波特率。
pSocketIP：控制器的IP地址；只有在网络通讯(固定IP模式)模式下该参数有效，例："192.168.0.199"
nSocketPort：控制器的端口地址；只有在网络通讯(固定IP模式)模式下该参数有效，例：5005
nServerMode     :0:服务器模式未启动；1：服务器模式启动。
pNetworkID      :服务器模式时的网络ID编号
pServerIP       :中转服务器IP地址
nServerPort     :中转服务器网络端口
pServerAccessUser:中转服务器访问用户名
pServerAccessPassword:中转服务器访问密码
pCommandDataFile：保存到文件方式时，命令保存命令文件名称。只有在保存到文件模式下该参数有效，例："curCommandData.dat"
返回值:    详见返回状态代码定义。
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pAddScreen_Dynamic)(int nControlType, int nScreenNo, int nSendMode, int nWidth, int nHeight,
        int nScreenType, int nPixelMode, char* pCom, int nBaud, char* pSocketIP, int nSocketPort, int nStaticIpMode, int nServerMode,
        char* pBarcode, char* pNetworkID, char* pServerIP, int nServerPort, char* pServerAccessUser, char* pServerAccessPassword,
        char* pCommandDataFile);

/*-------------------------------------------------------------------------------
过程名:    QuerryServerDeviceList
查询中转服务器设备的列表信息。
该函数与显示屏进行通讯
参数:      nScreenNo, nSendMode: Integer
nScreenNo       :显示屏屏号；该参数与AddScreen函数中的nScreenNo参数对应。
nSendMode       :与显示屏的通讯模式；
    0:串口模式、BX-5A2&RF、BX-5A4&RF等控制器为RF串口无线模式;
    2:网络模式;
    4:WiFi模式
    5:ONBON服务器-GPRS
    6:ONBON服务器-3G
pDeviceList       : 保存查询的设备列表信息
    将设备的信息用组成字符串, 比如：
    设备1：名称 条形码 状态 类型 网络ID
    设备2：名称 条形码 状态 类型 网络ID
    组成字符串为：'设备1名称,设备1条形码,设备1状态,设备1类型,设备1网络ID;设备2名称,设备2条形码,设备2状态,设备2类型,设备2网络ID'
    设备状态(Byte):  $10:设备未知
                    $11:设备在线
                    $12:设备不在线

    设备类型(Byte): $16:设备为2G
                    $17:设备为3G
pDeviceCount      : 查询的设备个数

返回值            :详见返回状态代码定义。
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pQuerryServerDeviceList)(int nScreenNo, byte pDeviceList[], int &nDeviceCount);

/*-------------------------------------------------------------------------------
过程名:    BindServerDevice
绑定中转服务器设备。
该函数不与显示屏进行通讯
参数:      nScreenNo, nSendMode: Integer
nScreenNo       :显示屏屏号；该参数与AddScreen函数中的nScreenNo参数对应。
pBarcode       :绑定设备的条形码；
pNetworkId     :绑定设备的网络ID；

返回值            :详见返回状态代码定义。
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pBindServerDevice)(int nScreenNo, char* pBarcode, char* pNetworkId);

/*-------------------------------------------------------------------------------
过程名:    AddScreenDynamicArea
向动态库中指定显示屏添加动态区域；该函数不与显示屏通讯。
参数:
nScreenNo：显示屏屏号；该参数与AddScreen函数中的nScreenNo参数对应。
nDYAreaID：动态区域编号；目前系统中最多5个动态区域；该值取值范围为0~4;
nRunMode：动态区域运行模式：
            0:动态区数据循环显示；
            1:动态区数据显示完成后静止显示最后一页数据；
            2:动态区数据循环显示，超过设定时间后数据仍未更新时不再显示；
            3:动态区数据循环显示，超过设定时间后数据仍未更新时显示Logo信息,Logo 信息即为动态区域的最后一页信息
            4:动态区数据顺序显示，显示完最后一页后就不再显示
nTimeOut：动态区数据超时时间；单位：秒 
nAllProRelate：节目关联标志；
            1：所有节目都显示该动态区域；
            0：在指定节目中显示该动态区域，如果动态区域要独立于节目显示，则下一个参数为空。
pProRelateList：节目关联列表，用节目编号表示；节目编号间用","隔开,节目编号定义为LedshowTW 2013软件中"P***"中的"***"
nPlayImmediately：动态区域是否立即播放0：该动态区域与异步节目一起播放；1：异步节目停止播放，仅播放该动态区域
nAreaX：动态区域起始横坐标；单位：像素
nAreaY：动态区域起始纵坐标；单位：像素
nAreaWidth：动态区域宽度；单位：像素
nAreaHeight：动态区域高度；单位：像素
nAreaFMode：动态区域边框显示标志；0：纯色；1：花色；255：无边框
nAreaFLine：动态区域边框类型, 纯色最大取值为FRAME_SINGLE_COLOR_COUNT；花色最大取值为：FRAME_MULI_COLOR_COUNT
nAreaFColor：边框显示颜色；选择为纯色边框类型时该参数有效；
nAreaFStunt：边框运行特技；
            0：闪烁；1：顺时针转动；2：逆时针转动；3：闪烁加顺时针转动；
            4:闪烁加逆时针转动；5：红绿交替闪烁；6：红绿交替转动；7：静止打出
nAreaFRunSpeed：边框运行速度；
nAreaFMoveStep：边框移动步长；该值取值范围：1~8；
返回值:    详见返回状态代码定义。
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pAddScreenDynamicArea)(int nScreenNo, int nDYAreaID, int nRunMode,
    int nTimeOut, int nAllProRelate, char* pProRelateList, int nPlayImmediately,
    int nAreaX, int nAreaY, int nAreaWidth, int nAreaHeight, int nAreaFMode, int nAreaFLine, int nAreaFColor,
    int nAreaFStunt, int nAreaFRunSpeed, int nAreaFMoveStep);
/*
过程名: DeleteScreenDynamicArea
删除动态库中指定显示屏指定的动态区域信息；该函数不与显示屏通讯。
参数：
nScreenNo	显示屏屏号；该参数与AddScreen_Dynamic函数中的nScreenNo参数对应。
nDYAreaID	动态区域编号；该参数与AddScreenDynamicArea函数中的nDYAreaID参数对应
*/
typedef int(__stdcall *pDeleteScreenDynamicArea)(int nScreenNo, int nDYAreaID);

/*-------------------------------------------------------------------------------
过程名:    AddScreenDynamicAreaFile
向动态库中指定显示屏的指定动态区域添加信息文件；该函数不与显示屏通讯。
参数:
nScreenNo：显示屏屏号；该参数与AddScreen函数中的nScreenNo参数对应。
nDYAreaID：动态区域编号；该参数与AddScreenDynamicArea函数中的nDYAreaID参数对应
pFileName：添加的信息文件名称；目前只支持txt(支持ANSI、UTF-8、Unicode等格式编码)、bmp的文件格式
nShowSingle：文字信息是否单行显示；0：多行显示；1：单行显示；显示该参数只有szFileName为txt格式文档时才有效；
pFontName：文字信息显示字体；该参数只有szFileName为txt格式文档时才有效；
nFontSize：文字信息显示字体的字号；该参数只有szFileName为txt格式文档时才有效；
nBold：文字信息是否粗体显示；0：正常；1：粗体显示；该参数只有szFileName为txt格式文档时才有效；
nFontColor：文字信息显示颜色；该参数只有szFileName为txt格式文档时才有效；
nStunt：动态区域信息运行特技；
            00：随机显示 
            01：静止显示
            02：快速打出 
            03：向左移动 
            04：向左连移 
            05：向上移动 
            06：向上连移 
            07：闪烁 
            08：飘雪 
            09：冒泡 
            10：中间移出 
            11：左右移入 
            12：左右交叉移入 
            13：上下交叉移入 
            14：画卷闭合 
            15：画卷打开 
            16：向左拉伸 
            17：向右拉伸 
            18：向上拉伸 
            19：向下拉伸 
            20：向左镭射 
            21：向右镭射 
            22：向上镭射 
            23：向下镭射 
            24：左右交叉拉幕 
            25：上下交叉拉幕 
            26：分散左拉 
            27：水平百页 
            28：垂直百页 
            29：向左拉幕 
            30：向右拉幕 
            31：向上拉幕 
            32：向下拉幕 
            33：左右闭合 
            34：左右对开 
            35：上下闭合 
            36：上下对开 
            37：向右移动 
            38：向右连移 
            39：向下移动 
            40：向下连移
nRunSpeed：动态区域信息运行速度
nShowTime：动态区域信息显示时间；单位：10ms
返回值:    详见返回状态代码定义。
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pAddScreenDynamicAreaFile)(int nScreenNo, int nDYAreaID,
    char* pFileName, int nShowSingle, char* pFontName, int nFontSize, int nBold, int nFontColor,
    int nStunt, int nRunSpeed, int nShowTime);

/*-------------------------------------------------------------------------------
过程名:    AddScreenDynamicAreaText
向动态库中指定显示屏的指定动态区域添加信息文件；该函数不与显示屏通讯。
参数:
nScreenNo：显示屏屏号；该参数与AddScreen函数中的nScreenNo参数对应。
nDYAreaID：动态区域编号；该参数与AddScreenDynamicArea函数中的nDYAreaID参数对应
pText：添加的信息文件名称；目前只支持txt(支持ANSI、UTF-8、Unicode等格式编码)、bmp的文件格式
nShowSingle：文字信息是否单行显示；0：多行显示；1：单行显示；显示该参数只有szFileName为txt格式文档时才有效；
pFontName：文字信息显示字体；该参数只有szFileName为txt格式文档时才有效；
nFontSize：文字信息显示字体的字号；该参数只有szFileName为txt格式文档时才有效；
nBold：文字信息是否粗体显示；0：正常；1：粗体显示；该参数只有szFileName为txt格式文档时才有效；
nFontColor：文字信息显示颜色；该参数只有szFileName为txt格式文档时才有效；
nStunt：动态区域信息运行特技；
            00：随机显示 
            01：静止显示
            02：快速打出 
            03：向左移动 
            04：向左连移 
            05：向上移动 
            06：向上连移 
            07：闪烁 
            08：飘雪 
            09：冒泡 
            10：中间移出 
            11：左右移入 
            12：左右交叉移入 
            13：上下交叉移入 
            14：画卷闭合 
            15：画卷打开 
            16：向左拉伸 
            17：向右拉伸 
            18：向上拉伸 
            19：向下拉伸 
            20：向左镭射 
            21：向右镭射 
            22：向上镭射 
            23：向下镭射 
            24：左右交叉拉幕 
            25：上下交叉拉幕 
            26：分散左拉 
            27：水平百页 
            28：垂直百页 
            29：向左拉幕 
            30：向右拉幕 
            31：向上拉幕 
            32：向下拉幕 
            33：左右闭合 
            34：左右对开 
            35：上下闭合 
            36：上下对开 
            37：向右移动 
            38：向右连移 
            39：向下移动 
            40：向下连移
nRunSpeed：动态区域信息运行速度
nShowTime：动态区域信息显示时间；单位：10ms
返回值:    详见返回状态代码定义。
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pAddScreenDynamicAreaText)(int nScreenNo, int nDYAreaID,
    char* pText, int nShowSingle, char* pFontName, int nFontSize, int nBold, int nFontColor,
    int nStunt, int nRunSpeed, int nShowTime);

/*-------------------------------------------------------------------------------
    过程名:    DeleteScreen
    删除动态库中指定显示屏的所有信息；该函数不与显示屏通讯。
    参数:
    nScreenNo：显示屏屏号；该参数与AddScreen函数中的nScreenNo参数对应。
    返回值:    详见返回状态代码定义
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pDeleteScreen_Dynamic)(int nScreenNo);

/*-------------------------------------------------------------------------------
    过程名:    DeleteScreenDynamicAreaFile
    删除动态库中指定显示屏指定的动态区域指定文件信息；该函数不与显示屏通讯。
    参数:
    nScreenNo：显示屏屏号；该参数与AddScreen函数中的nScreenNo参数对应。
    nDYAreaID：动态区域编号；该参数与AddScreenDynamicArea函数中的nDYAreaID参数对应
    nFileOrd：动态区域的指定文件的文件序号；该序号按照文件添加顺序，从0顺序递增，如删除中间的文件，后面的文件序号自动填充。
    返回值:    详见返回状态代码定义
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pDeleteScreenDynamicAreaFile)(int nScreenNo, int nDYAreaID, int nFileOrd);

/*-------------------------------------------------------------------------------
    过程名:    SendDynamicAreasInfoCommand
    发送动态库中指定显示屏指定的动态区域信息到显示屏；该函数与显示屏通讯。
    参数:
    nScreenNo：显示屏屏号；该参数与AddScreen函数中的nScreenNo参数对应。
    nDelAllDYArea	动态区域编号列表；1：同时发送多个动态区域，0：发送单个动态区域
	pDYAreaIDList	动态区域编号；当nDelAllDYArea为1时，其值为” ”；当nDelAllDYArea为0时，该参数与AddScreenDynamicArea函数中的nDYAreaID参数对应，发送相应动态区域
    返回值:    详见返回状态代码定义
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pSendDynamicAreasInfoCommand)(int nScreenNo, int nDelAllDYArea, char* pDYAreaIDList);

/*-------------------------------------------------------------------------------
    过程名:    SendDeleteDynamicAreasCommand
    删除动态库中指定显示屏指定的动态区域信息；同时向显示屏通讯删除指定的动态区域信息。该函数与显示屏通讯
    参数:
    nScreenNo：显示屏屏号；该参数与AddScreen函数中的nScreenNo参数对应。
    nDelAllDYArea	动态区域编号列表；1：同时删除多个动态区域，0：删除单个动态区域
	pDYAreaIDList	动态区域编号；当nDelAllDYArea为1时，其值为” ”；当nDelAllDYArea为0时，该参数与AddScreenDynamicArea函数中的nDYAreaID参数对应，删除相应动态区域
    返回值:    详见返回状态代码定义
-------------------------------------------------------------------------------*/
typedef int(__stdcall *pSendDeleteDynamicAreasCommand)(int nScreenNo, int nDelAllDYArea, char* pDYAreaIDList);

class STATUS_DISPLAY_API CLedDispInfo
{
public:
	CLedDispInfo();
	~CLedDispInfo();
private:
	TDISPINFO m_dispInfo;		 //设置显示区域
	TDISPINFOTEXT m_dispInfoText;//设置显示文字

public:
	BOOL m_bSendBusy;//此变量在数据更新中非常重要，请务必保留。
	int g_nSendMode;//通讯模式值变量
	
public:
	BOOL addScene(TDISPINFO);
	BOOL addSceneText(TDISPINFOTEXT);
	byte* getDispCommandBytes();
	byte* getDispTextCommandBytes();
};

#endif