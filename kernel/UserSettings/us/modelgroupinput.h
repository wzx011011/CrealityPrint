#ifndef _US_MODELGROUPINPUT_1589340410147_H
#define _US_MODELGROUPINPUT_1589340410147_H
#include "usersettingsexport.h"
#include "us/modelinput.h"
#include "us/modellistsetting.h"

namespace us
{
	class USER_SETTINGS_API ModelGroupInput: public QObject
	{
	public:
		ModelGroupInput(QObject* parent = nullptr);
		virtual ~ModelGroupInput();

		virtual ModelListSetting* modelListSetting();
		virtual const ModelListSetting* modelListSetting() const;

		void addModelInput(ModelInput* modelInput);
		const QList<ModelInput*>& modelInputs() const;
	protected:
		QList<ModelInput*> m_modelInputs;
	};
}
#endif // _US_MODELGROUPINPUT_1589340410147_H
