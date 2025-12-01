#include "model/Book.h"

namespace pbl2::model {

const core::CustomString &Book::getId() const {
    return id;
}

void Book::setId(const core::CustomString &value) {
    id = value;
}

const core::CustomString &Book::getTitle() const {
    return title;
}

void Book::setTitle(const core::CustomString &value) {
    title = value;
}

const core::CustomString &Book::getAuthor() const {
    return author;
}

void Book::setAuthor(const core::CustomString &value) {
    author = value;
}

const core::CustomString &Book::getGenre() const {
    return genre;
}

void Book::setGenre(const core::CustomString &value) {
    genre = value;
}

const core::CustomString &Book::getPublisher() const {
    return publisher;
}

void Book::setPublisher(const core::CustomString &value) {
    publisher = value;
}

const core::Date &Book::getPublishDate() const {
    return publishDate;
}

void Book::setPublishDate(const core::Date &value) {
    publishDate = value;
}


int Book::getQuantity() const {
    return quantity;
}

void Book::setQuantity(const int value) {
    quantity = value;
}

int Book::getOriginalPrice() const {
    return originalPrice;
}

void Book::setOriginalPrice(const int value) {
    originalPrice = value;
}

const core::CustomString &Book::getStatus() const {
    return status;
}

void Book::setStatus(const core::CustomString &value) {
    status = value;
}

const core::CustomString &Book::getSummary() const {
    return summary;
}

void Book::setSummary(const core::CustomString &value) {
    summary = value;
}

}  // namespace pbl2::model
