// corn-cob.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "corn-cob.h"

CORNCOB_API void __stdcall BinkClose(uint* param_1)
{
    return;
}

CORNCOB_API uint __stdcall BinkCopyToBufferRect(uint* param_1, uint param_2, uint param_3, int param_4, uint param_5, uint param_6, uint param_7, uint param_8, uint param_9, uint param_10, uint param_11)
{
    return uint();
}

CORNCOB_API uint __stdcall BinkDoFrame(uint* param_1)
{
    return uint();
}

CORNCOB_API void __stdcall BinkNextFrame(int param_1)
{
    return;
}

CORNCOB_API int* __stdcall BinkOpen(int* param_1, uint param_2)
{
    return nullptr;
}

CORNCOB_API uint* __stdcall BinkOpenDirectSound(int* param_1)
{
    return nullptr;
}

CORNCOB_API uint __stdcall BinkPause(int param_1, int param_2)
{
    return uint();
}

CORNCOB_API bool __stdcall BinkSetSoundSystem(char* param_1, uint param_2)
{
    return false;
}

CORNCOB_API uint __stdcall BinkWait(int param_1)
{
    return uint();
}
