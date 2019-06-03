#ifndef _CASDEF_H_
#define _CASDEF_H_

//========�����붨��========================================
#define S_CAS_BASE			    	17000			  // �������ֵ

#define W_CAS_BASE                  S_CAS_BASE + 5    // 17005 �������ֵ
#define W_CAS_MHOPPER1_COUNT_ERROR  W_CAS_BASE + 1    // 17006 Hopper2�����������						(���ʧ��)	//�人11����ר��
#define W_CAS_MHOPPER2_COUNT_ERROR  W_CAS_BASE + 2    // 17007 Hopper1�����������						(���ʧ��)  //�人11����ר��
#define W_CAS_MHOPPER_COUNT_ERROR   W_CAS_BASE + 3    // 17008 ��Hopper�����������						(���ʧ��)

#define E_CAS_BASE				    S_CAS_BASE + 10   // 17010 ���ش���ֵ

#define E_CAS_COMMAND_ERROR         E_CAS_BASE + 1    // 17011 ��Ч����
#define E_CAS_NO_CHANGE			    E_CAS_BASE + 2    // 17012 ���㣬����ʧ��						(�����������㹦��)
#define E_CAS_ESCROW1_MIDDLE		E_CAS_BASE + 3    // 17013 �ݴ���1�Ҳ����м�λ��					(����ͣ��ά����Ӱ��������ձҹ���)
#define E_CAS_ESCROW1_LEFT			E_CAS_BASE + 4    // 17014 �ݴ���1�Ҳ������λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
#define E_CAS_ESCROW1_RIGHT	        E_CAS_BASE + 5    // 17015 �ݴ���1�Ҳ����ұ�λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
#define E_CAS_ESCROW2_MIDDLE		E_CAS_BASE + 6    // 17016 �ݴ���2�Ҳ����м�λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
#define E_CAS_ESCROW2_LEFT			E_CAS_BASE + 7    // 17017 �ݴ���2�Ҳ������λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
#define E_CAS_ESCROW2_RIGHT			E_CAS_BASE + 8    // 17018 �ݴ���2�Ҳ����ұ�λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
#define E_CAS_ROUTER1_LEFT			E_CAS_BASE + 9    // 17019 ������1�Ҳ������λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
#define E_CAS_ROUTER1_RIGHT			E_CAS_BASE + 10   // 17020 ������1�Ҳ����ұ�λ��  					(����ͣ��ά����Ӱ��������ձҹ���)
#define E_CAS_ROUTER2_LEFT			E_CAS_BASE + 11   // 17021 ������2�Ҳ������λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
#define E_CAS_ROUTER2_RIGHT			E_CAS_BASE + 12   // 17022 ������2�Ҳ����ұ�λ��   					(����ͣ��ά����Ӱ��������ձҹ���)

#define E_CAS_ROUTER1_MIDDLE		E_CAS_BASE + 13   // 17023 ������1�Ҳ����м�λ��   					(����ͣ��ά����Ӱ��������ձҹ���)
#define E_CAS_ROUTER2_MIDDLE		E_CAS_BASE + 14   // 17024 ������2�Ҳ����м�λ��   					(����ͣ��ά����Ӱ��������ձҹ���)

#define E_CAS_MHOPPER1_DISP			E_CAS_BASE + 15   // 17025 ������2����ʧ��       					(����ͣ��ά���������������㹦��)	//�人11����ר��
#define E_CAS_MHOPPER1_JAM			E_CAS_BASE + 16   // 17026 ������2����           					(����ͣ��ά���������������㹦��)	//�人11����ר��
#define E_CAS_MHOPPER2_DISP			E_CAS_BASE + 17   // 17027 ������1����ʧ��       					(����ͣ��ά���������������㹦��)	//�人11����ר��
#define E_CAS_MHOPPER2_JAM			E_CAS_BASE + 18   // 17028 ������1����           					(����ͣ��ά���������������㹦��)	//�人11����ר��

#define E_CAS_COINACCEPTOR_FAULT	E_CAS_BASE + 23   // 17033 Ӳ�ҽ���������          					(����ͣ��ά�������������ձҹ���)
#define E_CAS_IBUTTON1				E_CAS_BASE + 24   // 17034 �ӱ���2iButton����/RFID����  			(����ͣ��ά��)	//�人11����ר��
#define E_CAS_IBUTTON2				E_CAS_BASE + 25   // 17035 �ӱ���1iButton����/RFID����  			(����ͣ��ά��)	//�人11����ר��
#define E_CAS_WRITE_FLASH			E_CAS_BASE + 26   // 17036 ���FLASHʧ��           					(����ͣ��ά��)
#define E_CAS_FILE_NOTFINISH		E_CAS_BASE + 27   // 17037 �ļ�δ������            					(����ͣ��ά��)
#define E_CAS_WRITE_LOG				E_CAS_BASE + 28   // 17038 дLogʧ��               					(Ԥ��)
#define E_CAS_READ_LOG				E_CAS_BASE + 29   // 17039 ��Logʧ��               					(Ԥ��)
#define E_CAS_CLRCOIN_FAIL          E_CAS_BASE + 30   // 17040 ���ʧ��                					(����ͣ��ά������Ӱ������ͽ���Ӳ�ҹ���)
#define E_CAS_CAM_JAM				E_CAS_BASE + 31   // 17041 ͹�ֶ�ת                					(����ͣ��ά��)
#define E_CAS_CAM_MOTOR				E_CAS_BASE + 32   // 17042 ͹��������            					(����ͣ��ά��)
#define E_CAS_COININ_JAM			E_CAS_BASE + 33   // 17043 Ͷ�ҿڿ���              					(����ͣ��ά�������������ձҹ���)
#define E_CAS_EJECT_SIGNAL			E_CAS_BASE + 34   // 17044 Ӳ�ҽ������˱��ź��쳣  					(����ͣ��ά�������������ձҹ���)
#define E_CAS_ESCROW1_SENSOR		E_CAS_BASE + 35   // 17045 �ݴ���1����������       					(����ͣ��ά��)
#define E_CAS_ESCROW2_SENSOR		E_CAS_BASE + 36   // 17046 �ݴ���2����������       					(����ͣ��ά��)
#define E_CAS_ROUTER1_SENSOR		E_CAS_BASE + 37   // 17047 ������1����������       					(����ͣ��ά��)
#define E_CAS_ROUTER2_SENSOR		E_CAS_BASE + 38   // 17048 ������2����������       					(����ͣ��ά��)
#define E_CAS_EEPROM		 		E_CAS_BASE + 39   // 17049 EEPROM����              					(����ͣ��ά��)
#define E_CAS_COINACCEPTOR_TIMEOUT	E_CAS_BASE + 40   // 17050 Ӳ�ҽ�������ʱ          					(����ͣ��ά�������������ձҹ���)
#define E_CAS_COINACCEPTOR_DATA		E_CAS_BASE + 41   // 17051 Ӳ�ҽ������������ݴ�    					(����ͣ��ά�������������ձҹ���)
#define E_CAS_DEV_BUSY				E_CAS_BASE + 42   // 17052 �豸æ                  
#define E_CAS_IBUTTON_ERROR         E_CAS_BASE + 43   // 17053 Ǯ��iButton����/RFID����					(����ͣ��ά��)
#define E_CAS_COINOUT_JAM           E_CAS_BASE + 44   // 17054 Ͷ�ҳ��ڿ���            					(����ͣ��ά�������������ձҹ���)
#define E_CAS_MODULE_ERROR          E_CAS_BASE + 45   // 17055 ģ��λ���쳣          					(����ͣ��ά��)
#define E_CAS_HOPPER1_POS           E_CAS_BASE + 46   // 17056 HOPPER1����λ         					(����ͣ��ά��)
#define E_CAS_HOPPER2_POS           E_CAS_BASE + 47   // 17057 HOPPER2����λ           					(����ͣ��ά��)
#define E_CAS_COINBOX_POS           E_CAS_BASE + 48   // 17058 Ǯ�䲻��λ              					(����ͣ��ά��)
#define E_CAS_MODULE_POS            E_CAS_BASE + 49   // 17059 ģ�鲻��λ              					(����ͣ��ά��)
#define E_CAS_IMAGE_ERROR			E_CAS_BASE + 50   // 17060 ͼ��ʶ��ģ�����        					(����ͣ��ά��)

#define E_CAS_RFID_REQUEST			E_CAS_BASE + 51   // 17061 ��Ӧ������Ч����/��ģ��δ����			(����ҵ������������ϵȼ�)
#define E_CAS_RFID_AUTHEN			E_CAS_BASE + 52   // 17062 ��Ӧ������֤ʧ��							(����ҵ������������ϵȼ�)
#define E_CAS_RFID_PARAMETER		E_CAS_BASE + 53   // 17063 ��дRFIDʱ������							(����ҵ������������ϵȼ�)

#define E_CAS_READ_FILE 			E_CAS_BASE + 56   // 17066 ���ļ�ʧ��								(����ͣ��ά��)
#define E_CAS_FILE_LENGTH			E_CAS_BASE + 57   // 17067 �ļ�̫��									(����ͣ��ά��)
#define E_CAS_OPEN_FILE				E_CAS_BASE + 58   // 17068 �ļ���ʧ��								(����ͣ��ά��)
#define E_CAS_UPDATEDATA_EMPTY		E_CAS_BASE + 59	  // 17069 �޷�ִ�и��£��޸�������

#define E_CAS_COINBOXFULL			E_CAS_BASE + 63	  // 17073 Ӳ�һ���Ǯ����
#define E_CAS_VOLTAGE_LACK			E_CAS_BASE + 64	  // 17074 ��ѹ���� 
#define E_CAS_MHOPPER1_INTERVENE	E_CAS_BASE + 65   // 17075 ������1���ҹ����д�����������(��ǰ���׳��Ҽ�����׼)
#define E_CAS_MHOPPER1_LACK			E_CAS_BASE + 66   // 17076 ������1���ҹ����п����ٳ��� (��ǰ���׳��Ҽ�����׼)
#define E_CAS_MHOPPER2_INTERVENE	E_CAS_BASE + 67   // 17077 ������2���ҹ����д����������� (��ǰ���׳��Ҽ�����׼)
#define E_CAS_MHOPPER2_LACK			E_CAS_BASE + 68   // 17078 ������2���ҹ����п����ٳ��� (��ǰ���׳��Ҽ�����׼)

//added by ZacharyLiu 2015-12-03
#define E_CAS_UNSUPPORT				E_CAS_BASE + 69	  // 17079	�豸��֧�ֵĹ���						(ĳЩ�ӿڽ�����ض��ͺţ����ڲ�֧�ֵ��ͺŻ᷵�ش˴���)
#define E_CAS_RTRBOX2_RFID			E_CAS_BASE + 71	  // 17081  ������2 iButton/RFID ����
#define E_CAS_GATE_OPEN				E_CAS_BASE + 72	  // 17082  բ�Ŵ�ʧ��
#define E_CAS_GATE_CLOSE			E_CAS_BASE + 73	  // 17083  բ�Źر�ʧ��
//end added

#define E_CAS_UNKNOWN				E_CAS_BASE + 80   // 17090 δ֪��
#define E_CAS_PARAM_ERROR			E_CAS_BASE + 81   // 17091 ��������ȷ 
#define E_CAS_DEV_CFG				E_CAS_BASE + 82   // 17092 ��д�豸���ô�                
#define E_CAS_COMM_CFG	    		E_CAS_BASE + 83   // 17093 �������ô� 
#define E_CAS_RECEIVE_NAK			E_CAS_BASE + 84   // 17094 �յ�NAK,���������д�    
#define E_CAS_REVDATA_ERROR			E_CAS_BASE + 85   // 17095 ���յ�����������        
#define E_CAS_SENDENQ_ERROR			E_CAS_BASE + 86   // 17096 ����ENQ����             
#define E_CAS_SENDCMD_ERROR		    E_CAS_BASE + 87   // 17097 ���������              
#define E_CAS_WAITACK_TIMEOUT		E_CAS_BASE + 88   // 17098 �ȴ�ACK��ʱ             
#define E_CAS_WAITRSP_TIMEOUT		E_CAS_BASE + 89   // 17099 �ȴ���Ӧ��ʱ            

#endif	// #ifndef _CASDEF_H_