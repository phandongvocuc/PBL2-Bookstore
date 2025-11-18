#include "service/BookService.h"

#include "BookStatus.h"

using namespace std;  // project-wide request

namespace pbl2::service {

BookService::BookService(const custom::CustomString &dataDir) : repository(dataDir) {}

custom::DynamicArray<model::Book> BookService::fetchAll() const { return ensureLoaded(); }

custom::Optional<model::Book> BookService::findById(const custom::CustomString &bookId) const {
    const custom::CustomString trimmed = bookId.trimmed();
    if (trimmed.isEmpty()) return custom::Optional<model::Book>();
    const auto books = ensureLoaded();
    if (books.isEmpty()) return custom::Optional<model::Book>();
    for (custom::DynamicArray<model::Book>::ConstIterator it = books.cbegin(); it != books.cend(); ++it) {
        if (it->getId().compare(trimmed, custom::CaseSensitivity::Insensitive) == 0) {
            return custom::Optional(*it);
        }
    }
    return custom::Optional<model::Book>();
}

bool BookService::addBook(const model::Book &book) const {
    auto books = ensureLoaded();

    model::Book copy = book;

    const auto target = copy.getId();
    bool exists = false;
    for (custom::DynamicArray<model::Book>::ConstIterator it = books.cbegin(); it != books.cend(); ++it) {
        if (it->getId().compare(target, custom::CaseSensitivity::Insensitive) == 0) {
            exists = true;
            break;
        }
    }
    if (exists) return false;

    normalizeAvailability(copy);
    books.pushBack(copy);
    persist(books);
    return true;
}

bool BookService::updateBook(const model::Book &book) const {
    auto books = ensureLoaded();
    bool updated = false;
    for (custom::DynamicArray<model::Book>::Iterator it = books.begin(); it != books.end(); ++it) {
        if (auto &existing = *it; existing.getId() == book.getId()) {
            existing = book;
            normalizeAvailability(existing);
            updated = true;
            break;
        }
    }
    if (!updated) return false;
    persist(books);
    return true;
}

bool BookService::removeBook(const custom::CustomString &bookId) const {
    const custom::CustomString trimmed = bookId.trimmed();
    if (trimmed.isEmpty()) return false;
    auto books = ensureLoaded();
    const auto target = trimmed;
    bool removed = false;
    custom::DynamicArray<model::Book>::SizeType index = 0U;
    while (index < books.size()) {
        if (books[index].getId().compare(target, custom::CaseSensitivity::Insensitive) == 0) {
            books.removeAt(index);
            removed = true;
        } else {
            ++index;
        }
    }
    if (!removed) return false;
    persist(books);
    return true;
}

custom::DynamicArray<model::Book> BookService::ensureLoaded() const {
    auto books = repository.loadAll();
    bool normalized = false;
    for (auto &book : books) {
        normalized = normalizeAvailability(book) || normalized;
    }
    if (normalized) {
        repository.saveAll(books);
    }
    return books;
}

void BookService::persist(const custom::DynamicArray<model::Book> &books) const {
    repository.saveAll(books);
}

bool BookService::normalizeAvailability(model::Book &book) {
    if (!model::isAvailabilityStatus(book.getStatus())) return false;
    const auto desired = model::availabilityStatusForQuantity(book.getQuantity());
    if (model::canonicalBookStatus(book.getStatus()) == desired) return false;
    book.setStatus(desired);
    return true;
}

}  // namespace pbl2::service
