#include "infodispatch.h"
#include "trimesh2/TriMesh.h"

#include "qtuser3d/camera/cameracontroller.h"
#include "qtuserqml/property/qmlpropertysetter.h"

#include "interface/camerainterface.h"
#include "interface/selectorinterface.h"
#include "interface/spaceinterface.h"

#include "data/modeln.h"
#include <QTimer>
#include "kernel/abstractkernel.h"
#include <QtQml/QQmlProperty>
using namespace creative_kernel;
using namespace qtuser_3d;
using namespace qtuser_qml;

InfoDispatch::InfoDispatch(QObject* parent)
	:QObject(parent)
	, m_object(nullptr)
{
}

InfoDispatch::~InfoDispatch()
{
}

void InfoDispatch::onSelectionsChanged()
{
    QList<ModelN*> selections = selectionms();

    if (selections.size() == 1)
    {
        creative_kernel::ModelN* model = selections[0];
        qtuser_3d::Box3D box = model->globalSpaceBox();//model->localBox();
        trimesh::TriMesh* mesh = model->mesh();
        int verticessize = mesh->vertices.size();
        int facesize = mesh->faces.size();
        int errorNormals = model->getErrorNormals();
        int errorEdges = model->getErrorEdges();
        int errorHoles = model->getErrorHoles();
        int errorIntersect = model->getErrorIntersects();

        writeProperty(m_object, "modelname", model->objectName());
        writeProperty(m_object, "verticessize", verticessize);
        writeProperty(m_object, "facesize", facesize);

        writeProperty(m_object, "errornormals", errorNormals);
        writeProperty(m_object, "erroredges", errorEdges);
        writeProperty(m_object, "errorholes", errorHoles);
        writeProperty(m_object, "errorintersect", errorIntersect);

        //QQmlProperty::write(m_object, "facesize", facesize );
        writeProperty(m_object, "size", box.max - box.min);
        emit sigSendSize(box.max - box.min);
        QTimer::singleShot(200, this, SLOT(enableVisible()));

    }
    else if (selections.size() > 1)
    {
        QTimer::singleShot(200, this, SLOT(enableMulitVisible()));
    }
    else 
    {
        QTimer::singleShot(100, this, SLOT(disableVisible()));

    }
}
void InfoDispatch::enableVisible()
{
    QObject* pmainWindowObj = AbstractKernelUI::getSelf()->getUI("uiappwindow");
    bool isPreview = QQmlProperty::read(pmainWindowObj,"isPreview").toBool();

    if (!isPreview)
    {
        writeProperty(m_object, "visible", true);
        writeProperty(m_object, "infoPanelVisible", true);
        writeProperty(m_object, "infoPanelMiniVisible", false);
        writeProperty(m_object, "infoPanelMulitVisible", false);
    }
    else
    {
        writeProperty(m_object, "visible", false);
    }
}

void InfoDispatch::enableMulitVisible()
{
    int errorCnt = 0;
    int verticessizeAll = 0;
    int facesizeAll = 0;
    int errorNormalsAll = 0;
    int errorEdgesAll = 0;
    int errorIntersectAll = 0;
    int errorHolesAll = 0;
    QList<ModelN*> selections = selectionms();
    foreach(creative_kernel::ModelN * model, selections)
    {
        trimesh::TriMesh* mesh = model->mesh();
        int verticessize = mesh->vertices.size();
        int facesize = mesh->faces.size();
        //model->needDetectError();
        int errorNormals = model->getErrorNormals();
        int errorEdges = model->getErrorEdges();
        int errorHoles = model->getErrorHoles();
        int errorIntersect = model->getErrorIntersects();

        if (errorEdges > 0 || errorNormals > 0 || errorHoles > 0 || errorIntersect > 0)
        {
            verticessizeAll += verticessize;
            facesizeAll += facesize;
            errorNormalsAll += errorNormals;
            errorEdgesAll += errorEdges;
            errorIntersectAll += errorIntersect;
            errorHolesAll += errorHoles;
            errorCnt++;
        }
    }

    writeProperty(m_object, "infoPanelVisible", false);
    writeProperty(m_object, "infoPanelMiniVisible", false);

    if (errorCnt > 0)
    {
        writeProperty(m_object, "errormodel", errorCnt);
        writeProperty(m_object, "verticessize", verticessizeAll);
        writeProperty(m_object, "facesize", facesizeAll);

        writeProperty(m_object, "errornormals", errorNormalsAll);
        writeProperty(m_object, "erroredges", errorEdgesAll);
        writeProperty(m_object, "errorholes", errorHolesAll);
        writeProperty(m_object, "errorintersect", errorIntersectAll);

        writeProperty(m_object, "infoPanelMulitVisible", true);
    }
    else
    {
        writeProperty(m_object, "visible", false);
    }

    
}

void InfoDispatch::disableVisible()
{
    int errorCnt = 0;
    QList<creative_kernel::ModelN*> alls = creative_kernel::modelns();
    foreach(creative_kernel::ModelN * model, alls)
    {
        int errorNormals = model->getErrorNormals();
        int errorEdges = model->getErrorEdges();
        int errorHoles = model->getErrorHoles();
        int errorIntersect = model->getErrorIntersects();

        if (errorEdges > 0 || errorNormals > 0 || errorHoles > 0 || errorIntersect >0)
        {
            errorCnt++;
        }
    }

    if (errorCnt > 0)
    {
        writeProperty(m_object, "errormodel", errorCnt);
        writeProperty(m_object, "infoPanelMiniVisible", true);
        writeProperty(m_object, "infoPanelVisible", false);
        writeProperty(m_object, "infoPanelMulitVisible", false);
    }
    else
    {
        writeProperty(m_object, "visible", false);
    }
}
void InfoDispatch::selectChanged(Pickable* pickable)
{

}
void InfoDispatch::onBoxChanged(qtuser_3d::Box3D& box)
{}

void InfoDispatch::onSceneChanged(qtuser_3d::Box3D& box)
{
    QList<ModelN*> selections = selectionms();
    if (selections.size() == 1)
    {
        creative_kernel::ModelN* model = selections[0];
        qtuser_3d::Box3D box = model->globalSpaceBox();
        trimesh::TriMesh* mesh = model->mesh();
        int verticessize = mesh->vertices.size();
        int facesize = mesh->faces.size();
        writeProperty(m_object, "modelname", model->objectName());
        writeProperty(m_object, "verticessize", verticessize);

        //QQmlProperty::write(m_object, "facesize", facesize );
        writeProperty(m_object, "size", box.max - box.min);
        QTimer::singleShot(200, this, SLOT(enableVisible()));
    }
    else
    {
        QTimer::singleShot(200, this, SLOT(disableVisible()));
    }

}
void InfoDispatch::setObject(QObject* object)
{
	m_object = object;

    QObject* controller = cameraController();
    writeObjectProperty(m_object, "camera", controller);

    connect(m_object, SIGNAL(sigUpdateInfo()), this, SLOT(updateInfo()));
}

QString InfoDispatch::text()
{
	return m_text;
}

QVector3D InfoDispatch::size()
{
    return QVector3D(300,300,300);
}

void InfoDispatch::setText(const QString& text)
{

}

void InfoDispatch::updateInfo()
{
    onSelectionsChanged();
}
