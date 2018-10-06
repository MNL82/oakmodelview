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

#define __STDC_WANT_LIB_EXT1__ 1
#include <time.h>

namespace Oak {
namespace Model {

// =============================================================================
// (public)
DateTime::DateTime()
{
    m_time = std::chrono::system_clock::now();
}

// =============================================================================
// (public)
DateTime::DateTime(const std::string &str, const std::string & locale)
{

    std::tm tm = {};
    std::istringstream ss(str);
    ss.imbue(std::locale(locale));
    ss >> std::get_time(&tm, "%Y %m %d  %H:%M:%S");
    m_time = std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

// =============================================================================
// (public)
int DateTime::year() const
{
    time_t tt = std::chrono::system_clock::to_time_t(m_time);
    tm tm;
    gmtime_s(&tm, &tt);
    return 1900 +  tm.tm_year;
}

// =============================================================================
// (public)
int DateTime::month() const
{
    time_t tt = std::chrono::system_clock::to_time_t(m_time);
    tm tm;
    gmtime_s(&tm, &tt);
    return 1 +  tm.tm_mon;
}

// =============================================================================
// (public)
int DateTime::day() const
{
    time_t tt = std::chrono::system_clock::to_time_t(m_time);
    tm tm;
    gmtime_s(&tm, &tt);
    return tm.tm_mday;
}

// =============================================================================
// (public)
int DateTime::hour() const
{
    std::chrono::system_clock::duration tp = m_time.time_since_epoch();
    tp -= std::chrono::duration_cast<chrone_days>(tp);
    return std::chrono::duration_cast<std::chrono::hours>(tp).count();
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
ProcessedDateTime DateTime::dateTimeUTC() const
{
    time_t tt = std::chrono::system_clock::to_time_t(m_time);
    tm tm;
    gmtime_s(&tm, &tt);
    return _dateTime(tm);
}

// =============================================================================
// (public)
ProcessedDateTime DateTime::dateTimeLocal() const
{
    time_t tt = std::chrono::system_clock::to_time_t(m_time);
    tm tm;
    localtime_s(&tm, &tt);
    return _dateTime(tm);
}

// =============================================================================
// (public)
std::string DateTime::toStringLocal() const
{
    return toString(dateTimeLocal());
}

// =============================================================================
// (public)
std::string DateTime::toStringUTC() const
{
    return toString(dateTimeUTC());
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
std::string DateTime::toString(const char *format)
{
    std::time_t tt = std::time(nullptr);
    std::tm tm;
    localtime_s(&tm, &tt);

    std::ostringstream ss;
    ss << std::put_time(&tm, format);
    return ss.str();
}

// =============================================================================
// (protected)
ProcessedDateTime DateTime::_dateTime(tm t) const
{
    ProcessedDateTime pdt;

    pdt.year = 1900 + t.tm_year;
    pdt.month = 1 + t.tm_mon;
    pdt.day = t.tm_mday;

    pdt.hour = t.tm_hour;
    pdt.minute = t.tm_min;
    pdt.secound = t.tm_sec;

    return pdt;
}


} // namespace Model
} // namespace Oak
