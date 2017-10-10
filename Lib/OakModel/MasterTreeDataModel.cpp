/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "MasterTreeDataModel.h"

#include "NodeDefinitionBuilder.h"
#include "ContainerDefinitionBuilder.h"
#include "ValueDefinitionBuilder.h"

#ifdef XML_BACKEND
#include "XMLRefFactory.h"
#endif // XML_BACKEND

namespace Oak {
namespace Model {

std::string MasterTreeDataModel::s_master    = "master";
std::string MasterTreeDataModel::s_data      = "data";
std::string MasterTreeDataModel::s_filter    = "filter";
std::string MasterTreeDataModel::s_version   = "version";
std::string MasterTreeDataModel::s_major     = "major";
std::string MasterTreeDataModel::s_minor     = "minor";
std::string MasterTreeDataModel::s_patch     = "patch";
std::string MasterTreeDataModel::s_filePath  = "filePath";

// =============================================================================
// (public)
MasterTreeDataModel::MasterTreeDataModel()
{
    createMasterModel();
    notifier_rootNodeChanged.add(this, &MasterTreeDataModel::onRootItemChanged);
    notifier_rootNodeDefinitionChanged.add(this, &MasterTreeDataModel::onRootItemChanged);
}

// =============================================================================
// (public)
std::string MasterTreeDataModel::designFilePath() const
{
    if (isNull()) { return std::string(); }
    Item designItem = rootItem().firstChild(m_designModel.rootPKey<std::string>());
    if (designItem.isNull()) { return std::string(); }
    std::string filePath;
    designItem.value(s_filePath).getValue(filePath);
    return filePath;
}

// =============================================================================
// (public)
bool MasterTreeDataModel::loadDesignFilePath(const std::string& filePath)
{
    if (isNull()) { return false; }
    Item designItem = rootItem().firstChild(m_designModel.rootPKey<std::string>());
    if (designItem.isNull()) { return false; }
    if (designItem.value(s_filePath).setValue(filePath)) {
        return m_designModel.loadXMLRootNode(filePath);
    }
    return false;
}

// =============================================================================
// (public)
std::string MasterTreeDataModel::dataFilePath() const
{
    if (isNull()) { return std::string(); }
    Item dataItem = rootItem().firstChild(s_data);
    if (dataItem.isNull()) { return std::string(); }
    std::string filePath;
    dataItem.value(s_filePath).getValue(filePath);
    return filePath;
}

// =============================================================================
// (public)
bool MasterTreeDataModel::LoadDataFilePath(const std::string& filePath)
{
    if (isNull()) { return false; }
    Item dataItem = rootItem().firstChild(s_data);
    if (dataItem.isNull()) { return false; }
    if (dataItem.value(s_filePath).setValue(filePath)) {
        return m_dataModel.loadXMLRootNode(filePath);
    }
    return false;
}

// =============================================================================
// (public)
std::string MasterTreeDataModel::filterFilePath() const
{
    if (isNull()) { return std::string(); }
    Item filterItem = rootItem().firstChild(s_filter);
    if (filterItem.isNull()) { return std::string(); }
    std::string filePath;
    filterItem.value(s_filePath).getValue(filePath);
    return filePath;
}

// =============================================================================
// (public)
bool MasterTreeDataModel::loadFilterFilePath(const std::string& filePath)
{
    if (isNull()) { return false; }
    Item filterItem = rootItem().firstChild(s_filter);
    if (filterItem.isNull()) { return false; }
    if (filterItem.value(s_filePath).setValue(filePath)) {
        return m_filterModel.loadXMLRootNode(filePath);
    }
    return false;
}

// =============================================================================
// (public)
void MasterTreeDataModel::generateDataNodeDefinition()
{
    NodeDefinitionSPtr nodeI = m_designModel.generateModelDefinition();
    if (nodeI) {
        m_dataModel.setRootNodeDefinition(nodeI);
    }
}

// =============================================================================
// (protected)
void MasterTreeDataModel::createMasterModel()
{
    auto nodeMaster = NDB::Make(s_master);
    auto nodeDesign = NDB::Make(m_designModel.rootPKey<std::string>());
    auto nodeData = NDB::Make(s_data);
    auto nodeFilter = NDB::Make(s_filter);

    // Setup node "master"
    auto valueI = VDB::Make(0, s_major, 0);
    VDB::setValueRef(valueI, XML::RefFactory::MakeValueRef(s_version+";"+s_major));
    NDB::addValueDef(nodeMaster, std::move(valueI));

    valueI = VDB::Make(0, s_minor, 0);
    VDB::setValueRef(valueI, XML::RefFactory::MakeValueRef(s_version+";"+s_minor));
    NDB::addValueDef(nodeMaster, std::move(valueI));

    valueI = VDB::Make(0, s_patch, 0);
    VDB::setValueRef(valueI, XML::RefFactory::MakeValueRef(s_version+";"+s_patch));
    NDB::addValueDef(nodeMaster, std::move(valueI));

    NDB::addContainerDef(nodeMaster, CDB::Make(nodeDesign, 1, 1));
    NDB::addContainerDef(nodeMaster, CDB::Make(nodeData, 1, 1));
    NDB::addContainerDef(nodeMaster, CDB::Make(nodeFilter, 1, 1));

    // Setup node "design"
    NDB::addValueDef(nodeDesign, VDB::MakeXML(std::string(), std::string(), s_filePath, std::string()));

    // Setup node "data"
    NDB::addValueDef(nodeData, VDB::MakeXML(std::string(), std::string(), s_filePath, std::string()));

    // Setup node "filter"
    NDB::addValueDef(nodeFilter, VDB::MakeXML(std::string(), std::string(), s_filePath, std::string()));

    setRootNodeDefinition(nodeMaster);
}

// =============================================================================
// (protected)
void MasterTreeDataModel::onRootItemChanged()
{
    Item item = rootItem();
    if (item.isNull()) { return; }

    std::string filePath;

    // Design model
    Item designItem = item.childAt(m_designModel.rootPKey<std::string>(), 0);
    if (!designItem.isNull()) {
        designItem.value(s_filePath).getValue(filePath);
        if (filePath.empty()) {
            m_designModel.setRootNode(designItem.node());
        } else {
            m_designModel.loadXMLRootNode(filePath);
        }
        generateDataNodeDefinition();
    }

    // Data model
    Item dataItem = item.childAt(s_data, 0);
    if (dataItem.isNull()) { return; }
    if (!dataItem.isNull()) {
        dataItem.value(s_filePath).getValue(filePath);
        if (filePath.empty()) {
            if (!m_dataModel.isDefinitionNull()) {
                const std::string& dataRootKey = m_dataModel.rootPKey<std::string>();
                if (dataItem.node().isXML()) {
                    XML::Element dataElement = dataItem.node().xmlNode();
                    XML::Element rootElement = dataElement.firstChild(dataRootKey);
                    if (rootElement.isNull()) {
                        rootElement = dataElement.appendChild(dataRootKey);
                    }
                    m_dataModel.setRootNode(Node(rootElement));
                } else {
                    // m_type contains an unhandled type that needs to be implemented
                    assert(false);
                }
            }
        } else {
            m_dataModel.loadXMLRootNode(filePath);
        }
    }

    // Filter model
    Item filterItem = item.childAt(s_filter, 0);
    if (filterItem.isNull()) { return; }
    if (!filterItem.isNull()) {
        filterItem.value(s_filePath).getValue(filePath);
        if (filePath.empty()) {
            m_filterModel.setRootNode(filterItem.node());
        } else {
            m_filterModel.loadXMLRootNode(filePath);
        }
    }
}

} // namespace Model
} // namespace Oak
