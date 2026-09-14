#include "silentaim.h"
#include "../sdk/offsets.h"
#include "../sdk/structures.h"
#include <cmath>
#include <cfloat>

namespace SilentAim {

// Roblox silent aim is different from FiveM bullet redirection.
// Typical external approaches:
// 1. Write mouse hit / viewport ray end position
// 2. Override camera look vector for the shot frame only
// 3. Spoof the input ray used by tools / guns
// Exact method depends on the experience (guns vs tools).

void Update(Memory& mem) {
    if (!settings.enabled) return;

    // Flow:
    // 1. Find closest valid entity inside FOV
    // 2. Select preferred bone (head / torso)
    // 3. Optional prediction
    // 4. Write the silent target position / ray
    // 5. Restore on release / invalid target

    (void)mem;
}

} // namespace SilentAim
