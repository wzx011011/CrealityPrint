#include "previewworker.h"
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include "interface/jobsinterface.h"
#include "main/sliceplugin.h"
#include "main/gcodefilepreview.h"
#include "us/slicermanager.h"
#include "kernel/printerentityuse.h"
#include "kernel/kernel.h"
#include "kernel/reuseablecache.h"

using namespace creative_kernel;
PreviewWorker::PreviewWorker(QObject* parent)
	: QObject(parent)
	, m_receiver(nullptr)
{
	m_receiver = qobject_cast<SlicePlugin*>(parent);
}

PreviewWorker::~PreviewWorker()
{

}

QStringList PreviewWorker::supportFilters()
{
	QStringList list;
	list << "gcode";
	return list;
}

QStringList PreviewWorker::enableFilters()
{
	QStringList filters;
	filters << "gcode";
	return filters;
}

void PreviewWorker::handle(const QString& fileName)
{
    qDebug()<<"PreviewWorker::handle";
	QFileInfo info(fileName);
	QString suffix = info.suffix();
	suffix = suffix.toLower();

	PreviewBase* base = nullptr;

	bool check = false;
	if (suffix == "gcode")
	{
		check = true;
	}
	if (check)
	{
		base = new GcodeFilePreview();
	}

	if (base)
	{
        qDebug()<<"PreviewWorker::handle";
		connect(base, SIGNAL(sliceSuccess(SliceAttain*)), m_receiver, SLOT(onSliceSuccess(SliceAttain*)));
		base->setFileName(fileName);
		executeJob(QSharedPointer<Job>(base));
	}

	getKernel()->reuseableCache()->getCachedPrinterEntity()->visibleLogo(false);

	//PrinterEntityUse printer;
	//printer.visibleLogo(false);
}
