cmake_minimum_required(VERSION 3.16)
project(UberDelivery LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ImGui sources - clone Dear ImGui into third_party/imgui before building
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
    ${IMGUI_DIR}/backends/imgui_impl_dx11.cpp
)

add_executable(UberDelivery WIN32
    src/main.cpp
    src/memory/memory.cpp
    src/features/aimbot.cpp
    src/features/esp.cpp
    src/features/triggerbot.cpp
    src/features/silentaim.cpp
    src/render/overlay.cpp
    src/menu/menu.cpp
    src/stealth/streamproof.cpp
    src/stealth/selfdestruct.cpp
    ${IMGUI_SOURCES}
)

target_include_directories(UberDelivery PRIVATE
    src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

if(MSVC)
    target_compile_options(UberDelivery PRIVATE /W3 /O2 /MT)
    target_link_libraries(UberDelivery PRIVATE d3d11 dxgi dwmapi)
endif()
