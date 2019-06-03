#ifdef QRCode_EXPORTS
  #define QRCode_API __declspec(dllexport)
#else
  #define QRCode_API __declspec(dllimport)
#endif