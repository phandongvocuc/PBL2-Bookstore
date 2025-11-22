#include "model/Reader.h"
using namespace std;
namespace pbl2::model {

    const core::CustomString &Reader::getIdentityCard() const {
        return identityCard;
    }

    void Reader::setIdentityCard(const core::CustomString &value) {
        identityCard = value;
    }

    const core::Date &Reader::getCreatedDate() const {
        return createdDate;
    }

    void Reader::setCreatedDate(const core::Date &value) {
        createdDate = value;
    }

    const core::Date &Reader::getExpiryDate() const {
        return expiryDate;
    }

    void Reader::setExpiryDate(const core::Date &value) {
        expiryDate = value;
    }

    int Reader::getTotalBorrowed() const {
        return totalBorrowed;
    }

    void Reader::setTotalBorrowed(const int value) {
        totalBorrowed = value;
    }

    const core::CustomString &Reader::getNotes() const {
        return notes;
    }

    void Reader::setNotes(const core::CustomString &value) {
        notes = value;
    }

}  // namespace pbl2::model