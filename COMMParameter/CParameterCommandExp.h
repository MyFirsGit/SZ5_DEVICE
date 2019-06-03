#ifdef PARAMETERCOMMAND_EXPORTING
#define PARAMETERCOMMAND_DECLSPEC    __declspec(dllexport)
#else
#define PARAMETERCOMMAND_DECLSPEC    __declspec(dllimport)
#endif

#include "ParameterDef.h"