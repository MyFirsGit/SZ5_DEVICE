#ifdef TVMBHCHANGE_EXPORTS
	#define BH_API __declspec(dllexport)
#else
	#define BH_API __declspec(dllimport)
#endif