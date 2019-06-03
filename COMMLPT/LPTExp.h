#ifdef LPT_EXPORTS
#define LPT_API __declspec(dllexport)
#else
#define LPT_API __declspec(dllimport)
#endif