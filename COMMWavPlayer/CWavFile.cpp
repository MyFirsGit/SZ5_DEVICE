#include "stdafx.h"
#include "CWavFile.h"
#include <Shlwapi.h>
#include <tchar.h>
#pragma comment (lib,"Shlwapi.lib")

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CWavFile::CWavFile()
{
	m_dwDataSize=0;
	m_pWavData=NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CWavFile::~CWavFile()
{
	free();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      释放语音数据内存区

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CWavFile::free()
{
	if(NULL!=m_pWavData)
	{
		_aligned_free(m_pWavData);
		m_pWavData=NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检测语音播放时长

@param      (i)const TCHAR* tcPath	波形文件路径，绝对路径

@retval     UINT 加载的语音文件的播放时长

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT CWavFile::detectDuration(const TCHAR* tcPath)
{
	UINT uiDuration=0;
	if(PathFileExists(tcPath)==TRUE)
	{
		HMMIO hMmio = NULL;
		TCHAR tcWavePath[MAX_PATH];
		_tcscpy_s(tcWavePath, MAX_PATH, tcPath);
		hMmio = mmioOpen(tcWavePath, NULL, MMIO_READ | MMIO_ALLOCBUF);
		if (NULL != hMmio)
		{
			MMCKINFO		mmckinfoParent;
			MMCKINFO		mmckinfoSubChunk;
			DWORD			dwFmtSize=0;
			HLOCAL			hFormat;
			WAVEFORMATEX*	lpFormat=NULL;

			//进入块，检查打开文件是否是wave文件
			mmckinfoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
			MMRESULT descendResult = mmioDescend(hMmio, (LPMMCKINFO)& mmckinfoParent, NULL,
				MMIO_FINDRIFF);
			if (MMSYSERR_NOERROR == descendResult)
			{
				//寻找 ´fmt´ 块
				mmckinfoSubChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
				descendResult = mmioDescend(hMmio, &mmckinfoSubChunk, &mmckinfoParent,
					MMIO_FINDCHUNK);
				if (MMSYSERR_NOERROR == descendResult)
				{
					//获得 ´fmt ´块的大小，申请内存
					dwFmtSize = mmckinfoSubChunk.cksize;
					hFormat = LocalAlloc(LMEM_MOVEABLE, LOWORD(dwFmtSize));		//需要释放资源
					if (NULL != hFormat)
					{
						lpFormat = (WAVEFORMATEX *)LocalLock(hFormat);			//需要释放资源
						if (NULL != lpFormat)
						{
							if ((unsigned long)mmioRead(hMmio, (HPSTR)lpFormat, dwFmtSize) ==
							dwFmtSize)
							{
								memmove(&m_waveFormat,lpFormat,sizeof(WAVEFORMATEX));
								//离开 fmt 块
								MMRESULT ascendResult = mmioAscend(hMmio, &mmckinfoSubChunk, 0);
								if(MMSYSERR_NOERROR == ascendResult)
								{
									//寻找 ´data´ 块
									mmckinfoSubChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
									MMRESULT descendResult = mmioDescend(hMmio, &mmckinfoSubChunk, &mmckinfoParent,
										MMIO_FINDCHUNK);
									if (MMSYSERR_NOERROR == descendResult)
									{
										//获得 ´data´块的大小
										m_dwDataSize = mmckinfoSubChunk.cksize;
										if(m_waveFormat.nAvgBytesPerSec>0)
										{
											//计算语音文件播放时长
											uiDuration=m_dwDataSize/m_waveFormat.nAvgBytesPerSec;
										}
									}else
									{
										//进去data区失败
									}
								}else
								{
									//出 fmt 区失败
								}
							}else
							{
							}
							LocalUnlock(hFormat);
						}else
						{
							//锁定失败
						}
						LocalFree(hFormat);
						hFormat=NULL;
					}else
					{
						//分配内存失败
					}
				}else
				{
					//没有找到 fmt块
				}
			}else
			{
				//不是Wav文件
			}

			mmioClose(hMmio,NULL);
		}else
		{
			//mmioOpen失败
		}
	}else
	{
		//文件不存在
	}
	return uiDuration;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检测语音播放时长

@param      (i)const TCHAR* tcPath	波形文件路径，绝对路径

@retval     UINT 加载的语音文件的播放时长

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CWavFile::load(const TCHAR* tcPath)
{
	long lret=1;

	if(PathFileExists(tcPath)==TRUE)
	{
		HMMIO hMmio = NULL;
		TCHAR tcWavePath[MAX_PATH];
		_tcscpy_s(tcWavePath, MAX_PATH, tcPath);
		hMmio = mmioOpen(tcWavePath, NULL, MMIO_READ | MMIO_ALLOCBUF);
		if (NULL != hMmio)
		{
			MMCKINFO		mmckinfoParent;
			MMCKINFO		mmckinfoSubChunk;
			DWORD			dwFmtSize=0;
			HLOCAL			hFormat;
			WAVEFORMATEX*	lpFormat=NULL;

			//进入块，检查打开文件是否是wave文件
			mmckinfoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
			MMRESULT descendResult = mmioDescend(hMmio, (LPMMCKINFO)& mmckinfoParent, NULL,
				MMIO_FINDRIFF);
			if (MMSYSERR_NOERROR == descendResult)
			{
				//寻找 ´fmt´ 块
				mmckinfoSubChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
				descendResult = mmioDescend(hMmio, &mmckinfoSubChunk, &mmckinfoParent,
					MMIO_FINDCHUNK);
				if (MMSYSERR_NOERROR == descendResult)
				{
					//获得 ´fmt ´块的大小，申请内存
					dwFmtSize = mmckinfoSubChunk.cksize;
					hFormat = LocalAlloc(LMEM_MOVEABLE, LOWORD(dwFmtSize));		//需要释放资源
					if (NULL != hFormat)
					{
						lpFormat = (WAVEFORMATEX *)LocalLock(hFormat);			//需要释放资源
						if (NULL != lpFormat)
						{
							if ((unsigned long)mmioRead(hMmio, (HPSTR)lpFormat, dwFmtSize) ==
							dwFmtSize)
							{
								memmove(&m_waveFormat,lpFormat,sizeof(WAVEFORMATEX));
								//离开 fmt 块
								MMRESULT ascendResult = mmioAscend(hMmio, &mmckinfoSubChunk, 0);
								if(MMSYSERR_NOERROR == ascendResult)
								{
									//寻找 ´data´ 块
									mmckinfoSubChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
									MMRESULT descendResult = mmioDescend(hMmio, &mmckinfoSubChunk, &mmckinfoParent,
										MMIO_FINDCHUNK);
									if (MMSYSERR_NOERROR == descendResult)
									{
										//获得 ´data´块的大小
										m_dwDataSize = mmckinfoSubChunk.cksize;
										DWORD dwDataOffSet = mmckinfoSubChunk.dwDataOffset;
										if (m_dwDataSize > 0L)
										{
											m_pWavData = (char*)_aligned_malloc(sizeof(char)*m_dwDataSize,16);//需要释放资源
											if (NULL != m_pWavData)
											{
												LONG lActualOffset = mmioSeek(hMmio, dwDataOffSet, SEEK_SET);
												if (lActualOffset == dwDataOffSet)
												{
													DWORD dwActualRead = 0; 
													dwActualRead = mmioRead(hMmio, m_pWavData, m_dwDataSize);
													if (dwActualRead == m_dwDataSize)
													{
														if(m_waveFormat.nAvgBytesPerSec>0)
														{
															//计算语音文件播放时长
															m_uiPlayTime=(m_dwDataSize*1000)/m_waveFormat.nAvgBytesPerSec;
														}
														lret=0;
													}else
													{
														//读取大小不匹配
													}
												}else
												{
													//实际Seek和预期不等
												}
											}else
											{
												//内存溢出
											}
										}else
										{
											//空的波形文件
										}
									}else
									{
										//进去data区失败
									}
								}else
								{
									//出 fmt 区失败
								}
							}else
							{
							}
							LocalUnlock(hFormat);
						}else
						{
							//锁定失败
						}
						LocalFree(hFormat);
						hFormat=NULL;
					}else
					{
						//分配内存失败
					}
				}else
				{
					//没有找到 fmt块
				}
			}else
			{
				//不是Wav文件
			}

			mmioClose(hMmio,NULL);
		}else
		{
			//mmioOpen失败
		}
	}else
	{
		//文件不存在
	}

	return lret;
}