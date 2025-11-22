#include "datafile/DataPaths.h"

#include "core/PathUtils.h"

#include <filesystem>

using namespace std;  

namespace pbl2::util {

core::CustomString locateDataDir() {
    static core::CustomString cached;
    if (!cached.isEmpty()) return cached;

    namespace fs = filesystem;
    error_code ec;
    fs::path current = fs::current_path(ec);
    if (ec) current = fs::path();
    fs::path probe = current;

    for (int i = 0; i < 8; ++i) {
        if (const fs::path candidate = probe / "data"; fs::exists(candidate, ec) && fs::is_directory(candidate, ec)) {
            cached = core::path::fromFilesystemPath(candidate);
            return cached;
        }
        if (!probe.has_parent_path()) break;
        probe = probe.parent_path();
    }

    fs::path fallback = current / "data";
    if (fallback.empty()) {
        fallback = fs::path("data");
    }
    fs::create_directories(fallback, ec);
    cached = core::path::fromFilesystemPath(fallback);
    return cached;
}

}
