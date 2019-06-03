#ifdef THUPDATELIB_EXPORTS
#define THUPDATELIB __declspec(dllexport)
#else
#define THUPDATELIB __declspec(dllimport)
#endif