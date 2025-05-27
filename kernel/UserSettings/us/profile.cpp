#include "profile.h"

Profile::Profile()
{
	m_layerheight = "";
}

void Profile::setQualityName(const QString& qName)
{
    if (m_QualityName == qName)
        return;
    m_QualityName = qName;
}

//QString Profile::getQualityName() {
//    if (m_QualityName != QString())
//        return m_QualityName;
//
//    if (m_FullName.contains("high"))
//    {
//        return "high";
//    }
//    else if (m_FullName.contains("middle"))
//    {
//        return "middle";
//    }
//    else if (m_FullName.contains("low"))
//    {
//        return "low";
//    }
//    else if (m_FullName.contains("fast"))
//    {
//        return "fast";
//    }
//    else
//    {
//        return "middle";
//    }
//}

void Profile::setMaterialName(const QString& material)
{
    if (m_MaterialName == material)
        return;
    m_MaterialName = material;
}

QString Profile::getMaterialName() const
{
    if (m_MaterialName != QString())
        return m_MaterialName;

    if (!m_name.contains("-"))
        return "ALL";
    else
    {
        auto list = m_name.split("_");
        return list.at(0);
    }
}

QString Profile::translateName(QString name) {
    QString strTranslate;
    if (name == "high")
    {
        strTranslate = QObject::tr("high");
    }
    else if (name == "middle")
    {
        strTranslate = QObject::tr("middle");
    }
    else if (name == "low")
    {
        strTranslate = QObject::tr("low");
    }
    else
    {
        strTranslate = name;
    }
    return strTranslate;
}

QString Profile::machine()
{
	return m_machineName;
}

void Profile::setMachine(const QString& machine)
{
	if (m_machineName == machine)
		return;
	m_machineName = machine;
}

void Profile::setFullName(const QString& fullName)
{
	if (m_FullName == fullName)
		return;
	m_FullName = fullName;
}

bool Profile::isDefaultProfile()
{
	return m_IsDefaultFile;
}

void Profile::setIsDefaultProfile(bool isDefault)
{
	if (m_IsDefaultFile == isDefault)
		return;
	m_IsDefaultFile = isDefault;
}
