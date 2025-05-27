#include "cxhandlerbase.h"

CXHandleBase::CXHandleBase()
{

}

CXHandleBase::~CXHandleBase()
{

}

QStringList CXHandleBase::supportFilters()
{
	QStringList filters;
	return filters;
}

void CXHandleBase::setSupportOpenModel(QString openModel)
{
	m_open_model = openModel;
}
QString CXHandleBase::supportOpenModel()
{
	return m_open_model;
}

QStringList CXHandleBase::enableFilters()
{
	return supportFilters();
}

void CXHandleBase::handle(const QString& fileName)
{
	m_fileName = fileName;
};

void CXHandleBase::handle(const QStringList& fileNames)
{
	for (QString fileName : fileNames)
	{
		m_fileNames << fileName;
	}
};