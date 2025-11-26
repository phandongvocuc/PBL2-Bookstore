#pragma once

#include "../../core/Optional.h"
#include "../../core/CustomString.h"
#include "../../core/DynamicArray.h"

#include "model/Reader.h"
#include "repository/ReadersRepository.h"
#include "service/BaseService.h"

namespace pbl2::service {

    class ReaderService final : public BaseService<model::Reader, repository::ReadersRepository> {
    public:
        explicit ReaderService(const core::CustomString &dataDir);

        [[nodiscard]] core::DynamicArray<model::Reader> fetchAll() const override;
        [[nodiscard]] core::Optional<model::Reader> findById(const core::CustomString &readerId) const override;
        [[nodiscard]] bool addReader(const model::Reader &reader) const;
        [[nodiscard]] bool updateReader(const model::Reader &reader) const;
        [[nodiscard]] bool setReaderActive(const core::CustomString &readerId, bool active) const;
        [[nodiscard]] bool removeReader(const core::CustomString &readerId) const;
    private:
        [[nodiscard]] core::DynamicArray<model::Reader> ensureLoaded() const override;
        void persist(const core::DynamicArray<model::Reader> &readers) const override;
    public:
        // Hàm kiểm tra trùng lặp số điện thoại hoặc căn cước công dân
        [[nodiscard]] bool isDuplicatePhoneOrIdentityCard(const core::CustomString &phone, const core::CustomString &identityCard) const;
    };

} // namespace pbl2::service
