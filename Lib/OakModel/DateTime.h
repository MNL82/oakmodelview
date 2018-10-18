/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#pragma once

#include <string>
#include <chrono>

namespace Oak {
namespace Model {

// =============================================================================
// Struct definition
// =============================================================================
struct ProcessedDateTime {
    int year;
    int month;
    int day;

    int hour;
    int minute;
    int secound;

    ProcessedDateTime & operator-=(const ProcessedDateTime &other)
    {
        year -= other.year;
        month -= other.month;
        day -= other.day;

        hour -= other.hour;
        minute -= other.minute;
        secound -= other.secound;
        return *this;
    }

    bool isNull() const
    {
        return year == 0 &&
               month == 0 &&
               day == 0 &&
               hour == 0 &&
               minute == 0 &&
               secound == 0;
    }
};

// =============================================================================
// Class definition
// =============================================================================
class DateTime
{
public:

    enum class TimeZone {
        UTC = 0,
        Local = 1
    };

    DateTime();
    DateTime(int year, int month, int day, int hour = 0, int minute = 0, int secound = 0, TimeZone timeZone = TimeZone::Local);
    DateTime(const std::string &str, TimeZone timeZone = TimeZone::Local, const char *format = "%Y-%m-%d %H:%M:%S");

    DateTime(const DateTime &copy) noexcept;
    DateTime(DateTime &&move) noexcept;

    DateTime &operator=(const DateTime &copy);
    DateTime &operator=(DateTime &&move);

    bool operator==(const DateTime &other) const;
    bool operator!=(const DateTime &other) const;

    bool operator<(const DateTime &other) const;

    bool isNull() const;

    static DateTime now();

    void swap(DateTime &other) noexcept;

    int year(DateTime::TimeZone timeZone = TimeZone::Local) const;
    int month(DateTime::TimeZone timeZone = TimeZone::Local) const;
    int day(TimeZone timeZone = TimeZone::Local) const;

    int hour(DateTime::TimeZone timeZone = TimeZone::Local) const;
    int minute() const;
    int secound() const;

    ProcessedDateTime toStruct(TimeZone timeZone = TimeZone::Local) const;

    std::string toString(TimeZone timeZone = TimeZone::Local, const char *format = "%Y-%m-%d %H:%M:%S") const;
    std::string toString(const ProcessedDateTime & pdt) const;

    static const DateTime & emptyDateTime();

protected:
    static int _monthDaySum(size_t month);
    static int _monthDays(size_t month);

    static const std::chrono::system_clock::time_point & _emptyTimePoint();

protected:
    std::chrono::system_clock::time_point m_time;
};

void swap(Oak::Model::DateTime &left, Oak::Model::DateTime &right) noexcept;

typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24> >::type> chrono_days;

} // namespace Model
} // namespace Oak
