#pragma once

#include <QWidget>
#include "core/custom/QtContainers.h"
#include <QPair>
#include <QString>
#include <QColor>

namespace pbl2::ui {

class PieChart : public QWidget {
    Q_OBJECT
public:
    explicit PieChart(QWidget *parent = nullptr);

    // segments: pair<label, value>
    void setSegments(const custom::Vector<QPair<QString, int>> &segments);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    custom::Vector<QPair<QString, int>> segments_;
    custom::Vector<QColor> colors_;
    void ensureColors(int n);
};

}  // namespace pbl2::ui

using PieChart = pbl2::ui::PieChart;
