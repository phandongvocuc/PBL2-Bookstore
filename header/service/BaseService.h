#pragma once

#include "core/custom/Optional.h"
#include "core/custom/DynamicArray.h"
#include "core/custom/CustomString.h"

namespace pbl2::service {

    template<typename T, typename RepositoryT>
    class BaseService {
    public:
        virtual ~BaseService() = default;

        explicit BaseService(const custom::CustomString &dataDir);

        // Only the essential CRUD methods that ALL models need
        [[nodiscard]] virtual custom::DynamicArray<T> fetchAll() const;
        [[nodiscard]] virtual custom::Optional<T> findById(const custom::CustomString &id) const;
        [[nodiscard]] bool addItem(const T &item) const;
        [[nodiscard]] bool updateItem(const T &item) const;
        [[nodiscard]] bool removeItem(const custom::CustomString &id) const;

    protected:
        RepositoryT repository;

        [[nodiscard]] virtual custom::DynamicArray<T> ensureLoaded() const;
        virtual void persist(const custom::DynamicArray<T> &items) const;

    private:
        // Only getId() - models define their own identity
        [[nodiscard]] custom::CustomString getId(const T &item) const;
    };

} // namespace pbl2::service