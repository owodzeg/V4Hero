#include "Engine/CoreManager.h"
#include "Engine/V4Core.h"
#include <chrono>
#include <filesystem>
#include <format>
#include <span>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <vector>

namespace fs = std::filesystem;

#ifdef __linux__
#include <X11/Xlib.h>
// necessary for linux if i remember correctly
// TODO: what if: wayland
inline void init_platform()
{
    XInitThreads();
}
#else
// nothing particular needed for other platforms
inline void init_platform()
{
}
#endif

// prepare spdlog stuff
static void setup_logging()
{
    spdlog::cfg::load_env_levels();
    std::vector<spdlog::sink_ptr> sinks{std::make_shared<spdlog::sinks::stdout_color_sink_mt>()};

    std::error_code ec;
    fs::create_directories("logs", ec);

    if (!ec || ec == std::errc::file_exists)
    {
        try
        {
            // monstrous sink creation
            sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>((fs::path("logs") / std::format("{}-V4Hero-{}.log", std::format("{:%Y-%m-%d_%H-%M-%S}", std::chrono::current_zone()->to_local(std::chrono::system_clock::now())), PATAFOUR_VERSION)).string(), true));
        } 
        catch (...)
        {
            SPDLOG_WARN("Log creation failed!!!!!");
        }
    }

    // make sure it writes to file
    spdlog::set_default_logger(std::make_shared<spdlog::logger>("patalogger", sinks.begin(), sinks.end()));
}

// we NEED to have our WD in Patafour's dir no matter where and from where we launch.
static bool set_working_directory(const char* path)
{
    if (!path || !*path)
        return false;

    std::error_code ec;
    auto exe = fs::weakly_canonical(path, ec);

    if (ec)
    {
        SPDLOG_ERROR("Failed to resolve executable path: {}", ec.message());
        return false;
    }

    fs::current_path(exe.parent_path(), ec);

    if (ec)
    {
        SPDLOG_ERROR("Failed to change working directory: {}", ec.message());
        return false;
    }

    SPDLOG_INFO("Working directory set to: {}", exe.parent_path().string());
}

int main([[maybe_unused]] int argc, char* argv[])
{
    setup_logging();
    init_platform();

    SPDLOG_INFO("V4Hero v{}.", PATAFOUR_VERSION);
    SPDLOG_INFO("SFML: {}.{}.{}", SFML_VERSION_MAJOR, SFML_VERSION_MINOR, SFML_VERSION_PATCH);

    if (!set_working_directory(argv[0]))
        return 1; // can't run without proper workdir..

    try
    {
        auto& core = CoreManager::getInstance();
        core.init();
        core.getCore()->init();
    } 
    catch (const std::exception& e)
    {
        SPDLOG_CRITICAL("Something nasty happened: {}", e.what());
        return 1;
    } 
    catch (...)
    {
        SPDLOG_CRITICAL("Something absolutely horrible happened");
        return 1;
    }
    return 0;
}