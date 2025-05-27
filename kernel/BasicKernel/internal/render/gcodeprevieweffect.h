#ifndef GCODEPREVIEWEFFECT_H
#define GCODEPREVIEWEFFECT_H

#include "qtuser3d/refactor/xeffect.h"
#include <QVector2D>

namespace creative_kernel
{
    class GCodePreviewEffect : public qtuser_3d::XEffect
    {
        Q_OBJECT
    public:
        enum RenderType
        {
            PolyColor = 0,
            MonoColor
        };

        GCodePreviewEffect(RenderType type, Qt3DCore::QNode* parent = nullptr);
        ~GCodePreviewEffect();

        void setColor(QVector4D color);
        void setStepLimit(QVector2D limit);
        void setLayerStepRange(QVector2D range);
        void setAnimation(int state);
        void setGCodeVisualType(int type);
        void setNozzleColorList(const QVariantList& list);

        /* ExtrusionRole */
        void setRoleEnabled(int role, bool enabled);
        void setPerimeterEnabled(bool enabled);
        void setExternalPerimeterEnabled(bool enabled);
        void setInternalInfillEnabled(bool enabled);
        void setSolidInfillEnabled(bool enabled);
        void setTopSolidInfillEnabled(bool enabled);
        void setBottomSurfaceEnabled(bool enabled);
        void setBridgeInfillEnabled(bool enabled);
        void applyExtrusionRole();



    private:
        void initState();


    private:
        int m_showType{ -1};

        QList<float> m_extrusionRoles;

        QVariantList m_structurePalette;
        QVariantList m_extruderPalette;
        QVariantList m_gradientPalette;
        
    };
}

#endif // GCODEPREVIEWEFFECT_H