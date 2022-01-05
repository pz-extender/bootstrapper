#ifndef BOOTSTRAP_STEAM_INSTALL_DIR
#define BOOTSTRAP_STEAM_INSTALL_DIR

#include <optional>
#include <filesystem>

std::optional<std::filesystem::path> locate_steam_installation();

#endif