#include "PieChart.h"

#include <QColor>
#include <QPainter>
#include <QPaintEvent>
#include <QtMath>

using namespace std;  // project-wide request

namespace pbl2::ui {

PieChart::PieChart(QWidget *parent) : QWidget(parent) {
    setMinimumSize(200, 200);
}

void PieChart::setSegments(const custom::Vector<QPair<QString, int>> &segments) {
    segments_ = segments;
    ensureColors(segments_.size());
    update();
}

void PieChart::ensureColors(int n) {
    static const custom::Vector<QColor> base{
        QColor(0x4E, 0x79, 0xA7), QColor(0xA6, 0xCD, 0x6B), QColor(0xF6, 0xC1, 0x3E),
        QColor(0xE8, 0x5C, 0x4A), QColor(0x8E, 0x6D, 0xC7), QColor(0x4A, 0xC4, 0xC9),
        QColor(0xFF, 0x99, 0xCC), QColor(0x99, 0xCC, 0xFF)
    };
    colors_.clear();
    for (int i = 0; i < n; ++i) {
        colors_.push_back(base[i % base.size()].lighter(100 + (i / base.size()) * 10));
    }
}

void PieChart::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const int w = width();
    const int h = height();
    const int size = qMin(w, h) - 20;
    const QRectF pieRect((w - size) / 2.0, 10, size, size);

    qreal total = 0;
    for (const auto &s : segments_) total += s.second;
    if (total <= 0) {
        p.drawText(rect(), Qt::AlignCenter, QObject::tr("Khong co du lieu"));
        return;
    }

    qreal startAngle = 0.0;
    for (int i = 0; i < segments_.size(); ++i) {
        const qreal value = segments_[i].second;
        const qreal span = value / total * 360.0;
        p.setBrush(colors_.value(i, QColor(Qt::gray)));
        p.setPen(Qt::NoPen);
        p.drawPie(pieRect, int(startAngle * 16), int(span * 16));
        startAngle += span;
    }

    // draw legend on right
    const int legendX = pieRect.right() + 10;
    int legendY = int(pieRect.top());
    const int boxSize = 12;
    p.setPen(Qt::NoPen);
    for (int i = 0; i < segments_.size(); ++i) {
        p.setBrush(colors_.value(i, QColor(Qt::gray)));
        p.drawRect(legendX, legendY + i * 18, boxSize, boxSize);
        p.setPen(Qt::black);
        p.drawText(legendX + boxSize + 6,
                   legendY + i * 18 + boxSize,
                   segments_[i].first + QStringLiteral(" (%1)").arg(segments_[i].second));
        p.setPen(Qt::NoPen);
    }
}

}  // namespace ui
