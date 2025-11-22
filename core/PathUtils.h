#pragma once

#include "CustomString.h"

#include <filesystem>

namespace pbl2::core::path {

std::filesystem::path toFilesystemPath(const core::CustomString &value);
core::CustomString fromFilesystemPath(const std::filesystem::path &value);
core::CustomString join(const core::CustomString &base, const core::CustomString &child);
bool exists(const core::CustomString &path);
bool ensureDirectory(const core::CustomString &path);

}  // namespace pbl2::core::path
