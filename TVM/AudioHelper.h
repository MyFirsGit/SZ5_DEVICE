#pragma once
#include "mmsystem.h"
#include "commondefine.h"
#include "ServiceHelper.h"

#include <map>
#include <list>
#import "msxml6.dll"

//這個沒用 暫時爲了編譯不報錯
#define theAudio_HELPER CEmptyAudioHelper::GetInstance()

class CEmptyAudioHelper
{
public:
	static CEmptyAudioHelper& GetInstance();
	void PlayAudio(AUDIO_ID)
	{
		return;
	}
private:
	CEmptyAudioHelper()
	{}
	~CEmptyAudioHelper(){}
	static CEmptyAudioHelper thEmptyAudioHelper;
};

/**
@brief  语音播放控制管理类
*/

extern UINT g_uiDuration;

class CAudioHelper
{
public:
    CAudioHelper(void);
    ~CAudioHelper();
	void playEventAudio(DWORD dwEventID,UINT& uiDuration=g_uiDuration);
	void getKeyMap(std::map<int, DWORD>& mapKeyValue, DWORD dwEventID);
	void StopPlayEventAudio();
private:
	
	TCHAR	m_tcXmlPath[MAX_PATH];
	MSXML2::IXMLDOMDocumentPtr m_pDoc;
	MSXML2::IXMLDOMNodeListPtr m_pEvents;
	
	void	m_loadXml(const TCHAR* tcpath);
	void	m_getEventWavFiles(std::list<CString>& listWavFiles, DWORD dwEventID,LANGUAGETYPE_T lantype);
	void	m_getLanTypeWavFiles(MSXML2::IXMLDOMNodeListPtr& pVoiceChildren,LANGUAGETYPE_T lantype,std::list<CString>& listWavFiles);
	CString m_getVariableValue(const TCHAR* tcVariableType, DWORD dwCode, LANGUAGETYPE_T lantype);
	CString m_strCurDirectory;
protected:
	virtual DWORD	getCode(const _TCHAR* tcVariableType);
	virtual BOOL	getDegrade(const _TCHAR* tcDegradeVriableName);
};

class CIssueAudioHelper:public CAudioHelper,public CServiceHelper
{
public:
	CIssueAudioHelper(CService& service);
	~CIssueAudioHelper();
protected:
	virtual DWORD	getCode(const _TCHAR* tcVariableType);
	virtual BOOL	getDegrade(const _TCHAR* tcDegradeVriableName);
};

class CChargeAudioHelper:public CAudioHelper,public CServiceHelper
{
public:
	CChargeAudioHelper(CService& service);
	~CChargeAudioHelper();
protected:
	virtual DWORD	getCode(const _TCHAR* tcVariableType);
	virtual BOOL	getDegrade(const _TCHAR* tcDegradeVriableName);
};