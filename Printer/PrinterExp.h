#ifdef PRINTER_EXPORTS
  #define PRINTER_API __declspec(dllexport)
#else
  #define PRINTER_API __declspec(dllimport)
#endif