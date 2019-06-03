/*
<ModuleInfo>
<Comment>
<��Ʒ����>��DACS-NT(ODevBase)
<ģ������>��ODevBase
<��������>��2004.01.16
<��    ��>��������
<����ʱ��>��
<������Ա>��
<����˵��>�����������ʽ���豸��������
<���õ�ģ��>��	GrgCommanager		ͨѶ���
				GrgLogServer		��־���
				GrgCfgManager		�������
</Comment>

<ModiControl>
===========================��ģ����ʷ�޸ļ�¼==========================
<MCREC>
<ʱ��>��[2004.01.16 To 2004.01.18]
<�޸���>��	 ������
<��ǰ�汾>��[V3.0.0]
<��ϸ��¼>��
1��	<����>���������������ʽ���豸��������
	<ԭ��>������
	<ע��>���ο�<<DACS-NT(ODevBase) v3.0.0 ʹ��˵����>>
</MCREC>
-----------------------------------------------------------------------
<MCREC>
<ʱ��>��[2004.05.16]
<�޸���>������ʤ
<��ǰ�汾>��[V3.2.0]
<��ϸ��¼>��
1��	<����>����дʵ��
	<ԭ��>��
	<ע��>��
</MCREC>
-----------------------------------------------------------------------
<MCREC>
<ʱ��>��[2004.11.06]
<�޸���>������ʤ�����ı�
<��ǰ�汾>��[V3.2.0]
<��ϸ��¼>��
1��	<����>�������޸Ĳ��ֺ�����ʵ�֣��������USB��֧��
	<ԭ��>��
	<ע��>�������й��첽����Ӳ�������Ϸ��Ĳ��֣�ʹ��ʱ��Ҫע����õķ��������Բο�
			WOSA/SP2.0��SIU��SP���롣
			����ODevBase��virtual����vOnDataReceived�������̳���ODevBase�����಻�������أ�
			����Ϊvirtual������ֻΪ������ǰ����
</MCREC>
============================��ʷ�޸ļ�¼��=============================
</ModiControl>						
*/
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODEVBASE_H__8BBC195A_112D_11D4_B865_5254AB1A7D30__INCLUDED_COM_)
#define AFX_ODEVBASE_H__8BBC195A_112D_11D4_B865_5254AB1A7D30__INCLUDED_COM_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ������
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

// ͨ�ö���
//#define SUCCESS					0	// �ɹ�
//#define FAIL					1	// ʧ��

#define ERRORFLAG_CLEAR			0	// ������󻺴���

// ��־��Ϣ���
#define CATALOG_FATAL			0	// ��������
#define	CATALOG_NORMAL			1	// һ����Ϣ
#define	GATALOG_NOTE			2	// ��ʾ��Ϣ

// MSG type
#define MSG_GENERAL				1	// һ����Ϣ
#define	MSG_COMM_SEND			2	// ��������
#define	MSG_COMM_REV			3	// ��������

//==ODevBase==BEGIN==ERROR==DEFINE=======================================================================
#define E_DEVBASE_BASE				    	900

#define E_DEVBASE_HOLD				E_DEVBASE_BASE+10	// 910 ������ֵ
#define E_DEVBASE_COMMANAGER		E_DEVBASE_BASE+11	// 911 GrgComManger�������ʧ��
#define E_DEVBASE_CFGMANAGER		E_DEVBASE_BASE+12	// 912 GrgCfgManager�������ʧ��
#define E_DEVBASE_LOGSERVER			E_DEVBASE_BASE+13	// 913 GrgLogServer�������ʧ��
#define E_DEVBASE_COMMLOGINIT		E_DEVBASE_BASE+14	// 914 commlog�����ʼ��ʧ��
#define E_DEVBASE_TRACELOGINIT		E_DEVBASE_BASE+15	// 915 tracelog�����ʼ��ʧ��

#define E_DEVBASE_COMCFGFILE		E_DEVBASE_BASE+16	// 916 ��GRGDTATM_CommCfg.ini�ļ�ʧ��
#define E_DEVBASE_COMID				E_DEVBASE_BASE+17	// 917 ��ȡ����ComIDʧ��
#define E_DEVBASE_COMMTYPE			E_DEVBASE_BASE+18	// 918 ��ȡ����CommTypeʧ��
#define E_DEVBASE_COMBAUD			E_DEVBASE_BASE+19	// 919 ��ȡ����ComBaudʧ��
#define E_DEVBASE_COMBOARDPORT		E_DEVBASE_BASE+20	// 920 ��ȡ����ComBoardPortʧ��
#define E_DEVBASE_COMBOARDPORTBAUD	E_DEVBASE_BASE+21	// 921 ��ȡ����ComBoardPortBaudʧ��
#define E_DEVBASE_DEVCOMMLOGID		E_DEVBASE_BASE+22	// 922 ��ȡ����DevCommLogIDʧ��
#define E_DEVBASE_DEVTRACELOGID		E_DEVBASE_BASE+23	// 923 ��ȡ����DevTraceLogIDʧ��
#define E_DEVBASE_INICFGFILENAME	E_DEVBASE_BASE+24	// 924 ��IniCfgFileName�����õ��ļ�ʧ��
#define E_DEVBASE_LOGICNAMEISNULL	E_DEVBASE_BASE+25	// 925 �豸���߼��豸��Ϊ��
#define E_DEVBASE_USBSENDPIPE		E_DEVBASE_BASE+26	// 926 ��ȡ����SendPipesʧ��
#define E_DEVBASE_USBRECVPIPE		E_DEVBASE_BASE+27	// 927 ��ȡ����RecvPipesʧ��
#define E_DEVBASE_USBDEVICEGUID		E_DEVBASE_BASE+28	// 928 ��ȡ����DeviceGUIDʧ��
#define E_DEVBASE_TCPIPADDRESS		E_DEVBASE_BASE+29	//��ȡIP��ַʧ��
#define E_DEVBASE_TCPIPSENDPORT		E_DEVBASE_BASE+30	//��ȡ���Ͷ˿�ʧ��
#define E_DEVBASE_TCPIPRECVPORT		E_DEVBASE_BASE+31	//��ȡ���ն˿�ʧ��

//==ODevBase==END==ERROR==DEFINE=======================================================================
//==============================================================
#define TCPIP_ADDRESS_LENGTH	50
#define USB_PIPES_LENGTH		50
#define USB_EXTENDSTR_LENGTH	256

typedef struct
{
	int		iLogicCode;			// �߼�������
	int		iPhyCode;			// ���������
	int		iHandle;			// ��������0-������ 1-��ʼ�� 2-�ط�����
	int		iType;				// �������ͣ�0-���� 1-����

	char	acDevReturn[128];	// Ӳ��������Ϣ
	char	acReserve[128];		// ������Ϣ
} tDevReturn;

typedef struct
{
	// ͨѶ���ò���
	int iCommType;				// ͨѶ����0:����ͨѶ��1:���ڰ�ͨѶ��2:USB, 3:����
	int	iComBoardDevPort;		// ʹ�ô��ڰ�Ķ˿ں�
	int iComBoardDevPortBaud;	// ʹ�ô��ڰ�˿ڵĲ�����
	int iComId;					// ����ID��
	int iBaudRate;				// ���ڲ�����

	//���USB�Ĳ���
	GUID	sInterfaceGuid;						//�豸�ӿ�GUID
	char	acSendPipes[USB_PIPES_LENGTH];		//���͹ܵ���
	char	acRecvPipes[USB_PIPES_LENGTH];		//���չܵ���
	char	acExtendStr[USB_EXTENDSTR_LENGTH];	//��չ��
	int		iReserve;							//����ֵ
	
	// ��־���ò���
	CString strDevCommLogID;	// Comm LOG����Section
	CString strDevTraceLogID;	// Trace LOG����Section
}tDevCfgInfo;

typedef	void (*PROCFUN)(int p_iType, WPARAM p_pData, int p_iLen);
typedef	void (*PROCFUN2)(int p_iType, WPARAM p_pData, int p_iLen, const void* p_pvParam);

class DEVBASEDLL_API ODevBase  
{
private:
	tDevReturn		m_sDevErr;					// ���������ṹ
	char			m_acTrace[200];				// ��־��Ϣ����ʱ����: obsolete
	bool			m_bBreakAsynRecv;			// �ж��첽ͨѶ���յķ��ͣ����ٽ��¼����͸�����������
	map<IComm*, LPVOID> m_tmSinkObjects;		// ���ڽ����豸ͨѶ����������¼�
	IComm*			m_poComManager;				// ����ͨѶ���
	ILogServer*		m_poCommLogServer;			// comm��־���
	ILogServer*		m_poTraceLogServer;			// trace��־���
	ICfgAccess*		m_poCfgManager;				// �������

	tDevCfgInfo		m_sDevCfgInfo;				// ͨѶ������Ϣ

	PROCFUN			m_pfProcFun;				// �ϲ�ص�����ָ��
	PROCFUN2		m_pfProcFun2;
	void*			m_pSinkObject;				// �ⲿ����ص����������һ��ָ�����

public:
	char			m_acVersion[32];			// �����汾��Ϣ
	char			m_acDevVersion[32];			// Ӳ���汾��Ϣ 
	char			m_acLogicalDevName[32];		// �豸�߼���ʶ����  

public:    
	// ������Ϣ����
	int iGetIntValue(const char *p_pcSection, const char *p_pcKeyName, int &p_iKeyValue);
	int iGetStringValue(const char *p_pcSection, const char *p_pcKeyName, CString &p_strKeyValue);
    int iSetIntValue(const char *p_pcSection, const char *p_pcKeyName, long p_lKeyValue);
    int iSetStringValue(const char *p_pcSection, const char *p_pcKeyName, CString p_strKeyValue);
	int iDeleteSection(const char* p_pcSection);
	int iDeleteKey(const char* p_pcSection, const char* p_pcKeyName);

	// ͨѶ���������ӿ�
	int iPreInitDev();								
	int iGetDefaultCommConfig(tCommCfg& p_psCommCfg);	
	int iInitComm(tCommCfg p_sCommCfg, bool p_bEnableEvents = false);
	int iCloseComm();
  
	// ���ͺͽ�������ӿ�
	int iSendCommand(const unsigned char* p_pcCommand, unsigned int p_wLength, 
					 bool p_bClearRecvBuf = true, bool p_bLog = true);
	int iWaitResult(unsigned char* p_pcData, unsigned int& p_uiLength, unsigned int p_uiTime, 
					bool p_bLog = true);
	int	iSendAndRecvData(const unsigned char* p_pcSendData, const unsigned int p_iLenSend, 
						unsigned char* p_pcRevData, unsigned int& p_uiLenRev, unsigned int p_uiTime,
						bool p_bClearRecvBuf = true, bool p_bLog = true);
	
	// ���ͽ������ݼ�������ռ���ʶ˿�
	int iEnterExclusiveComm(int p_iTime);
	int iLeaveExclusiveComm(int p_iReserve = 0);

	// �첽����ͨѶ����ʱ���ص��ϲ�ص�����
	virtual void vCallProcFun(int p_iType, char* p_pcData, int p_iLen, const void* p_pvParam = NULL);

	// ���ñ�־���ж��첽ͨѶ���յķ��ͣ����ٽ��¼����͸�����������
	virtual void vSetBreakAsynRecvFlag(bool p_bBreakAsynRecv);
	virtual bool bGetBreakAsynRecvFlag();

public:
	// ��չ��ͨѶ����
	int	iExtendCommFunc(DWORD p_dwFunc);  

	// ����ͨѶ������¼���ʽ������Ϣ���
	int	iEnableRevEvent(bool p_bEnableEvent); // ����commanager���¼���ʽ������Ϣ���
	
	// �����ϲ�ص�����ָ��
	void vSetProcFun(const PROCFUN p_pvProcFun);
	// �����ϲ�ص�����ָ�룬�ڶ�������Ϊ�ⲿ����ص����������һ��ָ�����������Ϊ�����ָ��
	void vSetProcFun2(const PROCFUN2 p_pvProcFun, const void* p_pSinkObject = NULL);
	
	// �����������߼��豸��
	void vSetLogicalDevName(const char* p_pcLogicalDevName);	// �����������߼��豸��

	// ������ջ�����
	void vClearRecvBuf();

	// д��־������һ��ĳ����¼ֻ�踳ֵǰ��������, ��ֻ����һ��ֵ��
    // p_bBinaryData��ʾ�Ƿ�Ϊ���������ݡ�����ǣ�����Դ������ַ���������'\0'
	int	iWriteLog(const char* p_pcData, const unsigned int p_uiLen = 0, int	p_iInfoCatalog = GATALOG_NOTE, 
				  int p_iMSGType = MSG_GENERAL, bool p_bBinaryData = false);


	// Ӳ�������־����ӿ�
	int			iGetErrorFlag();							// ��ȡӲ�������־
	tDevReturn	tGetErrorFlag();							// ��ȡӲ�������־
	void		vSetErrorFlag(const int p_iFlag);			// ����Ӳ�������־
	void		vSetErrorFlag(const tDevReturn p_sDevErr);	// ����Ӳ�������־

	// ���º������ṩ�������Լ���ʵ��	
	virtual int	iGetVersion(char *p_pcVersion);			// ȡ��������汾��
	virtual int	iGetDevVersion(char *p_pcVersion);		// ȡӲ���汾��

	// �豸����ͨ�ýӿ�
	virtual int	iGetStatus(tDevReturn* p_sDevReturn);	
	virtual int	iInit(tDevReturn* p_sDevReturn);	
	virtual int	iTest(tDevReturn* p_sDevReturn);	

	// �����첽ͨѶ����
	virtual	void vOnDataReceived(char *p_pcData, int p_iLen);

	//2005-05-08����ӿ�
	int iInitComm2(tCommCfg2 p_sCommCfg, bool p_bEnableEvents = false);
	int iGetDefaultCommConfig2(tCommCfg2& p_sCommCfg);
	// ������ջ�����
	void vClearRecvBuf2(short p_shPipe = -1);
	//����
	int iSendCommand2(const unsigned char* p_pcCommand, unsigned int p_wLength, 
		short p_shSendPipe, bool p_bClearRecvBuf = true, bool p_bLog = true);
	//����
	int iWaitResult2(unsigned char* p_pcData, unsigned int& p_uiLength, unsigned int p_uiTime, 
		short p_shRecvPipe, bool p_bLog = true);
	//���Ͳ�����
	int	iSendAndRecvData2(const unsigned char* p_pcSendData, const unsigned int p_iLenSend, 
		unsigned char* p_pcRevData, unsigned int& p_uiLenRev, unsigned int p_uiTime,
		short p_shSendPipe, short p_shRecvPipe, 
		bool p_bClearRecvBuf = true, bool p_bLog = true);
	//��ռ����
	int iEnterExclusiveComm2(int p_iTime, short p_shPipe);
	//�˳���ռ
	int iLeaveExclusiveComm2(int p_iReserve, short p_shPipe);
	//�ر�CommͨѶ
	int iCloseComm2();
	ODevBase();
	virtual ~ODevBase();

private:
	void vReleaseComm();// �ͷű����COM���
	void trace(LPCTSTR Format, ...);
};

#endif // !defined(AFX_ODEVBASE_H__8BBC195A_112D_11D4_B865_5254AB1A7D30__INCLUDED_COM_)
