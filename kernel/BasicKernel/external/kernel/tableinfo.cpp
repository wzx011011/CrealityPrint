#include "tableinfo.h"

TableInfo::TableInfo(QObject *parent)
{
    id = 0;
    checkstate = false;
    m_name = "Undefine";
    m_status = 0;
    m_ipAddr = "172.17.40.10";
    m_macAddr = "xxx-xxx-xxx-xxx";
}
bool TableInfo::checkStatus()
{

    return checkstate;
}
