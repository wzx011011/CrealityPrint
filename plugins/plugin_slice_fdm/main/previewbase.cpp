#include "previewbase.h"
#include <QtCore/QDebug>

PreviewBase::PreviewBase(QObject* parent)
	: Job(parent)
	, m_attain(nullptr)
{
}

PreviewBase::~PreviewBase()
{

}

void PreviewBase::setFileName(const QString& fileName)
{
	m_fileName = fileName;
}

QString PreviewBase::name()
{
	return "Preview";
}

QString PreviewBase::description()
{
	return "";
}

void PreviewBase::failed()
{
	qDebug() << "preview failed.";
}

void PreviewBase::successed(qtuser_core::Progressor* progressor)
{
	qDebug() << "success";
	SliceAttain* attain = take();
	
	assert(attain);
	if(attain)
		emit sliceSuccess(attain);
}

SliceAttain* PreviewBase::take()
{
	SliceAttain* attain = m_attain;
	m_attain = nullptr;
	return attain;
}