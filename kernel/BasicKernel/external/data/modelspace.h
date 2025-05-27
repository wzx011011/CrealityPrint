#ifndef creative_kernel_MODEL_SPACE_H
#define creative_kernel_MODEL_SPACE_H
#include "basickernelexport.h"
#include <QtCore/QList>

#include "data/modelgroup.h"
#include <Qt3DCore/QEntity>
#include "modelspaceobserver.h"

namespace creative_kernel
{
	class BASIC_KERNEL_API ModelSpace : public QObject
	{
		friend class ModelSpaceUndoHandler;
		Q_OBJECT
	public:
		ModelSpace(QObject* parent = nullptr);
		~ModelSpace();

		void uninitialize();

		void addSpaceTracer(SpaceTracer* tracer);
		void removeSpaceTracer(SpaceTracer* tracer);

		qtuser_3d::Box3D baseBoundingBox();
		qtuser_3d::Box3D sceneBoundingBox();
		void setBaseBoundingBox(const qtuser_3d::Box3D& box);

		void onModelSceneChanged();

		void addModelGroup(ModelGroup* modelGroup);  // add a model group
		void removeModelGroup(ModelGroup* modelGroup);  // remove a model group

		int modelNumber();

		QList<ModelN*> modelns();

		Qt3DCore::QEntity* rootEntity();
		ModelGroup* currentGroup();
		QList<ModelGroup*> modelGroups();

		void setSpaceDirty(bool spaceDirty);
		bool spaceDirty();
	signals:
		void signalVisualChanged(bool capture);
	public:
		void checkCollide();
		int checkModelRange();
		int checkBedRange();
	private:
		qtuser_3d::Box3D m_baseBoundingBox;

		Qt3DCore::QEntity* m_root;
		ModelGroup* m_currentModelGroup;

		QList<SpaceTracer*> m_spaceTracers;

		bool m_spaceDirty;
	};
}
#endif // creative_kernel_MODEL_SPACE_H