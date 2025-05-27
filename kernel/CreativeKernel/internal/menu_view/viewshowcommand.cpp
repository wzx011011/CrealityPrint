#include "viewshowcommand.h"
#include "kernel/translator.h"
#include "../menu/menuCmdDataType.h"
#include "kernel/kernel.h"
#include "qtuser3d/camera/cameracontroller.h"
#include <QDebug>
ViewShowCommand::ViewShowCommand(QObject* parent)
    :ActionCommand(parent)
{
    m_actionname = "ModelShow";
    m_actionNameWithout = "ModelShow";
    m_eParentMenu = eMenuType_View;
}
using namespace creative_kernel;
ViewShowCommand::ViewShowCommand(EViewShow eType, QObject* parent)
    :ActionCommand(parent)
{
    m_nShowType = eType;
    m_eParentMenu = eMenuType_View;
    switch (eType)
    {
        case eFrontViewShow:
           m_actionname = tr("Front View") ;
        break;
        case eBackViewShow:
            m_actionname = tr("Back View");
        break;
        case eLeftViewShow:
          m_actionname = tr("Left View") ;
        break;
        case eRightViewShow:
          m_actionname = tr("Right View") ;
        break;
        case eTopViewShow:
           m_actionname = tr("Top View") ;
        break;
        case eBottomViewShow:
          m_actionname = tr("Bottom View") ;
        break;
        case ePerspectiveViewShow:
          m_actionname = tr("Perspective View") ;
        break;
        case eOrthographicViewShow:
          m_actionname = tr("Orthographic View") ;
        break;
    }
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
void ViewShowCommand::slotLanguageChanged()
{
    switch (m_nShowType)
    {
        case eFrontViewShow:
           m_actionname = tr("Front View") ;
        break;
        case eBackViewShow:
            m_actionname = tr("Back View");
        break;
        case eLeftViewShow:
          m_actionname = tr("Left View") ;
        break;
        case eRightViewShow:
          m_actionname = tr("Right View") ;
        break;
        case eTopViewShow:
           m_actionname = tr("Top View") ;
        break;
        case eBottomViewShow:
          m_actionname = tr("Bottom View") ;
        break;
        case ePerspectiveViewShow:
          m_actionname = tr("Perspective View") ;    //透视
        break;
        case eOrthographicViewShow:
          m_actionname = tr("Orthographic View") ;   //正交
        break;
    }
}
ViewShowCommand::~ViewShowCommand()
{
}

void ViewShowCommand::execute()
{
//    using namespace qtuser_3d;
    CameraController *obj = getKernel()->cameraController();

    switch (m_nShowType)
    {
        case eFrontViewShow:
            obj->viewFromFront();
        break;
        case eBackViewShow:
            obj->viewFromBack();
        break;
        case eLeftViewShow:
            obj->viewFromLeft();
        break;
        case eRightViewShow:
            obj->viewFromRight();
        break;
        case eTopViewShow:
            obj->viewFromTop();
        break;
        case eBottomViewShow:
            obj->viewFromBottom();
        break;
        case ePerspectiveViewShow:
            obj->viewFromPerspective();
        break;
        case eOrthographicViewShow:
            obj->viewFromOrthographic();
        break;
    }
}

void ViewShowCommand::showModelFace()
{

}

void ViewShowCommand::showModelLine()
{

}

void ViewShowCommand::showModelFaceLine()
{

}
