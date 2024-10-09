cmake_minimum_required(VERSION 3.19)
set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "Vcpkg target triplet")

project(V4Hero VERSION 1.2)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(!MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
endif()

# make sure everything is handled statically on msvc
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_definitions(-DPATAFOUR_VERSION="${CMAKE_PROJECT_VERSION}")

set(SFML_STATIC_LIBRARIES TRUE)

find_package(DiscordSDK 2.5.6 COMPONENTS x86_64 REQUIRED)

include(FetchContent)

if(MSVC)
    find_package(SFML REQUIRED COMPONENTS graphics audio)
    find_package(nlohmann_json REQUIRED)
else()
    FetchContent_Declare(
            SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 2.6.1)
    FetchContent_MakeAvailable(SFML)

    FetchContent_Declare(json
            GIT_REPOSITORY https://github.com/nlohmann/json
            GIT_TAG v3.11.3)
    FetchContent_GetProperties(json)

    if(NOT json_POPULATED)
        FetchContent_Populate(json)
        add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
endif()


FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.13.0)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(libzippp
        GIT_REPOSITORY https://github.com/ctabin/libzippp
        GIT_TAG libzippp-v7.1-1.10.1)
FetchContent_MakeAvailable(libzippp)

set(ENGINE_FILES
        Engine/Func.cpp
        Engine/V4Core.cpp
        Engine/Item/Item.cpp
        Engine/Item/InventoryData.cpp
        Engine/Item/Equipment.cpp
        Engine/Item/ItemRegistry.cpp
        Engine/Dialog/ControlTips.cpp
        Engine/Dialog/RoundedRect.cpp
        Engine/Dialog/MessageCloud.cpp
        Engine/Dialog/DialogBox.cpp
        Engine/Binary.cpp
        Engine/Menu/MainMenu.cpp
        Engine/Menu/Barracks.cpp
        Engine/Menu/IntroductionMenu.cpp
        Engine/Menu/Altar.cpp
        Engine/Menu/Patapolis.cpp
        Engine/Menu/Credits.cpp
        Engine/Menu/Obelisk.cpp
        Engine/Menu/OptionsMenu.cpp
        Engine/Menu/MaterOuter.cpp
        Engine/Menu/LoadingTip.cpp
        Engine/Graphics/PSpritesheet.cpp
        Engine/Graphics/PSprite.cpp
        Engine/Graphics/SpriteWrapper.cpp
        Engine/Graphics/Menu.cpp
        Engine/Graphics/PText.cpp
        Engine/Graphics/RichText.cpp
        Engine/Graphics/ScreenFade.cpp
        Engine/Math/PVector.cpp
        Engine/SaveReader.cpp
        Engine/Input/InputController.cpp
        Engine/Input/TextInputController.cpp
        Engine/Input/MouseController.cpp
        Engine/Rhythm/RhythmGUI.cpp
        Engine/Rhythm/RhythmController.cpp
        Engine/Rhythm/SongController.cpp
        Engine/Rhythm/Drum.cpp
        Engine/Rhythm/Rhythm.cpp
        Engine/Pon/Pon.cpp
        Engine/Pon/PonRegistry.cpp
        Engine/Pon/Squad.cpp
        Engine/Pon/SquadRegistry.cpp
        Engine/StringRepository.cpp
        Engine/Config.cpp
        Engine/TipsUtil.cpp
        Engine/P4A.cpp
        Engine/ResourceManager.cpp
        Engine/TextureManager.cpp
        Engine/CoreManager.cpp
        Engine/StateManager.cpp
        Engine/Mission/Background.cpp
        Engine/Mission/Camera.cpp
        Engine/Mission/MissionController.cpp
        Engine/Mission/Weather.cpp
        Engine/Mission/Units/CollidableObject.cpp
        Engine/Mission/Units/AnimatedObject.cpp
        Engine/Mission/Units/Hitbox.cpp
        Engine/Mission/Units/Object.cpp
        Engine/Mission/Units/Projectile.cpp
        Engine/Mission/Units/Entity.cpp
        Engine/Mission/Units/PlayableUnit.cpp
        Engine/Mission/Units/HitboxFrame.cpp
        Engine/Mission/Mission.cpp
        Engine/Utils.h
        Engine/Graphics/CurveShape.cpp
        Engine/Graphics/CurveSegment.cpp
        Engine/Graphics/ConcaveShape.cpp
        Engine/Graphics/AnimatedCurveShape.cpp
        Engine/Graphics/PNGAnimation.cpp
        Engine/Menu/TestChamber.cpp
        Engine/Menu/ErrorChamber.cpp
        Engine/Mechanics/Globals.cpp
        Engine/Mission/Units/Unit/Hatapon.cpp
        Engine/Mission/Units/Unit/Yaripon.cpp
        Engine/Mission/Units/Behavior.cpp
        Engine/Mission/Units/Behavior.h)

set(DISCORD_FILES
        DiscordSDK/achievement_manager.cpp
        DiscordSDK/activity_manager.cpp
        DiscordSDK/application_manager.cpp
        DiscordSDK/core.cpp
        DiscordSDK/image_manager.cpp
        DiscordSDK/lobby_manager.cpp
        DiscordSDK/network_manager.cpp
        DiscordSDK/overlay_manager.cpp
        DiscordSDK/relationship_manager.cpp
        DiscordSDK/storage_manager.cpp
        DiscordSDK/store_manager.cpp
        DiscordSDK/types.cpp
        DiscordSDK/user_manager.cpp
        DiscordSDK/voice_manager.cpp)

add_executable(V4Hero ${ENGINE_FILES} ${DISCORD_FILES} main.cpp)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Enabling Unity build for Release build")
    set_target_properties(V4Hero PROPERTIES
            UNITY_BUILD ON
            UNITY_BUILD_BATCH_SIZE 5)
endif()

if(MSVC)
    add_definitions(-DLEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
    target_compile_options(V4Hero PRIVATE /W4 /std:c++20)
    target_compile_definitions(V4Hero PRIVATE _USE_MATH_DEFINES=1)

else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
    target_compile_options(V4Hero PRIVATE -Wall -Wextra)
endif()

target_include_directories(V4Hero PRIVATE Engine/)
target_include_directories(V4Hero PRIVATE DiscordSDK)

target_link_libraries(V4Hero PRIVATE
        sfml-graphics
        sfml-audio
        DiscordSDK
        nlohmann_json::nlohmann_json
        spdlog::spdlog
        libzippp)

if (OS STREQUAL "Linux")
    find_package(X11 REQUIRED)
    target_link_libraries(V4Hero PRIVATE X11::X11)
endif (OS STREQUAL "Linux")

if (NOT (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR))
    add_custom_command(
            TARGET V4Hero POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/resources ${PROJECT_BINARY_DIR}/resources
            DEPENDS "${PROJECT_BINARY_DIR}/resources"
    )
endif()
