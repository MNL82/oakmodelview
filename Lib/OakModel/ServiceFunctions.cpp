/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ServiceFunctions.h"

#include <cctype>
#include <algorithm>
#include <fstream>
#include <sys/stat.h>
#include "../ServiceFunctions/Assert.h"
#include <utility>

#define BUFFER_SIZE 1000000

namespace Oak {
namespace Model {

// =============================================================================
// (public)
std::vector<std::string> split(const std::string &str, char seperator, bool ignoreEmpty)
{
    std::vector<std::string> sList;
    if (str.empty()) { return sList; }

    size_t first = 0, current = 0;
    do {
        if (current == str.size() || str.at(current) == seperator) {
            if (!ignoreEmpty || current-first > 0) {
                sList.push_back(str.substr(first, current-first));
            }
            first = current+1;
        }
        current++;
    } while (current <= str.size());

    return sList;
}

// =============================================================================
// (public)
std::vector<std::vector<std::string>> doubleSplit(const std::string &str, char outerSeperator, char innerSeperator, bool ignoreEmpty)
{
    std::vector<std::vector<std::string>> sMatrix;

    std::vector<std::string> outerList = split(str, outerSeperator, ignoreEmpty);
    for (const std::string& is: outerList) {
        sMatrix.push_back(split(is, innerSeperator, ignoreEmpty));
    }
    return sMatrix;
}

// =============================================================================
// (public)
std::vector<std::string> splitString(const std::string &str, char seperator, bool skipEmptyParts)
{
    std::vector<std::string> strings;
    size_t first = 0, current = 0;
    do {
        if (current == str.size() || str.at(current) == seperator) {
            if (!skipEmptyParts || current-first > 0) {
                strings.push_back(str.substr(first, current-first));
            }
            first = current+1;
        }
        current++;
    } while (current <= str.size());
    return strings;
}

// =============================================================================
// (public)
std::string joinString(const std::vector<std::string> strList, char seperator)
{
    std::string str;
    std::vector<std::string>::const_iterator it = strList.begin();
    while (it != strList.end()) {
        str.append(*it);
        it++;
        if (it != strList.end()) {
            str.push_back(seperator);
        }
    }
    return str;
}

// =============================================================================
// (public)
std::string simplified(const std::string &str)
{
    std::string resultStr;
    if (str.empty()) { return resultStr; }

    std::vector<char> spacers = {' ', '\t', '\n', '\v', '\f', '\r'};
    std::vector<char>::iterator charIt;

    std::string::const_iterator first = str.begin();
    std::string::const_iterator last = str.end();
    std::string::iterator result;

    // Ignore all trailing widespaces
    last--; // Points to the last element
    charIt = spacers.begin();
    while (charIt != spacers.end() && first != last) {
        if (*last == *charIt) {
            last--; // The last element is ignored
            charIt = spacers.begin();
        } else {
            charIt++;
        }
    }
    last++; // Points to the next to last element again

    bool isSpacer = false;
    while (first != last) {
        charIt = spacers.begin();
        if (resultStr.empty()) {
            // Remove all preceding spaces
            isSpacer = false;
            // Detect if the next char is a spacer char
            while (!isSpacer && charIt != spacers.end()) {
                if (*charIt == *first) {
                    isSpacer = true;
                }
                charIt++;
            }
            if (!isSpacer) {
                // Add the first none spacer char
                resultStr.push_back(*first);
                result = resultStr.begin();
            }
        } else {
            if (isSpacer && *result == *first) {
                // Skip the spacer duplicate
            } else {
                resultStr.push_back(*first);
                // Detect if the char is a spacer char
                while (!isSpacer && charIt != spacers.end()) {
                    if (*charIt == resultStr.front()) {
                        isSpacer = true;
                    }
                    charIt++;
                }
            }
        }
        first++;
    }

    return resultStr;
}

// =============================================================================
// (public)
std::string fromDouble(double value, char f, int prec)
{
    UNUSED(value);
    UNUSED(f);
    UNUSED(prec);
//    switch (f) {
//        case 'f':
//        case 'e':
//        case 'g':
//            break;
//        default:
//            ASSERT(true);
//            break;
//    }
    return std::string();
}

// =============================================================================
// (public)
bool compare(std::string s1, std::string s2, bool caseSensitive)
{
    if (caseSensitive) {
        return s1 == s2;
    } else {
        // TODO: Make a case insensitive comparison
        return s1 == s2;
    }
}

// =============================================================================
// (public)
bool contains(const std::vector<std::string> &strList, const std::string &str)
{
    return std::find(strList.begin(), strList.end(), str) != strList.end();
}

// =============================================================================
// (public)
int count(const std::vector<std::string> &strList, const std::string &str)
{
    auto it = std::find(strList.begin(), strList.end(), str);
    int c = 0;
    while (it != strList.end())
    {
        c++;
        it = std::find(++it, strList.end(), str);
    }
    return c;
}

// =============================================================================
// (public)
int toInteger(const std::string &str, bool *ok)
{
    try {
        if (ok) { *ok = true; }
        return std::stoi(str);
    } catch(...) {
        if (ok) { *ok = false; }
        return 0;
    }
}

// =============================================================================
// (public)
std::vector<int> toIntegerList(const std::vector<std::string> &values, bool *ok)
{
    std::vector<int> valueList;
    bool test;
    for (const std::string &value: values) {
        valueList.push_back(toInteger(value, &test));
        if (!test) {
            if (ok) { *ok = false; }
            return std::vector<int>();
        }
    }
    return valueList;
}

// =============================================================================
// (public)
std::vector<int> toIntegerList(const std::string &value, char seperator, bool *ok)
{
    std::vector<std::string> valueList = splitString(value, seperator);
    std::vector<int> intValueList;
    bool test;
    for (const std::string &str: valueList)
    {
        intValueList.push_back(toInteger(str, &test));
        if (!test) {
            if (ok) { *ok = false; }
            return std::vector<int>();
        }
    }
    if (ok) { *ok = true; }
    return intValueList;
}

// =============================================================================
// (public)
std::vector<std::vector<int>> toIntegerListOfList(const std::vector<std::string> &values, char seperator, bool *ok)
{
    std::vector<std::vector<int>> valueList;
    bool test;
    for (const std::string &value: values) {
        valueList.push_back(toIntegerList(value, seperator, &test));
        if (!test) {
            if (ok) { *ok = false; }
            return std::vector<std::vector<int>>();
        }
    }
    if (ok) { *ok = true; }
    return valueList;
}

// =============================================================================
// (public)
std::string toString(int value)
{
    return std::to_string(value);
}

// =============================================================================
// (public)
std::string toString(const std::vector<int> &values, char seperator)
{
    std::string str;
    auto it = values.begin();
    while (it != values.end()) {
        str.append(toString(*it));
        it++;
        if (it != values.end()) {
            str.push_back(seperator);
        }
    }
    return str;
}

// =============================================================================
// (public)
std::string toString(double value)
{
    return std::to_string(value);
}

// =============================================================================
// (public)
std::string toString(bool value, const std::vector<std::string> &itemList)
{
    return value ? itemList.back() : itemList.front();
}

// =============================================================================
// (public)
std::string toString(BoolState value, const std::vector<std::string> &itemList)
{
    if (value == BoolState_True) {
        return itemList.back();
    } else if (value == BoolState_False) {
        return itemList.front();
    } else {
        return std::string();
    }
}

// =============================================================================
// (public)
std::vector<std::string> toStringList(const std::vector<int> &values)
{
    std::vector<std::string> valueList;
    for (int value: values) {
        valueList.push_back(toString(value));
    }
    return valueList;
}

// =============================================================================
// (public)
std::vector<std::string> toStringList(const std::vector<std::vector<int>> &values, char seperator)
{
    std::vector<std::string> valueList;
    for (const std::vector<int> &value: values) {
        valueList.push_back(toString(value, seperator));
    }
    return valueList;
}

// =============================================================================
// (public)
std::vector<std::string> toStringList(const std::vector<double> &values)
{
    std::vector<std::string> valueList;
    for (double value: values) {
        valueList.push_back(toString(value));
    }
    return valueList;
}

// =============================================================================
// (public)
std::vector<std::string> toStringList(const std::vector<bool> &values, const std::vector<std::string> &itemList)
{
    std::vector<std::string> valueList;
    for (bool value: values) {
        valueList.push_back(toString(value, itemList));
    }
    return valueList;
}

// =============================================================================
// (public)
std::vector<std::string> toStringList(const std::vector<BoolState> &values, const std::vector<std::string> &itemList)
{
    std::vector<std::string> valueList;
    for (BoolState value: values) {
        valueList.push_back(toString(value, itemList));
    }
    return valueList;
}

// =============================================================================
// (public)
float toFloat(const std::string &str, bool *ok)
{
    try {
        if (ok) { *ok = true; }
        return std::stof(str);
    } catch(...) {
        if (ok) { *ok = false; }
        return 0.0;
    }
}

// =============================================================================
// (public)
double toDouble(const std::string &str, bool *ok)
{
    try {
        if (ok) { *ok = true; }
        return std::stod(str);
    } catch(...) {
        if (ok) { *ok = false; }
        return 0.0;
    }
}

// =============================================================================
// (public)
std::vector<double> toDoubleList(const std::vector<std::string> &values)
{
    std::vector<double> valueList;
    for (const std::string &value: values) {
        valueList.push_back(toDouble(value));
    }
    return valueList;
}

// =============================================================================
// (public)
bool toBool(BoolState value)
{
    // The value can not be converted to a bool if undefined
    ASSERT(value != BoolState_Undefined);
    return value == BoolState_True;
}

// =============================================================================
// (public)
bool toBool(const std::string &value, const std::vector<std::string> &itemList)
{
    return toBool(toBoolState(value, itemList));
}

// =============================================================================
// (public)
BoolState toBoolState(bool value)
{
    return value ? BoolState_True : BoolState_False;
}

// =============================================================================
// (public)
BoolState toBoolState(const std::string &value, const std::vector<std::string> &itemList)
{
    if (value == itemList.back()) {
        return BoolState_True;
    } else if (value == itemList.front()) {
        return BoolState_False;
    } else {
        return BoolState_Undefined;
    }
}

// =============================================================================
// (public)
std::vector<bool> toBoolList(const std::vector<BoolState> &values)
{
    std::vector<bool> boolList;
    for (const BoolState &value: values) {
        boolList.push_back(toBool(value));
    }
    return boolList;
}

// =============================================================================
// (public)
std::vector<bool> toBoolList(const std::vector<std::string> &values, const std::vector<std::string> &itemList)
{
    std::vector<bool> boolList;
    for (const std::string &value: values) {
        boolList.push_back(toBool(value, itemList));
    }
    return boolList;
}

// =============================================================================
// (public)
std::vector<BoolState> toBoolStateList(const std::vector<bool> &values)
{
    std::vector<BoolState> boolList;
    for (bool value: values) {
        boolList.push_back(toBoolState(value));
    }
    return boolList;
}

// =============================================================================
// (public)
std::vector<BoolState> toBoolStateList(const std::vector<std::string> &values, const std::vector<std::string> &itemList)
{
    std::vector<BoolState> boolList;
    for (const std::string &value: values) {
        boolList.push_back(toBoolState(value, itemList));
    }
    return boolList;
}

// =============================================================================
// (public)
int indexOf(const std::string &str, const char &value, int firstIndex)
{
    if (static_cast<int>(str.size()) < firstIndex) {
        return -1;
    }
    std::string::const_iterator it = str.begin();
    int index = 0;
    while (it != str.end()) {
        if (*it == value) {
            return index;
        }
        index++;
        it++;
    }
    return -1;
}

// =============================================================================
// (public)
void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// =============================================================================
// (public)
void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// =============================================================================
// (public)
void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// =============================================================================
// (public)
std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// =============================================================================
// (public)
std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// =============================================================================
// (public)
std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

} // namespace Model
} // namespace Oak
