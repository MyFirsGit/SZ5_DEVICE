#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <crtdbg.h>
#include <stddef.h>
#include <malloc.h>
#include "THUpdateLib.h"   

// TH������붨��
typedef enum {
	CMD_STATR_DOWNLOAD		= 0x31,  // IAP ���ؿ�ʼ
	CMD_STATR_DOWNLOAD_EX	= 0xF0,  // ����IAP ���ؿ�ʼ
	CMD_DOWNLOAD_COUNT		= 0xE0,  // �·��̼���������
	CMD_DOWNLOAD_FILE		= 0xE1,  // �̼������·�
	CMD_DOWNLOAD_END		= 0xE2,  // �̼��·����
	CMD_SWITCH_TO_NORMAL	= 0xE9   // �л�����������
} TH_CMD;

// TH����������붨��
typedef enum {
	RSP_START_DOWNLOAD_NORMAL	=0x41,	//��ʼ��������(��������)ȷ��Ӧ��
	RSP_START_DOWNLOAD_MSG		=0x42,	//��ʼ��������(��������)���Ĵ���

	RSP_DOWNLOAD_NORMAL			=0xF0,	// �·��̼�ȷ��Ӧ��
	RSP_DOWNLOAD_MSG			=0xF1,	// �·��̼����Ĵ���
	RSP_DOWNLOAD_EXT			=0xF2,	// �·��̼�ִ�д���

	RSP_SWITCH_TO_NORMAL_NORMAL = 0xF9,	// �л�����������ȷ��Ӧ��
	RSP_SWITCH_TO_NORMAL_MSG	= 0xF1,	// �л������������Ĵ���
	RSP_SWITCH_TO_NORMAL_EXT	= 0xF2,	// �л�����������ִ�д���
} TH_RSP_CODE;


typedef union _lc {
	_lc() : val(0) {}
	long val;
	char cs[4];
} lchar;

//////////////////////////////////////////////////////////////////////////
/**
@brief      BCC���㺯��

@param      (i)BYTE* pData     �������������ָ��
@param      (i)WORD dwLength   ������������ݳ���

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
inline BYTE BccCalc(BYTE* pData, WORD dwLength)
{
	BYTE Bcc = 0;
	while (dwLength-- != 0){
		Bcc = Bcc ^ *pData++;
	}
	return Bcc;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ת��4�ֽ��������ݵĴ��˳��(����ʹ��)

@param      (i)long l ��Ҫת��������

@retval     long      ת���������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
inline long lrev(long l)
{
	lchar s; s.val = l;
	lchar d;
	char* sp=s.cs;
	char* dp=&d.cs[3];
	*dp-- = *sp++;
	*dp-- = *sp++;
	*dp-- = *sp++;
	*dp = *sp;
	return d.val;
}