#pragma once

/**
  @brief �������ļ�����
*/
#include <MMSystem.h>
class CWavFile
{
public:
	CWavFile();
	~CWavFile();
public:
	long			load(const TCHAR* tcPath);				//���ز����ļ���Դ	������ȡ�������ݣ���ȡ���θ�ʽ�����ݴ�С����������ʱ��
	UINT		detectDuration(const TCHAR* tcPath);	//��Ⲩ���ļ�����ʱ�� �����ֽ���/ÿ���Ӳ����ֽ���
	void			free();									//�ͷ���������
public:
	char*			m_pWavData;		//ָ����������
	WAVEFORMATEX	m_waveFormat;	//Wav�ļ��ļ�ͷ
	DWORD			m_dwDataSize;	//Wav�ļ��������ݴ�С ���ֽڣ�
	UINT			m_uiPlayTime;	//Wav�ļ�����ʱ��		���룩
};