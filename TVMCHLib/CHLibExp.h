#ifdef CHLIB_EXPORTS
#define DLLIMP __declspec(dllexport)
#else
#define DLLIMP __declspec(dllimport)
#endif
