#pragma once

#include "core/Date.h"
#include "core/custom/CustomString.h"

namespace pbl2::model {

class Book {
public:
    const custom::CustomString &getId() const;
    void setId(const custom::CustomString &value);

    const custom::CustomString &getTitle() const;
    void setTitle(const custom::CustomString &value);

    const custom::CustomString &getAuthor() const;
    void setAuthor(const custom::CustomString &value);

    const custom::CustomString &getGenre() const;
    void setGenre(const custom::CustomString &value);

    const custom::CustomString &getPublisher() const;
    void setPublisher(const custom::CustomString &value);

    const core::Date &getPublishDate() const;
    void setPublishDate(const core::Date &value);

    int getPublishYear() const;
    void setPublishYear(int value);

    int getQuantity() const;
    void setQuantity(int value);

    const custom::CustomString &getStatus() const;
    void setStatus(const custom::CustomString &value);

    const custom::CustomString &getSummary() const;
    void setSummary(const custom::CustomString &value);

private:
    custom::CustomString id;
    custom::CustomString title;
    custom::CustomString author;
    custom::CustomString genre;
    custom::CustomString publisher;
    core::Date publishDate;
    int publishYear{0};
    int quantity{0};
    custom::CustomString status;
    custom::CustomString summary;
};

}  // namespace model
