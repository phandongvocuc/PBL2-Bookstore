#pragma once

#include "../core/CustomString.h"
namespace pbl2::model {

core::CustomString canonicalBookStatus(const core::CustomString &status);
core::CustomString availabilityStatusForQuantity(int quantity);
bool isAvailabilityStatus(const core::CustomString &status);

}  // namespace model
