#include "basicsliceui.h"
#include <QDebug>

BasicSliceUI::BasicSliceUI(QObject* parent)
    :QObject(parent)
{
   //
}

BasicSliceUI::~BasicSliceUI()
{
}
/*
void BasicSliceUI::preview()
{
    qInfo() << "no operation";
}

void BasicSliceUI::unpreview()
{
    qInfo() << "no operation";
}
 */

void BasicSliceUI::layerImgChange(const QImage& img)
{
    qInfo() << "no operation";
}

void BasicSliceUI::setObject(QString objname, QObject* object)
{
    m_setObjects[objname] = object;
}

void BasicSliceUI::setParam(QString objname, void* param)
{
    qInfo() << "no operation";
}

int BasicSliceUI::initialize()
{
    qInfo() << "no operation";
    return -1;
}

int BasicSliceUI::uninitialize()
{
    qInfo() << "no operation";
    return -1;
}

int BasicSliceUI::onSliceSuccess(QObject* object, void* param)
{
    qInfo() << "no operation";
    return -1;
}

int BasicSliceUI::onSliceFailed(QObject* object, void* param)
{
    qInfo() << "no operation";
    return -1;
}

int BasicSliceUI::handle(QObject* object, void* param)
{
    qInfo() << "no operation";
    return -1;
}

int BasicSliceUI::onExporSuccess(QObject* object, void* param)
{
    qInfo() << "no operation";
    return -1;
}

void BasicSliceUI::show(bool show)
{
    //
}

void BasicSliceUI::update()
{
    //
}



QObject* BasicSliceUI::findSetObject(QString objname)
{
    QMap<QString, QObject*>::iterator it = m_setObjects.find(objname);
    if (it == m_setObjects.end())
    {
        return nullptr;
    }
    return it.value();
}
