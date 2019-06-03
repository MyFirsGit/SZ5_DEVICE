#ifdef STATUSDISPLAY_EXPORTS
  #define STATUS_DISPLAY_API __declspec(dllexport)
#else
  #define STATUS_DISPLAY_API __declspec(dllimport)
#endif