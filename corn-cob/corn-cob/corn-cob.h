// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the CORNCOB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// CORNCOB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CORNCOB_EXPORTS
#define CORNCOB_API __declspec(dllexport)
#else
#define CORNCOB_API __declspec(dllimport)
#endif

#include <cstdint>

using uint = uint32_t;

extern "C" {
	CORNCOB_API void __stdcall BinkClose(uint* param_1);
	CORNCOB_API uint __stdcall BinkCopyToBufferRect(uint* param_1, uint param_2, uint param_3, int param_4, uint param_5, uint param_6, uint param_7, uint param_8, uint param_9, uint param_10, uint param_11);
	CORNCOB_API uint __stdcall BinkDoFrame(uint* param_1);
	CORNCOB_API void __stdcall BinkNextFrame(int param_1);
	CORNCOB_API int* __stdcall BinkOpen(int* param_1, uint param_2);
	CORNCOB_API uint* __stdcall BinkOpenDirectSound(int* param_1);
	CORNCOB_API uint __stdcall BinkPause(int param_1, int param_2);
	CORNCOB_API bool __stdcall BinkSetSoundSystem(char* param_1, uint param_2);
	CORNCOB_API uint __stdcall BinkWait(int param_1);
}
