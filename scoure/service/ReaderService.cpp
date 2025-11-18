#include "service/ReaderService.h"

using namespace std;

namespace pbl2::service {

ReaderService::ReaderService(const custom::CustomString &dataDir) : repository(dataDir) {}

custom::DynamicArray<model::Reader> ReaderService::fetchAll() const { return ensureLoaded(); }

custom::Optional<model::Reader> ReaderService::findById(const custom::CustomString &readerId) const {
    const auto trimmed = readerId.trimmed();
    if (trimmed.isEmpty()) return custom::Optional<model::Reader>();
    const auto readers = ensureLoaded();
    if (readers.isEmpty()) return custom::Optional<model::Reader>();
    for (custom::DynamicArray<model::Reader>::ConstIterator it = readers.cbegin(); it != readers.cend(); ++it) {
        if (it->getId().compare(trimmed, custom::CaseSensitivity::Insensitive) == 0) {
            return custom::Optional(*it);
        }
    }
    return custom::Optional<model::Reader>();
}

bool ReaderService::addReader(const model::Reader &reader) const {
    auto readers = ensureLoaded();

    const model::Reader copy = reader;
    bool exists = false;
    for (custom::DynamicArray<model::Reader>::ConstIterator it = readers.cbegin(); it != readers.cend(); ++it) {
        if (it->getId().compare(copy.getId(), custom::CaseSensitivity::Insensitive) == 0) {
            exists = true;
            break;
        }
    }
    if (exists) return false;

    readers.pushBack(copy);
    persist(readers);
    return true;
}

bool ReaderService::updateReader(const model::Reader &reader) const {
    auto readers = ensureLoaded();
    bool updated = false;
    for (custom::DynamicArray<model::Reader>::Iterator it = readers.begin(); it != readers.end(); ++it) {
        if (it->getId().compare(reader.getId(), custom::CaseSensitivity::Insensitive) == 0) {
            *it = reader;
            updated = true;
            break;
        }
    }
    if (!updated) return false;
    persist(readers);
    return true;
}

bool ReaderService::setReaderActive(const custom::CustomString &readerId, const bool active) const {
    auto readers = ensureLoaded();
    bool changed = false;
    for (custom::DynamicArray<model::Reader>::Iterator it = readers.begin(); it != readers.end(); ++it) {
        if (it->getId().compare(readerId, custom::CaseSensitivity::Insensitive) == 0) {
            if (it->isActive() == active) return true;
            it->setActive(active);
            changed = true;
            break;
        }
    }
    if (!changed) return false;
    persist(readers);
    return true;
}

bool ReaderService::removeReader(const custom::CustomString &readerId) const {
    auto readers = ensureLoaded();
    bool removed = false;
    custom::DynamicArray<model::Reader>::SizeType index = 0U;
    while (index < readers.size()) {
        if (readers[index].getId().compare(readerId, custom::CaseSensitivity::Insensitive) == 0) {
            readers.removeAt(index);
            removed = true;
        } else {
            ++index;
        }
    }
    if (!removed) return false;
    persist(readers);
    return true;
}

custom::DynamicArray<model::Reader> ReaderService::ensureLoaded() const {
    return repository.loadAll();
}

void ReaderService::persist(const custom::DynamicArray<model::Reader> &readers) const {
    repository.saveAll(readers);
}

}  // namespace pbl2::service
