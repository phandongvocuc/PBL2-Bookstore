#include "core/PathUtils.h"

#include <string>
#include <system_error>
using namespace std;
namespace pbl2::core::path {

namespace {

core::CustomString fromStdString(const string &value) {
    if (value.empty()) return {};
    return {value.c_str(), static_cast<core::CustomString::SizeType>(value.size())};
}

}  // namespace

filesystem::path toFilesystemPath(const core::CustomString &value) {
    if (value.isEmpty()) return {};
    return {value.cStr()};
}

core::CustomString fromFilesystemPath(const filesystem::path &value) {
    const string converted = value.string();
    return fromStdString(converted);
}

core::CustomString join(const core::CustomString &base, const core::CustomString &child) {
    const filesystem::path combined = toFilesystemPath(base) / toFilesystemPath(child);
    return fromFilesystemPath(combined);
}

bool exists(const core::CustomString &path) {
    error_code ec;
    const bool present = filesystem::exists(toFilesystemPath(path), ec);
    return !ec && present;
}

bool ensureDirectory(const core::CustomString &path) {
    error_code ec;
    const filesystem::path fsPath = toFilesystemPath(path);
    if (fsPath.empty()) return false;
    if (filesystem::exists(fsPath, ec)) {
        return !ec;
    }
    filesystem::create_directories(fsPath, ec);
    return !ec;
}

}  // namespace pbl2::core::path
