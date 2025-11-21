#include "service/BookService.h"

#include "BookStatus.h"

using namespace std;

namespace pbl2::service {

BookService::BookService(const custom::CustomString &dataDir) : repository(dataDir) {}

custom::DynamicArray<model::Book> BookService::fetchAll() const { return ensureLoaded(); }

custom::Optional<model::Book> BookService::findById(const custom::CustomString &bookId) const {
    const custom::CustomString trimmed = bookId.trimmed();
    if (trimmed.isEmpty()) return {};
    const auto books = ensureLoaded();
    if (books.isEmpty()) return {};
    for (const auto & book : books) {
        if (book.getId().compare(trimmed, custom::CaseSensitivity::Insensitive) == 0) {
            return custom::Optional(book);
        }
    }
    return {};
}

bool BookService::addBook(const model::Book &book) const {
    auto books = ensureLoaded();

    model::Book copy = book;

    const auto target = copy.getId();
    bool exists = false;
    for (const auto & book1 : books) {
        if (book1.getId().compare(target, custom::CaseSensitivity::Insensitive) == 0) {
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
    const auto books = ensureLoaded();
    bool updated = false;
    for (auto & existing : books) {
        if (existing.getId() == book.getId()) {
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
    const auto& target = trimmed;
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
