#ifndef __TOKENCOMMANDS_H__
#define __TOKENCOMMANDS_H__

#pragma once
#include "StdAfx.h"
#include "TOKENLIBExp.h"
#include "TOKENCOMLIB_DEF.h"
#include "Command.h"
#include "ToIMApi.h"


//function:		ͨѶ����
//return:		0��ִ�гɹ���1��ִ��ʧ��
extern "C" typedef int( *pToIM_CommOpen)(const char *sCommPort, uint uiBaudRate);


//function:		�ر�ͨѶ
//return:		0��ִ�гɹ���1��ִ��ʧ��
extern "C" typedef int( *pToIM_CommClose)(void);


//function:		ģ���ʼ�����豸������λ����Ҫ���ñ�������
//parameter:	ucAcionMode(IN 1��ͨ����Ʊ��2��ͨ������Ʊ)
//				ucErrorCode(OUT ���ش�����)	
//				xModuleStatus(OUT �����豸״̬)	
//				ucTokenNum(OUT ���տ�����)	
//return:		's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_Init)(uchar ucActionMode,uchar & ucErrorCode, TOKEN_NAMESPACE::ModuleStatus & xModuleStatus, uchar &ucTokenNum);


//function:		��ȡ�豸״̬����ȡ��д���Ƿ��п���Ʊ��״̬��Ʊ�������Լ���������ʵʱ״̬��
//parameter:	ucErrorCode(OUT ���ش�����)	
//				xModuleStatus(OUT �����豸״̬)	
//return:		's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_GetDevStatus)(uchar & ucErrorCode, TOKEN_NAMESPACE::ModuleStatus & xModuleStatus);


//function:		token������������
//parameter:	ucBoxNo(IN 1��AƱ�䣻2��BƱ��)
//				ucErrorCode(OUT ���ش�����)	
//				xModuleStatus(OUT �����豸״̬)	
//				ucTokenNum(OUT ��ȡ��token����)	
//return:		's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_TokenToAntenna)(uchar ucBoxNo, uchar & ucErrorCode, TOKEN_NAMESPACE::ModuleStatus & xModuleStatus, uchar & ucTokenNum);


//function:		token��������Ʊ��
//parameter:	ucErrorCode(OUT ���ش�����)	
//				xModuleStatus(OUT �����豸״̬)	
//				ucTokenNum(OUT ���β������͵Ŀ�Ƭ��)	
//return:		's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_TokenToOutlet)(uchar & ucErrorCode, TOKEN_NAMESPACE::ModuleStatus & xModuleStatus, uchar & ucTokenNum);


//function:		token������������
//parameter:	ucErrorCode(OUT ���ش�����)	
//				xModuleStatus(OUT �����豸״̬)	
//				ucTokenNum(OUT ���λ��յĿ�Ƭ��)	
//return:		's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_TokenRetrieve)(uchar & ucErrorCode, TOKEN_NAMESPACE::ModuleStatus & xModuleStatus, uchar & ucTokenNum);


//function:		ģ�鸴λ��������������ģ��Ŀ��Ƴ������豸���ϻ��ߴ�ά��ģʽ�˳�ʱ������֧��ά��ģʽ������£���ִ�б�������
//parameter:	ucErrorCode(���ش�����)	
//return:		0��ִ�гɹ���1��ִ��ʧ��
//ps:			's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *poTIM_Reset)(uchar &ucErrorCode);


//function:		��ȡ�豸�����汾��Ϣ����ȡ�豸�����汾�š��̼��汾��(�緢��ģ��� ID ��)��״̬��Ϣ��
//parameter:	ucErrorCode(���ش�����)	
//				xModuleInfo��������Ϣ�ṹ��
//return:		0��ִ�гɹ���1��ִ��ʧ��
//ps:			's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_ReadVersion)(uchar & ucErrorCode, TOKEN_NAMESPACE::ModuleInfo & xModuleInfo);


//function:		���Ʊ�䣨���豸�ڵ�token������գ�
//parameter:	ucBoxNo(IN 1��AƱ�䣻2��BƱ��)
//				ucErrorCode(OUT ���ش�����)	
//return:		's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_CleanOut)(uchar cBoxNo, uchar & ucErrorCode);


//function:		ֹͣ���Ʊ��
//parameter:	ucBoxNo(IN 1��AƱ�䣻2��BƱ��)
//				ucErrorCode(OUT ���ش�����)	
//return:		's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_StopCleanOut)(uchar cBoxNo, uchar & ucErrorCode);


//function:		��ȡƱ���������
//parameter:	ucBoxNo(IN 1��AƱ�䣻2��BƱ��)
//				ucErrorCode(OUT ���ش�����)	
//				usCoNum(OUT ���Ʊ��Token��������
//return:		's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_GetNumOfCleanOut)(uchar cBoxNo, uchar & ucErrorCode,ushort & usCoNum);


//function:		���ÿ�RFID��Ϣ
//parameter:	ucRfidNo(IN RFID�˿ں� 0x04��AƱ�䣻0x03��BƱ�䣻0x05����Ʊ�䣻0x06����Ʊ��)
//				ucBlockNo(IN Block�� ȡֵ��ΧΪ�� 8,9,10 / 12,13,14 / 16,17,18 / 20,21,22 / 24,25,26 / 28,29,30 / 32,33,34 / 36,37,38 / 40,41,42 / 44,45,46 / 48,49,50 / 52,53,54 / 56,57,58 / 60,61,62)
//				xBlockData(IN ��ṹ��)
//				ucErrorCode(OUT ���ش�����)	
//return:		's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_RfidWriteBlock)(uchar ucRfidNo, uchar ucBlockNo,TOKEN_NAMESPACE::BlockData xBlockData, uchar &ucErrorCode);


//function:		��ȡ��RFID��Ϣ
//parameter:	ucRfidNo(IN RFID�˿ں� 0x04��AƱ�䣻0x03��BƱ�䣻0x05����Ʊ�䣻0x06����Ʊ��)
//				ucBlockNo(IN Block�� ȡֵ��ΧΪ�� 8,9,10 / 12,13,14 / 16,17,18 / 20,21,22 / 24,25,26 / 28,29,30 / 32,33,34 / 36,37,38 / 40,41,42 / 44,45,46 / 48,49,50 / 52,53,54 / 56,57,58 / 60,61,62)
//				xBlockData(IN ��ṹ��)
//				ucErrorCode(OUT ���ش�����)	
//return:		's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_RfidReadBlock)(uchar ucRfidNo, uchar ucBlockNo, uchar &ucErrorCode,TOKEN_NAMESPACE::BlockData &xBlockData);


//function:		��������RFID��Ϣ
//parameter:	ucRfidNo(IN RFID�˿ں� 0x04��AƱ�䣻0x03��BƱ�䣻0x05����Ʊ�䣻0x06����Ʊ��)
//				ucSectorNo(IN Sector�ţ� ȡֵ��ΧΪ�� 2~15 )
//				xSectorData(IN д�����ݣ��䳤��)
//				ucLen(IN д�����ݵĳ��ȣ����Ϊ48�ֽ�)
//				ucErrorCode(OUT ���ش�����)	
//return:		's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_RfidWriteSector)(uchar ucRfidNo, uchar ucSectorNo, uchar ucLen,TOKEN_NAMESPACE::SectorData xSectorData,uchar &ucErrorCode);


//function:		��ȡ����RFID��Ϣ
//parameter:	ucRfidNo(IN RFID�˿ں� 0x04��AƱ�䣻0x03��BƱ�䣻0x05����Ʊ�䣻0x06����Ʊ��)
//				ucSectorNo(IN Sector�ţ� ȡֵ��ΧΪ�� 2~15 )
//				ucErrorCode(OUT ���ش�����)	
//				xSectorData(IN �������ݣ��̶�48�ֽڣ�)
//return:		's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_RfidReadSector)(uchar ucRfidNo, uchar ucSectorNo, uchar &ucErrorCode,TOKEN_NAMESPACE::SectorData &xSectorData);


//function:		��ȡRFID����������
//parameter:	ucRfidNo(IN RFID�˿ں� 0x04��AƱ�䣻0x03��BƱ�䣻0x05����Ʊ�䣻0x06����Ʊ��)
//				ucErrorCode(OUT ���ش�����)	
//				uiRfidID(OUT ��ȡ�����ţ� ��ȡ���� S50 ���������ţ� 4 �ֽڣ�)
//return:		's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_RfidReadID)(uchar ucRfidNo, uchar &ucErrorCode, uchar* uiRfidID);


//function:		���������
//parameter:	ucErrorCode(OUT ���ش�����)	
//				sensorData(OUT 3���ֽڴ�������Ϣ��
//return:		's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_SensorCheck)(uchar &ucErrorCode, char *sensorData);


//function:		��ȡHopper�汾
//parameter:	ucErrorCode(OUT ���ش�����)	
//				xModuleInfo(OUT Hopper�汾32���ֽ� ���ṹ��˵�� ��
//return:		's'��ִ�гɹ���'e'��ִ��ʧ�ܣ�'w'������
extern "C" typedef int( *pToIM_GetHopperVersion)(uchar &ucErrorCode, TOKEN_NAMESPACE::HopperModuleInfo &xModuleInfo);


/** 
@brief   TOKEN command ��
*/
class TOKENCOMLIB_DECLSPEC CTOKENCommands : public CCommand
{		
public:
	CTOKENCommands();
	~CTOKENCommands();

	// �׸���API��װ
	virtual long CommOpen(UINT uiCommPort, UINT uiBaudRate);
	virtual long CommClose(); 
	virtual long Init(uchar ucActionMode, uchar &ucRetriveNum);
	virtual long Reset(); 
	virtual long GetVersion(TOKEN_NAMESPACE::ModuleInfo & xModuleInfo);
	virtual long GetDevStatus(TOKEN_NAMESPACE::ModuleStatus& xModuleStatus);

	virtual long CardOut(uchar ucBoxNo, uchar & ucTokenNum);
	virtual long SendCard(uchar & ucTokenNum);
	virtual long RetractCard(uchar & ucTokenNum);

	virtual long CleanOut(uchar cBoxNo);
	virtual long StopCleanOut(uchar cBoxNo);
	virtual long GetCleanOutOfNum(uchar cBoxNo, ushort &usCoNum);

	virtual long RfidWriteBlock(uchar ucRfidNo, uchar ucBlockNo,TOKEN_NAMESPACE::BlockData xBlockData);
	virtual long RfidReadBlock(uchar ucRfidNo, uchar ucBlockNo,TOKEN_NAMESPACE::BlockData& xBlockData);
	virtual long RfidWriteSector(uchar ucRfidNo, uchar ucSectorNo, uchar ucLen, TOKEN_NAMESPACE::SectorData xSectorData);
	virtual long RfidReadSector(uchar ucRfidNo, uchar ucSectorNo, TOKEN_NAMESPACE::SectorData xSectorData);
	virtual long RfidReadID(uchar ucRfidNo, uchar* uiRfidID);

	virtual long SensorCheck(char* sSensorData);

	virtual long GetHopperVersion(TOKEN_NAMESPACE::HopperModuleInfo& xModuleInfo);

public:
	long DealReturnCode(long retCode, uchar ucErrorCode);

	HINSTANCE hInst;
	pToIM_CommOpen TIM_CommOpen;
	pToIM_CommClose TIM_CommClose;
	pToIM_Init TIM_Init;
	poTIM_Reset TIM_Reset;
	pToIM_ReadVersion TIM_GetVersion;
	pToIM_GetDevStatus TIM_GetDevStatus;
	pToIM_TokenToAntenna TIM_CardOut;
	pToIM_TokenToOutlet TIM_SendCard;
	pToIM_TokenRetrieve TIM_RetractCard;
	pToIM_CleanOut TIM_CleanOut;
	pToIM_StopCleanOut TIM_StopCleanOut;
	pToIM_GetNumOfCleanOut TIM_GetNumOfCleanOut;
	pToIM_RfidWriteBlock TIM_RfidWriteBlock;
	pToIM_RfidReadBlock TIM_RfidReadBlock;
	pToIM_RfidWriteSector TIM_RfidWriteSector;
	pToIM_RfidReadSector TIM_RfidReadSector;
	pToIM_RfidReadID TIM_RfidReadID;
	pToIM_SensorCheck TIM_SensorCheck;
	pToIM_GetHopperVersion TIM_GetHopperVersion;

protected:
	virtual long ExecuteCommand();
	virtual long IsValidCommand();

};
#endif