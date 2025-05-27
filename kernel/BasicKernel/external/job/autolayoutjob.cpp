#include "autolayoutjob.h"
#include "interface/spaceinterface.h"
#include "utils/modelpositioninitializer.h"
#include "interface/visualsceneinterface.h"
#include "interface/modelinterface.h"
#include "mmesh/util/modelbblayouter.h"
#include "nestplacer/nestplacer.h"
#include "concavehull.h"

#include "us/slicermanager.h"

using namespace creative_kernel;
AutoLayoutJob::AutoLayoutJob(QObject *parent) : qtuser_core::Job(parent)
{

}

QString AutoLayoutJob::name()
{
    return "Layout Calculate";
}

QString AutoLayoutJob::description()
{
    return "";
}

void AutoLayoutJob::failed()
{

}

bool caseInsensitiveLessThan( ModelN* s1,  ModelN* s2)
{

    Box3D s1box = s1->globalSpaceBox();
    Box3D s2box = s2->globalSpaceBox();
    return  s1box.size().z() > s2box.size().z();
}

std::vector<trimesh::vec3> outLine(ModelN* model)
{
    QVector3D offset = model->localPosition();
    trimesh::vec3 toffset = trimesh::vec3(offset.x(), offset.y(), offset.z());
    std::vector<trimesh::vec3> lines;
    qtuser_3d::ConvexPolygon& convex = model->convexPolygon();
    for (QVector3D& v : convex.m_polygons)
        lines.push_back(trimesh::vec3(v.x(), v.y(), v.z()) - toffset);
    return lines;
}

std::vector<trimesh::vec3> outLine_concave(ModelN* model)
{
    QVector3D scale = model->localScale();
    trimesh::TriMesh* mesh = model->mesh();
    std::vector<double> vertices_data, vertices_data_dst;
    vertices_data.reserve(mesh->vertices.size() * 2);
    for (trimesh::point pt : mesh->vertices)
    {
        vertices_data.push_back(pt.x * scale.x());
        vertices_data.push_back(pt.y * scale.y());
    }
    vertices_data_dst = concavehull(vertices_data);

    std::vector<trimesh::vec3> lines;
    for (int i = 0; i < vertices_data_dst.size(); i += 2)
        lines.push_back(trimesh::vec3(vertices_data_dst[i], vertices_data_dst[i + 1], 0.f));
    return lines;
}

// invoke from main thread
void AutoLayoutJob::successed(qtuser_core::Progressor* progressor)
{
    QList<ModelN*> models = modelns();
    removeAllModel(false);
    std::sort(models.begin(), models.end(), caseInsensitiveLessThan);


    Box3D baseBox = baseBoundingBox();
    trimesh::box3 workspaceBox(trimesh::vec3(baseBox.min.x(), baseBox.min.y(), baseBox.min.z()), trimesh::vec3(baseBox.max.x(), baseBox.max.y(), baseBox.max.z()));

    std::vector<int> modelIndices;
    for (int i = 0; i < models.size(); i++)
    {
        modelIndices.push_back(i);
    }

    std::function<trimesh::box3(int)> getModelBoxFunc = [models](int modelIndex)->trimesh::box3 {
        Box3D box = models[modelIndex]->globalSpaceBox();
        trimesh::box3 modelBox(trimesh::vec3(box.min.x(), box.min.y(), box.min.z()), trimesh::vec3(box.max.x(), box.max.y(), box.max.z()));
        return modelBox;
    };

#if (0)
        std::function<void(int, trimesh::vec3)> modelPositionUpdateFunc = [models](int modelIndex, trimesh::vec3 newBoxCenter) {
            QVector3D newBC = QVector3D(newBoxCenter.x, newBoxCenter.y, newBoxCenter.z);
            Box3D curgbox = models[modelIndex]->globalSpaceBox();
            QVector3D curgboxcenter = (curgbox.max + curgbox.min) * 0.5;
            QVector3D translate = newBC - curgboxcenter + models[modelIndex]->localPosition();

            models[modelIndex]->SetInitPosition(translate);
            models[modelIndex]->setLocalPosition(translate);
        };
        mmesh::ModelBBLayouter::layout_all(workspaceBox, modelIndices, getModelBoxFunc, modelPositionUpdateFunc);
#else
        us::GlobalSetting* gsets = SlicerManager::instance().globalsettings();
        trimesh::vec3 offset(0, 0, 0);
        if (gsets->settings().value("machine_center_is_zero")->value().toBool())
        {
            offset = trimesh::vec3((workspaceBox.max.x - workspaceBox.min.x) / 2, (workspaceBox.max.y - workspaceBox.min.y) / 2, 0);
        }

        nestplacer::PlaceType packType = nestplacer::PlaceType::CENTER_TO_SIDE;
        bool isBelt = gsets->settings().value("machine_is_belt")->value().toBool();
        if (isBelt)
            packType = nestplacer::PlaceType::ONELINE;

        std::function<void(int, trimesh::vec3)> modelPositionUpdateFunc_nest = [models, offset, packType](int modelIndex, trimesh::vec3 newBoxCenter) {
            QQuaternion dq = QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 0.0f, 1.0f), newBoxCenter.z);
            QQuaternion q = models[modelIndex]->localQuaternion();
            QVector3D translate = QVector3D(newBoxCenter.x - offset.x, newBoxCenter.y - offset.y, 0.0f);
            translate.setZ(models[modelIndex]->localPosition().z());
            models[modelIndex]->setLocalPosition(translate);
            if (packType != nestplacer::PlaceType::CONCAVE)
                dq = dq * q;
            setModelRotation(models[modelIndex], dq, true); 
        };
        std::vector < std::vector<trimesh::vec3>> modelsData;
        for (ModelN* m : models)
        {
            std::vector<trimesh::vec3> oItem = outLine(m);
            if (packType == nestplacer::PlaceType::CONCAVE)
                oItem = outLine_concave(m);      
            modelsData.push_back(oItem);
        }

        nestplacer::NestParaFloat para = nestplacer::NestParaFloat(workspaceBox, 10.0f, packType, false);
        nestplacer::NestPlacer::layout_all_nest(modelsData, modelIndices, para, modelPositionUpdateFunc_nest);
#endif

    checkModelRange();
    checkBedRange();


    for (ModelN* m : models)
    {
        addModel(m,false);
    }
    requestVisUpdate();
}

void AutoLayoutJob::work(qtuser_core::Progressor* progressor)
{

}
