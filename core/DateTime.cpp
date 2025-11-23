#include "core/DateTime.h"

#include <chrono>

using namespace std;

namespace pbl2::core {
namespace {

bool isLeap(const int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

int daysInMonth(const int year, const int month) {
    static const int table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month < 1 || month > 12) return 0;
    if (month == 2 && isLeap(year)) return 29;
    return table[month - 1];
}
}  // namespace

DateTime::DateTime(const int year, const int month, const int day, const int hour, const int minute, const int second, const int millisecond)
    : year_(year),
      month_(month),
      day_(day),
      hour_(hour),
      minute_(minute),
      second_(second),
      millisecond_(millisecond) {}

bool DateTime::isValid() const {
    if (year_ <= 0 || month_ <= 0 || day_ <= 0) return false;
    if (hour_ < 0 || hour_ > 23) return false;
    if (minute_ < 0 || minute_ > 59) return false;
    if (second_ < 0 || second_ > 59) return false;
    if (millisecond_ < 0 || millisecond_ > 999) return false;
    if (const int dim = daysInMonth(year_, month_); dim == 0 || day_ > dim) return false;
    return true;
}

core::CustomString DateTime::toCompactTimestamp() const {
    if (!isValid()) return {};
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%04d%02d%02d%02d%02d%02d%03d", year_, month_, day_, hour_, minute_, second_, millisecond_);
    return core::CustomString(buffer);
}

DateTime DateTime::nowUtc() {
    const auto now = chrono::system_clock::now();
    const auto nowMs = chrono::time_point_cast<chrono::milliseconds>(now);
    const auto millis = nowMs.time_since_epoch();
    const auto secs = chrono::duration_cast<chrono::seconds>(millis);
    const int millisecondPart = static_cast<int>((millis - secs).count());
    const time_t seconds = chrono::system_clock::to_time_t(nowMs);
    tm tmValue {};
#if defined(_WIN32)
    gmtime_s(&tmValue, &seconds);
#else
    gmtime_r(&seconds, &tmValue);
#endif
    return {tmValue.tm_year + 1900,
                    tmValue.tm_mon + 1,
                    tmValue.tm_mday,
                    tmValue.tm_hour,
                    tmValue.tm_min,
                    tmValue.tm_sec,
                    millisecondPart};
}

}  // namespace pbl2::core
