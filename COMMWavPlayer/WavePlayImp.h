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
  @brief ���������ӿ�ʵ��
*/
class CWavePlayImp
{
public:
	CWavePlayImp();
	~CWavePlayImp();
public:
	long pausWave();		//��ͣ��������
	long resumeWave();		//�ָ��������ţ����ϴβ���λ�ÿ�ʼ����
	long stopWave();		//ֹͣ��������

	long addWaveItem(const TCHAR* tcPath);	//��������ļ��������б���
	long clearWaveItem();					//��յ�ǰ�����б�
	long playWaveItems();					//���Ų����б��е���������

	UINT detectDuration();	//�����������ʱ��
public:
	std::vector<const TCHAR*>					m_vector_waveItems;
	std::vector <const TCHAR*>::iterator 		m_iterator_waveIter;
};

