#ifdef TVMDB_EXPORTS
#define TVMDB_API __declspec(dllexport)
#else
#define TVMDB_API __declspec(dllimport)
#endif