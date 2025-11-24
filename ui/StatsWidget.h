#pragma once

#include <QLabel>
#include <QFrame>
#include <QListWidget>
#include "../core/DynamicArray.h"
#include "../core/Map.h"
#include "StatsChart.h"
#include "PieChart.h"

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

    void updatePieChart(int cardFees, int fines) const;
    
    void updateLoansList(const core::DynamicArray<QPair<QString, QString>> &loans) const;
    
    void updateActiveReadersList(const core::DynamicArray<QString> &readers) const;

private:
    void setupUi();
    
    // Summary cards
    QLabel *totalBooksValue{nullptr};
    QLabel *totalReadersValue{nullptr};
    QLabel *totalLoansValue{nullptr};
    QLabel *totalFinesValue{nullptr};
    QLabel *fineSummaryLabel{nullptr};
    QLabel *overdueBooksLabel{nullptr};
    QLabel *overdueReadersLabel{nullptr};
    
    // Charts
    StatsChart *topBooksChart{nullptr};
    StatsChart *categoryChart{nullptr};
    StatsChart *monthlyChart{nullptr};
    PieChart *revenuePieChart{nullptr};
    
    // Lists
    QListWidget *recentLoansList{nullptr};
    QListWidget *activeReadersList{nullptr};
    
    QFrame *createStatsCard(const QString &title, const QString &icon = QString());
};

}  // namespace pbl2::ui
