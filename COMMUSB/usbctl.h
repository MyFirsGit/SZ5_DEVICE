// usbctl.h

#ifndef UC_USBCTL
#define UC_USBCTL

#ifdef UC_DECLARE_DLLEXPORT
#define UC_DECLARE(type) extern "C" __declspec(dllexport) type __stdcall
#else
#define UC_DECLARE(type) extern "C" __declspec(dllimport) type __stdcall
#endif

static bool uc_print_log = false;

// ��ʼ��usb�豸
// return 0:error; 1:OK
UC_DECLARE(int) uc_initialize();

// ��֤usb�豸
// return 0:error; 1:OK
UC_DECLARE(int)  uc_authenticate();

#endif

