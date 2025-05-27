#ifndef CXHANDLERBASE_H
#define CXHANDLERBASE_H
#include "basickernelexport.h"

#include <QtCore/QString>

class BASIC_KERNEL_API CXHandleBase
{
public:
	CXHandleBase();
	virtual ~CXHandleBase();

public:
	virtual QStringList supportFilters();
	virtual QStringList enableFilters();
	virtual void setSupportOpenModel(QString openModel);
	virtual QString supportOpenModel();///支持的打开模式 FDM，LASER
	virtual void handle(const QString& fileName);
	virtual void handle(const QStringList& fileNames);

private:
	QStringList m_fileNames;
	QString m_fileName;

protected:
	QString m_open_model="fdm";
};

#endif // CXHANDLERBASE_H


