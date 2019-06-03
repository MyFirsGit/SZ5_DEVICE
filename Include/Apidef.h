#ifdef _LIB_EXPORTS
#define DLLIMP __declspec(dllexport)
#else
#define DLLIMP __declspec(dllimport)
#endif

