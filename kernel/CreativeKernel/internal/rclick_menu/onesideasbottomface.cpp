#include "onesideasbottomface.h"

#include <QVector3D>

#include <QSettings>
#include "interface/loadinterface.h"
#include "interface/selectorinterface.h"
#include "interface/modelinterface.h"
#include "interface/visualsceneinterface.h"
#include "interface/commandoperation.h"

#include "kernel/kernelui.h"
#include "utils/meshloaderwrapper.h"
#include "kernel/translator.h"
#include "rclickmenuinterface.h"

#include "data/modeln.h"
#include "qtuser3d/math/angles.h"

using namespace creative_kernel;
using namespace qtuser_3d;

OneSideAsBottomFace::OneSideAsBottomFace()
{
    m_actionname = tr("OneSide As Bottomface");
    m_actionNameWithout = "OneSide As Bottomface";
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
OneSideAsBottomFace::~OneSideAsBottomFace()
{}
void OneSideAsBottomFace:: execute()
{
    //cmdOneSideAsBottomFace(m_point);
    QObject* lefttoolbar = AbstractKernelUI::getSelf()->getUI("lefttoolbar");
    QMetaObject::invokeMethod(lefttoolbar, "pButtomBtnClick");

}

bool OneSideAsBottomFace::enabled()
{
    return selectionms().size() > 0;
}

void OneSideAsBottomFace::slotLanguageChanged()
{
    m_actionname = tr("OneSide As Bottomface");
}
