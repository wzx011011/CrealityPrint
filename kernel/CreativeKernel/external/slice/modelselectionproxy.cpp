#include "modelselectionproxy.h"
#include <QtCore/QItemSelectionModel>

#include "interface/selectorinterface.h"
#include "kernel/abstractkernel.h"
#include "data/modeln.h"
#include "qtuserqml/plugin/treemodel.h"

using namespace creative_kernel;
ModelSelectionProxy::ModelSelectionProxy(QObject* parent)
	:QObject(parent)
	, m_selectionProxy(nullptr)
{
}

ModelSelectionProxy::~ModelSelectionProxy()
{
}

void ModelSelectionProxy::setSelectionProxy(QObject* object)
{
	m_selectionProxy = object;
}

void ModelSelectionProxy::onSelectionsChanged()
{
    QList<ModelN*> selections = selectionms();

//    //QMetaObject::invokeMethod(m_selectionProxy, "clear");
//    if (selections.size() > 0)
//    {
//        qtuser_qml::TreeModel* treeModel = AbstractKernelUI::getSelf()->treeModel();    // getKernelUI()->treeModel();
//        for (ModelN* model : selections)
//        {
//            QModelIndex index = treeModel->getModelIndex(model);
//            //QMetaObject::invokeMethod(m_selectionProxy, "select", Q_ARG(QModelIndex, index), Q_ARG(QItemSelectionModel::SelectionFlags, QItemSelectionModel::Select));
//        }
//    }
    emit selectChanged();
}

void ModelSelectionProxy::selectChanged(qtuser_3d::Pickable* pickable)
{

}
