#pragma once

#include <QWidget>
#include <QLabel>
#include <QFrame>
#include <QListWidget>
#include "core/custom/QtContainers.h"
#include "StatsChart.h"
#include "PieChart.h"

namespace pbl2::ui {

class StatsWidget : public QWidget {
    Q_OBJECT

public:
    explicit StatsWidget(QWidget *parent = nullptr);

    void updateStats(int totalBooks, int totalReaders, int totalLoans,
                     int overdueLoans, qint64 totalFines);
    
    void updateTopBooksChart(const custom::Map<QString, int> &bookBorrowCounts);
    
    void updateCategoryChart(const custom::Map<QString, int> &categoryBorrowCounts);

    void updateMonthlyChart(const custom::Vector<int> &monthlyBorrowCounts);

    void updatePieChart(int cardFees, int fines);
    
    void updateLoansList(const custom::Vector<QPair<QString, QString>> &loans);
    
    void updateActiveReadersList(const custom::Vector<QString> &readers);

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
