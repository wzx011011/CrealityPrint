#include "modelnselector.h"
#include "qtuser3d/module/pickable.h"
#include "qtuser3d/module/facepicker.h"
#include "qtuser3d/module/pickableselecttracer.h"

#include "data/modeln.h"
using namespace qtuser_3d;
namespace creative_kernel
{
	ModelNSelector::ModelNSelector(QObject* parent)
		:QObject(parent)
		, m_pickSource(nullptr)
		, m_hoverPickable(nullptr)
		, m_hoverCandidate(nullptr)
		, m_currentFaceBase(0)
		, selectNotifying(false)
		, m_disableReverseSelect(false)
		, m_supportMode(false)
	{
		m_selectStatus = false;
	}
	
	ModelNSelector::~ModelNSelector()
	{
	}

	void ModelNSelector::setPickerSource(FacePicker* picker)
	{
		m_pickSource = picker;
	}

	void ModelNSelector::disableReverseSelect(bool disable)
	{
		m_disableReverseSelect = disable;
	}

	void ModelNSelector::enableSupportMode(bool able)
	{
		m_supportMode = able;
	}

	QList<ModelN*> ModelNSelector::selectionms()
	{
		QList<ModelN*> models;
		for (Pickable* pickable : m_selectedPickables)
		{
			ModelN* m = qobject_cast<ModelN*>(pickable);
			if (m) models << m;
		}
		return models;
	}

	void ModelNSelector::changed(qtuser_3d::Pickable* pickable)
	{
		if (pickable && pickable->selected())
		{
			for (SelectorTracer* tracer : m_selectorTracers)
			{
				tracer->selectChanged(pickable);
			}
		}
	}

	void ModelNSelector::addPickable(Pickable* pickable, bool request)
	{
		if (pickable)
		{
			_add(pickable);

			if (m_disableReverseSelect && pickable->enableSelect())
			{
				select(pickable);
			}

			if (m_pickSource && request)
				m_pickSource->sourceMayChanged();
		}
	}

	void ModelNSelector::removePickable(Pickable* pickable, bool request)
	{
		if (pickable)
		{
			_remove(pickable);

			pickable->setState(ControlState::none);
			if (m_hoverPickable && (m_hoverPickable == pickable))
			{
				clearHover();
			}

			m_selectedPickables.removeOne(pickable);
			if (pickable->enableSelect())
				notifyTracers();

			if (m_pickSource && request)
				m_pickSource->sourceMayChanged();
		}
	}

	void ModelNSelector::addTracer(SelectorTracer* tracer)
	{
		if (tracer) m_selectorTracers.push_back(tracer);
	}

	void ModelNSelector::removeTracer(SelectorTracer* tracer)
	{
		if (tracer) m_selectorTracers.removeOne(tracer);
	}

	QList<qtuser_3d::Pickable*> ModelNSelector::selectionmPickables()
	{
		return m_selectedPickables;
	}

	Pickable* ModelNSelector::check(const QPoint& p, int* primitiveID)
	{
		Pickable* pickable = checkPickableFromList(m_pickSource, p, m_pickables, primitiveID);

		return pickable;
	}

	ModelN* ModelNSelector::hoverCandidate()
	{
		qDebug() << "hover candidate = " << m_hoverCandidate;
		if (m_hoverCandidate != nullptr)
		{
			QString qname = m_hoverCandidate->metaObject()->className();
			if (qname.indexOf( "ModelN") == -1)
			{
				return nullptr;
			}
		}
		return (ModelN*)m_hoverCandidate;
	}

	void ModelNSelector::_add(Pickable* pickable)
	{
		m_pickables.push_back(pickable);

		bool noPrimitive = pickable->noPrimitive();
		int primitiveNum = pickable->primitiveNum();
		if (noPrimitive) primitiveNum = 1;

		pickable->setFaceBase(m_currentFaceBase);
		m_currentFaceBase += primitiveNum;
	}

	void ModelNSelector::_remove(Pickable* pickable)
	{
		//rebase 
		bool noPrimitive = pickable->noPrimitive();
		int primitiveNum = pickable->primitiveNum();
		if (noPrimitive) primitiveNum = 1;

		int offset = primitiveNum;

		int size = m_pickables.size();
		int index = m_pickables.indexOf(pickable);
		if (index >= 0)
		{
			for (int i = index + 1; i < size; ++i)
			{
				Pickable* p = m_pickables.at(i);
				int fBase = p->faceBase();
				fBase -= offset;
				p->setFaceBase(fBase);
			}

			m_pickables.removeAt(index);
			m_currentFaceBase -= offset;
		}
	}

	void ModelNSelector::updateFaceBases()
	{
		m_currentFaceBase = 0;

		for (qtuser_3d::Pickable* pickable : m_pickables)
		{
			bool noPrimitive = pickable->noPrimitive();
			int primitiveNum = pickable->primitiveNum();
			if (noPrimitive) primitiveNum = 1;

			pickable->setFaceBase(m_currentFaceBase);
			m_currentFaceBase += primitiveNum;
		}
	}

	void ModelNSelector::clearHover()
	{
		if (m_hoverPickable)
		{
			m_hoverPickable->setState(qtuser_3d::ControlState::none);
			m_hoverPickable = nullptr;
		}
	}

	bool ModelNSelector::hover(const QPoint& p)
	{
		Pickable* hoverEntity = nullptr;
		int primitiveID = -1;
		m_hoverCandidate = check(p, &primitiveID);

		if (m_hoverCandidate)
		{
			if (m_hoverCandidate->isGroup())
			{
				hoverEntity = m_hoverCandidate;
			}
			else
			{
				if (m_hoverCandidate && m_hoverCandidate->state() != qtuser_3d::ControlState::selected)
				{
					hoverEntity = m_hoverCandidate;
				}
			}
		}

		if (m_hoverPickable == hoverEntity)
		{
			if (hoverEntity && hoverEntity->isGroup())
			{
				bool changed = hoverEntity->hoverPrimitive(primitiveID);
				return changed;
			}
			
			return false;
		}
		if (m_hoverPickable)
		{
			if (m_hoverPickable->isGroup())
			{
				m_hoverPickable->unHoverPrimitive();
				m_hoverPickable = nullptr;
			}
			else if (m_hoverPickable->state() != ControlState::selected)
			{
				m_hoverPickable->setState(ControlState::none);
				m_hoverPickable = nullptr;
			}
		}

		m_hoverPickable = hoverEntity;

		if (m_hoverPickable && !m_hoverPickable->isGroup())
		{
			m_hoverPickable->setState(ControlState::hover);
		}
		return true;
	}

	void ModelNSelector::select(const QPoint& p)
	{
		if (selectNotifying)
			return;

		Pickable* pickable = check(p, nullptr);

		if (pickable)
		{
			ModelN* model = qobject_cast<ModelN*>(pickable);
			if (!model)
				return;
		}

		if (m_selectStatus) selectGroup(pickable);
		else select(pickable);
	}
	
	void ModelNSelector::selectGroup(qtuser_3d::Pickable* pickable)
	{
		QList<Pickable*> offList;
		QList<Pickable*> onList= m_selectedPickables;
		for (size_t i = 0; i < onList.size(); i++)
		{
			if (pickable == onList[i])
			{				
				offList << onList[i];//by TCJ -ctrl uncheck
				onList.removeAt(i);				
				select(onList, offList);
				return;
			}
		}
		if (pickable && pickable->enableSelect()) onList << pickable;
		select(onList, offList);
	}
	
	
	void ModelNSelector::select(Pickable* pickable)
	{
		if (selectNotifying)
			return;

		QList<Pickable*> offList = m_selectedPickables;
		QList<Pickable*> onList;

		if (m_disableReverseSelect && !pickable)
		{
			if (m_supportMode) {
				return;
			}
			select(onList, offList);//by TCJ -Click blank cancel select
			return;
		}

		if (pickable && pickable->enableSelect()) onList << pickable;
		select(onList, offList);
	}

	void ModelNSelector::appendSelect(qtuser_3d::Pickable* pickable)
	{
		if (selectNotifying)
			return;

		if (pickable && !pickable->selected() && pickable->enableSelect())
		{
			pickable->setSelected(true);
			m_selectedPickables << pickable;
			notifyTracers();
		}
	}

	void ModelNSelector::selectAll()
	{
		if (selectNotifying)
			return;

		QList<Pickable*> onLists;
		QList<Pickable*> offlist;
		for (Pickable* pickable : m_pickables)
		{
			onLists << pickable;
		}
		select(onLists, offlist);
	}


	void ModelNSelector::setSelectStatus(bool bStatus)
	{
		m_selectStatus = bStatus;
	}
	void ModelNSelector::select(QList<Pickable*>& onList, QList<Pickable*>& offList)
	{
		if (selectNotifying)
			return;

		if ((onList.size() == 0) && (offList.size() == 0))
			return;

		for (Pickable* pickable : offList)
		{
			pickable->setSelected(false);
		}

		for (Pickable* pickable : onList)
		{
			pickable->setSelected(true);
		}

		m_selectedPickables = onList;
		notifyTracers();
	}

	void ModelNSelector::notifyTracers()
	{
		selectNotifying = true;
			
		for (SelectorTracer* tracer : m_selectorTracers)
		{
			tracer->onSelectionsChanged();
		}

		selectNotifying = false;
	}
}