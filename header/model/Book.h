#pragma once

#include "core/Date.h"
#include "../../core/CustomString.h"

namespace pbl2::model {

class Book {
public:
    [[nodiscard]] const core::CustomString &getId() const;
    void setId(const core::CustomString &value);

    [[nodiscard]] const core::CustomString &getTitle() const;
    void setTitle(const core::CustomString &value);

    [[nodiscard]] const core::CustomString &getAuthor() const;
    void setAuthor(const core::CustomString &value);

    [[nodiscard]] const core::CustomString &getGenre() const;
    void setGenre(const core::CustomString &value);

    [[nodiscard]] const core::CustomString &getPublisher() const;
    void setPublisher(const core::CustomString &value);

    [[nodiscard]] const core::Date &getPublishDate() const;
    void setPublishDate(const core::Date &value);

    [[nodiscard]] int getQuantity() const;
    void setQuantity(int value);

    [[nodiscard]] int getOriginalPrice() const;
    void setOriginalPrice(int value);

    [[nodiscard]] const core::CustomString &getStatus() const;
    void setStatus(const core::CustomString &value);

    [[nodiscard]] const core::CustomString &getSummary() const;
    void setSummary(const core::CustomString &value);

private:
    core::CustomString id;
    core::CustomString title;
    core::CustomString author;
    core::CustomString genre;
    core::CustomString publisher;
    core::Date publishDate;
    int quantity{0};
    int originalPrice{0};
    core::CustomString status;
    core::CustomString summary;
    core::CustomString coverImagePath; // Đường dẫn ảnh bìa

public:
    [[nodiscard]] const core::CustomString &getCoverImagePath() const { return coverImagePath; }
    void setCoverImagePath(const core::CustomString &path) { coverImagePath = path; }
};

}  // namespace model
