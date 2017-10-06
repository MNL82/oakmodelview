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

#include <list>
#include <string>
#include <vector>
#include <memory>
#include <limits>

namespace Oak {
namespace XML {

// =============================================================================
// Conversion service functions
// =============================================================================

std::vector<std::string> split(const std::string &str, char seperator = ';', bool ignoreEmpty = false);
std::vector<std::vector<std::string>> doubleSplit(const std::string &str, char outerSeperator = ';', char innerSeperator = ':', bool ignoreEmpty = false);

int toInteger(const std::string &str, bool *ok = 0);

template<typename TO, typename FROM>
std::unique_ptr<TO> dynamic_unique_pointer_cast(std::unique_ptr<FROM>&& old)
{
    TO* ptr = dynamic_cast<TO*>(old.get());
    if (ptr) {
        old.release();
        return std::unique_ptr<TO>(ptr);
    }
    assert(false);
    return std::unique_ptr<TO>(nullptr);
}

} // namespace XML
} // namespace Oak
