#include "QOakNodeProxyModel.h"

#include "../ServiceFunctions/Trace.h"

#include <QIntValidator>
#include <QDoubleValidator>

// =============================================================================
// (public)
QOakNodeProxyModel::QOakNodeProxyModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

// =============================================================================
// (public)
QOakNodeProxyModel::~QOakNodeProxyModel()
{
    setSourceModelItem(QModelIndex());
}

// =============================================================================
// (public)
bool QOakNodeProxyModel::isNull() const
{
    return !m_sourceModelItem.isValid();
}

// =============================================================================
// (public)
QModelIndex QOakNodeProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (isNull()) { return QModelIndex(); }
    if (parent.isValid()) { return QModelIndex(); }

    return createIndex(row, column, m_sourceModelItem.internalPointer());
}

// =============================================================================
// (public)
QModelIndex QOakNodeProxyModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

// =============================================================================
// (public)
QModelIndex QOakNodeProxyModel::sibling(int row, int column, const QModelIndex& _index) const
{
    if (isNull()) { return QModelIndex(); }
    if (_index.model() != this) { return QModelIndex(); }
    return index(row, column);
}

// =============================================================================
// (public)
int QOakNodeProxyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (isNull()) { return 0; }
    return m_node.leafCount();
}

// =============================================================================
// (public)
int QOakNodeProxyModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (isNull()) { return 0; }

    return 1;
}

// =============================================================================
// (public)
bool QOakNodeProxyModel::hasChildren(const QModelIndex& parent) const
{
    if (isNull()) { return false; }
    if (parent.isValid()) { return false; }
    return m_node.leafCount() > 0;
}

// =============================================================================
// (public)
QVariant QOakNodeProxyModel::data(const QModelIndex &index, int role) const
{
    if (isNull()) { return QVariant(); }

    if (!index.isValid()) { return QVariant(); }

    const Oak::Model::Leaf &leaf = toLeaf(index);
    if (leaf.isNull()) { return QVariant(); }

    switch (role) {
    case Qt::DisplayRole: { // QML: display

        return QString::fromStdString(leaf.value<std::string>());
    }
    case Qt::DecorationRole: { // QML: decoration
        return QVariant();
    }
    case Qt::EditRole: { // QML: edit
        return toQVariant(leaf.value());
    }
    case Qt::ToolTipRole: { // QML: toolTip
        return QString::fromStdString(leaf.tooltip());
    }
    case Qt::StatusTipRole: { // QML: statusTip
        return QVariant();
    }
    case Qt::WhatsThisRole: { // QML: whatsThis
        return QVariant();
    }
    case QOakModel::Name:
        return QString::fromStdString(leaf.def()->name());
    case QOakNodeProxyModel::DisplayName:
        return QString::fromStdString(leaf.def()->displayName());
    case QOakNodeProxyModel::LeafValidator: {
        QValidator *v = nullptr;
        if (leaf.def()->valueType() == Oak::Model::UnionType::Integer) {
            v = new QIntValidator();
        } else if (leaf.def()->valueType() == Oak::Model::UnionType::Double) {
            v =new QDoubleValidator();
        }
        return QVariant::fromValue(v);
    }
    case QOakNodeProxyModel::HasOptions:
        return leaf.def()->options().isUsed();
    case QOakNodeProxyModel::OptionsOnly:
        return leaf.def()->settings().value(OPTION_ONLY).value<bool>();
    case QOakNodeProxyModel::Options: {
        if (!leaf.def()->options().isUsed()) { return QVariant(); }

//        std::vector<std::string> optionList2;
//        if (!leaf.getOptions(optionList2)) { return QVariant(); }
//        QStringList qOptionList2 = toQStringList(optionList2);

        Oak::Model::UnionValueList optionList;
        if (!leaf.getOptions(optionList)) { return QVariant(); }

        QVariantList qOptionList = toQVariantList(optionList);
        return qOptionList;
    }
    case QOakNodeProxyModel::Unit:
        return QString::fromStdString(leaf.def()->settings().value(UNIT).value<std::string>());
    case QOakNodeProxyModel::Required:
        return leaf.def()->settings().value(REQUIRED).value<int>() == 1;
    default:
        break;
    }


    return QVariant();
}

// =============================================================================
// (public)
bool QOakNodeProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (isNull()) { return false; }
    if (role != Qt::EditRole) { return false; }
    if (!index.isValid()) { return false; }

    const Oak::Model::Leaf &leaf = toLeaf(index);
    if (leaf.isNull()) { return false; }

    if (leaf.setValue(toUnionValue(value))) {
        // The underlying model will emit a changed signal
        return true;
    }
//    if (data(index, role) != value) {
//        // FIXME: Implement me!
//        emit dataChanged(index, index, QVector<int>() << role);
//        return true;
//    }
    return false;
}

// =============================================================================
// (public)
Qt::ItemFlags QOakNodeProxyModel::flags(const QModelIndex &index) const
{
    if (isNull()) { return Qt::NoItemFlags; }

    if (!index.isValid()) { return Qt::NoItemFlags; }

    Oak::Model::Leaf leaf = toLeaf(index);
    ASSERT(!leaf.isNull());

    Qt::ItemFlags flagList;

//    // Currently no nodes can be selected
//    flagList |= Qt::ItemIsSelectable;

    // Leafs can be edited from this model
    flagList |= Qt::ItemIsEditable;

//    // Drag and drop will not be used for leaf items
//    flagList |= Qt::ItemIsDragEnabled;
//    flagList |= Qt::ItemIsDropEnabled;

//    // No leaf can be checked
//    flagList |= Qt::ItemIsUserCheckable;

    // Currently all leafs are enabled
    flagList |= Qt::ItemIsEnabled;

    // No leaf can have children
    flagList |= Qt::ItemNeverHasChildren;

    return flagList;
}

// =============================================================================
// (public)
QHash<int, QByteArray> QOakNodeProxyModel::roleNames() const
{
    QHash<int, QByteArray> result = QAbstractItemModel::roleNames();
    result.insert(QOakNodeProxyModel::DisplayName, QByteArrayLiteral("displayName"));
    result.insert(QOakNodeProxyModel::LeafValidator, QByteArrayLiteral("leafValidator"));
    result.insert(QOakModel::Name, QByteArrayLiteral("name"));
    result.insert(QOakNodeProxyModel::HasOptions, QByteArrayLiteral("hasOptions"));
    result.insert(QOakNodeProxyModel::Options, QByteArrayLiteral("options"));
    result.insert(QOakNodeProxyModel::OptionsOnly, QByteArrayLiteral("optionsOnly"));
    result.insert(QOakNodeProxyModel::Unit, QByteArrayLiteral("unit"));
    result.insert(QOakNodeProxyModel::Required, QByteArrayLiteral("required"));
    return result;
}

// =============================================================================
// (public)
QModelIndex QOakNodeProxyModel::sourceModelItem() const
{
    return m_sourceModelItem;
}

// =============================================================================
// (public)
const Oak::Model::Leaf &QOakNodeProxyModel::toLeaf(const QModelIndex& index) const
{
    ASSERT(index.internalPointer() == m_node.nodeData().internalPtr());
    ASSERT(index.row() < m_node.leafCount());

    return m_node.leafAt(index.row());
}

// =============================================================================
// (public slots)
void QOakNodeProxyModel::setSourceModelItem(QModelIndex sourceModelItem)
{
    if (m_sourceModelItem == sourceModelItem) { return; }

    // 1. The model of the index have to be the QOakModel
    const QOakModel *newModel = qobject_cast<const QOakModel*>(sourceModelItem.model());

    // 2. Find the node the index refers to
    Oak::Model::Node newNode;
    if (newModel != nullptr) {
        newNode = newModel->toNode(sourceModelItem);
    }

    // 3. Calculate the NodeIndex the process events faster
    Oak::Model::NodeIndexUPtr newNodeIndex = Oak::Model::NodeIndex::create(newNode);

    if (newNode.isNull()) { // If one fail all fail
        sourceModelItem = QModelIndex();
        newModel = nullptr;
    }
    if (newNode.isNull() && m_node.isNull()) { return; }

    // The model source have changed and needs to be reset
    beginResetModel();

    // Only disconnect/connect notifiers if model changed
    bool modelChanged = newNode.model() != m_node.model();

    // Disconnect notifiers from previous connected model
    if (modelChanged && m_node.model() != nullptr) {
        m_node.model()->notifier_destroyed.remove(this);
        m_node.model()->notifier_leafChangeAfter.remove(this);
    }

    m_sourceModelItem = sourceModelItem;
    m_sourceModel = newModel;
    m_node = newNode;
    m_nodeIndexUPtr = std::move(newNodeIndex);

    // Connect notifiers to the new model
    if (modelChanged && m_node.model() != nullptr) {
        m_node.model()->notifier_destroyed.add(this, &QOakNodeProxyModel::clearModel);
        m_node.model()->notifier_leafChangeAfter.add(this, &QOakNodeProxyModel::onLeafValueChanged);
    }

    emit sourceModelItemChanged(m_sourceModelItem);
    resetInternalData();
    endResetModel();
}

// =============================================================================
// (protected)
void QOakNodeProxyModel::onLeafValueChanged(const Oak::Model::NodeIndex& nIndex, const std::string& str)
{
    // Do nothing if the change related to an other node
    if (!m_nodeIndexUPtr || !m_nodeIndexUPtr->equal(nIndex)) { return; }

    const Oak::Model::Leaf &leaf = m_node.leaf(str);
    if (leaf.isNull()) { return; }

    int leafIndex = m_node.leafIndex(leaf);
    if (leafIndex < 0) { return; }

    QModelIndex index = createIndex(leafIndex, 0, m_node.nodeData().internalPtr());

    emit dataChanged(index, index, QVector<int>() << Qt::EditRole << Qt::DisplayRole);
}

// =============================================================================
// (protected)
void QOakNodeProxyModel::clearModel()
{
    m_sourceModelItem = QModelIndex();
    m_sourceModel = nullptr;
    m_node = Oak::Model::Node();
    m_nodeIndexUPtr = Oak::Model::NodeIndexUPtr();
}
