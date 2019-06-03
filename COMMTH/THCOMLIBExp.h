#ifdef TH_EXPORTS
   #define THCOMLIB_DECLSPEC    __declspec(dllexport)
#else
   #define THCOMLIB_DECLSPEC    __declspec(dllimport)
#endif