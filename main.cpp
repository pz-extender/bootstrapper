#include "steam_install_dir.h"

#include <vdf_parser.hpp>

#include <array>
#include <iostream>
#include <fstream>

template <typename... Names>
auto get_or_create_vdf_section(tyti::vdf::object& root, Names... names) -> std::shared_ptr<tyti::vdf::object>
{
    std::array<std::basic_string<char>, sizeof...(Names)> names_array = { (names)... };

    auto children = &root.childs;
    std::shared_ptr<tyti::vdf::object> obj = nullptr;

    for (auto& name : names_array) {
        if (!children->contains(name)) {
            children->insert(std::make_pair(name, std::make_shared<tyti::vdf::object>()));
        }

        obj = children->at(name);
        children = &obj->childs;
    }

    return obj;
}


int main(int argc, char *argv[])
{
    if (argc < 3) {
        return -1;
    }

    namespace fs = std::filesystem;

    auto steam_install_path = locate_steam_installation();
    if (steam_install_path) {
        std::cout << steam_install_path->string() << std::endl;
    }

    auto userdata_collection_path = *steam_install_path / "userdata";
    for (const auto& userdata_entry : fs::directory_iterator(userdata_collection_path)) {
        if (!userdata_entry.is_directory()) {
            continue;
        }

        auto localconfig_path = userdata_entry.path() / "config" / "localconfig.vdf";
        if (!fs::exists(localconfig_path)) {
            continue;
        }

        auto localconfig_reader = std::ifstream(localconfig_path);
        auto localconfig = tyti::vdf::read(localconfig_reader);
        localconfig_reader.close();

        auto zomboid_config = get_or_create_vdf_section(localconfig, "Software", "valve", "Steam", "apps", argv[1]);
        zomboid_config->attribs["LaunchOptions"] = argv[2];

        auto localconfig_backup_path = localconfig_path;
        localconfig_backup_path.replace_extension(".old");

        if (fs::exists(localconfig_backup_path)) {
            fs::remove(localconfig_backup_path);
        }

        fs::rename(localconfig_path, localconfig_backup_path);
        
        auto localconfig_writer = std::ofstream(localconfig_path, std::ios::trunc);
        tyti::vdf::write(localconfig_writer, localconfig);
    }

    return 0;
}
