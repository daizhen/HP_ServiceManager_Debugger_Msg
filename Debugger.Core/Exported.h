#ifdef HookDll

// MYLIBAPI should be defined in all of the DLL's source
// code modules before this header file is included.

// All functions/variables are being exported.

#else

// This header file is included by an EXE source code module.
// Indicate that all functions/variables are being imported.
#define HookDll extern "C" __declspec(dllimport)

#endif
#include "string"
using namespace std;
HookDll void Test();