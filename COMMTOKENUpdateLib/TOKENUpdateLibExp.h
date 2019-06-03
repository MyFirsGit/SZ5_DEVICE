#ifdef TOKENUPDATELIB_EXPORTS
#define TOKENUPDATELIB __declspec(dllexport)
#else
#define TOKENUPDATELIB __declspec(dllimport)
#endif