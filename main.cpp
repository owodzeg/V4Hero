#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include <filesystem>
#include <iostream>
#include "Engine/V4Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

using namespace std;
namespace fs = std::filesystem;

#ifdef __linux__
#include <X11/Xlib.h>
void setupPlatform() {
    XInitThreads();
}
#else
void setupPlatform() {}
#endif

int main(int argc, char *argv[])
{
    spdlog::cfg::load_env_levels();
    spdlog::set_level(spdlog::level::trace);
    auto rhythm_logger = spdlog::stdout_color_mt("rhythm");

    std::string log_file = "logs/V4Hero-" + std::string(PATAFOUR_VERSION) + ".log";

    /// Create a multi-sink logger to output for console and the logfile
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_st>(log_file));

    // Combine them and set as default
    auto combined_logger = std::make_shared<spdlog::logger>("patalogger", begin(sinks), end(sinks));

    // Setting custom log pattern
    // [30-07-2021 16:01:24] [<start color>main.cpp: main:29<end color>] message
    combined_logger->set_pattern("[%d-%m-%Y %H:%M:%S] [%^%l%$] [%s: %!:%#] %v");
    combined_logger->set_level(spdlog::level::trace);

    // Set the logger as default logger
    spdlog::register_logger(combined_logger);
    spdlog::set_default_logger(combined_logger);

    // note i'm using SPDLOG_x macros, its because they allow to log cpp file, function and line
    SPDLOG_INFO("Starting V4Hero v{}.", std::string(PATAFOUR_VERSION));
    SPDLOG_INFO("SFML version: {}.{}.{}", SFML_VERSION_MAJOR, SFML_VERSION_MINOR, SFML_VERSION_PATCH);
    SPDLOG_INFO("Saving logs to {}", log_file);

    setupPlatform();

    auto executable_path = weakly_canonical(fs::path(argv[0])).parent_path();
    SPDLOG_INFO("Executable directory: {}", executable_path.string());
    //_setmode(_fileno(stdout), _O_U16TEXT);
    //wchar_t * unicode_text = L"aäbcdefghijklmnoöpqrsßtuüvwxyz";
    //wprintf(L"%s", unicode_text);

    fs::current_path(executable_path);

    V4Core V4;
    V4.init();

    return 0;
}
