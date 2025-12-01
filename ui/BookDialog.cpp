#include "BookDialog.h"
#include "BookStatus.h"
#include "QtBridge.h"
// ...existing code...
#include <QComboBox>
#include <QDateEdit>
#include <QDate>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QStringList>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>

using namespace std;

namespace pbl2::ui{
namespace {

QString cleanId(const QString &value) {
    QString id = value.trimmed();
    return id.replace(' ', '_');
}

    QStringList defaultGenres() {
    return {
        QStringLiteral("Tất cả"),
        QStringLiteral("Truyện tranh"),
        QStringLiteral("Khoa học"),
        QStringLiteral("Kỹ năng mềm"),
        QStringLiteral("Kỹ năng sống"),
        QStringLiteral("Văn học"),
        QStringLiteral("Lịch sử"),
        QStringLiteral("Khoa học viễn tưởng"),
        QStringLiteral("Kinh tế"),
        QStringLiteral("Tiểu thuyết"),
        QStringLiteral("Công nghệ"),
        QStringLiteral("Tâm lý"),
        QStringLiteral("Ngôn tình"),
        QStringLiteral("Khác")
    };
}

}


BookDialog::BookDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Thông tin sách"));
    setModal(true);
    setWindowIcon(QIcon(":/ui/resources/icons/book.png"));
    const QFont font("Segoe UI", 11);
    setFont(font);
    // Dùng chung style đơn giản với Thong tin ban doc
    setStyleSheet("QDialog { background: #f8fafc; border-radius: 12px; } "
                  "QGroupBox { font-weight: bold; } "
                  "QLineEdit, QComboBox, QSpinBox, QDateEdit, QPlainTextEdit { "
                  "  min-height: 32px; font-size: 11pt; } "
                  "QDialogButtonBox QPushButton { "
                  "  min-width: 90px; min-height: 32px; font-size: 11pt; } "
                  "QLabel { font-size: 11pt; } ");

    idEdit = new QLineEdit(this);
    titleEdit = new QLineEdit(this);
    authorEdit = new QLineEdit(this);
    genreCombo = new QComboBox(this);
    genreCombo->setEditable(true);
    genreCombo->addItems(defaultGenres());
    genreCombo->setInsertPolicy(QComboBox::NoInsert);
    publisherEdit = new QLineEdit(this);

    publishDateEdit = new QDateEdit(this);
    publishDateEdit->setCalendarPopup(true);
    publishDateEdit->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));

    quantitySpin = new QSpinBox(this);
    quantitySpin->setRange(0, 100000);
    quantitySpin->setReadOnly(true);
    quantitySpin->setButtonSymbols(QAbstractSpinBox::NoButtons);
    originalPriceSpin = new QSpinBox(this);
    originalPriceSpin->setRange(0, 1000000000);
    originalPriceSpin->setSingleStep(1000);
    originalPriceSpin->setSuffix(tr(" VND"));

    // Ảnh bìa
    coverLabel = new QLabel(this);
    coverLabel->setFixedSize(120, 160);
    coverLabel->setStyleSheet("border: 1px solid #ccc; background: #eee; margin-bottom: 8px;");
    coverLabel->setAlignment(Qt::AlignCenter);
    chooseCoverButton = new QPushButton(tr("Chọn ảnh bìa"), this);
    connect(chooseCoverButton, &QPushButton::clicked, this, [this]() {
        QString file = QFileDialog::getOpenFileName(this, tr("Chọn ảnh bìa"), QString(), "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!file.isEmpty()) {
            QPixmap pix(file);
            coverLabel->setPixmap(pix.scaled(coverLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            coverImagePath = bridge::toCustomString(file);
        }
    });

    statusCombo = new QComboBox(this);
    statusCombo->addItem(tr("CÒN"), QStringLiteral("CÒN"));
    statusCombo->addItem(tr("HẾT"), QStringLiteral("HẾT"));


    summaryEdit = new QPlainTextEdit(this);
    summaryEdit->setPlaceholderText(tr("Mô tả ngắn hoặc ghi chú về cuốn sách..."));

    errorLabel = new QLabel(this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setVisible(false);

    auto *formGroup = new QGroupBox(tr("Thông tin sách"), this);
    auto *form = new QFormLayout;
    form->setContentsMargins(12, 12, 12, 12);
    form->setHorizontalSpacing(12);
    form->setVerticalSpacing(10);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    // Thêm ảnh bìa vào đầu form
    auto *coverLayout = new QHBoxLayout;
    coverLayout->addWidget(coverLabel);
    coverLayout->addWidget(chooseCoverButton);
    form->addRow(tr("Ảnh bìa"), coverLayout);
    form->addRow(tr("Mã sách"), idEdit);
    form->addRow(tr("Tiêu đề"), titleEdit);
    form->addRow(tr("Tác giả"), authorEdit);
    form->addRow(tr("Thể loại"), genreCombo);
    form->addRow(tr("Nhà xuất bản"), publisherEdit);
    form->addRow(tr("Ngày phát hành"), publishDateEdit);
    quantitySpin->setReadOnly(false);
    quantitySpin->setButtonSymbols(QAbstractSpinBox::UpDownArrows);

    form->addRow(tr("Số lượng"), quantitySpin);
    form->addRow(tr("Giá gốc"), originalPriceSpin);
    form->addRow(tr("Tình trạng"), statusCombo);
    form->addRow(tr("Tóm tắt"), summaryEdit);
    formGroup->setLayout(form);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &BookDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &BookDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);
    layout->addWidget(formGroup);
    layout->addWidget(errorLabel);
    layout->addWidget(buttonBox);
    setMinimumSize(640, 600);
}

void BookDialog::setBook(const model::Book &book, const bool editing) {
    editingMode = editing;
    idEdit->setText(bridge::toQString(book.getId()));
    idEdit->setReadOnly(editing || forceIdReadOnly);
    idEdit->setEnabled(!editing && !forceIdReadOnly);
    titleEdit->setText(bridge::toQString(book.getTitle()));
    authorEdit->setText(bridge::toQString(book.getAuthor()));
    genreCombo->setEditText(bridge::toQString(book.getGenre()));
    publisherEdit->setText(bridge::toQString(book.getPublisher()));
    if (editing) {
        const QDate date = bridge::toQDate(book.getPublishDate());
        publishDateEdit->setDate(date.isValid() ? date : QDate::currentDate());
    } else {
        publishDateEdit->setDate(QDate::currentDate());
    }
    quantitySpin->setValue(book.getQuantity());
    originalPriceSpin->setValue(book.getOriginalPrice());
    if (editing) {
        // Khóa những trường không muốn chỉnh sửa khi cập nhật
        titleEdit->setReadOnly(true);
        authorEdit->setReadOnly(true);
        publisherEdit->setReadOnly(true);

        quantitySpin->setReadOnly(true);
        quantitySpin->setButtonSymbols(QAbstractSpinBox::NoButtons);
        quantitySpin->setEnabled(true);

        publishDateEdit->setEnabled(true);
        originalPriceSpin->setEnabled(true);
        chooseCoverButton->setEnabled(true);
        genreCombo->setEnabled(true);
        summaryEdit->setReadOnly(false);
    } else {
        quantitySpin->setReadOnly(true);
        quantitySpin->setButtonSymbols(QAbstractSpinBox::NoButtons);
        quantitySpin->setEnabled(true);

        publishDateEdit->setEnabled(true);
        originalPriceSpin->setEnabled(true);
        chooseCoverButton->setEnabled(true);
        genreCombo->setEnabled(true);
        publisherEdit->setReadOnly(false);
        titleEdit->setReadOnly(false);
        authorEdit->setReadOnly(false);
        summaryEdit->setReadOnly(false);
    }
    const core::CustomString status = model::canonicalBookStatus(book.getStatus());
    for (int i = 0; i < statusCombo->count(); ++i) {
        if (const core::CustomString option = bridge::toCustomString(statusCombo->itemData(i).toString()); option.compare(status, core::CaseSensitivity::Sensitive) == 0) {
            statusCombo->setCurrentIndex(i);
            break;
        }
    }
    summaryEdit->setPlainText(bridge::toQString(book.getSummary()));
    // Hiển thị ảnh bìa nếu có
    coverImagePath = book.getCoverImagePath();
    QString coverPath = bridge::toQString(coverImagePath);
    if (!coverPath.isEmpty()) {
        QPixmap pix(coverPath);
        coverLabel->setPixmap(pix.scaled(coverLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        coverLabel->setPixmap(QPixmap());
    }
}

void BookDialog::presetId(const QString &id, const bool lockField) {
    forceIdReadOnly = lockField;
    idEdit->setText(cleanId(id));
    idEdit->setReadOnly(editingMode || forceIdReadOnly);
    publishDateEdit->setDate(QDate::currentDate()); // Đảm bảo ngày phát hành là hôm nay khi thêm mới
}

model::Book BookDialog::book() const {
    model::Book b;
    b.setId(bridge::toCustomString(cleanId(idEdit->text())));
    b.setTitle(bridge::toCustomString(titleEdit->text().trimmed()));
    b.setAuthor(bridge::toCustomString(authorEdit->text().trimmed()));
    b.setGenre(bridge::toCustomString(genreCombo->currentText().trimmed()));
    b.setPublisher(bridge::toCustomString(publisherEdit->text().trimmed()));
    b.setPublishDate(bridge::toCoreDate(publishDateEdit->date()));
    b.setQuantity(quantitySpin->value());
    b.setOriginalPrice(originalPriceSpin->value());
    const auto statusValue = model::canonicalBookStatus(bridge::toCustomString(statusCombo->currentData().toString()));
    b.setStatus(statusValue.isEmpty() ? core::CustomStringLiteral("CÒN") : statusValue);
    b.setSummary(bridge::toCustomString(summaryEdit->toPlainText().trimmed()));
    b.setCoverImagePath(coverImagePath);
    return b;
}

    bool BookDialog::validateInputs() const {
    if (idEdit->text().trimmed().isEmpty()) {
        showError(tr("Mã sách không được để trống."));
        return false;
    }
    if (titleEdit->text().trimmed().isEmpty()) {
        showError(tr("Tiêu đề không được để trống."));
        return false;
    }
    if (authorEdit->text().trimmed().isEmpty()) {
        showError(tr("Tác giả không được để trống."));
        return false;
    }
    return true;
}

void BookDialog::showError(const QString &message) const{
    errorLabel->setText(message);
    errorLabel->setVisible(true);
}

void BookDialog::accept() {
    errorLabel->setVisible(false);
    if (!validateInputs()) return;
    QDialog::accept();
}

}  // namespace ui
