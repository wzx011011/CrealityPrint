#ifndef _MODELGROUPINPUTPROXY_1594551275994_H
#define _MODELGROUPINPUTPROXY_1594551275994_H
#include "basickernelexport.h"
#include "us/modelgroupinput.h"

namespace creative_kernel
{
	class ModelGroup;
}

class BASIC_KERNEL_API ModelGroupInputProxy : public us::ModelGroupInput
{
	Q_OBJECT
public:
	ModelGroupInputProxy(QObject* parent = nullptr);
	virtual ~ModelGroupInputProxy();

	void setGroup(creative_kernel::ModelGroup* group);
	us::ModelListSetting* modelListSetting() override;
	virtual const us::ModelListSetting* modelListSetting() const;
protected:
	creative_kernel::ModelGroup* m_group;
};

#endif // _MODELGROUPINPUTPROXY_1594551275994_H