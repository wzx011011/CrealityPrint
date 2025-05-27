#include "gcodeprevieweffect.h"
#include "qtuser3d/refactor/xrenderpass.h"
#include <QVector4D>
#include "interface/appsettinginterface.h"
#include <cmath>
namespace creative_kernel
{
    GCodePreviewEffect::GCodePreviewEffect(RenderType type, Qt3DCore::QNode* parent) : qtuser_3d::XEffect(parent)
    {
        qtuser_3d::XRenderPass* pass;
        if (type == MonoColor)
        {
            pass = new qtuser_3d::XRenderPass("gcodemonoc", this);
            pass->setLineWidth(2);
        }
        else
            pass = new qtuser_3d::XRenderPass("gcodepolyc", this);

        pass->addFilterKeyMask("alpha2nd", 0);
        pass->setPassDepthTest();
        pass->setPassCullFace(Qt3DRender::QCullFace::NoCulling);
        addRenderPass(pass);

        initState();
    }

    GCodePreviewEffect::~GCodePreviewEffect()
    {
        // Destructor implementation
    }

    /* mono */
    void GCodePreviewEffect::setColor(QVector4D color)
    {
        setParameter("color", color);
    }

    void GCodePreviewEffect::setStepLimit(QVector2D limit)
    {
        setParameter("clipValue", limit);
    }

    void GCodePreviewEffect::setLayerStepRange(QVector2D range)
    {
        setParameter("layerStepRange", range);
    }

    void GCodePreviewEffect::setAnimation(int state)
    {
        setParameter("animation", state);
    }

#define APPLY_PALETTE(palette) \
        QVariantList variants;  \
        for (int i = 0, count = palette.size(), maxCount = m_gradientPalette.size(); i < maxCount && i < count; ++i)  \
        {   \
            if (i < count) variants << palette[i]; \
            else  variants << QVector4D(0, 0, 0, 1.0); \
        } \
        setParameter("palette[0]", variants);

    void GCodePreviewEffect::setGCodeVisualType(int type)
    {
        if (m_showType == type)
            return;

        m_showType = type;
        if (m_showType == 1)
        {   // structure
            APPLY_PALETTE(m_structurePalette)
        }
        else if (m_showType == 2)
        {   // extruder
            APPLY_PALETTE(m_extruderPalette)
        }
        else
        {   // gradient
            APPLY_PALETTE(m_gradientPalette)
        }
    }

    void GCodePreviewEffect::setNozzleColorList(const QVariantList& list)
    {
        QList < QColor > colors;
        QVariantList palette;
        for (const QVariant& var : list)
        {   /* mix color */
            QColor qcolor = var.value<QColor>();
            QVector4D color(qcolor.red(), qcolor.green(), qcolor.blue(), 255.0);
            float bn = color[0] * 0.3 + color[1] * 0.59 + color[2] * 0.11;
            float gainFactor = pow((255 - bn * 0.14), 2) / 255.0;	// 根据颜色亮度添加增益，避免暗色系太暗，亮色系太亮。(b - bn * k)中b越大, 暗色系越亮, k越大, 亮色系越暗
            QVector4D gain(gainFactor, gainFactor, gainFactor, 255);
            color = color * (1 - 0.17) + gain * 0.17;
            colors << QColor(color[0], color[1], color[2]);
            palette << QColor(color[0], color[1], color[2]);
        }

        m_extruderPalette = palette;
        if (m_showType == 2)
        {
            APPLY_PALETTE(m_extruderPalette)
        }
    }

    void GCodePreviewEffect::setRoleEnabled(int role, bool enabled)
    {
        m_extrusionRoles[role] = (float)enabled;
    }

    void GCodePreviewEffect::setPerimeterEnabled(bool enabled)
    {//     erPerimeter,1 内壁
        m_extrusionRoles[1] = (float)enabled;
    }

    void GCodePreviewEffect::setExternalPerimeterEnabled(bool enabled)
    {//     erExternalPerimeter,2 外壁
        m_extrusionRoles[2] = (float)enabled;
    }

    void GCodePreviewEffect::setInternalInfillEnabled(bool enabled)
    {//     erInternalInfill,4 稀疏填充
        m_extrusionRoles[4] = (float)enabled;
    }

    void GCodePreviewEffect::setSolidInfillEnabled(bool enabled)
    {//     erSolidInfill,5  内部实心填充
        m_extrusionRoles[5] = (float)enabled;
    }

    void GCodePreviewEffect::setTopSolidInfillEnabled(bool enabled)
    {//     erTopSolidInfill,6 顶部
        m_extrusionRoles[6] = (float)enabled;
    }

    void GCodePreviewEffect::setBottomSurfaceEnabled(bool enabled)
    {//     erBottomSurface,7 底部
        m_extrusionRoles[7] = (float)enabled;
    }

    void GCodePreviewEffect::setBridgeInfillEnabled(bool enabled)
    {//     erInternalBridgeInfill,10 内部搭桥
        m_extrusionRoles[10] = (float)enabled;
    }

    void GCodePreviewEffect::applyExtrusionRole()
    {
        QVariantList tempExtrusionRoles;
        for (float role : m_extrusionRoles)
            tempExtrusionRoles << role;
        setParameter("roleState[0]", tempExtrusionRoles);
    }

    void GCodePreviewEffect::initState()
    {
        QList<QVector4D> baseGradientColors;
        baseGradientColors
			<< QVector4D(0.0431373f, 0.172549f, 0.470588f, 1.0f)
			<< QVector4D(0.0745098f, 0.34902f, 0.521569f, 1.0f)
			<< QVector4D(0.109804f, 0.533333f, 0.568627f, 1.0f)
			<< QVector4D(0.0588235f, 0.839216f, 0.0588235f, 1.0f)
			<< QVector4D(0.666667f, 0.94902f, 0.0f, 1.0f)
			<< QVector4D(0.988235f, 0.976471f, 0.117647f, 1.0f)
			<< QVector4D(0.960784f, 0.807843f, 0.0392157f, 1.0f)
			<< QVector4D(0.890196f, 0.533333f, 0.12549f, 1.0f)
			<< QVector4D(0.819608f, 0.407843f, 0.188235f, 1.0f)
			<< QVector4D(0.760784f, 0.321569f, 0.235294f, 1.0f)
			<< QVector4D(0.580392f, 0.14902f, 0.0862745f, 1.0f);

        m_gradientPalette << baseGradientColors[0];
        int splitNum = 10;
        for (int i = 1; i < baseGradientColors.size(); i++)
        {
            QVector4D lastColor = baseGradientColors[i - 1];
            QVector4D currColor = baseGradientColors[i];
            QVector4D dcolor = currColor - lastColor;
            QVector4D step = dcolor / splitNum;

            for (int j = 1; j <= splitNum; j++)
            {
               m_gradientPalette << lastColor + step * j;
            }
            // for (int j = 1; j <= splitNum; j++)
            // {
            //     m_gradientPalette << currColor;
            // }
        }

		m_structurePalette = CONFIG_PLUGIN_VARLIST(gcodeeffect_typecolors, slice_group);
		m_extruderPalette = CONFIG_PLUGIN_VARLIST(gcodeeffect_nozzlecolors, slice_group);
        setGCodeVisualType(1);

        setAnimation(0);

        m_extrusionRoles << 1.0 << 1.0 << 1.0 << 1.0 << 1.0 << 
                            1.0 << 1.0 << 1.0 << 1.0 << 1.0 << 
                            1.0 << 1.0 << 1.0 << 1.0 << 1.0 << 
                            1.0 << 1.0 << 1.0 << 1.0 << 1.0;
        applyExtrusionRole();
    }
}


