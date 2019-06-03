// WavPlayer.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "WavPlayer.h"
#include "WavePlayImp.h"
CWavePlayImp g_waveImp;

WAV_PLAYER long addWave(const TCHAR* tcPath)
{
	return g_waveImp.addWaveItem(tcPath);
}

WAV_PLAYER long clearWaves()
{
	return g_waveImp.clearWaveItem();
}

WAV_PLAYER long playWave()
{
	return g_waveImp.playWaveItems();
}

WAV_PLAYER long stopWave()
{
	return g_waveImp.stopWave();
}

WAV_PLAYER long pauseWave()
{
	return g_waveImp.pausWave();
}

WAV_PLAYER long resumeWave()
{
	return g_waveImp.resumeWave();
}

WAV_PLAYER unsigned long detectDuration()
{
	return g_waveImp.detectDuration();
}