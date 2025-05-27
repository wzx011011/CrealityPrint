#include "rclickmenudata.h"
#include <QVariant>
#include "rclickmenulist.h"
#include <QVariantList>
#include <QMap>
#include <QString>
#include <QDebug>
#include "kernel/translator.h"
RClickMenuData::RClickMenuData(QObject *parent) : QObject(parent)
{
    if(m_varCommandList.isEmpty())
        m_varCommandList = RClickMenuList::getInstance()->getActionCommandList();
   // createMenuNameMap();
    //menu need to do change language opreator
//    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
//    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}

void RClickMenuData::slotLanguageChanged()
{
    m_mapMenuName.clear();
    //make the .ts file effect
//    createMenuNameMap();
}
RClickMenuData::~RClickMenuData()
{}

void RClickMenuData:: createMenuNameMap()
{
  //  int index = eMenuType_File;
    m_mapMenuName.clear();
//    m_mapMenuName.insert(QString::number(eMenuType_File),tr("File(&F)"));
//    m_mapMenuName.insert(QString::number(eMenuType_Edit),tr("Edit(&E)"));
//    m_mapMenuName.insert(QString::number(eMenuType_View),tr("View(&V)"));
//    m_mapMenuName.insert(QString::number(eMenuType_Repair),tr("Repair(&R)"));
//    m_mapMenuName.insert(QString::number(eMenuType_Slice),tr("Slice(&R)"));
//    m_mapMenuName.insert(QString::number(eMenuType_Tool),tr("Tool(&T)"));
//    m_mapMenuName.insert(QString::number(eMenuType_CrealityGroup),tr("CrealityGroup(&C)"));
//    m_mapMenuName.insert(QString::number(eMenuType_Help),tr("Help(&H)"));

}
void RClickMenuData::addCommads(ActionCommand *pAction)
{
    if (pAction)
    {
        m_varCommandList.push_back(pAction);
    }
}
QVariantList  RClickMenuData::getCommandsList(void)
{
    QVariantList varList;
    varList.clear();
    for (int i = 0; i < m_varCommandList.count(); ++i)
    {
        varList.push_back(QVariant::fromValue(m_varCommandList.at(i)));
    }
    return varList;
}

QString RClickMenuData::getMenuNameFromKey(QString key)
{
    //m_mapMenuName.value(key);
    return m_mapMenuName.value(key);
}
void RClickMenuData::initMapData(QVariantMap &map)
{
    QVariantList fileList;
    map.insert(QString::number(eMenuType_File),fileList);
}
QVariantMap RClickMenuData::getCommandsMap(void)
{
    QVariantMap dataMap;
    dataMap.clear();

    initMapData(dataMap);
    //get actionCommand Data
    //m_varCommandList = RClickMenuList::getInstance()->getActionCommandList();
    if(m_varCommandList.isEmpty())
    {
        return dataMap;
    }
    foreach (auto var, m_varCommandList)
    {
        QVariantList  list;
        int index = 0;
        if(dataMap.isEmpty())
        {
            return dataMap;
        }
        if(dataMap.find(QString::number(index))->isNull())
        {
            list.append(QVariant::fromValue(var));
        }
        else
        {
            list = dataMap.find(QString::number(index)).value().value<QVariantList>();
            list.append(QVariant::fromValue(var));
        }
        dataMap.insert(QString::number(index),list);
    }
    return dataMap;
}
