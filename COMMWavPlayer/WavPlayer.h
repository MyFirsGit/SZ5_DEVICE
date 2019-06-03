#pragma once

#ifdef WAVPLAYER_EXPORTS
#define WAV_PLAYER __declspec(dllexport)
#else
#define WAV_PLAYER __declspec(dllimport)
#endif


extern "C" WAV_PLAYER long addWave(const TCHAR* tcPath);
extern "C" WAV_PLAYER long clearWaves();
extern "C" WAV_PLAYER long playWave();
extern "C" WAV_PLAYER long stopWave();
extern "C" WAV_PLAYER long pauseWave();
extern "C" WAV_PLAYER long resumeWave();
extern "C" WAV_PLAYER unsigned long detectDuration();