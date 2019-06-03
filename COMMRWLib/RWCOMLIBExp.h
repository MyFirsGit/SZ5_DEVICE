#ifdef RW_EXPORTS
   #define RWCOMLIB_DECLSPEC    __declspec(dllexport)
#else
   #define RWCOMLIB_DECLSPEC    __declspec(dllimport)
#endif