#ifndef _US_MANAGERSETTING_1589868862633_H
#define _US_MANAGERSETTING_1589868862633_H
#include "usersettingsexport.h"
#include "us/usettings.h"
#include "us/slicegradesetting.h"
#include "us/machinesetting.h"
#include "us/machinetypesetting.h"

namespace us
{
	class USER_SETTINGS_API managersetting: public QObject
	{
	public:
		managersetting(QObject* parent = nullptr);
		virtual ~managersetting();
		const QHash<QString, USetting*>& initMachineSetting(const QString& machineType);
		const QHash<QString, USetting*>& initSliceGradeSetting(const QString& Grade);

	protected:
		SliceGradeSetting*  m_SliceGradeSetting;
		MachineSetting*     m_MachineSetting;
		MachineTypeSetting* m_MachineTypeSetting;

	};
}
#endif // _US_MANAGERSETTING_1589868862633_H
