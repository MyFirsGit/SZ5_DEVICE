#ifdef BRH_EXPORTS
#define BRH_DECLSPEC    __declspec(dllexport)
#else
#define BRH_DECLSPEC    __declspec(dllimport)
#endif