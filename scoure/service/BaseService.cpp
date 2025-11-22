#include "service/BaseService.h"

#include <type_traits>

#include "model/Reader.h"
#include "model/Book.h"
#include "model/Staff.h"
#include "model/Loan.h"
#include "model/ReportRequest.h"
#include "model/Account.h"
#include "repository/ReadersRepository.h"
#include "repository/BooksRepository.h"
#include "repository/StaffRepository.h"
#include "repository/LoansRepository.h"
#include "repository/ReportRequestRepository.h"
#include "repository/AccountsRepository.h"
using namespace std;
namespace pbl2::service {

template<typename T, typename RepositoryT>
BaseService<T, RepositoryT>::BaseService(const core::CustomString &dataDir)
    : repository(dataDir) {}

template<typename T, typename RepositoryT>
core::DynamicArray<T> BaseService<T, RepositoryT>::fetchAll() const {
    return ensureLoaded();
}

template<typename T, typename RepositoryT>
core::Optional<T> BaseService<T, RepositoryT>::findById(const core::CustomString &id) const {
    const core::CustomString trimmed = id.trimmed();
    if (trimmed.isEmpty()) return {};

    const auto items = ensureLoaded();
    if (items.isEmpty()) return {};

    for (const auto &item : items) {
        if (getId(item).compare(trimmed, core::CaseSensitivity::Insensitive) == 0) {
            return core::Optional(item);
        }
    }
    return {};
}

template<typename T, typename RepositoryT>
bool BaseService<T, RepositoryT>::addItem(const T &item) const {
    auto items = ensureLoaded();

    const core::CustomString targetId = getId(item);
    if (targetId.isEmpty()) return false;

    // Check for duplicates
    for (const auto &existing : items) {
        if (getId(existing).compare(targetId, core::CaseSensitivity::Insensitive) == 0) {
            return false;
        }
    }

    items.pushBack(item);
    persist(items);
    return true;
}

template<typename T, typename RepositoryT>
bool BaseService<T, RepositoryT>::updateItem(const T &item) const {
    auto items = ensureLoaded();
    bool updated = false;

    for (auto &existing : items) {
        if (getId(existing).compare(getId(item), core::CaseSensitivity::Insensitive) == 0) {
            existing = item;
            updated = true;
            break;
        }
    }

    if (!updated) return false;
    persist(items);
    return true;
}

template<typename T, typename RepositoryT>
bool BaseService<T, RepositoryT>::removeItem(const core::CustomString &id) const {
    auto items = ensureLoaded();
    bool removed = false;

    typename core::DynamicArray<T>::SizeType index = 0U;
    while (index < items.size()) {
        if (getId(items[index]).compare(id, core::CaseSensitivity::Insensitive) == 0) {
            items.removeAt(index);
            removed = true;
        } else {
            ++index;
        }
    }

    if (!removed) return false;
    persist(items);
    return true;
}

template<typename T, typename RepositoryT>
core::DynamicArray<T> BaseService<T, RepositoryT>::ensureLoaded() const {
    return repository.loadAll();
}

template<typename T, typename RepositoryT>
void BaseService<T, RepositoryT>::persist(const core::DynamicArray<T> &items) const {
    repository.saveAll(items);
}
template<typename T, typename RepositoryT>
core::CustomString BaseService<T, RepositoryT>::getId(const T &item) const {
if constexpr (is_same_v<T, model::Loan>) {
    return item.getLoanId();
}
else if constexpr (is_same_v<T, model::ReportRequest>) {
    return item.getRequestId();
}
else if constexpr (is_same_v<T, model::Account>) {
    return item.getUsername();
}
else {
    return item.getId();
}
}

template class BaseService<model::Reader, repository::ReadersRepository>;
template class BaseService<model::Book, repository::BooksRepository>;
template class BaseService<model::Staff, repository::StaffRepository>;
template class BaseService<model::Loan, repository::LoansRepository>;
template class BaseService<model::ReportRequest, repository::ReportRequestRepository>;
template class BaseService<model::Account, repository::AccountsRepository>;

} // namespace pbl2::service