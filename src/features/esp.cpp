#include "esp.h"
#include "../sdk/offsets.h"

namespace ESP {

void Update(Memory& mem) {
    players.clear();
    if (!settings.enabled) return;

    // Entity list walk:
    // Read Players service -> children
    // For each player: Character, Humanoid, RootPart, Head, Team, Health
    // WorldToScreen using ViewMatrix
    // Push into players vector

    (void)mem;
}

void Render() {
    if (!settings.enabled) return;

    for (const auto& p : players) {
        if (!p.isValid) continue;
        if (settings.teamCheck && p.isTeammate) continue;

        // Draw based on active box style
        // 2D / 3D / Corner
        // Name + Distance text
        // Skeleton lines between joints
        // Health bar
    }
}

}
