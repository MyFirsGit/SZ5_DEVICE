#ifdef TOKENLIB_EXPORTS
#define TOKENCOMLIB_DECLSPEC __declspec(dllexport)
#else
#define TOKENCOMLIB_DECLSPEC __declspec(dllimport)
#endif