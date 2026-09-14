#include "esp.h"
#include "../sdk/offsets.h"
#include "../render/overlay.h"
#include <cmath>

namespace ESP {

void Update(Memory& mem) {
    players.clear();
    if (!settings.enabled) return;

    // Entity enumeration using offsets:
    // VisualEngine -> DataModel -> Workspace / Players
    // Players children -> each Player -> ModelInstance (Character)
    // Character -> Humanoid, HumanoidRootPart, Head
    // Read Position from Primitive, Health from Humanoid
    // WorldToScreen via ViewMatrix

    (void)mem;
}

void Render() {
    if (!settings.enabled) return;

    for (const auto& p : players) {
        if (!p.isValid) continue;
        if (settings.teamCheck && p.isTeammate) continue;

        // Box styles
        if (settings.box2D) {
            // Overlay::DrawBox(...)
        }
        if (settings.boxCorner) {
            // Overlay::DrawCornerBox(...)
        }
        if (settings.box3D) {
            // 3D box from part size + rotation matrix
        }

        if (settings.name) {
            // Overlay::DrawText name
        }
        if (settings.distance) {
            // Overlay::DrawText distance m
        }
        if (settings.skeleton) {
            // bone lines: Head-Neck-Torso-Pelvis + limbs
        }
        if (settings.healthBar) {
            // vertical/horizontal bar from health/maxHealth
        }
    }
}

} // namespace ESP
