#ifdef TH_EXPORTS
#define TH_API __declspec(dllexport)
#else
#define TH_API __declspec(dllimport)
#endif