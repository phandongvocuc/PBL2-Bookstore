#include "model/Book.h"

namespace pbl2::model {

const custom::CustomString &Book::getId() const {
    return id;
}

void Book::setId(const custom::CustomString &value) {
    id = value;
}

const custom::CustomString &Book::getTitle() const {
    return title;
}

void Book::setTitle(const custom::CustomString &value) {
    title = value;
}

const custom::CustomString &Book::getAuthor() const {
    return author;
}

void Book::setAuthor(const custom::CustomString &value) {
    author = value;
}

const custom::CustomString &Book::getGenre() const {
    return genre;
}

void Book::setGenre(const custom::CustomString &value) {
    genre = value;
}

const custom::CustomString &Book::getPublisher() const {
    return publisher;
}

void Book::setPublisher(const custom::CustomString &value) {
    publisher = value;
}

const core::Date &Book::getPublishDate() const {
    return publishDate;
}

void Book::setPublishDate(const core::Date &value) {
    publishDate = value;
}

int Book::getPublishYear() const {
    return publishYear;
}

void Book::setPublishYear(const int value) {
    publishYear = value;
}

int Book::getQuantity() const {
    return quantity;
}

void Book::setQuantity(const int value) {
    quantity = value;
}

const custom::CustomString &Book::getStatus() const {
    return status;
}

void Book::setStatus(const custom::CustomString &value) {
    status = value;
}

const custom::CustomString &Book::getSummary() const {
    return summary;
}

void Book::setSummary(const custom::CustomString &value) {
    summary = value;
}

}  // namespace pbl2::model
