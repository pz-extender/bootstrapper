#include "steam_install_dir.h"

#include <windows.h>

namespace fs = std::filesystem;

struct RegKey {
    RegKey(HKEY parent, LPCWSTR subkey)
    {
        status = RegOpenKeyExW(parent, subkey, 0, KEY_READ, &handle);
    }

    ~RegKey()
    {
        if (status == 0) {
            RegCloseKey(handle);
        }
    }

    LSTATUS status;
    HKEY handle;
};

std::optional<fs::path> locate_steam_installation()
{
    WCHAR buffer[MAX_PATH];
    DWORD buffer_size = MAX_PATH;

    RegKey key(HKEY_CURRENT_USER, L"SOFTWARE\\Valve\\Steam");
    if (key.status != ERROR_SUCCESS) {
        return std::nullopt;
    }

    LSTATUS read_status = RegQueryValueExW(key.handle, L"SteamPath", 0, nullptr, (LPBYTE)buffer, &buffer_size);
    if (read_status != ERROR_SUCCESS) {
        return std::nullopt;
    }

    return fs::path(std::wstring(buffer, buffer_size  / sizeof(wchar_t) - 1));
}