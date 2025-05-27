#include "modelgroupinputproxy.h"
#include "data/modelgroup.h"

ModelGroupInputProxy::ModelGroupInputProxy(QObject* parent)
	:ModelGroupInput(parent)
	, m_group(nullptr)
{

}

ModelGroupInputProxy::~ModelGroupInputProxy()
{

}

void ModelGroupInputProxy::setGroup(creative_kernel::ModelGroup* group)
{
	m_group = group;
}

us::ModelListSetting* ModelGroupInputProxy::modelListSetting()
{
	return m_group->setting();
}

const us::ModelListSetting* ModelGroupInputProxy::modelListSetting() const
{
	return m_group->setting();
}