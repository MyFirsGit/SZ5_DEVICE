#ifdef TXNAUDIT_EXPORTING
#define TXNAUDIT_DECLSPEC    __declspec(dllexport)
#else
#define TXNAUDIT_DECLSPEC    __declspec(dllimport)
#endif
