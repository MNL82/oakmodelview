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

namespace Oak {
namespace Model {

// =============================================================================
// Class definition
// =============================================================================
class Color
{
public:
    Color();
    Color(int red, int green, int blue, int alpha = 255);
    Color(const Color& copy);

    Color& operator=(const Color &copy);

    bool isValid() const;

    int red() const;
    int green() const;
    int blue() const;
    int alpha() const;

    bool setRed(int value);
    bool setGreen(int value);
    bool setBlue(int value);
    bool setAlpha(int value);

    bool set(int red, int green, int blue, int alpha = 255);

protected:
    int m_red;
    int m_green;
    int m_blue;
    int m_alpha;
};

} // namespace Model
} // namespace Oak
