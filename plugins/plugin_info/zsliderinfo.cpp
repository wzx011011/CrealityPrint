#include "zsliderinfo.h"
#include "qtuserqml/property/qmlpropertysetter.h"

#include "interface/selectorinterface.h"
#include "interface/spaceinterface.h"
#include "data/modeln.h"
#include "interface/reuseableinterface.h"
#include "interface/visualsceneinterface.h"
#include <QDebug>

using namespace creative_kernel;
using namespace qtuser_3d;
using namespace qtuser_qml;

ZSliderInfo::ZSliderInfo(QObject* parent)
	:QObject(parent)
	, m_object(nullptr)
{
    initMaxLayer();
}

ZSliderInfo::~ZSliderInfo()
{
}
void ZSliderInfo::enableVisible()
{
    writeProperty(m_object, "visible", true);
}
void ZSliderInfo::disableVisible()
{
     writeProperty(m_object, "visible", false);

}

void ZSliderInfo::setObject(QObject* object)
{
    m_object = object;
}
void ZSliderInfo::updateZHeight(float layer)
{
    m_maxLayer = ceil(layer) + 1;
    qDebug()<<"m_maxLayer =" << m_maxLayer;
    emit maxZHeightChanged();
}

void ZSliderInfo::initMaxLayer()
{
    m_maxLayer = 1;
}

int ZSliderInfo::layers()
{
    return m_maxLayer;
}
void ZSliderInfo::setTopCurrentLayer(float layer)
{
    setModelEffectClipMaxZ(layer);
    requestVisUpdate();
}
void ZSliderInfo::setBottomCurrentLayer(float layer)
{
    setModelEffectClipBottomZ(layer);
    requestVisUpdate();
}

void ZSliderInfo::onBoxChanged(qtuser_3d::Box3D& box)
{
//    qDebug()<<"ZSliderInfo::onBoxChanged";
    //qDebug()<<sceneBoundingBox().max;
    //qtuser_3d::Box3D box3d = baseBoundingBox();
    //float maxZ = box3d.max.z();
    //updateZHeight(maxZ);
}

void ZSliderInfo::onSceneChanged(qtuser_3d::Box3D& box)
{
//    qDebug()<<"onSceneChanged success !!!";
    qtuser_3d::Box3D box3d = sceneBoundingBox();
    float maxZ = box.max.z();
    if (m_maxLayer == ceil(maxZ) + 1)
    {
        return;
    }
    updateZHeight(maxZ);
}
