#include "menu.h"
#include "../features/aimbot.h"
#include "../features/silentaim.h"
#include "../features/triggerbot.h"
#include "../features/esp.h"

namespace Menu {

static bool open = true;

void Initialize() {}

void Toggle() { open = !open; }

bool IsOpen() { return open; }

void Render() {
    if (!open) return;

    // ImGui or custom menu rendering goes here
    // Tabs: Combat | Visuals | Stealth | Config
    // Bind all Settings structs
}

}
