#ifdef CH_EXPORTS
	#define CH_API __declspec(dllexport)
#else
	#define CH_API __declspec(dllimport)
#endif