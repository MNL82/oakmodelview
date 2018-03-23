/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "XMLServiceFunctions.h"

#include <cctype>
#include <algorithm>
#include <fstream>
#include <sys/stat.h>
#include <assert.h>
#include <utility>

#define BUFFER_SIZE 1000000

namespace Oak {
namespace XML {

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

} // namespace XML
} // namespace Oak
