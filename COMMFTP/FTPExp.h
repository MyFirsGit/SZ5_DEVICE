#ifdef FTP_EXPORTS
#define FTP_API __declspec(dllexport)
#else
#define FTP_API __declspec(dllimport)
#endif