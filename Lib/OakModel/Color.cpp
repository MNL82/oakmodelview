/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "Color.h"


namespace Oak::Model {

// =============================================================================
// (public)
Color::Color()
{
    m_red = 0;
    m_green = 0;
    m_blue = 0;
    m_alpha = -1;
}

// =============================================================================
// (public)
Color::Color(int red, int green, int blue, int alpha)
{
    m_red = red;
    m_green = green;
    m_blue = blue;
    m_alpha = alpha;
}

// =============================================================================
// (public)
Color::Color(const Color &copy)
{
    *this = copy;
}

// =============================================================================
// (public)
Color &Color::operator=(const Color &copy)
{
    m_red = copy.m_red;
    m_green = copy.m_green;
    m_blue = copy.m_blue;
    m_alpha = copy.m_alpha;

    return *this;
}

// =============================================================================
// (public)
bool Color::isValid() const
{
    return m_alpha >= 0;
}

// =============================================================================
// (public)
int Color::red() const
{
    return m_red;
}

// =============================================================================
// (public)
int Color::green() const
{
    return m_green;
}

// =============================================================================
// (public)
int Color::blue() const
{
    return m_blue;
}

// =============================================================================
// (public)
int Color::alpha() const
{
    return m_alpha;
}

// =============================================================================
// (public)
bool Color::setRed(int value)
{
    if (value == m_red || value < 0 || value > 255) { return false; }
    m_red = value;
    return true;
}

// =============================================================================
// (public)
bool Color::setGreen(int value)
{
    if (value == m_green || value < 0 || value > 255) { return false; }
    m_green = value;
    return true;
}

// =============================================================================
// (public)
bool Color::setBlue(int value)
{
    if (value == m_blue || value < 0 || value > 255) { return false; }
    m_blue = value;
    return true;
}

// =============================================================================
// (public)
bool Color::setAlpha(int value)
{
    if (value == m_alpha || value < -1 || value > 255) { return false; }
    m_alpha = value;
    return true;
}

// =============================================================================
// (public)
bool Color::set(int red, int green, int blue, int alpha)
{
    return setRed(red) &&
           setGreen(green) &&
           setBlue(blue) &&
           setAlpha(alpha);
}

} // namespace Oak::Model

