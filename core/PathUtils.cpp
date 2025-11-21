#include "core/PathUtils.h"

#include <string>
#include <system_error>
using namespace std;
namespace pbl2::core::path {

namespace {

custom::CustomString fromStdString(const string &value) {
    if (value.empty()) return {};
    return {value.c_str(), static_cast<custom::CustomString::SizeType>(value.size())};
}

}  // namespace

filesystem::path toFilesystemPath(const custom::CustomString &value) {
    if (value.isEmpty()) return {};
    return {value.cStr()};
}

custom::CustomString fromFilesystemPath(const filesystem::path &value) {
    const string converted = value.string();
    return fromStdString(converted);
}

custom::CustomString join(const custom::CustomString &base, const custom::CustomString &child) {
    const filesystem::path combined = toFilesystemPath(base) / toFilesystemPath(child);
    return fromFilesystemPath(combined);
}

bool exists(const custom::CustomString &path) {
    error_code ec;
    const bool present = filesystem::exists(toFilesystemPath(path), ec);
    return !ec && present;
}

bool ensureDirectory(const custom::CustomString &path) {
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
