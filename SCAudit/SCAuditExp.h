#pragma once

#ifdef SCAUDIT_EXPORTS
#define SCAUDIT_API __declspec(dllexport)
#else 
#define SCAUDIT_API __declspec(dllimport) 
#endif

