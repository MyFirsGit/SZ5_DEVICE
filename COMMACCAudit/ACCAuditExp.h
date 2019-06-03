#ifdef ACCAUDIT_EXPORTING
#define ACCAUDIT_DECLSPEC    __declspec(dllexport)
#else
#define ACCAUDIT_DECLSPEC    __declspec(dllimport)
#endif
