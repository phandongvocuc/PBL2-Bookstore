#include "StatsWidget.h"

#include <QFont>
#include <QGraphicsDropShadowEffect>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLocale>
#include <QPixmap>
#include <QScrollArea>
#include <QVBoxLayout>
#include <algorithm>
#include <ranges>

namespace pbl2::ui {

StatsWidget::StatsWidget(QWidget *parent) : QWidget(parent) {
    setupUi();
}

static void applyShadow(QWidget *widget, const qreal blur = 22.0, const QPointF &offset = QPointF(0, 6)) {
    if (!widget) return;
    auto *shadow = new QGraphicsDropShadowEffect(widget);
    shadow->setBlurRadius(blur);
    shadow->setOffset(offset);
    shadow->setColor(QColor(0, 0, 0, 35));
    widget->setGraphicsEffect(shadow);
}

void StatsWidget::setupUi() {
    setObjectName(QStringLiteral("statsRoot"));
    setStyleSheet(
        "QWidget#statsRoot {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #55b5fa, stop:1 #3f9fe8);"
        "}"
        "QLabel { color: #0b1524; }"
        "QListWidget { border: none; }"
        "QListWidget::item { border: none; }"
    );

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(14, 14, 14, 14);
    mainLayout->setSpacing(14);

    auto *titleRow = new QHBoxLayout();
    auto *titleLabel = new QLabel(tr("THỐNG KÊ"), this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(22);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #ffffff;");
    titleRow->addWidget(titleLabel);
    titleRow->addStretch();
    mainLayout->addLayout(titleRow);

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { background: transparent; }");

    auto *scrollContent = new QWidget();
    auto *contentLayout = new QVBoxLayout(scrollContent);
    contentLayout->setSpacing(12);
    contentLayout->setContentsMargins(4, 4, 4, 10);

    // Summary cards
    auto *cardsRow = new QHBoxLayout();
    cardsRow->setSpacing(12);

    auto *booksCard = createMetricCard(QStringLiteral(":/icons/book.png"), QColor(0x2b, 0x8b, 0xe6),
                                       tr("Tổng số sách"), totalBooksValue);
    cardsRow->addWidget(booksCard);

    auto *loansCard = createMetricCard(QStringLiteral(":/icons/loan.png"), QColor(0x22, 0xc5, 0x67),
                                       tr("Sách đang được mượn"), totalLoansValue);
    cardsRow->addWidget(loansCard);

    auto *readersCard = createMetricCard(QStringLiteral(":/icons/reader.png"), QColor(0x64, 0x8d, 0xff),
                                         tr("Tổng số độc giả"), totalReadersValue);
    cardsRow->addWidget(readersCard);

    auto *finesCard = createMetricCard(QStringLiteral(":/icons/report.png"), QColor(0xff, 0xb0, 0x4a),
                                       tr("Tổng tiền phạt"), totalFinesValue);
    cardsRow->addWidget(finesCard);

    contentLayout->addLayout(cardsRow);

    const QString panelStyle = QStringLiteral(
        "QFrame { background: #fdfdfd; border-radius: 18px; }"
        "QLabel { color: #0f172a; }");

    // Charts row
    auto *chartsRow = new QHBoxLayout();
    chartsRow->setSpacing(12);

    auto *categoryPanel = new QFrame(this);
    categoryPanel->setStyleSheet(panelStyle);
    applyShadow(categoryPanel);
    auto *categoryLayout = new QVBoxLayout(categoryPanel);
    categoryLayout->setContentsMargins(16, 14, 16, 14);
    categoryLayout->setSpacing(10);
    auto *categoryLabel = new QLabel(tr("Lượt mượn theo thể loại"), categoryPanel);
    QFont sectionFont = categoryLabel->font();
    sectionFont.setPointSize(12);
    sectionFont.setBold(true);
    categoryLabel->setFont(sectionFont);
    categoryLayout->addWidget(categoryLabel);

    categoryChart = new StatsChart(categoryPanel);
    categoryChart->setShowLegend(false);
    categoryChart->setMode(StatsChart::Mode::Bar);
    categoryChart->setAxisLabels(tr("Thể loại"), tr("Sách"));
    categoryChart->setMinimumWidth(700);  // kéo dài trục Ox cho nhiều nhãn
    categoryChart->setMinimumHeight(220); // kéo trục Ox dài hơn cho nhiều nhãn
    categoryLayout->addWidget(categoryChart);
    chartsRow->addWidget(categoryPanel, 7);

    auto *topBooksPanel = new QFrame(this);
    topBooksPanel->setStyleSheet(panelStyle);
    applyShadow(topBooksPanel);
    auto *topLayout = new QVBoxLayout(topBooksPanel);
    topLayout->setContentsMargins(16, 14, 16, 14);
    topLayout->setSpacing(10);

    auto *topLabel = new QLabel(tr("Top sách được mượn nhiều nhất"), topBooksPanel);
    topLabel->setFont(sectionFont);
    topLayout->addWidget(topLabel);

    topBooksList = new QListWidget(topBooksPanel);
    topBooksList->setSpacing(8);
    topBooksList->setFrameShape(QFrame::NoFrame);
    topBooksList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    topBooksList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    topLayout->addWidget(topBooksList);

    chartsRow->addWidget(topBooksPanel, 5);
    contentLayout->addLayout(chartsRow);

    // Bottom row
    auto *bottomRow = new QHBoxLayout();
    bottomRow->setSpacing(12);

    auto *monthlyPanel = new QFrame(this);
    monthlyPanel->setStyleSheet(panelStyle);
    applyShadow(monthlyPanel);
    auto *monthlyLayout = new QVBoxLayout(monthlyPanel);
    monthlyLayout->setContentsMargins(16, 14, 16, 14);
    monthlyLayout->setSpacing(10);

    auto *monthlyLabel = new QLabel(tr("Lượt mượn theo thời gian"), monthlyPanel);
    monthlyLabel->setFont(sectionFont);
    monthlyLayout->addWidget(monthlyLabel);

    monthlyChart = new StatsChart(monthlyPanel);
    monthlyChart->setMode(StatsChart::Mode::Line);
    monthlyChart->setShowLegend(false);
    monthlyChart->setAxisLabels(tr("Tháng"), tr("Sách"));
    monthlyChart->setMinimumHeight(300);
    monthlyLayout->addWidget(monthlyChart);
    bottomRow->addWidget(monthlyPanel, 7);

    auto *finePanel = new QFrame(this);
    finePanel->setStyleSheet(panelStyle);
    applyShadow(finePanel);
    auto *fineLayout = new QVBoxLayout(finePanel);
    fineLayout->setContentsMargins(16, 14, 16, 14);
    fineLayout->setSpacing(8);

    auto *fineTitle = new QLabel(tr("Thống kê phạt"), finePanel);
    fineTitle->setFont(sectionFont);
    fineLayout->addWidget(fineTitle);

    fineHeadlineLabel = new QLabel(tr("Tạm tính: 0 VND"), finePanel);
    overdueBooksLabel = new QLabel(tr("Sách trễ hạn: 0"), finePanel);
    const QString subStyle = QStringLiteral("color: #111827; font-size: 10pt;");
    fineHeadlineLabel->setStyleSheet(subStyle);
    overdueBooksLabel->setStyleSheet(subStyle);

    fineLayout->addWidget(fineHeadlineLabel);
    fineLayout->addWidget(overdueBooksLabel);
    fineLayout->addSpacing(6);

    auto *note = new QLabel(tr("Số liệu được lọc theo khoảng thời gian chọn ở trên."), finePanel);
    note->setStyleSheet("color: #6b7280; font-size: 9pt;");
    fineLayout->addWidget(note);
    fineLayout->addStretch();

    bottomRow->addWidget(finePanel, 5);
    contentLayout->addLayout(bottomRow);

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);
}

QFrame *StatsWidget::createMetricCard(const QString &iconPath, const QColor &accent,
                                      const QString &title, QLabel *&valueLabel) {
    auto *card = new QFrame(this);
    card->setStyleSheet(
        "QFrame { background: #fdfdfd; border-radius: 16px; }"
        "QLabel { background: transparent; }");
    card->setMinimumHeight(120);
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    applyShadow(card, 20.0, QPointF(0, 5));

    auto *layout = new QHBoxLayout(card);
    layout->setContentsMargins(14, 12, 14, 12);
    layout->setSpacing(12);

    auto *iconWrap = new QLabel(card);
    iconWrap->setFixedSize(52, 52);
    iconWrap->setAlignment(Qt::AlignCenter);
    iconWrap->setStyleSheet(QStringLiteral("background:%1; border-radius:26px;").arg(accent.name()));
    if (!iconPath.isEmpty()) {
        QPixmap pix(iconPath);
        iconWrap->setPixmap(pix.scaled(28, 28, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    layout->addWidget(iconWrap);

    auto *textCol = new QVBoxLayout();
    textCol->setSpacing(2);

    valueLabel = new QLabel(QStringLiteral("0"), card);
    QFont valueFont = valueLabel->font();
    valueFont.setPointSize(24);
    valueFont.setBold(true);
    valueLabel->setFont(valueFont);
    valueLabel->setStyleSheet("color: #0b1524;");
    textCol->addWidget(valueLabel);

    auto *titleLabel = new QLabel(title, card);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #4b5563;");
    textCol->addWidget(titleLabel);

    layout->addLayout(textCol);
    layout->addStretch();
    return card;
}

QWidget *StatsWidget::buildTopBookRow(const QString &title, const int count, const int maxCount, const int colorIndex) const {
    auto *rowWidget = new QWidget(topBooksList);
    auto *rowLayout = new QHBoxLayout(rowWidget);
    rowLayout->setContentsMargins(0, 0, 0, 0);
    rowLayout->setSpacing(10);

    auto *titleLabel = new QLabel(title, rowWidget);
    titleLabel->setStyleSheet("color: #0b1524; font-weight: 600;");
    rowLayout->addWidget(titleLabel, 2);

    auto *bar = new QProgressBar(rowWidget);
    bar->setRange(0, qMax(1, maxCount));
    bar->setValue(count);
    bar->setTextVisible(false);
    bar->setFixedHeight(12);
    static const QVector<QColor> colors = {
        QColor(0x1d, 0x4e, 0xd8),
        QColor(0x2d, 0xa5, 0x6a),
        QColor(0xf2, 0x9d, 0x2e),
        QColor(0xf5, 0x4b, 0x4b),
        QColor(0x8b, 0x5c, 0xe0)
    };
    const QColor accent = colors[colorIndex % colors.size()];
    bar->setStyleSheet(QStringLiteral(
        "QProgressBar {"
        "  background: #e5e7eb;"
        "  border: none;"
        "  border-radius: 6px;"
        "}"
        "QProgressBar::chunk {"
        "  border-radius: 6px;"
        "  background: %1;"
        "}").arg(accent.name()));
    rowLayout->addWidget(bar, 4);

    auto *countLabel = new QLabel(QString::number(count), rowWidget);
    countLabel->setStyleSheet("color: #0b1524; font-weight: bold;");
    rowLayout->addWidget(countLabel);

    return rowWidget;
}

void StatsWidget::updateStats(const int totalBooks, const int totalReaders, const int totalLoans, const int overdueLoans, const qint64 totalFines) const {
    const QLocale locale(QLocale::Vietnamese, QLocale::Vietnam);
    if (totalBooksValue) totalBooksValue->setText(locale.toString(totalBooks));
    if (totalReadersValue) totalReadersValue->setText(locale.toString(totalReaders));
    if (totalLoansValue) totalLoansValue->setText(locale.toString(totalLoans));
    if (totalFinesValue) totalFinesValue->setText(locale.toString(totalFines));

    const QString fineText = locale.toString(totalFines) + QStringLiteral(" VND");
    if (fineHeadlineLabel) fineHeadlineLabel->setText(tr("Tạm tính: %1").arg(fineText));
    if (overdueBooksLabel) overdueBooksLabel->setText(tr("Sách trễ hạn: %1").arg(locale.toString(overdueLoans)));
}

void StatsWidget::updateTopBooksChart(const core::Map<QString, int> &bookBorrowCounts) const {
    if (!topBooksList) return;

    QList<QPair<QString, int>> sortedBooks;
    for (auto it = bookBorrowCounts.constBegin(); it != bookBorrowCounts.constEnd(); ++it) {
        sortedBooks.append(qMakePair(it.key(), it.value()));
    }
    std::ranges::sort(sortedBooks, [](const QPair<QString, int> &a, const QPair<QString, int> &b) {
        return a.second > b.second;
    });

    const int displayCount = qMin(5, sortedBooks.size());
    int maxCount = 0;
    for (int i = 0; i < displayCount; ++i) {
        maxCount = qMax(maxCount, sortedBooks[i].second);
    }

    topBooksList->clear();
    for (int i = 0; i < displayCount; ++i) {
        const auto &entry = sortedBooks[i];
        auto *item = new QListWidgetItem(topBooksList);
        QWidget *row = buildTopBookRow(entry.first, entry.second, maxCount, i);
        item->setSizeHint(QSize(item->sizeHint().width(), 32));
        topBooksList->addItem(item);
        topBooksList->setItemWidget(item, row);
    }
}

void StatsWidget::updateCategoryChart(const core::Map<QString, int> &categoryBorrowCounts) const {
    if (!categoryChart) return;

    QList<QPair<QString, int>> items;
    for (auto it = categoryBorrowCounts.constBegin(); it != categoryBorrowCounts.constEnd(); ++it) {
        items.append(qMakePair(it.key(), it.value()));
    }
    std::ranges::sort(items, [](const QPair<QString, int> &a, const QPair<QString, int> &b) {
        if (a.second == b.second) return a.first.toLower() < b.first.toLower();
        return a.second > b.second;
    });

    // Giới hạn 8 cột để tránh dồn label, gom phần còn lại vào mục "Khác (+N)".
    QStringList categories;
    core::DynamicArray<double> values;
    const int limit = 8;
    int othersTotal = 0;
    for (int i = 0; i < items.size(); ++i) {
        if (i < limit) {
            categories.append(items[i].first);
            values.append(items[i].second);
        } else {
            othersTotal += items[i].second;
        }
    }
    if (items.size() > limit) {
        categories.append(tr("Khác (+%1)").arg(items.size() - limit));
        values.append(othersTotal);
    }

    StatsChart::Series series;
    series.name = tr("Lượt mượn");
    series.values = values;
    series.color = QColor(0x20, 0x9c, 0xf0);

    core::DynamicArray<StatsChart::Series> seriesVec;
    seriesVec.append(series);
    categoryChart->setCategories(categories);
    categoryChart->setSeries(seriesVec);
    categoryChart->setMode(StatsChart::Mode::Bar);
    categoryChart->setShowLegend(false);
}

void StatsWidget::updateMonthlyChart(const core::DynamicArray<int> &monthlyBorrowCounts) const {
    if (!monthlyChart) return;

    QStringList categories;
    core::DynamicArray<double> values;
    const int size = monthlyBorrowCounts.size();
    for (int i = 0; i < size; ++i) {
        categories.append(tr("T%1").arg(i + 1));
        values.append(monthlyBorrowCounts[i]);
    }

    StatsChart::Series series;
    series.name = tr("Lượt mượn");
    series.values = values;
    series.color = QColor(0x1d, 0x4e, 0xd8);

    core::DynamicArray<StatsChart::Series> seriesVec;
    seriesVec.append(series);
    monthlyChart->setCategories(categories);
    monthlyChart->setSeries(seriesVec);
    monthlyChart->setMode(StatsChart::Mode::Line);
    monthlyChart->setShowLegend(false);
}

}  // namespace pbl2::ui
