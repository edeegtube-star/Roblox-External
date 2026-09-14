#include "streamproof.h"

#ifndef WDA_EXCLUDEFROMCAPTURE
#define WDA_EXCLUDEFROMCAPTURE 0x00000011
#endif

namespace Streamproof {

bool Enable(HWND hwnd) {
    // Blocks OBS (WGC + DXGI), Discord, Game Bar, most software capture
    // NVIDIA Shadowplay / AMD ReLive (NVFBC/AMFBC) require kernel/DMA for full coverage
    return SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);
}

void Disable(HWND hwnd) {
    SetWindowDisplayAffinity(hwnd, WDA_NONE);
}

}
