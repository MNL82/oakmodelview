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
};

// =============================================================================
// Class definition
// =============================================================================
class DateTime
{
public:
    DateTime();
    DateTime(const std::string &str, const std::string &locale = "");

    int year() const;
    int month() const;
    int day() const;

    int hour() const;
    int minute() const;
    int secound() const;

    ProcessedDateTime dateTimeUTC() const;
    ProcessedDateTime dateTimeLocal() const;

    std::string toStringLocal() const;
    std::string toStringUTC() const;
    std::string toString(const ProcessedDateTime & pdt) const;

    std::string toString(const char *format = "%Y-%m-%d %H:%M:%S");
protected:
    ProcessedDateTime _dateTime(tm t) const;

protected:
    std::chrono::system_clock::time_point m_time;
};

typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24> >::type> chrone_days;

} // namespace Model
} // namespace Oak
