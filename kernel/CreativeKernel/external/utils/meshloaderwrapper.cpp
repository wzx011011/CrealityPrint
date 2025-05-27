#include "meshloaderwrapper.h"
#include "data/meshiocenter.h"

#include "qtusercore/string/urlutils.h"

#include "job/meshloadjob.h"
#include "job/calculatenormaljob.h"
#include "job/removedumplicatevertexjob.h"
#include "job/meshloadinfo.h"
#include "job/geometrygenjob.h"

#include "interface/jobsinterface.h"
#include "kernel/kernel.h"
#include "kernel/kernelui.h"

#include "data/modeln.h"
#include "data/meshiocenter.h"
#include "interface/selectorinterface.h"
#include "interface/spaceinterface.h"
#include "utils/modelpositioninitializer.h"
#include "interface/modelinterface.h"
#include "us/slicermanager.h"
//#include "repairop.h"
using namespace creative_kernel;
MeshLoaderWrapper::MeshLoaderWrapper(QObject* parent)
{
	connect(&MESHIO, &MeshIOCenter::loadExtensionFiltersChanged, this, &MeshLoaderWrapper::slotSupportFiltersChanged);
}

MeshLoaderWrapper::~MeshLoaderWrapper()
{
}

QStringList MeshLoaderWrapper::supportFilters()
{
	//QStringList typeList;
	//typeList << "stl";
	//typeList << "obj";
	//return typeList;

	return MESHIO.loadExtensionList();
}

QStringList MeshLoaderWrapper::enableFilters()
{
	QStringList filterlist;
	filterlist << "stl *.obj";
	//filterlist << "stp *.step *.dae *.3mf *.3ds *.iges *.wrl *.cxbin *.f3d *.off *.ply *.off";
	filterlist << "dae *.3mf *.3ds *.wrl *.cxbin *.f3d *.off *.ply *.off";
	return filterlist;
}

void MeshLoaderWrapper::handle(const QString& fileName)
{
	QStringList fileNames;
	fileNames << fileName;
	load(fileNames);

	emit sigFinished(fileName);
};

void MeshLoaderWrapper::handle(const QStringList& fileNames)
{
	load(fileNames);

	for (const QString& fileName : fileNames)
		emit sigFinished(fileName);
};

void MeshLoaderWrapper::load(const QStringList& fileNames)
{
	if (jobExecutorAvaillable())
	{
		QList<JobPtr> jobs;
		for (const QString& fileName : fileNames)
		{
			qDebug() << "load " << fileName;

			MeshLoadJob* loadJob = new MeshLoadJob();
			CalculateNormalJob* calculateNormalJob = new CalculateNormalJob();
			RemoveDumplicateVertexJob* removeDumplicateVertexJob = new RemoveDumplicateVertexJob();
			GeometryGenJob* geometryGenJob = new GeometryGenJob();
			MeshLoadInfo* meshLoadInfo = new MeshLoadInfo(geometryGenJob);
			//MeshLoadInfo* meshLoadInfo = new MeshLoadInfo(calculateNormalJob);
			//meshLoadInfo->m_meshType = 2;

			meshLoadInfo->m_name = fileName;
			loadJob->m_meshLoadInfo = meshLoadInfo;
			removeDumplicateVertexJob->m_meshLoadInfo = meshLoadInfo;
			calculateNormalJob->m_meshLoadInfo = meshLoadInfo;
			geometryGenJob->m_meshLoadInfo = meshLoadInfo;
			
			bool useRemove = true;
			JobPtr removeJob(removeDumplicateVertexJob);
			jobs.push_back(JobPtr(loadJob));
			if(useRemove) jobs.push_back(removeJob);
			jobs.push_back(JobPtr(calculateNormalJob));
			jobs.push_back(JobPtr(geometryGenJob));
		}
		executeJobs(jobs);


		disconnect(getKernel()->jobExecutor(), SIGNAL(jobsEnd()), this, SLOT(finishJob()));
		connect(getKernel()->jobExecutor(), SIGNAL(jobsEnd()), this, SLOT(finishJob()));
		
        
	}
}
void MeshLoaderWrapper::finishOpen(QString strCurrentFile)
{
    emit sigFinished(strCurrentFile);
}

void MeshLoaderWrapper::finishJob()
{
	qDebug() << "finishJob success !!";

	QList<creative_kernel:: ModelN*> selections = selectionms();

	if (selections.size())
	{
		creative_kernel::ModelN* model = selections[selections.size() - 1];
        Box3D baseBox = baseBoundingBox();
		Box3D modelbox = model->globalSpaceBox();
        if (modelbox.size().x() > baseBox.size().x() ||
			modelbox.size().y() > baseBox.size().y() ||
			modelbox.size().z() > baseBox.size().z())
        {
            //QMetaObject::invokeMethod(getKernelUI()->footer(), "showAdaptShow",Q_ARG(QVariant, QVariant::fromValue(this)));
			AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "messageTooBigDlg");
        }
		else 
		{
			//emit sigDetectModel();
		}		
	}
	qDebug() << "MeshLoaderWrapper::finishJob()";
	QMetaObject::invokeMethod(getKernelUI()->leftToolbar(), "switchPickMode", Qt::ConnectionType::QueuedConnection);

	disconnect(getKernel()->jobExecutor(), SIGNAL(jobsEnd()), this, SLOT(finishJob()));
}

void MeshLoaderWrapper::slotSupportFiltersChanged(QStringList newExtensions)
{
	emit sigSupportFiltersChanged(newExtensions);
}

void MeshLoaderWrapper::adaptPlatform()
{
	QList<creative_kernel::ModelN*> selections = selectionms();
	if (selections.size())
	{
		creative_kernel::ModelN* model = selections[selections.size() - 1];
		ModelPositionInitializer::AdaptionModel(model);

		//	ModelPositionInitializer::layout(model, NULL, true);
		//emit sigDetectModel();
	}
}

void MeshLoaderWrapper::delSelectedModel()
{
	removeSelectionModels();
}

QString MeshLoaderWrapper::getMessageText()
{
	QString str = tr("The model is too large, Scale to the fit size?");
	return str;
}
