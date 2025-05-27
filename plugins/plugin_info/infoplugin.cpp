#include "infoplugin.h"
#include "infodispatch.h"

#include <QtQml/QQmlComponent>
#include <QQmlContext>
#include <QSettings>
//#include "kernel/abstractkernel.h"
#include "interface/camerainterface.h"
#include "interface/selectorinterface.h"
#include "interface/spaceinterface.h"

#include "qtuser3d/camera/cameracontroller.h"
#include "qtuserqml/property/qmlpropertysetter.h"

#include "zsliderinfo.h"
#include "kernel/translator.h"
#include "kernel/kernelui.h"
#include "kernel/abstractkernel.h"


using namespace creative_kernel;
using namespace qtuser_3d;
using namespace qtuser_qml;

InfoPlugin::InfoPlugin(QObject* parent)
	:QObject(parent)
	, m_root(nullptr)
	, m_infoUI(nullptr)
    , m_infocomponent(nullptr)
    , m_btnUI(nullptr)
    , m_btncomponent(nullptr)
	, m_infoDispatch(nullptr)
    ,m_zSlidercomponent(nullptr)
    ,m_zSliderUI(nullptr)
    ,m_sliderInfo(nullptr)
{
    disconnect(Translator::getInstance(), SIGNAL(languageChanged()), this, SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(), SIGNAL(languageChanged()), this, SLOT(slotLanguageChanged()));

    disconnect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
    connect(getKernelUI(), SIGNAL(sigChangeThemeColor()), this, SLOT(slotThemeChanged()));
}

InfoPlugin::~InfoPlugin()
{
}

QString InfoPlugin::name()
{
	return "InfoPlugin";
}

QString InfoPlugin::info()
{
	return "";
}

void InfoPlugin::initialize()
{
    m_root = AbstractKernelUI::getSelf()->getUI("UIRoot");   // getUIRoot();
    QQmlContext* context = qmlContext(m_root);
    QQmlEngine* engine = qmlEngine(m_root);
    if (!m_infocomponent)
	{
//        m_infocomponent = new QQmlComponent(engine, QUrl::fromLocalFile(":/info/info/info.qml"), this);
//        m_infoUI = m_infocomponent->create(context);
//        m_infoUI->setObjectName("infoshowObj");
//        m_infoUI->setParent(m_root);
	}
    if(!m_btncomponent)
    {
        m_btncomponent = new QQmlComponent(engine, QUrl::fromLocalFile(":/info/info/RightBtns.qml"), this);
        m_btnUI = m_btncomponent->create(context);
       // m_btnUI = m_btncomponent->create(context);
    }

	m_infoDispatch = new InfoDispatch(this);    
    engine->rootContext()->setContextProperty("infoDispatch", m_infoDispatch);
    m_infoDispatch->setObject(m_infoUI);
    addSelectTracer(m_infoDispatch);
    //start 2020-10-13 lisugui
    creative_kernel::traceSpace(m_infoDispatch);
    //end

    QObject* controller = cameraController();
    writeObjectProperty(m_btnUI, "parent", m_root);
    writeObjectProperty(m_btnUI, "camera", controller);
    writeObjectProperty(m_btnUI, "plugin", this);
    QMetaObject::invokeMethod(m_btnUI, "connectFunc");
    writeObjectProperty(m_infoUI, "parent", m_root);
    //QQmlProperty::write(m_infoUI, "modeldata", );
	//QQmlProperty::write(m_infoUI, "source", QVariant::fromValue<QObject*>(m_infoDispatch));
    if(!m_zSlidercomponent)
    {
        m_zSlidercomponent = new QQmlComponent(engine, QUrl::fromLocalFile(":/info/info/ZSliderInfoPreview.qml"), this);
        m_zSliderUI = m_zSlidercomponent->create(context);
        if(m_zSliderUI)
        {
            m_zSliderUI->setObjectName("zsliderObj");
            m_zSliderUI->setParent(m_root);
        }
    }
    writeObjectProperty(m_zSliderUI, "parent", m_root);
    m_sliderInfo = new ZSliderInfo(this);
    m_sliderInfo->setObject(m_zSliderUI);
    creative_kernel::traceSpace(m_sliderInfo);
    writeObjectProperty(m_zSliderUI,"sliderObj",m_sliderInfo);
    slotLanguageChanged();
}

void InfoPlugin::uninitialize()
{
    creative_kernel::unTraceSpace(m_infoDispatch);
    removeSelectorTracer(m_infoDispatch);

    creative_kernel::unTraceSpace(m_sliderInfo);
    writeObjectProperty(m_infoUI, "parent", nullptr);
}

void InfoPlugin::slotLanguageChanged()
{
    QSettings setting;
    setting.beginGroup("language_perfer_config");
    QString lang = setting.value("language_type", "en.ts").toString();
    setting.endGroup();

    qDebug() << lang;
    if (lang == "en.ts")
    {
        emit sigLanguageChanged(0);
    }
    else
    {
        emit sigLanguageChanged(1);
    }
    
}

void InfoPlugin::slotThemeChanged()
{
    QSettings setting;
    setting.beginGroup("themecolor_config");
    int nThemeType = setting.value("themecolor_config", 0).toInt();
    setting.endGroup();

    emit sigThemeChanged(nThemeType);
}
