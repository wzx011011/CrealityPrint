#include "laserplugin.h"
#include "laserdispatch.h"
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlProperty>
#include "kernel/abstractkernel.h"
#include "interface/camerainterface.h"
#include "interface/selectorinterface.h"
#include "interface/spaceinterface.h"

#include "qtuser3d/camera/cameracontroller.h"
#include "qtuserqml/property/qmlpropertysetter.h"

using namespace creative_kernel;
using namespace qtuser_3d;
using namespace qtuser_qml;

LaserPlugin::LaserPlugin(QObject* parent)
	:QObject(parent)
    , m_lasterscene(nullptr),m_plotterscene(nullptr)

{

}

LaserPlugin::~LaserPlugin()
{
    delete m_lasterscene;
    delete m_plotterscene;
}

QString LaserPlugin::name()
{
	return "LaserPlugin";
}

QString LaserPlugin::info()
{
	return "";
}
void LaserPlugin::onNewDrawObject(DrawObject::SHAPETYPE shaptype)
{

}
void LaserPlugin::initialize()
{
    m_lasterscene = new LaserScene(this,LaserScene::SCENETYPE::LASER);
    m_plotterscene = new LaserScene(this,LaserScene::SCENETYPE::PLOTTER);


    //m_root = AbstractKernelUI::getSelf()->getUI("glmainview")->parent()->findChild<QObject*>("otherView");   // getUIRoot();
    /*QQmlContext* context = qmlContext(m_root);
    QQmlEngine* engine = qmlEngine(m_root);
    if (!m_infocomponent)
	{
        m_infocomponent = new QQmlComponent(engine, QUrl::fromLocalFile(":/laser/laser/RightPanel.qml"), this);
        m_infoUI = m_infocomponent->create(context);
        m_infoUI->setObjectName("LaserLayerOut");
        m_infoUI->setParent(m_root);
        //QQmlProperty::write(m_infoUI, "visible", QVariant::fromValue(true));
    }*/




}

void LaserPlugin::uninitialize()
{

}

