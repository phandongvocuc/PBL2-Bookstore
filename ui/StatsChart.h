#pragma once

#include <QWidget>
#include <QStringList>
#include <QVector>
#include <QColor>

namespace pbl2::ui {

class StatsChart final : public QWidget {
    Q_OBJECT
    
public:
    enum class Mode { Bar, Line, StackedBar };

    struct Series {
        QString name;
        QVector<double> values;
        QColor color;
    };

    explicit StatsChart(QWidget *parent = nullptr);

    void setMode(Mode mode);
    void setTitle(const QString &title);
    void setCategories(const QStringList &categories);
    void setSeries(const QVector<Series> &series);
    void setValueSuffix(const QString &suffix);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Mode mode_{Mode::Bar};
    QString title_;
    QString valueSuffix_;
    QStringList categories_;
    QVector<Series> series_;

    void drawBackground(QPainter &painter) const;
    void drawTitle(QPainter &painter, const QRect &area) const;
    void drawAxes(QPainter &painter, const QRect &plotArea) const;
    void drawBarChart(QPainter &painter, const QRect &plotArea) const;
    void drawStackedBarChart(QPainter &painter, const QRect &plotArea) const;
    void drawLineChart(QPainter &painter, const QRect &plotArea) const;
    void drawLegend(QPainter &painter, const QRect &plotArea) const;
    double findMaxValue() const;
};

}  // namespace pbl2::ui

using StatsChart = pbl2::ui::StatsChart;
