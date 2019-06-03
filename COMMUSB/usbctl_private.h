

#pragma once

typedef struct uc_removable_logical_device {
	char			LogicalDrive[4];
	char			SerialNumber[MAX_PATH];
	DWORD			DeviceType;
	DWORD			DeviceNumber;
	DWORD			PartitionNumber;
	unsigned long	DeviceInstParent;
} uc_removable_logical_device_t;

// ��ȡUSB�洢�豸��serial number������������
vector<uc_removable_logical_device_t> uc_get_all_removable_logical_device();

// ���ݻ�õ�device id �жϸ��豸�Ƿ���USB�豸
bool uc_is_usb(char *devid);

// ���ݻ�ȡ��device id����ȡserial number
bool uc_get_serial_number(char* serial, 
						  int size, 
						  const char *devid);

// ��ȡUSB�洢�豸��serial number��DevInst
int uc_get_device_serial_number(char *serialnumber, 
								int size, 
								unsigned long *devinst, 
								long deviceNumber, 
								unsigned int devicetype);
// ж��USB�豸
int uc_eject(unsigned long DevInstParent);

// USB�洢�豸�����ƹ���
int uc_ctl(char type);
