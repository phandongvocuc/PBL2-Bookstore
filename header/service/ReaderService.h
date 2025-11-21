#pragma once

#include "core/custom/Optional.h"

#include "core/custom/CustomString.h"

#include "core/custom/DynamicArray.h"

#include "model/Reader.h"
#include "repository/ReadersRepository.h"

namespace pbl2::service {

class ReaderService {
public:
    explicit ReaderService(const custom::CustomString &dataDir);

    [[nodiscard]] custom::DynamicArray<model::Reader> fetchAll() const;
    [[nodiscard]] custom::Optional<model::Reader> findById(const custom::CustomString &readerId) const;
    [[nodiscard]] bool addReader(const model::Reader &reader) const;
    [[nodiscard]] bool updateReader(const model::Reader &reader) const;
    [[nodiscard]] bool setReaderActive(const custom::CustomString &readerId, bool active) const;
    [[nodiscard]] bool removeReader(const custom::CustomString &readerId) const;

private:
    repository::ReadersRepository repository;

    [[nodiscard]] custom::DynamicArray<model::Reader> ensureLoaded() const;
    void persist(const custom::DynamicArray<model::Reader> &readers) const;
};

}  // namespace pbl2::service
