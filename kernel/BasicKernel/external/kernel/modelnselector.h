#ifndef CREATIVE_KERNEL_MODELNSELECTOR_1595659762117_H
#define CREATIVE_KERNEL_MODELNSELECTOR_1595659762117_H
#include "basickernelexport.h"

namespace qtuser_3d
{
	class FacePicker;
	class Pickable;
	class SelectorTracer;
}

namespace creative_kernel
{
	class ModelN;
	class BASIC_KERNEL_API ModelNSelector : public QObject
	{
		Q_OBJECT
	public:
		ModelNSelector(QObject* parent = nullptr);
		virtual ~ModelNSelector();

		void setPickerSource(qtuser_3d::FacePicker* picker);
		void disableReverseSelect(bool disable);
		void enableSupportMode(bool able);
		void addPickable(qtuser_3d::Pickable* pickable, bool request = true);
		void removePickable(qtuser_3d::Pickable* pickable, bool request = true);
		void addTracer(qtuser_3d::SelectorTracer* tracer);
		void removeTracer(qtuser_3d::SelectorTracer* tracer);
		QList<qtuser_3d::Pickable*> selectionmPickables();

		qtuser_3d::Pickable* check(const QPoint& p, int* primitiveID = nullptr);
		ModelN* hoverCandidate();

		void clearHover();
		bool hover(const QPoint& p);
		void select(const QPoint& p);
		void select(qtuser_3d::Pickable* pickable);
		void appendSelect(qtuser_3d::Pickable* pickable);
		void selectGroup(qtuser_3d::Pickable* pickable);
		void updateFaceBases();
		
		void select(QList<qtuser_3d::Pickable*>& onList, QList<qtuser_3d::Pickable*>& offList);
		void selectAll();
		void setSelectStatus(bool bStatus);

		QList<ModelN*> selectionms();
		void changed(qtuser_3d::Pickable* pickable);
	private:
		void _add(qtuser_3d::Pickable* pickable);
		void _remove(qtuser_3d::Pickable* pickable);

		void notifyTracers();
	protected:
		qtuser_3d::FacePicker* m_pickSource;
		QList<qtuser_3d::Pickable*> m_pickables;

		qtuser_3d::Pickable* m_hoverPickable;
		qtuser_3d::Pickable* m_hoverCandidate;

		QList<qtuser_3d::Pickable*> m_selectedPickables;
		QList<qtuser_3d::SelectorTracer*> m_selectorTracers;

		int m_currentFaceBase;
		bool m_selectStatus;
		bool selectNotifying;

		bool m_disableReverseSelect;
	private:
		bool m_supportMode;
	};
}
#endif // CREATIVE_KERNEL_MODELNSELECTOR_1595659762117_H