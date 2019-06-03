#ifdef UPS_EXPORTS
#define UPS_API __declspec(dllexport)
#else
#define UPS_API __declspec(dllimport)
#endif