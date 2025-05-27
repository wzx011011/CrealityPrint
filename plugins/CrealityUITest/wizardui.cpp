#include "wizardui.h"
#include <QQuickItem>
#include <QGuiApplication>

WizardUI::WizardUI(QObject *parent) : QObject(parent) { }

WizardUI::WizardUI(QObject *rootObject, QObject *parent)
    : QObject(parent), pRootObject(rootObject)
{
}

WizardUI::~WizardUI() { }
void WizardUI::setRootObjet(QObject *rootObj)
{
    pRootObject = rootObj;
}
void WizardUI::findRootByNode(QObject *nodeObject)
{
    pRootObject = nodeObject;
    if (pRootObject) {
        QObject *pParent = nullptr;
        while ((pParent = pRootObject->parent())) {
            pRootObject = pParent;
        }
    }
}
QRect WizardUI::getItemGeometryToScene(const QString &targetObjName) const
{
    if (!pRootObject) {
        return {};
    }
    auto pItem = pRootObject->findChild<QQuickItem *>(targetObjName);
    if (pItem) {
        if (pItem->parentItem()) {
            auto pos = pItem->parentItem()->mapToScene(pItem->position());
            return QRectF { pos.x(), pos.y(), pItem->width(), pItem->height() }.toRect();
        } else {
            return pItem->boundingRect().toRect();
        }
    }
    return {};
}

void WizardUI::setAppOverrideCursor(QCursor cursor)
{
    qApp->setOverrideCursor(cursor);
}

void WizardUI::restoreAppOverrideCursor()
{
    qApp->restoreOverrideCursor();
}

QPoint WizardUI::cursorGlobalPos() const
{
    return QCursor::pos();
}

QObject *WizardUI::getObject(const QString &targetObjName) const
{
    if (!pRootObject) {
        return nullptr;
    }
    return pRootObject->findChild<QObject *>(targetObjName);
}

QVariant WizardUI::getObjectProperty(QObject *targetObj, const QString &propertyName) const
{
    return targetObj->property(propertyName.toUtf8().constData());
}

void WizardUI::setObjectProperty(QObject *targetObj, const QString &propertyName,
                                  const QVariant &value) const
{
    targetObj->setProperty(propertyName.toUtf8().constData(), value);
}

QVariant WizardUI::getObjectProperty(const QString &targetObjName,
                                      const QString &propertyName) const
{
    auto pObj = getObject(targetObjName);
    if (pObj) {
        return getObjectProperty(pObj, propertyName);
    }
    return {};
}

void WizardUI::setObjectProperty(const QString &targetObjName, const QString &propertyName,
                                  const QVariant &value) const
{
    auto pObj = getObject(targetObjName);
    if (pObj) {
        setObjectProperty(pObj, propertyName, value);
    }
}
