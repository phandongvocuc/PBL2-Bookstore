#pragma once

#include "CustomString.h"
#include "DynamicArray.h"

namespace pbl2::core {

class IdGenerator {
public:
    static core::CustomString nextId(const core::DynamicArray<core::CustomString> &existing,
                                       const core::CustomString &prefix,
                                       int width = 4);
};

}  // namespace pbl2::core
