#include "QOakToolButton.h"

#include "QOakToolbarModel.h"
#include "QServiceFunctions.h"

#include "../ServiceFunctions/Trace.h"

#include <QIcon>

// =============================================================================
// (public)
QOakToolButton::QOakToolButton(QObject *parent)
    : QObject(parent)
{

}

// =============================================================================
// (public)
QOakToolButton::~QOakToolButton()
{

}

// =============================================================================
// (public)
int QOakToolButton::count() const
{
    return 1;
}

// =============================================================================
// (public)
QVariant QOakToolButton::data(int index, int role) const
{
    const Oak::Model::NodeDef *nodeDef = m_toolBarModel->node().def();

    Q_UNUSED(index)
    switch(role)
    {
    case QOakModel::Name:
        return QString::fromStdString(nodeDef->name());
    case QOakToolBarModel::ToolTip:
        if (!nodeDef->tooltip().empty()) {
            QString toolTip = QString::fromStdString(nodeDef->tooltip());
            return toolTip;
        }
        break;
    case QOakToolBarModel::Icon:
        if (nodeDef->hasImagePath()) {
            QString imagePath = QString::fromStdString(nodeDef->imagePath());
            return QIcon(imagePath);
        }
        break;
    case QOakToolBarModel::Color:
        if (nodeDef->hasColor()) {
            Oak::Model::Color color = nodeDef->color();
            return toQColor(color);
        }
        break;
    case QOakToolBarModel::Enabled:
        return true;
    }
    return QVariant();
}

// =============================================================================
// (public)
void QOakToolButton::sourceModelConnect()
{
}

// =============================================================================
// (public)
void QOakToolButton::sourceModelDisconnect()
{
}

// =============================================================================
// (public)
void QOakToolButton::setToolBarModel(const QOakToolBarModel *toolBarModel)
{
    m_toolBarModel = toolBarModel;
}

// =============================================================================
// (public)
void QOakToolButton::trigger(int index)
{
    Q_UNUSED(index)
    TRACE("Remember to implement the trigger function in your QOakToolButton\n");
}
