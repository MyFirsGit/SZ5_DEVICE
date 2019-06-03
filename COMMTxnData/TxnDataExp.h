#ifdef TXNDATA_EXPORTING
#define TXNDATA_API    __declspec(dllexport)
#else
#define TXNDATA_API    __declspec(dllimport)
#endif