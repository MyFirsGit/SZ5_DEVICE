#ifdef RW_EXPORTS
#define RW_API __declspec(dllexport)
#else
#define RW_API __declspec(dllimport)
#endif