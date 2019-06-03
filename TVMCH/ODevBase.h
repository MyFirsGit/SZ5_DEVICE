/*
<ModuleInfo>
<Comment>
<产品名称>：DACS-NT(ODevBase)
<模块名称>：ODevBase
<创建日期>：2004.01.16
<作    者>：李正军
<测试时间>：
<测试人员>：
<功能说明>：基于组件方式的设备驱动基类
<调用的模块>：	GrgCommanager		通讯组件
				GrgLogServer		日志组件
				GrgCfgManager		配置组件
</Comment>

<ModiControl>
===========================本模块历史修改记录==========================
<MCREC>
<时间>：[2004.01.16 To 2004.01.18]
<修改者>：	 李正军
<当前版本>：[V3.0.0]
<详细记录>：
1、	<内容>：开发基于组件方式的设备驱动基类
	<原因>：创建
	<注意>：参考<<DACS-NT(ODevBase) v3.0.0 使用说明书>>
</MCREC>
-----------------------------------------------------------------------
<MCREC>
<时间>：[2004.05.16]
<修改者>：刘永胜
<当前版本>：[V3.2.0]
<详细记录>：
1、	<内容>：重写实现
	<原因>：
	<注意>：
</MCREC>
-----------------------------------------------------------------------
<MCREC>
<时间>：[2004.11.06]
<修改者>：刘永胜，刘文斌
<当前版本>：[V3.2.0]
<详细记录>：
1、	<内容>：重新修改部分函数的实现，另外加入USB的支持
	<原因>：
	<注意>：其中有关异步接收硬件主动上发的部分，使用时需要注意调用的方法，可以参考
			WOSA/SP2.0中SIU的SP代码。
			其中ODevBase的virtual函数vOnDataReceived，各个继承于ODevBase的子类不允许重载，
			保留为virtual函数，只为了与以前兼容
</MCREC>
============================历史修改记录完=============================
</ModiControl>						
*/
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODEVBASE_H__8BBC195A_112D_11D4_B865_5254AB1A7D30__INCLUDED_COM_)
#define AFX_ODEVBASE_H__8BBC195A_112D_11D4_B865_5254AB1A7D30__INCLUDED_COM_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 导出宏
#ifdef DEVBASEDLL_EXPORTS
#define  DEVBASEDLL_API __declspec(dllexport)
#else
#define DEVBASEDLL_API __declspec(dllimport)
	#ifdef _DEBUG
		#pragma comment(lib,"DevBaseDllD.lib")
		#pragma message("Automatically linking with DevBaseDllD.dll (Debug)")
	#else
		#pragma comment(lib,"DevBaseDll.lib")
		#pragma message("Automatically linking with DevBaseDll.dll (Release)")
	#endif
#endif

#include <map>
using namespace std;

#include "CommManager.h"
#include "LogServer.h"
#include "CfgManager.h"

// 通用定义
//#define SUCCESS					0	// 成功
//#define FAIL					1	// 失败

#define ERRORFLAG_CLEAR			0	// 清除错误缓存区

// 日志信息类别
#define CATALOG_FATAL			0	// 致命错误
#define	CATALOG_NORMAL			1	// 一般信息
#define	GATALOG_NOTE			2	// 提示信息

// MSG type
#define MSG_GENERAL				1	// 一般信息
#define	MSG_COMM_SEND			2	// 发送数据
#define	MSG_COMM_REV			3	// 接收数据

//==ODevBase==BEGIN==ERROR==DEFINE=======================================================================
#define E_DEVBASE_BASE				    	900

#define E_DEVBASE_HOLD				E_DEVBASE_BASE+10	// 910 错误阙值
#define E_DEVBASE_COMMANAGER		E_DEVBASE_BASE+11	// 911 GrgComManger组件创建失败
#define E_DEVBASE_CFGMANAGER		E_DEVBASE_BASE+12	// 912 GrgCfgManager组件创建失败
#define E_DEVBASE_LOGSERVER			E_DEVBASE_BASE+13	// 913 GrgLogServer组件创建失败
#define E_DEVBASE_COMMLOGINIT		E_DEVBASE_BASE+14	// 914 commlog组件初始化失败
#define E_DEVBASE_TRACELOGINIT		E_DEVBASE_BASE+15	// 915 tracelog组件初始化失败

#define E_DEVBASE_COMCFGFILE		E_DEVBASE_BASE+16	// 916 打开GRGDTATM_CommCfg.ini文件失败
#define E_DEVBASE_COMID				E_DEVBASE_BASE+17	// 917 读取配置ComID失败
#define E_DEVBASE_COMMTYPE			E_DEVBASE_BASE+18	// 918 读取配置CommType失败
#define E_DEVBASE_COMBAUD			E_DEVBASE_BASE+19	// 919 读取配置ComBaud失败
#define E_DEVBASE_COMBOARDPORT		E_DEVBASE_BASE+20	// 920 读取配置ComBoardPort失败
#define E_DEVBASE_COMBOARDPORTBAUD	E_DEVBASE_BASE+21	// 921 读取配置ComBoardPortBaud失败
#define E_DEVBASE_DEVCOMMLOGID		E_DEVBASE_BASE+22	// 922 读取配置DevCommLogID失败
#define E_DEVBASE_DEVTRACELOGID		E_DEVBASE_BASE+23	// 923 读取配置DevTraceLogID失败
#define E_DEVBASE_INICFGFILENAME	E_DEVBASE_BASE+24	// 924 打开IniCfgFileName项配置的文件失败
#define E_DEVBASE_LOGICNAMEISNULL	E_DEVBASE_BASE+25	// 925 设备的逻辑设备名为空
#define E_DEVBASE_USBSENDPIPE		E_DEVBASE_BASE+26	// 926 读取配置SendPipes失败
#define E_DEVBASE_USBRECVPIPE		E_DEVBASE_BASE+27	// 927 读取配置RecvPipes失败
#define E_DEVBASE_USBDEVICEGUID		E_DEVBASE_BASE+28	// 928 读取配置DeviceGUID失败
#define E_DEVBASE_TCPIPADDRESS		E_DEVBASE_BASE+29	//读取IP地址失败
#define E_DEVBASE_TCPIPSENDPORT		E_DEVBASE_BASE+30	//读取发送端口失败
#define E_DEVBASE_TCPIPRECVPORT		E_DEVBASE_BASE+31	//读取接收端口失败

//==ODevBase==END==ERROR==DEFINE=======================================================================
//==============================================================
#define TCPIP_ADDRESS_LENGTH	50
#define USB_PIPES_LENGTH		50
#define USB_EXTENDSTR_LENGTH	256

typedef struct
{
	int		iLogicCode;			// 逻辑错误码
	int		iPhyCode;			// 物理错误码
	int		iHandle;			// 处理方法：0-不处理 1-初始化 2-重发命令
	int		iType;				// 错误类型：0-警告 1-严重

	char	acDevReturn[128];	// 硬件返回信息
	char	acReserve[128];		// 保留信息
} tDevReturn;

typedef struct
{
	// 通讯配置参数
	int iCommType;				// 通讯类型0:串口通讯，1:串口板通讯，2:USB, 3:其它
	int	iComBoardDevPort;		// 使用串口板的端口号
	int iComBoardDevPortBaud;	// 使用串口板端口的波特率
	int iComId;					// 串口ID；
	int iBaudRate;				// 串口波特率

	//针对USB的参数
	GUID	sInterfaceGuid;						//设备接口GUID
	char	acSendPipes[USB_PIPES_LENGTH];		//发送管道号
	char	acRecvPipes[USB_PIPES_LENGTH];		//接收管道号
	char	acExtendStr[USB_EXTENDSTR_LENGTH];	//扩展串
	int		iReserve;							//保留值
	
	// 日志配置参数
	CString strDevCommLogID;	// Comm LOG配置Section
	CString strDevTraceLogID;	// Trace LOG配置Section
}tDevCfgInfo;

typedef	void (*PROCFUN)(int p_iType, WPARAM p_pData, int p_iLen);
typedef	void (*PROCFUN2)(int p_iType, WPARAM p_pData, int p_iLen, const void* p_pvParam);

class DEVBASEDLL_API ODevBase  
{
private:
	tDevReturn		m_sDevErr;					// 错误描述结构
	char			m_acTrace[200];				// 日志信息的临时变量: obsolete
	bool			m_bBreakAsynRecv;			// 中断异步通讯接收的发送，不再将事件发送给其他接收者
	map<IComm*, LPVOID> m_tmSinkObjects;		// 用于接收设备通讯组件产生的事件
	IComm*			m_poComManager;				// 串口通讯组件
	ILogServer*		m_poCommLogServer;			// comm日志组件
	ILogServer*		m_poTraceLogServer;			// trace日志组件
	ICfgAccess*		m_poCfgManager;				// 配置组件

	tDevCfgInfo		m_sDevCfgInfo;				// 通讯配置信息

	PROCFUN			m_pfProcFun;				// 上层回调函数指针
	PROCFUN2		m_pfProcFun2;
	void*			m_pSinkObject;				// 外部传入回调函数的最后一个指针参数

public:
	char			m_acVersion[32];			// 驱动版本信息
	char			m_acDevVersion[32];			// 硬件版本信息 
	char			m_acLogicalDevName[32];		// 设备逻辑标识名称  

public:    
	// 配置信息处理
	int iGetIntValue(const char *p_pcSection, const char *p_pcKeyName, int &p_iKeyValue);
	int iGetStringValue(const char *p_pcSection, const char *p_pcKeyName, CString &p_strKeyValue);
    int iSetIntValue(const char *p_pcSection, const char *p_pcKeyName, long p_lKeyValue);
    int iSetStringValue(const char *p_pcSection, const char *p_pcKeyName, CString p_strKeyValue);
	int iDeleteSection(const char* p_pcSection);
	int iDeleteKey(const char* p_pcSection, const char* p_pcKeyName);

	// 通讯及组件管理接口
	int iPreInitDev();								
	int iGetDefaultCommConfig(tCommCfg& p_psCommCfg);	
	int iInitComm(tCommCfg p_sCommCfg, bool p_bEnableEvents = false);
	int iCloseComm();
  
	// 发送和接收命令接口
	int iSendCommand(const unsigned char* p_pcCommand, unsigned int p_wLength, 
					 bool p_bClearRecvBuf = true, bool p_bLog = true);
	int iWaitResult(unsigned char* p_pcData, unsigned int& p_uiLength, unsigned int p_uiTime, 
					bool p_bLog = true);
	int	iSendAndRecvData(const unsigned char* p_pcSendData, const unsigned int p_iLenSend, 
						unsigned char* p_pcRevData, unsigned int& p_uiLenRev, unsigned int p_uiTime,
						bool p_bClearRecvBuf = true, bool p_bLog = true);
	
	// 发送接收数据加锁，独占访问端口
	int iEnterExclusiveComm(int p_iTime);
	int iLeaveExclusiveComm(int p_iReserve = 0);

	// 异步接收通讯数据时，回调上层回调函数
	virtual void vCallProcFun(int p_iType, char* p_pcData, int p_iLen, const void* p_pvParam = NULL);

	// 设置标志，中断异步通讯接收的发送，不再将事件发送给其他接收者
	virtual void vSetBreakAsynRecvFlag(bool p_bBreakAsynRecv);
	virtual bool bGetBreakAsynRecvFlag();

public:
	// 扩展的通讯方法
	int	iExtendCommFunc(DWORD p_dwFunc);  

	// 允许通讯组件以事件方式发送消息与否
	int	iEnableRevEvent(bool p_bEnableEvent); // 允许commanager以事件方式发送消息与否
	
	// 设置上层回调函数指针
	void vSetProcFun(const PROCFUN p_pvProcFun);
	// 设置上层回调函数指针，第二个参数为外部传入回调函数的最后一个指针参数，可以为对象的指针
	void vSetProcFun2(const PROCFUN2 p_pvProcFun, const void* p_pSinkObject = NULL);
	
	// 设置驱动的逻辑设备名
	void vSetLogicalDevName(const char* p_pcLogicalDevName);	// 设置驱动的逻辑设备名

	// 清除接收缓冲区
	void vClearRecvBuf();

	// 写日志，对于一般的出错记录只需赋值前两个数据, 或只赋第一个值。
    // p_bBinaryData表示是否为二进制数据。如果是，则可以传入多个字符串结束符'\0'
	int	iWriteLog(const char* p_pcData, const unsigned int p_uiLen = 0, int	p_iInfoCatalog = GATALOG_NOTE, 
				  int p_iMSGType = MSG_GENERAL, bool p_bBinaryData = false);


	// 硬件错误标志处理接口
	int			iGetErrorFlag();							// 读取硬件错误标志
	tDevReturn	tGetErrorFlag();							// 读取硬件错误标志
	void		vSetErrorFlag(const int p_iFlag);			// 设置硬件错误标志
	void		vSetErrorFlag(const tDevReturn p_sDevErr);	// 设置硬件错误标志

	// 以下函数是提供给驱动自己来实现	
	virtual int	iGetVersion(char *p_pcVersion);			// 取驱动软件版本号
	virtual int	iGetDevVersion(char *p_pcVersion);		// 取硬件版本号

	// 设备驱动通用接口
	virtual int	iGetStatus(tDevReturn* p_sDevReturn);	
	virtual int	iInit(tDevReturn* p_sDevReturn);	
	virtual int	iTest(tDevReturn* p_sDevReturn);	

	// 接收异步通讯数据
	virtual	void vOnDataReceived(char *p_pcData, int p_iLen);

	//2005-05-08扩充接口
	int iInitComm2(tCommCfg2 p_sCommCfg, bool p_bEnableEvents = false);
	int iGetDefaultCommConfig2(tCommCfg2& p_sCommCfg);
	// 清除接收缓冲区
	void vClearRecvBuf2(short p_shPipe = -1);
	//发送
	int iSendCommand2(const unsigned char* p_pcCommand, unsigned int p_wLength, 
		short p_shSendPipe, bool p_bClearRecvBuf = true, bool p_bLog = true);
	//接收
	int iWaitResult2(unsigned char* p_pcData, unsigned int& p_uiLength, unsigned int p_uiTime, 
		short p_shRecvPipe, bool p_bLog = true);
	//发送并接收
	int	iSendAndRecvData2(const unsigned char* p_pcSendData, const unsigned int p_iLenSend, 
		unsigned char* p_pcRevData, unsigned int& p_uiLenRev, unsigned int p_uiTime,
		short p_shSendPipe, short p_shRecvPipe, 
		bool p_bClearRecvBuf = true, bool p_bLog = true);
	//独占发送
	int iEnterExclusiveComm2(int p_iTime, short p_shPipe);
	//退出独占
	int iLeaveExclusiveComm2(int p_iReserve, short p_shPipe);
	//关闭Comm通讯
	int iCloseComm2();
	ODevBase();
	virtual ~ODevBase();

private:
	void vReleaseComm();// 释放本身的COM组件
	void trace(LPCTSTR Format, ...);
};

#endif // !defined(AFX_ODEVBASE_H__8BBC195A_112D_11D4_B865_5254AB1A7D30__INCLUDED_COM_)
