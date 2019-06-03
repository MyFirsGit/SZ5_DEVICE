
#ifdef SC_EXPORTS
#define SC_API __declspec(dllexport)
#else
#define SC_API __declspec(dllimport)
#endif