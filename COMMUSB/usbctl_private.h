

#pragma once

typedef struct uc_removable_logical_device {
	char			LogicalDrive[4];
	char			SerialNumber[MAX_PATH];
	DWORD			DeviceType;
	DWORD			DeviceNumber;
	DWORD			PartitionNumber;
	unsigned long	DeviceInstParent;
} uc_removable_logical_device_t;

// 获取USB存储设备的serial number，磁盘驱动号
vector<uc_removable_logical_device_t> uc_get_all_removable_logical_device();

// 根据获得的device id 判断该设备是否是USB设备
bool uc_is_usb(char *devid);

// 根据获取的device id，抽取serial number
bool uc_get_serial_number(char* serial, 
						  int size, 
						  const char *devid);

// 获取USB存储设备的serial number与DevInst
int uc_get_device_serial_number(char *serialnumber, 
								int size, 
								unsigned long *devinst, 
								long deviceNumber, 
								unsigned int devicetype);
// 卸载USB设备
int uc_eject(unsigned long DevInstParent);

// USB存储设备主控制过程
int uc_ctl(char type);
