#include "extendloaderwrapper.h"
#include "data/meshiocenter.h"

#include "qtusercore/string/urlutils.h"

#include "interface/jobsinterface.h"
#include "kernel/abstractkernel.h"
#include "kernel/kernel.h"

#include "interface/selectorinterface.h"
#include "interface/spaceinterface.h"
#include "utils/modelpositioninitializer.h"
#include "job/meshloadinfo.h"
#include "job/geometrygenjob.h"

#include "extendmeshgenjob.h"
#include "interface/modelinterface.h"
using namespace creative_kernel;
ExtendLoaderWrapper::ExtendLoaderWrapper(QObject* parent)
{
    m_filesName.clear();
}

ExtendLoaderWrapper::~ExtendLoaderWrapper()
{
}

QStringList ExtendLoaderWrapper::supportFilters()
{
	QStringList typeList;
	typeList << "bmp *.jpg *.jpeg *.png";
	//typeList << "jpg";
	//typeList << "jpeg";
	//typeList << "png";

	typeList << "gtl *.gbl *.gbo *.gbp *.gbs *.gko *.gml *.gpb *.gpt *.gto *.gtp *.gts";

	return typeList;
}

void ExtendLoaderWrapper::handle(const QString& fileName)
{
	QStringList fileNames;
	fileNames << fileName;
    m_filesName = fileNames;
	AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "importimageDlg");
	//getKernelUI()->requestMenuDialog(this, "importimageDlg");

	/*load(fileNames);

	emit sigFinished(fileName);*/
};

void ExtendLoaderWrapper::handle(const QStringList& fileNames)
{
    m_filesName = fileNames;
	load(fileNames);

	for (const QString& fileName : fileNames)
		emit sigFinished(fileName);
};

void ExtendLoaderWrapper::load(const QStringList& fileNames)
{
	if (jobExecutorAvaillable())
	{
		QList<JobPtr> jobs;
		for (const QString& fileName : fileNames)
		{
			qDebug() << "extend load " << fileName;

			ExtendMeshGenJob* loadJob = new ExtendMeshGenJob();
			loadJob->setName(fileName);
			GeometryGenJob* geometryGenJob = new GeometryGenJob();
			MeshLoadInfo* meshLoadInfo = new MeshLoadInfo(geometryGenJob);

			meshLoadInfo->m_name = fileName;
			loadJob->m_meshLoadInfo = meshLoadInfo;
			geometryGenJob->m_meshLoadInfo = meshLoadInfo;
            loadJob->m_p2m = m_pt2mesh;
			jobs.push_back(JobPtr(loadJob));
			jobs.push_back(JobPtr(geometryGenJob));
		}
		
		executeJobs(jobs);

		disconnect(getKernel()->jobExecutor(), SIGNAL(jobsEnd()), this, SLOT(finishJob()));
		connect(getKernel()->jobExecutor(), SIGNAL(jobsEnd()), this, SLOT(finishJob()));
		
		AbstractKernelUI::getSelf()->switchPickMode();
        //QMetaObject::invokeMethod(getKernelUI()->leftToolbar(), "switchPickMode");
	}
}
void ExtendLoaderWrapper::finishOpen(QString strCurrentFile)
{
    emit sigFinished(strCurrentFile);
}

void ExtendLoaderWrapper::finishJob()
{
	qDebug() << "extendmesh finishJob success !!";

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
			AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "messageTooBigDlg");
			/*AbstractKernelUI::getSelf()->showAdaptShow(this);*/
            //QMetaObject::invokeMethod(getKernelUI()->footer(), "showAdaptShow",Q_ARG(QVariant, QVariant::fromValue(this)));
        }
	}
	disconnect(getKernel()->jobExecutor(), SIGNAL(jobsEnd()), this, SLOT(finishJob()));
}
/*void ExtendLoaderWrapper::adaptFunction()
{
	
	QList<creative_kernel::ModelN*> selections = selectionms();

	if (selections.size())
	{
		creative_kernel::ModelN* model = selections[selections.size() - 1];
		ModelPositionInitializer::layout(model, NULL,true);
	}
}*/

Q_INVOKABLE void ExtendLoaderWrapper::accept()
{

    load(m_filesName);
}

Q_INVOKABLE void ExtendLoaderWrapper::cancel()
{
	return Q_INVOKABLE void();
}


Q_INVOKABLE void ExtendLoaderWrapper::setBaseHeight(float baseHeight)
{
    m_pt2mesh.setBaseHeight(baseHeight);
}
Q_INVOKABLE void ExtendLoaderWrapper::setMaxHeight(float maxHeight)
{

    m_pt2mesh.setMaxHeight(maxHeight);
}

Q_INVOKABLE void ExtendLoaderWrapper::setLighterOrDarker(int index)
{
    if(index ==0)
    {
        m_pt2mesh.darkerIsHigh();
    }
    else
    {
        m_pt2mesh.lighterIsHigh();
    }
}
Q_INVOKABLE void ExtendLoaderWrapper::setMeshWidth(float meshX)
{
    m_pt2mesh.setMeshWidth(meshX);
}
 Q_INVOKABLE void ExtendLoaderWrapper::setBlur(int blur)
{
    m_pt2mesh.setBlur(blur);
}

void ExtendLoaderWrapper::adaptPlatform()
{
	QList<creative_kernel::ModelN*> selections = selectionms();

	if (selections.size())
	{
		creative_kernel::ModelN* model = selections[selections.size() - 1];
		ModelPositionInitializer::layout(model, NULL, true);
	}
}

 void ExtendLoaderWrapper::delSelectedModel()
 {
	 removeSelectionModels();
 }

QString ExtendLoaderWrapper::getMessageText()
{
	QString str = tr("The model is too large, Scale to the fit size?");
	return str;
}