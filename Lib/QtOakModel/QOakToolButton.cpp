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
    Q_UNUSED(index)
    Q_UNUSED(role)
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
