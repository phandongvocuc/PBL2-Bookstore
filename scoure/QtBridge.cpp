#include "QtBridge.h"

#include <QByteArray>
#include <QDateTime>
#include <QTimeZone>
#include <QString>

using namespace std;  // project-wide request

namespace pbl2::bridge {

    custom::CustomString toCustomString(const QString &value) {
        const QByteArray bytes = value.toUtf8();
        return custom::CustomString(bytes.constData(), static_cast<custom::CustomString::SizeType>(bytes.size()));
    }

    QString toQString(const custom::CustomString &value) {
        return QString::fromUtf8(reinterpret_cast<const char *>(value.bytes()), static_cast<int>(value.length()));
    }

    core::Date toCoreDate(const QDate &value) {
        if (!value.isValid()) return core::Date();
        return core::Date(value.year(), value.month(), value.day());
    }

    QDate toQDate(const core::Date &value) {
        if (!value.isValid()) return QDate();
        return QDate(value.year(), value.month(), value.day());
    }

    core::DateTime toCoreDateTime(const QDateTime &value) {
        if (!value.isValid()) return core::DateTime();
        const QDateTime utc = value.toUTC();
        const QDate date = utc.date();
        const QTime time = utc.time();
        return core::DateTime(date.year(),
                              date.month(),
                              date.day(),
                              time.hour(),
                              time.minute(),
                              time.second(),
                              time.msec());
    }

    QDateTime toQDateTime(const core::DateTime &value) {
        if (!value.isValid()) return QDateTime();
        const QDate date(value.year(), value.month(), value.day());
        const QTime time(value.hour(), value.minute(), value.second(), value.millisecond());
        return QDateTime(date, time, QTimeZone::utc());
    }

}
