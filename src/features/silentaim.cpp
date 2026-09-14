#include "silentaim.h"
#include "../sdk/offsets.h"

namespace SilentAim {

void Update(Memory& mem) {
    if (!settings.enabled) return;

    // Silent aim redirects the shot vector / mouse hit without moving camera
    // Requires intercepting the ray or writing to the hit position buffer
    // Implementation depends on current client structures

    (void)mem;
}

}
