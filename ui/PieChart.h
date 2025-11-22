#pragma once

#include <QWidget>
#include "../core/CustomContainers.h"
#include <QPair>
#include <QString>
#include <QColor>

namespace pbl2::ui {

class PieChart : public QWidget {
    Q_OBJECT
public:
    explicit PieChart(QWidget *parent = nullptr);

    // segments: pair<label, value>
    void setSegments(const core::Vector<QPair<QString, int>> &segments);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    core::Vector<QPair<QString, int>> segments_;
    core::Vector<QColor> colors_;
    void ensureColors(int n);
};

}  // namespace pbl2::ui

using PieChart = pbl2::ui::PieChart;
