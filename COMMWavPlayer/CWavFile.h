#pragma once

/**
  @brief 处理波形文件的类
*/
#include <MMSystem.h>
class CWavFile
{
public:
	CWavFile();
	~CWavFile();
public:
	long			load(const TCHAR* tcPath);				//加载波形文件资源	包括读取语音数据，获取波形格式，数据大小，语音播放时长
	UINT		detectDuration(const TCHAR* tcPath);	//检测波形文件播放时长 数据字节数/每秒钟播放字节数
	void			free();									//释放语音数据
public:
	char*			m_pWavData;		//指向语音数据
	WAVEFORMATEX	m_waveFormat;	//Wav文件文件头
	DWORD			m_dwDataSize;	//Wav文件语音数据大小 （字节）
	UINT			m_uiPlayTime;	//Wav文件播放时长		（秒）
};