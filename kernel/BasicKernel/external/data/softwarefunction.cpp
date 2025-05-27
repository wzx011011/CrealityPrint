#include "softwarefunction.h"

SoftwareFunction::SoftwareFunction(QObject* parent)
	:QObject(parent)
{
	m_modelAdaption = true;
}
SoftwareFunction::~SoftwareFunction()
{
}
Q_INVOKABLE void SoftwareFunction::getData(QString name, QString value)
{
	if (name == "self_Adaption")
	{
		if ("true" == value)
		{
			m_modelAdaption = true;
		}
		else
		{
			m_modelAdaption = false;
		}	 
	}
}

bool SoftwareFunction::modelAdaption()
{
	return m_modelAdaption;
}

