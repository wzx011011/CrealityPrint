#include "builtinui.h"

#include "internal/tool/clearcommand.h"
#include "internal/tool/clonecommand.h"
#include "internal/tool/supportcommand.h"
#include "internal/tool/layouttoolcommand.h"
#include "internal/tool/mirrorcommand.h"
//#include "internal/tool/modelshowcommand.h"
#include "internal/tool/meshimportcommand.h"
//#include "internal/tool/undocommand.h"
//#include "internal/tool/redocommand.h"
//#include "internal/tool/websitecommand.h"
//#include "internal/tool/settingscommand.h"
#include "internal/tool/pickmode.h"
#include "internal/tool/translatemode.h"
#include "internal/tool/rotatemode.h"
#include "internal/tool/scalemode.h"
//#include "internal/tool/resetcommand.h"

#include "qtusercore/module/systemutil.h"


#include "qtuserqml/plugin/toolcommandcenter.h"
#include "kernel/kernelui.h"

void registerBuiltin()
{
    qInfo() << "register UI";
    qtuser_qml::ToolCommandCenter* left = getKernelUI()->lCenter();
    if(left)
    {
        PickMode* pickMode = new PickMode(left);
        left->addCommand(pickMode, 0);

        TranslateMode* translateMode = new TranslateMode(left);
        left->addCommand(translateMode, 1);

        ScaleMode* scaleMode = new ScaleMode(left);
        left->addCommand(scaleMode, 2);

        RotateMode* rotateMode = new RotateMode(left);
        left->addCommand(rotateMode, 3);

        CloneCommand* cloneCommand = new CloneCommand(left);
        left->addCommand(cloneCommand, 4);

        LayoutToolCommand* layoutCommand = new LayoutToolCommand(left);
        left->addCommand(layoutCommand, 8);

        SupportCommand* supportCommand = new SupportCommand(left);
        left->addCommand(supportCommand, 9);
    }

    qInfo() << "register UI over";
    showSysMemory();
}

