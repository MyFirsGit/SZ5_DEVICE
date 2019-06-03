#ifdef CS_EXPORTS
#define CS_API __declspec(dllexport)
#else
#define CS_API __declspec(dllimport)
#endif