#pragma once

#include <QColor>
#include <QPair>
#include <QString>
#include <QWidget>
#include "../core/DynamicArray.h"

namespace pbl2::ui {

class PieChart : public QWidget {
    Q_OBJECT
public:
    explicit PieChart(QWidget *parent = nullptr);

    void setSegments(const core::DynamicArray<QPair<QString, int>> &segments);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    core::DynamicArray<QPair<QString, int>> segments_;
    core::DynamicArray<QColor> colors_;
    void ensureColors(int n);
};

}  // namespace pbl2::ui

using PieChart = pbl2::ui::PieChart;
