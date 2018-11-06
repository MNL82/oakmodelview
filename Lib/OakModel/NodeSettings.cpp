/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeSettings.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
NodeSettings::NodeSettings()
{

}

// =============================================================================
// (public)
NodeSettings& NodeSettings::operator=(const NodeSettings& copy)
{
    m_hideVariantInstance = copy.m_hideVariantInstance;
    return *this;
}

// =============================================================================
// (public)
bool NodeSettings::hideVariantInstance() const
{
    return m_hideVariantInstance;
}

// =============================================================================
// (public)
void NodeSettings::sethideVariantInstance(bool value)
{
    m_hideVariantInstance = value;
}

} // namespace Model
} // namespace Oak
