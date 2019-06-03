// 这是主 DLL 文件。

#include "stdafx.h"

#include <io.h>

#include "usbctl_private.h"
#include "usbctl.h"
#include "md5.h"

const int UC_OK = 1;
const int UC_ERROR = 0;
const char UC_INITIALIZATION = 'i';
const char UC_AUTHENTICATION = 'a';

static char AuthenticatedFileName[] = "X:\\AuthenticationFile.dat";

UC_DECLARE(int) uc_initialize()
{
	return uc_ctl(UC_INITIALIZATION);
}

UC_DECLARE(int) uc_authenticate()
{
	return uc_ctl(UC_AUTHENTICATION);
}

int uc_ctl(char type) 
{
	int res = UC_OK;
	vector<uc_removable_logical_device_t> vucrld = uc_get_all_removable_logical_device();
	if (vucrld.size() == 0) {
		if (uc_print_log){
			printf("unable to find any removable storage devices.\n");
		}
		return UC_ERROR;
	}

	vector<uc_removable_logical_device_t>::iterator it = vucrld.begin(), end = vucrld.end();
	for (; it != end; it++) {
		if (uc_print_log)
			printf("[%s] SerialNumber = %s, DeviceType = %d, DeviceNumber = %d, PartitionNumber = %d, DeviceInstParent = %d. \n", 
			it->LogicalDrive, it->SerialNumber, it->DeviceType, it->DeviceNumber, it->PartitionNumber, it->DeviceInstParent);

		string log;
		bool unload = false;
		AuthenticatedFileName[0] = it->LogicalDrive[0];
		// 设备不存在serial number, 立刻将其卸载
		if (strlen(it->SerialNumber) == 0) {
			unload = true;
			log = "device does not have a legal serial number.";
		}
		else {
			if (type == UC_AUTHENTICATION) {
				// 认证过程
				int size =0;
				FILE* file = fopen(AuthenticatedFileName, "rb");
				if (file)
					size = filelength(fileno(file));

				if (size == 16) {			
					char digest[16], digestfile[16];
					MD5_Digest(digest, 16, it->SerialNumber);
					fread(digestfile, 16, 16, file);
					for (int i = 0; i < 16; i++) {
						if (digest[i] != digestfile[i])	{
							unload = true;
							break;
						}
					}
				}
				else
					unload = true;
				if (file)
					fclose(file);
				log = unload ? "authentication failed." : "authentication ok.";
			}
			else if (type == UC_INITIALIZATION) {
				// 初始化过程
				char digest[16];
				MD5_Digest(digest, 16, it->SerialNumber);
				FILE* file = fopen(AuthenticatedFileName, "wb");
				if (file) {
					fwrite(digest, 16, 1, file);
					fclose(file);
				}
				else
					unload = true;

				log = unload ? "initialize failed." : "initialize ok.";
			}
		}

		if (uc_print_log)
			printf("[%s] %s\n", it->LogicalDrive, log.c_str());

		if (unload)	{
			uc_eject(it->DeviceInstParent);
			res = UC_ERROR;
		}
	}
	return res;
}

vector<uc_removable_logical_device_t> uc_get_all_removable_logical_device()
{
	vector<uc_removable_logical_device_t> vucrld;
	char szVolumeAccessPath[] = "\\\\.\\X:";	// "\\.\X:"  -> to open the volume
	char szRootPath[] = "X:\\";					// "X:\\"  -> to open the logical disk
	char drives[MAX_PATH];

	int len = GetLogicalDriveStringsA(MAX_PATH, drives);

	for (int i = 0; i < len; i+=4)
	{
		szRootPath[0] = szVolumeAccessPath[4] = drives[i];
		uc_removable_logical_device_t ucrld;
		// open the storage volume
		HANDLE hVolume = CreateFileA(szVolumeAccessPath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,OPEN_EXISTING, NULL, NULL);
		if (hVolume == INVALID_HANDLE_VALUE) {
			continue;
		}
		// get the volume's device number
		STORAGE_DEVICE_NUMBER sdn;
		DWORD dwBytesReturned = 0;
		long res = DeviceIoControl(hVolume, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &dwBytesReturned, NULL);
		if (res) {
			ucrld.DeviceNumber = sdn.DeviceNumber;
			ucrld.PartitionNumber = sdn.PartitionNumber;
			ucrld.DeviceType = sdn.DeviceType;
		}
		CloseHandle(hVolume);

		if (res) {
			res = uc_get_device_serial_number(ucrld.SerialNumber, MAX_PATH, &ucrld.DeviceInstParent, ucrld.DeviceNumber, ucrld.DeviceType);
		}

		if (res) {
			vector<uc_removable_logical_device_t>::iterator it = vucrld.begin(),
				itend = vucrld.end();
			// only record min partition on the same device
			for (; it != itend; it++) {
				if (it->DeviceNumber == ucrld.DeviceNumber) { 
					if (it->PartitionNumber > ucrld.PartitionNumber) {
						it->DeviceInstParent = ucrld.DeviceInstParent;
						it->PartitionNumber = ucrld.PartitionNumber;
						it->DeviceType = ucrld.DeviceType;
						it->LogicalDrive[0] = szRootPath[0];
					}
					break;
				}
			}

			// not find other partition on the same device, then record it.
			if (it == itend) {
				strcpy(ucrld.LogicalDrive, szRootPath);
				vucrld.push_back(ucrld);
			}
		}
	}

	return vucrld;
}

bool uc_is_usb(char *devid)
{
	return strlen(devid) > 3 && devid[0] == 'U' && devid[1] == 'S' && devid[2] == 'B';
}

bool uc_get_serial_number(char* serial, int size, const char *devid)
{
	string str = devid;
	int pos = str.find_last_of('\\');
	int len = strlen(devid) - pos;
	if (size < len)
		return false;
	memcpy(serial, devid + pos + 1, len);
	return true;
}

int uc_get_device_serial_number(char *serialnumber, int size, unsigned long *devinst, long deviceNumber, unsigned int devicetype)
{
	GUID* guid = (GUID*)&GUID_DEVINTERFACE_DISK;

	// Get device interface info set handle for all devices attached to system
	HDEVINFO hDevInfo = SetupDiGetClassDevs(guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	if (hDevInfo == INVALID_HANDLE_VALUE)	{
		return 0;
	}

	// Retrieve a context structure for a device interface of a device information set
	DWORD dwIndex = 0;
	long res;

	BYTE Buf[1024];
	PSP_DEVICE_INTERFACE_DETAIL_DATA pspdidd = (PSP_DEVICE_INTERFACE_DETAIL_DATA)Buf;
	SP_DEVICE_INTERFACE_DATA         spdid;
	SP_DEVINFO_DATA                  spdd;
	DWORD                            dwSize;

	spdid.cbSize = sizeof(spdid);

	while (true)	{
		res = SetupDiEnumDeviceInterfaces(hDevInfo, NULL, guid, dwIndex, &spdid);
		if (!res) {
			break;
		}

		dwSize = 0;
		SetupDiGetDeviceInterfaceDetail(hDevInfo, &spdid, NULL, 0, &dwSize, NULL); // check the buffer size

		if (dwSize!=0 && dwSize<=sizeof(Buf)) {
			pspdidd->cbSize = sizeof(*pspdidd); // 5 Bytes!
			ZeroMemory(&spdd, sizeof(spdd));
			spdd.cbSize = sizeof(spdd);

			res = SetupDiGetDeviceInterfaceDetail(hDevInfo, &spdid, pspdidd, dwSize, &dwSize, &spdd);

			if (res) {
				// open the disk or cdrom or floppy
				HANDLE hDrive = CreateFile(pspdidd->DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
				if (hDrive != INVALID_HANDLE_VALUE) {
					// get its device number
					STORAGE_DEVICE_NUMBER sdn;
					DWORD dwBytesReturned = 0;
					res = DeviceIoControl(hDrive, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &dwBytesReturned, NULL);
					if (res) {
						// match the given device number with the one of the current device
						if (deviceNumber == (long)sdn.DeviceNumber) {  
							SetupDiDestroyDeviceInfoList(hDevInfo);

							// get serial number and get parent's device instance
							DEVINST DevInstParent = 0;
							CM_Get_Parent(&DevInstParent, spdd.DevInst, 0); 
							char szDeviceIdString[MAX_PATH];
							CM_Get_Device_IDA(DevInstParent, szDeviceIdString, MAX_PATH, 0);

							if (uc_is_usb(szDeviceIdString) 
								&& uc_get_serial_number(serialnumber, size, szDeviceIdString)) {
									CloseHandle(hDrive);
									*devinst = DevInstParent;
									return UC_OK;
							}
						}
					}
					CloseHandle(hDrive);
				}
			}
		}
		dwIndex++;
	}

	SetupDiDestroyDeviceInfoList(hDevInfo);

	return UC_ERROR;
}

int uc_eject(unsigned long DevInstParent)
{
	PNP_VETO_TYPE VetoType = PNP_VetoTypeUnknown; 
	WCHAR VetoNameW[MAX_PATH];
	VetoNameW[0] = 0;
	bool bSuccess = false;
	long res;

	for (long tries = 1; tries <= 10; tries++) { // sometimes we need some tries...

		VetoNameW[0] = 0;

		// CM_Query_And_Remove_SubTree doesn't work for restricted users
		//res = CM_Query_And_Remove_SubTreeW(DevInstParent, &VetoType, VetoNameW, MAX_PATH, CM_REMOVE_NO_RESTART); // CM_Query_And_Remove_SubTreeA is not implemented under W2K!
		//res = CM_Query_And_Remove_SubTreeW(DevInstParent, NULL, NULL, 0, CM_REMOVE_NO_RESTART);  // with messagebox (W2K, Vista) or balloon (XP)

		//res = CM_Request_Device_EjectW(DevInstParent, &VetoType, VetoNameW, MAX_PATH, 0);
		res = CM_Request_Device_EjectW(DevInstParent, NULL, NULL, 0, 0); // with messagebox (W2K, Vista) or balloon (XP)

		bSuccess = (res == CR_SUCCESS && VetoType == PNP_VetoTypeUnknown);
		if (bSuccess)  { 
			break;
		}

		Sleep(500); // required to give the next tries a chance!
	}

	return bSuccess ? UC_OK : UC_ERROR;
}
