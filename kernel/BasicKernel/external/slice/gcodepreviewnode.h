#ifndef GCODEPREVIEWNODE_H
#define GCODEPREVIEWNODE_H

#include "qtuser3d/refactor/xentity.h"
#include <QVector2D>
#include "sliceattain.h"

namespace creative_kernel
{
    class GCodePreviewNode : public Qt3DCore::QEntity
    {
        Q_OBJECT
    public:
        GCodePreviewNode(Qt3DCore::QNode* parent = nullptr);
        ~GCodePreviewNode();

        void setEffect(qtuser_3d::XEffect* polyEffect, qtuser_3d::XEffect* monoEffect);
        void setPose(const QMatrix4x4& pose);

        Qt3DCore::QEntity* getComponent(GCodeComponentType type);
        void setGeometries(GCodeComponentType type, const QList<Qt3DRender::QGeometry*>& geometries);

        void setComponentEnabled(GCodeComponentType type, bool enabled);

    private:
        qtuser_3d::XEffect* m_polyEffect {NULL};
        qtuser_3d::XEffect* m_monoEffect {NULL};
        QMatrix4x4 m_pose;

        QList<qtuser_3d::XEntity*> m_nodes;
        Qt3DCore::QEntity* m_components[(int)GCodeComponentType::Count];
        
        QList<QVector4D> m_gcodeComponentColors;
        
    };
}

#endif // GCODEPREVIEWNODE_H