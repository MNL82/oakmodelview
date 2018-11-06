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
class NodeSettings
{
public:
    NodeSettings();

    NodeSettings& operator=(const NodeSettings& copy);

    bool hideVariantInstance() const;
    void sethideVariantInstance(bool value);

private:
    bool m_hideVariantInstance = false;
};

} // namespace Model
} // namespace Oak
