#pragma once
#include <vector>
#include "scmessagecontentdefine.h"
#include "versioninfo.h"
#include "sccommand.h"

using namespace std;

#define  theUPGRADE_HELPER CUpgradeHelper

static const char* const STR_BOM_MC_00  = "BOM_MC_00";	// BOM����
static const char* const STR_BOM_TC_00  = "BOM_TC_00";	// THӦ�ó���
static const char* const STR_BOM_TC_01  = "BOM_TC_01";	// TH���س���
static const char* const STR_BOM_RW_00  = "*4109*";		// RWӦ�ó���
static const char* const STR_BOM_RW_01  = "*4110*";		// RW���س���
static const char* const STR_TVM_TH_APP	= "*4111*";		// TVM ��THӦ�ó���

class  CUpgradeHelper
{

public:

	typedef enum  _UpgradeProcess
	{
		BEGIN = 0x0001,
		SUCCEED = 0x0002,
		FAILED = 0x0004
	} UPGRADE_PROCESS ;

	static CString m_strCurrentDirectory;

	//����������֪ͨ���������塣
	typedef void (*OnUpgradeProcessFunc)(CVersionInfo::PROG_TYPE,UPGRADE_PROCESS);


	//****************************************************************************
	// 
	// function  : ���������ļ�
	// input:    vector<ParameterAndSoftwareUpdate> downLoadFiles ��Ҫ���ص������ļ����б�
	// output:   ��   
	// return:	  true ���سɹ�
	//			  false ����ʧ��
	//****************************************************************************
	static bool DownLoadFiles(vector<ParameterAndSoftwareUpdate> downLoadFiles);

	//****************************************************************************
	// 
	// function  : �ƶ����ص�������������Ŀ¼
	// input:     ��
	// output:   ��   
	// return:	  true �����ɹ�
	//			  false ����ʧ��
	//****************************************************************************
	static bool MoveDownloadFileToUpgradeFolder();

	//****************************************************************************
	// 
	// function  : ���BOM�����������Ƿ���Ҫ����
	// input:     ��
	// output:   ��   
	// return:	  true ��Ҫ����
	//			  false ����Ҫ����
	//****************************************************************************
	static bool CheckDownloadFileNeedUpgrade();
	
	//****************************************************************************
	// 
	// function  : ��������
	// input:     ��������֪ͨ������
	// output:   ��   
	// return:	 
	//			 
	//****************************************************************************
	static bool Upgrade(OnUpgradeProcessFunc func);


	//****************************************************************************
	// 
	// function  : ����Ĭ��֪ͨ��������������
	// input:     ��
	// output:   ��   
	// return:	 
	//			 
	//****************************************************************************
	static bool Upgrade();

	//****************************************************************************
	// 
	// function  : ���ø���֪ͨ�����������ض��ĳ���
	// input:     CVersionInfo::PROG_TYPE& progType Ҫ�����ĳ������͡�
	// input:     OnUpgradeProcessFunc func ֪ͨ��������
	// output:      
	// return:	 
	//			 
	//****************************************************************************
	static bool Upgrade(CVersionInfo::PROG_TYPE progType,OnUpgradeProcessFunc func);

	//****************************************************************************
	// 
	// function  : ����Ĭ��֪ͨ�����������ض��ĳ���
	// input:     CVersionInfo::PROG_TYPE& progType Ҫ�����ĳ������͡�
	// output:      
	// return:	 
	//			 
	//****************************************************************************
	static bool Upgrade(CVersionInfo::PROG_TYPE progType);
	//****************************************************************************
	// 
	// function : ��ô�������ļ���Ŀ¼��
	// input:    PARAMETER_ID& progType ��������
	// input:   PARAM_VERSION_TYPE_CODE& verCode �����İ汾���͡�      
	// return:	  CString ����·��
	//			  
	//****************************************************************************
	__inline static CString GetDownloadFolder(__in PARAMETER_ID& progType,__in  PARAM_VERSION_TYPE_CODE& verCode);

	//****************************************************************************
	// 
	// function : ����Voice and Templateģ���ļ�
	// input:    
	// input:         
	// return:	  bool true���ɹ�  false��ʧ��
	//			  
	//****************************************************************************
	static bool UpdateVoiceAndTemplateFolder();

	static bool CheckTPUNeedUpgrade();
private:

	//****************************************************************************
	// 
	// function  : ������������ʼ���ɹ���ʧ��ͨ���Ĵ�����
	// input:     CVersionInfo::PROG_TYPE& progType ��������
	// input:     UPGRADE_PROCESS& process ֪ͨ����
	// output:    ��       
	// return:	  ��
	//			 
	//****************************************************************************
	static void OnUpgrade(CVersionInfo::PROG_TYPE progType,UPGRADE_PROCESS process);

	//****************************************************************************
	// 
	// function  : ���������ͳ����Ƿ���Ҫ����
	// input:     CVersionInfo::PROG_TYPE progType ��������
	// output:    CVersionInfo::VER_TYPE& verType �����İ汾���͡�         
	// return:	  true ��Ҫ�����������汾��verType����
	//			  false ����Ҫ����
	//****************************************************************************
	static bool CheckDownloadFileNeedUpgrade(__in CVersionInfo::PROG_TYPE progType,__out CVersionInfo::VER_TYPE& verType);

	
	//****************************************************************************
	// 
	// function  : �ƶ����ص�BOM����������������Ŀ¼
	// input:    ��
	// output:   ��         
	// return:	  true �ɹ�
	//			  false ʧ��
	//****************************************************************************
	static bool MoveDownloadFileToUpgradeFolderMC();

	//****************************************************************************
	// 
	// function  : �ƶ����ص�GUI�ļ�������Ŀ¼
	// input:    ��
	// output:   ��         
	// return:	  true �ɹ�
	//			  false ʧ��
	//****************************************************************************
	static bool MoveDownloadFileToUpgradeFolderGUI();

	//****************************************************************************
	// 
	// function  : �ƶ����ص���·վ�㳵վ�ļ�������Ŀ¼
	// input:    ��
	// output:   ��         
	// return:	  true �ɹ�
	//			  false ʧ��
	//****************************************************************************
	static bool MoveDownloadFileToUpgradeFolderStationMap();

	//****************************************************************************
	// 
	// function  : �ƶ����ص�TPU����������������Ŀ¼
	// input:    ��
	// output:   ��         
	// return:	  true �ɹ�
	//			  false ʧ��
	//****************************************************************************
	static bool MoveDownloadFileToUpgradeFolderTPU();


	//****************************************************************************
	// 
	// function  : �ƶ����ص�TH����������������Ŀ¼
	// input:    ��
	// output:   ��         
	// return:	  true �ɹ�
	//			  false ʧ��
	//****************************************************************************
	static bool MoveDownloadFileToUpgradeFolderTH();


	//****************************************************************************
	// 
	// function  : ��鵱ǰ�汾�ļ��кͽ����汾�ļ����������ļ��Ƿ��ܹ�����
	// input:     CString& currentVersionFolder ��ǰ�汾�����ļ���ŵ��ļ��� 
	//			  CString& futureVersionFolder  �����汾�����ļ���ŵ��ļ���
	// output:    CVersionInfo::VER_TYPE& verType �����İ汾���͡�         
	// return:	  true ��Ҫ�����������汾��verType����
	//			  false ����Ҫ����
	//****************************************************************************
	static bool CheckDownloadFileNeedUpgrade(__in CString& currentVersionFolder,__in CString& futureVersionFolder,__out CVersionInfo::VER_TYPE& verType);

	//****************************************************************************
	// 
	// function : �������������ļ���������Чʱ���Ƿ����á�
	// input:     ParameterAndSoftwareUpdate& ParameterAndSoftwareUpdate ���������ࡣ
	// output:    ��       
	// return:	  true �ѹ���Чʱ�䣬Ӧ������
	//			  false δ����Чʱ�䣬��Ӧ������
	//****************************************************************************
	static bool CheckDownloadFileUpgradeDateHasStart(__in const ParameterAndSoftwareUpdate& ParameterAndSoftwareUpdate);

	//****************************************************************************
	// 
	// function : ���ĳ����Ŀ¼�������ļ��Ƿ��ܹ����á�
	// input:     CString& upgradeDataFolder �����ļ���ŵ�Ŀ¼��
	// output:    ParameterAndSoftwareUpdate& ParameterAndSoftwareUpdate ����������       
	// return:	  true �ѹ���Чʱ�䣬Ӧ������
	//			  false δ����Чʱ�䣬��Ӧ������
	//****************************************************************************
	static bool CheckDownloadFileUpgradeDateHasStart(__in const CString& upgradeDataFolder,__out ParameterAndSoftwareUpdate& ParameterAndSoftwareUpdate);

	//****************************************************************************
	/**
	@brief      ��ѹ��������°�

	@param      (i)CString sProgramFileUpdateDir   ѹ���ļ�·��
	@param      (i)CString sFileName			   ѹ���ļ���
	@param      (i)bool isAPP					   �Ƿ���BOM������

	@retval     bool

	@exception  ��
	*/
	//****************************************************************************
	static bool ExtractUpdateFileToCurrentFolder(CString sProgramFileUpdateDir,CString sFileName,bool isAPP);

	//****************************************************************************
	/**
	@brief      ��ѹ��������°�

	@param      (i)CString sProgramFileUpdateDir   ѹ���ļ�·��
	@param      (i)CString sFileName			   ѹ���ļ���
	@param      (i)bool isAPP					   �Ƿ���BOM������

	@retval     bool

	@exception  ��
	*/
	//****************************************************************************
	static bool ExtractUpdateFile(CString sFilePath, CString targetFolder, bool isAPP);

	//****************************************************************************
	// 
	// function : ��ô�����س��������ļ���Ŀ¼��
	// input:     CVersionInfo::PROG_TYPE progType ��������
	// input:    CVersionInfo::VER_TYPE& verType �����İ汾���͡�      
	// return:	  CString ����·��
	//			  
	//****************************************************************************
	__inline static CString GetDownloadFolder(__in  CVersionInfo::PROG_TYPE& progType,__in CVersionInfo::VER_TYPE& verType);




	//****************************************************************************
	// 
	// function : ��������ļ���ŵ�Ŀ¼��
	// input:     CVersionInfo::PROG_TYPE progType �������� 
	// output:	 vector<CString>& updateFolders �����ļ���ŵ�Ŀ¼
	// return:	  ��
	//			  
	//****************************************************************************
	__inline static void GetUpgradeFolderFromDownloadProgramType(__in CVersionInfo::PROG_TYPE& progType,__out vector<CString>& updateFolders);


	//****************************************************************************
	// 
	// function : ��������ļ���ŵ�Ŀ¼��
	// input:     CVersionInfo::PROG_TYPE progType �������� 
	// output:	 vector<CString>& updateFolders �����ļ���ŵ�Ŀ¼
	// return:	  ��
	//			  
	//****************************************************************************
	__inline static void GetUpgradeFolder(__in CVersionInfo::PROG_TYPE& progType,__out vector<CString>& updateFolders);


	//****************************************************************************
	// 
	// function : ��������ļ���
	// input:     CVersionInfo::PROG_TYPE progType ��������
	// input:    CVersionInfo::VER_TYPE& verType �����İ汾���͡� 
	// output:	 vector<CString>& upgradeFiles �����ļ�
	// return:	  ��
	//			  
	//****************************************************************************
	__inline static void GetDownloadUpgradeFiles(__in  CVersionInfo::PROG_TYPE& progType,__in CVersionInfo::VER_TYPE& verType,__out vector<CString>& upgradeFiles);

	//****************************************************************************
	// 
	// function : ��VersionCodeת��ΪVersionType��
	// input:	 const PARAM_VERSION_TYPE_CODE& verCode
	// output:	 CVersionInfo::VER_TYPE& verType
	// return:   bool �ɹ�ת��������ת�����ɹ��� 
	//			  
	//****************************************************************************
	__inline static bool TryConvertVerCodeToVerType(__in const PARAM_VERSION_TYPE_CODE& verCode,__out CVersionInfo::VER_TYPE& verType);

	//****************************************************************************
	// 
	// function : ��PARAMETER_IDת��ΪPROG_TYPE��
	// input:	 const PARAMETER_ID& praameterID
	// output:	 CVersionInfo::PROG_TYPE& progType
	// return:   bool �ɹ�ת��������ת�����ɹ��� 
	//			  
	//****************************************************************************
	__inline static bool TryConvertParameterIDToProgarmType(__in const PARAMETER_ID& praameterID,__out CVersionInfo::PROG_TYPE& progType);

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ����Ŀ¼
	@param      none
	@retval     none
	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	static BOOL CopyDirectory(LPCTSTR lpszSourceDir, LPCTSTR lpszDestDir);
	static BOOL RecursiveFileList(LPCTSTR lpszPath, LPCTSTR lpszOrgPath, LPCTSTR lpszDestDir, BOOL bIsFirst);
	static CString RenServerName2LocalName(CString sServerFileName);
};