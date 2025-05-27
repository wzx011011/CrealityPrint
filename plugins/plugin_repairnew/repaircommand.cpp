#include "repaircommand.h"
#include "repairop.h"

#include "interface/spaceinterface.h"
#include "interface/visualsceneinterface.h"
#include "kernel/translator.h"
#include "kernel/abstractkernel.h"

#include"kernel/kernelui.h"
#include "utils/meshloaderwrapper.h"
//#include "hole_filling_TMESH.h"
#include "mmesh/trimesh/trimeshutil.h"
#include "omp.h"  
#include "interface/modelinterface.h"
#include "interface/gadatainterface.h"
using namespace trimesh;


RepairCommand::RepairCommand()
	: m_op(nullptr)
{
	m_repairModels.clear();
	m_shortcut = "Ctrl+R";
    m_actionname = tr("Model Repair");
    m_actionNameWithout = "Model Repair";
	m_strMessageText = tr("Please import model");
	m_eParentMenu = eMenuType_Tool;//eMenuType_Repair;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));

	MeshLoaderWrapper* loader = getKernelUI()->getMeshLoaderWrapper();
	connect(loader, SIGNAL(sigDetectModel()), this, SLOT(slotsDetectModel()));

	QObject* pRoot = AbstractKernelUI::getSelf()->getUI("UIRoot");
	QObject* pinfoshowObj = pRoot->findChild<QObject*>("infoshowObj");
	connect(pinfoshowObj, SIGNAL(sigRepair()), this, SLOT(slotRepair()));

	QMetaObject::invokeMethod(pinfoshowObj, "updateInfo");
}

RepairCommand::~RepairCommand()
{
	bool ret = disconnect(m_op, SIGNAL(repairSuccess()), this, SLOT(slotRepairSuccess()));
}

void RepairCommand::slotRepair()
{
	execute();
}

void RepairCommand::slotLanguageChanged()
{
	m_actionname = tr("Model Repair") + "        " + m_shortcut;
	m_strMessageText = tr("Please import model");
}
void RepairCommand::slotLanguageChanged2()
{
	m_actionname = tr("Model Repair") + "        " + m_shortcut;
	m_strMessageText = tr("Please select model");
}

void RepairCommand::execute()
{
	m_nverticessizeAll = 0;
	m_facesizeAll = 0;
	m_errorNormalsAll = 0;
	m_errorEdgesAll = 0;
	m_shellsAll = 0;
	m_holesAll = 0;
	m_intersectsAll = 0;
	if (!haveModels())
	{
		slotLanguageChanged();
		AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "messageSingleBtnDlg");
		return;
	}

	QList<ModelN*> selections = selectionms();
	if (selections.size() < 1)
	{
		slotLanguageChanged2();
		AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "messageSingleBtnDlg");
		//getKernelUI()->requestMenuDialog(this, "messageSingleBtnDlg");
		return;
	}

	foreach(creative_kernel::ModelN * model, selections)
	{
		trimesh::TriMesh* mesh = model->mesh();
		int verticessize = mesh->vertices.size();
		int facesize = mesh->faces.size();
		model->needDetectError();
		int errorNormals = model->getErrorNormals();
		int errorEdges = model->getErrorEdges();
		int errorHoles = model->getErrorHoles();
		int errorIntersect = model->getErrorIntersects();

		if (errorEdges > 0 || errorNormals > 0 || errorHoles > 0 || errorIntersect > 0)
		{
			//m_nverticessizeAll += verticessize;
			//m_facesizeAll += facesize;
			m_errorNormalsAll += errorNormals;
			m_errorEdgesAll += errorEdges;
			m_holesAll += errorHoles;
			m_intersectsAll += errorIntersect;
			//errorCnt++;
		}
	}

	AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "repaircmdDlg");

}

void RepairCommand::slotRepairSuccess()
{
	m_strMessageText = tr("Repair Model Finish!!!");
	AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "messageSingleBtnDlg");
	
	bool ret = disconnect(m_op, SIGNAL(repairSuccess()), this, SLOT(slotRepairSuccess()));
	//getKernelUI()->requestMenuDialog(this, "messageSingleBtnDlg");
	//QMetaObject::invokeMethod(getKernelUI()->footer(), "showJobFinishMessage", Q_ARG(QVariant, QVariant::fromValue(this)), Q_ARG(QVariant, QVariant::fromValue(str)));
	delete m_op;
	m_op = nullptr;
}
QString RepairCommand::getMessageText()
{
	return m_strMessageText;
}

void RepairCommand::acceptRepair()
{
	QList<ModelN*> selections = selectionms();
	if (!m_op)
	{
		m_op = new RepairOp(this);
	}
	m_op->repairModel(selections);

	bool ret = disconnect(m_op, SIGNAL(repairSuccess()), this, SLOT(slotRepairSuccess()));
	connect(m_op, SIGNAL(repairSuccess()), this, SLOT(slotRepairSuccess()));
}
void RepairCommand::accept()
{
	

	//update info
	QObject* pinfoshowObj = AbstractKernelUI::getSelf()->getUI("UIRoot")->findChild<QObject*>("infoshowObj");
	QMetaObject::invokeMethod(pinfoshowObj, "updateInfo");
}

void RepairCommand::slotsDetectModel()
{
	qDebug() << "detect model";
	QList<ModelN*> selections = selectionms();
	/*if (!m_op)
	{
		m_op = new RepairOp(this);
		m_op->judgeModel(selections);
	}
	bool ret = disconnect(m_op, SIGNAL(judgeSuccess()), this, SLOT(slotJudgeSuccess()));
	connect(m_op, SIGNAL(judgeSuccess()), this, SLOT(slotJudgeSuccess()));
	setVisOperationMode(m_op);*/
	m_repairModels.clear();
	for (int i=0; i<selections.size(); ++i)
	{
		ModelN* m = selections.at(i);
		TriMesh* choosedMesh = m->mesh();
		//if (judgeModelHasBoundaries(choosedMesh))
		{
			m_repairModels.push_back(m);
		}
	}
	if(m_repairModels.size() > 0)
		AbstractKernelUI::getSelf()->executeCommand("requestMenuDialog", this, "modelRepairMessageDlg");
}
/*
void RepairCommand::slotJudgeSuccess()
{
	bool ret = disconnect(m_op, SIGNAL(judgeSuccess()), this, SLOT(slotJudgeSuccess()));
	delete m_op;
	m_op = nullptr;
}*/

void RepairCommand::repairModel()
{
	if (m_repairModels.size() < 1)
	{
		return;
	}

	if (!m_op)
	{
		m_op = new RepairOp(this);
		m_op->repairModel(m_repairModels);
	}

	bool ret = disconnect(m_op, SIGNAL(repairSuccess()), this, SLOT(slotRepairSuccess()));
	connect(m_op, SIGNAL(repairSuccess()), this, SLOT(slotRepairSuccess()));
	setVisOperationMode(m_op);
}

void RepairCommand::delSelectedModel()
{
	removeSelectionModels();
}
