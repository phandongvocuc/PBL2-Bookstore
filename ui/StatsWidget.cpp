#include "StatsWidget.h"

#include <QColor>
#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QLocale>
#include <QFrame>
#include <QListWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <algorithm>

namespace pbl2::ui {

StatsWidget::StatsWidget(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void StatsWidget::setupUi() {
    setStyleSheet("background-color: #f8fafc;");

    auto applyShadow = [](QWidget *w, qreal blur = 18.0, const QPointF &offset = QPointF(0, 4)) {
        auto *shadow = new QGraphicsDropShadowEffect(w);
        shadow->setBlurRadius(blur);
        shadow->setColor(QColor(0, 0, 0, 35));
        shadow->setOffset(offset);
        w->setGraphicsEffect(shadow);
    };

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(14);
    mainLayout->setContentsMargins(18, 14, 18, 14);

    auto *titleRow = new QHBoxLayout();
    auto *titleLabel = new QLabel(tr("THỐNG KÊ"), this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
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
    contentLayout->setSpacing(14);
    contentLayout->setContentsMargins(4, 4, 4, 4);

    const QString panelStyle = QStringLiteral(
        "QFrame { background: white; border: 1px solid #e5e7eb; border-radius: 12px; padding: 12px; }");

    // Summary cards row
    auto *cardsRow = new QHBoxLayout();
    cardsRow->setSpacing(10);

    auto *booksCard = createStatsCard(tr("Tổng số sách"), QString());
    totalBooksValue = booksCard->findChild<QLabel *>("valueLabel");
    cardsRow->addWidget(booksCard, 1);

    auto *readersCard = createStatsCard(tr("Tổng số độc giả"), QString());
    totalReadersValue = readersCard->findChild<QLabel *>("valueLabel");
    cardsRow->addWidget(readersCard, 1);

    auto *loansCard = createStatsCard(tr("Tổng số phiếu mượn"), QString());
    totalLoansValue = loansCard->findChild<QLabel *>("valueLabel");
    cardsRow->addWidget(loansCard, 1);

    auto *finesCard = createStatsCard(tr("Tổng tiền phạt"), QString());
    totalFinesValue = finesCard->findChild<QLabel *>("valueLabel");
    cardsRow->addWidget(finesCard, 1);

    contentLayout->addLayout(cardsRow);

    // First row: category chart + top books
    auto *chartsRow = new QHBoxLayout();
    chartsRow->setSpacing(16);

    auto *categoryPanel = new QFrame(this);
    categoryPanel->setStyleSheet(panelStyle);
    categoryPanel->setMinimumHeight(300);
    applyShadow(categoryPanel);
    auto *categoryLayout = new QVBoxLayout(categoryPanel);
    categoryLayout->setSpacing(10);

    auto *categoryLabel = new QLabel(tr("Lượt mượn theo thể loại"), categoryPanel);
    QFont sectionFont = categoryLabel->font();
    sectionFont.setPointSize(13);
    sectionFont.setBold(true);
    categoryLabel->setFont(sectionFont);
    categoryLabel->setStyleSheet("color: #1f2937;");
    categoryLayout->addWidget(categoryLabel);

    categoryChart = new StatsChart(categoryPanel);
    categoryChart->setAxisLabels(QString(), QString());
    categoryChart->setShowLegend(false);
    categoryChart->setMode(StatsChart::Mode::Bar);
    categoryChart->setMinimumHeight(240);
    categoryLayout->addWidget(categoryChart);
    chartsRow->addWidget(categoryPanel, 1);

    auto *topBooksPanel = new QFrame(this);
    topBooksPanel->setStyleSheet(panelStyle);
    topBooksPanel->setMinimumHeight(300);
    applyShadow(topBooksPanel);
    auto *topBookLayout = new QVBoxLayout(topBooksPanel);
    topBookLayout->setSpacing(10);

    auto *topBookLabel = new QLabel(tr("Top sách được mượn nhiều nhất"), topBooksPanel);
    topBookLabel->setFont(sectionFont);
    topBookLabel->setStyleSheet("color: #1f2937;");
    topBookLayout->addWidget(topBookLabel);

    topBooksChart = new StatsChart(topBooksPanel);
    topBooksChart->setAxisLabels(QString(), QString());
    topBooksChart->setShowLegend(false);
    topBooksChart->setMode(StatsChart::Mode::Bar);
    topBooksChart->setMinimumHeight(240);
    topBookLayout->addWidget(topBooksChart);
    chartsRow->addWidget(topBooksPanel, 1);

    contentLayout->addLayout(chartsRow);

    // Second row: monthly chart + fines summary
    auto *bottomRow = new QHBoxLayout();
    bottomRow->setSpacing(16);

    auto *monthlyPanel = new QFrame(this);
    monthlyPanel->setStyleSheet(panelStyle);
    monthlyPanel->setMinimumHeight(280);
    applyShadow(monthlyPanel);
    auto *monthlyLayout = new QVBoxLayout(monthlyPanel);
    monthlyLayout->setSpacing(10);

    auto *monthlyLabel = new QLabel(tr("Lượt mượn theo tháng"), monthlyPanel);
    monthlyLabel->setFont(sectionFont);
    monthlyLabel->setStyleSheet("color: #1f2937;");
    monthlyLayout->addWidget(monthlyLabel);

    monthlyChart = new StatsChart(monthlyPanel);
    monthlyChart->setAxisLabels(QString(), QString());
    monthlyChart->setShowLegend(false);
    monthlyChart->setMode(StatsChart::Mode::Bar);
    monthlyChart->setMinimumHeight(220);
    monthlyLayout->addWidget(monthlyChart);
    bottomRow->addWidget(monthlyPanel, 1);

    auto *finePanel = new QFrame(this);
    finePanel->setStyleSheet(panelStyle);
    finePanel->setMinimumHeight(280);
    applyShadow(finePanel);
    auto *fineLayout = new QVBoxLayout(finePanel);
    fineLayout->setSpacing(10);

    auto *fineTitle = new QLabel(tr("Thống kê phạt"), finePanel);
    fineTitle->setFont(sectionFont);
    fineTitle->setStyleSheet("color: #1f2937;");
    fineLayout->addWidget(fineTitle);

    fineSummaryLabel = new QLabel(tr("Tổng tiền phạt tháng này: 0 VND"), finePanel);
    overdueBooksLabel = new QLabel(tr("Sách trễ hạn: 0 quyển"), finePanel);
    overdueReadersLabel = new QLabel(tr("Số người đang trễ: 0 người"), finePanel);
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
        "QFrame { background: white; border: 1px solid #e5e7eb; border-radius: 10px; padding: 10px; }"
        "QLabel { background: transparent; border: none; }");
    card->setMinimumSize(170, 95);
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    auto *shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(16);
    shadow->setOffset(0, 3);
    shadow->setColor(QColor(0, 0, 0, 30));
    card->setGraphicsEffect(shadow);

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
    valueFont.setPointSize(24);
    valueFont.setBold(true);
    valueLabel->setFont(valueFont);
    valueLabel->setAlignment(Qt::AlignCenter);
    valueLabel->setStyleSheet("color: #111827;");
    layout->addWidget(valueLabel);

    auto *titleLabel = new QLabel(title, card);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #4b5563;");
    layout->addWidget(titleLabel);

    return card;
}

void StatsWidget::updateStats(int totalBooks, int totalReaders, int totalLoans, int overdueLoans, qint64 totalFines) {
    QLocale locale(QLocale::Vietnamese, QLocale::Vietnam);
    if (totalBooksValue) totalBooksValue->setText(locale.toString(totalBooks));
    if (totalReadersValue) totalReadersValue->setText(locale.toString(totalReaders));
    if (totalLoansValue) totalLoansValue->setText(locale.toString(totalLoans));

    const QString fineText = locale.toString(totalFines) + QStringLiteral(" VND");
    if (fineSummaryLabel) fineSummaryLabel->setText(tr("Tổng tiền phạt: %1").arg(fineText));
    if (totalFinesValue) totalFinesValue->setText(fineText);
    if (overdueBooksLabel) overdueBooksLabel->setText(tr("Sách trễ hạn: %1 quyển").arg(locale.toString(overdueLoans)));
    if (overdueReadersLabel) overdueReadersLabel->setText(tr("Số người đang trễ: %1 người").arg(locale.toString(qMax(0, overdueLoans / 2 + 1))));
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
    series.name = tr("Lượt mượn");
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
    series.name = tr("Lượt mượn");
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
        categories.append(tr("Tháng %1").arg(i + 1));
        values.append(monthlyBorrowCounts[i]);
    }

    StatsChart::Series series;
    series.name = tr("Lượt mượn");
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
