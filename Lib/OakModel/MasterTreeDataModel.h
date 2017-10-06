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

#include "DesignTreeDataModel.h"

namespace Oak {
namespace Model {

// =============================================================================
// Class definition
// =============================================================================
class MasterTreeDataModel : public TreeDataModel
{
public:
    MasterTreeDataModel();

    DesignTreeDataModel* designModel() { return &m_designModel; }
    TreeDataModel* dataModel() { return &m_dataModel; }
    TreeDataModel* filterModel() { return &m_filterModel; }

    std::string designFilePath() const;
    bool loadDesignFilePath(const std::string& filePath);

    std::string dataFilePath() const;
    bool LoadDataFilePath(const std::string& filePath);

    std::string filterFilePath() const;
    bool loadFilterFilePath(const std::string& filePath);

    void generateDataNodeDefinition();

protected:
    void createMasterModel();

    void onRootItemChanged();

protected:
    DesignTreeDataModel m_designModel;
    TreeDataModel m_dataModel;
    TreeDataModel m_filterModel;

    static std::string s_master;
    static std::string s_data;
    static std::string s_filter;
    static std::string s_version;
    static std::string s_major;
    static std::string s_minor;
    static std::string s_patch;
    static std::string s_filePath;
};

} // namespace Model
} // namespace Oak
