#pragma once

#include <QFrame>
#include <QLabel>
#include <QListWidget>
#include <QProgressBar>
#include "../core/DynamicArray.h"
#include "../core/Map.h"
#include "StatsChart.h"

namespace pbl2::ui {

class StatsWidget final : public QWidget {
    Q_OBJECT

public:
    explicit StatsWidget(QWidget *parent = nullptr);

    void updateStats(int totalBooks, int totalReaders, int totalLoans,
                     int overdueLoans, qint64 totalFines) const;

    void updateTopBooksChart(const core::Map<QString, int> &bookBorrowCounts) const;

    void updateCategoryChart(const core::Map<QString, int> &categoryBorrowCounts) const;

    void updateMonthlyChart(const core::DynamicArray<int> &monthlyBorrowCounts) const;

private:
    void setupUi();
    QFrame *createMetricCard(const QString &iconPath, const QColor &accent,
                             const QString &title, QLabel *&valueLabel);
    QWidget *buildTopBookRow(const QString &title, int count, int maxCount, int colorIndex) const;

    QLabel *totalBooksValue{nullptr};
    QLabel *totalReadersValue{nullptr};
    QLabel *totalLoansValue{nullptr};
    QLabel *totalFinesValue{nullptr};
    QLabel *fineHeadlineLabel{nullptr};
    QLabel *overdueBooksLabel{nullptr};

    StatsChart *categoryChart{nullptr};
    StatsChart *monthlyChart{nullptr};
    QListWidget *topBooksList{nullptr};
};

}  // namespace pbl2::ui

using StatsWidget = pbl2::ui::StatsWidget;
