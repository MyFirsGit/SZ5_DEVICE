#pragma once
#include <mmsystem.h>
#include <vector>

typedef enum _UE_PLAYSTATUS
{
	UE_PLAYING=0,
	UE_STOP,
	UE_PAUSE
}UE_PLAYSTATUS;

/**
  @brief 播放语音接口实现
*/
class CWavePlayImp
{
public:
	CWavePlayImp();
	~CWavePlayImp();
public:
	long pausWave();		//暂停语音播放
	long resumeWave();		//恢复语音播放，从上次播放位置开始播放
	long stopWave();		//停止语音播放

	long addWaveItem(const TCHAR* tcPath);	//添加语音文件到播放列表中
	long clearWaveItem();					//清空当前播放列表
	long playWaveItems();					//播放播放列表中的所有语音

	UINT detectDuration();	//检测语音播放时长
public:
	std::vector<const TCHAR*>					m_vector_waveItems;
	std::vector <const TCHAR*>::iterator 		m_iterator_waveIter;
};

