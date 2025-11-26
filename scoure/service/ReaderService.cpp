#include "service/ReaderService.h"

namespace pbl2::service {

    bool ReaderService::isDuplicatePhoneOrIdentityCard(const core::CustomString &phone, const core::CustomString &identityCard) const {
        auto readers = ensureLoaded();
        for (const auto &reader : readers) {
            // Kiểm tra số điện thoại
            if (reader.getPhone().compare(phone, core::CaseSensitivity::Insensitive) == 0) {
                return true;
            }
            // Kiểm tra căn cước công dân
            if (reader.getIdentityCard().compare(identityCard, core::CaseSensitivity::Insensitive) == 0) {
                return true;
            }
        }
        return false;
    }
    ReaderService::ReaderService(const core::CustomString &dataDir)
        : BaseService(dataDir) {}

    // Simply forward to BaseService implementations
    core::DynamicArray<model::Reader> ReaderService::fetchAll() const {
        return BaseService::fetchAll();
    }

    core::Optional<model::Reader> ReaderService::findById(const core::CustomString &readerId) const {
        return BaseService::findById(readerId);
    }

    bool ReaderService::addReader(const model::Reader &reader) const {
        return addItem(reader);
    }

    bool ReaderService::updateReader(const model::Reader &reader) const {
        return updateItem(reader);
    }

    bool ReaderService::setReaderActive(const core::CustomString &readerId, bool active) const {
        auto readers = ensureLoaded();
        bool changed = false;
        for (auto &reader : readers) {
            if (reader.getId().compare(readerId, core::CaseSensitivity::Insensitive) == 0) {
                if (reader.isActive() == active) return true; // No change needed
                reader.setActive(active);
                changed = true;
                break;
            }
        }
        if (!changed) return false;
        persist(readers);
        return true;
    }

    bool ReaderService::removeReader(const core::CustomString &readerId) const {
        return removeItem(readerId);
    }

    core::DynamicArray<model::Reader> ReaderService::ensureLoaded() const {
        return repository.loadAll();
    }

    void ReaderService::persist(const core::DynamicArray<model::Reader> &readers) const {
        repository.saveAll(readers);
    }

} // namespace pbl2::service
