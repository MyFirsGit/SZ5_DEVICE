#include "stdafx.h"
#include "WavePlayImp.h"
#include <tchar.h>
#include <malloc.h>
#include <process.h>
#include "CWavFile.h"

#include <MMSystem.h>
#pragma comment(lib,"winmm.lib")

#define UD_BUFFERPLAYED	WM_USER+100
#define UD_STOPPLAY		WM_USER+101
#define UD_PAUSE		WM_USER+102
#define UD_RESUME		WM_USER+103

HANDLE				g_hPlayThread;
UINT				g_uiPlayThread;
UE_PLAYSTATUS		g_playStatus;
HANDLE				g_msgCompleteEvent;
HANDLE				g_stopCompleteEvent;
CRITICAL_SECTION	g_cs_vector;


CWavePlayImp::CWavePlayImp()
{
	g_hPlayThread=NULL;
	g_uiPlayThread=0;

	TCHAR tcStopEvent[1024];
	_stprintf_s(tcStopEvent,1024,_T("%d_StopComplete"),(unsigned)this);
	g_stopCompleteEvent=CreateEvent(NULL,FALSE,FALSE,tcStopEvent);

	TCHAR tcEventName[1024];
	_stprintf_s(tcEventName,1024,_T("%d_MsgQueneComplete"),(unsigned)this);
	g_msgCompleteEvent=CreateEvent(NULL,FALSE,FALSE,tcEventName);

	InitializeCriticalSection(&g_cs_vector);
}


CWavePlayImp::~CWavePlayImp()
{
	clearWaveItem();
	if(NULL!=g_hPlayThread)
	{
		CloseHandle(g_hPlayThread);
		g_hPlayThread=NULL;
	}
	if(NULL!=g_msgCompleteEvent)
	{
		CloseHandle(g_msgCompleteEvent);
		g_msgCompleteEvent=NULL;
	}
	DeleteCriticalSection(&g_cs_vector);
}


long CWavePlayImp::pausWave()
{
	long lret = 1;
	BOOL bPost=PostThreadMessage(g_uiPlayThread,UD_PAUSE,0,0);
	if(bPost==TRUE)
	{
		lret=0;
		//::MessageBox(NULL,L"消息发送失败",L"消息发送失败",MB_OK);
	}
	return lret;
}

long CWavePlayImp::resumeWave()
{
	long lret = 1;
	BOOL bPost=PostThreadMessage(g_uiPlayThread,UD_RESUME,0,0);
	if(bPost==TRUE)
	{
		lret=0;
		//::MessageBox(NULL,L"消息发送失败",L"消息发送失败",MB_OK);
	}
	return lret;
}

long CWavePlayImp::stopWave()
{
	long lret = 1;
	BOOL bPost=PostThreadMessage(g_uiPlayThread,UD_STOPPLAY,0,0);
	if(bPost==TRUE)
	{
		lret=0;
		//:MessageBox(NULL,L"消息发送失败",L"消息发送失败",MB_OK);
		WaitForSingleObject(g_stopCompleteEvent,INFINITE);
	}
	return lret;
}

long CWavePlayImp::addWaveItem(const TCHAR* tcPath)
{
	long lret = 1;
	TCHAR* pNewPath=(TCHAR*)_aligned_malloc(MAX_PATH*sizeof(TCHAR),16);
	if(NULL!=pNewPath)
	{
		_tcscpy_s(pNewPath,MAX_PATH,tcPath);
		
		m_vector_waveItems.push_back(pNewPath);
		
	}else
	{
	}
	return lret;
}

long CWavePlayImp::clearWaveItem()
{
	long lret = 1;
	std::vector <const TCHAR*>::iterator it;
	for(it=m_vector_waveItems.begin();
		it!=m_vector_waveItems.end();
		it++)
	{
		TCHAR* pNewPath=(TCHAR*)*it;
		_aligned_free(pNewPath);
		pNewPath=NULL;
	}
	m_vector_waveItems.clear();
	return lret;
}

void CALLBACK waveOutProc(
  HWAVEOUT hwo,
  UINT uMsg,
  DWORD dwInstance,
  DWORD dwParam1,
  DWORD dwParam2
)
{
	switch(uMsg)
	{
	case MM_WOM_DONE:
		{
			BOOL bPost=PostThreadMessage(g_uiPlayThread,UD_BUFFERPLAYED,0,0);
			if(bPost==TRUE)
			{
				//::MessageBox(NULL,L"消息发送失败",L"消息发送失败",MB_OK);
			}
		}
		break;
	default:
		break;
	}
}

long openDevie(HWAVEOUT& hWavOut,WAVEFORMATEX* lpFormat)
{
	long lret=1;
	//检查音频设备，返回音频输出设备的性能
	WAVEOUTCAPS pwoc;
	if (waveOutGetDevCaps(WAVE_MAPPER, &pwoc, sizeof(WAVEOUTCAPS)) == 0)
	{

		if (waveOutOpen(&hWavOut, WAVE_MAPPER, lpFormat, (DWORD)waveOutProc,NULL, CALLBACK_FUNCTION) ==
			0)
		{
			lret=0;
		}else
		{
		}
	}else
	{
	}
	return lret;
}

long playDevie(HWAVEOUT hWavOut,WAVEHDR& wavHdr)
{
	long lret=1;
	if (waveOutPrepareHeader(hWavOut, &wavHdr, sizeof(WAVEHDR)) == 0)
	{
		if(waveOutWrite(hWavOut, &wavHdr, sizeof(WAVEHDR)) == MMSYSERR_NOERROR)
		{
			//将数据写入设备并开始播放
			lret = 0;
		}else
		{
		}
	}else
	{
	}
	return lret;
}

long stopDevice(HWAVEOUT hWavOut,WAVEHDR& wavHdr)
{
	long lret=1;

	if (MMSYSERR_NOERROR == waveOutReset(hWavOut))
	{
		if (MMSYSERR_NOERROR == waveOutUnprepareHeader(hWavOut, &wavHdr, sizeof(WAVEHDR)))
		{
			hWavOut=NULL;
			lret=0;
		}else
		{
		}
	}else
	{
	}

	return lret;
}

long closeDevice(HWAVEOUT& hWaveOut)
{
	long lret=1;
	if(MMSYSERR_NOERROR == waveOutClose(hWaveOut))
	{
		hWaveOut=NULL;
		lret=0;
	}
	return lret;
}

long pauseDevice(HWAVEOUT hWaveOut)
{
	return waveOutPause(hWaveOut);
}

long resumeDevice(HWAVEOUT hWaveOut)
{
	return waveOutRestart(hWaveOut);
}

unsigned __stdcall playList(void* lpParam)
{
	CWavePlayImp* pImp=(CWavePlayImp*)lpParam;
	if(NULL!=pImp)
	{
		//创建线程消息队列   
		MSG msg;
		PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

		SetEvent(g_msgCompleteEvent);
		CWavFile wavFile;
		EnterCriticalSection(&g_cs_vector);
		std::vector<const TCHAR*> vec;
		std::vector <const TCHAR*>::iterator itcopy;
		for(itcopy=pImp->m_vector_waveItems.begin();itcopy!=pImp->m_vector_waveItems.end();itcopy++)
		{
			size_t uiBufferLen=sizeof(TCHAR)*(_tcslen(*itcopy)+1);
			//TCHAR* pThreadPath=(TCHAR*)_aligned_malloc(uiBufferLen,16);
			TCHAR* pThreadPath=new TCHAR[uiBufferLen];
			if(NULL!=pThreadPath)
			{
				_tcscpy_s(pThreadPath,uiBufferLen,*itcopy);
				vec.push_back(pThreadPath);
			}
		}
		LeaveCriticalSection(&g_cs_vector);
		std::vector <const TCHAR*>::iterator it;
		for(it=vec.begin();
			it!=vec.end();
			it++)
		{
			bool bIsStop = false;
			long lload=wavFile.load(*it);
			if(lload==0)
			{
				HWAVEOUT hWaveOut=NULL;
				openDevie(hWaveOut,&wavFile.m_waveFormat);
				if(NULL!=hWaveOut)
				{
					WAVEHDR wavHdr;
					wavHdr.lpData = (HPSTR)wavFile.m_pWavData;
					wavHdr.dwBufferLength = wavFile.m_dwDataSize;
					wavHdr.dwFlags = 0;
					wavHdr.dwLoops = 5;
					wavHdr.dwUser=NULL;

					long lplay=playDevie(hWaveOut,wavHdr);
					if(lplay==0)
					{

						while(true)  
						{  
							BOOL bBreak=FALSE;
							GetMessage(&msg, NULL, 0, 0);
							switch(msg.message)
							{
							case UD_BUFFERPLAYED:
								bBreak=TRUE;
								break;
							case UD_STOPPLAY:
								bBreak=TRUE;
								//it=vec.end();
								bIsStop = true;
								break;
							case UD_PAUSE:
								pauseDevice(hWaveOut);
								break;
							case UD_RESUME:
								resumeDevice(hWaveOut);
								break;
							default:
								break;
							}
							if(bBreak)
							{
								break;
							}
						}
						
						stopDevice(hWaveOut,wavHdr);
						closeDevice(hWaveOut);

					}else
					{
					}
				}else
				{
				}

				wavFile.free();
				// 终止跳出循环
				if(bIsStop)	break;
			}else
			{
			}
		}
		std::vector <const TCHAR*>::iterator itdelete;
		for(itdelete=vec.begin();
			itdelete!=vec.end();
			itdelete++)
		{
			TCHAR* pThreadPath=(TCHAR*)*itdelete;
			//_aligned_free(pThreadPath);
			delete pThreadPath;
			pThreadPath=NULL;
		}
		vec.clear();
	}else
	{
	}
	SetEvent(g_stopCompleteEvent);
	_endthreadex(0);
	return 0;
}

long CWavePlayImp::playWaveItems()
{
	long lret = 1;
	g_playStatus=UE_PLAYING;
	g_hPlayThread = (HANDLE)_beginthreadex(NULL, 0, &playList, this, 0, &g_uiPlayThread);
	WaitForSingleObject(g_msgCompleteEvent,INFINITE);
	CloseHandle(g_hPlayThread);
	g_hPlayThread=NULL;
	return lret;
}

UINT CWavePlayImp::detectDuration()
{
	UINT uiDuration=0;
	std::vector <const TCHAR*>::iterator it;
	for(it=m_vector_waveItems.begin();
		it!=m_vector_waveItems.end();
		it++)
	{
		CWavFile wavFile;
		uiDuration+=100;
		uiDuration+=wavFile.detectDuration(*it);
	}
	return uiDuration;
}

