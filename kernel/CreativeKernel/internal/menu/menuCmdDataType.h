#ifndef MENUCMDDATATYPE_H
#define MENUCMDDATATYPE_H

#include "menu/menuShowType.h"

enum EMirrorCommand
{
    eMirror_X = 0,
    eMirror_Y,
    eMirror_Z,
    eMirroe_Reset

};

enum EViewShow
{
    eFrontViewShow =0,
    eBackViewShow,
    eLeftViewShow,
    eRightViewShow,
    eTopViewShow,
    eBottomViewShow,
    ePerspectiveViewShow,
    eOrthographicViewShow
};

#endif // MENUCMDDATATYPE_H
