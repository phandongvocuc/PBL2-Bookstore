#pragma once

#include <QDialog>
#include <QString>

#include "model/Book.h"

class QComboBox;
class QDateEdit;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QSpinBox;

namespace pbl2::ui{
class BookDialog final : public QDialog {
    Q_OBJECT

public:
    explicit BookDialog(QWidget *parent = nullptr);

    void setBook(const model::Book &book, bool editing);
    void presetId(const QString &id, bool lockField = true);
    [[nodiscard]] model::Book book() const;

protected:
    void accept() override;

private:
    [[nodiscard]] bool validateInputs() const;
    void showError(const QString &message) const;

    QLineEdit *idEdit{nullptr};
    QLineEdit *titleEdit{nullptr};
    QLineEdit *authorEdit{nullptr};
    QComboBox *genreCombo{nullptr};
    QLineEdit *publisherEdit{nullptr};
    QDateEdit *publishDateEdit{nullptr};
    QSpinBox *quantitySpin{nullptr};
    QSpinBox *originalPriceSpin{nullptr};
    QComboBox *statusCombo{nullptr};
    QPlainTextEdit *summaryEdit{nullptr};
    QDialogButtonBox *buttonBox{nullptr};
    QLabel *errorLabel{nullptr};
    QLabel *coverLabel{nullptr}; // Hiển thị ảnh bìa
    QPushButton *chooseCoverButton{nullptr}; // Nút chọn ảnh
    core::CustomString coverImagePath; // Đường dẫn ảnh bìa
    bool editingMode{false};
    bool forceIdReadOnly{false};
};

}  // namespace ui
