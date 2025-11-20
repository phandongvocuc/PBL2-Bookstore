#include "StatsWidget.h"

#include <QColor>
#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLocale>
#include <QFrame>
#include <QListWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <algorithm>

namespace pbl2::ui {

StatsWidget::StatsWidget(QWidget *parent) : QWidget(parent) {
    setupUi();

    // Seed demo data so the view looks meaningful out of the box.
    updateStats(12000, 1750, 380, 45, 200000);

    custom::Map<QString, int> categoryCounts;
    categoryCounts.insert(tr("Truyen tranh"), 125);
    categoryCounts.insert(tr("Khoa hoc"), 90);
    categoryCounts.insert(tr("Ky nang mem"), 75);
    categoryCounts.insert(tr("Ky nang song"), 65);
    categoryCounts.insert(tr("Van hoc"), 58);
    categoryCounts.insert(tr("Lich su"), 45);
    categoryCounts.insert(tr("Khoa hoc vien tuong"), 40);
    updateCategoryChart(categoryCounts);

    custom::Map<QString, int> bookCounts;
    bookCounts.insert(QStringLiteral("Dragon Ball"), 240);
    bookCounts.insert(QStringLiteral("Conan"), 180);
    bookCounts.insert(QStringLiteral("Doraemon"), 150);
    bookCounts.insert(QStringLiteral("Harry Potter"), 120);
    bookCounts.insert(QStringLiteral("One Piece"), 95);
    bookCounts.insert(QStringLiteral("Crinto"), 90);
    updateTopBooksChart(bookCounts);

    custom::Vector<int> monthly;
    monthly.append(150);
    monthly.append(160);
    monthly.append(145);
    monthly.append(155);
    monthly.append(165);
    monthly.append(175);
    monthly.append(185);
    monthly.append(195);
    monthly.append(215);
    updateMonthlyChart(monthly);
}

void StatsWidget::setupUi() {
    setStyleSheet("background-color: #f8fafc;");

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(16, 16, 16, 16);

    auto *titleRow = new QHBoxLayout();
    auto *titleLabel = new QLabel(tr("THONG KE"), this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(22);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #0f172a;");
    titleRow->addWidget(titleLabel);
    titleRow->addStretch();
    mainLayout->addLayout(titleRow);

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; }");

    auto *scrollContent = new QWidget();
    auto *contentLayout = new QVBoxLayout(scrollContent);
    contentLayout->setSpacing(16);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    // Summary cards row
    auto *cardsRow = new QHBoxLayout();
    cardsRow->setSpacing(12);

    auto *booksCard = createStatsCard(tr("Tong so sach"), QString());
    totalBooksValue = booksCard->findChild<QLabel *>("valueLabel");
    cardsRow->addWidget(booksCard);

    auto *monthlyCard = createStatsCard(tr("So luot muon trong thang"), QString());
    monthlyLoansValue = monthlyCard->findChild<QLabel *>("valueLabel");
    cardsRow->addWidget(monthlyCard);

    auto *activeCard = createStatsCard(tr("Sach dang muon"), QString());
    activeLoansValue = activeCard->findChild<QLabel *>("valueLabel");
    cardsRow->addWidget(activeCard);

    auto *overdueCard = createStatsCard(tr("Sach tre han"), QString());
    overdueValue = overdueCard->findChild<QLabel *>("valueLabel");
    cardsRow->addWidget(overdueCard);

    cardsRow->addStretch();
    contentLayout->addLayout(cardsRow);

    // First row: category chart + top books
    auto *chartsRow = new QHBoxLayout();
    chartsRow->setSpacing(16);

    auto *categoryPanel = new QFrame(this);
    categoryPanel->setStyleSheet("QFrame { background: white; border: 1px solid #e5e7eb; border-radius: 10px; padding: 12px; }");
    categoryPanel->setMinimumHeight(320);
    auto *categoryLayout = new QVBoxLayout(categoryPanel);
    categoryLayout->setSpacing(8);

    auto *categoryLabel = new QLabel(tr("Luot muon theo the loai"), categoryPanel);
    QFont sectionFont = categoryLabel->font();
    sectionFont.setPointSize(13);
    sectionFont.setBold(true);
    categoryLabel->setFont(sectionFont);
    categoryLabel->setStyleSheet("color: #1f2937;");
    categoryLayout->addWidget(categoryLabel);

    categoryChart = new StatsChart(categoryPanel);
    categoryChart->setAxisLabels(tr("The loai"), tr("Luot muon"));
    categoryChart->setMode(StatsChart::Mode::Bar);
    categoryChart->setMinimumHeight(240);
    categoryLayout->addWidget(categoryChart);
    chartsRow->addWidget(categoryPanel, 1);

    auto *topBooksPanel = new QFrame(this);
    topBooksPanel->setStyleSheet("QFrame { background: white; border: 1px solid #e5e7eb; border-radius: 10px; padding: 12px; }");
    topBooksPanel->setMinimumHeight(320);
    auto *topBookLayout = new QVBoxLayout(topBooksPanel);
    topBookLayout->setSpacing(8);

    auto *topBookLabel = new QLabel(tr("Top sach duoc muon nhieu nhat"), topBooksPanel);
    topBookLabel->setFont(sectionFont);
    topBookLabel->setStyleSheet("color: #1f2937;");
    topBookLayout->addWidget(topBookLabel);

    topBooksChart = new StatsChart(topBooksPanel);
    topBooksChart->setAxisLabels(tr("Sach"), tr("Luot muon"));
    topBooksChart->setMode(StatsChart::Mode::Bar);
    topBooksChart->setMinimumHeight(240);
    topBookLayout->addWidget(topBooksChart);
    chartsRow->addWidget(topBooksPanel, 1);

    contentLayout->addLayout(chartsRow);

    // Second row: monthly chart + fines summary
    auto *bottomRow = new QHBoxLayout();
    bottomRow->setSpacing(16);

    auto *monthlyPanel = new QFrame(this);
    monthlyPanel->setStyleSheet("QFrame { background: white; border: 1px solid #e5e7eb; border-radius: 10px; padding: 12px; }");
    monthlyPanel->setMinimumHeight(280);
    auto *monthlyLayout = new QVBoxLayout(monthlyPanel);
    monthlyLayout->setSpacing(8);

    auto *monthlyLabel = new QLabel(tr("Luot muon theo thang"), monthlyPanel);
    monthlyLabel->setFont(sectionFont);
    monthlyLabel->setStyleSheet("color: #1f2937;");
    monthlyLayout->addWidget(monthlyLabel);

    monthlyChart = new StatsChart(monthlyPanel);
    monthlyChart->setAxisLabels(tr("Thang"), tr("Luot muon"));
    monthlyChart->setMode(StatsChart::Mode::Bar);
    monthlyChart->setMinimumHeight(220);
    monthlyLayout->addWidget(monthlyChart);
    bottomRow->addWidget(monthlyPanel, 1);

    auto *finePanel = new QFrame(this);
    finePanel->setStyleSheet("QFrame { background: white; border: 1px solid #e5e7eb; border-radius: 10px; padding: 12px; }");
    finePanel->setMinimumHeight(280);
    auto *fineLayout = new QVBoxLayout(finePanel);
    fineLayout->setSpacing(8);

    auto *fineTitle = new QLabel(tr("Thong ke phat"), finePanel);
    fineTitle->setFont(sectionFont);
    fineTitle->setStyleSheet("color: #1f2937;");
    fineLayout->addWidget(fineTitle);

    fineSummaryLabel = new QLabel(tr("Tong tien phat thang nay: 0 VND"), finePanel);
    overdueBooksLabel = new QLabel(tr("Sach tre han: 0 quyen"), finePanel);
    overdueReadersLabel = new QLabel(tr("So nguoi dang tre: 0 nguoi"), finePanel);
    const QString subStyle = "color: #111827; font-size: 11pt;";
    fineSummaryLabel->setStyleSheet(subStyle);
    overdueBooksLabel->setStyleSheet(subStyle);
    overdueReadersLabel->setStyleSheet(subStyle);
    fineLayout->addWidget(fineSummaryLabel);
    fineLayout->addWidget(overdueBooksLabel);
    fineLayout->addWidget(overdueReadersLabel);

    fineLayout->addSpacing(8);

    auto *finesGrid = new QGridLayout();
    finesGrid->setHorizontalSpacing(20);
    finesGrid->setVerticalSpacing(6);

    auto makeFineRow = [&](int row, const QString &name, const QString &value) {
        auto *nameLabel = new QLabel(QStringLiteral("* %1").arg(name), finePanel);
        auto *valueLabel = new QLabel(value, finePanel);
        nameLabel->setStyleSheet("color: #1f2937; font-size: 11pt;");
        valueLabel->setStyleSheet("color: #1f2937; font-size: 11pt;");
        finesGrid->addWidget(nameLabel, row, 0, Qt::AlignLeft);
        finesGrid->addWidget(valueLabel, row, 1, Qt::AlignRight);
    };

    makeFineRow(0, tr("Nguoi A"), QStringLiteral("80.000"));
    makeFineRow(1, tr("Nguoi B"), QStringLiteral("60.000"));
    makeFineRow(2, tr("Nguoi C"), QStringLiteral("40.000"));

    fineLayout->addLayout(finesGrid);
    fineLayout->addStretch();

    bottomRow->addWidget(finePanel, 1);
    contentLayout->addLayout(bottomRow);

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);
}

QFrame *StatsWidget::createStatsCard(const QString &title, const QString &icon) {
    auto *card = new QFrame(this);
    card->setStyleSheet(
        "QFrame { background: white; border: 1px solid #e5e7eb; border-radius: 10px; padding: 14px; }"
        "QLabel { background: transparent; border: none; }");
    card->setMinimumSize(180, 110);

    auto *layout = new QVBoxLayout(card);
    layout->setSpacing(6);
    layout->setAlignment(Qt::AlignCenter);

    if (!icon.isEmpty()) {
        auto *iconLabel = new QLabel(icon, card);
        QFont iconFont = iconLabel->font();
        iconFont.setPointSize(22);
        iconLabel->setFont(iconFont);
        iconLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(iconLabel);
    }

    auto *valueLabel = new QLabel("0", card);
    valueLabel->setObjectName("valueLabel");
    QFont valueFont = valueLabel->font();
    valueFont.setPointSize(30);
    valueFont.setBold(true);
    valueLabel->setFont(valueFont);
    valueLabel->setAlignment(Qt::AlignCenter);
    valueLabel->setStyleSheet("color: #111827;");
    layout->addWidget(valueLabel);

    auto *titleLabel = new QLabel(title, card);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #4b5563;");
    layout->addWidget(titleLabel);

    return card;
}

void StatsWidget::updateStats(int totalBooks, int monthlyLoans, int activeLoans, int overdueLoans, qint64 totalFines) {
    QLocale locale(QLocale::Vietnamese, QLocale::Vietnam);
    if (totalBooksValue) totalBooksValue->setText(locale.toString(totalBooks));
    if (monthlyLoansValue) monthlyLoansValue->setText(locale.toString(monthlyLoans));
    if (activeLoansValue) activeLoansValue->setText(locale.toString(activeLoans));
    if (overdueValue) overdueValue->setText(locale.toString(overdueLoans));

    const QString fineText = locale.toString(totalFines) + QStringLiteral(" VND");
    if (fineSummaryLabel) fineSummaryLabel->setText(tr("Tong tien phat thang nay: %1").arg(fineText));
    if (totalFinesValue) totalFinesValue->setText(fineText);
    if (overdueBooksLabel) overdueBooksLabel->setText(tr("Sach tre han: %1 quyen").arg(locale.toString(overdueLoans)));
    if (overdueReadersLabel) overdueReadersLabel->setText(tr("So nguoi dang tre: %1 nguoi").arg(locale.toString(qMax(0, overdueLoans / 2 + 1))));
}

void StatsWidget::updateTopBooksChart(const custom::Map<QString, int> &bookBorrowCounts) {
    if (!topBooksChart) return;

    QList<QPair<QString, int>> sortedBooks;
    for (auto it = bookBorrowCounts.constBegin(); it != bookBorrowCounts.constEnd(); ++it) {
        sortedBooks.append(qMakePair(it.key(), it.value()));
    }
    std::sort(sortedBooks.begin(), sortedBooks.end(), [](const QPair<QString, int> &a, const QPair<QString, int> &b) {
        return a.second > b.second;
    });

    QStringList categories;
    custom::Vector<double> values;
    int count = qMin(6, sortedBooks.size());
    for (int i = 0; i < count; ++i) {
        categories.append(sortedBooks[i].first);
        values.append(sortedBooks[i].second);
    }

    StatsChart::Series series;
    series.name = tr("Luot muon");
    series.values = values;
    series.color = QColor(0x3b, 0x82, 0xf6);

    topBooksChart->setCategories(categories);
    topBooksChart->setSeries({series});
    topBooksChart->setMode(StatsChart::Mode::Bar);
}

void StatsWidget::updateCategoryChart(const custom::Map<QString, int> &categoryBorrowCounts) {
    if (!categoryChart) return;

    QList<QPair<QString, int>> items;
    for (auto it = categoryBorrowCounts.constBegin(); it != categoryBorrowCounts.constEnd(); ++it) {
        items.append(qMakePair(it.key(), it.value()));
    }
    std::sort(items.begin(), items.end(), [](const QPair<QString, int> &a, const QPair<QString, int> &b) {
        return a.second > b.second;
    });

    QStringList categories;
    custom::Vector<double> values;
    for (const auto &item : items) {
        categories.append(item.first);
        values.append(item.second);
    }

    StatsChart::Series series;
    series.name = tr("Luot muon");
    series.values = values;
    series.color = QColor(0x3b, 0x82, 0xf6);

    categoryChart->setCategories(categories);
    categoryChart->setSeries({series});
    categoryChart->setMode(StatsChart::Mode::Bar);
}

void StatsWidget::updateMonthlyChart(const custom::Vector<int> &monthlyBorrowCounts) {
    if (!monthlyChart) return;

    QStringList categories;
    custom::Vector<double> values;
    const int size = monthlyBorrowCounts.size();
    for (int i = 0; i < size; ++i) {
        categories.append(tr("Thang %1").arg(i + 1));
        values.append(monthlyBorrowCounts[i]);
    }

    StatsChart::Series series;
    series.name = tr("Luot muon");
    series.values = values;
    series.color = QColor(0x3b, 0x82, 0xf6);

    monthlyChart->setCategories(categories);
    monthlyChart->setSeries({series});
    monthlyChart->setMode(StatsChart::Mode::Bar);
}

void StatsWidget::updatePieChart(int cardFees, int fines) {
    if (!revenuePieChart) return;

    custom::Vector<QPair<QString, int>> segments;
    segments.append(qMakePair(tr("Lam the"), cardFees));
    segments.append(qMakePair(tr("Tien phat"), fines));

    revenuePieChart->setSegments(segments);
}

void StatsWidget::updateLoansList(const custom::Vector<QPair<QString, QString>> &loans) {
    if (!recentLoansList) return;

    recentLoansList->clear();
    for (const auto &loan : loans) {
        auto *item = new QListWidgetItem(loan.first, recentLoansList);
        item->setData(Qt::UserRole, loan.second);
        recentLoansList->addItem(item);
    }
}

void StatsWidget::updateActiveReadersList(const custom::Vector<QString> &readers) {
    if (!activeReadersList) return;

    activeReadersList->clear();
    for (const auto &reader : readers) {
        auto *item = new QListWidgetItem(reader, activeReadersList);
        activeReadersList->addItem(item);
    }
}

}  // namespace pbl2::ui
