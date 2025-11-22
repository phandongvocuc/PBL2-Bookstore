#include "service/ReaderService.h"

namespace pbl2::service {

    ReaderService::ReaderService(const custom::CustomString &dataDir)
        : BaseService(dataDir) {}

    // Simply forward to BaseService implementations
    custom::DynamicArray<model::Reader> ReaderService::fetchAll() const {
        return BaseService::fetchAll();
    }

    custom::Optional<model::Reader> ReaderService::findById(const custom::CustomString &readerId) const {
        return BaseService::findById(readerId);
    }

    bool ReaderService::addReader(const model::Reader &reader) const {
        return addItem(reader);
    }

    bool ReaderService::updateReader(const model::Reader &reader) const {
        return updateItem(reader);
    }

    bool ReaderService::setReaderActive(const custom::CustomString &readerId, bool active) const {
        const auto readers = ensureLoaded();
        bool changed = false;
        for (auto &reader : readers) {
            if (reader.getId().compare(readerId, custom::CaseSensitivity::Insensitive) == 0) {
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

    bool ReaderService::removeReader(const custom::CustomString &readerId) const {
        return removeItem(readerId);
    }

    custom::DynamicArray<model::Reader> ReaderService::ensureLoaded() const {
        return repository.loadAll();
    }

    void ReaderService::persist(const custom::DynamicArray<model::Reader> &readers) const {
        repository.saveAll(readers);
    }

} // namespace pbl2::service