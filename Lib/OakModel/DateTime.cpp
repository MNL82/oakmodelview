/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "DateTime.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <vector>

#include <time.h>

#include "../ServiceFunctions/Trace.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
DateTime::DateTime()
    : m_time { _emptyTimePoint() }
{
    //TRACE(L"Constructor");
}

// =============================================================================
// (public)
DateTime::DateTime(int year, int month, int day, int hour, int minute, int secound, TimeZone timeZone)
    : DateTime(toString(ProcessedDateTime{ year, month, day, hour, minute, secound }), timeZone, "%Y-%m-%d %H:%M:%S")
{
}

// =============================================================================
// (public)
DateTime::DateTime(const std::string &inputStr, TimeZone timeZone, const char * format)
{
    //TRACE(L"Constructor(%S, %S, %S)", str.c_str(), timeZone == TimeZone::UTC ? "UTC" : "Local", format);
    std::tm tm = {};
    std::istringstream iss(inputStr);
    iss >> std::get_time(&tm, format);

    if (iss.fail()) {
        TRACE(L"Faild to construct DateTime from string %S (%S)", inputStr.c_str(), format);
        return;
    }
    std::time_t tt = std::mktime(&tm);
    m_time = std::chrono::system_clock::from_time_t(tt);

    // Time is parsed without taking account for daylight savings flag
    if (tm.tm_isdst) { m_time -= std::chrono::hours(1); }

    if (timeZone == TimeZone::UTC) {
        // Time is parsed as local time and needs to be adjusted to UTC time
        std::tm tm_UTC;
        gmtime_s(&tm_UTC, &tt);

        int offset = tm.tm_hour - tm_UTC.tm_hour;

        // Adjust for changing day/year
        if (tm.tm_year != tm_UTC.tm_year) {
            offset += tm.tm_year > tm_UTC.tm_year ? 24 : -24;
        } else if (tm.tm_yday != tm_UTC.tm_yday) {
            offset += tm.tm_yday > tm_UTC.tm_yday ? 24 : -24;
        }

        //TRACE(L"Local time zone offset: %i", offset);
        m_time += std::chrono::hours(offset);
    }

    // End to end testing: toString() result should be equal to 'inputStr'
    std::string outputStr = toString(timeZone, format);
    if (inputStr != outputStr) {
        TRACE(L"Testing faild: %S != %S", inputStr.c_str(), outputStr.c_str());
        m_time = _emptyTimePoint();
        ASSERT(false);
    }
    // TESTING END
}

// =============================================================================
// (public)
DateTime::DateTime(const DateTime &copy) noexcept
    : m_time { copy.m_time }
{
    //TRACE(L"Copy constructor");
}

// =============================================================================
// (public)
DateTime::DateTime(DateTime &&move) noexcept
    : m_time { std::move(move.m_time) }
{
    //TRACE(L"Move constructor");
}

// =============================================================================
// (public)
DateTime &DateTime::operator=(const DateTime &copy)
{
    //TRACE(L"Copy assignment");
    m_time = copy.m_time;
    return *this;
}

// =============================================================================
// (public)
DateTime &DateTime::operator=(DateTime &&move)
{
    //TRACE(L"Move assignment");
    m_time = std::move(move.m_time);
    return *this;
}

// =============================================================================
// (public)
bool DateTime::operator==(const DateTime &other) const
{
    return m_time == other.m_time;
}

// =============================================================================
// (public)
bool DateTime::operator!=(const DateTime &other) const
{
    return m_time != other.m_time;
}

// =============================================================================
// (public)
bool DateTime::operator<(const DateTime &other) const
{
    return m_time < other.m_time;
}

// =============================================================================
// (public)
bool DateTime::isNull() const
{
    return m_time == _emptyTimePoint();
}

// =============================================================================
// (public)
DateTime DateTime::now()
{
    DateTime dt;
    dt.m_time = std::chrono::system_clock::now();
    return dt;
}

// =============================================================================
// (public)
void DateTime::swap(DateTime &other) noexcept
{
    //TRACE(L"swap()");
    std::swap(m_time, other.m_time);
}

// =============================================================================
// (public)
int DateTime::year(DateTime::TimeZone timeZone) const
{
    time_t tt = std::chrono::system_clock::to_time_t(m_time);
    tm tm;
    if (timeZone == TimeZone::UTC) {
        gmtime_s(&tm, &tt);
    } else if (timeZone == TimeZone::Local) {
        localtime_s(&tm, &tt);
    }
    return 1900 +  tm.tm_year;
}

// =============================================================================
// (public)
int DateTime::month(TimeZone timeZone) const
{
    time_t tt = std::chrono::system_clock::to_time_t(m_time);
    tm tm;
    if (timeZone == TimeZone::UTC) {
        gmtime_s(&tm, &tt);
    } else if (timeZone == TimeZone::Local) {
        localtime_s(&tm, &tt);
    }
    return 1 +  tm.tm_mon;
}

// =============================================================================
// (public)
int DateTime::day(DateTime::TimeZone timeZone) const
{
    time_t tt = std::chrono::system_clock::to_time_t(m_time);
    tm tm;
    if (timeZone == TimeZone::UTC) {
        gmtime_s(&tm, &tt);
    } else if (timeZone == TimeZone::Local) {
        localtime_s(&tm, &tt);
    }
    return tm.tm_mday;
}

// =============================================================================
// (public)
int DateTime::hour(DateTime::TimeZone timeZone) const
{
    time_t tt = std::chrono::system_clock::to_time_t(m_time);
    tm tm;
    if (timeZone == TimeZone::UTC) {
        gmtime_s(&tm, &tt);
    } else if (timeZone == TimeZone::Local) {
        localtime_s(&tm, &tt);
    }
    return tm.tm_hour;
}

// =============================================================================
// (public)
int DateTime::minute() const
{
    std::chrono::system_clock::duration tp = m_time.time_since_epoch();
    tp -= std::chrono::duration_cast<std::chrono::hours>(tp);
    return std::chrono::duration_cast<std::chrono::minutes>(tp).count();
}

// =============================================================================
// (public)
int DateTime::secound() const
{
    std::chrono::system_clock::duration tp = m_time.time_since_epoch();
    tp -= std::chrono::duration_cast<std::chrono::minutes>(tp);
    return static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(tp).count());
}

// =============================================================================
// (public)
ProcessedDateTime DateTime::toStruct(DateTime::TimeZone timeZone) const
{
    std::time_t tt = std::chrono::system_clock::to_time_t(m_time);
    std::tm tm;

    if (timeZone == TimeZone::UTC) {
        gmtime_s(&tm, &tt);
    } else if (timeZone == TimeZone::Local) {
        localtime_s(&tm, &tt);
    }

    ProcessedDateTime pdt;

    pdt.year = 1900 + tm.tm_year;
    pdt.month = 1 + tm.tm_mon;
    pdt.day = tm.tm_mday;

    pdt.hour = tm.tm_hour;
    pdt.minute = tm.tm_min;
    pdt.secound = tm.tm_sec;

    return pdt;
}

// =============================================================================
// (public)
std::string DateTime::toString(TimeZone timeZone, const char *format) const
{
    if (format == nullptr) {
        return toString(toStruct(timeZone));
    }
    std::time_t tt = std::chrono::system_clock::to_time_t(m_time);
    std::tm tm;

    if (timeZone == TimeZone::UTC) {
        gmtime_s(&tm, &tt);
    } else if (timeZone == TimeZone::Local) {
        localtime_s(&tm, &tt);
    }

    std::ostringstream ss;
    ss << std::put_time(&tm, format);
    return ss.str();
}

// =============================================================================
// (public)
std::string DateTime::toString(const ProcessedDateTime &pdt) const
{
    char dateTime[] = "0000-00-00 00:00:00";
    char *ptr = dateTime;

    *ptr++  += pdt.year/1000;
    *ptr++ += pdt.year/100%10;
    *ptr++ += pdt.year/10%10;
    *ptr++ += pdt.year%10;
    ptr++;
    *ptr++ += pdt.month/10;
    *ptr++ += pdt.month%10;
    ptr++;
    *ptr++ += pdt.day/10;
    *ptr++ += pdt.day%10;
    ptr++;
    *ptr++ += pdt.hour/10;
    *ptr++ += pdt.hour%10;
    ptr++;
    *ptr++ += pdt.minute/10;
    *ptr++ += pdt.minute%10;
    ptr++;
    *ptr++ += pdt.secound/10;
    *ptr++ += pdt.secound%10;

    return std::string(dateTime);
}

// =============================================================================
// (public)
const DateTime &DateTime::emptyDateTime()
{
    static DateTime s_emptyDateTime = DateTime();
    return s_emptyDateTime;
}

// =============================================================================
// (public)
// Return the number of days of the previous months
int DateTime::_monthDaySum(size_t month)
{
    ASSERT(month >= 1 && month <= 12);
    static std::vector<int> s_monthDaySum;
    if (s_monthDaySum.empty()) {
        s_monthDaySum.resize(13);
        s_monthDaySum[1] = 0;
        for (size_t i = 2; i < 13; i++) {
            s_monthDaySum[i] = s_monthDaySum[i-1] + _monthDays(i-1);
        }
    }
    return s_monthDaySum[month];
}

// =============================================================================
// (public)
int DateTime::_monthDays(size_t month)
{
    ASSERT(month >= 1 && month <= 12);
    static std::vector<int> s_monthDays;
    if (s_monthDays.empty()) {
        s_monthDays.resize(13);
        s_monthDays[1] = 31;
        s_monthDays[2] = 28;
        s_monthDays[3] = 31;
        s_monthDays[4] = 30;
        s_monthDays[5] = 31;
        s_monthDays[6] = 30;
        s_monthDays[7] = 31;
        s_monthDays[8] = 31;
        s_monthDays[9] = 30;
        s_monthDays[10] = 31;
        s_monthDays[11] = 30;
        s_monthDays[12] = 31;
    }
    return s_monthDays[month];
}

// =============================================================================
// (public)
const std::chrono::system_clock::time_point &DateTime::_emptyTimePoint()
{
    static auto s_emptyTimePoint = std::chrono::system_clock::time_point(std::chrono::seconds(-1));
    return s_emptyTimePoint;
}

// =============================================================================
// (public)
void swap(DateTime &left, DateTime &right) noexcept {
    left.swap(right);
}

} // namespace Model
} // namespace Oak
