#ifndef _creative_kernel_MODELPOSITIONINITIALIZER_1589275563011_H
#define _creative_kernel_MODELPOSITIONINITIALIZER_1589275563011_H
#include "basickernelexport.h"
#include "data/modeln.h"
#include "qtusercore/module/progressor.h"

namespace creative_kernel
{
	class ModelSpace;
	class BASIC_KERNEL_API ModelPositionInitializer: public QObject
	{
	public:
		ModelPositionInitializer(QObject* parent = nullptr);
		virtual ~ModelPositionInitializer();

		static void initialize(ModelN* model);

		static void initialize(ModelN* model, QList<ModelN*> models, const qtuser_3d::Box3D& baseBox);
		static void layout(ModelN* model, qtuser_core::Progressor* progressor,bool bAdaption = true);
		static void layoutBelt(ModelN* model, qtuser_core::Progressor* progressor, bool bAdaption = true);
		static void AdaptionModel(ModelN* model);

		static bool nestLayout(ModelN* model, bool bAddModel);

		/* layout all models in the workspace using a Bottom-Left Algorithm */
		static void layout_all(QList<ModelN*>& models, qtuser_core::Progressor* progressor);

	private:
		/* first, search for an appropriate position inside the workspace, if cannot find such a position, search outside the workspace then.*/
		static void layout_by_search(int model_index, QList<ModelN*>& models, QList<int>& model_flags, float step_size);
	};
}
#endif // _creative_kernel_MODELPOSITIONINITIALIZER_1589275563011_H
