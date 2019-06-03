
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "SerialPort.h"


#define AFC_INVALID_RS232	 -1
#define afc_rs232_t          int

// ����ͨѶ���ݵ��δ�����󳤶�
#define AFC_RS232_DEF_LEN	0x0440
// ����ͨѶ���ݴ������󳤶�
#define AFC_RS232_MAX_LEN	0x0880


#define iTYPE_2    2

#define iERROR_CODE_DIO_NOT_SUPPORT     0x4E20  

typedef unsigned char UCHAR;
typedef unsigned int  UINT;
typedef unsigned short USHORT;
typedef unsigned char BYTE;

 int m_drvRS232 = -1;
/**
 * ������Ϣ������
 */
char DIO_m_outBuf[128] = { 0 };
/**
 * ������Ϣ������
 */
char DIO_m_inBuf[128] = { 0 };


UCHAR DIO_SOFT_VERSION[] = "DIO_20190304_DIO_V01";		//�汾

/*-----------------------------------------
-----------------�ṹ��--------------------
-------------------------------------------*/
//����ɫ�ṹ��
struct status_LED
{
	bool R;
	bool G;
	bool B; //�����������졢�̡���(����ͬʱ��)�����޸ģ�ʼ��Ϊ0;
};

/*-----------------------------------------
-------�����������,�����ƣ�ά����---------
-------------------------------------------*/
#define DIO_OUT_BUZZER 		0x0013		//������


#define DIO_COLOR_R			1			//��
#define DIO_COLOR_G			2			//��
#define DIO_COLOR_Y			4			//��

/**
 * ��������״̬
 1.����ģ��ͶƱ���źŵ�R, G		
 2.��վͨ��ָʾ��R, G			
 3.��վͨ��ָʾ��R, G			
 4.��վˢ����ʾ��R, G			
 5.��վˢ����ʾ��R, G			
 6.��վ����R, G, Y(R��Gͬʱ��)
 7.��վ����R, G, Y(R��Gͬʱ��)
 */
status_LED DIO_allLED[] =
{
	{0,		0,		0},	
	{0,		0,		0},	
	{0,		0,		0},	
	{0,		0,		0},	
	{0,		0,		0},	
	{0,		0,		0},	
	{0,		0,		0},	
};

/*---------------------------------------------------------------------------*/
/*  	                       ����ָ��                                  	 */
/*---------------------------------------------------------------------------*/
long long DIO_Init_CMD = 0x0165000055AAF2ED;		//��ʼ������
long long DIO_Init_Recv = 0x01650000AA55F35D;		//��ʼ��������ȷ����
long long DIO_Get_ver = 0x016400000005B001;			//��ȡ�汾����

/**
01 05 03 3e ff 00 ed b2 ��ֽ��ָʾ��
01 05 03 3e 00 00 ac 42 ��ֽ��ָʾ��
01 05 03 35 ff 00 9c 70 ��������
01 05 03 3a 00 00 ed 83 ��������
01 05 03 3b ff 00 fd b3 ��Ͷ��ָʾ�� 
01 05 03 3b 00 00 bc 43 ��Ͷ��ָʾ��
01 05 03 3c 00 00 0d 82 ��ȡƱָʾ��
01 05 03 44 ff 00 cc 6b ����Ʊ�ڵ���� 
01 05 03 44 00 00 8d 9b �س�Ʊ�ڵ����
01 05 03 42 ff 00 2c 6a ��Ӳ�ҵ����
01 05 03 42 00 00 6d 9a ��Ӳ�ҵ����
 */


long long DIO_LED_CMD[][5] =
{
	//		openR						openG				closeR					 closeG				B(����)
	
	{0x01050356FF006C6E,		0x01050357FF003DAE,		0x0105035600002D9E,		0x0105035700007C5E,		0},			//����ģ��ͶƱ���źŵ�

	{0x01050341FF00DC6A,		0x01050342FF002C6A,		0x0105034100009D9A,		0x0105034200006D9A,		0},			//��վͨ��ָʾ��

	{0x01050353FF007C6F,		0x01050354FF00CDAE,		0x0105035300003D9F,		0x0105035400008C5E,		0},			//��վͨ��ָʾ��

	{0x0105033DFF001DB2,		0x0105033FFF00BC72,		0x0105033D00005C42,		0x0105033F0000FD82,		0},			//��վˢ����ʾ��	

	{0x01050349FF005DA8,		0x01050351FF00DDAF,		0x0105034900001C58,		0x0105035100009C5F,		0},			//��վˢ����ʾ��

	{0x0105033AFF00AC73,		0x0105033CFF004C72,		0x0105033A0000ED83,		0x0105033C00000D82,		0},			//��վ����

	{0x01050346FF006DAB,		0x01050348FF000C68,		0x0105034600002C5B,		0x0105034800004D98,		0}			//��վ����

};




/**
* ��������ͨ��һ������
* �����������ȷ����ֵ����������
* ����ֵ:
*  0 - ִ�гɹ�; 1 - ִ�д���; 
*/

int DIO_Send_flow(long long sendCMD, UCHAR *recvCMD, int recvlength)
{
	int cmdLength = sizeof(sendCMD);
	memcpy(DIO_m_inBuf, "0", 128);
	memcpy(DIO_m_inBuf, &sendCMD, cmdLength);
	sp_write(DIO_m_inBuf, cmdLength);
	memcpy(DIO_m_outBuf, "0", 128);
	sp_read(DIO_m_outBuf, recvlength);
	if (0 != memcmp(DIO_m_outBuf, recvCMD, recvlength) )
	{
		//LOG_ERROR("<DIO_Set_CMD_flow> Receive is err. Send CMD is %llx, receive is %llx", sendCMD, DIO_m_outBuf);
		return 1;
	}

	return 0;
}

/**
* ���ص�����
* ����ֵ:
opeColor �����ĵ�ɫ
opeBit	 �������ĸ�ģ��
open	 �Ƿ��Ǵ򿪲���
*  0 - ִ�гɹ�; 1 - ִ�д���;
*/
int DIO_LED(UCHAR opeColor, UCHAR opeBit,bool open)
{
	int offset = open ? 0 : 2;
	UCHAR recv1[8],recv2[8];
	if (opeColor != 2)
	{
		memcpy(recv1, &DIO_LED_CMD[opeBit][opeColor + offset], 8);
		if (DIO_Send_flow(DIO_LED_CMD[opeBit][opeColor + offset], recv1, 8))
		{
		//	LOG_ERROR("<DIO_SetLampSignal> opeBit = %d, Color = %d. %s LED err.", opeBit, opeColor,open ? "Open" : "close");
			return 1;
		}
		opeColor == 0 ? DIO_allLED[opeBit].R = open : DIO_allLED[opeBit].G = open;
	}
	else
	{
		if (opeBit != 5 && opeBit != 6)
		{
		//	LOG_ERROR("<DIO_SetLampSignal>  opeBit = %d can not set Color == yellow", opeBit);
			return 1;
		}
		memcpy(recv1, &DIO_LED_CMD[opeBit][0 + offset], 8);
		memcpy(recv1, &DIO_LED_CMD[opeBit][1 + offset], 8);
		if (DIO_Send_flow(DIO_LED_CMD[opeBit][0 + offset], recv1, 8) ||
			DIO_Send_flow(DIO_LED_CMD[opeBit][1 + offset], recv2, 8))
		{
			//LOG_ERROR("<DIO_SetLampSignal> opeBit = %d, Color = %d. %s LED err.", opeBit, opeColor, open ? "Open" : "close");
			return 1;
		}
		DIO_allLED[opeBit].R = open;
		DIO_allLED[opeBit].G = open;
	}
	return 0;
}

/**
* ���õ����ƺ���
*  pLampConfig - ������Ϣ
* ����ֵ:
*  0 - ִ�гɹ�; 1 - �������; 2-ִ�г���
*/
int DIO_Set_LED(STDioLampConfig pLampConfig)
{
	char openLED = pLampConfig.ucOperType;
	UCHAR opeBit = 7,	opeColor = 3,	opeNum = pLampConfig.ucBlinkTimes;

	if (opeBit == 7) 
	{
		//LOG_ERROR("<DIO_SetLampSignal> pLampConfig.usLampType set err!");
		return 1;
	}
	switch (pLampConfig.ucLampColor)
	{
	case DIO_COLOR_G: opeColor = 0;	break;
	case DIO_COLOR_R: opeColor = 1;	break;
	case DIO_COLOR_Y: opeColor = 2;	break;
	}
	if (opeColor == 3)
	{
		//LOG_ERROR("<DIO_SetLampSignal> pLampConfig.ucLampColor set err!");
		return 1;
	}
	if (openLED)			//���Ʋ��� 
	{
		if (opeColor == 0 && DIO_allLED[opeBit].G == 1)
		{
			if (DIO_LED(1, opeBit, 0))
			{
				return 2;
			}
		}
		else if (opeColor == 1 && DIO_allLED[opeBit].R == 1)
		{
			if (DIO_LED(0, opeBit, 0))
			{
				return 2;
			}
		}
		if(opeNum)
		{
			clock_t start;
			while(opeNum--)
			{
				if (DIO_LED(opeColor, opeBit, 1))
				{
					return 2;
				}
				start = clock();

				while ((clock() - start) < 350);
				
				if (DIO_LED(opeColor, opeBit, 0))
				{
					return 2;
				}
			}
		}
		else
		{
			if(DIO_LED(opeColor, opeBit, 1))
			{
				return 2;
			}
		}
	
	}
	else					//�صƲ���
	{
		if(DIO_LED(opeColor, opeBit, 0))
		{
			return 1;
		}
	}

	return 0;
}

/**
	* DIOģ���
	*  uiCommPort - ���ں�(�Ǵ��ڵ�DIOģ��������)
	*  uiBaudRate - ����ͨѶ�Ĳ�����(�Ǵ��ڵ�DIOģ��������)
	* ����ֵ:
	*  0 - ִ�гɹ�; ���� - ִ��ʧ��;
	*/

 int DIO_Open( int nNum, tSerialConfigInfo* pSerialConfigInfo )
 {	
	//LOG_DEBUG("start.");
	int iRet = iERROR_CODE_DIO_0;
	if (nNum <= 0) {
		return iERROR_CODE_DIO_OPEN_FAIL;
	}
	
	if (m_drvRS232 != -1)
		DIO_Close();

	m_drvRS232 = -1;    
	//chSerialPort = 1;
	//nIoSpeed = 115200;
	//chParity = 'N';
	//nDatabits = 8;
	//nStopbits = 1;
	//LOG_DEBUG("<DIO_Open> befor fd=%d, SerialPort=%d", (int)m_drvRS232,pSerialConfigInfo->chSerialPort);

	m_drvRS232 =sp_open(pSerialConfigInfo->chSerialPort,	// ���ں�
						pSerialConfigInfo->nIoSpeed,		// ������
						1,									//0. tty  1.usb
						pSerialConfigInfo->nParity,			// У��λ(N/O/E)
						pSerialConfigInfo->nDatabits,		// ����λ
						pSerialConfigInfo->nStopbits);		// ֹͣλ
						
	if (m_drvRS232 == 0)
	{
		/*LOG_DEBUG("<DIO_Open> openCom[%d, %d, %d, %d, %d] error!",
			pSerialConfigInfo->chSerialPort, pSerialConfigInfo->nIoSpeed,
			pSerialConfigInfo->nDatabits, pSerialConfigInfo->nStopbits, pSerialConfigInfo->nParity);*/
		m_drvRS232 = -1;
		return iERROR_CODE_DIO_OPEN_FAIL;
	}

	return iRet;
}

/**
	 * ��ʼ��ģ�飬���ģ���Ƿ����
	 *  tReturn - ���ص�ģ��״̬��Ϣ
	 * ����ֵ:
	 *  0 - ִ�гɹ�; ���� - ִ��ʧ��;
	 */

   int DIO_Init( STDioDevReturn& tReturn )
   {
	//	LOG_DEBUG("start.");
		int iRet = iERROR_CODE_DIO_0;
		UCHAR recvCMD[8];
		memcpy(recvCMD, &DIO_Init_Recv, 8);
		if(DIO_Send_flow(DIO_Init_CMD, recvCMD, 8))
		{
			tReturn.iType = 2;
			tReturn.iStdErrorCode = iERROR_CODE_DIO_INIT_FAIL;
			return iERROR_CODE_DIO_INIT_FAIL;
		}
		//LOG_DEBUG("end.");
		return iRet;
   }
	/**
	 * DIOģ��ر�
	 * ����ֵ:
	 *  0 - ִ�гɹ�; ���� - ִ��ʧ��;
	 */
   int DIO_Close()
   {
		//LOG_DEBUG("start.");
		if (m_drvRS232 != -1)
		{
			//sp_close();
			m_drvRS232 = -1;
		}
	//	LOG_DEBUG("end.");
		return 0;
   }

	/**
	 * ������ʾ���ź�
	 *  pLampConfig - ��Ҫ���õĵ���Ϣ
	 *  ucLampCnt - ��Ҫ������ʾ�Ƶ�������tDioLampConfig�ĸ�����
	 *  tReturn - ���ص�ģ��״̬��Ϣ
	 * ����ֵ:
	 *  0 - ִ�гɹ�; ���� - ִ��ʧ��;
	 */
   int DIO_SetLampSignal( STDioLampConfig* pLampConfig, unsigned char ucLampCnt, STDioDevReturn& tReturn )
   {
		//LOG_DEBUG("start.");
		int iRet = iERROR_CODE_DIO_0;
		if (!(ucLampCnt > 0)) 
		{
			tReturn.iType = 1;
			tReturn.iStdErrorCode = iERROR_CODE_DIO_PARAM_LAMP;
			return iERROR_CODE_DIO_PARAM_LAMP;
		}
		for(int i = 0; i < ucLampCnt; i++)
		{
			int errFlag = DIO_Set_LED(pLampConfig[i]);
			if ( 1 == errFlag) 
			{
				tReturn.iType = 1;
				tReturn.iStdErrorCode = iERROR_CODE_DIO_PARAM_LAMP;
				return iERROR_CODE_DIO_PARAM_LAMP;
			}
			else if(2 == errFlag)
			{
				tReturn.iType = 2;
				tReturn.iStdErrorCode = iERROR_CODE_DIO_CONFIG_LAMP;
				return iERROR_CODE_DIO_CONFIG_LAMP;
			}
		}
		//LOG_DEBUG("end.");
		return iRet;
   }
	/**
	 * ������ʾ���ź�
	 *  pSignalConfig - ��Ҫ���õĿ����ź�
	 *  ucSignalCnt - ��Ҫ���õĿ����źŵ�������tDioSignalConfig�ĸ�����
	 *  tReturn - ���ص�ģ��״̬��Ϣ
	 * ����ֵ:
	 *  0 - ִ�гɹ�; ���� - ִ��ʧ��;
	 */
     int DIO_SetSwitchSignal( STDioSignalConfig* pSignalConfig, unsigned char ucSignalCnt, STDioDevReturn& tReturn )
	{
		//LOG_DEBUG("start.");
		//LOG_ERROR("<DIO_SetSwitchSignal> Haven't is function, only get swirch signal");
		tReturn.iType = 1;
		tReturn.iStdErrorCode = iERROR_CODE_DIO_NOT_SUPPORT;
		//LOG_DEBUG("end");
		return 0;
    }
	/**
	 * ������ʾ���ź�
	 *  pSignalConfig - ��Ҫ���صĿ����ź����ýṹ����Ҫ�����߿��ٷ��������ڴ�ռ䣩
	 *  ucSignalCnt - ���ؿ����źŵ�����������tDioSignalConfig�ĸ����������������������������
						����ֵΪ0ʱ���������еģ����÷�Ӧ���ϵ���ǰΪpSignalConfig�����㹻�Ŀռ䣩
	 *  tReturn - ���ص�ģ��״̬��Ϣ
	 * ����ֵ:
	 *  0 - ִ�гɹ�; ���� - ִ��ʧ��;
	 */
	int DIO_GetSwitchSignal( STDioSignalConfig* pSignalConfig, unsigned char& ucSignalCnt, STDioDevReturn& tReturn )
	{
		if(ucSignalCnt > 4 || ucSignalCnt == 0)
		{
			ucSignalCnt = 4;
		}
		for(int i = 0; i < ucSignalCnt; i++)
		{
			memcpy(DIO_m_inBuf, "0", 128);
		//	memcpy(DIO_m_inBuf, &DIO_Get_Gate[i], 8);
			sp_write(DIO_m_inBuf, 8);
			memcpy(DIO_m_outBuf, "0", 128);
			sp_read(DIO_m_outBuf, 7);
			//if(memcmp(DIO_m_outBuf, &DIO_Gate_Open, 7))
			{
				pSignalConfig[i].ucOperType = 1;
			}
			//else if(memcmp(DIO_m_outBuf, &DIO_Gate_Close, 7))
			{
				pSignalConfig[i].ucOperType = 0;
			}
		//	else
			{
			//	LOG_ERROR("<DIO_GetSwitchSignal> receive CMD is %llx", DIO_m_outBuf);
				tReturn.iStdErrorCode = iERROR_CODE_DIO_GET_SIGNAL;
				tReturn.iType = 2;
				return 1;
			}
			pSignalConfig[i].usSignalType = DIO_OUT_BUZZER + i;
		}
		return 0;
}
	/**
	 * ��ȡģ��̼��汾
	 *  tVersion - ģ��̼��汾��Ϣ
	 *  tReturn - ���ص�ģ��״̬��Ϣ
	 * ����ֵ:
	 *  0 - ִ�гɹ�; ���� - ִ��ʧ��;
	 */
    int DIO_GetVersion( STDioVerInfo& tVersion, STDioDevReturn& tReturn )
	{
		//LOG_DEBUG("start.");
		unsigned char hardVer[11];
		memcpy(hardVer, "0", 11);
		memcpy(DIO_m_inBuf, "0", 128);
		memcpy(DIO_m_inBuf, &DIO_Get_ver, 8);
		sp_write(DIO_m_inBuf, 8);
	
		memcpy(DIO_m_outBuf, "0", 128);
		sp_read(DIO_m_outBuf, 15);
		for (int i = 3; i < 13; i++)
		{
			hardVer[i - 3] = DIO_m_outBuf[i];
		}
		memcpy(tVersion.pDriverVerInfo, DIO_SOFT_VERSION, 21);
		memcpy(tVersion.pHardwareVerInfo, hardVer, 11);
		//LOG_DEBUG("end");
		return 0;
}

