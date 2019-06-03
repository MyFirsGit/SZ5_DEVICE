#ifdef BRH_EXPORTS
#define BRH_API __declspec(dllexport)
#else
#define BRH_API __declspec(dllimport)
#endif