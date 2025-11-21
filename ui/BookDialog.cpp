#include "BookDialog.h"
#include "BookStatus.h"
#include "QtBridge.h"
#include <QAbstractSpinBox>
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

using namespace std;  // project-wide request

namespace pbl2::ui{
namespace {

QString cleanId(const QString &value) {
    QString id = value.trimmed();
    return id.replace(' ', '_');
}

QStringList defaultGenres() {
    return {
        QStringLiteral("Tat ca"),
        QStringLiteral("Truyen tranh"),
        QStringLiteral("Khoa hoc"),
        QStringLiteral("Ky nang mem"),
        QStringLiteral("Ky nang song"),
        QStringLiteral("Van hoc"),
        QStringLiteral("Lich su"),
        QStringLiteral("Khoa hoc vien tuong"),
        QStringLiteral("Kinh te"),
        QStringLiteral("Tieu thuyet"),
        QStringLiteral("Cong nghe"),
        QStringLiteral("Tam ly"),
        QStringLiteral("Ngon tinh"),
        QStringLiteral("Khac")
    };
}

}


BookDialog::BookDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Thong tin sach"));
    setModal(true);
    setWindowIcon(QIcon(":/ui/resources/icons/book.png"));
    QFont font("Segoe UI", 11);
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

    publishYearSpin = new QSpinBox(this);
    publishYearSpin->setRange(0, 9999);

    quantitySpin = new QSpinBox(this);
    quantitySpin->setRange(0, 100000);

    statusCombo = new QComboBox(this);
    statusCombo->addItem(tr("CON"), QStringLiteral("CON"));
    statusCombo->addItem(tr("HET"), QStringLiteral("HET"));


    summaryEdit = new QPlainTextEdit(this);
    summaryEdit->setPlaceholderText(tr("Mo ta ngan hoac ghi chu..."));

    errorLabel = new QLabel(this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setVisible(false);

    auto *formGroup = new QGroupBox(tr("Thong tin sach"), this);
    auto *form = new QFormLayout;
    form->setContentsMargins(12, 12, 12, 12);
    form->setHorizontalSpacing(12);
    form->setVerticalSpacing(10);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    form->addRow(tr("Ma sach"), idEdit);
    form->addRow(tr("Tieu de"), titleEdit);
    form->addRow(tr("Tac gia"), authorEdit);
    form->addRow(tr("The loai"), genreCombo);
    form->addRow(tr("Nha xuat ban"), publisherEdit);
    form->addRow(tr("Ngay phat hanh"), publishDateEdit);
    form->addRow(tr("Nam xuat ban"), publishYearSpin);
    quantitySpin->setReadOnly(true);
    quantitySpin->setButtonSymbols(QAbstractSpinBox::NoButtons);

    form->addRow(tr("So luong (tu dong)"), quantitySpin);
    form->addRow(tr("Tinh trang"), statusCombo);
    form->addRow(tr("Tom tat"), summaryEdit);
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

void BookDialog::setBook(const model::Book &book, bool editing) {
    editingMode = editing;
    idEdit->setText(bridge::toQString(book.getId()));
    idEdit->setReadOnly(editing || forceIdReadOnly);
    titleEdit->setText(bridge::toQString(book.getTitle()));
    authorEdit->setText(bridge::toQString(book.getAuthor()));
    genreCombo->setEditText(bridge::toQString(book.getGenre()));
    publisherEdit->setText(bridge::toQString(book.getPublisher()));
    const QDate date = bridge::toQDate(book.getPublishDate());
    publishDateEdit->setDate(date.isValid() ? date : QDate::currentDate());
    publishYearSpin->setValue(book.getPublishYear());
    quantitySpin->setValue(book.getQuantity());
    const custom::CustomString status = model::canonicalBookStatus(book.getStatus());
    for (int i = 0; i < statusCombo->count(); ++i) {
        const custom::CustomString option = bridge::toCustomString(statusCombo->itemData(i).toString());
        if (option.compare(status, custom::CaseSensitivity::Sensitive) == 0) {
            statusCombo->setCurrentIndex(i);
            break;
        }
    }
    summaryEdit->setPlainText(bridge::toQString(book.getSummary()));
}

void BookDialog::presetId(const QString &id, bool lockField) {
    forceIdReadOnly = lockField;
    idEdit->setText(cleanId(id));
    idEdit->setReadOnly(editingMode || forceIdReadOnly);
}

model::Book BookDialog::book() const {
    model::Book b;
    b.setId(bridge::toCustomString(cleanId(idEdit->text())));
    b.setTitle(bridge::toCustomString(titleEdit->text().trimmed()));
    b.setAuthor(bridge::toCustomString(authorEdit->text().trimmed()));
    b.setGenre(bridge::toCustomString(genreCombo->currentText().trimmed()));
    b.setPublisher(bridge::toCustomString(publisherEdit->text().trimmed()));
    b.setPublishDate(bridge::toCoreDate(publishDateEdit->date()));
    b.setPublishYear(publishYearSpin->value());
    b.setQuantity(quantitySpin->value());
    const auto statusValue = model::canonicalBookStatus(bridge::toCustomString(statusCombo->currentData().toString()));
    b.setStatus(statusValue.isEmpty() ? custom::CustomStringLiteral("CON") : statusValue);
    b.setSummary(bridge::toCustomString(summaryEdit->toPlainText().trimmed()));
    return b;
}

bool BookDialog::validateInputs() const {
    if (idEdit->text().trimmed().isEmpty()) {
        showError(tr("Ma sach khong duoc de trong."));
        return false;
    }
    if (titleEdit->text().trimmed().isEmpty()) {
        showError(tr("Tieu de khong duoc de trong."));
        return false;
    }
    if (authorEdit->text().trimmed().isEmpty()) {
        showError(tr("Tac gia khong duoc de trong."));
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
