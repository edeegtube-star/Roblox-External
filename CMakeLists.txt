cmake_minimum_required(VERSION 3.16)
project(UberDelivery LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(UberDelivery
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
)

target_include_directories(UberDelivery PRIVATE src)

if(MSVC)
    target_compile_options(UberDelivery PRIVATE /W4 /O2)
    target_link_libraries(UberDelivery PRIVATE d3d11 dxgi dwmapi)
endif()
