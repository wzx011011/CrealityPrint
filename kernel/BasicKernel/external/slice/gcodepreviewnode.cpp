#include "gcodepreviewnode.h"
#include "qtuser3d/refactor/xrenderpass.h"
#include <QVector4D>
#include "interface/appsettinginterface.h"

namespace creative_kernel
{
    GCodePreviewNode::GCodePreviewNode(Qt3DCore::QNode* parent) : Qt3DCore::QEntity(parent)
    {
        for (int i = 0; i < (int)GCodeComponentType::Count; i++)
        {
            m_components[i] = new Qt3DCore::QEntity(this);
        }

        m_gcodeComponentColors << QVector4D(0.803922, 0.133333, 0.839216, 1.0) <<
                                    QVector4D(0.286275, 0.678431, 0.811765, 1.0) <<
                                    QVector4D(1.000000, 1.000000, 1.000000, 1.0) <<
                                    QVector4D(1, 0, 0, 1) <<
                                    QVector4D(1, 0, 0, 1) <<
                                    QVector4D(1, 0, 0, 1) <<
                                    QVector4D(1, 0, 0, 1) <<
                                    QVector4D(0.219608, 0.282353, 0.607843, 1.0) <<
                                    QVector4D(1.000000, 1.000000, 0.000000, 1.0) <<
                                    QVector4D(1, 0, 0, 1);
    }

    GCodePreviewNode::~GCodePreviewNode()
    {
        // Destructor implementation
    }
        
    void GCodePreviewNode::setEffect(qtuser_3d::XEffect* polyEffect, qtuser_3d::XEffect* monoEffect)
    {
        m_polyEffect = polyEffect;
        m_monoEffect = monoEffect;
    }

    void GCodePreviewNode::setPose(const QMatrix4x4& pose)
    {
        m_pose = pose;


    }

    Qt3DCore::QEntity* GCodePreviewNode::getComponent(GCodeComponentType type) 
    { 
        return m_components[(int)type]; 
    }

    void GCodePreviewNode::setGeometries(GCodeComponentType type, const QList<Qt3DRender::QGeometry*>& geometries)
    {
		Qt3DRender::QGeometryRenderer::PrimitiveType renderType = type == GCodeComponentType::Travel ? 
                                                                Qt3DRender::QGeometryRenderer::Lines : 
                                                                Qt3DRender::QGeometryRenderer::Triangles;
        qtuser_3d::XEffect* effect = NULL;
        QVector4D color = m_gcodeComponentColors[(int)type];
        if (type == GCodeComponentType::Extrude)
            effect = m_polyEffect;
        else if (color != QVector4D(1, 0, 0, 1))
            effect = m_monoEffect;

        for (int i = 0, c = geometries.size(); i < c; ++i)
        {
            Qt3DRender::QGeometry* geo = geometries[i];
            //Qt3DRender::QGeometryRenderer* geo = geometrys[i];
            qtuser_3d::XEntity* node = new qtuser_3d::XEntity(NULL);
            node->setParent(m_components[(int)type]);
            node->setGeometry(geo, renderType);		
            node->setEffect(effect);
            node->setParameter("color", m_gcodeComponentColors[(int)type]);
            node->setPose(m_pose);
        }
    }

    void GCodePreviewNode::setComponentEnabled(GCodeComponentType type, bool enabled)
    {
        m_components[(int)type]->setEnabled(enabled);
    }
};

