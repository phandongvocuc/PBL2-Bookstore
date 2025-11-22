#pragma once

#include "../../core/Optional.h"
#include "../../core/DynamicArray.h"
#include "../../core/CustomString.h"

namespace pbl2::service {

    template<typename T, typename RepositoryT>
    class BaseService {
    public:
        virtual ~BaseService() = default;

        explicit BaseService(const core::CustomString &dataDir);

        // Only the essential CRUD methods that ALL models need
        [[nodiscard]] virtual core::DynamicArray<T> fetchAll() const;
        [[nodiscard]] virtual core::Optional<T> findById(const core::CustomString &id) const;
        [[nodiscard]] bool addItem(const T &item) const;
        [[nodiscard]] bool updateItem(const T &item) const;
        [[nodiscard]] bool removeItem(const core::CustomString &id) const;

    protected:
        RepositoryT repository;

        [[nodiscard]] virtual core::DynamicArray<T> ensureLoaded() const;
        virtual void persist(const core::DynamicArray<T> &items) const;

    private:
        // Only getId() - models define their own identity
        [[nodiscard]] core::CustomString getId(const T &item) const;
    };

} // namespace pbl2::service
