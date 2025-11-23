#include "QtBridge.h"

#include <QByteArray>
#include <QDateTime>
#include <QTimeZone>
#include <QString>

using namespace std;

namespace pbl2::bridge {

    core::CustomString toCustomString(const QString &value) {
        const QByteArray bytes = value.toUtf8();
        return {bytes.constData(), static_cast<core::CustomString::SizeType>(bytes.size())};
    }

    QString toQString(const core::CustomString &value) {
        return QString::fromUtf8(reinterpret_cast<const char *>(value.bytes()), static_cast<int>(value.length()));
    }

    core::Date toCoreDate(const QDate &value) {
        if (!value.isValid()) return {};
        return {value.year(), value.month(), value.day()};
    }

    QDate toQDate(const core::Date &value) {
        if (!value.isValid()) return {};
        return {value.year(), value.month(), value.day()};
    }

    core::DateTime toCoreDateTime(const QDateTime &value) {
        if (!value.isValid()) return {};
        const QDateTime utc = value.toUTC();
        const QDate date = utc.date();
        const QTime time = utc.time();
        return {date.year(),
                              date.month(),
                              date.day(),
                              time.hour(),
                              time.minute(),
                              time.second(),
                              time.msec()};
    }

    QDateTime toQDateTime(const core::DateTime &value) {
        if (!value.isValid()) return {};
        const QDate date(value.year(), value.month(), value.day());
        const QTime time(value.hour(), value.minute(), value.second(), value.millisecond());
        return {date, time, QTimeZone::utc()};
    }

}
